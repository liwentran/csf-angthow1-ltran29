#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
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
    ~ConnectionInfo() {
      delete conn;
    }
};



////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////
// communication loops for the senders
void chat_with_sender(Connection *conn, Server *server, User *user){
  Room *room = nullptr;
  std::cout<< "DEBUGGIN: enterd chat with sender" << std::endl;
  while (1) {
    std::cout<< "DEBUGGIN: waiting for messages" << std::endl;
      
    Message message;
    if (!conn->receive(message)) {     //error checking
        std::cout << "DEBUGGIN: could not receieve message"  <<std::endl;

        // error message receieved
      if (conn->get_last_result() == Connection::INVALID_MSG || conn->get_last_result() == Connection::EOF_OR_ERROR) {
        conn->send(Message(TAG_ERR, "Invalid message\n"));
        return;
      }
    } else{
        std::cout << "DEBUGGIN: processing message with "<< message.tag << " : " <<message.data  <<std::endl;

      if (message.tag == TAG_ERR) {
        std::cerr << message.data;
        return;
      }
      if (message.tag == TAG_QUIT) { //quit the program
        conn->send(Message(TAG_OK, "Bye\n"));
        return;
      } else if (room == nullptr) {  //cannot send message without joining a room first
        if (message.tag == TAG_JOIN) {
          std::cout << "DEBUGGIN: room is nullptr and we are trying to join room "  <<std::endl;
          room = server->find_or_create_room(message.data);
          std::cout << "DEBUGGIN: about to join the room " << room->get_room_name()  <<std::endl;

          room->add_member(user);
          std::cout << "DEBUGGIN: joined room " << room->get_room_name()  <<std::endl;

          if (!conn->send(Message(TAG_OK, "joined room\n"))) {
            return;
          }
        } else {
          conn->send(Message(TAG_ERR, "Must join room to send a message!\n"));
        }
      }
    // respond to /join [room]
    else if (message.tag == TAG_JOIN) {
      // TODO: register sender to room
      room->remove_member(user);
      room = server->find_or_create_room(message.data);
      room->add_member(user);
      //need to register user to room
      if (!conn->send(Message(TAG_OK, "joined new room\n"))) {
            return;
        }
    } else if (message.tag == TAG_SENDALL){     // respond to [message text]
      std::cout << "DEBUGGIN: broadcasting message " << message.data << std::endl;
      room->broadcast_message(user->username, message.data);
      if (!conn->send(Message(TAG_OK, "message sent to all in room\n"))) {
          return;
      }
    }
    // respond to /leave
    else if (message.tag == TAG_LEAVE) {
      // TODO: de-register sender form room
      room->remove_member(user);
      room = nullptr;
      if (!conn->send(Message(TAG_OK, "left room\n"))) {
          return;
      }    
    } 
    }
  }
  // For all synchronous messages, you must ensure that the server always transmits some kind of response
  // tear down the client thread if any message fails to send
}

void chat_with_receiver(Connection *conn, Server *server, User *user){
  //terminate the loop and tear down the client thread if any message transmission fails or if quit message
    std::cout << "DEBUGGIN: enter chat with reciever function" << std::endl;
      // respond to join room
    Message message = Message();
    Room *room = nullptr;
    if (!conn->receive(message)) {
      if (conn->get_last_result() == Connection::INVALID_MSG) {
        conn->send(Message(TAG_ERR, "invalid message"));
      }
      return;
    }
    std::cout << "DEBUGGING: chat_with_receiver:  messsage receieved " << message.tag  << " : " << message.data <<std::endl;

    if (message.tag == TAG_JOIN) {
          room = server->find_or_create_room(message.data);
          room->add_member(user);
          std::cout << "DEBUGGIN: successfully joined the room " << room->get_room_name() << std::endl;
          if (!conn->send(Message(TAG_OK, "Joined room"))) {
              return;
          }
  } else {
      conn->send(Message(TAG_ERR, "invalid message, Sender needs to join a room\n"));
  }

  //send queued messages to receiver
  while(1){ 
    std::cout << "DEBUGGIN (R): waiting for qued messages " << std::endl;
    Message *msg = user->mqueue.dequeue();
    std::cout << "DEBUGGIN (R): got message: " << msg->data << std::endl;
    if(msg != nullptr){
      if (!conn->send(*msg)) {
        break;
      }
    }
  }
  room->remove_member(user);
  return;
}

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
  std::cout << "DEBUGGING: thread has detact≈he≈d. we are stillusing clientfd " << info->clientfd << std::endl;

  if (!info->conn->receive(login_message)) {

    if (info->conn->get_last_result() == Connection::INVALID_MSG) {
      info->conn->send(Message(TAG_ERR, "invalid message"));
    }
    return nullptr;
  } 

  std::cout << "DEBUGGING: worker: message messsage receieved " << login_message.tag  << " : " << login_message.data <<std::endl;

  if (login_message.tag != TAG_SLOGIN && login_message.tag != TAG_RLOGIN) {
    info->conn->send(Message(TAG_ERR, "User needs to login first"));
    return nullptr;
  } else {
    if (!info->conn->send(Message(TAG_OK, "logged in"))) {
      return nullptr;
    }
  }


  // create user using its usernmae
  User *user = new User(login_message.data);


  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)
  
  // sender
  if (login_message.tag == TAG_SLOGIN) {
    std::cout << "DEBUGGIN: goingto chat_with_sender" <<std::endl;
    chat_with_sender(info->conn, info->server, user);
  // receiver
        //need to register user to room
  } else if (login_message.tag == TAG_RLOGIN) {
    chat_with_receiver(info->conn, info->server, user);
    
  }
  // tear down client thread 
  delete user;
  delete info;	
  return nullptr;
}
}







////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  pthread_mutex_init(&m_lock, nullptr);
}

Server::~Server() {
  pthread_mutex_destroy(&m_lock);
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
    std::cout << "DEBUGGIN: entered handle_client_request" << std::endl;

    while(1){
      int clientfd = accept(m_ssock, nullptr, nullptr);
      if (clientfd < 0) { 
        std::cerr << "Error accepting client connection";
        return;
      } 

      struct ConnectionInfo *info = new ConnectionInfo();
      info->conn = new Connection(clientfd);
      info->clientfd = clientfd;
      info->server = this;
      
      pthread_t thr_id;

      std::cout << "DEBUGGIN: accepting this connection, witch clientfd: " << clientfd << std::endl;

      // starts a new prethread for this connected client
      if (pthread_create(&thr_id, NULL, worker, info) != 0) {
        std::cerr << "pthread_create failed";
        return;
      }
      std::cout << "DEBUGGIN: finished creating pthreard with threadid" << thr_id << std::endl;

      
    }

  

}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new /one if necessary
  Guard g(m_lock);
  auto i = m_rooms.find(room_name);
  if(i != m_rooms.end()){
     std::cout << "DEBUGGIN: found room " << i->second->get_room_name() << std::endl;
    return i->second;
  }
  else{ //create new room
    Room *r = new Room(room_name);
    m_rooms[room_name] = r;
    std::cout << "DEBUGGIN: created new room with name " << r->get_room_name() << std::endl;
    return r;
  }
  
}

