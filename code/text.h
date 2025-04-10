#ifndef text_h_already_included
#define text_h_already_included

#include "common_data_types.h"
#include "memory.h"


enum UTF8CharacterWidth: Size
{
  OneByteWide = 1,
  TwoBytesWide = 2,
  ThreeBytesWide = 3,
  FourBytesWide = 4
};

constexpr auto utf8_max_character_width = FourBytesWide;

Text CopyText(
  Text full_text,
  Offset snippet_start_o,
  Offset just_after_snippet_end_o,
  struct Allocator *allocator);

enum UTF8CharacterWidth UTF8CharacterWidth(
  CharacterBundle character_bundle);

UTFCodepoint UTF8Codepoint(
  CharacterBundle character_bundle,
  enum UTF8CharacterWidth character_bundle_w);

YesNo IsUTFCodepointChinese(UTFCodepoint code);

#endif
