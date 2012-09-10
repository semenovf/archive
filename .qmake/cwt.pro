TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS  =            \
#    cwt-pcre-dftables \
    cwt               \
    cwt-csv           \
    cwt-mysql         \
    cwt-pcre          \
    tests/bytearr     \
    tests/csv         \
    tests/event       \
    tests/echannel    \
    tests/eclock      \
    tests/emcsock     \
    tests/esocket     \
    tests/crc64       \
    tests/ddi         \
    tests/hashtab     \
    tests/ini         \
    tests/list        \
    tests/pcre        \
    tests/ringbuf     \
    tests/utils       \
    tests/strlist     \
    tests/strptime    \
    tests/strutils    \
    tests/dbi         \
    tests/unitype     \
    tests/net/udp     \
    tests/net/tcp     \
    tests/io/channel  \
    tests/io/channel_tcp_socket
