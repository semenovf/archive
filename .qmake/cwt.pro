TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS  =         \
    cwt            \
    cwt-mysql      \
    tests/event    \
    tests/echannel \
    tests/eclock   \
    tests/emcsock  \
    tests/crc64    \
    tests/utils    \
    tests/strlist  \
    tests/strutils \
    tests/dbi      \
    tests/io/channel
