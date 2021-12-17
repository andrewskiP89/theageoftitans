 // testing sfml project
#include "../headers/MenuManager.h"
#include "../headers/sfml.h"
#include "../headers/WindowManager.h"
//#include "../headers/EventManager.h" // commented as everything should only be included in WindowManager
//#include "../headers/MapManager.h"

#include <iostream>
#include <string>
// MAIN APP CONFIGS


// managing general font
sf::Font app_font;

bool font_inited = false;

bool load_assets(){
  // load app font
  app_font;
  return(app_font.loadFromFile(SRC_FLDR));
}

sf::Font getFont(){
  return app_font;
}

sf::RenderWindow window(sf::VideoMode(DEFAULT_WIDTH, DEFAULT_HEIGHT), PB_DASHBOARD_TITLE);

Scenery::Scenery(){

}

bool Scenery::loadMapFromFile(std::string mapSrc){
  if(texture.loadFromFile(mapSrc)){
    //std::cout << "map loaded correctly " << "x: " << texture.getSize().x << " \t y: " << texture.getSize().y  << "\n";
    map.setTexture(texture);
    map.setScale(10.0f, 10.0f);
    //map.setPosition(100.0f, 100.0f);
    mapSize = sf::Vector2u(10.0f * texture.getSize().x, 10.0f * texture.getSize().y);
    //map.setTextureRect(sf::IntRect(0, 0, mapSize.x, mapSize.y));
    return true;
  }
  return false;
}

///////////// CAMERA ////////////
Camera::Camera(){

}

void Camera::setTarget(PDObject *trgt){
  target = trgt;
  updateCamera(horizon);
}

void Camera::initCamera(sf::Vector2u size, sf::FloatRect horizon){
  //size = size;
  view.setSize(size.x, size.y);
  view.setCenter(size.x / 2, size.y / 2);
  horizon = horizon;
}

void Camera::updateCamera(sf::FloatRect horizon){
  //std::cout << "target x: " << target->getPosition().x << "\t target y: " << target->getPosition().y << "\n";
  horizon = horizon;
  sf::Vector2f position = target->getPosition();
  sf::Vector2f trgtPos = target->getPosition();

  //std::cout << "Printing horizon left " << horizon.left  <<  "\t Printing horizon right  " << horizon.left + horizon.width << "\n";
  if((trgtPos.x - horizon.left) < view.getSize().x / 2){
    position.x = horizon.left + (view.getSize().x / 2);
  }

  if((horizon.left + horizon.width - trgtPos.x) < view.getSize().x / 2){
    position.x = horizon.left + horizon.width  - (view.getSize().x / 2);
  }

  if((trgtPos.y - horizon.top) < view.getSize().y / 2){
    position.y = horizon.top + (view.getSize().y / 2);
  }

  if((horizon.top + horizon.height - trgtPos.y) < view.getSize().y / 2){
    position.y = horizon.top + horizon.height  - (view.getSize().y / 2);
  }
  //std::cout << "Camera position  x:" << position.x << "\ty: " << position.y << "\n";
  view.setCenter(position);
}

////////// ANIMATION ////////////
Animation::Animation()
{
  timeSinceUpdate = 0.0f;
}

