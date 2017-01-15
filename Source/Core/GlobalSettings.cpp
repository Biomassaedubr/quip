#include "GlobalSettings.hpp"

namespace quip {
  GlobalSettings::GlobalSettings ()
  : m_defaultFontName("Menlo")
  , m_defaultFontSize(13.0f) {
  }
  
  const std::string & GlobalSettings::defaultFontFace () const {
    return m_defaultFontName;
  }
  
  float GlobalSettings::defaultFontSize () const {
    return m_defaultFontSize;
  }
}
