#!/bin/sh

#REPO_NAME=pfs.legacy
#REPO_NAME=gbs
#REPO_NAME=pfs-debby
REPO_NAME=pfs-griotte

git remote add ${REPO_NAME} git@github.com:semenovf/${REPO_NAME}.git
git fetch ${REPO_NAME}
git merge ${REPO_NAME}/master
git remote remove ${REPO_NAME}
