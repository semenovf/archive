#ifndef __PFS_PP_UTILITY_H__
#define __PFS_PP_UTILITY_H__

#define PFS_CAT(pfx,x) pfx ## x

#define PFS_SWAP(a,b) b a

/*
PFS_SWAP(1,2)                            -> 2 1
PFS_SWAP(PFS_SWAP(1,2), 3)               -> 3 2 1
PFS_SWAP(PFS_SWAP(PFS_SWAP(1,2), 3), 4)  -> 4 3 2 1
*/

#ifdef __cplusplus

#	define PFS_AUTO_CTOR_FUNCTION(func)        \
    static const struct func ## _ctor_struct { \
        func ## _ctor_struct () { func(); }    \
    } func ## _ctor_struct;

#	define PFS_AUTO_DTOR_FUNCTION(func)        \
    static const struct func ## _dtor_struct { \
        func ## _dtor_struct () { }            \
        ~ func ## _dtor_struct () { func(); }  \
    } func ## _dtor_instance;

#endif

#endif /* __PFS_PP_UTILITY_H__ */
