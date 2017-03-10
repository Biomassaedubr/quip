cmake_minimum_required(VERSION 2.6)
project(Quip)

set(CoreSourceFiles
  Code/Core/Documents/Document.cpp
  Code/Core/Documents/Document.hpp
  Code/Core/Documents/DocumentIterator.cpp
  Code/Core/Documents/DocumentIterator.hpp
  Code/Core/Documents/DocumentIterator.inl
  Code/Core/Documents/EditContext.cpp
  Code/Core/Documents/EditContext.hpp
  Code/Core/Documents/FileTypeDatabase.cpp
  Code/Core/Documents/FileTypeDatabase.hpp
  Code/Core/Modes/CursorFlags.hpp
  Code/Core/Modes/CursorStyle.hpp
  Code/Core/Modes/Key.cpp
  Code/Core/Modes/Key.hpp
  Code/Core/Modes/KeySequence.cpp
  Code/Core/Modes/KeySequence.hpp
  Code/Core/Modes/MapTrie.cpp
  Code/Core/Modes/MapTrie.hpp
  Code/Core/Modes/MapTrieNode.cpp
  Code/Core/Modes/MapTrieNode.hpp
  Code/Core/Modes/Mode.cpp
  Code/Core/Modes/Mode.hpp
  Code/Core/Modes/Edit/EditMode.cpp
  Code/Core/Modes/Edit/EditMode.hpp
  Code/Core/Modes/Jump/JumpMode.cpp
  Code/Core/Modes/Jump/JumpMode.hpp
  Code/Core/Modes/Normal/NormalMode.cpp
  Code/Core/Modes/Normal/NormalMode.hpp
  Code/Core/Modes/Search/SearchExpression.cpp
  Code/Core/Modes/Search/SearchExpression.hpp
  Code/Core/Modes/Search/SearchMode.cpp
  Code/Core/Modes/Search/SearchMode.hpp
  Code/Core/Scripting/Lua.hpp
  Code/Core/Scripting/Script.cpp
  Code/Core/Scripting/Script.hpp
  Code/Core/Scripting/ScriptHost.cpp
  Code/Core/Scripting/ScriptHost.hpp
  Code/Core/Selections/Selection.cpp
  Code/Core/Selections/Selection.hpp
  Code/Core/Selections/SelectionDrawInfo.cpp
  Code/Core/Selections/SelectionDrawInfo.hpp
  Code/Core/Selections/SelectionSet.cpp
  Code/Core/Selections/SelectionSet.hpp
  Code/Core/Selections/SelectionSetIterator.hpp
  Code/Core/Selections/SelectionSetIterator.inl
  Code/Core/Selections/Selector.cpp
  Code/Core/Selections/Selector.hpp
  Code/Core/Services/DrawingService.cpp
  Code/Core/Services/DrawingService.hpp
  Code/Core/Services/PopupService.cpp
  Code/Core/Services/PopupService.hpp
  Code/Core/Services/StatusService.cpp
  Code/Core/Services/StatusService.hpp
  Code/Core/Settings/GlobalSettings.cpp
  Code/Core/Settings/GlobalSettings.hpp
  Code/Core/Transactions/AppendTransaction.cpp
  Code/Core/Transactions/AppendTransaction.hpp
  Code/Core/Transactions/EraseTransaction.cpp
  Code/Core/Transactions/EraseTransaction.hpp
  Code/Core/Transactions/InsertTransaction.cpp
  Code/Core/Transactions/InsertTransaction.hpp
  Code/Core/Transactions/Transaction.cpp
  Code/Core/Transactions/Transaction.hpp
  Code/Core/Utilities/AttributeRange.cpp
  Code/Core/Utilities/AttributeRange.hpp
  Code/Core/Utilities/Color.cpp
  Code/Core/Utilities/Color.hpp
  Code/Core/Utilities/Coordinate.cpp
  Code/Core/Utilities/Coordinate.hpp
  Code/Core/Utilities/Extent.cpp
  Code/Core/Utilities/Extent.hpp
  Code/Core/Utilities/Location.cpp
  Code/Core/Utilities/Location.hpp
  Code/Core/Utilities/Optional.hpp
  Code/Core/Utilities/Rectangle.cpp
  Code/Core/Utilities/Rectangle.hpp
  Code/Core/Utilities/Signal.hpp
  Code/Core/Utilities/ViewController.hpp
)

set(TestSourceFiles
  Code/Tests/CoordinateTests.cpp
  Code/Tests/DocumentIteratorTests.cpp
  Code/Tests/DocumentTests.cpp
  Code/Tests/ExtentTests.cpp
  Code/Tests/KeySequenceTests.cpp
  Code/Tests/LocationTests.cpp
  Code/Tests/SearchExpressionTests.cpp
  Code/Tests/SelectionSetTests.cpp
  Code/Tests/SelectionTests.cpp
  Code/Tests/SelectorTests.cpp
  Code/Tests/SignalTests.cpp
  Code/Tests/main.cpp
)

