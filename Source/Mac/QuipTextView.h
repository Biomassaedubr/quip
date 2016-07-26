#import <Cocoa/Cocoa.h>

#include "Document.hpp"

#include <memory>

@class QuipPopupView;
@class QuipStatusView;

@interface QuipTextView : NSView

- (quip::Document &)document;
- (void)setDocument:(std::shared_ptr<quip::Document>)document;

- (void)setStatus:(QuipStatusView *)status;

- (void)setActBackgrounded:(BOOL)shouldActBackgrounded;

- (QuipPopupView *)createPopupAtLocation:(NSString *)text atLocation:(quip::Location)location;

@end
