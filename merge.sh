#!/bin/sh

REPO_NAME=pfs.legacy

git remote add ${REPO_NAME} git@github.com:semenovf/${REPO_NAME}.git
git fetch ${REPO_NAME}
git merge ${REPO_NAME}/master
git remote remove ${REPO_NAME}
