#import "PopupServiceProvider.hpp"

namespace quip {
  PopupServiceProvider::PopupServiceProvider (QuipTextView * view)
  : m_view(view) {
  }
  
  PopupServiceProvider::~PopupServiceProvider() {
    m_view = nullptr;
  }
  
  void PopupServiceProvider::tick (double elapsedSeconds) {
    for (const std::pair<PopupHandle, QuipPopupView *> & record : m_popups) {
      QuipPopupView * popup = record.second;
      [popup setDuration:[popup duration] - elapsedSeconds];
      if ([popup duration] <= 0.0) {
        [popup removeFromSuperview];
      }
    }
  }
  
  PopupHandle PopupServiceProvider::createPopupAtLocation (const Location & location, const std::string & text) {
    QuipPopupView * popup = [m_view createPopupAtLocation:[NSString stringWithCString:text.c_str() encoding:NSUTF8StringEncoding] atLocation:location];
    PopupHandle result = static_cast<PopupHandle>(m_popups.size());
    m_popups.emplace(result, popup);
    
    return result;
  }
}