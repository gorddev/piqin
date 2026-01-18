#!/bin/bash

if [ "../.protected/run.sh" -nt "./run.sh" ]; then
  echo "Updating ./run.sh file"
  cp -r "../.protected/run.sh" "./run.sh"
  sh ./run.sh
else

  # Recreate directory structure of ../assets/ in ./assets/
  find "../assets/" -type d -print0 | while IFS= read -r -d '' dir; do
      target_dir="./assets/${dir#../assets/}"
      mkdir -p "$target_dir"
  done

  # Copy all .png files into ./assets/ mirror
  find "../assets/" -type f -name "*.png" -print0 | while IFS= read -r -d '' file; do
      target_file="./assets/${file#../assets/}"
      cp "$file" "$target_file"
  done

  find "../assets/" -type f -name "*.ttf" -print0 | while IFS= read -r -d '' file; do
        target_file="./assets/${file#../assets/}"
        cp "$file" "$target_file"
    done

  # Convert all .world files and place .lvl in the ./assets/ mirror
  find "../assets/" -type f -name "*.world" -print0 | while IFS= read -r -d '' file; do
      world_dir="$(dirname "$file")"
      world_base="$(basename "$file")"
      target_dir="./assets/${world_dir#../assets/}"
      mkdir -p "$target_dir"
      echo "Worldbase: $world_base"
      echo "target_dir: $target_dir/${world_base%.*}"
      (
        cd "$world_dir" || exit 1
        ../../../.protected/json-importer/level-converter \
          "$world_base" \
          "$OLDPWD/$target_dir/${world_base%.*}.lvl"
      )
  done

  cmake --build . && codesign --force --sign - ./norton && ./norton
fi