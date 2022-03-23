#include "../headers/EventManager.h"
//#include "../headers/EventsDefinition.h"

std::vector<AppEvent*> EventManager::m_eventQueue; // definition
std::set<std::string> EventManager::m_fireEventIds;



void AppEvent::fire(){
  if(type == EventType::ShowMessage || singleTime){
    std::cout << "inserting single time event for current id " << id << "\n";
    for(auto exId : EventManager::m_fireEventIds)
      std::cout << "Existing id " << exId << "\n";

    if(EventManager::m_fireEventIds.size() == 0 ||
          EventManager::m_fireEventIds.find(id) == EventManager::m_fireEventIds.end()){
      std::cout << "Triggering single time event " << id << "\n";
      EventManager::m_eventQueue.insert(EventManager::m_eventQueue.begin(), this);
      EventManager::m_fireEventIds.insert(id);
    }
  }else{
    std::cout << "inserting general event \n";
    EventManager::m_eventQueue.insert(EventManager::m_eventQueue.begin(), this);
  }
  std::cout << "Event queue size " << EventManager::m_eventQueue.size() << "\n";
}

bool EventManager::pollEvent(AppEvent &event){
  if(m_eventQueue.size() > 0){
    event = * (m_eventQueue.back());
    m_eventQueue.pop_back();
    return true;
  }
  return false;
}

void EventManager::fireEvent(AppEvent &event){
  AppEvent *eventToFire = new AppEvent(event);
  eventToFire->fire();
}

void EventManager::clearEvents(std::vector<AppEvent> &events){
  for(auto event : events){
    EventManager::m_fireEventIds.erase(event.id);
  }
}
void EventManager::triggerActionMenu(std::vector<AppEvent> eventsToLoad, const UsableItem& iitem){
  EventManager::changeGameState(GameState::OnActionMenu);
}


void EventManager::changeGameState(GameState targetGameState){
  AppEvent *aEvent = new AppEvent();
  aEvent->targetState = targetGameState;
  aEvent->type = EventType::GameStateChange;
  //aEvent->source = this;
  aEvent->fire();
}

std::map<TriggerType, std::vector<AppEvent>> EventManager::loadEventsFromMap(EventCoordinate coordinate, std::map<std::string, std::vector<std::string>> sourceMap){
  std::vector<AppEvent> eventList;
  std::map<TriggerType, std::vector<AppEvent>>  eventsTTypeMap;

  uint8_t currentLevel = coordinate.currentLevel;
  std::map<std::string, AppEvent> eventMap = EventManager::getAllEvents(currentLevel);

  std::string eventKey = "l" + std::to_string(coordinate.currentLayer)
                          + "r" + std::to_string(coordinate.currentRow)
                          + "c" + std::to_string(coordinate.currentColumn)
                          + "lv" + std::to_string(coordinate.currentLevel);
  std::vector<std::string> eventIds;

  auto eventPair = sourceMap.find(eventKey);

  if(eventPair != sourceMap.end()){
    std::cout << "checking event for key : " << eventKey << "\n";
    eventIds = eventPair->second;
  }
  for(auto id : eventIds){
    auto currentEvent = eventMap.find(id);
    if(currentEvent != eventMap.end()){
      std::cout << "Printing event type " << currentEvent->second.type << "\n";
      if(eventsTTypeMap.find(currentEvent->second.triggerType) == eventsTTypeMap.end()){
        eventsTTypeMap[currentEvent->second.triggerType] = {};
      }
      eventsTTypeMap.find(currentEvent->second.triggerType)->second.push_back(currentEvent->second);

      eventList.push_back(currentEvent->second);
    }
  }
  return eventsTTypeMap;
}
