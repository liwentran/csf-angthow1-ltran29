#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  // TODO: initialize the mutex
  pthread_mutex_lock(&lock);

}

Room::~Room() {
  // TODO: destroy the mutex
    pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user) {
  // add User to the room
  Guard g(lock);
  members.insert(user);
}

void Room::remove_member(User *user) {
  // remove User from the room
  Guard g(lock);
  members.erase(user);
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // send a message to every (receiver) User in the room
  Guard g(lock);
  std::set<User *>::iterator itr;

  // loop through every member
  for(itr = members.begin(); itr != members.end(); itr++) {
    std::string data = this->get_room_name() + ":" + sender_username + ":" + message_text;
    (*itr)->mqueue.enqueue(new Message(TAG_DELIVERY, data));
  }
}

