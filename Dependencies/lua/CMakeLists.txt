set(LuaDistribution lua-5.3.4)

set(SourceFiles
  ${LuaDistribution}/src/lapi.c
  ${LuaDistribution}/src/lapi.h
  ${LuaDistribution}/src/lauxlib.c
  ${LuaDistribution}/src/lauxlib.h
  ${LuaDistribution}/src/lbaselib.c
  ${LuaDistribution}/src/lbitlib.c
  ${LuaDistribution}/src/lcode.c
  ${LuaDistribution}/src/lcode.h
  ${LuaDistribution}/src/lcorolib.c
  ${LuaDistribution}/src/lctype.c
  ${LuaDistribution}/src/lctype.h
  ${LuaDistribution}/src/ldblib.c
  ${LuaDistribution}/src/ldebug.c
  ${LuaDistribution}/src/ldebug.h
  ${LuaDistribution}/src/ldo.c
  ${LuaDistribution}/src/ldo.h
  ${LuaDistribution}/src/ldump.c
  ${LuaDistribution}/src/lfunc.c
  ${LuaDistribution}/src/lfunc.h
  ${LuaDistribution}/src/lgc.c
  ${LuaDistribution}/src/lgc.h
  ${LuaDistribution}/src/linit.c
  ${LuaDistribution}/src/liolib.c
  ${LuaDistribution}/src/llex.c
  ${LuaDistribution}/src/llex.h
  ${LuaDistribution}/src/lmathlib.c
  ${LuaDistribution}/src/lmem.c
  ${LuaDistribution}/src/lmem.h
  ${LuaDistribution}/src/loadlib.c
  ${LuaDistribution}/src/lobject.c
  ${LuaDistribution}/src/lobject.h
  ${LuaDistribution}/src/lopcodes.c
  ${LuaDistribution}/src/lopcodes.h
  ${LuaDistribution}/src/loslib.c
  ${LuaDistribution}/src/lparser.c
  ${LuaDistribution}/src/lparser.h
  ${LuaDistribution}/src/lstate.c
  ${LuaDistribution}/src/lstate.h
  ${LuaDistribution}/src/lstring.c
  ${LuaDistribution}/src/lstring.h
  ${LuaDistribution}/src/lstrlib.c
  ${LuaDistribution}/src/ltable.c
  ${LuaDistribution}/src/ltable.h
  ${LuaDistribution}/src/ltablib.c
  ${LuaDistribution}/src/ltm.c
  ${LuaDistribution}/src/ltm.h
  ${LuaDistribution}/src/lundump.c
  ${LuaDistribution}/src/lundump.h
  ${LuaDistribution}/src/lutf8lib.c
  ${LuaDistribution}/src/lvm.c
  ${LuaDistribution}/src/lvm.h
  ${LuaDistribution}/src/lzio.c
  ${LuaDistribution}/src/lzio.h
)
source_group(Code FILES ${SourceFiles})

# Include distribution makefile, even though it isn't used, for reference.
set(ReferenceFiles
  ${LuaDistribution}/src/Makefile
)
source_group(Reference FILES ${ReferenceFiles})

add_library(Lua STATIC ${SourceFiles} ${ReferenceFiles})

target_include_directories(Lua INTERFACE ${CMAKE_CURRENT_LIST_DIR}/${LuaDistribution}/src)
