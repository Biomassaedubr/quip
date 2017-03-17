#import "StatusServiceProvider.hpp"

namespace quip {
  StatusServiceProvider::StatusServiceProvider(QuipStatusView* view)
  : m_view(view) {
  }

  void StatusServiceProvider::setStatus(const std::string& text) {
    [m_view setStatus:text.c_str()];
  }
  
  void StatusServiceProvider::setFileType(const std::string & text) {
    [m_view setFileType:text.c_str()];
  }
  
  void StatusServiceProvider::setLineCount(const std::size_t count) {
    [m_view setLineCount:count];
  }
}
