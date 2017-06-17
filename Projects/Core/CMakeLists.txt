set(DocumentSourceFiles
  Document.cpp
  Document.hpp
  DocumentIterator.cpp
  DocumentIterator.hpp
  ReverseDocumentIterator.cpp
  ReverseDocumentIterator.hpp
  Traversal.cpp
  Traversal.hpp
  Traversal.inl
)
source_group(Document FILES ${DocumentSourceFiles})

set(InputSourceFiles
  EditContext.cpp
  EditContext.hpp
  Key.cpp
  Key.hpp
  KeySequence.cpp
  KeySequence.hpp
  MapTrie.cpp
  MapTrie.hpp
  MapTrieNode.cpp
  MapTrieNode.hpp
  Modifiers.cpp
  Modifiers.hpp
  ViewController.hpp
)
source_group(Input FILES ${InputSourceFiles})

set(ModeSourceFiles
  CursorFlags.hpp
  CursorStyle.hpp
  EditMode.cpp
  EditMode.hpp
  JumpMode.cpp
  JumpMode.hpp
  Mode.cpp
  Mode.hpp
  NormalMode.cpp
  NormalMode.hpp
  SearchMode.cpp
  SearchMode.hpp
)
source_group(Mode FILES ${ModeSourceFiles})

set(ScriptingSourceFiles
  Lua.hpp
  LuaBinding.cpp
  LuaBinding.hpp
  Script.cpp
  Script.hpp
  ScriptBoundObject.cpp
  ScriptBoundObject.hpp
  ScriptHost.cpp
  ScriptHost.hpp
  SearchExpression.cpp
  SearchExpression.hpp
  GlobalSettings.cpp
  GlobalSettings.hpp
)
source_group(Scripting FILES ${ScriptingSourceFiles})

set(SelectionSourceFiles
  Selection.cpp
  Selection.hpp
  SelectionDrawInfo.cpp
  SelectionDrawInfo.hpp
  SelectionSet.cpp
  SelectionSet.hpp
  SelectionSetIterator.hpp
  SelectionSetIterator.inl
  Selector.cpp
  Selector.hpp
)
source_group(Selection FILES ${SelectionSourceFiles})

set(ServiceSourceFiles
  DrawingService.cpp
  DrawingService.hpp
  PopupService.cpp
  PopupService.hpp
  StatusService.cpp
  StatusService.hpp
)
source_group(Service FILES ${ServiceSourceFiles})

set(SyntaxSourceFiles
  AttributeRange.cpp
  AttributeRange.hpp
  Color.cpp
  Color.hpp
  FileTypeDatabase.cpp
  FileTypeDatabase.hpp
)
source_group(Syntax FILES ${SyntaxSourceFiles})

set(TransactionSourceFiles
  AppendTransaction.cpp
  AppendTransaction.hpp
  ChangeType.hpp
  EraseTransaction.cpp
  EraseTransaction.hpp
  InsertTransaction.cpp
  InsertTransaction.hpp
  Transaction.cpp
  Transaction.hpp
)
source_group(Transaction FILES ${TransactionSourceFiles})

set(UtilitySourceFiles
  Coordinate.cpp
  Coordinate.hpp
  Extent.cpp
  Extent.hpp
  Location.cpp
  Location.hpp
  Optional.hpp
  Rectangle.cpp
  Rectangle.hpp
  Signal.hpp
)
source_group(Utility FILES ${UtilitySourceFiles})

add_library(Quip.Core
  ${DocumentSourceFiles}
  ${InputSourceFiles}
  ${ModeSourceFiles}
  ${ScriptingSourceFiles}
  ${SelectionSourceFiles}
  ${ServiceSourceFiles}
  ${SyntaxSourceFiles}
  ${TransactionSourceFiles}
  ${UtilitySourceFiles}
)

set_target_properties(Quip.Core PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
target_include_directories(Quip.Core PRIVATE ../../Dependencies/optional-lite)

target_link_libraries(Quip.Core Lua)
