#************************************************************
#* Generated automatically by 'gbs/ws.sh'
#* Command: gbs/ws.sh -create -git cwt-sys
#* Author:  wladt
#* Date:    Tue Dec 31 12:11:32 YEKT 2013
#************************************************************
TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS  = cwt-sys       \
	tests/dl         \
	tests/fsm        \
	tests/regexp     \
	tests/safeformat \
	tests/thread     \
	tests/uri        \
	examples/logger
