#!/bin/sh

cd man

TO=$1
VERSION="0.5"
DATE=`date "+%Y-%m-%d"`
ZIP="gzip -f9"

test -z $TO  && TO="/usr/share/man"

echo -n "Installing Manpages: "
for t in 1 2 3 4 5 6 7 8; do
 for i in *.$t; do
 test -f $i || continue
 echo -n "$i "
 cat $i | sed \
  -e "s/%VER%/${VERSION}/g" \
  -e "s/%DATE%/${DATE}/g" | \
  $ZIP > "$TO/man$t/$i.gz"
 done
done
echo "done!"
