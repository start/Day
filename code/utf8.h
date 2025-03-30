#ifndef utf8_h_included
#define utf8_h_included

#include "basic_types.h"

Size Utf8CharacterWidth(CharacterBundle character_bundle);
UTFCodepoint UTF8Codepoint(
  CharacterBundle character_bundle,
  Size character_bundle_s);
YesNo IsUTFCodepointChinese(UTFCodepoint code);

#endif
