#include "Key.hpp"

#include <cctype>

namespace quip {
  Key modifierDown(Key modifierMask) {
    std::uint32_t mask = static_cast<std::uint32_t>(modifierMask);
    std::uint32_t down = static_cast<std::uint32_t>(Key::ModifierDownMask);
    return static_cast<Key>(mask | down);
  }
  
  Key modifierUp(Key modifierMask) {
    std::uint32_t mask = static_cast<std::uint32_t>(modifierMask);
    std::uint32_t up = static_cast<std::uint32_t>(Key::ModifierUpMask);
    return static_cast<Key>(mask | up);
  }
  
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
  
  bool keyIsNumber (Key key) {
    switch (key) {
      case Key::Key1:
      case Key::Key2:
      case Key::Key3:
      case Key::Key4:
      case Key::Key5:
      case Key::Key6:
      case Key::Key7:
      case Key::Key8:
      case Key::Key9:
      case Key::Key0:
        return true;
      default:
        return false;
    }
  }
  
  std::uint32_t numberFromKey (Key key) {
    switch (key) {
      case Key::Key1:
        return 1;
      case Key::Key2:
        return 2;
      case Key::Key3:
        return 3;
      case Key::Key4:
        return 4;
      case Key::Key5:
        return 5;
      case Key::Key6:
        return 6;
      case Key::Key7:
        return 7;
      case Key::Key8:
        return 8;
      case Key::Key9:
        return 9;
      case Key::Key0:
      default:
        return 0;
    }
  }
  
}
