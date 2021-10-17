#include "../headers/MapManager.h"
#include "../headers/WindowManager.h"


MapLayer::MapLayer(uint8_t currentLayer){
  m_currentLayer = currentLayer;
}
void MapLayer::setTileset(sf::Texture * tileSet, sf::Vector2u tileSize){
  m_tileSize = tileSize;
  m_tileset = tileSet;

}

void MapLayer::setTileset(const TileSet& tileSet){
  m_tilesetNew = tileSet;
}

void MapLayer::generateLayer(const LayerData& data, const float mapScale){
  m_vertices.setPrimitiveType(sf::Quads);
  m_vertices.resize(data.size.x * data.size.y * 4); // 4 is the number of
                                                    // of vertex for each quads

  uint16_t totalColumns = data.size.x;
  for(uint16_t i = 0; i < data.data.size(); i++){
    uint16_t column = i % totalColumns;
    uint16_t row = i / totalColumns;

    uint16_t tileNumber = data.data[i] - m_tilesetNew.offset;
    if(tileNumber == -1)
      continue;
    // The world map events need to be defined
    // at level configuration
    EventCoordinate coordinate;
    coordinate.currentLayer   = m_currentLayer;
    coordinate.currentLevel   = 1; // @TODO manage levels
    coordinate.currentRow     = row;
    coordinate.currentColumn  = column;

    int tu = tileNumber % (m_tilesetNew.texture.getSize().x / m_tilesetNew.size.x);
    int tv = tileNumber / (m_tilesetNew.texture.getSize().x / m_tilesetNew.size.x);

    sf::Vertex* quad = &m_vertices[(column + row * data.size.x) * 4];
    quad[0].position = sf::Vector2f(column * m_tilesetNew.size.x, row * m_tilesetNew.size.y);
    quad[1].position = sf::Vector2f((column + 1) * m_tilesetNew.size.x, row * m_tilesetNew.size.y);
    quad[2].position = sf::Vector2f((column + 1) * m_tilesetNew.size.x, (row + 1) * m_tilesetNew.size.y);
    quad[3].position = sf::Vector2f(column * m_tilesetNew.size.x, (row + 1) * m_tilesetNew.size.y);

    unsigned int tolerance = 50;
    bool isCollidable = false, isCollectable = false;
    if(m_tilesetNew.tiles.find(tileNumber) != m_tilesetNew.tiles.end()){
      isCollidable = m_tilesetNew.tiles[tileNumber].isCollidable;
      isCollectable = m_tilesetNew.tiles[tileNumber].isCollectable;
    }

    if(isCollidable){
      sf::FloatRect *r  = new sf::FloatRect(column * m_tilesetNew.size.x * mapScale + tolerance,
                                            row * m_tilesetNew.size.y * mapScale + tolerance,
                                            m_tilesetNew.size.x * mapScale - (2 * tolerance),
                                            m_tilesetNew.size.y * mapScale - (2 * tolerance));
      m_collidableItems.push_back(r);
    }

    std::map<TriggerType, std::vector<AppEvent>> appEventMap = EventManager::loadEventsFromMap(coordinate, WORLD_EVENT_MAP);

    if(appEventMap.find(TriggerType::OnCollision) != appEventMap.end()){
      sf::FloatRect *r  = new sf::FloatRect(column * m_tilesetNew.size.x * mapScale,
                                            row * m_tilesetNew.size.y * mapScale,
                                            m_tilesetNew.size.x * mapScale,
                                            m_tilesetNew.size.y * mapScale);

      m_eventAreas[r] =  appEventMap.find(TriggerType::OnCollision)->second;

    }else if(appEventMap.find(TriggerType::OnClick) != appEventMap.end()){
      sf::FloatRect *r  = new sf::FloatRect(column * m_tilesetNew.size.x * mapScale,
                                            row * m_tilesetNew.size.y * mapScale,
                                            m_tilesetNew.size.x * mapScale,
                                            m_tilesetNew.size.y * mapScale);

      std::vector<AppEvent> clickEvents =  appEventMap.find(TriggerType::OnClick)->second;
      WorldItem *wi = new WorldItem(r);
      wi->m_events = clickEvents;
      m_worldItems.push_back(wi);
    }
    // define its 4 texture coordinates
    quad[0].texCoords = sf::Vector2f(tu * m_tilesetNew.size.x, tv * m_tilesetNew.size.y);
    quad[1].texCoords = sf::Vector2f((tu + 1) * m_tilesetNew.size.x, tv * m_tilesetNew.size.y);
    quad[2].texCoords = sf::Vector2f((tu + 1) * m_tilesetNew.size.x, (tv + 1) * m_tilesetNew.size.y);
    quad[3].texCoords = sf::Vector2f(tu * m_tilesetNew.size.x, (tv + 1) * m_tilesetNew.size.y);

  }
}
void MapLayer::generateLayer(const std::string& mapFile, const unsigned int width, const unsigned int height, const float mapScale){

  std::vector<int> collidableItemsInMap = {
    120, 121, 124, 125, 44, 45, 206, 207, 166, 167
  };

  m_vertices.setPrimitiveType(sf::Quads);
  unsigned int scaleFactor = 4;
  m_vertices.resize(width * height * scaleFactor);
  std::cout << "Printing scale factor " << scaleFactor << "\n";
  std::string line;
  std::ifstream mFile(mapFile);
  unsigned int i = 0;

  while (std::getline(mFile, line)) {
    std::vector<std::string> rowItems;
    boost::algorithm::split(rowItems, line, boost::is_any_of(","));
    for(int j = 0; j < rowItems.size(); j ++){
      bool isCollidable = false;
      std::string itemString = rowItems[j];
      int tileNumber = std::stoi(itemString);

      if(tileNumber == -1)
        continue;

      EventCoordinate coordinate;
      coordinate.currentLayer   = m_currentLayer;
      coordinate.currentLevel   = 1; // @TODO manage levels
      coordinate.currentRow     = i;
      coordinate.currentColumn  = j;

      if (std::find(collidableItemsInMap.begin(), collidableItemsInMap.end(), tileNumber) != collidableItemsInMap.end()) {
        isCollidable = true;
      }
      int tu = tileNumber % (m_tileset->getSize().x / m_tileSize.x);
      int tv = tileNumber / (m_tileset->getSize().x / m_tileSize.x);

      sf::Vertex* quad = &m_vertices[(j + i * width) * 4];

      quad[0].position = sf::Vector2f(j * m_tileSize.x , i * m_tileSize.y);
      quad[1].position = sf::Vector2f((j + 1) * m_tileSize.x, i * m_tileSize.y);
      quad[2].position = sf::Vector2f((j + 1) * m_tileSize.x, (i + 1) * m_tileSize.y);
      quad[3].position = sf::Vector2f(j * m_tileSize.x, (i + 1) * m_tileSize.y);

      unsigned int tolerance = 50;
      if(isCollidable){
        sf::FloatRect *r  = new sf::FloatRect(j * m_tileSize.x * mapScale + tolerance,
                                              i * m_tileSize.y * mapScale + tolerance,
                                              m_tileSize.x * mapScale - (2 * tolerance),
                                              m_tileSize.y * mapScale - (2 * tolerance));
        m_collidableItems.push_back(r);
      }


      std::map<TriggerType, std::vector<AppEvent>> appEventMap = EventManager::loadEventsFromMap(coordinate, WORLD_EVENT_MAP);

      if(appEventMap.find(TriggerType::OnCollision) != appEventMap.end()){
        sf::FloatRect *r  = new sf::FloatRect(j * m_tileSize.x * mapScale,
                                              i * m_tileSize.y * mapScale,
                                              m_tileSize.x * mapScale,
                                              m_tileSize.y * mapScale);

        m_eventAreas[r] =  appEventMap.find(TriggerType::OnCollision)->second;


      }else if(appEventMap.find(TriggerType::OnClick) != appEventMap.end()){
        sf::FloatRect *r  = new sf::FloatRect(j * m_tileSize.x * mapScale,
                                              i * m_tileSize.y * mapScale,
                                              m_tileSize.x * mapScale,
                                              m_tileSize.y * mapScale);

        std::vector<AppEvent> clickEvents =  appEventMap.find(TriggerType::OnClick)->second;
        WorldItem *wi = new WorldItem(r);
        wi->m_events = clickEvents;
        m_worldItems.push_back(wi);
      }

      // define its 4 texture coordinates
      quad[0].texCoords = sf::Vector2f(tu * m_tileSize.x, tv * m_tileSize.y);
      quad[1].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x, tv * m_tileSize.y);
      quad[2].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x, (tv + 1) * m_tileSize.y);
      quad[3].texCoords = sf::Vector2f(tu * m_tileSize.x, (tv + 1) * m_tileSize.y);
    }
    i ++;
  }
  std::cout << "Printing collision event map size: " << m_eventAreas.size() << "\n";
  std::cout << "Printing world clickcable events size: " << m_worldItems.size() << "\n";
}


