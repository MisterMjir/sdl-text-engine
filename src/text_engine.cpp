#include "text_engine.h"

static int ticks = 0; // For demonstration

TextEngine::TextEngine(std::string mesg, int ptsize)
{
  wholeMessage = mesg;
  message = wholeMessage[0];
  font = TTF_OpenFont("res/Roboto_Mono/RobotoMono-Regular.ttf", ptsize);
  currentChar = 0;
}

TextEngine::~TextEngine()
{
  TTF_CloseFont(font);
}

/*
 * You can (and probably should) change this function to match your program
 */
void TextEngine::update()
{
  ticks++;
  if (ticks % 64 == 0)
    nextCharacter();
}

void TextEngine::draw(SDL_Renderer* ren)
{
  // Blended looks nicer than Solid, looks are priority
  SDL_Surface* txtSurface = TTF_RenderText_Blended_Wrapped(font, message.c_str(), {0, 0, 0}, 1200);
  texture = SDL_CreateTextureFromSurface(ren, txtSurface);

  SDL_Rect dRect = {32, 32, txtSurface->w, txtSurface->h};
  SDL_RenderCopy(ren, texture, NULL, &dRect);

  SDL_FreeSurface(txtSurface);
}

void TextEngine::nextCharacter()
{
  if (currentChar < wholeMessage.length())
    currentChar++;

  message += wholeMessage[currentChar];
}

void TextEngine::fullMessage()
{

}
