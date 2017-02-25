cmake_minimum_required(VERSION 2.6)
project(Quip)

set(QuipCoreSourceFiles
  Source/Core/AppendTransaction.cpp
  Source/Core/AppendTransaction.hpp
  Source/Core/AttributeRange.cpp
  Source/Core/AttributeRange.hpp
  Source/Core/ChangeType.hpp
  Source/Core/Color.cpp
  Source/Core/Color.hpp
  Source/Core/Coordinate.cpp
  Source/Core/Coordinate.hpp
  Source/Core/CursorFlags.hpp
  Source/Core/CursorStyle.hpp
  Source/Core/Document.cpp
  Source/Core/Document.hpp
  Source/Core/DocumentIterator.cpp
  Source/Core/DocumentIterator.hpp
  Source/Core/DocumentIterator.inl
  Source/Core/DrawingService.cpp
  Source/Core/DrawingService.hpp
  Source/Core/EditContext.cpp
  Source/Core/EditContext.hpp
  Source/Core/EditMode.cpp
  Source/Core/EditMode.hpp
  Source/Core/EraseTransaction.cpp
  Source/Core/EraseTransaction.hpp
  Source/Core/Extent.cpp
  Source/Core/Extent.hpp
  Source/Core/FileTypeDatabase.cpp
  Source/Core/FileTypeDatabase.hpp
  Source/Core/GlobalSettings.cpp
  Source/Core/GlobalSettings.hpp
  Source/Core/InsertTransaction.cpp
  Source/Core/InsertTransaction.hpp
  Source/Core/JumpMode.cpp
  Source/Core/JumpMode.hpp
  Source/Core/Key.cpp
  Source/Core/Key.hpp
  Source/Core/KeySequence.cpp
  Source/Core/KeySequence.hpp
  Source/Core/Location.cpp
  Source/Core/Location.hpp
  Source/Core/Lua.hpp
  Source/Core/MapTrie.cpp
  Source/Core/MapTrie.hpp
  Source/Core/MapTrieNode.cpp
  Source/Core/MapTrieNode.hpp
  Source/Core/Mode.cpp
  Source/Core/Mode.hpp
  Source/Core/NormalMode.cpp
  Source/Core/NormalMode.hpp
  Source/Core/Optional.hpp
  Source/Core/PopupService.cpp
  Source/Core/PopupService.hpp
  Source/Core/Rectangle.cpp
  Source/Core/Rectangle.hpp
  Source/Core/Script.cpp
  Source/Core/Script.hpp
  Source/Core/ScriptHost.cpp
  Source/Core/ScriptHost.hpp
  Source/Core/SearchExpression.cpp
  Source/Core/SearchExpression.hpp
  Source/Core/SearchMode.cpp
  Source/Core/SearchMode.hpp
  Source/Core/Selection.cpp
  Source/Core/Selection.hpp
  Source/Core/SelectionDrawInfo.cpp
  Source/Core/SelectionDrawInfo.hpp
  Source/Core/SelectionSet.cpp
  Source/Core/SelectionSet.hpp
  Source/Core/SelectionSetIterator.hpp
  Source/Core/SelectionSetIterator.inl
  Source/Core/Selector.cpp
  Source/Core/Selector.hpp
  Source/Core/Signal.hpp
  Source/Core/StatusService.cpp
  Source/Core/StatusService.hpp
  Source/Core/Transaction.cpp
  Source/Core/Transaction.hpp
  Source/Core/ViewController.hpp
)

set(QuipTestSourceFiles
  Source/Tests/CoordinateTests.cpp
  Source/Tests/DocumentIteratorTests.cpp
  Source/Tests/DocumentTests.cpp
  Source/Tests/ExtentTests.cpp
  Source/Tests/KeySequenceTests.cpp
  Source/Tests/LocationTests.cpp
  Source/Tests/SearchExpressionTests.cpp
  Source/Tests/SelectionSetTests.cpp
  Source/Tests/SelectionTests.cpp
  Source/Tests/SelectorTests.cpp
  Source/Tests/SignalTests.cpp
  Source/Tests/main.cpp
)

