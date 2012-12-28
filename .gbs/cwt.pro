TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS  =             \
     cwt               \
     cwt-csv           \
     tests/array       \
     tests/bytearr     \
     tests/crc32       \    
     tests/crc64       \
     tests/ringbuf     \
     tests/utf8        \
    
#    tests/event       \
#    tests/echannel    \
#    tests/eclock      \
#    tests/emcsock     \
#    tests/esocket     \
#    tests/hashtab     \
#    tests/ini         \
#    tests/list        \
#    tests/logger      \
#    tests/options     \
#    tests/pack        \    
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
#    examples/app


#unix {
#    SUBDIRS  +=  cwt-event-signal-posix \
#        tests/esignal
#}