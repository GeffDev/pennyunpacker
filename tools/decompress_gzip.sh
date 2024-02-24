#!/bin/bash

function decompress_if_gzipped() {
    local file="$1"
    local mime_type=$(file -b --mime-type "$file")

    if [ "$mime_type" == "application/gzip" ]; then
        zcat "$file" > "${file%.gz}.pvr"
        echo "Decompressed $file to ${file%.gz}.pvr"
    fi
}

function recursively_check_and_decompress() {
    local directory="$1"
    local filelist=$(find "$directory" -type f)

    for file in $filelist; do
        decompress_if_gzipped "$file"
    done
}

for dir in Data{001..121}; do
    if [ -d "$dir" ]; then
        echo "processing dir: $dir"
        recursively_check_and_decompress "$dir"
    fi
done