void Animation::loadAsset(std::string file_path, sf::Sprite &sprite, int wmax, int hmax)
{

  if(texture.loadFromFile(file_path)){
    sprite.setTexture(texture);
    //sprite.setScale(0.25f, 0.25f);
    frameSize = frameSize;
    orientation = LEFT;
    currentFrame = 0;
    timeSinceUpdate = 0.0f;
    m_rateFactor = 1.0f;
    int textureWidth = texture.getSize().x;
    int textureHeight = texture.getSize().y;
    frameSize.x = textureWidth / wmax;
    frameSize.y = textureHeight / hmax;

    //std::cout << "Printing frameSize.x  " << frameSize.x  << "\n";
    //std::cout << "Printing frameSize.y  " << frameSize.y  << "\n";
    _hmax = hmax;
    _wmax = wmax;

    rate = ANIMATION_PERIOD;
    sprite.setTextureRect(sf::IntRect(0, 5 * frameSize.y, frameSize.x, frameSize.y));
  }
}
void Animation::update(P_State pState){
  if(pState == P_State::RUNNING){
    m_rateFactor = 1.0f;
    if(orientation == HOrientation::UP){
      currentRow = 7;
      _wmax = 8;
    }else if(orientation == HOrientation::RIGHT){
      currentRow = 8;
      _wmax = 8;
    }else if(orientation == HOrientation::DOWN){
      currentRow = 5;
      _wmax = 8;
    }else if(orientation == HOrientation::LEFT){
      currentRow = 6;
      _wmax = 8;
    }
  }else{
    m_rateFactor = 4.0f;
    if(orientation == HOrientation::UP){
      currentRow = 3;
      _wmax = 1;
    }else if(orientation == HOrientation::RIGHT){
      currentRow = 4;
      _wmax = 3;
    }else if(orientation == HOrientation::DOWN){
      currentRow = 1;
      _wmax = 3;
    }else if(orientation == HOrientation::LEFT){
      currentRow = 2;
      _wmax = 3;
    }
  }
}
void Animation::getCurrentFrame(float delta, sf::Sprite &sprite)
{

  //std::cout << "Updating at timeSinceUpdate: " << timeSinceUpdate << "\n";
  timeSinceUpdate += delta;
  float currentRateFactor = currentFrame == 0 ? m_rateFactor : 1.00f;
  if(timeSinceUpdate > rate * currentRateFactor){
    //timeSinceUpdate = 0;
    //std::cout << "Updating at time " << timeSinceUpdate << "\n";
    timeSinceUpdate -= rate;
    if(timeSinceUpdate > rate)
      timeSinceUpdate = 0;
    currentFrame = ( currentFrame >= _wmax - 1 ) ? 0 : currentFrame + 1;
    //std::cout << "Updating currentFrame  " << _wmax - 1 << "\t" << currentFrame << "\n";
  }
  sprite.setTextureRect(sf::IntRect(currentFrame * frameSize.x, (currentRow - 1) * frameSize.y, frameSize.x, frameSize.y));
}

Player::Player()
{

}

void Player::update(float deltas){

  animation.update(pState);
  animation.getCurrentFrame(deltas, sprite);
  //std::cout << "Printing deltas " << deltas << "\n";
  m_oldPosition = sprite.getPosition();

  sprite.move(speed.x * deltas, speed.y * deltas);
  m_position = sprite.getPosition();
  //std::cout << "Printing speed x " << speed.x << "\t y " << speed.y << "\n";
}

void Player::move(sf::Vector2f delta){
  sprite.move(delta.x, delta.y);
  m_position = sprite.getPosition();
  m_oldPosition = m_position;
}



void Player::draw(sf::RenderWindow * window){

  window -> draw(sprite);
}

void Player::onEvent(sf::Event event){
  /*if (event.type == sf::Event::KeyPressed){

    /*if(event.key.code == sf::Keyboard::Up){
      animation.orientation = HOrientation::UP;
      speed.y = - LINK_SPEED;
    }
    if(event.key.code == sf::Keyboard::Right){
      animation.orientation = HOrientation::RIGHT;
      speed.x =  LINK_SPEED;
    }
    if(event.key.code == sf::Keyboard::Down){
      animation.orientation = HOrientation::DOWN;
      speed.y =  LINK_SPEED;
    }
    if(event.key.code == sf::Keyboard::Left){
      animation.orientation = HOrientation::LEFT;
      speed.x = -  LINK_SPEED;
    }*/

  /*}else{
    pState = P_State::NORMAL;
    speed.x = 0;
    speed.y = 0;
  }*/
  bool moving = false;
  if(event.type == sf::Event::KeyPressed){
    if(event.key.code == sf::Keyboard::A){
      AppEvent *tAMenu = new AppEvent();
      tAMenu->type = EventType::GameStateChange;
      tAMenu->targetState = GameState::OnActionMenu;
      tAMenu->fire();
    }

  }else if(event.type == sf::Event::KeyReleased){
    if(event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down){
      speed.y = 0;
    }
    if(event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right){
      speed.x = 0;
    }
  }

  sf::Keyboard::Key lastKeyPressed;
  if(event.type == sf::Event::KeyPressed){
    lastKeyPressed = event.key.code;
  }

  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && lastKeyPressed != sf::Keyboard::Left){
    moving = true;
    animation.orientation = HOrientation::RIGHT;
    speed.x =  LINK_SPEED;
  }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && lastKeyPressed != sf::Keyboard::Right){
    moving = true;
    animation.orientation = HOrientation::LEFT;
    speed.x = -  LINK_SPEED;
  }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && lastKeyPressed != sf::Keyboard::Down){
    moving = true;
    animation.orientation = HOrientation::UP;
    speed.y = - LINK_SPEED;
  }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && lastKeyPressed != sf::Keyboard::Up){
    moving = true;
    animation.orientation = HOrientation::DOWN;
    speed.y =  LINK_SPEED;
  }

  if(moving){
    pState = P_State::RUNNING;
  }else{
    pState = P_State::NORMAL;
    speed.x = 0;
    speed.y = 0;
  }
}
void Player::onCollision(){

  sprite.setPosition(m_oldPosition);
  m_position = m_oldPosition;
}
sf::Vector2f Player::getPosition(){
  return sprite.getPosition();
}

