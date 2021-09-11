#pragma once

#include "sfml.h"
#include "Object.h"
//#include "EventManager.h"
#include "MapManager.h"
#include "MusicManager.h"
#include "MenuManager.h"

#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

class TextContainer : public PDObject{
public:
  void setDialog(Dialog dialog);
  void init(Camera * camera, float width, float height);
  void setCamera(Camera * camera);
  void onSFEvent(sf::Event event);
  void update(float deltas);
  void draw(sf::RenderWindow * window);
private:
  std::string getCurrentText();
  std::vector<sf::RectangleShape*> m_textBox;
  AppEvent *relatedEvent;
  sf::Font m_appFont;
  sf::Text *m_currentText;
  sf::Vector2f m_anchorPoint;
  float m_padding;
  Camera * m_camera;
  bool m_dialogIsSet;
  Dialog m_currentDialog;
  uint8_t m_currentMessage;
};

class WindowManager {

private:
  std::vector<PDObject*> _windowItems;
  sf::RenderWindow *_window;
  std::vector<PDObject*> _itemsToDisplay;
  Player *m_currentPlayer;
  Scenery *scenary;
  static WindowManager *wm;

  uint16_t m_currentTask; // il programma recupera sempre gli eventi corrispondenti
                          // a task immediatamente maggiore a task corrente del gioco. In questo modo gestiamo
                          // progress della storia. Il completamento di un task sara' indivudato dall'esecuzione di
                          // un evento di tipo EventType::TaskCompleted

public:
  Camera camera;
  GameState m_gameState;
  TextContainer m_textContainer;
  ActionMenu m_actionMenu;
  MusicManager m_musicMgr;
  void setScenery(Scenery *sc);
  sf::Clock clock;
  static WindowManager * getManager();
  EventManager * eventMgr;
  WindowManager ();
  LevelMap gameMap;
  void manageEvents();
  void checkCollisions();
  void notifyObjects(sf::Event event);
  bool init();
  void clearItems();
  void addDrawable(PDObject* obj);
  void removeDrawable();
  virtual ~WindowManager ();
  sf::RenderWindow* getWindow();
  void draw();
  void update();
};


#endif
