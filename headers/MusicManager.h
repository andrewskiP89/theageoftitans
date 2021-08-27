#include "sfml.h"

#define BG_MUSIC "./assets/music/bg_music.wav"


class MusicManager {
public:
  bool loadTrack(){
    m_music = new sf::Music();
    m_trackLoaded = m_music->openFromFile(BG_MUSIC);
    if(!m_trackLoaded)
      std::cout << "Unable to play music for " << BG_MUSIC << "\n";
    return m_trackLoaded;
  }

  void play(){
    if(m_playing)
      return;
    m_music->play();
    m_playing = true;
  }

private:
  sf::Music *m_music;
  bool m_trackLoaded;
  bool m_playing;
};
