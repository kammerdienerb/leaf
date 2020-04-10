#!/usr/bin/env bash

gcc leaf.c $(pkg-config --cflags freetype2) -g -lfreetype -lSDL2 -lOpenGL -o leaf
