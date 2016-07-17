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

- (void)testConstructWithExpressionForSingleKey {
  KeySequence sequence("Q");
  XCTAssertEqual(sequence.count(), 1);
  
  Key expected[] = { Key::Q };
  Key * cursor = expected;
  for (const Key & key : sequence) {
    XCTAssertEqual(key, *cursor);
    ++cursor;
  }
}

- (void)testConstructWithExpressionForMultipleKeys {
  KeySequence sequence("QAZ");
  XCTAssertEqual(sequence.count(), 3);
  
  Key expected[] = { Key::Q, Key::A, Key::Z };
  Key * cursor = expected;
  for (const Key & key : sequence) {
    XCTAssertEqual(key, *cursor);
    ++cursor;
  }
}

- (void)testConstructWithExpressionForSingleNamedKey {
  KeySequence sequence("<ESC>");
  XCTAssertEqual(sequence.count(), 1);
  
  Key expected[] = { Key::Escape };
  Key * cursor = expected;
  for (const Key & key : sequence) {
    XCTAssertEqual(key, *cursor);
    ++cursor;
  }
}

- (void)testConstructWithExpressionForSingleModifiedKey {
  KeySequence sequence("<C-X>");
  XCTAssertEqual(sequence.count(), 3);
  
  std::int32_t modifierDown = static_cast<std::int32_t>(Key::ModifierDownMask) | static_cast<std::int32_t>(Key::ControlMask);
  std::int32_t modifierUp = static_cast<std::int32_t>(Key::ModifierUpMask) | static_cast<std::int32_t>(Key::ControlMask);

  Key expected[] = { static_cast<Key>(modifierDown), Key::X, static_cast<Key>(modifierUp) };
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
