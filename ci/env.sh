#!/usr/bin/env sh

if [ "$CXX" = "g++" ]; then export CXX="g++" CC="gcc"; fi
if [ "$CXX" = "clang++" ]; then export CXX="clang++" CC="clang"; fi
