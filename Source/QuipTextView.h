#import <Cocoa/Cocoa.h>

#include "Document.hpp"

#include <memory>

@class QuipStatusView;

@interface QuipTextView : NSView

- (instancetype)initWithFrame:(NSRect)frame document:(std::shared_ptr<quip::Document>)document status:(QuipStatusView *)status;

- (quip::Document &)document;
- (void)setDocument:(std::shared_ptr<quip::Document>)document;

@end
