if [ "../.protected/run.sh" -nt "./run.sh" ]; then
  echo "Updating ./run.sh file"
  cp -r "../.protected/run.sh" "./run.sh"
  sh ./run.sh

else
  source /Users/gordie/emsdk/emsdk_env.sh EMSDK_QUIET=1
  find "../assets/" -type f -print0 | while IFS= read -r -d '' file; do
    rel="${file#assets/}"

    dir="$(dirname "$rel")"
    fname="$(basename "$rel")"
    base="${fname%.*}"
    outdir="assets/$dir"
    out="$outdir/${base}.ktx2"

    mkdir -p "$outdir"

    if [[ $fname == *.png ]]; then
      if [[ $file -nt $out ]]; then
        echo "Updating basis: $rel › $out"
        basisu -ktx2 -uastc -file "$rel"  -output_file "$out" > /dev/null
      fi
    elif [[ $fname == *.png ]]; then
      echo "Converting PNG: $rel › $out"
      basisu -ktx2 -uastc -file "$rel"  -output_file "$out" > /dev/null
    fi
  done

  emrun norton.html
fi