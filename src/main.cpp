 // testing sfml project
#include "../headers/MenuManager.h"
#include "../headers/sfml.h"
#include "../headers/WindowManager.h"
#include "../headers/EventManager.h"
#include "../headers/MapManager.h"

#include <iostream>
// MAIN APP CONFIGS
#define PB_DASHBOARD_TITLE "The age of Titans"
#define DEFAULT_WIDTH 1600
#define DEFAULT_HEIGHT 900
#define MENU_FONT_SIZE 25

#define SRC_FLDR "./assets/fonts/dos_font.ttf"
#define LINK_SPEED 5.0e5f
#define LINK_ASSET "./assets/imgs/link_new.png"

#define MAP_ASSET "./assets/imgs/worldmap.png"

#define TILE_SET "./assets/imgs/titanstileset.png"
#define LAYER_1 "./assets/levelmap/titans_level1_layer1.csv"
#define LAYER_2 "./assets/levelmap/titans_level1_layer2.csv"

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
    frameSize = frameSize;
    orientation = LEFT;
    currentFrame = 0;
    timeSinceUpdate = 0.0f;
    int textureWidth = texture.getSize().x;
    int textureHeight = texture.getSize().y;
    frameSize.x = textureWidth / wmax;
    frameSize.y = textureHeight / hmax;

    //std::cout << "Printing frameSize.x  " << frameSize.x  << "\n";
    //std::cout << "Printing frameSize.y  " << frameSize.y  << "\n";
    _hmax = hmax;
    _wmax = wmax;

    rate = 0.35e-3f;
    sprite.setTextureRect(sf::IntRect(0, 5 * frameSize.y, frameSize.x, frameSize.y));
  }
}
void Animation::update(P_State pState){
  if(pState == P_State::RUNNING){
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

  if(timeSinceUpdate > rate){
    //timeSinceUpdate = 0;
    //std::cout << "Updating at time " << timeSinceUpdate << "\n";
    timeSinceUpdate -= rate;
    if(timeSinceUpdate > rate)
      timeSinceUpdate = 0;
    currentFrame = ( currentFrame >= _wmax - 1 ) ? 0 : currentFrame + 1;
    //std::cout << "Updating currentFrame  " << _wmax - 1 << "\t" << currentFrame << "\n";
  }
  sprite.setTextureRect(sf::IntRect(currentFrame * frameSize.x, (currentRow - 1) * frameSize.y , frameSize.x, frameSize.y));
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
  if (event.type == sf::Event::KeyPressed){
    pState = P_State::RUNNING;
    if(event.key.code == sf::Keyboard::Up){
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
    }

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
  load_assets();

  eventMgr = new EventManager();
  MenuContainer *menu = new MenuContainer();
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

  file->linkDropdown(dropdown);

  _windowItems.push_back(menu);
  //_windowItems.push_back(dropdown);

  for(auto item : menu->getClickables()){
    eventMgr->registerItem(item);
  }
  gameMap.load(TILE_SET, sf::Vector2u(16, 16));
  gameMap.addLayer(LAYER_1, 50, 50);
  gameMap.addLayer(LAYER_2, 50, 50);


  //add player here
  Player * link = new Player();
  link->animation.loadAsset(LINK_ASSET, link->sprite, 10, 8);
  addDrawable(link);

  camera.initCamera(_window->getSize(), sf::FloatRect(0, 0, 2000.0f, 2000.0f));
  camera.setTarget(link);

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

void WindowManager::draw(){
    _window->clear();
    //sf::RectangleShape bGround(sf::Vector2f(DEFAULT_WIDTH, DEFAULT_HEIGHT));
    //bGround.setFillColor(sf::Color::Blue);
    //bGround.setOutlineColor(sf::Color::Blue);
    //_window->draw(bGround);
    _window->setView(camera.view);
    /*if(scenary != nullptr){

      _window->draw(scenary->map);
    }*/

    std::vector<MapLayer*> levelLayers = gameMap.m_layers;
    if(levelLayers.size() > 0){
      _window->draw(*levelLayers[0]);
    }
    for(int i = 0; i < _windowItems.size(); i ++){
      _windowItems[i] -> draw(_window);
    }
    for(int i = 0; i < _itemsToDisplay.size(); i ++){
      _itemsToDisplay[i] -> draw(_window);
    }
    if(levelLayers.size() > 1){
      for(int i = 1; i < levelLayers.size(); i ++){
        _window->draw(*levelLayers[i]);
      }
    }

    _window->display();
}
void WindowManager::setScenery(Scenery * sc){
  scenary = sc;
}
void WindowManager::update(){
  float deltas = clock.getElapsedTime().asSeconds();
  //std::cout << "Printing frame rate " << 1 / deltas << " \n";
  for(int i = 0; i < _itemsToDisplay.size(); i ++){
    _itemsToDisplay[i] -> update(deltas);
  }
  camera.updateCamera(sf::FloatRect(0, 0, scenary->mapSize.x, scenary->mapSize.y));
}

void WindowManager::checkCollisions(){
  //
  int currentLayer = 1; // this info needs to be calculated dynamically
                        // based on player position

  for(int i = 0; i < _itemsToDisplay.size(); i ++){
    sf::FloatRect itemArea = _itemsToDisplay[i] -> getBounds();
    MapLayer *currentLevelLayer = gameMap.m_layers[currentLayer];
    for(auto item : currentLevelLayer->m_collidableItems){
      if(itemArea.intersects(*item)){
        _itemsToDisplay[i] -> onCollision();
      }
    }
  }
}

void WindowManager::notifyObjects(sf::Event event){
  for(int i = 0; i < _itemsToDisplay.size(); i ++){
    _itemsToDisplay[i] -> onEvent(event);
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
    wm->clearItems();
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

  Scenery scene;
  scene.loadMapFromFile(MAP_ASSET);
  wManager->setScenery(&scene);

  wManager->getWindow()->setVerticalSyncEnabled(true);
  //wManager->getWindow()->setFramerateLimit(60);
  while (wManager->getWindow()->isOpen())
  {
      sf::Event event;
      while (wManager->getWindow()->pollEvent(event))
      {
          wManager->eventMgr->notifyAll(event);
          wManager->notifyObjects(event);
          if (event.type == sf::Event::Closed)
              wManager->getWindow()->close();
      }
      wManager->update();
      wManager->checkCollisions();
      wManager->draw();

      wManager->clock.restart();
  }
  std::cout << "Exiting .. \n";
  return 0;
}
