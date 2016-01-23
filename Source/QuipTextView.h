#import <Cocoa/Cocoa.h>

#include "Document.hpp"

#include <memory>

@class StatusView;

@interface QuipTextView : NSView

- (instancetype)initWithFrame:(NSRect)frame document:(std::shared_ptr<quip::Document>)document status:(StatusView *)status;

- (quip::Document &)document;
- (void)setDocument:(std::shared_ptr<quip::Document>)document;

@end
