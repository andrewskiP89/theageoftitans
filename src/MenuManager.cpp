#include "../headers/MenuManager.h"
#include "../headers/EventManager.h"

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
  if(m_cameraInited == false){
    m_oldPosition = camera.getView().getCenter();
    m_cameraInited = true;
  }
}

void ActionMenu::update(float deltas){

  sf::Vector2f currentPosition = m_camera.getView().getCenter();

  if(currentPosition != m_oldPosition){
    std::cout << "Number of events loaded : " << m_loadedEvents.size();
    sf::Vector2f menuOffset = currentPosition - m_oldPosition;

    std::cout << "Moving camera offset x:" << menuOffset.x << "\ty: " << menuOffset.y << "\n";
    _menuBar->move(menuOffset);
    for(auto item : _menuItems){
      item->_label.move(menuOffset);
    }
    m_oldPosition = currentPosition;
  }
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

void ActionMenu::onEvent(sf::Event event){
  if (event.type == sf::Event::KeyPressed){
    if(event.key.code == sf::Keyboard::Escape){
      std::cout << "Exiting the Action Menu\n";
      EventManager::changeGameState(GameState::Playing);
    }
  }
}

///////////// STARTING MENU ////////////////

StartingMenu::StartingMenu(){
  _menuBar = new sf::RectangleShape(sf::Vector2f(DEFAULT_WIDTH, DEFAULT_HEIGHT ));
  _menuBar -> setFillColor(sf::Color::Black);
  _menuBar -> setOutlineColor(sf::Color::White);
  _menuBar -> setOutlineThickness(5.0f);
  _menuBar-> setPosition(0.0f, 0.0f);
}

void StartingMenu::addMenuItem(MenuItem * item) {

  int menuLength = _menuItems.size();
  sf::Vector2f menuSize = _menuBar->getSize();
  sf::Vector2f menuPosition = _menuBar->getPosition();
  sf::FloatRect bound = item->getLabel().getLocalBounds();

  item-> setPosition(DEFAULT_WIDTH / 2 - bound.width / 2, ( DEFAULT_HEIGHT / 3 - bound.height / 2 ) + menuLength * (bound.height + 50.0f));

  //item->setPosition(90.0f * menuLength, 0.0f);
  _menuItems.push_back(item);
}
