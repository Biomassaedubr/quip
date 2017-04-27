#include "GlobalSettings.hpp"

namespace quip {
  GlobalSettings::GlobalSettings()
  : m_fontFace("Menlo")
  , m_fontSize(13.0f) {
  }
  
  const std::string& GlobalSettings::fontFace() const {
    return m_fontFace;
  }
  
  void GlobalSettings::setFontFace(const std::string& face) {
    m_fontFace = face;
  }
  
  float GlobalSettings::fontSize() const {
    return m_fontSize;
  }
  
  void GlobalSettings::setFontSize(float size) {
    m_fontSize = size;
  }
  
  LuaBinding GlobalSettings::binding() {
    LuaBinding result;
    result.addProperty("fontFace", &GlobalSettings::fontFace, &GlobalSettings::setFontFace);
    result.addProperty("fontSize", &GlobalSettings::fontSize, &GlobalSettings::setFontSize);
    
    return result;
  }
}
