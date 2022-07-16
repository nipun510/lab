#! /bin/bash

set -e 

cwd=`pwd`
cd $LAB/go/src
cd mutility
go build
cd - 

cd dcache
go build
cd -

cd main
go build
cd $cwd