/*CLICKABLES - start **/

/*Clickable::Clickable()
{

}*/
void Clickable::onclick()
{

}

sf::FloatRect Clickable::getClickableArea(){
  sf::FloatRect rect;
  return  rect;
}

bool const Clickable::isLocal(){
  return m_isLocalItem;
}

sf::Text Clickable::getLabel(){
  return _label;
}

/*CLICKABLES - end **/


/*************** EVENT MANAGER - start *******************/

void EventManager::notifyAll(const sf::Event &event){
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            //std::cout << "the left button was pressed" << std::endl;
            //std::cout << "mouse x: " << event.mouseButton.x << std::endl;
            //std::cout << "mouse y: " << event.mouseButton.y << std::endl;
            //std::cout << "Subscribers size : " << _clickSubscribers.size() << std::endl;
            // Using a for loop with iterator

            for(int i = 0; i < _clickSubscribers.size(); i++){
              Clickable *c = _clickSubscribers[i];
              sf::FloatRect carea = c->getClickableArea();
              sf::Vector2f localMouse(event.mouseButton.x, event.mouseButton.y);
              WindowManager *wm = WindowManager::getManager();
              Camera camera = wm->camera;
              sf::Vector2f cameraCenter = camera.getView().getCenter();
              sf::Vector2f cameraTranslation(cameraCenter.x - DEFAULT_WIDTH / 2, cameraCenter.y - DEFAULT_HEIGHT / 2);
              sf::Vector2f globalMouse = localMouse + cameraTranslation;
              //std::cout << "Global mouse x: " << globalMouse.x << std::endl;
              //std::cout << "Global mouse y: " << globalMouse.y << std::endl;
              if(c->isLocal()){
                if(carea.contains(sf::Vector2f(localMouse.x, localMouse.y))){
                  std::cout << "clicking on item \n";
                  c->onclick();
                }
              }else{
                if(carea.contains(sf::Vector2f(globalMouse.x, globalMouse.y))){
                  std::cout << "clicking on world item \n";
                  c->onclick();
                }
              }


            }
        }
    }
}


void EventManager::registerItem(Clickable* s){
    _clickSubscribers.push_back(s);
}

EventManager::EventManager(){

}
EventManager::~EventManager(){

}
/*************** EVENT MANAGER - end *******************/

/*************** WINDOW MANAGER - start *******************/

WindowManager *WindowManager::wm = 0;

WindowManager * WindowManager::getManager(){
  if(!wm)
    wm = new WindowManager();

  return wm;
}
void PDObject::draw(sf::RenderWindow *window){}
void PDObject::update(float deltas){}
void PDObject::onEvent(sf::Event event){}
void PDObject::onAppEvent(AppEvent event){}
void PDObject::onCollision(){}
sf::Vector2f  PDObject::getPosition(){ return sf::Vector2f(0.0f, 0.0f);}

