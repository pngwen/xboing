#!/bin/bash

while IFS= read -r file; do
    # Compile the file to object code
    if gcc -c -I "include" "$file" 2>/dev/null; then
        echo "$file: succeed"
        rm -f "${file%.c}.o"
        rm -f "${file}.gch"
    else
        echo "$file: failed"
    fi
done < group3-files.txt