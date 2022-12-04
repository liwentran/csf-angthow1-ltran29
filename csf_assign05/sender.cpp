#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];

  Connection conn;
  // connect to server
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    std::cerr << "Failed to connect to server";
    return 1;
  }

  // TODO: send slogin message
    conn.send(Message(TAG_SLOGIN, username));

    Message slogin_response = Message();
    conn.receive(slogin_response);

  if (slogin_response.tag == TAG_ERR) {
    std::cerr << slogin_response.data;
    conn.close();
    return 1;
  }
  // loop reading commands from user, sending messages to
  //       server as appropriate
  while(1){

    Message message;
    std::string input;
    std::getline(std::cin, input);

    if(input.substr(0, 6) == "/join "){
      message.tag = TAG_JOIN;
      message.data = input.substr(6);
    }
    else if(input == "/leave"){
      message.tag = TAG_LEAVE;
    }
    else if(input == "/quit"){
      message.tag = TAG_QUIT;
      conn.send(message);
      Message quit_response = Message();
      conn.receive(quit_response);
      if (quit_response.tag == TAG_ERR || conn.get_last_result() == Connection::INVALID_MSG) {
        std::cerr << quit_response.data;
      }
      break;
    }
    else{
      message.tag = TAG_SENDALL;
      message.data = input;
    }
      conn.send(message);
      Message message_response = Message();
      conn.receive(message_response);
      if (message_response.tag == TAG_ERR || conn.get_last_result() == Connection::INVALID_MSG) {
        std::cerr << message_response.data;
        continue;
      }
  }
  conn.close();
  return 0;
}
