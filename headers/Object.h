#include "sfml.h"

#ifndef OBJECT_H
#define OBJECT_H

// @TODO all definitions to be moved to a dedicated header
#define PB_DASHBOARD_TITLE "The age of Titans"
#define DEFAULT_WIDTH 1600
#define DEFAULT_HEIGHT 900
#define MENU_FONT_SIZE 35

#define SRC_FLDR "./assets/fonts/dos_font.ttf"
#define LINK_SPEED 4.0e2f
#define ANIMATION_PERIOD 0.7e-1f
#define LINK_ASSET "./assets/imgs/link_new.png"

#define MAP_ASSET "./assets/imgs/worldmap.png"

#define TILE_SET "./assets/imgs/titanstileset.png"
//#define LAYER_1 "./assets/levelmap/titans_level1_layer1.csv"
//#define LAYER_2 "./assets/levelmap/titans_level1_layer2.csv"

#define LAYER_1 "./assets/levelmap/TAOTMap_LevelGround.csv"
#define LAYER_2 "./assets/levelmap/TAOTMap_WorldLayer.csv"

#define APP_FONT "./assets/fonts/dos_font.ttf"
#define APP_FONT_SIZE 35

enum GameState { Starting,
                  Playing,
                  Pause,
                  OnActionMenu,
                  OnMenu,
                  OnDialog
                  };

enum TriggerType {
  OnClick,
  OnCollision
};

enum EventType{
  OnAction,
  GameStateChange,
  ShowMessage,
  TaskCompleted
};
enum PActionType{
  Pull,
  Push,
  Use,
  PickUp,
  Throw,
  Study,
  None
};
struct UsableItem{
  uint16_t id;
};
struct Inventary{
  std::vector<UsableItem*> items;
};
struct PlayerAction{
  UsableItem *item;
  PActionType type;
};
struct Message{
  Message(){
    //sourcePlayer = nullptr;
    from = "";
  };
  std::string content;
  std::string from;
  //Player *sourcePlayer;
};
struct Dialog{
  std::vector<Message> messages;
};
struct AppEvent{
  std::string id;
  void *source;
  GameState targetState;
  int requiredTask;
  uint16_t taskCompleted;
  Dialog dialog;
  PlayerAction action;
  EventType type;
  TriggerType triggerType = TriggerType::OnCollision;
  bool singleTime;
  void fire();
};

struct EventCoordinate {
  uint8_t currentLayer;
  uint8_t currentLevel;

  uint8_t currentRow;
  uint8_t currentColumn;
};

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
  sf::Sprite sprite;
  size_t m_zLayer;
  virtual void draw(sf::RenderWindow * window);
  virtual void update(float deltas);
  virtual void onEvent(sf::Event event); // sf event needs to be updated
  virtual void onAppEvent(AppEvent event);
  virtual void onCollision();
  sf::FloatRect getBounds(){
    return sprite.getGlobalBounds();
  }
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
  sf::View getView() const{
    return view;
  }
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
  bool m_isLocalItem = true;
  bool const isLocal();
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
private:
  float m_rateFactor;
};
class Player : public PDObject{
public:
  Player();
  Animation animation;
  //sf::Sprite sprite;
  void update(float deltas);
  void move(sf::Vector2f p);
  void draw(sf::RenderWindow * window);
  void onEvent(sf::Event event);
  void onCollision();
  sf::Vector2f getPosition();
private:
  P_State pState;
  sf::Vector2f speed;
  sf::Vector2f m_position;
  sf::Vector2f m_oldPosition;
};

#endif
