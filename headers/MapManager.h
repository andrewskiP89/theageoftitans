#include "sfml.h"
#include "Object.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <boost/algorithm/string.hpp>


#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H


class MapLayer : public sf::Drawable, public sf::Transformable
{
public:
  void setTileset(sf::Texture * tileSet, sf::Vector2u tileSize){
    m_tileSize = tileSize;
    m_tileset = tileSet;
  }
  void generateLayer(const std::string& mapFile, unsigned int width, unsigned int height){
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
        std::string itemString = rowItems[j];
        int tileNumber = std::stoi(itemString);
        if(tileNumber == -1) continue;

        int tu = tileNumber % (m_tileset->getSize().x / m_tileSize.x);
        int tv = tileNumber / (m_tileset->getSize().x / m_tileSize.x);

        sf::Vertex* quad = &m_vertices[(i + j * width) * scaleFactor];

        // define its 4 corners
        /*quad[0].position = sf::Vector2f(i * m_tileSize.x , j * m_tileSize.y);
        quad[1].position = sf::Vector2f((i + 1) * m_tileSize.x, j * m_tileSize.y);
        quad[2].position = sf::Vector2f((i + 1) * m_tileSize.x, (j + 1) * m_tileSize.y);
        quad[3].position = sf::Vector2f(i * m_tileSize.x, (j + 1) * m_tileSize.y);*/

        quad[0].position = sf::Vector2f(j * m_tileSize.x , i * m_tileSize.y);
        quad[1].position = sf::Vector2f((j + 1) * m_tileSize.x, i * m_tileSize.y);
        quad[2].position = sf::Vector2f((j + 1) * m_tileSize.x, (i + 1) * m_tileSize.y);
        quad[3].position = sf::Vector2f(j * m_tileSize.x, (i + 1) * m_tileSize.y);

        // define its 4 texture coordinates
        quad[0].texCoords = sf::Vector2f(tu * m_tileSize.x, tv * m_tileSize.y);
        quad[1].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x, tv * m_tileSize.y);
        quad[2].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x, (tv + 1) * m_tileSize.y);
        quad[3].texCoords = sf::Vector2f(tu * m_tileSize.x, (tv + 1) * m_tileSize.y);
      }
      i ++;
    }
  }
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
    sf::Texture * m_tileset;
};

class LevelMap {
public:
  bool load(const std::string& tileSet, sf::Vector2u tileSize){
    if (!m_tileset.loadFromFile(tileSet))
            return false;
    m_tileSize = tileSize;
    return true;
  }
  void addLayer(const std::string& mapFile, unsigned int width, unsigned int height){
    MapLayer *ml = new MapLayer();
    float scaleFactor = 5.0f;
    ml->scale(scaleFactor, scaleFactor);
    ml->setTileset(&m_tileset, m_tileSize);
    ml->generateLayer(mapFile, width, height);
    m_layers.push_back(ml);
  }
  std::vector<MapLayer*> m_layers;
private:
  sf::Vector2u m_tileSize;
  sf::Texture m_tileset;
};
#endif
