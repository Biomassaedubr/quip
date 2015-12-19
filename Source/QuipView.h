#import <Cocoa/Cocoa.h>

#include "Document.hpp"

#include <memory>

@interface QuipView : NSView

- (instancetype)initWithFrame:(NSRect)frame document:(std::shared_ptr<quip::Document>)document;

- (quip::Document &)document;
- (void)setDocument:(std::shared_ptr<quip::Document>)document;

@end