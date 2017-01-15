#pragma once

#include <string>

namespace quip {
  //! Stores global configuration settings.
  struct GlobalSettings {
    GlobalSettings ();
    
    const std::string& defaultFontFace () const;
    float defaultFontSize () const;
    
  private:
    std::string m_defaultFontName;
    float m_defaultFontSize;
  };
}