bool WindowManager::init(){
  // initing the menu
  m_gameState = GameState::Starting; // the moment has come

  load_assets();

  eventMgr = new EventManager();

  // =================================================//

  // LOADING INITIAL MENU - START

  MenuItem *newGame = new MenuItem(NEW_GAME_LABEL, true);
  m_startingMenu.addMenuItem(newGame);
  MenuItem *gameLoad = new MenuItem(CONTINUE_LABEL, true);
  m_startingMenu.addMenuItem(gameLoad);
  MenuItem *quit = new MenuItem(QUIT_LABEL, true);
  m_startingMenu.addMenuItem(quit);

  for(auto item : m_startingMenu.getClickables()){
    eventMgr->registerItem(item);
  }

  // LOADING INITIAL MENU - END

  // =================================================//

  // LOADING LEVEL INFO - -start //@TODO This will be moved to a dedicated procedure

  Player * link = new Player();
  link->animation.loadAsset(LINK_ASSET, link->sprite, 10, 8);
  link->move(sf::Vector2f(250.0f, 300.0f));
  link->m_zLayer = 1;
  //link->move(sf::Vector2f(20.0f, 0.0f));
  m_currentPlayer = link;
  addDrawable(link);

  //gameMap.load(TILE_SET, sf::Vector2u(16, 16));
  gameMap.loadLevel(1);
  //gameMap.addLayer(LAYER_1, 50, 50);
  //gameMap.addLayer(LAYER_2, 50, 50);

  // @TODO - the following code needs to be inovked
  // whenever the currentPlayer level changes
  size_t playableLayer = m_currentPlayer-> m_zLayer;
  if(gameMap.m_layerMap.find(playableLayer) != gameMap.m_layerMap.end()){
    for(auto currentLevelLayer : gameMap.m_layerMap[playableLayer]){
      for(auto item : currentLevelLayer->m_worldItems){
          std::cout << "Registering world item \n";
          eventMgr->registerItem(item);
      }
    }
  }
  // following commented code has been deprecated
  /*MapLayer *currentLevelLayer = gameMap.m_layers[playableLayer];
  for(auto item : currentLevelLayer->m_worldItems){
      std::cout << "Registering world item \n";
      eventMgr->registerItem(item);
  }*/

  camera.initCamera(_window->getSize(), sf::FloatRect(0, 0, 2000.0f, 2000.0f));
  camera.setTarget(link);

  // LOADING LEVEL INFO - -end

  // =================================================//

  // LOADING - ACTION MENU AND INVENTORY - start

  ActionMenu menu;
  MenuItem *push = new MenuItem("Push", true);
  menu.addMenuItem(push);
  MenuItem *pull = new MenuItem("Pull", true);
  menu.addMenuItem(pull);
  MenuItem *take = new MenuItem("Pick Up", true);
  menu.addMenuItem(take);
  MenuItem *use = new MenuItem("Use", true);
  menu.addMenuItem(use);

  m_actionMenu = menu;

  for(auto item : menu.getClickables()){
    eventMgr->registerItem(item);
  }

  // LOADING - ACTION MENU AND INVENTORY - end

  //m_musicMgr.loadTrack();

  // LOADING MESSAGES ENGINE - start
  m_textContainer.init(&camera, DEFAULT_WIDTH, DEFAULT_HEIGHT * 0.2f);
  // LOADING MESSAGES ENGINE - end
  return true;
}

WindowManager::WindowManager(){
  _window = new sf::RenderWindow(sf::VideoMode(DEFAULT_WIDTH, DEFAULT_HEIGHT), PB_DASHBOARD_TITLE);
  scenary = nullptr;
}

WindowManager::~WindowManager(){}

sf::RenderWindow* WindowManager::getWindow(){
  return _window;
}

