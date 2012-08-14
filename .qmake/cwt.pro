TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS  =            \
#    cwt-pcre-dftables \
    cwt               \
    cwt-mysql         \
    cwt-pcre          \
    tests/bytearr     \
    tests/csv         \
    tests/event       \
    tests/echannel    \
    tests/eclock      \
    tests/emcsock     \
    tests/crc64       \
    tests/ddi         \
    tests/hashtab     \
    tests/ini         \
    tests/list        \
    tests/pcre        \
    tests/utils       \
    tests/strlist     \
    tests/strptime    \
    tests/strutils    \
    tests/dbi         \
    tests/unitype     \
    tests/io/channel
