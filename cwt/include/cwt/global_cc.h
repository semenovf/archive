/**
 * @file   global_cc.h
 * @author wladt
 * @date   Nov 9, 2012 9:58:39 AM
 *
 * @brief
 */

#ifndef __CWT_GLOBAL_CC_H__
#define __CWT_GLOBAL_CC_H__

/*
   The compiler, must be one of: (Q_CC_x)
     SYM      - Digital Mars C/C++ (used to be Symantec C++)
     MWERKS   - Metrowerks CodeWarrior
     MSVC     - Microsoft Visual C/C++, Intel C++ for Windows
     BOR      - Borland/Turbo C++
     WAT      - Watcom C++
     GNU      - GNU C++
     COMEAU   - Comeau C++
     EDG      - Edison Design Group C++
     OC       - CenterLine C++
     SUN      - Forte Developer, or Sun Studio C++
     MIPS     - MIPSpro C++
     DEC      - DEC C++
     HPACC    - HP aC++
     USLC     - SCO OUDK and UDK
     CDS      - Reliant C++
     KAI      - KAI C++
     INTEL    - Intel C++ for Linux, Intel C++ for Windows
     HIGHC    - MetaWare High C/C++
     PGI      - Portland Group C++
     GHS      - Green Hills Optimizing C++ Compilers
     GCCE     - GCCE (Symbian GCCE builds)
     RVCT     - ARM Realview Compiler Suite
     NOKIAX86 - Nokia x86 (Symbian WINSCW builds)
     CLANG    - C++ front-end for the LLVM compiler

   Should be sorted most to least authoritative.
*/

/* Symantec C++ is now Digital Mars */
#if defined(__DMC__) || defined(__SC__)
#	define CWT_CC_SYM
#elif defined(__MWERKS__)
#	define CWT_CC_MWERKS
# 	if defined(__EMU_SYMBIAN_OS__)
#   	define Q_CC_NOKIAX86
#	endif
#elif defined(_MSC_VER)
#	define CWT_CC_MSVC
#	define CWT_CC_MSC
/* Intel C++ disguising as Visual C++: the `using' keyword avoids warnings */
#	if defined(__INTEL_COMPILER)
#		define CWT_CC_INTEL
#	endif
#elif defined(__BORLANDC__) || defined(__TURBOC__)
#	define CWT_CC_BORLAND
#elif defined(__WATCOMC__)
#	define CWT_CC_WATCOM
#elif defined(__GCCE__) /* Symbian GCCE */
#	define Q_CC_GCCE
#elif defined(__ARMCC__) || defined(__CC_ARM) /* ARM Realview Compiler Suite */
#	define Q_CC_RVCT
#elif defined(__GNUC__)
#  	define CWT_CC_GNUC
#	if defined(__MINGW32__)
#		define CWT_CC_MINGW
#	endif
#	if defined(__INTEL_COMPILER)
    	/* Intel C++ also masquerades as GCC 3.2.0 */
#		define CWT_CC_INTEL
#	endif
#	if defined(__clang__)
/* Clang also masquerades as GCC 4.2.1 */
#		define CWT_CC_CLANG
#	endif
#endif

#endif /* __CWT_GLOBAL_CC_H__ */
