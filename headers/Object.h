#include "sfml.h"
#ifndef OBJECT_H
#define OBJECT_H

#define APP_FONT "./assets/fonts/dos_font.ttf"
#define APP_FONT_SIZE 35
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
  virtual void draw(sf::RenderWindow * window);
  virtual void update(float deltas);
  virtual void onEvent(sf::Event event);
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

class TextContainer : public PDObject{
public:
  TextContainer(){}
  void init(sf::Vector2f anchorPoint, float width, float height){
    m_currentText = nullptr;
    if(m_appFont.loadFromFile(APP_FONT)){
      float padding = 5.0f;
      // building the text container
      m_anchorPoint = sf::Vector2f(anchorPoint.x + padding, anchorPoint.y + padding);
      sf::RectangleShape rectShape(sf::Vector2f(width - 2 * padding,
                                                height - 2 * padding));
      rectShape.setFillColor(sf::Color::Black);
      rectShape.setPosition(m_anchorPoint);
      rectShape.setOutlineColor(sf::Color::Black);
      rectShape.setOutlineThickness(1.0f);
      m_textBox.push_back(rectShape);
    }
  }
  void update(float deltas){
    std::string currentText = getCurrentText();
    if(m_currentText == nullptr){
      m_currentText = new sf::Text(currentText, m_appFont, APP_FONT_SIZE);
      m_currentText->setOutlineColor(sf::Color::White);
      float margin = 5.0f;
      m_currentText->setPosition(sf::Vector2f(m_anchorPoint.x + margin, m_anchorPoint.y));
    }else{
      m_currentText->setString(currentText);
    }
  }
  void draw(sf::RenderWindow * window){
    for(auto item : m_textBox){
      window->draw(item);
    }
    window->draw(*m_currentText);
  }
private:
  std::string getCurrentText(){
    std::string currentText = "This is a sample text";
    return currentText;
  }
  std::vector<sf::RectangleShape> m_textBox;
  sf::Font m_appFont;
  sf::Text *m_currentText;
  sf::Vector2f m_anchorPoint;
};
#endif
