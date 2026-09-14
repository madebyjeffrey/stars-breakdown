#!/bin/sh
set -eu

runtime_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
project_dir=$(dirname -- "$runtime_dir")
cd "$project_dir/runs/step4"

exec "$project_dir/.tools/StarsRuntime.app/Contents/MacOS/dosbox-x" \
    -nopromptfolder -fastlaunch -conf "$runtime_dir/win95.conf" "$@"
