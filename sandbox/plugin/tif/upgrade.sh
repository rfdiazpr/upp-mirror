#!/bin/sh

# This is a script intended for semi-automatic import of libraries into U++.
# It takes a library in common form of configure script and makefile and copies
# the required files. The source files are tweaked in the process - guards are
# added to headers and a flag is added to them to allow switching between
# statically linked version and dnymicaly linked library. Note that some minor
# adjustments still may be needed on the package resulting from this script
# when upgrading the upstream library.

# LAST MODIFICATION: upgrade to libtiff-4.0.1 (http://download.osgeo.org/libtiff/tiff-4.0.1.tar.gz)


# Where to look for the sources
SRC="tiff-4.0.1/libtiff"
# Destination of the imported files
DST="lib"
# Options for configure script
OPTS="--disable-lzma"
# Path to configure script
CONFIGURE="$SRC/../"
# Flag switching between library and embeded version 
FLAG="EMBED_TIF"
# Prefix for header gueards
GUARD_PREFIX="_plugin_tif_"
# How to print a list of all source files - this will be invoked from makefile, 
# so it can use makefile variables as well as (properly escaped) shell scripts.
# The correct variable names can be usually found in Makefile.am.
SOURCES='echo $(libtiffinclude_HEADERS) $(noinst_HEADERS) $(nodist_libtiffinclude_HEADERS) $(libtiff_la_SOURCES) tif_win32.c tif_fax3.h tif_config.h'
# Files that need to be only copied, without beeing added into the package
COPY_FILES=""
# Informational files (docs, changelogs, license, readme,...)
DOC_FILES="../COPYRIGHT ../ChangeLog ../README ../RELEASE-DATE ../VERSION"
# Package file filename
UPP="tif.upp"
# Create a skeleton of pacakge file, the source files will be appended in form
# of ", file1.h, file2.c, ..." and ';' added at the end.
cat > "$UPP" <<EOF
description "TIFF image file format\3770,128,128";

acceptflags
	$FLAG,
	EMBED_JPG;

library(!$FLAG&!WIN32) tiff;

uses
	plugin\jpg;

file
	tif.h,
	tifupp.cpp,
	tifreg.icpp,
	$DST readonly separator
EOF

# include the generic upgrade script
. ../upgrade.sh

# fix problematic includes
sed -i 's%#include "jpeglib[.]h"%#if defined(flagEMBED_JPG) || defined(flagWIN32)\
#include <plugin/jpg/lib/jpeglib.h>\
#else\
#include "jpeglib.h"\
#endif%' "$DST/tif_jpeg.c" "$DST/tif_ojpeg.c"

sed -i 's%#include "jerror[.]h"%#if defined(flagEMBED_JPG) || defined(flagWIN32)\
#include <plugin/jpg/lib/jerror.h>\
#else\
#include "jerror.h"\
#endif%' "$DST/tif_jpeg.c" "$DST/tif_ojpeg.c"

sed -i 's%#include "zlib[.]h"%#if defined(flagWIN32)\
#include <plugin/z/lib/zlib.h>\
#else\
#include "zlib.h"\
#endif%' "$DST/tif_pixarlog.c" "$DST/tif_zip.c"

# extra guards in posix/win32 specific files
{   echo "#if 0"
    cat "$DST/tif_unix.c"
    echo "#endif"
} > "$DST/tif_unix.c.tmp"
mv "$DST/tif_unix.c.tmp" "$DST/tif_unix.c"

{   echo "#ifdef flagWIN32"
    cat "$DST/tif_win32.c"
    echo "#endif"
} > "$DST/tif_win32.c.tmp"
mv "$DST/tif_win32.c.tmp" "$DST/tif_win32.c"

# fix inlining to work in both GCC and MSC 
sed -i 's/inline/__inline/' "$DST/tif_fax3.c"
