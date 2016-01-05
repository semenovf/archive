/**
 * @file   compiler.h
 * @author wladt
 * @date   Nov 9, 2012 9:58:39 AM
 *
 * @brief
 */

#ifndef __PFS_BITS_COMPILER_H__
#define __PFS_BITS_COMPILER_H__

/*
   The compiler, must be one of: (CWT_CC_x)
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
#	define PFS_CC_SYM
#elif defined(__MWERKS__)
#	define PFS_CC_MWERKS
# 	if defined(__EMU_SYMBIAN_OS__)
#   	define PFS_CC_NOKIAX86
#	endif
#elif defined(_MSC_VER)
#	define PFS_CC_MSVC
#	define PFS_CC_MSC
// see http://stackoverflow.com/questions/70013/how-to-detect-if-im-compiling-code-with-visual-studio-2008
//MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
//MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
//MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
//MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
//MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
//MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio 2003)
//MSVC++ 7.0  _MSC_VER == 1300
//MSVC++ 6.0  _MSC_VER == 1200
//MSVC++ 5.0  _MSC_VER == 1100
#	define PFS_CC_MSVC_VERSION _MSC_VER /* In Visual Studio 2010, _MSC_VER is defined as 1600 */
#	define PFS_CC_MSC_VERSION  _MSC_VER
/* Intel C++ disguising as Visual C++: the `using' keyword avoids warnings */
#	if defined(__INTEL_COMPILER)
#		define PFS_CC_INTEL
#		define PFS_CC_INTEL_VERSION _MSC_VER
#		define PFS_CC_INTEL_VERSION _MSC_VER
#	endif
#elif defined(__BORLANDC__) || defined(__TURBOC__)
#	define PFS_CC_BORLAND
#	if (defined(__BORLANDC__) && __BORLANDC__ <= 0x410) || defined(__TURBOC__)
#		define PFS_CC_BORLAND_REAL
#	endif
#elif defined(__WATCOMC__)
#	define PFS_CC_WATCOM
#elif defined(__GCCE__) /* Symbian GCCE */
#	define PFS_CC_GCCE
#elif defined(__ARMCC__) || defined(__CC_ARM) /* ARM Realview Compiler Suite */
#	define PFS_CC_RVCT
#elif defined(__GNUC__)
#  	define PFS_CC_GNUC
#	define PFS_CC_GCC
#	if defined(__MINGW32__)
#		define PFS_CC_MINGW
#	endif
#	if defined(__INTEL_COMPILER)
    	/* Intel C++ also masquerades as GCC 3.2.0 */
#		define PFS_CC_INTEL
#	endif
#	if defined(__clang__)
/* Clang also masquerades as GCC 4.2.1 */
#		define PFS_CC_CLANG
#	endif
#endif

#endif /* __PFS_BITS_COMPILER_H__ */
