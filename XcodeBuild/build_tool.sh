#!/bin/sh
cd "$PROJECT_DIR/XcodeBuild"
case $1 in
  clean)
    scons -c
    ;;
  *)
    scons
    ;;
esac