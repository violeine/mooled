#!/usr/bin/env bash

for brightness in {1..255}
do
  node client.js -B $brightness
done
