#import "QuipStatusView.h"

#include "DrawingServiceProvider.hpp"

@interface QuipStatusView () {
@private
  quip::DrawingService * m_drawingService;
  
  NSString * m_text;
  std::size_t m_lineCount;
  NSString * m_fileType;
}
@end

static CGFloat gStatusLineBottomPadding = 4.0;
static CGFloat gStatusLineLeftPadding = 2.0;

@implementation QuipStatusView

- (instancetype)initWithFrame:(NSRect)frame {
  self = [super initWithFrame:frame];
  if (self != nil) {
    m_drawingService = nullptr;
    m_text = @"";
  }
  
  return self;
}

- (void)attachDrawingService:(quip::DrawingService *)drawingService {
  m_drawingService = drawingService;
}

- (void)setStatus:(const char *)status {
  m_text = [NSString stringWithCString:status encoding:NSUTF8StringEncoding];
  [self setNeedsDisplay:YES];
}

- (void)setLineCount:(std::size_t)lineCount {
  m_lineCount = lineCount;
  [self setNeedsDisplay:YES];
}

- (void)setFileType:(const char *)fileType {
  m_fileType = [NSString stringWithCString:fileType encoding:NSUTF8StringEncoding];
  [self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)dirtyRect {
  [super drawRect:dirtyRect];
  
  if (m_text.length == 0) {
    return;
  }
  
  std::string textLabel([m_text cStringUsingEncoding:NSUTF8StringEncoding]);
  m_drawingService->drawText(textLabel, quip::Coordinate(gStatusLineLeftPadding, gStatusLineBottomPadding));
  
  NSString * lineCountLabel = [NSString stringWithFormat:@"%lu line%@ (%@)", m_lineCount, m_lineCount == 1 ? @"" : @"s", m_fileType];
  std::string lineLabel([lineCountLabel cStringUsingEncoding:NSUTF8StringEncoding]);
  quip::Rectangle lineBounds = m_drawingService->measureText(lineLabel);
  quip::Coordinate lineLocation([self frame].size.width - gStatusLineLeftPadding - lineBounds.width(), gStatusLineBottomPadding);
  m_drawingService->drawText(lineLabel, lineLocation);
}

@end
