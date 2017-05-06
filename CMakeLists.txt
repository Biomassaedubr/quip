cmake_minimum_required(VERSION 3.0)
project(Quip)

find_library(CocoaFramework Cocoa)
find_library(CoreGraphicsFramework CoreGraphics)

add_subdirectory("Dependencies/lpeg")
add_subdirectory("Dependencies/lua")

add_subdirectory("Projects/Core")
add_subdirectory("Projects/Core.Tests")
add_subdirectory("Projects/Launcher")
add_subdirectory("Projects/Quip")
