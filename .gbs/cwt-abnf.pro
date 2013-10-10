#************************************************************
#* Generated automatically by './gbs/ws.sh'
#* Command: ./gbs/ws.sh -create cwt-abnf
#* Author:  Trifochkin Vladislav
#* Date:    Wed Oct 31 14:04:53 UTC 2012
#************************************************************
TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS  = cwt-abnf              \
           tests/test_abnf       \
           tests/test_abnf_gen   \
           tests/test_abnf_build \
           tests/test_abnf_norm  \
	   tests/test_abnf_tmp   \
           abnf2fsm
