#include "../headers/WindowManager.h"

void TextContainer::setDialog(Dialog dialog){
  if(m_dialogIsSet == true) // are we sure to keep it?
    return;

  m_currentDialog = dialog;
  m_currentMessage = 0;
  m_dialogIsSet = true;
}

void TextContainer::init(Camera * camera, float width, float height){
  m_currentText = nullptr;
  m_dialogIsSet = false;
  if(m_appFont.loadFromFile(APP_FONT)){
    m_padding = 5.0f;
    // building the text container
    m_camera = camera;
    sf::View currentView = camera->getView();

    m_anchorPoint = sf::Vector2f(currentView.getCenter().x - (currentView.getSize().x /2 ) + m_padding,
                                  currentView.getCenter().y - (currentView.getSize().y /2 ) + m_padding);


    sf::RectangleShape *rectShape = new sf::RectangleShape(sf::Vector2f(width - 2 * m_padding,
                                              height - 2 * m_padding));
    rectShape->setFillColor(sf::Color::Black);
    rectShape->setPosition(m_anchorPoint.x, m_anchorPoint.y);
    rectShape->setOutlineColor(sf::Color::Black);
    rectShape->setOutlineThickness(1.0f);
    m_textBox.push_back(rectShape);
  }
}
void TextContainer::setCamera(Camera * camera){
  m_camera = camera;
}
void TextContainer::onSFEvent(sf::Event event) {
  if (event.type == sf::Event::KeyPressed){
    if(event.key.code == sf::Keyboard::Escape){
      m_currentMessage ++;
      if(m_currentMessage == m_currentDialog.messages.size()){
        m_dialogIsSet = false;
        AppEvent *aEvent = new AppEvent();
        aEvent->targetState = GameState::Playing;
        aEvent->type = EventType::GameStateChange;
        aEvent->source = this;
        aEvent->fire();
      }
    }
  }
}
void TextContainer::update(float deltas){
  sf::View currentView = m_camera->getView();
  //std::cout << "text current view:  x " << currentView.getCenter().x << "\t y " << currentView.getCenter().y << "\n";
  m_anchorPoint = sf::Vector2f(currentView.getCenter().x - (currentView.getSize().x /2 ) + m_padding,
                                currentView.getCenter().y + (currentView.getSize().y /4 ) + m_padding);
  for(auto box : m_textBox){
      box->setPosition(m_anchorPoint);
  }

  std::string currentText = getCurrentText();
  if(m_currentText == nullptr){
    m_currentText = new sf::Text(currentText, m_appFont, APP_FONT_SIZE);
    m_currentText->setOutlineColor(sf::Color::White);


  }else{
    m_currentText->setString(currentText);
  }
  float margin = 5.0f;
  m_currentText->setPosition(sf::Vector2f(m_anchorPoint.x + margin, m_anchorPoint.y));
}
void TextContainer::draw(sf::RenderWindow * window){
  for(auto item : m_textBox){
    window->draw(*item);
  }
  window->draw(*m_currentText);
}

//// privates ////
std::string TextContainer::getCurrentText(){
  // = "Welcome to the Age of Titans. This is a sample long text to test the engine. \n I'm quite sure everything will be displayed on a single line";
  std::string currentText;
  if(m_dialogIsSet){
    Message message = m_currentDialog.messages[m_currentMessage];
    currentText = message.content;
  }else{
    currentText = "";
  }
  return currentText;
}
