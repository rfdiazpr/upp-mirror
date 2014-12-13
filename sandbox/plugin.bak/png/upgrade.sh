#!/bin/sh

# This is a script intended for semi-automatic import of libraries into U++.
# It takes a library in common form of configure script and makefile and copies
# the required files. The source files are tweaked in the process - guards are
# added to headers and a flag is added to them to allow switching between
# statically linked version and dnymicaly linked library. Note that some minor
# adjustments still may be needed on the package resulting from this script
# when upgrading the upstream library.

# LAST MODIFICATION: upgrade to libjpng-1.5.10 (http://download.sourceforge.net/libpng/libpng-1.5.10.tar.gz)

# Where to look for the sources
SRC="libpng-1.5.10"
# Destination of the imported files
DST="lib"
# Options for configure script
OPTS=""
# Path to configure script
CONFIGURE="$SRC"
# Flag switching between library and embeded version 
FLAG="EMBED_PNG"
# Prefix for header gueards
GUARD_PREFIX="_plugin_png_"
# How to print a list of all source files - this will be invoked from makefile, 
# so it can use makefile variables as well as (properly escaped) shell scripts.
# The correct variable names can be usually found in Makefile.am.
SOURCES='echo $(libpng15_la_SOURCES) pnglibconf.h'
# Files that need to be only copied, without beeing added into the package
COPY_FILES=""
# Informational files (docs, changelogs, license, readme,...)
DOC_FILES="CHANGES LICENSE README libpng-manual.txt"
# Package file filename
UPP="png.upp"
# Create a skeleton of pacakge file, the source files will be appended in form
# of ", file1.h, file2.c, ..." and ';' added at the end.
cat > "$UPP" <<EOF
description "PNG image file format U++ encapsulation\3770,128,128";

acceptflags
	$FLAG;

flags(WIN32) $FLAG;

library(!$FLAG&!WIN32) png;

uses
	Core,
	Draw;
file
	png.h,
	pngupp.cpp,
	pngreg.icpp,
	$DST readonly separator
EOF

# copy prebuilt pnglibconf
cp "$SRC/scripts/pnglibconf.h.prebuilt" "$SRC/pnglibconf.h"

# include the generic upgrade script
. ../upgrade.sh

# fix problematic includes
sed -i 's%#include "zlib[.]h"%#if defined(flagWIN32)\
#include <plugin/z/lib/zlib.h>\
#else\
#include "zlib.h"\
#endif%' "$DST/pngstruct.h"


