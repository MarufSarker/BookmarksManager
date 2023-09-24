#!/usr/bin/bash

if [[ ! -d ".git" ]]; then
    git init
fi
if [[ "$(git config --get user.name)" == "" ]]; then
    git config user.name "User"
fi
if [[ "$(git config --get user.email)" == "" ]]; then
    git config user.email "user@localhost"
fi

git status
git add . && git commit -m "$(date)"
git status

# git reset --hard && git clean -fdx
