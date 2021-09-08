#include "sfml.h"
#include "Object.h"

#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

class EventManager{
private:
  std::vector<Clickable*> _clickSubscribers; // all clickable objects are subribed to click event by default
  std::vector<sf::Event> _clickQueue;
public:
  static std::vector<AppEvent*> m_eventQueue;
  static std::set<std::string> m_fireEventIds;
  static void changeGameState(GameState targetGameState);
  static std::vector<AppEvent> loadEventsFromMap(EventCoordinate coordinate, std::map<std::string, std::vector<std::string>> sourceMap);
  static std::map<std::string, AppEvent> getAllEvents(uint8_t currentLevel);
  static void publishEvent(AppEvent);
  void notifyAll(const sf::Event &event);
  void registerItem(Clickable* s);
  void fireEvent(AppEvent &event);
  void clearEvents(std::vector<AppEvent> &events);
  bool pollEvent(AppEvent &event);
  EventManager();
  ~EventManager();
};


#endif
