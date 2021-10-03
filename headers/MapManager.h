#include "sfml.h"
#include "Object.h"
#include "EventsDefinition.h"
#include "EventManager.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <boost/algorithm/string.hpp>


#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

extern std::map<std::string, std::vector<std::string>> WORLD_EVENT_MAP;
extern std::map<std::string, std::vector<std::string>> TILE_SET_EVENT_MAP;

class WorldItem : public Clickable{
public:
  WorldItem(sf::FloatRect *area);
  virtual void onclick() override;
  virtual sf::FloatRect getClickableArea() override;
  std::vector<AppEvent> m_events;
private:
  sf::FloatRect *m_area;
};

class MapLayer : public sf::Drawable, public sf::Transformable
{
public:
  MapLayer(uint8_t currentLayer){
    m_currentLayer = currentLayer;
  }
  void setTileset(sf::Texture * tileSet, sf::Vector2u tileSize){
    m_tileSize = tileSize;
    m_tileset = tileSet;

  }
  void generateLayer(const std::string& mapFile, unsigned int width, unsigned int height, float mapScale){

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

        sf::Vertex* quad = &m_vertices[(i + j * width) * scaleFactor];

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
  std::vector<sf::FloatRect*> m_collidableItems;
  std::map<sf::FloatRect*, std::vector<AppEvent>> m_eventAreas;
  std::vector<WorldItem*> m_worldItems;

private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }
    sf::Vector2u m_tileSize;
    sf::VertexArray m_vertices;
    uint8_t m_currentLayer;
    bool m_isCollidable;
    Camera *camera; // to manage rendering optimization
    sf::Texture * m_tileset;
};

class LevelMap {
public:
  bool load(const std::string& tileSet, sf::Vector2u tileSize){
    if (!m_tileset.loadFromFile(tileSet))
            return false;
    m_tileSize = tileSize;
    m_scaleFactor =  5.0f;
    return true;
  }

  bool loadWorld(const std::string& tileSet, sf::Vector2u tileSize){
    if (!m_worldTileset.loadFromFile(tileSet))
            return false;
    m_worldTileSize = tileSize;
    return true;
  }

  void addLayer(const std::string& mapFile, unsigned int width, unsigned int height){
    MapLayer *ml = new MapLayer(m_layers.size());
    //float scaleFactor = 5.0f;
    ml->scale(m_scaleFactor, m_scaleFactor);
    ml->setTileset(&m_tileset, m_tileSize);
    ml->generateLayer(mapFile, width, height, m_scaleFactor);
    m_layers.push_back(ml);
  }
  std::vector<MapLayer*> m_layers;
private:
  float m_scaleFactor;
  sf::Vector2u m_tileSize;
  sf::Texture m_tileset;
  sf::Texture m_worldTileset;
  sf::Vector2u m_worldTileSize;
};
#endif
