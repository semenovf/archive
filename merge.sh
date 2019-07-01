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
#REPO_NAME=cwt-mysql5
#REPO_NAME=Class-Accessor-Validated
#REPO_NAME=Webject
#REPO_NAME=Validator-Var
#REPO_NAME=MetaPage
#REPO_NAME=DBI-DDI
#REPO_NAME=Dancer-Template-MetaPage
#REPO_NAME=Text-Simplify
#REPO_NAME=jq-sc
#REPO_NAME=simple-clock-indicator
REPO_NAME=jq-logindialog-qt

mkdir -p ${REPO_NAME} || exit -1

git remote add ${REPO_NAME} git@github.com:semenovf/${REPO_NAME}.git \
    && git -C ${REPO_NAME} fetch ${REPO_NAME}                        \
    && git merge --allow-unrelated-histories ${REPO_NAME}/master     \
    && git remote remove ${REPO_NAME}
