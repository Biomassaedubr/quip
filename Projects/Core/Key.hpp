#pragma once

#include <cstdint>

namespace quip {
  enum struct Key {
    A = 0x0,
    B = 0xb,
    C = 0x8,
    D = 0x2,
    E = 0xe,
    F = 0x3,
    G = 0x5,
    H = 0x4,
    I = 0x22,
    J = 0x26,
    K = 0x28,
    L = 0x25,
    M = 0x2e,
    N = 0x2d,
    O = 0x1f,
    P = 0x23,
    Q = 0xc,
    R = 0xf,
    S = 0x1,
    T = 0x11,
    U = 0x20,
    V = 0x9,
    W = 0xd,
    X = 0x7,
    Y = 0x10,
    Z = 0x6,
    Tilde = 0x32,
    Key1 = 0x12,
    Key2 = 0x13,
    Key3 = 0x14,
    Key4 = 0x15,
    Key5 = 0x17,
    Key6 = 0x16,
    Key7 = 0x1a,
    Key8 = 0x1c,
    Key9 = 0x19,
    Key0 = 0x1d,
    Minus = 0x1b,
    Plus = 0x18,
    Delete = 0x33,
    Tab = 0x30,
    LeftSquareBracket = 0x21,
    RightSquareBracket = 0x1e,
    BackSlash = 0x2c,
    ForwardSlash = 0x2a,
    Semicolon = 0x29,
    Quote = 0x27,
    Return = 0x24,
    Comma = 0x2b,
    Period = 0x2f,
    Slash = 0x2c,
    Space = 0x31,
    Escape = 0x35,
    F1 = 0x7a,
    F2 = 0x78,
    F3 = 0x63,
    F4 = 0x76,
    F5 = 0x60,
    F6 = 0x61,
    F7 = 0x62,
    F8 = 0x64,
    F9 = 0x65,
    F10 = 0x6d,
    F11 = 0x6e,
    F12 = 0x6f,
    ArrowUp = 0x7e,
    ArrowDown = 0x7d,
    ArrowLeft = 0x7b,
    ArrowRight = 0x7c,
    
    // Masks used to indicate a modifier is held or released.
    ModifierDownMask = 0x1000,
    ModifierUpMask = 0x2000,
    
    // Masks used to indicate which modifier is held or released.
    ShiftMask = 0x10000,
    ControlMask = 0x20000,
    OptionMask = 0x40000,
  };
  
  Key modifierDown(Key modifierMask);
  Key modifierUp(Key modifierMask);
  
  Key keyFromScanCode (int code);
  Key keyFromCharacter (char character);
  
  bool keyIsNumber (Key key);
  std::uint32_t numberFromKey (Key key);
}
