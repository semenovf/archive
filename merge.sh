#!/bin/sh

#REPO_NAME=pfs.legacy
#REPO_NAME=gbs
#REPO_NAME=pfs-debby
#REPO_NAME=pfs-griotte
#REPO_NAME=pfs-types
#REPO_NAME=pfs-dom
#REPO_NAME=pfs-app
#REPO_NAME=pfs-debby-sqlite3
#REPO_NAME=pfs-cli
#REPO_NAME=cwt-abnf
#REPO_NAME=cwt-xml
#REPO_NAME=hg-quest
#REPO_NAME=rw-meccano
#REPO_NAME=cwt-core
#REPO_NAME=hgweb
#REPO_NAME=cwt
#REPO_NAME=cwt-sqlite3
REPO_NAME=cwt-mysql5


git remote add ${REPO_NAME} git@github.com:semenovf/${REPO_NAME}.git
git fetch ${REPO_NAME}
git merge ${REPO_NAME}/master
git remote remove ${REPO_NAME}
