#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/neko.png
ICON_DST=../../src/qt/res/icons/neko.ico
convert ${ICON_SRC} -resize 16x16 neko-16.png
convert ${ICON_SRC} -resize 32x32 neko-32.png
convert ${ICON_SRC} -resize 48x48 neko-48.png
convert neko-16.png neko-32.png neko-48.png ${ICON_DST}

