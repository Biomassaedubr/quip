#import "PopupServiceProvider.hpp"

#import "QuipPopupView.h"

namespace quip {
  PopupServiceProvider::PopupServiceProvider (QuipTextView * view)
  : m_view(view) {
  }
  
  PopupServiceProvider::~PopupServiceProvider() {
    m_view = nullptr;
  }
  
  void PopupServiceProvider::tick (double elapsedSeconds) {
    for (QuipPopupView * popup in [m_view subviews]) {
      [popup setDuration:[popup duration] - elapsedSeconds];
      if ([popup duration] <= 0.0) {
        [popup removeFromSuperview];
      }
    }
  }
  
  void PopupServiceProvider::createPopupAtLocation (const Location & location, const std::string & text) {
    [m_view createPopupAtLocation:[NSString stringWithCString:text.c_str() encoding:NSUTF8StringEncoding] atLocation:location];
  }
}