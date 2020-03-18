#include "text_engine.h"
#include <cmath>

TextEngineDefaults TextEngine::defaults = {16, "res/Roboto_Mono/RobotoMono-Regular.ttf", 64, {0, 0, 0}};

// These variables aren't 100% needed
static int WINDOW_WIDTH = 1200;
static int WRAP_WIDTH = WINDOW_WIDTH - 32;

TextEngine::TextEngine(std::string mesg, int x, int y)
{
  wholeMessage = mesg;
  message = wholeMessage[0];
  currentChar = 0;
  currentSpeed = defaults.speed;
  currentFont = 0;
  currentX = messageX = x;
  currentY = messageY = y;
  outputSpeeds.push_back({0, 64});
  charTicks.push_back(0);
  moving = false;
}

TextEngine::~TextEngine()
{
  for (auto font : fonts)
    TTF_CloseFont(font.font);

  for (auto text : chars)
    SDL_DestroyTexture(text.texture);
}

/*
 * You can (and probably should) change this function to match your program
 */
void TextEngine::update()
{
  for (int i = 0; i < charTicks.size(); i++)
    charTicks[i]++;

  ticks++;
  if (ticks % currentSpeed == 0)
    nextChar();
  //fullMessage();
}

void TextEngine::draw(SDL_Renderer* ren)
{
  makeTexture(ren, currentChar);
  for (int i = 0; i < chars.size(); i++)
  {
    SDL_Rect dRect;
    if (chars[i].moving) // Edit sine function as needed (Remember Algebra II: vertical stretch/compression * sin(x * horizontal strech/compression))
      dRect = {chars[i].myPos.x, chars[i].myPos.y + (int) (5 * std::sin(charTicks[i] / 50)), chars[i].myPos.w, chars[i].myPos.h};
    else
      dRect = chars[i].myPos;
    SDL_RenderCopy(ren, chars[i].texture, NULL, &dRect);
  }
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
    int totalWidth = 0;
    for (auto text : chars)
      totalWidth += text.myPos.w;
    int charWidth = totalWidth / currentChar; // This is approximate, should be good enough
    int wordLen = wholeMessage.find(" ", currentChar + 1) - (currentChar + 1); // wordLen = index of new ' ' - index of first letter of word
    if (wordLen < 0) // Last word
      wordLen = wholeMessage.length() - (currentChar + 1);

    if (wordLen * charWidth + (messageX + currentX) > WRAP_WIDTH) // if the new word will be greater than the width which will wrap
    {
      message += '\n';
    }
    else
    {
      message += ' ';
    }
  }
  else
    message += wholeMessage[currentChar];

  changeProperties();

  charTicks.push_back(0);
}

/*void TextEngine::fullMessage()
{
  currentSpeed = 1;
  while (message.length() != wholeMessage.length())
  {
    nextChar();
  }
}*/

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

void TextEngine::setMoving(int index, bool moving)
{
  moveEvents.push_back({index, moving});
}

void TextEngine::changeProperties()
{
  for (auto os : outputSpeeds)
  {
    if (currentChar == os.index)
    {
      currentSpeed = os.speed;
      break;
    }
  }

  for (int i = 0; i < fonts.size(); i ++)
  {
    if (currentChar == fonts[i].index)
    {
      currentFont = i;
      break;
    }
  }

  for (int i = 0; i < moveEvents.size(); i++)
  {
    if (currentChar == moveEvents[i].index)
    {
      moving = moveEvents[i].moving;
      break;
    }
  }
}

void TextEngine::makeTexture(SDL_Renderer* ren, int i)
{
  // Can't have more characters than the message; don't want to print that \0
  if (chars.size() < message.length() && chars.size() < wholeMessage.size())
  {
    if (fonts.size() == 0)
      fonts.push_back({0, TTF_OpenFont(defaults.fontLoc, defaults.ptsize), defaults.color});

    // Blended looks nicer than Solid, looks are priority
    SDL_Surface* txtSurface = TTF_RenderText_Blended_Wrapped(fonts[currentFont].font, &message[i], fonts[currentFont].color, WRAP_WIDTH);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, txtSurface);

    chars.push_back({texture, {currentX, currentY, txtSurface->w, txtSurface->h}, moving});

    currentX += txtSurface->w;
    if (currentX > WRAP_WIDTH || message[i] == '\n')
    {
      currentX = messageX;
      currentY += txtSurface->h;
    }

    SDL_FreeSurface(txtSurface);
  }
}
