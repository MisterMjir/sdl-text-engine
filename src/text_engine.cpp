#include "text_engine.h"

#include <iostream>

TextEngineDefaults TextEngine::defaults = {16, "res/Roboto_Mono/RobotoMono-Regular.ttf", 64, {0, 0, 0}};

static int ticks = 0; // For demonstration

TextEngine::TextEngine(std::string mesg, int x, int y)
{
  wholeMessage = mesg;
  message = wholeMessage[0];
  currentChar = 0;
  currentSpeed = defaults.speed;
  this->x = x;
  this->y = y;
  w = h = 0;
  outputSpeeds.push_back({0, 64});
  charTicks.push_back(0);
}

TextEngine::~TextEngine()
{
  for (auto font : fonts)
    TTF_CloseFont(font.font);
}

/*
 * You can (and probably should) change this function to match your program
 */
void TextEngine::update()
{
  for (auto charTick : charTicks)
    charTick++;

  // This segment can probably be optimized
  for (auto os : outputSpeeds)
  {
    if (currentChar == os.index)
    {
      currentSpeed = os.speed;
      break;
    }
  }

  ticks++;
  if (ticks % currentSpeed == 0)
    nextChar();
}

void TextEngine::draw(SDL_Renderer* ren)
{
  /*
   * Multi-fonts not supported yet
   * Just using the first font available
   * I will need to redo the smart wrap code and a lot of other code
   */
  if (fonts.size() == 0)
    fonts.push_back({0, TTF_OpenFont(defaults.fontLoc, defaults.ptsize), defaults.color});
  // Blended looks nicer than Solid, looks are priority
  SDL_Surface* txtSurface = TTF_RenderText_Blended_Wrapped(fonts[0].font, message.c_str(), {0, 0, 0}, 1200 - x); // 1200 is window width
  SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, txtSurface);

  w = txtSurface->w;
  h = txtSurface->h;
  SDL_Rect dRect = {x, y, txtSurface->w, txtSurface->h};
  SDL_RenderCopy(ren, texture, NULL, &dRect);

  SDL_FreeSurface(txtSurface);
  SDL_DestroyTexture(texture);
}

void TextEngine::nextChar()
{
  if (currentChar < wholeMessage.length())
    currentChar++;

  /*
   * Smart Wrap
   * Checks if there is enough space for the next word
   * All 'words' are substrings seperated by spaces
   */
  if (wholeMessage[currentChar] == ' ') // New word is approaching
  {
    int charWidth = w / currentChar; // This is approximate, should be good enough
    int wordLen = wholeMessage.find(" ", currentChar + 1) - (currentChar + 1); // wordLen = index of new ' ' - index of first letter of word
    if (wordLen < 0) // Last word
      wordLen = wholeMessage.length() - (currentChar + 1);

    if (wordLen * charWidth + (x + w) > 1200 - x) // if the new word will be greater than the width which will wrap
      message += '\n';
    else
      message += ' ';
  }
  else
    message += wholeMessage[currentChar];

  charTicks.push_back(0);
}

void TextEngine::fullMessage()
{
  message = wholeMessage;
  while (charTicks.size() != wholeMessage.length())
    charTicks.push_back(0);
}

bool TextEngine::finished()
{
  return currentChar == wholeMessage.length();
}

void TextEngine::addSpeed(int index, int speed)
{
  outputSpeeds.push_back({index, speed});
}

void TextEngine::addFont(int index, const char* fontLoc, int ptsize)
{
  fonts.push_back({index, TTF_OpenFont(fontLoc, ptsize), defaults.color});
}

void TextEngine::addFont(int index, const char* fontLoc, int ptsize, SDL_Color color)
{
  fonts.push_back({index, TTF_OpenFont(fontLoc, ptsize), color});
}
