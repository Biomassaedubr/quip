set(SourceFiles
  CoordinateTests.cpp
  DocumentIteratorTests.cpp
  DocumentTests.cpp
  ExtentTests.cpp
  KeySequenceTests.cpp
  LocationTests.cpp
  main.cpp
  SearchExpressionTests.cpp
  SelectionSetTests.cpp
  SelectionTests.cpp
  SelectorTests.cpp
  SignalTests.cpp
)

add_executable(Quip.Tests ${SourceFiles})
set_target_properties(Quip.Tests PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
target_include_directories(Quip.Tests PRIVATE ../../Dependencies/catch)
target_include_directories(Quip.Tests PRIVATE ../../Dependencies/optional-lite)
target_include_directories(Quip.Tests PRIVATE ../Core)
target_link_libraries(Quip.Tests PRIVATE Quip.Core)
