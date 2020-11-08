# RTNIOW
Ray Tracing (Not) in One Weekend

## Motivation
I've always had an interest in ray tracing and rendering, and now I'm taking a
stab at it. In addition to ray tracing, I am also pushing as much as possible
into the compilation step, by leveraging constexpr compliant code. 

## Requirements
RTNIOW aims to be as lightweight as possible, but there are some dependencies:
   1. git (for CMake FetchContent)
   2. CMake (>=3.16)
   3. A modern c++ compiler (clang-10, gcc-10 or greater) with concept support
    
This project provides a dockerfile to build this project if your system does not 
have a modern enough compiler.

## Dependencies
Catch2 is the unit test framework, it is fetched by CMake at configuration time.
