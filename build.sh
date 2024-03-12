#!/bin/bash

cd cat
make
make -f ./cat/Makefile clean

cd ../grep
make
make -f./grep/Makefile clean

cd ..

mv ./cat/my_cat ./my_cat
mv ./grep/my_grep ./my_grep