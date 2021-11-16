#!/bin/bash

if [[ -n $(git status -s) ]] 
then
  echo 'clang-format modified files. build failed. check the .github/workflows/checks.yml file'
  exit 1
fi

