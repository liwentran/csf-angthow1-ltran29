#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  // TODO: initialize the mutex
}

Room::~Room() {
  // TODO: destroy the mutex
}

void Room::add_member(User *user) {
  // TODO: add User to the room
}

void Room::remove_member(User *user) {
  // TODO: remove User from the room
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // TODO: send a message to every (receiver) User in the room
}
