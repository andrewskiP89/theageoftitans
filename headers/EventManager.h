#include "sfml.h"
#include "Object.h"

#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

enum GameState { Starting,
                  Playing,
                  Pause,
                  OnActionMenu,
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
struct Message{
  Message(){
    sourcePlayer = nullptr;
    from = "";
  };
  std::string content;
  std::string from;
  Player *sourcePlayer;
};
struct Dialog{
  std::vector<Message> messages;
};
struct AppEvent{
  std::string id;
  void *source;
  GameState targetState;
  int requiredTask;
  uint16_t taskCompleted;
  Dialog dialog;
  EventType type;
  bool singleTime;
  void fire();
};

struct EventCoordinate {
  uint8_t currentLayer;
  uint8_t currentLevel;

  uint8_t currentRow;
  uint8_t currentColumn;
};

class EventManager{
private:
  std::vector<Clickable*> _clickSubscribers; // all clickable objects are subribed to click event by default
  std::vector<sf::Event> _clickQueue;
public:
  static std::vector<AppEvent*> m_eventQueue;
  static std::set<std::string> m_fireEventIds;
  static std::vector<AppEvent> loadEventsFromMap(EventCoordinate coordinate, std::map<std::string, std::vector<std::string>> sourceMap);
  static std::map<std::string, AppEvent> getAllEvents(uint8_t currentLevel);
  static void publishEvent(AppEvent);
  void notifyAll(const sf::Event &event);
  void registerItem(Clickable* s);
  void fireEvent(AppEvent &event);
  void clearEvents(std::vector<AppEvent> &events);
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
