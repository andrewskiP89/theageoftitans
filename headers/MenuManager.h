
#include "sfml.h"
#include "Object.h"

#ifndef MENU_H
#define MENU_H

#define NEW_GAME_LABEL "New Game"
#define CONTINUE_LABEL "Continue"
#define QUIT_LABEL "Quit"

enum ActionType { show_menu, execute};

class MenuItem : public Clickable{
private:
  std::string _menuText;

  ActionType _atype;
  bool _isRoot;
public:
  sf::Text _label;
  sf::FloatRect m_clickableArea;
  void *_md;
  sf::Text getLabel();
  void linkDropdown(void *md);
  void onclick();
  sf::FloatRect getClickableArea();
  void setPosition(float x, float y);
  MenuItem (const std::string menuText, bool isRoot = false);
};

class MenuContainer : public PDObject{
protected:
  sf::RectangleShape *_menuBar;
  std::vector<MenuItem*> _menuItems;
public:
  MenuContainer ();
  virtual void addMenuItem(MenuItem * item);
  virtual void draw(sf::RenderWindow * window);
  std::vector<MenuItem*> getClickables();
  ~MenuContainer ();
};

class ActionMenu : public MenuContainer{
public:
  ActionMenu ();
  void setCamera(const Camera &camera);
  void addMenuItem(MenuItem * item) override;
  void update(float deltas) override;
  void onEvent(sf::Event event) override;
  std::vector<AppEvent> m_loadedEvents;
private:
  bool m_cameraInited;
  sf::Vector2f m_oldPosition;
  Camera m_camera;
};

class StartingMenu : public MenuContainer{
public:
  StartingMenu();
  void addMenuItem(MenuItem* item) override;
  //void update(float deltas) override;
  //void onEvent(sf::Event event) override;
};

class MenuDropdown :  public PDObject{
private:
  sf::Vector2f _anchorPoint;
  sf::RectangleShape *_menuBar;
  std::vector<MenuItem*> _menuItems;
public:
  MenuDropdown (MenuItem &item);
  void addMenuItem(MenuItem * item);
  void draw(sf::RenderWindow * window);
  std::vector<MenuItem*> getClickables();
  ~MenuDropdown ();
};



#endif
