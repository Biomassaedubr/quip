cmake_minimum_required(VERSION 2.6)
project(Quip)

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

    if(${ItemDirectory} MATCHES "^Code/Runtime")
      string(REPLACE "Code/" "Resources/" ResourceDirectory ${ItemDirectory})
      set_source_files_properties(${Item} PROPERTIES MACOSX_PACKAGE_LOCATION "${ResourceDirectory}")
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

process_source_files("${ApplicationSourceFiles}" ApplicationIncludeDirectories)
process_source_files("${RuntimeFiles}" RuntimeIncludeDirectories)

find_library(CocoaFramework Cocoa)
find_library(CoreGraphicsFramework CoreGraphics)

add_subdirectory("Code/Core")
add_subdirectory("Code/Launcher")
add_subdirectory("Code/Tests")

add_executable(Quip MACOSX_BUNDLE ${ApplicationSourceFiles} ${RuntimeFiles})
set_target_properties(Quip PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
set_target_properties(Quip PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Code/Mac/Info.plist.in)
target_compile_options(Quip PRIVATE "-fobjc-arc")
target_include_directories(Quip PRIVATE ${ApplicationIncludeDirectories})
target_include_directories(Quip PRIVATE ${CoreIncludeDirectories})
target_include_directories(Quip PRIVATE Dependencies/lua/include)
target_include_directories(Quip PRIVATE Dependencies/optional-lite)
target_include_directories(Quip PRIVATE Code/Core)
target_link_libraries(Quip PRIVATE Quip.Core)
target_link_libraries(Quip PRIVATE ${CMAKE_SOURCE_DIR}/Dependencies/lua/bin/liblua.a)
target_link_libraries(Quip PRIVATE ${CocoaFramework} ${CoreGraphicsFramework})

set_target_properties(Quip PROPERTIES RESOURCE Resources/Images.xcassets)
set_target_properties(Quip PROPERTIES XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME AppIcon)

set_source_files_properties(Dependencies/lpeg/lpeg.so PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
