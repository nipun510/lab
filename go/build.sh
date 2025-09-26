#! /bin/bash

set -e 

cwd=`pwd`

cd $LAB/go/src/mutility
go build .

cd $LAB/go/src/dcache
go build .


cd $LAB/go/src/main
go build .

cd $cwd
