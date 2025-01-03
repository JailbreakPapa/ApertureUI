#!/usr/bin/env bash
set -eo pipefail

this_dir="$( cd "$( dirname "$0" )" && pwd )"
repo_dir="$(realpath "${this_dir}/../")"

find "${repo_dir}" -type f -name '*.onnx' -exec basename '{}' '.onnx' \; | sort | uniq