set(ApplicationSourceFiles
  Dependencies/lpeg/lpeg.so
  Resources/Images.xcassets
  Code/Mac/DrawingServiceProvider.hpp
  Code/Mac/DrawingServiceProvider.mm
  Code/Mac/Info.plist.in
  Code/Mac/MainMenu.xib
  Code/Mac/PopupServiceProvider.hpp
  Code/Mac/PopupServiceProvider.mm
  Code/Mac/QuipApplicationDelegate.h
  Code/Mac/QuipApplicationDelegate.mm
  Code/Mac/QuipDocument.h
  Code/Mac/QuipDocument.mm
  Code/Mac/QuipDocumentController.h
  Code/Mac/QuipDocumentController.mm
  Code/Mac/QuipPopupView.h
  Code/Mac/QuipPopupView.mm
  Code/Mac/QuipStatusView.h
  Code/Mac/QuipStatusView.mm
  Code/Mac/QuipTextView.h
  Code/Mac/QuipTextView.mm
  Code/Mac/QuipWindowController.h
  Code/Mac/QuipWindowController.mm
  Code/Mac/QuipWindowController.xib
  Code/Mac/StatusServiceProvider.hpp
  Code/Mac/StatusServiceProvider.mm
  Code/Mac/main.m
)

set(LauncherSourceFiles
  Code/Launcher/main.m
)

set(RuntimeFiles
  Code/Runtime/boot.lua
  Code/Runtime/syntax.lua
  Code/Runtime/syntax/cpp.lua
  Code/Runtime/syntax/glsl.lua
  Code/Runtime/syntax/markdown.lua
  Code/Runtime/syntax/text.lua
)

set(RESOURCE
  Resources/Images.xcassets
)

function(process_source_files SourceFiles IncludePathVariable)
  foreach(Item ${SourceFiles})
    get_filename_component(ItemName ${Item} NAME_WE)
    get_filename_component(ItemExtension ${Item} EXT)
    get_filename_component(ItemDirectory ${Item} DIRECTORY)
    get_filename_component(ItemAbsolute ${Item} ABSOLUTE)

    if(${ItemDirectory} MATCHES "^Runtime/.*")
      set_source_files_properties(${Item} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/${ItemDirectory}")
    elseif(${ItemExtension} STREQUAL ".h")
      list(APPEND LocalIncludePaths ${ItemDirectory})
    elseif(${ItemExtension} STREQUAL ".hpp")
      list(APPEND LocalIncludePaths ${ItemDirectory})
    elseif(${ItemExtension} STREQUAL ".xib")
      set_source_files_properties(${Item} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    endif()

    string(REPLACE "/" "\\" GroupName ${ItemDirectory})
    string(REPLACE "Code\\" "" GroupName ${GroupName})
    source_group(${GroupName} FILES ${Item})
  endforeach()

  set(${IncludePathVariable} ${LocalIncludePaths} PARENT_SCOPE)
endfunction()

process_source_files("${CoreSourceFiles}" CoreIncludeDirectories)
process_source_files("${TestSourceFiles}" TestIncludeDirectories)
process_source_files("${ApplicationSourceFiles}" ApplicationIncludeDirectories)
process_source_files("${RuntimeFiles}" RuntimeIncludeDirectories)

find_library(CocoaFramework Cocoa)
find_library(CoreGraphicsFramework CoreGraphics)

add_library(Quip.Core ${CoreSourceFiles})
set_target_properties(Quip.Core PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
target_include_directories(Quip.Core PRIVATE ${CoreIncludeDirectories})
target_include_directories(Quip.Core PRIVATE Dependencies/lua/include)
target_include_directories(Quip.Core PRIVATE Dependencies/optional-lite)

add_executable(Quip.Tests ${TestSourceFiles})
set_target_properties(Quip.Tests PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
target_include_directories(Quip.Tests PRIVATE ${CoreIncludeDirectories})
target_include_directories(Quip.Tests PRIVATE ${TestIncludeDirectories})
target_include_directories(Quip.Tests PRIVATE Dependencies/catch)
target_include_directories(Quip.Tests PRIVATE Dependencies/optional-lite)
target_link_libraries(Quip.Tests PRIVATE Quip.Core)

add_executable(Quip.Launcher ${LauncherSourceFiles})
target_link_libraries(Quip.Launcher PRIVATE ${CocoaFramework})

add_executable(Quip MACOSX_BUNDLE ${ApplicationSourceFiles} ${RuntimeFiles})
set_target_properties(Quip PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
set_target_properties(Quip PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Code/Mac/Info.plist.in)
target_compile_options(Quip PRIVATE "-fobjc-arc")
target_include_directories(Quip PRIVATE ${ApplicationIncludeDirectories})
target_include_directories(Quip PRIVATE ${CoreIncludeDirectories})
target_include_directories(Quip PRIVATE Dependencies/lua/include)
target_include_directories(Quip PRIVATE Dependencies/optional-lite)
target_link_libraries(Quip PRIVATE Quip.Core)
target_link_libraries(Quip PRIVATE ${CMAKE_SOURCE_DIR}/Dependencies/lua/bin/liblua.a)
target_link_libraries(Quip PRIVATE ${CocoaFramework} ${CoreGraphicsFramework})

set_target_properties(Quip PROPERTIES RESOURCE Resources/Images.xcassets)
set_target_properties(Quip PROPERTIES XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME AppIcon)

set_source_files_properties(Dependencies/lpeg/lpeg.so PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
