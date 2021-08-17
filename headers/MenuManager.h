#include "sfml.h"
#include "Object.h"

#ifndef MENU_H
#define MENU_H

enum ActionType { show_menu, execute};

class MenuItem : public Clickable{
private:
  std::string _menuText;

  ActionType _atype;
  bool _isRoot;
public:
  sf::Text _label;
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
  void addMenuItem(MenuItem * item);
  void draw(sf::RenderWindow * window);
  std::vector<MenuItem*> getClickables();
  ~MenuContainer ();
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
