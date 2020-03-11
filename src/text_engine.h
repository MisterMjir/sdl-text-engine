#ifndef TEXT_ENGINE
#define TEXT_ENGINE

#include <string>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

class TextEngine
{
public:
  TextEngine(std::string mesg, int ptsize);
  ~TextEngine();

  void update();
  void draw(SDL_Renderer*);
private:
  void nextCharacter();
  void fullMessage();

  std::string wholeMessage;
  std::string message;
  TTF_Font* font;
  int currentChar;
  SDL_Texture* texture;
};

#endif
