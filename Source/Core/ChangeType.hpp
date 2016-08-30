#pragma once

namespace quip {
  // Values that describe the type of change effected by a transaction.
  enum struct ChangeType {
    Do,
    Undo,
    Redo,
  };
}