void WindowManager::manageEvents(){
  sf::Event event;

  while (getWindow()->pollEvent(event)){

      eventMgr->notifyAll(event);
      notifyObjects(event);

      if(m_gameState == GameState::OnDialog){
        m_textContainer.onSFEvent(event);
      }
      if(m_gameState == GameState::OnActionMenu){
        m_actionMenu.onEvent(event);
      }
      if(m_gameState == GameState::Starting){
        //
      }
      if (m_gameState == GameState::Exiting || event.type == sf::Event::Closed)
          getWindow()->close();
  }
  // managing custom app events
  AppEvent appEvent;
  while(eventMgr->pollEvent(appEvent)){
    std::cout << "Printing event type "  << appEvent.type << "\n";
    if(appEvent.type == EventType::GameStateChange){
      std::cout << "Changing the GameState to " << appEvent.targetState << "\n";
      m_gameState = appEvent.targetState;
    }
    if(appEvent.type == EventType::ShowMessage){
      m_gameState = GameState::OnDialog;
      m_textContainer.setDialog(appEvent.dialog);
    }
    // pick an item event
    if(appEvent.type == EventType::PickUpItem){
      if(appEvent.source != nullptr){
        ItemSource *item = (ItemSource *) appEvent.source;
        m_inventory.addItem(item);
        std::cout << "Adding item to the player inventory \n";
      }
    }
  }
}

void WindowManager::draw(){
    _window->clear();

    if(m_gameState == GameState::Starting){
      m_startingMenu.draw(_window);
    }else{
      _window->setView(camera.view);

      size_t currentLayer = m_currentPlayer->m_zLayer;

      for(size_t l = 0; l < currentLayer; l++){
        if(gameMap.m_layerMap.find(l) != gameMap.m_layerMap.end()){
          std::vector<MapLayer*> levelLayers = gameMap.m_layerMap[l];
          for(auto layer : levelLayers){
            _window->draw(*layer);
          }
        }
      }
      for(int i = 0; i < _itemsToDisplay.size(); i ++){
        _itemsToDisplay[i]->draw(_window);
      }
      for(size_t l = currentLayer; l < gameMap.m_layerMap.size(); l++){
        if(gameMap.m_layerMap.find(l) != gameMap.m_layerMap.end()){
          std::vector<MapLayer*> levelLayers = gameMap.m_layerMap[l];
          for(auto layer : levelLayers){
            _window->draw(*layer);
          }
        }
      }
      // following commented code has been deprecated
      /*std::vector<MapLayer*> levelLayers = gameMap.m_layers;
      if(levelLayers.size() > 0){
        _window->draw(*levelLayers[0]);
      }
      for(int i = 0; i < _itemsToDisplay.size(); i ++){
        _itemsToDisplay[i]->draw(_window);
      }
      if(levelLayers.size() > 1){
        for(int i = 1; i < levelLayers.size(); i ++){
          _window->draw(*levelLayers[i]);
        }
      }*/
      if(m_gameState == GameState::OnDialog){
        m_textContainer.draw(_window);
      }else if(m_gameState == GameState::OnActionMenu){
        // draw menu
        m_actionMenu.draw(_window);
        m_inventory.draw(_window);
        /*for(int i = 0; i < _windowItems.size(); i ++){
          _windowItems[i]->draw(_window);
        }*/
      }
    }
    _window->display();
}
void WindowManager::setScenery(Scenery * sc){
  scenary = sc;
}

void WindowManager::update(){
  float deltas = clock.getElapsedTime().asSeconds();
  clock.restart();
  if(m_gameState != GameState::Starting){
    if(m_gameState == GameState::OnDialog || m_gameState == GameState::Playing){
      for(int i = 0; i < _itemsToDisplay.size(); i ++){
        _itemsToDisplay[i]->update(deltas);
      }
    }
    camera.updateCamera(sf::FloatRect(0, 0, scenary->mapSize.x, scenary->mapSize.y));
    m_actionMenu.setCamera(camera);

    if(m_gameState == GameState::OnActionMenu){
      m_inventory.setCamera(&camera);
      m_actionMenu.update(deltas);
      m_inventory.update();
    }
    if(m_gameState == GameState::OnDialog){
      m_textContainer.update(deltas);
    }
  }

}

