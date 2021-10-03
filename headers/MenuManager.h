
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
  sf::FloatRect m_clickableArea;
  void *_md;
  sf::Text getLabel() override;
  void linkDropdown(void *md);
  void onclick();
  sf::FloatRect getClickableArea();
  void setPosition(float x, float y);
  MenuItem (const std::string menuText, bool isRoot = false);
};

class MenuContainer : public PDObject{
protected:
  sf::RectangleShape *_menuBar;
  std::vector<Clickable*> _menuItems;
public:
  MenuContainer ();
  virtual void addMenuItem(MenuItem * item);
  virtual void draw(sf::RenderWindow * window);
  std::vector<Clickable*> getClickables();
  ~MenuContainer ();
};

class InventoryItem : public Clickable{
private:
  AppEvent m_action;
  sf::Sprite *m_sprite;
public:
  InventoryItem(sf::Sprite * sprite, const std::string labelString);
  sf::FloatRect m_clickableArea;
  void onClick();
  sf::FloatRect getClickableArea();
  void setPosition(float x, float y);
};

class Inventory : public MenuContainer{
protected:
  std::vector<Clickable*> _menuItems;
public:
  void addItem(Clickable *item);
  void removeItem(Clickable *item);
  std::vector<Clickable*> getClickables();
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
  std::vector<Clickable*> _menuItems;
public:
  MenuDropdown (MenuItem &item);
  void addMenuItem(MenuItem * item);
  void draw(sf::RenderWindow * window);
  std::vector<Clickable*> getClickables();
  ~MenuDropdown ();
};



#endif
