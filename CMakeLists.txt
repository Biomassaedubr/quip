cmake_minimum_required(VERSION 2.6)
project(Quip)

find_library(CocoaFramework Cocoa)
find_library(CoreGraphicsFramework CoreGraphics)

add_subdirectory("Dependencies/lua")

add_subdirectory("Projects/Core")
add_subdirectory("Projects/Core.Tests")
add_subdirectory("Projects/Launcher")
add_subdirectory("Projects/Quip")
