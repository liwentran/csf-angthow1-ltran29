#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <set>
#include <pthread.h>

struct User;

// A Room object is a representation of a chat room.
// At a minimum, it should keep track of the User objects representing
// receivers who have joined the room.
class Room {
public:
  Room(const std::string &room_name);
  ~Room();

  std::string get_room_name() const { return room_name; }

  void add_member(User *user);
  void remove_member(User *user);

  void broadcast_message(const std::string &sender_username, const std::string &message_text);

private:
  std::string room_name;
  pthread_mutex_t lock;

  typedef std::set<User *> UserSet;
  UserSet members;
};

#endif // ROOM_H
