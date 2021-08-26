#include "../headers/EventManager.h"

std::vector<AppEvent*> EventManager::m_eventQueue; // definition
void AppEvent::fire(){
  EventManager::m_eventQueue.insert(EventManager::m_eventQueue.begin(), this);
}
