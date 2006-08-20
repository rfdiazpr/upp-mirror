#ifndef CORE_H
#define CORE_H

#define QLIB3
#define UPP

#if defined(flagMT) || defined(flagDEBUG)
	#define _MULTITHREADED
#endif

#ifdef flagLINUX
	#define PLATFORM_LINUX
	#define PLATFORM_POSIX
	#ifdef flagGUI
		#define PLATFORM_X11
	#endif

	#ifdef flagTESTLEAKS
		#define TESTLEAKS
	#endif

#endif

#ifdef flagSOLARIS
	#define PLATFORM_SOLARIS
	#define PLATFORM_POSIX
	#ifdef flagGUI
		#define PLATFORM_X11
	#endif

	#ifdef flagTESTLEAKS
		#define TESTLEAKS
	#endif
#endif

#ifdef flagFREEBSD
	#define PLATFORM_FREEBSD
	#define PLATFORM_POSIX
	#ifdef flagGUI
		#define PLATFORM_X11
	#endif

	#ifdef flagTESTLEAKS
		#define TESTLEAKS
	#endif
#endif

#if defined(flagWIN) || defined(flagWIN32)
	#define PLATFORM_WIN32
	#ifdef __CYGWIN__
		#define PLATFORM_CYGWIN
	#endif
#endif

#ifdef flagWINCE_PPC
	#define PLATFORM_WIN32
	#define PLATFORM_WINCE
	#define PLATFORM_WINCE_PPC
#endif

#ifdef flagWINCE_SP
	#define PLATFORM_WIN32
	#define PLATFORM_WINCE
	#define PLATFORM_WINCE_SP
#endif

#ifdef _MSC_VER
	#define COMPILER_MSC
	#ifndef _CPPRTTI
		#error  RTTI must be enabled !!!
	#endif  //_CPPRTTI
	#if _MSC_VER <= 1300
		#error  MSC 6.0 not supported anymore
	#endif
	#pragma warning(disable: 4786)
	#define _CRT_SECURE_NO_DEPRECATE // we really need strcpy etc. to work with MSC 8.0
#endif

#ifdef flagEVC
	#define COMPILER_EVC
	#define COMPILER_MSC
	#ifndef _CPPRTTI
		#error RTTI must be enabled !!!
	#endif // _CPPRTTI
	#pragma warning(disable: 4786)
#endif

#ifdef flagGNU
	#define COMPILER_GCC
	#define COMPILER_GNU
	#define COMPILER_GNU2
#endif

#ifdef flagGNU3
	#define COMPILER_GCC
	#define COMPILER_GNU
	#define COMPILER_GNU3
#endif

#if defined(flagGCC) || defined(flagGCC32)
	#define COMPILER_GCC
	#define COMPILER_GNU
	#define COMPILER_GNU3
#endif

#ifdef flagDM
	#define COMPILER_DM
#endif

#ifdef flagOSTRING
	#define OSTRING
#endif

#if defined(flagDEBUG)
	#ifndef _DEBUG
		#define _DEBUG
	#endif
	#ifndef _TEST_LEAKS
		#define _TEST_LEAKS
	#endif
#else
	#ifndef _RELEASE
		#define _RELEASE
	#endif
#endif

#ifdef flagDLL
	#define _USRDLL
#endif

#ifdef flagEXPERIMENTAL
	#define EXPERIMENTAL
#endif

#ifdef flagNEWSTRING
	#define NEWSTRING
#endif

#if defined(flagSPARC)
	#define CPU_32
	#define CPU_SPARC
	#define CPU_BE
	#define CPU_BIG_ENDIAN
	#define CPU_ALIGNED
#elif defined(flagARM)
	#define CPU_32
	#define CPU_ARM
	#define CPU_LE
	#define CPU_LITTLE_ENDIAN // is it really?
	#define CPU_ALIGNED
#else
	#define CPU_X86
	#if defined(__amd64) || defined(_WIN64)
		#define CPU_AMD64
		#define CPU_64
	#else
		#define CPU_IA32
		#define CPU_32
	#endif
	#define CPU_LE
	#define CPU_LITTLE_ENDIAN
	#define CPU_UNALIGNED
#endif

#include <typeinfo>
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <ctype.h>

