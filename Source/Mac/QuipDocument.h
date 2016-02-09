#import <Cocoa/Cocoa.h>

#include "Document.hpp"

@interface QuipDocument : NSDocument

- (std::shared_ptr<quip::Document>)document;

@end
