#!/usr/bin/env bash

fin=profile_fifo_in
mkfifo $fin
trap "rm -f $fin" EXIT

LEAF_PAUSE="yes" ./leaf <$fin &
THE_PID=$!
flame-prof -p $THE_PID > profile.svg &
sleep 0.1
echo "" > $fin
wait
firefox-nightly $(realpath profile.svg)
