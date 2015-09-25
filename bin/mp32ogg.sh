#! /bin/bash

for file in ./files/*.mp3
  do ffmpeg -i "${file}" "${file/%mp3/ogg}"
done
