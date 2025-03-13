#!/bin/bash

############################################
# ! - Run this at the root of the repo - ! #
############################################

find . \( -path "./components/third-party" -o -path "./build" -o -path "./managed_components" \) -prune -o \( -name "*.cpp" -o -name "*.h" \) -print0 |
    while IFS= read -r -d '' file; do
        echo "Formatting $file"
        clang-format -i "$file"
    done

