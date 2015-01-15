#************************************************************
#* Generated automatically by 'gbs/ws.sh'
#* Command: gbs/ws.sh -create -git pfs
#* Author:  wladt
#* Date:    Wed Nov 20 13:31:18 YEKT 2013
#************************************************************
TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS  = \
     pfs               \
     tests/shared_ptr  \
     tests/bit_array   \
     tests/byte_string \
     tests/mbcs_string \
     tests/fsm        \
     tests/function   \
     tests/strtolong  \
#     tests/strtoreal  \
     tests/vector     \
     tests/stack      \
#    tests/map_hash   \
     tests/date       \
     tests/time       \
#    tests/utf8string_ptr \
#    tests/utf8string     \
#    tests/unitype        \
##    tests/cow_ptr_qt     \
     tests/pimpl
