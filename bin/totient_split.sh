#! /bin/bash

if [ "$#" -lt 1 ]; then
  echo "Usage $0 filename [tracker ip] [tracker port]"
  exit 1
fi

split -b 256000 $1 current

for i in current*; do
  name=$(sha1sum $i | awk '{ print $1 }')
  echo $name >> pieces/list
  mv $i pieces/$name
done

