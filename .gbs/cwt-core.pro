#************************************************************
#* Generated automatically by 'gbs/ws.sh'                           
#* Command: gbs/ws.sh -create -git cwt-core                                         
#* Author:  wladt
#* Date:    Wed Feb  6 08:31:48 YEKT 2013                                            
#************************************************************
TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS  = cwt-core  \
    tests/array      \
    tests/bytestream \
    tests/dl         \
    tests/fsm        \
    tests/hash       \
    tests/logger     \
    tests/md5        \
    tests/petaloid   \
    tests/sepaloid   \
    tests/string     \
    tests/textstream \
    tests/uri        \
    tests/uuid       \
    utils/uuid-gen


