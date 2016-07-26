#import "StatusServiceProvider.hpp"

namespace quip {
  StatusServiceProvider::StatusServiceProvider (QuipStatusView * view)
  : m_view(view) {
  }
  
  StatusServiceProvider::StatusServiceProvider() {
    m_view = nullptr;
  }
  
  void StatusServiceProvider::setStatus (const std::string & text) {
    [m_view setStatus:text.c_str()];
  }
}