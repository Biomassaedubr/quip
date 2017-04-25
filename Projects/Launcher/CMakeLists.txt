set(SourceFiles
  main.m
)
source_group(Code FILES ${SourceFiles})

add_executable(Quip.Launcher ${SourceFiles})
target_link_libraries(Quip.Launcher PRIVATE ${CocoaFramework})
