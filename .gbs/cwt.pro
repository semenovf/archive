TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS  =             \
     cwt               \
     tests/array 
#    cwt-csv           \
#    tests/bytearr     \
#    tests/csv         \
#    tests/event       \
#    tests/echannel    \
#    tests/eclock      \
#    tests/emcsock     \
#    tests/esocket     \
#    tests/crc32       \    
#    tests/crc64       \
#    tests/hashtab     \
#    tests/ini         \
#    tests/list        \
#    tests/logger      \
#    tests/options     \
#    tests/pack        \    
#    tests/ringbuf     \
#    tests/utils       \
#    tests/strlist     \
#    tests/strptime    \
#    tests/strutils    \
#    tests/unitype     \
#    tests/net/local   \
#    tests/net/udp     \
#    tests/net/tcp     \
#    tests/io/channel  \
#    tests/io/channel_tcp_socket \
#    tests/fsm         \
#    tests/fsm_date    \
#    tests/fsm_rpt     \
#    tests/mdarr       \
#    tests/snprintf    \
#    tests/uri         \
#    tests/utf8        \
#    examples/app


#unix {
#    SUBDIRS  +=  cwt-event-signal-posix \
#        tests/esignal
#}