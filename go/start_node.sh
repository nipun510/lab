#! /bin/bash

set -e 

cwd=`pwd`
cd $LAB/go/src
$LAB/go/src/main/main.exe --node "$2"
