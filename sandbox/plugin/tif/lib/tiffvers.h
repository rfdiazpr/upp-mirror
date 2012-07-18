#ifndef _plugin_tif_lib_tiffvers_h_
#define _plugin_tif_lib_tiffvers_h_
#if defined(flagEMBED_TIF) || defined(flagWIN32)
#define TIFFLIB_VERSION_STR "LIBTIFF, Version 4.0.1\nCopyright (c) 1988-1996 Sam Leffler\nCopyright (c) 1991-1996 Silicon Graphics, Inc."
/*
 * This define can be used in code that requires
 * compilation-related definitions specific to a
 * version or versions of the library.  Runtime
 * version checking should be done based on the
 * string returned by TIFFGetVersion.
 */
#define TIFFLIB_VERSION 20120218
#endif
#endif
