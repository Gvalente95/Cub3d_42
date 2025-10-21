#!/bin/bash

root="${1:-.}"

while IFS= read -r -d '' dir; do
  rm -f "$dir/46.xpm"
  [ -f "$dir/45.xpm" ] && cp -f "$dir/45.xpm" "$dir/46.xpm"

  rm -f "$dir/57.xpm"
  [ -f "$dir/56.xpm" ] && cp -f "$dir/56.xpm" "$dir/57.xpm"

  rm -f "$dir/68.xpm"
  [ -f "$dir/67.xpm" ] && cp -f "$dir/67.xpm" "$dir/68.xpm"
done < <(find "$root" -type d -print0)
