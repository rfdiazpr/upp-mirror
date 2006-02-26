#ifndef OLE_H
#define OLE_H

#include <Core/Core.h>

#if defined(PLATFORM_WIN32) && !defined(PLATFORM_WINCE) && defined(COMPILER_MSC)

#define Ptr Ptr_
#define StdFont StdFont_
#define Picture Picture_
#include <comdef.h>
#include <comip.h>
#undef Ptr
#undef StdFont
#undef Picture

#endif

#include "util.h"

#endif//OLE_H
