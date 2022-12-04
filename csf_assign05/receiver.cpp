#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  Connection conn;

  // connect to server
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    std::cerr << "Failed to connect to server";
    return 1;
  }

  // is connection success?

  // send rlogin and join messages (expect a response from
  //       the server for each one)

  //login
  conn.send(Message(TAG_RLOGIN, username));
  
  Message rlogin_response = Message();
  conn.receive(rlogin_response);

  if (rlogin_response.tag == TAG_ERR) {
    std::cerr << rlogin_response.data;
    return 1;
  }

  //join 
  conn.send(Message(TAG_JOIN, room_name));

  Message join_response = Message();
  conn.receive(join_response);
  
  if (join_response.tag == TAG_ERR) {
    std::cerr << join_response.data;
    return 1;
  }
  

  // loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)

  while(1) {
    Message server_msg = Message();
    conn.receive(server_msg);
    if (server_msg.tag == TAG_DELIVERY) {
      std::vector<std::string> data_vector = tokenize(server_msg.data, ":");

      std::cout << data_vector[1] << ": " << data_vector[2];
    }
  }

  conn.close();

  return 0;
}
