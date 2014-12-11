#! /bin/bash

if [ "$#" -ne "1" ];then
  echo "Usage : $0 your-file.totient"
  exit 0
fi

head -n 4 "$1" > tmp

while read tracker name  piece_length length;do
  echo $tracker $name $piece_length $length
done < tmp

tail -n +5 $1 > tmp

TARGET=$(echo $1 | awk -F "/" '{print $3}')
TARGET=./files/$(echo $TARGET | awk -F "." '{print $1}').$(echo $TARGET | awk -F "." '{print $2}')

while read hash;do
  cat pieces/$hash >> $TARGET
done < tmp

rm tmp

