#import <Cocoa/Cocoa.h>

#include "Document.hpp"
#include "DrawingService.hpp"

#include <memory>

@class QuipPopupView;
@class QuipStatusView;

@interface QuipTextView : NSView
- (void)attachDrawingService:(quip::DrawingService *)drawingService;

- (quip::Document &)document;
- (void)setDocument:(std::shared_ptr<quip::Document>)document;

- (void)setStatus:(QuipStatusView *)status;

- (void)setActBackgrounded:(BOOL)shouldActBackgrounded;

- (void)scrollLocationIntoView:(quip::Location)location;

- (QuipPopupView *)createPopupAtLocation:(NSString *)text atLocation:(quip::Location)location;

@end