set(QuipApplicationSourceFiles
  Dependencies/lpeg/lpeg.so
  Resources/Images.xcassets
  Source/Mac/DrawingServiceProvider.hpp
  Source/Mac/DrawingServiceProvider.mm
  Source/Mac/Info.plist.in
  Source/Mac/MainMenu.xib
  Source/Mac/PopupServiceProvider.hpp
  Source/Mac/PopupServiceProvider.mm
  Source/Mac/QuipApplicationDelegate.h
  Source/Mac/QuipApplicationDelegate.mm
  Source/Mac/QuipDocument.h
  Source/Mac/QuipDocument.mm
  Source/Mac/QuipDocumentController.h
  Source/Mac/QuipDocumentController.mm
  Source/Mac/QuipPopupView.h
  Source/Mac/QuipPopupView.mm
  Source/Mac/QuipStatusView.h
  Source/Mac/QuipStatusView.mm
  Source/Mac/QuipTextView.h
  Source/Mac/QuipTextView.mm
  Source/Mac/QuipWindowController.h
  Source/Mac/QuipWindowController.mm
  Source/Mac/QuipWindowController.xib
  Source/Mac/StatusServiceProvider.hpp
  Source/Mac/StatusServiceProvider.mm
  Source/Mac/main.m
)

set(QuipRuntimeFiles
  Source/Runtime/boot.lua
  Source/Runtime/syntax.lua
  Source/Runtime/syntax/cpp.lua
  Source/Runtime/syntax/glsl.lua
  Source/Runtime/syntax/markdown.lua
  Source/Runtime/syntax/text.lua
)

set(QuipLauncherSourceFiles
  Launcher/main.m
)

set(RESOURCE
  Resources/Images.xcassets
)

find_library(CocoaFramework Cocoa)
find_library(CoreGraphicsFramework CoreGraphics)

add_library(Quip.Core ${QuipCoreSourceFiles})
set_target_properties(Quip.Core PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
target_include_directories(Quip.Core PRIVATE Dependencies/lua/include)
target_include_directories(Quip.Core PRIVATE Dependencies/optional-lite)


add_executable(Quip.Tests ${QuipTestSourceFiles})
set_target_properties(Quip.Tests PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
target_include_directories(Quip.Tests PRIVATE Dependencies/catch)
target_include_directories(Quip.Tests PRIVATE Dependencies/optional-lite)
target_include_directories(Quip.Tests PRIVATE Source/Core)
target_link_libraries(Quip.Tests PRIVATE Quip.Core)


add_executable(Quip.Launcher ${QuipLauncherSourceFiles})
target_link_libraries(Quip.Launcher PRIVATE ${CocoaFramework})


add_executable(Quip MACOSX_BUNDLE ${QuipApplicationSourceFiles} ${QuipRuntimeFiles})
set_target_properties(Quip PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
set_target_properties(Quip PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Source/Mac/Info.plist.in)

target_compile_options(Quip PRIVATE "-fobjc-arc")
target_include_directories(Quip PRIVATE Dependencies/lua/include)
target_include_directories(Quip PRIVATE Dependencies/optional-lite)
target_include_directories(Quip PRIVATE Source/Core)
target_link_libraries(Quip PRIVATE Quip.Core)
target_link_libraries(Quip PRIVATE ${CMAKE_SOURCE_DIR}/Dependencies/lua/bin/liblua.a)
target_link_libraries(Quip PRIVATE ${CocoaFramework} ${CoreGraphicsFramework})

set_target_properties(Quip PROPERTIES RESOURCE Resources/Images.xcassets)
set_target_properties(Quip PROPERTIES XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME AppIcon)

set_source_files_properties(Source/Mac/MainMenu.xib PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
set_source_files_properties(Source/Mac/QuipWindowController.xib PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

set_source_files_properties(Source/Runtime/boot.lua PROPERTIES MACOSX_PACKAGE_LOCATION Resources/Runtime)
set_source_files_properties(Source/Runtime/syntax.lua PROPERTIES MACOSX_PACKAGE_LOCATION Resources/Runtime)
set_source_files_properties(Source/Runtime/syntax/cpp.lua PROPERTIES MACOSX_PACKAGE_LOCATION Resources/Runtime/syntax)
set_source_files_properties(Source/Runtime/syntax/glsl.lua PROPERTIES MACOSX_PACKAGE_LOCATION Resources/Runtime/syntax)
set_source_files_properties(Source/Runtime/syntax/markdown.lua PROPERTIES MACOSX_PACKAGE_LOCATION Resources/Runtime/syntax)
set_source_files_properties(Source/Runtime/syntax/text.lua PROPERTIES MACOSX_PACKAGE_LOCATION Resources/Runtime/syntax)

set_source_files_properties(Dependencies/lpeg/lpeg.so PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
