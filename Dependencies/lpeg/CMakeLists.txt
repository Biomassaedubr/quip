set(LPegDistribution lpeg-1.0.1)

set(SourceFiles
  ${LPegDistribution}/lpcap.c
  ${LPegDistribution}/lpcap.h
  ${LPegDistribution}/lpcode.c
  ${LPegDistribution}/lpcode.h
  ${LPegDistribution}/lpprint.c
  ${LPegDistribution}/lpprint.h
  ${LPegDistribution}/lptree.c
  ${LPegDistribution}/lptree.h
  ${LPegDistribution}/lptypes.h
  ${LPegDistribution}/lpvm.c
  ${LPegDistribution}/lpvm.h
)
source_group(Code FILES ${SourceFiles})

# Include distribution makefile, even though it isn't used, for reference.
set(ReferenceFiles
  ${LPegDistribution}/makefile
)
source_group(Reference FILES ${ReferenceFiles})

add_library(LPeg MODULE ${SourceFiles} ${ReferenceFiles})
target_include_directories(LPeg PRIVATE "$<TARGET_PROPERTY:Lua,INTERFACE_INCLUDE_DIRECTORIES>")
target_link_libraries(LPeg "-undefined dynamic_lookup")