#if defined(PLATFORM_POSIX)
	#define DIR_SEP  '/'
	#define DIR_SEPS "/"
	#define PLATFORM_PATH_HAS_CASE 1

	#include <errno.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/time.h>
	#include <time.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <pthread.h>
	#include <memory.h>
	#include <dirent.h>
	#ifdef PLATFORM_SOLARIS
		#include <inttypes.h>
	#else
		#include <stdint.h>
	#endif
#endif //PLATFORM_POSIX

#ifdef PLATFORM_POSIX
#define	LOFF_T_      off_t
#define LSEEK64_     lseek
#define FTRUNCATE64_ ftruncate
#endif

#ifdef PLATFORM_LINUX
#undef  LOFF_T_
#define LOFF_T_      loff_t
#undef  LSEEK64_
#define LSEEK64_     lseek64
#undef  FTRUNCATE64_
#define FTRUNCATE64_ ftruncate64
#endif

#ifdef PLATFORM_WIN32
	#define DIR_SEP  '\\'
	#define DIR_SEPS "\\"
	#define PLATFORM_PATH_HAS_CASE 0
	#include <io.h>
	#ifndef PLATFORM_MFC // just mini Windows headers
		#ifdef COMPILER_MSC
			#ifndef _X86_
				#define _X86_
			#endif
			#ifndef _WINDOWS_
				#define _WINDOWS_
			#endif
			#ifndef _INC_WINDOWS
				#define _INC_WINDOWS
			#endif
			#ifndef _STRUCT_NAME
				#define _STRUCT_NAME(x)
				#define DUMMYSTRUCTNAME
				#define DUMMYSTRUCTNAME2
				#define DUMMYSTRUCTNAME3
			#endif
			#ifndef NO_STRICT
				#ifndef STRICT
					#define STRICT 1
				#endif
			#endif
			#include <stdarg.h>
			#include <windef.h>
			#include <winbase.h>
			#include <wingdi.h>
			#include <winuser.h>
			typedef DWORD LCTYPE;
		#else
			#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
			#include <windows.h>
			#include <stdint.h>
		#endif
	#endif
#endif

#include <Core/Defs.h>
#include <Core/Mt.h>
#include <Core/Global.h>
#include <Core/Topt.h>
#include <Core/Profile.h>
#include <Core/String.h>

#ifndef CONSERVATIVE
#include <Core/String.hpp>
#endif

#include <Core/CharSet.h>
#include <Core/TimeDate.h>
#include <Core/Path.h>
#include <Core/Stream.h>
#include <Core/Diag.h>

#include <Core/Vcont.h>
#include <Core/BiCont.h>
#include <Core/Index.h>
#include <Core/Map.h>
#include <Core/Algo.h>
#include <Core/Other.h>
#include <Core/Vcont.hpp>
#include <Core/Index.hpp>

#include <Core/Value.h>
#include <Core/Gtypes.h>
#include <Core/Color.h>

#include <Core/Uuid.h>
#include <Core/Ptr.h>

#include <Core/Callback.h>
#include <Core/Util.h>

#include <Core/Format.h>
#include <Core/Convert.h>

#include <Core/Thread.h>

#include <Core/z.h>

#include <Core/Parser.h>
#include <Core/XML.h>
#include <Core/Lang.h>
#include <Core/i18n.h>
#include <Core/Topic.h>

#include <Core/App.h>

#include <Core/Xmlize.h>

#include <Core/Win32Com.h>

#ifdef PLATFORM_WIN32
NTL_MOVEABLE(POINT)
NTL_MOVEABLE(SIZE)
NTL_MOVEABLE(RECT)
#endif

#if (defined(_DEBUG) || defined(_TEST_LEAKS)) && defined(PLATFORM_POSIX)

//Place it to the begining of each file to be first function called in whole code...

//$-
extern int sMemDiagInitCount;
struct MemDiagCls {
	MemDiagCls()  { if(!sMemDiagInitCount++) MemoryInitDiagnostics(); }
	~MemDiagCls() { if(!--sMemDiagInitCount) MemoryDumpLeaks(); }
};
static const MemDiagCls sMemDiagHelper;
//$+

#endif

#endif //CORE_H
