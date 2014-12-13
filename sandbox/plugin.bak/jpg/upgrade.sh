#!/bin/sh

# This is a script intended for semi-automatic import of libraries into U++.
# It takes a library in common form of configure script and makefile and copies
# the required files. The source files are tweaked in the process - guards are
# added to headers and a flag is added to them to allow switching between
# statically linked version and dnymicaly linked library. Note that some minor
# adjustments still may be needed on the package resulting from this script
# when upgrading the upstream library.

# LAST MODIFICATION: upgrade to libjpeg-turbo-1.2.0 (http://sourceforge.net/projects/libjpeg-turbo/files/1.2.0/libjpeg-turbo-1.2.0.tar.gz/download)

# Where to look for the sources
SRC="libjpeg-turbo-1.2.0"
# Destination of the imported files
DST="lib"
# Options for configure script
OPTS="--with-jpeg8 --without-java --without-simd"
# Path to configure script
CONFIGURE="$SRC"
# Flag switching between library and embeded version 
FLAG="EMBED_JPG"
# Prefix for header gueards
GUARD_PREFIX="_plugin_jpg_"
# How to print a list of all source files - this will be invoked from makefile, 
# so it can use makefile variables as well as (properly escaped) shell scripts.
# The correct variable names can be usually found in Makefile.am.
SOURCES='echo $(libjpeg_la_SOURCES) config.h jconfig.h'
# Files that need to be only copied, without beeing added into the package
COPY_FILES="jccolext.c jdcolext.c jdmrgext.c"
# Informational files (docs, changelogs, license, readme,...)
DOC_FILES="filelist.txt ChangeLog.txt libjpeg.txt README-turbo.txt structure.txt"
# Package file filename
UPP="jpg.upp"
# Create a skeleton of pacakge file, the source files will be appended in form
# of ", file1.h, file2.c, ..." and ';' added at the end.
cat > "$UPP" <<EOF
description "JPEG image file format\3770,128,128";

acceptflags
	$FLAG;

library(!$FLAG&!WIN32) jpeg;

uses
	Draw;

file
	jpg.h,
	jpgupp.cpp,
	jpgreg.icpp,
	src.tpp,
	$DST readonly separator
EOF

# include the generic upgrade script
. ../upgrade.sh

# fix INLINE to work with MSVC
sed -i 's%#define INLINE .*%#ifdef flagMSC\
  #define INLINE __forceinline\
#else\
  #define INLINE __attribute__((always_inline))\
#endif%' "$DST/config.h"
