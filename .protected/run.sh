#!/bin/bash

if [ "../.protected/run.sh" -nt "./run.sh" ]; then
  echo "Updating ./run.sh file"
  cp -r "../.protected/run.sh" "./run.sh"
  sh ./run.sh
else
  # Set up Emscripten environment
  source /Users/gordie/emsdk/emsdk_env.sh EMSDK_QUIET=1

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

  # Convert all .world files and place .lvl in the ./assets/ mirror
  find "../assets/" -type f -name "*.world" -print0 | while IFS= read -r -d '' file; do
      target_dir="./assets/$(dirname "${file#../assets/}")"
      mkdir -p "$target_dir"  # ensure target directory exists
      ../.protected/json-importer/level-converter "$file" "$target_dir/$(basename "${file%.*}.lvl")"
  done

  # Finally, run emrun
  emrun --browser=chrome norton.html
fi