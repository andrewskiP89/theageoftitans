#include "sfml.h"
#include "Object.h"
#include "EventManager.h"
#include "MapManager.h"

#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

class WindowManager {

private:
  std::vector<PDObject*> _windowItems;
  sf::RenderWindow *_window;
  std::vector<PDObject*> _itemsToDisplay;
  Scenery *scenary;
  static WindowManager *wm;
  Camera camera;

public:
  void setScenery(Scenery *sc);
  sf::Clock clock;
  static WindowManager * getManager();
  EventManager * eventMgr;
  WindowManager ();
  LevelMap gameMap;
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
