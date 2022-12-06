#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions

struct ConnectionInfo {
    // Used to pass Connection object
    // along with any userful information
    int clientfd;
    Connection* conn;
    Server* server;
};



////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {

void *worker(void *arg) {

  pthread_detach(pthread_self());

  // use a static cast to convert arg from a void* to
  //       whatever pointer type describes the object(s) needed
  //       to communicate with a client (sender or receiver)
  struct ConnectionInfo *info = (ConnectionInfo *) arg;

  // read login message (should be tagged either with
  //       TAG_SLOGIN or TAG_RLOGIN), send response
  Message login_message = Message();
  info->conn->receive(login_message);

  // create user using its usernmae
  User *user = &User(login_message.data);

  // send confirmation
  if (login_message.tag == TAG_SLOGIN || login_message.tag == TAG_RLOGIN) {
    info->conn->send(Message(TAG_OK, "logged in"));
  }

  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)
  
  // sender
  if (login_message.tag == TAG_SLOGIN) {
    chat_with_sender(info->conn);
  // receiver
  } else if (login_message.tag == TAG_RLOGIN) {
    // respond to join room
    Message join_room_message = Message();
    info->conn->receive(join_room_message);
    Room *r = info->server->find_or_create_room(join_room_message.data);
    r->add_member(user);

    info->conn->send(Message(TAG_OK, "joined room"));

    
    chat_with_receiver(info->conn);
  }
  // tear down client thread 
	close(info->clientfd);
	free(info);
	
	return NULL;
  return nullptr;
}

// communication loops for the senders
void chat_with_sender(Connection *conn){

  while (1) {
    Message message;
    conn->receive(message);

    // error message receieved
    if (message.tag == TAG_ERR) {
      std::cerr << message.data;
      return;
    }
    // respond to /join [room]
    else if (message.tag == TAG_JOIN) {
      // TODO: register sender to room
      conn->send(Message(TAG_OK, "joined room"));
    }
    // respond to /leave
    else if (message.tag == TAG_LEAVE) {
      // TODO: de-register sender form room
      
      conn->send(Message(TAG_OK, "left room"));
    
    // respond to [message text]
    } else {
      
    }
  
    
  }
  
  // For all synchronous messages, you must ensure that the server always transmits some kind of response
  // tear down the client thread if any message fails to send
}

void chat_with_receiver(Connection *conn){
  //terminate the loop and tear down the client thread if any message transmission fails or if quit message
  
}

}



////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // TODO: initialize mutex
}

Server::~Server() {
  // TODO: destroy mutex
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
  const char* port_string = std::to_string(m_port).c_str();
  m_ssock = open_listenfd(port_string);
  if (m_ssock < 0) { // error
    std::cerr << "Couldn't open server socket";
    return false;
  }
  return true;
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  

  if(listen()){
    while(1){
      int clientfd = Accept(m_ssock, NULL, NULL);
      if (clientfd < 0) { 
        std::cerr << "Error accepting client connection";
      } 

      struct ConnectionInfo *info = NULL;
      info = (ConnectionInfo *) malloc(sizeof(struct ConnectionInfo));
      info->conn = &Connection(clientfd);
      info->clientfd = clientfd;
      info->server = this;
      
      pthread_t thr_id;

      // starts a new prethread for this connected client
      if (pthread_create(&thr_id, NULL, worker, info) != 0) {
        std::cerr << "pthread_create failed";
      }
      
    }

  }

}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
}

