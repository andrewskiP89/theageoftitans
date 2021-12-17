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

/////////// INVENTORY MENU ///////////////
#define ITEMS_PER_ROW 5
#define PADDING 20
#define INVENTORY_X_OFFSET PADDING
#define INVENTORY_Y_OFFSET 0.7f * DEFAULT_HEIGHT
InventoryDrawable::InventoryDrawable(sf::Vertex *m_quad, sf::Texture *texture){
  std::cout << "Loading the texture\n";
  m_texture = * texture;
  m_vertices.setPrimitiveType(sf::Quads);
  m_vertices.resize(4);
  std::cout << "Loading the vertices " << m_quad  << "\n";
  std::cout << "Gen Map : Printing vertex 1 " << m_quad[0].texCoords.x << "\t" << m_quad[0].texCoords.y << "\n";
  std::cout << "Gen Map : Printing vertex 2 " << m_quad[1].texCoords.x << "\t" << m_quad[1].texCoords.y << "\n";
  std::cout << "Gen Map : Printing vertex 3 " << m_quad[2].texCoords.x << "\t" << m_quad[2].texCoords.y << "\n";
  std::cout << "Gen Map : Printing vertex 4 " << m_quad[3].texCoords.x << "\t" << m_quad[3].texCoords.y << "\n";
  sf::Vertex *vertex = &m_vertices[0];
  vertex[0].texCoords = m_quad[0].texCoords;
  vertex[1].texCoords = m_quad[1].texCoords;
  vertex[2].texCoords = m_quad[2].texCoords;
  vertex[3].texCoords = m_quad[3].texCoords;
  std::cout << "Loading the texture\n";
}

InventoryDrawable::~InventoryDrawable(){

}
void InventoryDrawable::draw(sf::RenderTarget& target, sf::RenderStates states) const{
  states.transform *= getTransform();
  //states.transform.scale(300, 300);
  // apply the tileset texture
  states.texture = &m_texture;

  target.draw(m_vertices, states);
}

Inventory::Inventory(){
  _menuBar = new sf::RectangleShape(sf::Vector2f( DEFAULT_WIDTH - 2 * PADDING,
                                                  DEFAULT_HEIGHT - INVENTORY_Y_OFFSET - PADDING));
  _menuBar -> setFillColor(sf::Color::Black);
  _menuBar -> setOutlineColor(sf::Color::White);
  _menuBar -> setOutlineThickness(4.0f);
  _menuBar->setPosition(INVENTORY_X_OFFSET, INVENTORY_Y_OFFSET);
  m_oldPosition = sf::Vector2f(0,0);
  m_cameraInited = false;
  _menuItems = {};
}
void Inventory::addItem(Clickable * item){

}
void Inventory::setCamera(Camera *camera){
  m_camera = camera;
  m_cameraInited = true;
}
void Inventory::update(){
  // convert window coordinates to locals
  sf::Vector2f cposition = m_camera->getView().getCenter() - sf::Vector2f(DEFAULT_WIDTH / 2, DEFAULT_HEIGHT / 2);
  if(cposition != m_oldPosition){
    sf::Vector2f delta = cposition - m_oldPosition;
    std::cout << "Updating inventory position " << delta.x << "\t" << delta.y <<"\n";
    _menuBar->setPosition(_menuBar->getPosition() + delta);
    for(auto item : _menuItems){
      item ->move(delta);
    }
    m_oldPosition = cposition;
  }

}
void Inventory::addItem(const ItemSource * source){

  InventoryItem *item =  new InventoryItem();
  item->loadVertexArray(source);
  uint16_t offsetX = INVENTORY_X_OFFSET, offsetY = INVENTORY_Y_OFFSET;
  uint16_t tu = _menuItems.size() % ITEMS_PER_ROW;
  uint16_t tv = _menuItems.size() / ITEMS_PER_ROW;
  //item->setScale(3.0f, 3.0f);
  item->setPosition(offsetX + item->m_size.x * tu, offsetY + item->m_size.y * tv);
  _menuItems.push_back(item);
  std::cout << "Inventory size " << _menuItems.size();
}

void Inventory::removeItem(Clickable *item){

}

void Inventory::draw(sf::RenderWindow * window){
  window->draw(*_menuBar);
  //std::cout << "Menu container position " << _menuBar->getPosition().x << "\t" << _menuBar->getPosition().y << "\n";
  for(auto item : _menuItems){
    InventoryItem *iItem =  (InventoryItem *) item;
    sf::Vertex *vertexToDraw = & (iItem->m_drawable->m_vertices[0]);
    sf::Vector2f vertex1 = vertexToDraw[0].texCoords;
    sf::Vector2f vertex2 = vertexToDraw[1].texCoords;
    sf::Vector2f vertex3 = vertexToDraw[2].texCoords;
    sf::Vector2f vertex4 = vertexToDraw[3].texCoords;

    window->draw(*(iItem->m_drawable));
  }
}
 // inventory item
InventoryItem::InventoryItem(){

}

InventoryItem::~InventoryItem(){

}
void InventoryItem::loadVertexArray(const ItemSource * source){
  size_t currentScale = 3.0f;
  m_size = sf::Vector2u(source-> size.x * currentScale, source-> size.y * currentScale );
  m_drawable = new InventoryDrawable(source->quad, source->texture);
  //m_drawable->setScale(m_drawable->getScale().x * currentScale, m_drawable->getScale().y * currentScale);
  std::cout << "Printing current scale " << m_drawable->getScale().x << "\t" << m_drawable->getScale().y << "\n";
  std::cout << "Printing current scale " << m_drawable->getScale().x << "\t" << m_drawable->getScale().y << "\n";
  //m_drawable->scale(3.0f, 3.0f);

}
void InventoryItem::onClick(){

}

sf::FloatRect InventoryItem::getClickableArea(){
  return m_clickableArea;
}

void InventoryItem::move(sf::Vector2f delta){
  sf::Vertex *vertex = &( m_drawable->m_vertices[0] );
  vertex[0].position += delta;
  vertex[1].position += delta;
  vertex[2].position += delta;
  vertex[3].position += delta;
}

void InventoryItem::setPosition(float x, float y){
  m_clickableArea = sf::FloatRect(x,y,m_size.x,m_size.y);
  sf::Vertex *vertex = &( m_drawable->m_vertices[0] );
  vertex[0].position = sf::Vector2f(x, y);
  vertex[1].position = sf::Vector2f(x + m_size.x, y);
  vertex[2].position = sf::Vector2f(x + m_size.x, y + m_size.y);
  vertex[3].position = sf::Vector2f(x, y + m_size.y);

  std::cout << "Printing position 1 " << vertex[0].position.x << "\t" << vertex[0].position.y << "\n";
  std::cout << "Printing position 2 " << vertex[1].position.x << "\t" << vertex[1].position.y << "\n";
  std::cout << "Printing position 3 " << vertex[2].position.x << "\t" << vertex[2].position.y << "\n";
  std::cout << "Printing position 4 " << vertex[3].position.x << "\t" << vertex[3].position.y << "\n";

}
