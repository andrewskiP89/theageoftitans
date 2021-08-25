#include "sfml.h"
#include "Object.h"

#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

enum GameState { Starting,
                  Playing,
                  Pause,
                  OnMenu,
                  OnDialog
                  };

enum EventType{
  CustomEvent,
  GameStateChange,
  TaskCompleted
};
struct AppEvent{
  void *source;
  GameState targetState;
  uint16_t taskCompleted;
  EventType type;
  void fire();
};
class EventManager{
private:
  std::vector<Clickable*> _clickSubscribers; // all clickable objects are subribed to click event by default
  std::vector<sf::Event> _clickQueue;
public:
  static std::vector<AppEvent*> m_eventQueue;
  static void publishEvent(AppEvent);
  void notifyAll(const sf::Event &event);
  void registerItem(Clickable* s);
  bool pollEvent(AppEvent &event){
    if(m_eventQueue.size() > 0){
      event = * (m_eventQueue.back());
      m_eventQueue.pop_back();
      return true;
    }
    return false;
  }
  EventManager();
  ~EventManager();
};

std::vector<AppEvent*> EventManager::m_eventQueue; // definition
void AppEvent::fire(){
  EventManager::m_eventQueue.insert(EventManager::m_eventQueue.begin(), this);
}
#endif
