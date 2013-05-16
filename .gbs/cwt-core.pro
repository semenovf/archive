#************************************************************
#* Generated automatically by 'gbs/ws.sh'                           
#* Command: gbs/ws.sh -create -git cwt-core                                         
#* Author:  wladt
#* Date:    Wed Feb  6 08:31:48 YEKT 2013                                            
#************************************************************
TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS  = cwt-core  \
    examples/logger  \
    tests/array      \
    tests/bytearray  \
    tests/bytestream \
    tests/dl         \
    tests/file       \
    tests/fsm        \
    tests/hash       \
    tests/md5        \
    tests/petaloid   \
    tests/sepaloid   \
    tests/shared_ptr \
    tests/string     \
    tests/textstream \
    tests/uri        \
    tests/uuid       \
    tests/vector     \
    utils/uuid-gen


