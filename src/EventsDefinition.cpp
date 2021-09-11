#include "../headers/EventsDefinition.h"

std::map<std::string, AppEvent> EventManager::getAllEvents(uint8_t currentLevel){
  std::map<std::string, AppEvent> returnMap;
  AppEvent welcomeEvent;
  welcomeEvent.id = "welcomeEvent";
  welcomeEvent.requiredTask = -1; //
  welcomeEvent.type = EventType::ShowMessage;

  Dialog welcomeDialog;
  Message  welcomeMessage;
  welcomeMessage.content = "This is the entry for the labyrinth.";
  Message welcomeMessage2;
  welcomeMessage2.content = "It will be dangerous inside. Take care and good luck!";
  welcomeDialog.messages.push_back(welcomeMessage);
  welcomeDialog.messages.push_back(welcomeMessage2);
  welcomeEvent.dialog = welcomeDialog;

  returnMap["welcomeEvent"] = welcomeEvent;

  AppEvent hitTheRockEvent;
  hitTheRockEvent.id = "hitTheRockEvent";
  hitTheRockEvent.requiredTask = -1; //
  hitTheRockEvent.type = EventType::ShowMessage;

  Dialog hitTheRockDialog;
  Message  hitTheRockMessage;
  hitTheRockMessage.content = "Ouch! That was hard.";
  hitTheRockDialog.messages.push_back(hitTheRockMessage);
  hitTheRockEvent.dialog = hitTheRockDialog;

  returnMap["hitTheRockEvent"] = hitTheRockEvent;

  AppEvent pushTheRock;
  pushTheRock.id = "pushTheRock";
  pushTheRock.requiredTask = -1; //
  pushTheRock.type = EventType::ShowMessage;
  pushTheRock.triggerType = TriggerType::OnClick;

  PlayerAction pAction;
  pAction.type = PActionType::Push;
  Dialog pushTheRockDialog;
  Message  pushTheRockMessage;
  pushTheRockMessage.content = "It's too heavy to lift!";
  pushTheRockDialog.messages.push_back(pushTheRockMessage);
  pushTheRock.dialog = pushTheRockDialog;
  pushTheRock.action = pAction;
  returnMap["pushTheRock"] = pushTheRock;
  return returnMap;
}

extern std::map<std::string, std::vector<std::string>> TILE_SET_EVENT_MAP = {

};
// una mappa che assegna un evento/ lista di eventi ad un insieme di punti della mappa
extern std::map<std::string, std::vector<std::string>> WORLD_EVENT_MAP = {
  {"l1r28c12lv1", {
      "welcomeEvent"
  }},
  {"l1r36c8lv1", {
      "hitTheRockEvent"
  }},
  {"l1r43c5lv1", {
      "pushTheRock"
  }}
};
