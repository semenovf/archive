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
    tests/async      \
    tests/bitarray   \
    tests/bytearray  \
    tests/bytestream \
    tests/dl         \
    tests/fsm        \
    tests/hash       \
    tests/md5        \
    tests/shared_ptr \
    tests/string     \
    tests/uri        \
    tests/uuid       \
    tests/vector     \
    tests/unitype    \
    tests/utf8string \
    utils/uuid-gen
