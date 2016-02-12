#!/bin/sh
cd "$PROJECT_DIR/XcodeBuild/" #$TARGET_NAME/"
case $1 in
  clean)
    scons -c
    ;;
  *)
    scons
    ;;
esac