#ifndef TEXT_ENGINE
#define TEXT_ENGINE

#include <string>
#include <vector>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

struct OutputSpeed
{
  int index;
  int speed;
};

struct MoveEvent
{
  int index;
  bool moving;
};

struct TextEngineDefaults
{
  int ptsize;
  const char* fontLoc;
  int speed;
  SDL_Color color;
};

struct TextEngineFont
{
  int index;
  TTF_Font* font;
  SDL_Color color;
};

struct TextEngineChar
{
  SDL_Texture* texture;
  SDL_Rect myPos;
  bool moving;
};

class TextEngine
{
public:
  TextEngine(std::string mesg, int x, int y);
  ~TextEngine();

  void update();
  void draw(SDL_Renderer*);
  bool finished();
  void addFont(int index, const char* fontLoc, int ptsize);
  void addFont(int index, const char* fontLoc, int ptsize, SDL_Color color);
  void addSpeed(int index, int speed);
  void setMoving(int index, bool);

  void nextChar();
  //void fullMessage();

  static TextEngineDefaults getDefaults() {return defaults;}
private:
  void changeProperties();
  void makeTexture(SDL_Renderer*, int i);

  static TextEngineDefaults defaults;

  int ticks;
  std::string wholeMessage;
  std::string message;
  int currentChar;
  int currentSpeed;
  int currentFont;
  bool moving;
  int messageX, messageY, currentX, currentY;
  std::vector<OutputSpeed> outputSpeeds;
  std::vector<TextEngineFont> fonts;
  std::vector<MoveEvent> moveEvents;
  std::vector<int> charTicks;
  std::vector<TextEngineChar> chars;
};

#endif
