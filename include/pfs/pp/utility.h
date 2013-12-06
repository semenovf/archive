#ifndef __PFS_PP_UTILITY_H__
#define __PFS_PP_UTILITY_H__

#define PFS_COMMA ,

#define PFS_CAT(pfx,x) pfx ## x

#define PFS_SWAP(a,b) b a

/*
PFS_SWAP(1,2)                            -> 2 1
PFS_SWAP(PFS_SWAP(1,2), 3)               -> 3 2 1
PFS_SWAP(PFS_SWAP(PFS_SWAP(1,2), 3), 4)  -> 4 3 2 1
*/

#endif /* __PFS_PP_UTILITY_H__ */
