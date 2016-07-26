#include "Key.hpp"

#include <cctype>

namespace quip {
  Key keyFromScanCode (int code) {
    return static_cast<Key>(code);
  }
  
  Key keyFromCharacter (char character) {
    switch(std::toupper(character)) {
      case 'A':
        return Key::A;
      case 'B':
        return Key::B;
      case 'C':
        return Key::C;
      case 'D':
        return Key::D;
      case 'E':
        return Key::E;
      case 'F':
        return Key::F;
      case 'G':
        return Key::G;
      case 'H':
        return Key::H;
      case 'I':
        return Key::I;
      case 'J':
        return Key::J;
      case 'K':
        return Key::K;
      case 'L':
        return Key::L;
      case 'M':
        return Key::M;
      case 'N':
        return Key::N;
      case 'O':
        return Key::O;
      case 'P':
        return Key::P;
      case 'Q':
        return Key::Q;
      case 'R':
        return Key::R;
      case 'S':
        return Key::S;
      case 'T':
        return Key::T;
      case 'U':
        return Key::U;
      case 'V':
        return Key::V;
      case 'W':
        return Key::W;
      case 'X':
        return Key::X;
      case 'Y':
        return Key::Y;
      case 'Z':
        return Key::Z;
      case '/':
        return Key::BackSlash;
      case '\\':
        return Key::ForwardSlash;
      default:
        return Key::Escape;
    }
  }
}