 // testing sfml project
#include "../headers/MenuManager.h"
#include "../headers/sfml.h"
#include "../headers/WindowManager.h"
//#include "../headers/EventManager.h" // commented as everything should only be included in WindowManager
//#include "../headers/MapManager.h"

#include <iostream>
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

Clickable::Clickable()
{

}
void Clickable::onclick()
{

}

sf::FloatRect Clickable::getClickableArea(){
  sf::FloatRect rect;
  return  rect;
}

/*CLICKABLES - end **/


/*************** EVENT MANAGER - start *******************/

void EventManager::notifyAll(const sf::Event &event){
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            std::cout << "the left button was pressed" << std::endl;
            std::cout << "mouse x: " << event.mouseButton.x << std::endl;
            std::cout << "mouse y: " << event.mouseButton.y << std::endl;
            // Using a for loop with iterator
            for(int i = 0; i < _clickSubscribers.size(); i++){
              Clickable *c = _clickSubscribers[i];
              sf::FloatRect carea = c->getClickableArea();

              if(carea.contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
                c->onclick();
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
void PDObject::onCollision(){}
sf::Vector2f  PDObject::getPosition(){ return sf::Vector2f(0.0f, 0.0f);}

bool WindowManager::init(){
  // initing the menu
  m_gameState = GameState::Playing; // default will be set to Starting at one point

  load_assets();

  eventMgr = new EventManager();
  /*MenuContainer *menu = new MenuContainer();
  MenuItem *file = new MenuItem("File", true);
  menu->addMenuItem(file);
  MenuItem *edit = new MenuItem("Edit", true);
  menu->addMenuItem(edit);
  MenuItem *view = new MenuItem("View", true);
  menu->addMenuItem(view);


  MenuDropdown *dropdown = new MenuDropdown(*file);
  MenuItem *createFile = new MenuItem("New File");
  dropdown->addMenuItem(createFile);
  MenuItem *openFile = new MenuItem("Open File");
  dropdown->addMenuItem(openFile);

  file->linkDropdown(dropdown);*/

  gameMap.load(TILE_SET, sf::Vector2u(16, 16));
  gameMap.addLayer(LAYER_1, 50, 50);
  gameMap.addLayer(LAYER_2, 50, 50);

  //add player here
  Player * link = new Player();
  link->animation.loadAsset(LINK_ASSET, link->sprite, 10, 8);
  addDrawable(link);

  camera.initCamera(_window->getSize(), sf::FloatRect(0, 0, 2000.0f, 2000.0f));
  camera.setTarget(link);

  //ActionMenu *menu = new ActionMenu();
  ActionMenu menu;
  MenuItem *push = new MenuItem("Push", true);
  menu.addMenuItem(push);
  MenuItem *pull = new MenuItem("Pull", true);
  menu.addMenuItem(pull);
  MenuItem *take = new MenuItem("Take", true);
  menu.addMenuItem(take);
  MenuItem *use = new MenuItem("Use", true);
  menu.addMenuItem(use);

  //menu.setCamera(camera);
  m_actionMenu = menu;
  //_windowItems.push_back(menu);
  //_windowItems.push_back(dropdown);

  for(auto item : menu.getClickables()){
    eventMgr->registerItem(item);
  }

  //m_musicMgr.loadTrack();

  m_textContainer.init(&camera, DEFAULT_WIDTH, DEFAULT_HEIGHT * 0.2f);
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
      if (event.type == sf::Event::Closed)
          getWindow()->close();
  }
  // managing custom app events
  AppEvent appEvent;
  while(eventMgr->pollEvent(appEvent)){
    std::cout << "Printing event type "  << appEvent.type << "\n";
    if(appEvent.type == EventType::GameStateChange){
      m_gameState = appEvent.targetState;
    }
    if(appEvent.type == EventType::ShowMessage){
      m_gameState = GameState::OnDialog;
      m_textContainer.setDialog(appEvent.dialog);
    }
  }
}

void WindowManager::draw(){
    _window->clear();

    _window->setView(camera.view);

    std::vector<MapLayer*> levelLayers = gameMap.m_layers;
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
    }
    if(m_gameState == GameState::OnDialog){
      m_textContainer.draw(_window);
    }else if(m_gameState == GameState::OnActionMenu){
      // draw menu
      m_actionMenu.draw(_window);
      /*for(int i = 0; i < _windowItems.size(); i ++){
        _windowItems[i]->draw(_window);
      }*/
    }
    _window->display();
}
void WindowManager::setScenery(Scenery * sc){
  scenary = sc;
}

void WindowManager::update(){
  float deltas = clock.getElapsedTime().asSeconds();
  clock.restart();
  //m_musicMgr.play();
  //std::cout << "Printing frame rate " << 1 / deltas << " \n";
  if(m_gameState == GameState::OnDialog || m_gameState == GameState::Playing){
    for(int i = 0; i < _itemsToDisplay.size(); i ++){
      _itemsToDisplay[i]->update(deltas);
    }
  }


  camera.updateCamera(sf::FloatRect(0, 0, scenary->mapSize.x, scenary->mapSize.y));
  m_actionMenu.setCamera(camera);

  if(m_gameState == GameState::OnActionMenu){

    m_actionMenu.update(deltas);
    /*for(auto menuItem : _windowItems){
      menuItem->update(deltas);
    }*/
  }
  if(m_gameState == GameState::OnDialog){
    m_textContainer.update(deltas);
  }
}

void WindowManager::checkCollisions(){
  int currentLayer = 1; // this info needs to be calculated dynamically
                        // based on player position
  for(int i = 0; i < _itemsToDisplay.size(); i ++){
    sf::FloatRect itemArea = _itemsToDisplay[i] -> getBounds();
    MapLayer *currentLevelLayer = gameMap.m_layers[currentLayer];
    for(auto item : currentLevelLayer->m_collidableItems){
      if(itemArea.intersects(*item)){
        _itemsToDisplay[i]->onCollision();
      }
    }
    for(auto item  : currentLevelLayer-> m_eventAreas){
      if(itemArea.intersects(*item.first)){

        for(auto event : item.second){
          /*AppEvent *eventToFire = new AppEvent(event);
          eventToFire->fire();*/
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

std::vector<MenuItem*> MenuContainer::getClickables(){
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

std::vector<MenuItem*> MenuDropdown::getClickables(){
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
}

sf::Text MenuItem::getLabel(){
  return _label;
}

void MenuItem::onclick(){
  std::cout << "You pressed on item " << this->_menuText << "\n";
  WindowManager * wm = WindowManager::getManager();
  //std::cout << "Here we go again";
  if(_isRoot){
    //std::cout << "clearing the  displayed items";
    //wm->clearItems();
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
  return _label.getGlobalBounds();
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
