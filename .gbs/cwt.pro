TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS  =            \
#    cwt-pcre-dftables \
    cwt               \
    cwt-csv           \
    cwt-mysql         \
    tests/bytearr     \
    tests/csv         \
    tests/event       \
    tests/echannel    \
    tests/eclock      \
    tests/emcsock     \
    tests/esocket     \
    tests/crc32       \    
    tests/crc64       \
    tests/ddi         \
    tests/hashtab     \
    tests/ini         \
    tests/list        \
    tests/options     \
    tests/ringbuf     \
    tests/utils       \
    tests/strlist     \
    tests/strptime    \
    tests/strutils    \
    tests/dbi         \
    tests/unitype     \
    tests/net/local   \
    tests/net/udp     \
    tests/net/tcp     \
    tests/io/channel  \
    tests/io/channel_tcp_socket \
    examples/app

#    cwt-pcre          \
#    tests/pcre        \


unix {
    SUBDIRS  +=  cwt-event-signal-posix \
        tests/esignal
}