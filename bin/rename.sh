#! /bin/bash

a=1
for i in ./files/*; do
  ext="${i##*.}"
  new=$(printf "./files/%04d.${ext}" "$a") #04 pad to length of 4
  mv -- "$i" "$new"
  let a=a+1
done