void WindowManager::checkCollisions(){

  for(int i = 0; i < _itemsToDisplay.size(); i ++){
    sf::FloatRect itemArea = _itemsToDisplay[i] -> getBounds();

    if(gameMap.m_layerMap.find(_itemsToDisplay[i] -> m_zLayer) != gameMap.m_layerMap.end()){
      for(auto currentLevelLayer : gameMap.m_layerMap[_itemsToDisplay[i] -> m_zLayer]){
        for(auto item : currentLevelLayer->m_collidableItems){
          if(itemArea.intersects(*item)){
            _itemsToDisplay[i]->onCollision();
          }
        }
        for(auto item  : currentLevelLayer-> m_eventAreas){
          if(itemArea.intersects(*item.first)){
            for(auto event : item.second){
              //AppEvent *eventToFire = new AppEvent(event);
              //eventToFire->fire();
              eventMgr->fireEvent(event);
              //eventToFire->type = EventType::ShowMessage;
              //std::cout << "Registered event having type : " << event.type << "\n";
            }
          }else{
            eventMgr->clearEvents(item.second);
          }
        }
      }
    }
    // following commented code has been deprecated
    /*MapLayer *currentLevelLayer = gameMap.m_layers[_itemsToDisplay[i] -> m_zLayer];
    for(auto item : currentLevelLayer->m_collidableItems){
      if(itemArea.intersects(*item)){
        _itemsToDisplay[i]->onCollision();
      }
    }
    for(auto item  : currentLevelLayer-> m_eventAreas){
      if(itemArea.intersects(*item.first)){
        for(auto event : item.second){
          //AppEvent *eventToFire = new AppEvent(event);
          //eventToFire->fire();
          eventMgr->fireEvent(event);
          //eventToFire->type = EventType::ShowMessage;
          //std::cout << "Registered event having type : " << event.type << "\n";
        }
      }else{
        eventMgr->clearEvents(item.second);
      }
    }*/
  }
}

void WindowManager::notifyObjects(sf::Event event){
  for(int i = 0; i < _itemsToDisplay.size(); i ++){
    _itemsToDisplay[i] -> onEvent(event);
  }
  if(m_gameState == GameState::OnDialog){

  }
}
void WindowManager::clearItems(){
  _itemsToDisplay.clear();
}

void WindowManager::addDrawable(PDObject * obj){
  _itemsToDisplay.push_back(obj);
}

/*************** WINDOW MANAGER - end *******************/

/*************** MENU MANAGER - start *******************/
// CONTAINER
MenuContainer::MenuContainer(){
  _menuBar = new sf::RectangleShape(sf::Vector2f(DEFAULT_WIDTH, 30));
  _menuBar -> setFillColor(sf::Color(192, 192, 192));
  _menuBar -> setOutlineColor(sf::Color::Black);
  _menuBar -> setOutlineThickness(1.0f);
}

MenuContainer::~MenuContainer(){
}

void MenuContainer::addMenuItem(MenuItem * item){
  int menuLength = _menuItems.size();
  item->setPosition(90.0f * menuLength, 0.0f);
  _menuItems.push_back(item);
}

void MenuContainer::draw(sf::RenderWindow * window){
  window -> draw(*_menuBar);
  for(int i = 0; i < _menuItems.size(); i++){
    window -> draw(_menuItems[i] -> getLabel());
  }
}

std::vector<Clickable*> MenuContainer::getClickables(){
  return _menuItems;
}
// MenuDropdown

MenuDropdown::MenuDropdown(MenuItem &menuItem){
  sf::Vector2f menuPosition = menuItem.getLabel().getPosition();
  sf::Vector2f anchorPoint(menuPosition.x, menuPosition.y + 30.f);
  _anchorPoint = anchorPoint;
}

MenuDropdown::~MenuDropdown(){
}

void MenuDropdown::addMenuItem(MenuItem * item){
  int menuHeight = _menuItems.size();
  item->setPosition(_anchorPoint.x, _anchorPoint.y + 30.0f * menuHeight);
  _menuItems.push_back(item);
}

void MenuDropdown::draw(sf::RenderWindow * window){
  //std::cout << "Drawing a dropdown with "  << _menuItems.size() << " menu items\n";
  sf::RectangleShape dropdownContainer(sf::Vector2f(200.0f, _menuItems.size() * 30.0f));
  dropdownContainer.setFillColor(sf::Color(192, 192, 192));
  dropdownContainer.setPosition(_anchorPoint);
  dropdownContainer.setOutlineColor(sf::Color::Black);
  dropdownContainer.setOutlineThickness(1.0f);

  window->draw(dropdownContainer);
  for(int i = 0; i < _menuItems.size(); i++){
    window -> draw(_menuItems[i] -> getLabel());
  }
}