// ml privates
void MapLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = & (m_tilesetNew.texture);
    // following commented code has been deprecated
    //states.texture = m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
}

bool LevelMap::load(const std::string& tileSet, sf::Vector2u tileSize){
  if (!m_tileset.loadFromFile(tileSet))
          return false;
  m_tileSize = tileSize;
  m_scaleFactor =  5.0f;
  return true;
}

bool LevelMap::loadWorld(const std::string& tileSet, sf::Vector2u tileSize){
  if (!m_worldTileset.loadFromFile(tileSet))
          return false;
  m_worldTileSize = tileSize;
  return true;
}
// new version to load level info from a
// JSON map
bool LevelMap::loadLevel(const size_t levelNumber){
  m_tilesetMap = {};
  m_layerMap = {};

  assert(levelNumber != 0);
  std::string prefix = MAP_FLDR ;
  std::string folder_path = TEXTURE_FLDR;

  std::string levelName = prefix + "Level_" + std::to_string(levelNumber) + ".json";
  std::cout << "Printing file  to open  " << levelName << "\n";
  std::ifstream lfile(levelName);
  nlohmann::json levelContent;
  lfile >> levelContent;
  lfile.close();

  size_t tilewidth, tileheight;
  tilewidth = levelContent["tilewidth"].get<size_t>();
  tileheight = levelContent["tileheight"].get<size_t>();
  m_tileSize = sf::Vector2u(tilewidth, tileheight);
  m_scaleFactor = 5.0f; // @TODO put this value in the configuration file
  // LOADING TILESET FROM MAP FILE
  for(auto item : levelContent["tilesets"]){
    std::string filename = item["source"].get<std::string>();
    std::string tsPath = prefix + filename;
    std::cout << "Adding tileset  " << tsPath << "\n";
    if(m_tilesetMap.find(filename) == m_tilesetMap.end()){

      std::cout << "Opening file " << tsPath << "\n";
      nlohmann::json tsContent;
      std::ifstream tsFile(tsPath);
      tsFile >> tsContent;
      tsFile.close();

      TileSet *ts = new TileSet();
      std::string tsImagePath = folder_path + tsContent["image"].get<std::string>();
      if(!ts->texture.loadFromFile(tsImagePath)){
        std::cout << "Unable to open tileset image path " << tsImagePath << "\n";
        return false;
      }
      uint16_t tsWidth = tsContent["tilewidth"].get<uint16_t>();
      uint16_t tsHeight = tsContent["tileheight"].get<uint16_t>();
      ts->offset = item["firstgid"].get<uint16_t>();
      ts->size = sf::Vector2u(tsWidth,tsHeight);
      ts->tiles = {};
      for (auto jsonTile : tsContent["tiles"]){
        Tile tile;
        tile.id = jsonTile["id"].get<uint16_t>();
        tile.type = jsonTile["type"].get<std::string>();

        for(auto jsonProp : jsonTile["properties"]){
          //Property prop;
          std::string pName = jsonProp["name"];
          if (pName == "isCollidable") {
            tile.isCollidable = true;
          } else if (pName == "isCollectable") {
            tile.isCollectable = true;
          }
        }
        ts->tiles[tile.id] = tile;
      }

      m_tilesetMap[filename] = ts;
    }
  }
  std::cout << "Printing tileset map size: "  << m_tilesetMap.size() << " elements.\n";
  for(auto item : levelContent["layers"]){
    LayerData lData;
    lData.data = item["data"].get<std::vector<uint16_t>>();
    uint16_t dataWidth = item["width"];
    uint16_t dataHeight = item["height"];
    lData.size = sf::Vector2u(dataWidth, dataHeight);
    uint8_t currentLayer = -1;
    std::string currentTs = "NOT_INITED";
    for(auto lProp : item["properties"]){
      std::string pName = lProp["name"];
      std::cout << "Printing map properties for " << pName << "\n";
      if (pName == "currentLayer") {
        currentLayer = lProp["value"];
      } else if (pName == "tileSet") {
        currentTs = lProp["value"];
      }
    }
    assert( currentLayer != -1);
    assert( currentTs != "NOT_INITED");

    addLayer(lData, *m_tilesetMap[currentTs], currentLayer);

  }
  std::cout << "Printing tile size " << tilewidth << "x" << tileheight << "\n";
  return false;
}
void LevelMap::addLayer(const LayerData& data, const TileSet& tileSet, uint8_t& currentLayer){
  MapLayer *ml = new MapLayer(currentLayer);
  // add logic to correctly generate the layer

  ml->scale(m_scaleFactor, m_scaleFactor);
  ml->setTileset(tileSet);
  ml->generateLayer(data, m_scaleFactor);
  if(m_layerMap.find(currentLayer) == m_layerMap.end()){
    m_layerMap[currentLayer] = {};
  }
  m_layerMap[currentLayer].push_back(ml);
}

void LevelMap::addLayer(const std::string& mapFile, unsigned int width, unsigned int height){
  MapLayer *ml = new MapLayer(m_layers.size());
  //float scaleFactor = 5.0f;
  ml->scale(m_scaleFactor, m_scaleFactor);
  ml->setTileset(&m_tileset, m_tileSize);
  ml->generateLayer(mapFile, width, height, m_scaleFactor);
  m_layers.push_back(ml);
}

/// WORLD ITEMS ///

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
