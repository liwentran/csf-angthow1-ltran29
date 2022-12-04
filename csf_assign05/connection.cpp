#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include <iostream>
#include <string.h>


Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);
}

void Connection::connect(const std::string &hostname, int port) {
  // call open_clientfd to connect to the server

  // open_clientfd requires char strxgs
  const char* hostname_string = hostname.c_str();
  const char* port_string = std::to_string(port).c_str();

  m_fd = open_clientfd(hostname_string, port_string);

  // call rio_readinitb to initialize the rio_t object

  if (m_fd < 0) { // error
    std::cerr << "Open clientfd failed";
  } else { 
    rio_readinitb(&m_fdbuf, m_fd);
  }
  
}

Connection::~Connection() {
  // close the socket if it is open
  if(is_open()){
    close();
  }
}

bool Connection::is_open() const {
  // return true if the connection is open (when m_fd is not negative)
  return m_fd >= 0;
}

void Connection::close() {
  // close the connection if it is open
  if (is_open()) {
    Close(m_fd);
    m_fd = -1;
  }
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // convert Message object to char string
  const std::string message = msg.tag + ":" + msg.data + "\n";
  const char* message_string = message.c_str();

  ssize_t res = rio_writen(m_fd, message_string, message.length());

  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  if (res >= 0) {
    m_last_result = SUCCESS;
    return true;
  } else {
    m_last_result = EOF_OR_ERROR;
    return false;
  }
}

bool Connection::receive(Message &msg) {
  // TODO: receive a message, storing its tag and data in msg

  // make sure that m_last_result is set appropriately

  //create buffer to store result, ensure that we e
  char buffer[Message::MAX_LEN + 1];

  //usrbuf, maxlen
  ssize_t rc = rio_readlineb(&m_fdbuf, buffer, Message::MAX_LEN);
  if (rc < 0) { 
    m_last_result = INVALID_MSG;
    return false; // error reading data from client
  }

  // get tag and data from buffer
  char* ptr = strtok(buffer, ":"); // separate using ":"
  msg.tag = ptr;
  ptr = strtok(NULL, "");
  msg.data = ptr; 

  // return true if successful, false if not
  m_last_result = SUCCESS;
  return true;
}

