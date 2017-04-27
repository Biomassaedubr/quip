#pragma once

#include "LuaBinding.hpp"

#include <string>

namespace quip {
  //! Stores global configuration settings.
  struct GlobalSettings {
    GlobalSettings ();
    
    const std::string& fontFace() const;
    void setFontFace(const std::string& face);
    
    float fontSize() const;
    void setFontSize(float size);
    
    static LuaBinding binding();
    
  private:
    std::string m_fontFace;
    float m_fontSize;
  };
}
