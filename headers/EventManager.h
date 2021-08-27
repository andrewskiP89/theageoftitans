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
  OnAction,
  GameStateChange,
  ShowMessage,
  TaskCompleted
};
enum PActionType{
  Pull,
  Push,
  Use,
  PickUp,
  Throw,
  Study
};
struct UsableItem{
  uint16_t id;
};
struct Inventary{
  std::vector<UsableItem*> items;
};
struct PlayerAction{
  UsableItem *item;
  PActionType type;
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


#endif
