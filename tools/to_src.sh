#!/usr/bin/env bash

BASE_DIR=$(pwd)
if [ "$BASE_DIR" = "/" ]; then
  BASE_DIR=""
fi

# Copy u8g2lib files to src, cmake load files from src directory.
U8G2LIB="$BASE_DIR/../libraries/U8g2lib"
FROM1="$BASE_DIR/../libraries/U8g2lib/cppsrc"
FROM2="$BASE_DIR/../libraries/U8g2lib/csrc"
SRC="$U8G2LIB/src"

if [ ! -d $SRC ]; then
  echo "SRC not exists, create $SRC"
  mkdir $SRC
else
  echo "Clean SRC dir $SRC"
  find $SRC -type f -delete
fi

echo "Copy from $FROM1"
cp -aT $FROM1 $SRC
echo "Copy from $FROM2"
cp -aT $FROM2 $SRC
