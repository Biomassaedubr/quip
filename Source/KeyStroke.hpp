#pragma once

#include "Key.hpp"

#include <string>

namespace quip {
  struct KeyStroke {
    KeyStroke (Key key, const std::string & text);
  
    Key key () const;
    const std::string & text () const;
    
  private:
    Key m_key;
    std::string m_text;
  };
}