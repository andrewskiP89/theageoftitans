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

#define MAP_FLDR "./assets/levelmap/"
#define TEXTURE_FLDR "./assets/imgs/"

//typedef std::vector<uint16_t> LayerData;

struct LayerData {
  sf::Vector2u size;
  std::vector<uint16_t> data;
};

struct Tile {
  uint16_t id;
  std::string type;
  bool isCollidable;
  bool isCollectable;
  std::vector<std::string> events;
};

struct TileSet {
  sf::Texture texture;
  sf::Vector2u size;
  uint16_t offset;
  std::map<uint16_t,Tile> tiles;
};

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
  MapLayer(uint8_t currentLayer, size_t layerNumber);
  void setTileset(sf::Texture * tileSet, sf::Vector2u tileSize); // to be deprecated
  void setTileset(const TileSet& tileSet); // new method
  void generateLayer(const std::string& mapFile, const unsigned int width, const unsigned int height, const float mapScale); // to be deprecated
  void generateLayer(const LayerData& data, const float mapScale);// new method
  std::vector<sf::FloatRect*> m_collidableItems;
  std::map<sf::FloatRect*, std::vector<AppEvent>> m_eventAreas;
  std::vector<WorldItem*> m_worldItems;

private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  sf::Vector2u m_tileSize;
  sf::VertexArray m_vertices;
  uint8_t m_currentLayer;
  size_t m_layerNumber;
  bool m_isCollidable;
  Camera *camera; // to manage rendering optimization
  sf::Texture * m_tileset;
  TileSet m_tilesetNew;
};

class LevelMap {
public:
  bool load(const std::string& tileSet, sf::Vector2u tileSize);
  bool loadWorld(const std::string& tileSet, sf::Vector2u tileSize);
  void addLayer(const std::string& mapFile, unsigned int width, unsigned int height);
  void addLayer(const LayerData& data, const TileSet& tileSet, uint8_t& currentLayer);
  bool loadLevel(const size_t levelNumber);
  std::vector<MapLayer*> m_layers; // will be replaced by m_layerMap
  std::map<uint8_t, std::vector<MapLayer*>> m_layerMap;
private:
  float m_scaleFactor;
  size_t m_lCounter;
  sf::Vector2u m_tileSize;
  sf::Texture m_tileset;
  std::map<std::string, TileSet*> m_tilesetMap;
  sf::Texture m_worldTileset;
  sf::Vector2u m_worldTileSize;
};

// struct LayerWrapper{
//
// };
#endif
