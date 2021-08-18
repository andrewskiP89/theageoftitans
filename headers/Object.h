#include "sfml.h"
#ifndef OBJECT_H
#define OBJECT_H
struct Sector {
  float size;
  uint8_t currentColumn;
  uint8_t currentRow;
};
enum P_State {
    NORMAL,
    RUNNING,
    DEAD
 };
enum HOrientation{
  UP, RIGHT, DOWN, LEFT
};
class PDObject{
public:
  virtual void draw(sf::RenderWindow * window);
  virtual void update(float deltas);
  virtual void onEvent(sf::Event event);
  virtual sf::Vector2f getPosition();
};

class Camera {
private:
  PDObject *target;
  sf::FloatRect horizon;
public:
  Camera();
  void setTarget(PDObject *trgt);
  void initCamera(sf::Vector2u size, sf::FloatRect horizon);
  void updateCamera(sf::FloatRect horizon);
  sf::View view;
};


class Scenery : public PDObject{
public:
  Scenery();
  bool loadMapFromFile(std::string mapSrc);
  sf::Vector2u mapSize;
  sf::Texture texture;
  sf::Sprite map;
};

class Clickable {
public:
  Clickable();
  virtual void onclick();
  virtual sf::FloatRect getClickableArea();
};
class Animation{
public:
  Animation();
  void loadAsset(std::string file_path, sf::Sprite &sprite, int wmax, int hmax);
  void update(P_State pState);
  void getCurrentFrame(float ds, sf::Sprite &sprite);
  sf::Texture texture;
  sf::Sprite sprite;
  sf::Vector2u frameSize;
  HOrientation orientation;
  int _hmax;
  int _wmax;
  float timeSinceUpdate;
  float rate;
  int currentFrame;
  int currentRow;
};
class Player : public PDObject{
public:
  Player();
  Animation animation;
  sf::Sprite sprite;
  void update(float deltas);
  void move(sf::Vector2f p);
  void draw(sf::RenderWindow * window);
  void onEvent(sf::Event event);
  sf::Vector2f getPosition();
private:
  P_State pState;
  sf::Vector2f speed;
  sf::Vector2f position;
};

#endif
