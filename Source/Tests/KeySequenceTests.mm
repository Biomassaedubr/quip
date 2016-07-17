#import <XCTest/XCTest.h>

#include "KeySequence.hpp"

@interface KeySequenceTests : XCTestCase

@end

@implementation KeySequenceTests

using namespace quip;

- (void)testDefaultConstruction {
  KeySequence sequence;
  
  XCTAssertEqual(sequence.count(), 0);
}

- (void)testConstructWithSingleKey {
  KeySequence sequence(Key::A);
  XCTAssertEqual(sequence.count(), 1);

  Key expected[] = { Key::A };
  Key * cursor = expected;
  for (const Key & key : sequence) {
    XCTAssertEqual(key, *cursor);
    ++cursor;
  }
}

- (void)testConstructWithMultipleKeys {
  KeySequence sequence({Key::A, Key::B, Key::C});
  XCTAssertEqual(sequence.count(), 3);
  
  Key expected[] = { Key::A, Key::B, Key::C };
  Key * cursor = expected;
  for (const Key & key : sequence) {
    XCTAssertEqual(key, *cursor);
    ++cursor;
  }
}

- (void)testAppend {
  KeySequence sequence(Key::A);
  XCTAssertEqual(sequence.count(), 1);

  sequence.append(Key::B);
  XCTAssertEqual(sequence.count(), 2);

  Key expected[] = { Key::A, Key::B};
  Key * cursor = expected;
  for (const Key & key : sequence) {
    XCTAssertEqual(key, *cursor);
    ++cursor;
  }
}

- (void)testClear {
  KeySequence sequence({Key::A, Key::B, Key::C});
  XCTAssertEqual(sequence.count(), 3);
  
  sequence.clear();
  XCTAssertEqual(sequence.count(), 0);
}


@end
