#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

class TextRC
{
private:
  FT_Library library;
  FT_Face face;

public:

  TextRC();
  ~TextRC();
};