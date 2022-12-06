#include <cassert>
#include <ctime>
#include "message_queue.h"

MessageQueue::MessageQueue() {
  // initialize the mutex and the semaphore
  unsigned max_items = 25; // TODO: Find out how many max items
	sem_init(&m_avail, 0, max_items);
	pthread_mutex_init(&m_lock, NULL);
}

MessageQueue::~MessageQueue() {
  // destroy the mutex and the semaphore
  pthread_mutex_destroy(&m_lock);
  sem_destroy(&m_avail);
}

void MessageQueue::enqueue(Message *msg) {
  // wait for empty slot
  sem_wait(&m_avail);
  // put the specified message on the queue
  pthread_mutex_lock(&m_lock);
  m_messages.push_back(msg);
  pthread_mutex_unlock(&m_lock);
  // be sure to notify any thread waiting for a message to be
  // available by calling sem_post
  sem_post(&m_avail);
}

Message *MessageQueue::dequeue() {
  struct timespec ts;

  // get the current time using clock_gettime:
  // we don't check the return value because the only reason
  // this call would fail is if we specify a clock that doesn't
  // exist
  clock_gettime(CLOCK_REALTIME, &ts);

  // compute a time one second in the future
  ts.tv_sec += 1;

  // call sem_timedwait to wait up to 1 second for a message
  //       to be available, return nullptr if no message is available

  sem_timedwait(&m_avail, &ts);

  // remove the next message from the queue, return it
  Message *msg = nullptr;

  pthread_mutex_lock(&m_lock);

  // pop message from queue
  if (m_messages.empty()) {
    return msg;
  } else {
    msg = m_messages.front();
    m_messages.pop_front();
  }
  pthread_mutex_unlock(&m_lock);
  sem_post(&m_avail); /* empty slot is available */

  return msg;
}
