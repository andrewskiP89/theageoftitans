#include "../headers/MenuManager.h"

ActionMenu::ActionMenu(){
  m_cameraInited = false;
  _menuBar = new sf::RectangleShape(sf::Vector2f(DEFAULT_WIDTH / 4, DEFAULT_HEIGHT / 4));
  _menuBar -> setFillColor(sf::Color::Transparent);
  _menuBar -> setOutlineColor(sf::Color::Black);
  _menuBar -> setOutlineThickness(1.0f);
  sf::Vector2f menuSize = _menuBar->getSize();
  _menuBar-> setPosition(DEFAULT_WIDTH / 2 - menuSize.x / 2, DEFAULT_HEIGHT / 2 - menuSize.y / 2);
}

void ActionMenu::setCamera(const Camera &camera){
  m_camera = camera;
  m_cameraInited = true;
}

void ActionMenu::addMenuItem(MenuItem * item) {

  int menuLength = _menuItems.size();
  sf::Vector2f menuSize = _menuBar->getSize();
  sf::Vector2f menuPosition = _menuBar->getPosition();
  sf::FloatRect bound = item->getLabel().getLocalBounds();
  if(menuLength == 0){
    item-> setPosition(menuPosition.x + menuSize.x / 2 - bound.width / 2, menuPosition.y - bound.height / 2);
  }else if(menuLength == 1){
    item-> setPosition(menuPosition.x + menuSize.x - bound.width, menuPosition.y + menuSize.y / 2 - bound.height );
  }else if(menuLength == 2){
    item-> setPosition(menuPosition.x + menuSize.x / 2 - bound.width / 2, menuPosition.y + menuSize.y - 2 * bound.height);
  }else if(menuLength == 3){
    item-> setPosition(menuPosition.x , menuPosition.y + menuSize.y / 2 - bound.height );
  }

  //item->setPosition(90.0f * menuLength, 0.0f);
  _menuItems.push_back(item);
}
