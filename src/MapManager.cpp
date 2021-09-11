#include "../headers/MapManager.h"
#include "../headers/WindowManager.h"

WorldItem::WorldItem(sf::FloatRect *area){
  m_area = area;
  m_isLocalItem = false;
}

sf::FloatRect WorldItem::getClickableArea(){
  std::cout << m_area->top << "\t" << m_area->left + m_area->width << "\t" << m_area->top + m_area->height << "\t" << m_area->left << "\n";
  return *m_area;
}

void WorldItem::onclick(){
  WindowManager *wm = WindowManager::getManager();
  // loading action events on the action menu
  if(wm){
    wm->m_actionMenu.m_loadedEvents = m_events;
    EventManager::changeGameState(GameState::OnActionMenu);
  }
}
