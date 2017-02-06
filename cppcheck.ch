#!/bin/sh

cppcheck `cat cppcheck/options` --includes-file=cppcheck/includes-file --file-list=cppcheck/sources-file