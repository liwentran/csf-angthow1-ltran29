#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include <iostream>
#include "user.h"
#include "room.h"

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  // TODO: initialize the mutex
  pthread_mutex_init(&lock, nullptr);

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
  std::cout << "DEBUGGIN: In broadcast message" << std::endl;
  for(auto user: members){
   if(user->username != sender_username) {
      std::cout << "DEBUGGIN: Looping and sending messages...." << std::endl;
      std::string data = this->get_room_name() + ":" + sender_username + ":" + message_text;
      (*user).mqueue.enqueue(new Message(TAG_DELIVERY, data));
    }

  }
}

