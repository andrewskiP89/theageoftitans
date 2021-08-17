#include "sfml.h"
#include "Object.h"

#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H


class EventManager{
private:
  std::vector<Clickable*> _clickSubscribers; // all clickable objects are subribed to click event by default
  std::vector<sf::Event> _clickQueue;

public:
  void notifyAll(const sf::Event &event);
  void registerItem(Clickable* s);
  EventManager();
  ~EventManager();
};
#endif
