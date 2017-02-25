#include "catch.hpp"

#include "KeySequence.hpp"

using namespace quip;

TEST_CASE("Sequences can be default-constructed.", "[KeySequenceTests]") {
  KeySequence sequence;
  
  REQUIRE(sequence.count() == 0);
}

TEST_CASE("Sequences can be constructed with a single key.", "[KeySequenceTests]") {
  KeySequence sequence(Key::A);
  REQUIRE(sequence.count() == 1);
  
  Key expected[] = { Key::A };
  Key * cursor = expected;
  for (const Key & key : sequence) {
    REQUIRE(key == *cursor);
    ++cursor;
  }
}

TEST_CASE("Sequences can be constructed with multiple keys.", "[KeySequenceTests]") {
  KeySequence sequence({Key::A, Key::B, Key::C});
  REQUIRE(sequence.count() == 3);
  
  Key expected[] = { Key::A, Key::B, Key::C };
  Key * cursor = expected;
  for (const Key & key : sequence) {
    REQUIRE(key == *cursor);
    ++cursor;
  }
}

TEST_CASE("Sequences can be constructed with an expression for a single key.", "[KeySequenceTests]") {
  KeySequence sequence("Q");
  REQUIRE(sequence.count() == 1);
  
  Key expected[] = { Key::Q };
  Key * cursor = expected;
  for (const Key & key : sequence) {
    REQUIRE(key == *cursor);
    ++cursor;
  }
}

TEST_CASE("Sequences can be constructed with an expression for multiple keys.", "[KeySequenceTests]") {
  KeySequence sequence("QAZ");
  REQUIRE(sequence.count() == 3);
  
  Key expected[] = { Key::Q, Key::A, Key::Z };
  Key * cursor = expected;
  for (const Key & key : sequence) {
    REQUIRE(key == *cursor);
    ++cursor;
  }
}

TEST_CASE("Sequences can be constructed with an expression for a single named key.", "[KeySequenceTests]") {
  KeySequence sequence("<ESC>");
  REQUIRE(sequence.count() == 1);
  
  Key expected[] = { Key::Escape };
  Key * cursor = expected;
  for (const Key & key : sequence) {
    REQUIRE(key == *cursor);
    ++cursor;
  }
}

TEST_CASE("Sequences can be constructed with an expression for a single modified key.", "[KeySequenceTests]") {
  KeySequence sequence("<C-X>");
  REQUIRE(sequence.count() == 3);
  
  std::int32_t modifierDown = static_cast<std::int32_t>(Key::ModifierDownMask) | static_cast<std::int32_t>(Key::ControlMask);
  std::int32_t modifierUp = static_cast<std::int32_t>(Key::ModifierUpMask) | static_cast<std::int32_t>(Key::ControlMask);
  
  Key expected[] = { static_cast<Key>(modifierDown), Key::X, static_cast<Key>(modifierUp) };
  Key * cursor = expected;
  for (const Key & key : sequence) {
    REQUIRE(key == *cursor);
    ++cursor;
  }
}

TEST_CASE("Sequences can be constructed with an expression for the back slash key.", "[KeySequenceTests]") {
  KeySequence sequence("/");
  REQUIRE(sequence.count() == 1);
  
  Key expected[] = { Key::BackSlash };
  Key * cursor = expected;
  for (const Key & key : sequence) {
    REQUIRE(key == *cursor);
    ++cursor;
  }
}

TEST_CASE("Sequences can be constructed with an expression for the forward slash key.", "[KeySequenceTests]") {
  KeySequence sequence("\\");
  REQUIRE(sequence.count() == 1);
  
  Key expected[] = { Key::ForwardSlash };
  Key * cursor = expected;
  for (const Key & key : sequence) {
    REQUIRE(key == *cursor);
    ++cursor;
  }
}

TEST_CASE("Sequences can be appended to.", "[KeySequenceTests]") {
  KeySequence sequence(Key::A);
  REQUIRE(sequence.count() == 1);
  
  sequence.append(Key::B);
  REQUIRE(sequence.count() == 2);
  
  Key expected[] = { Key::A, Key::B};
  Key * cursor = expected;
  for (const Key & key : sequence) {
    REQUIRE(key == *cursor);
    ++cursor;
  }
}

TEST_CASE("Sequences can be cleared.", "[KeySequenceTests]") {
  KeySequence sequence({Key::A, Key::B, Key::C});
  REQUIRE(sequence.count() == 3);
  
  sequence.clear();
  REQUIRE(sequence.count() == 0);
}

