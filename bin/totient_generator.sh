#! /bin/bash

if [ "$#" -lt 1 ]; then
  echo "Usage $0 filename [tracker ip] [tracker port]"
  exit 1
fi

CHUNK_SIZE=256000
split -b $CHUNK_SIZE $1 current

TRACKER=localhost
if [ "$#" -gt 1 ]; then
  TRACKER=$2
fi

PORT=6666

if [ "$#" -gt 2 ]; then
  PORT=$3
fi

TARGET=totient/$(echo $1 | awk -F "/" '{print $2}').totient
echo $TRACKER:$PORT > $TARGET
echo $1 >> $TARGET
echo $CHUNK_SIZE >> $TARGET
echo $(du -b $1 | awk '{ print $1 }') >> $TARGET

for i in current*; do
  name=$(sha1sum $i | awk '{ print $1 }')
  echo $name >> pieces/list
  echo $name >> $TARGET
  mv $i pieces/$name
done