std::vector<Clickable*> MenuDropdown::getClickables(){
  return _menuItems;
}
// MENU ITEMS

MenuItem::MenuItem(const std::string menuText, bool isRoot){

  sf::Text label(menuText, app_font, MENU_FONT_SIZE);
  label.setOutlineColor(sf::Color::White);
  _label = label;
  _menuText = menuText;
  _isRoot = isRoot;
  _atype = ActionType::execute;

}

void MenuItem::setPosition(float x, float y){
  _label.setPosition(x, y);
  m_clickableArea = _label.getGlobalBounds();
}

sf::Text MenuItem::getLabel(){
  return _label;
}

void MenuItem::onclick(){
  std::cout << "You pressed on item " << this->_menuText << "\n";
  WindowManager * wm = WindowManager::getManager();
  if( wm->m_gameState == GameState::OnActionMenu){
    //making checks on the possible triggerable Events
    std::cout << "available event number " << wm->m_actionMenu.m_loadedEvents.size() << "\n";
    PActionType aType = PActionType::None;

    if(this->_menuText == "Push"){
      aType = PActionType::Push;
    }else if(this->_menuText == "Pull"){
      aType = PActionType::Pull;
    }else if(this->_menuText == "Pick Up"){
      aType = PActionType::PickUp;
    }else if(this->_menuText == "Use"){
      aType = PActionType::Use;
    }
    std::cout << "Priting selected aType " << aType << "\n";

    for(auto lEvent : wm->m_actionMenu.m_loadedEvents){
      std::cout << "Priting lEvent type" << lEvent.action.type << "\n";
      if(lEvent.action.type == aType){
        std::cout << "Firing event for " << this->_menuText << "\n";
        wm->eventMgr->fireEvent(lEvent);
      }
    }
  }else if(wm->m_gameState == GameState::Starting){
    if(this->_menuText == NEW_GAME_LABEL)
      EventManager::changeGameState(GameState::Playing);
    else if(this->_menuText == CONTINUE_LABEL)
      std::cout << "Continuing the game \n"; // @TODO
    else if(this->_menuText == QUIT_LABEL)
      EventManager::changeGameState(GameState::Exiting);

  }

  if(_atype == ActionType::show_menu){
    // input commands to show the menu
    std::cout << "adding the dropdown to items to display";

    MenuDropdown *ddown = static_cast<MenuDropdown*>(_md);
    wm->addDrawable(ddown);

  }else if(_atype == ActionType::execute){
    // exectuting a specific action (ex: open explorer)
  }else{
    std::cout << "There is no action link to this menu item.";
  }
}

void MenuItem::linkDropdown(void *md){
  _atype = ActionType::show_menu;
  _md = md;
}

sf::FloatRect MenuItem::getClickableArea(){
  sf::FloatRect globalBounds = m_clickableArea;
  std::string labelText = _label.getString();

  return m_clickableArea;
  //return _label.getLocalBounds();
}
/*************** MENU MANAGER - end *******************/

int main(int argc, char const *argv[]) {

  WindowManager *wManager = WindowManager::getManager();
  if(!wManager->init())
    return EXIT_FAILURE;

  try{
    Scenery scene;
    scene.loadMapFromFile(MAP_ASSET);
    wManager->setScenery(&scene);

    wManager->getWindow()->setVerticalSyncEnabled(true);

    sf::Music music;
    music.openFromFile("./assets/music/bg_music.wav");
    music.play();

    sf::Cursor cursor;
    if (cursor.loadFromSystem(sf::Cursor::Hand))
      wManager->getWindow()->setMouseCursor(cursor);
    //wManager->getWindow()->setFramerateLimit(60);
    while (wManager->getWindow()->isOpen())
    {
        //wManager->clock.restart();

        wManager->manageEvents();
        wManager->update();
        wManager->checkCollisions();
        wManager->draw();


    }
  }catch(const std::exception& exc){
    std::cout << "Something went wrong " << exc.what();
  }

  std::cout << "Exiting .. \n";
  return 0;
}
