#ifndef CONNECTION_H
#define CONNECTION_H

#include "csapp.h"
struct Message;

class Connection {
public:
  // enumeration type describing reasons why a call to
  // send or receive failed
  enum Result {
    SUCCESS,      // send or receive was successful
    EOF_OR_ERROR, // EOF or error receiving or sending data
    INVALID_MSG,  // message format was invalid
  };

  // Default constructor: Connection starts out as not connected,
  // the connect member function must be called to create a connection.
  // This is how a client should connect to the server.
  Connection();

  // Constructor from an open file descriptor, which is assumed
  // to be a TCP socket. This is the constructor that the server
  // should use when it has accepted a connection from a client.
  Connection(int fd);

  // Destructor. Should make sure that the file descriptor is closed.
  ~Connection();

  // Connect to a server via specified hostname and port number.
  void connect(const std::string &hostname, int port);

  bool is_open() const;

  void close();

  // send and receive should set m_last_result to indicate
  // whether the most recent send or receive was successful,
  // and if not, whether the reason was an I/O error or reaching EOF,
  // or whether the format of the received message was invalid
  bool send(const Message &msg);
  bool receive(Message &msg);

  Result get_last_result() const { return m_last_result; }

private:
  // prohibit value semantics
  Connection(const Connection &);
  Connection &operator=(const Connection &);

  // these are the recommended member variables for the
  // Connection class
  int m_fd;
  rio_t m_fdbuf; // used to allow buffered input
  Result m_last_result;
};

#endif // CONNECTION_H
