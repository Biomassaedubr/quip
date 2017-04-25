
set(SourceFiles
  main.m
)

add_executable(Quip.Launcher ${SourceFiles})
target_link_libraries(Quip.Launcher PRIVATE ${CocoaFramework})
