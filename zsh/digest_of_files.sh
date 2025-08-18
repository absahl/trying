#!/bin/zsh

if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

file=$1

if [[ ! -f $file ]]; then
    echo "Error: File '$file' not found"
    exit 1
fi

while IFS= read -r line; do
    md5sum "$line"
done < "$file"
