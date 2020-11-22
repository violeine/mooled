#!/usr/bin/env bash

weather=$(curl -s wttr.in/$1?format=%C)
weather=${weather,,}
echo $weather
case $weather
  in
  *cloudy* )
    node client.js -S "#c0dfff"
    ;;
  *rain* )
    node client.js -S "#3c5369"
    ;;
  *sun* | *clear* )
    node client.js -S "#ffb400"
    ;;
  *freezing* )
    echo "runnging"
    node client.js -S "#40bcd8"
    ;;

esac
