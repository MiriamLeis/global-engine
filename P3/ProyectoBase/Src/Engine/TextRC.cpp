#include "TextRC.h"

#include <iostream>

TextRC::TextRC() {
  int error = FT_Init_FreeType(&library);
  if (error) {
    std::cout << "An error occurred during library initialization";
  }

  error = FT_New_Face(library, "../../assets/fonts/arial.ttf", 0, &face);
  if (error == FT_Err_Unknown_File_Format) {
    std::cout << "The font file could be opened and read, but it appears that its font format is unsupported";
  }
  else if (error) {
    std::cout << "The font file could not be opened or read, or it is broken";
  }
}
