#!/bin/sh

#REPO_NAME=pfs.legacy
#REPO_NAME=gbs
#REPO_NAME=pfs-debby
#REPO_NAME=pfs-griotte
#REPO_NAME=pfs-types
#REPO_NAME=pfs-dom
#REPO_NAME=pfs-app
REPO_NAME=pfs-debby-sqlite3

git remote add ${REPO_NAME} git@github.com:semenovf/${REPO_NAME}.git
git fetch ${REPO_NAME}
git merge ${REPO_NAME}/master
git remote remove ${REPO_NAME}
