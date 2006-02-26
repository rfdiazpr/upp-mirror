# Project modules: Work CtrlLib CtrlCore Draw Core plugin\zlib DocTypes
# Configurations: 
# make CONFIG=1: Debug [default]
# make CONFIG=2: Release
# make CONFIG=3: Linux Release
# make CONFIG=4: Linux Debug

UPPDIR_WIN = f:\\uppsrc\\

UPPDIR_LNX = /f/uppsrc/

OUTDIR_WIN = e:\\out\\upp\\
OUTDIR_LNX = e:\out\upp\

CONFIG ?= 1

ifeq ($(CONFIG), 1) # Debug
CMACRO =  -DflagMSC -DflagGUI -DflagDEBUG -DflagWIN32 -DflagST -DflagIMAGE3
CINC =  -I"$(UPPDIR)" -I"f:/source/include/msvc" -I"f:\source\include\msvc" -I"D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE" -I"D:\Program Files\Microsoft Visual Studio\VC98\MFC\INCLUDE" -I"D:\Program Files\Microsoft Visual Studio\VC98\ATL\INCLUDE" -I"f:/uppsrc" -I"f:/uppsrc/platform" -I"e:/v/include" -I"d:\ORANT\OCI80\INCLUDE" -I"d:\mysql\include"
OBJ = .obj
LIB = .lib
CFLAGS = cl -c -nologo -W3 -GR -GX -FD -Gy -YX -Fp$(OUTDIR_WORK)Work.pch -Fd$(OUTDIR_WORK)Work.pdb -MLd -GZ -Zi -Od
UPPDIR = $(UPPDIR_WIN)
OUTDIR_WORK = $(OUTDIR_WIN)Work\\DEBUG-GUI-IMAGE3-MSC-ST-WIN32\\
OUTDIR_CTRLLIB = $(OUTDIR_WIN)CtrlLib\\DEBUG-GUI-IMAGE3-MSC-ST-WIN32\\
OUTDIR_CTRLCORE = $(OUTDIR_WIN)CtrlCore\\DEBUG-GUI-IMAGE3-MSC-ST-WIN32\\
OUTDIR_DRAW = $(OUTDIR_WIN)Draw\\DEBUG-GUI-IMAGE3-MSC-ST-WIN32\\
OUTDIR_CORE = $(OUTDIR_WIN)Core\\DEBUG-GUI-IMAGE3-MSC-ST-WIN32\\
OUTDIR_PLUGIN_ZLIB = $(OUTDIR_WIN)plugin\\zlib\\DEBUG-GUI-IMAGE3-MSC-ST-WIN32\\
OUTDIR_DOCTYPES = $(OUTDIR_WIN)DocTypes\\DEBUG-GUI-IMAGE3-MSC-ST-WIN32\\
else
endif

ifeq ($(CONFIG), 2) # Release
CMACRO =  -DflagMSC -DflagGUI -DflagRELEASE -DflagWIN32 -DflagST -DflagIMAGE3
CINC =  -I"$(UPPDIR)" -I"f:/source/include/msvc" -I"f:\source\include\msvc" -I"D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE" -I"D:\Program Files\Microsoft Visual Studio\VC98\MFC\INCLUDE" -I"D:\Program Files\Microsoft Visual Studio\VC98\ATL\INCLUDE" -I"f:/uppsrc" -I"f:/uppsrc/platform" -I"e:/v/include" -I"d:\ORANT\OCI80\INCLUDE" -I"d:\mysql\include"
OBJ = .obj
LIB = .lib
CFLAGS = cl -c -nologo -W3 -GR -GX -FD -Gy -YX -Fp$(OUTDIR_WORK)Work.pch -Fd$(OUTDIR_WORK)Work.pdb -ML -O2
UPPDIR = $(UPPDIR_WIN)
OUTDIR_WORK = $(OUTDIR_WIN)Work\\GUI-IMAGE3-MSC-RELEASE-ST-WIN32\\
OUTDIR_CTRLLIB = $(OUTDIR_WIN)CtrlLib\\GUI-IMAGE3-MSC-RELEASE-ST-WIN32\\
OUTDIR_CTRLCORE = $(OUTDIR_WIN)CtrlCore\\GUI-IMAGE3-MSC-RELEASE-ST-WIN32\\
OUTDIR_DRAW = $(OUTDIR_WIN)Draw\\GUI-IMAGE3-MSC-RELEASE-ST-WIN32\\
OUTDIR_CORE = $(OUTDIR_WIN)Core\\GUI-IMAGE3-MSC-RELEASE-ST-WIN32\\
OUTDIR_PLUGIN_ZLIB = $(OUTDIR_WIN)plugin\\zlib\\GUI-IMAGE3-MSC-RELEASE-ST-WIN32\\
OUTDIR_DOCTYPES = $(OUTDIR_WIN)DocTypes\\GUI-IMAGE3-MSC-RELEASE-ST-WIN32\\
else
endif

ifeq ($(CONFIG), 3) # Linux Release
CMACRO =  -DflagGNU -DflagGUI -DflagRELEASE -DflagLINUX -DflagST -DflagSHARED -DflagXFT
CINC =  -I"$(UPPDIR)" -I"G:/mingw2/i386-mingw32/include" -I"f:/uppsrc" -I"f:/uppsrc/platform" -I"e:/v/include" -I"d:/ORANT/OCI80/INCLUDE" -I"d:/mysql/include"
OBJ = .o
LIB = .a
CFLAGS = c++ -c -fuse-cxa-atexit -O2 -ffunction-sections
UPPDIR = $(UPPDIR_LNX)
OUTDIR_WORK = $(OUTDIR_LNX)Work/GNU-GUI-LINUX-RELEASE-SHARED-ST-XFT/
OUTDIR_CTRLLIB = $(OUTDIR_LNX)CtrlLib/GNU-GUI-LINUX-RELEASE-SHARED-ST-XFT/
OUTDIR_CTRLCORE = $(OUTDIR_LNX)CtrlCore/GNU-GUI-LINUX-RELEASE-SHARED-ST-XFT/
OUTDIR_DRAW = $(OUTDIR_LNX)Draw/GNU-GUI-LINUX-RELEASE-SHARED-ST-XFT/
OUTDIR_CORE = $(OUTDIR_LNX)Core/GNU-GUI-LINUX-RELEASE-SHARED-ST-XFT/
OUTDIR_PLUGIN_ZLIB = $(OUTDIR_LNX)plugin/zlib/GNU-GUI-LINUX-RELEASE-SHARED-ST-XFT/
OUTDIR_DOCTYPES = $(OUTDIR_LNX)DocTypes/GNU-GUI-LINUX-RELEASE-SHARED-ST-XFT/
else
endif

ifeq ($(CONFIG), 4) # Linux Debug
CMACRO =  -DflagGNU -DflagGUI -DflagDEBUG -DflagLINUX -DflagST -DflagSHARED -DflagXFT
CINC =  -I"$(UPPDIR)" -I"G:/mingw2/i386-mingw32/include" -I"f:/uppsrc" -I"f:/uppsrc/platform" -I"e:/v/include" -I"d:/ORANT/OCI80/INCLUDE" -I"d:/mysql/include"
OBJ = .o
LIB = .a
CFLAGS = c++ -c -fuse-cxa-atexit -O0 -ggdb -g2 -D_DEBUG
UPPDIR = $(UPPDIR_LNX)
OUTDIR_WORK = $(OUTDIR_LNX)Work/DEBUG-GNU-GUI-LINUX-SHARED-ST-XFT/
OUTDIR_CTRLLIB = $(OUTDIR_LNX)CtrlLib/DEBUG-GNU-GUI-LINUX-SHARED-ST-XFT/
OUTDIR_CTRLCORE = $(OUTDIR_LNX)CtrlCore/DEBUG-GNU-GUI-LINUX-SHARED-ST-XFT/
OUTDIR_DRAW = $(OUTDIR_LNX)Draw/DEBUG-GNU-GUI-LINUX-SHARED-ST-XFT/
OUTDIR_CORE = $(OUTDIR_LNX)Core/DEBUG-GNU-GUI-LINUX-SHARED-ST-XFT/
OUTDIR_PLUGIN_ZLIB = $(OUTDIR_LNX)plugin/zlib/DEBUG-GNU-GUI-LINUX-SHARED-ST-XFT/
OUTDIR_DOCTYPES = $(OUTDIR_LNX)DocTypes/DEBUG-GNU-GUI-LINUX-SHARED-ST-XFT/
else
endif

BUILD_DIRS := ``

.PHONY: all
all: install $(OUTDIR_WORK)Work.exe

.PHONY: install
install:
	-mkdir -p $(OUTDIR_WORK)
	-mkdir -p $(OUTDIR_CTRLLIB)
	-mkdir -p $(OUTDIR_CTRLCORE)
	-mkdir -p $(OUTDIR_DRAW)
	-mkdir -p $(OUTDIR_CORE)
	-mkdir -p $(OUTDIR_PLUGIN_ZLIB)
	-mkdir -p $(OUTDIR_DOCTYPES)

$(OUTDIR_CTRLLIB)LabelBase$(OBJ): $(UPPDIR)CtrlLib\LabelBase.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\LabelBase.cpp -Fo$(OUTDIR_CTRLLIB)LabelBase$(OBJ)

$(OUTDIR_CTRLLIB)Button$(OBJ): $(UPPDIR)CtrlLib\Button.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\Button.cpp -Fo$(OUTDIR_CTRLLIB)Button$(OBJ)

$(OUTDIR_CTRLLIB)Switch$(OBJ): $(UPPDIR)CtrlLib\Switch.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\Switch.cpp -Fo$(OUTDIR_CTRLLIB)Switch$(OBJ)

$(OUTDIR_CTRLLIB)EditField$(OBJ): $(UPPDIR)CtrlLib\EditField.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\EditField.cpp -Fo$(OUTDIR_CTRLLIB)EditField$(OBJ)

$(OUTDIR_CTRLLIB)LineEdit$(OBJ): $(UPPDIR)CtrlLib\LineEdit.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\LineEdit.cpp -Fo$(OUTDIR_CTRLLIB)LineEdit$(OBJ)

$(OUTDIR_CTRLLIB)DocEdit$(OBJ): $(UPPDIR)CtrlLib\DocEdit.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\DocEdit.cpp -Fo$(OUTDIR_CTRLLIB)DocEdit$(OBJ)

$(OUTDIR_CTRLLIB)ScrollBar$(OBJ): $(UPPDIR)CtrlLib\ScrollBar.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\ScrollBar.cpp -Fo$(OUTDIR_CTRLLIB)ScrollBar$(OBJ)

$(OUTDIR_CTRLLIB)HeaderCtrl$(OBJ): $(UPPDIR)CtrlLib\HeaderCtrl.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\HeaderCtrl.cpp -Fo$(OUTDIR_CTRLLIB)HeaderCtrl$(OBJ)

$(OUTDIR_CTRLLIB)ArrayCtrl$(OBJ): $(UPPDIR)CtrlLib\ArrayCtrl.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\ArrayCtrl.cpp -Fo$(OUTDIR_CTRLLIB)ArrayCtrl$(OBJ)

$(OUTDIR_CTRLLIB)DropList$(OBJ): $(UPPDIR)CtrlLib\DropList.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\DropList.cpp -Fo$(OUTDIR_CTRLLIB)DropList$(OBJ)

$(OUTDIR_CTRLLIB)DropChoice$(OBJ): $(UPPDIR)CtrlLib\DropChoice.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\DropChoice.cpp -Fo$(OUTDIR_CTRLLIB)DropChoice$(OBJ)

$(OUTDIR_CTRLLIB)Static$(OBJ): $(UPPDIR)CtrlLib\Static.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\Static.cpp -Fo$(OUTDIR_CTRLLIB)Static$(OBJ)

$(OUTDIR_CTRLLIB)Splitter$(OBJ): $(UPPDIR)CtrlLib\Splitter.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\Splitter.cpp -Fo$(OUTDIR_CTRLLIB)Splitter$(OBJ)

$(OUTDIR_CTRLLIB)ColumnList$(OBJ): $(UPPDIR)CtrlLib\ColumnList.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\ColumnList.cpp -Fo$(OUTDIR_CTRLLIB)ColumnList$(OBJ)

$(OUTDIR_CTRLLIB)QtfCtrl$(OBJ): $(UPPDIR)CtrlLib\QtfCtrl.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\QtfCtrl.cpp -Fo$(OUTDIR_CTRLLIB)QtfCtrl$(OBJ)

$(OUTDIR_CTRLLIB)Progress$(OBJ): $(UPPDIR)CtrlLib\Progress.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\Progress.cpp -Fo$(OUTDIR_CTRLLIB)Progress$(OBJ)

$(OUTDIR_CTRLLIB)Bar$(OBJ): $(UPPDIR)CtrlLib\Bar.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\Bar.cpp -Fo$(OUTDIR_CTRLLIB)Bar$(OBJ)

$(OUTDIR_CTRLLIB)MenuBar$(OBJ): $(UPPDIR)CtrlLib\MenuBar.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\MenuBar.cpp -Fo$(OUTDIR_CTRLLIB)MenuBar$(OBJ)

$(OUTDIR_CTRLLIB)ToolBar$(OBJ): $(UPPDIR)CtrlLib\ToolBar.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\ToolBar.cpp -Fo$(OUTDIR_CTRLLIB)ToolBar$(OBJ)

$(OUTDIR_CTRLLIB)ToolTip$(OBJ): $(UPPDIR)CtrlLib\ToolTip.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\ToolTip.cpp -Fo$(OUTDIR_CTRLLIB)ToolTip$(OBJ)

$(OUTDIR_CTRLLIB)StatusBar$(OBJ): $(UPPDIR)CtrlLib\StatusBar.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\StatusBar.cpp -Fo$(OUTDIR_CTRLLIB)StatusBar$(OBJ)

$(OUTDIR_CTRLLIB)TabCtrl$(OBJ): $(UPPDIR)CtrlLib\TabCtrl.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\TabCtrl.cpp -Fo$(OUTDIR_CTRLLIB)TabCtrl$(OBJ)

$(OUTDIR_CTRLLIB)FileList$(OBJ): $(UPPDIR)CtrlLib\FileList.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\FileList.cpp -Fo$(OUTDIR_CTRLLIB)FileList$(OBJ)

$(OUTDIR_CTRLLIB)FileSel$(OBJ): $(UPPDIR)CtrlLib\FileSel.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\FileSel.cpp -Fo$(OUTDIR_CTRLLIB)FileSel$(OBJ)

$(OUTDIR_CTRLLIB)Windows$(OBJ): $(UPPDIR)CtrlLib\Windows.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\Windows.cpp -Fo$(OUTDIR_CTRLLIB)Windows$(OBJ)

$(OUTDIR_CTRLLIB)CtrlUtil$(OBJ): $(UPPDIR)CtrlLib\CtrlUtil.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	$(UPPDIR)Draw\iml_source.h \
	$(UPPDIR)Core\lng_master.h \
	$(UPPDIR)Core\lng_version.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.lay \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\CtrlUtil.cpp -Fo$(OUTDIR_CTRLLIB)CtrlUtil$(OBJ)

$(OUTDIR_CTRLLIB)Update$(OBJ): $(UPPDIR)CtrlLib\Update.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\Update.cpp -Fo$(OUTDIR_CTRLLIB)Update$(OBJ)

$(OUTDIR_CTRLLIB)LangEdit$(OBJ): $(UPPDIR)CtrlLib\LangEdit.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)Core\lng_master.h \
	$(UPPDIR)Core\lng_version.h \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\LangEdit.cpp -Fo$(OUTDIR_CTRLLIB)LangEdit$(OBJ)

$(OUTDIR_CTRLLIB)Report$(OBJ): $(UPPDIR)CtrlLib\Report.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\Report.cpp -Fo$(OUTDIR_CTRLLIB)Report$(OBJ)

$(OUTDIR_CTRLLIB)ShowReport$(OBJ): $(UPPDIR)CtrlLib\ShowReport.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlLib\ShowReport.cpp -Fo$(OUTDIR_CTRLLIB)ShowReport$(OBJ)

$(OUTDIR_CTRLLIB)CtrlLib.lib: $(OUTDIR_CTRLLIB)LabelBase$(OBJ) \
	$(OUTDIR_CTRLLIB)Button$(OBJ) \
	$(OUTDIR_CTRLLIB)Switch$(OBJ) \
	$(OUTDIR_CTRLLIB)EditField$(OBJ) \
	$(OUTDIR_CTRLLIB)LineEdit$(OBJ) \
	$(OUTDIR_CTRLLIB)DocEdit$(OBJ) \
	$(OUTDIR_CTRLLIB)ScrollBar$(OBJ) \
	$(OUTDIR_CTRLLIB)HeaderCtrl$(OBJ) \
	$(OUTDIR_CTRLLIB)ArrayCtrl$(OBJ) \
	$(OUTDIR_CTRLLIB)DropList$(OBJ) \
	$(OUTDIR_CTRLLIB)DropChoice$(OBJ) \
	$(OUTDIR_CTRLLIB)Static$(OBJ) \
	$(OUTDIR_CTRLLIB)Splitter$(OBJ) \
	$(OUTDIR_CTRLLIB)ColumnList$(OBJ) \
	$(OUTDIR_CTRLLIB)QtfCtrl$(OBJ) \
	$(OUTDIR_CTRLLIB)Progress$(OBJ) \
	$(OUTDIR_CTRLLIB)Bar$(OBJ) \
	$(OUTDIR_CTRLLIB)MenuBar$(OBJ) \
	$(OUTDIR_CTRLLIB)ToolBar$(OBJ) \
	$(OUTDIR_CTRLLIB)ToolTip$(OBJ) \
	$(OUTDIR_CTRLLIB)StatusBar$(OBJ) \
	$(OUTDIR_CTRLLIB)TabCtrl$(OBJ) \
	$(OUTDIR_CTRLLIB)FileList$(OBJ) \
	$(OUTDIR_CTRLLIB)FileSel$(OBJ) \
	$(OUTDIR_CTRLLIB)Windows$(OBJ) \
	$(OUTDIR_CTRLLIB)CtrlUtil$(OBJ) \
	$(OUTDIR_CTRLLIB)Update$(OBJ) \
	$(OUTDIR_CTRLLIB)LangEdit$(OBJ) \
	$(OUTDIR_CTRLLIB)Report$(OBJ) \
	$(OUTDIR_CTRLLIB)ShowReport$(OBJ)
	lib -nologo -out:$(OUTDIR_CTRLLIB)CtrlLib.lib $(OUTDIR_CTRLLIB)LabelBase$(OBJ) $(OUTDIR_CTRLLIB)Button$(OBJ) $(OUTDIR_CTRLLIB)Switch$(OBJ) $(OUTDIR_CTRLLIB)EditField$(OBJ) $(OUTDIR_CTRLLIB)LineEdit$(OBJ) $(OUTDIR_CTRLLIB)DocEdit$(OBJ) $(OUTDIR_CTRLLIB)ScrollBar$(OBJ) $(OUTDIR_CTRLLIB)HeaderCtrl$(OBJ) $(OUTDIR_CTRLLIB)ArrayCtrl$(OBJ) $(OUTDIR_CTRLLIB)DropList$(OBJ) $(OUTDIR_CTRLLIB)DropChoice$(OBJ) $(OUTDIR_CTRLLIB)Static$(OBJ) $(OUTDIR_CTRLLIB)Splitter$(OBJ) $(OUTDIR_CTRLLIB)ColumnList$(OBJ) $(OUTDIR_CTRLLIB)QtfCtrl$(OBJ) $(OUTDIR_CTRLLIB)Progress$(OBJ) $(OUTDIR_CTRLLIB)Bar$(OBJ) $(OUTDIR_CTRLLIB)MenuBar$(OBJ) $(OUTDIR_CTRLLIB)ToolBar$(OBJ) $(OUTDIR_CTRLLIB)ToolTip$(OBJ) $(OUTDIR_CTRLLIB)StatusBar$(OBJ) $(OUTDIR_CTRLLIB)TabCtrl$(OBJ) $(OUTDIR_CTRLLIB)FileList$(OBJ) $(OUTDIR_CTRLLIB)FileSel$(OBJ) $(OUTDIR_CTRLLIB)Windows$(OBJ) $(OUTDIR_CTRLLIB)CtrlUtil$(OBJ) $(OUTDIR_CTRLLIB)Update$(OBJ) $(OUTDIR_CTRLLIB)LangEdit$(OBJ) $(OUTDIR_CTRLLIB)Report$(OBJ) $(OUTDIR_CTRLLIB)ShowReport$(OBJ)

$(OUTDIR_CTRLCORE)Frame$(OBJ): $(UPPDIR)CtrlCore\Frame.cpp \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlCore\Frame.cpp -Fo$(OUTDIR_CTRLCORE)Frame$(OBJ)

$(OUTDIR_CTRLCORE)Ctrl$(OBJ): $(UPPDIR)CtrlCore\Ctrl.cpp \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlCore\Ctrl.cpp -Fo$(OUTDIR_CTRLCORE)Ctrl$(OBJ)

$(OUTDIR_CTRLCORE)CtrlChild$(OBJ): $(UPPDIR)CtrlCore\CtrlChild.cpp \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlCore\CtrlChild.cpp -Fo$(OUTDIR_CTRLCORE)CtrlChild$(OBJ)

$(OUTDIR_CTRLCORE)CtrlPos$(OBJ): $(UPPDIR)CtrlCore\CtrlPos.cpp \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlCore\CtrlPos.cpp -Fo$(OUTDIR_CTRLCORE)CtrlPos$(OBJ)

$(OUTDIR_CTRLCORE)CtrlDraw$(OBJ): $(UPPDIR)CtrlCore\CtrlDraw.cpp \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlCore\CtrlDraw.cpp -Fo$(OUTDIR_CTRLCORE)CtrlDraw$(OBJ)

$(OUTDIR_CTRLCORE)CtrlMouse$(OBJ): $(UPPDIR)CtrlCore\CtrlMouse.cpp \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlCore\CtrlMouse.cpp -Fo$(OUTDIR_CTRLCORE)CtrlMouse$(OBJ)

$(OUTDIR_CTRLCORE)CtrlKbd$(OBJ): $(UPPDIR)CtrlCore\CtrlKbd.cpp \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlCore\CtrlKbd.cpp -Fo$(OUTDIR_CTRLCORE)CtrlKbd$(OBJ)

$(OUTDIR_CTRLCORE)CtrlTimer$(OBJ): $(UPPDIR)CtrlCore\CtrlTimer.cpp \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlCore\CtrlTimer.cpp -Fo$(OUTDIR_CTRLCORE)CtrlTimer$(OBJ)

$(OUTDIR_CTRLCORE)Win32Wnd$(OBJ): $(UPPDIR)CtrlCore\Win32Wnd.cpp \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)CtrlCore\Win32Msg.i \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlCore\Win32Wnd.cpp -Fo$(OUTDIR_CTRLCORE)Win32Wnd$(OBJ)

$(OUTDIR_CTRLCORE)Win32Proc$(OBJ): $(UPPDIR)CtrlCore\Win32Proc.cpp \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlCore\Win32Proc.cpp -Fo$(OUTDIR_CTRLCORE)Win32Proc$(OBJ)

$(OUTDIR_CTRLCORE)X11Wnd$(OBJ): $(UPPDIR)CtrlCore\X11Wnd.cpp \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)CtrlCore\X11Event.i \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlCore\X11Wnd.cpp -Fo$(OUTDIR_CTRLCORE)X11Wnd$(OBJ)

$(OUTDIR_CTRLCORE)X11Proc$(OBJ): $(UPPDIR)CtrlCore\X11Proc.cpp \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlCore\X11Proc.cpp -Fo$(OUTDIR_CTRLCORE)X11Proc$(OBJ)

$(OUTDIR_CTRLCORE)LocalLoop$(OBJ): $(UPPDIR)CtrlCore\LocalLoop.cpp \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlCore\LocalLoop.cpp -Fo$(OUTDIR_CTRLCORE)LocalLoop$(OBJ)

$(OUTDIR_CTRLCORE)TopWindow$(OBJ): $(UPPDIR)CtrlCore\TopWindow.cpp \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlCore\TopWindow.cpp -Fo$(OUTDIR_CTRLCORE)TopWindow$(OBJ)

$(OUTDIR_CTRLCORE)TopWin32$(OBJ): $(UPPDIR)CtrlCore\TopWin32.cpp \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlCore\TopWin32.cpp -Fo$(OUTDIR_CTRLCORE)TopWin32$(OBJ)

$(OUTDIR_CTRLCORE)TopWinX11$(OBJ): $(UPPDIR)CtrlCore\TopWinX11.cpp \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)Draw\PixelUtil.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)CtrlCore\TopWinX11.cpp -Fo$(OUTDIR_CTRLCORE)TopWinX11$(OBJ)

$(OUTDIR_CTRLCORE)CtrlCore.lib: $(OUTDIR_CTRLCORE)Frame$(OBJ) \
	$(OUTDIR_CTRLCORE)Ctrl$(OBJ) \
	$(OUTDIR_CTRLCORE)CtrlChild$(OBJ) \
	$(OUTDIR_CTRLCORE)CtrlPos$(OBJ) \
	$(OUTDIR_CTRLCORE)CtrlDraw$(OBJ) \
	$(OUTDIR_CTRLCORE)CtrlMouse$(OBJ) \
	$(OUTDIR_CTRLCORE)CtrlKbd$(OBJ) \
	$(OUTDIR_CTRLCORE)CtrlTimer$(OBJ) \
	$(OUTDIR_CTRLCORE)Win32Wnd$(OBJ) \
	$(OUTDIR_CTRLCORE)Win32Proc$(OBJ) \
	$(OUTDIR_CTRLCORE)X11Wnd$(OBJ) \
	$(OUTDIR_CTRLCORE)X11Proc$(OBJ) \
	$(OUTDIR_CTRLCORE)LocalLoop$(OBJ) \
	$(OUTDIR_CTRLCORE)TopWindow$(OBJ) \
	$(OUTDIR_CTRLCORE)TopWin32$(OBJ) \
	$(OUTDIR_CTRLCORE)TopWinX11$(OBJ)
	lib -nologo -out:$(OUTDIR_CTRLCORE)CtrlCore.lib $(OUTDIR_CTRLCORE)Frame$(OBJ) $(OUTDIR_CTRLCORE)Ctrl$(OBJ) $(OUTDIR_CTRLCORE)CtrlChild$(OBJ) $(OUTDIR_CTRLCORE)CtrlPos$(OBJ) $(OUTDIR_CTRLCORE)CtrlDraw$(OBJ) $(OUTDIR_CTRLCORE)CtrlMouse$(OBJ) $(OUTDIR_CTRLCORE)CtrlKbd$(OBJ) $(OUTDIR_CTRLCORE)CtrlTimer$(OBJ) $(OUTDIR_CTRLCORE)Win32Wnd$(OBJ) $(OUTDIR_CTRLCORE)Win32Proc$(OBJ) $(OUTDIR_CTRLCORE)X11Wnd$(OBJ) $(OUTDIR_CTRLCORE)X11Proc$(OBJ) $(OUTDIR_CTRLCORE)LocalLoop$(OBJ) $(OUTDIR_CTRLCORE)TopWindow$(OBJ) $(OUTDIR_CTRLCORE)TopWin32$(OBJ) $(OUTDIR_CTRLCORE)TopWinX11$(OBJ)

$(OUTDIR_DRAW)DrawWin32$(OBJ): $(UPPDIR)Draw\DrawWin32.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\DrawWin32.cpp -Fo$(OUTDIR_DRAW)DrawWin32$(OBJ)

$(OUTDIR_DRAW)DrawX11$(OBJ): $(UPPDIR)Draw\DrawX11.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\DrawX11.cpp -Fo$(OUTDIR_DRAW)DrawX11$(OBJ)

$(OUTDIR_DRAW)DrawText$(OBJ): $(UPPDIR)Draw\DrawText.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\DrawText.cpp -Fo$(OUTDIR_DRAW)DrawText$(OBJ)

$(OUTDIR_DRAW)DrawTextWin32$(OBJ): $(UPPDIR)Draw\DrawTextWin32.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\DrawTextWin32.cpp -Fo$(OUTDIR_DRAW)DrawTextWin32$(OBJ)

$(OUTDIR_DRAW)DrawTextX11$(OBJ): $(UPPDIR)Draw\DrawTextX11.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\DrawTextX11.cpp -Fo$(OUTDIR_DRAW)DrawTextX11$(OBJ)

$(OUTDIR_DRAW)Draw$(OBJ): $(UPPDIR)Draw\Draw.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\Draw.cpp -Fo$(OUTDIR_DRAW)Draw$(OBJ)

$(OUTDIR_DRAW)DrawOpWin32$(OBJ): $(UPPDIR)Draw\DrawOpWin32.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\DrawOpWin32.cpp -Fo$(OUTDIR_DRAW)DrawOpWin32$(OBJ)

$(OUTDIR_DRAW)DrawOpX11$(OBJ): $(UPPDIR)Draw\DrawOpX11.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\DrawOpX11.cpp -Fo$(OUTDIR_DRAW)DrawOpX11$(OBJ)

$(OUTDIR_DRAW)Drawing$(OBJ): $(UPPDIR)Draw\Drawing.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\Drawing.cpp -Fo$(OUTDIR_DRAW)Drawing$(OBJ)

$(OUTDIR_DRAW)MetaFile$(OBJ): $(UPPDIR)Draw\MetaFile.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\MetaFile.cpp -Fo$(OUTDIR_DRAW)MetaFile$(OBJ)

$(OUTDIR_DRAW)DrawUtil$(OBJ): $(UPPDIR)Draw\DrawUtil.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\DrawUtil.cpp -Fo$(OUTDIR_DRAW)DrawUtil$(OBJ)

$(OUTDIR_DRAW)Display$(OBJ): $(UPPDIR)Draw\Display.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\Display.cpp -Fo$(OUTDIR_DRAW)Display$(OBJ)

$(OUTDIR_DRAW)Debug$(OBJ): $(UPPDIR)Draw\Debug.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\Debug.cpp -Fo$(OUTDIR_DRAW)Debug$(OBJ)

$(OUTDIR_DRAW)PixelArray$(OBJ): $(UPPDIR)Draw\PixelArray.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Draw\PixelUtil.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\PixelArray.cpp -Fo$(OUTDIR_DRAW)PixelArray$(OBJ)

$(OUTDIR_DRAW)PixelBlt$(OBJ): $(UPPDIR)Draw\PixelBlt.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\PixelBlt.cpp -Fo$(OUTDIR_DRAW)PixelBlt$(OBJ)

$(OUTDIR_DRAW)PixelPal$(OBJ): $(UPPDIR)Draw\PixelPal.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Draw\PixelUtil.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\PixelPal.cpp -Fo$(OUTDIR_DRAW)PixelPal$(OBJ)

$(OUTDIR_DRAW)PixelUtil$(OBJ): $(UPPDIR)Draw\PixelUtil.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Draw\PixelUtil.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\PixelUtil.cpp -Fo$(OUTDIR_DRAW)PixelUtil$(OBJ)

$(OUTDIR_DRAW)Image$(OBJ): $(UPPDIR)Draw\Image.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Draw\PixelUtil.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\Image.cpp -Fo$(OUTDIR_DRAW)Image$(OBJ)

$(OUTDIR_DRAW)ImageUtil$(OBJ): $(UPPDIR)Draw\ImageUtil.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Draw\PixelUtil.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\ImageUtil.cpp -Fo$(OUTDIR_DRAW)ImageUtil$(OBJ)

$(OUTDIR_DRAW)io$(OBJ): $(UPPDIR)Draw\io.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\io.cpp -Fo$(OUTDIR_DRAW)io$(OBJ)

$(OUTDIR_DRAW)bmp$(OBJ): $(UPPDIR)Draw\bmp.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\bmp.cpp -Fo$(OUTDIR_DRAW)bmp$(OBJ)

$(OUTDIR_DRAW)zimage$(OBJ): $(UPPDIR)Draw\zimage.cpp \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)plugin\z.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Draw\zimage.cpp -Fo$(OUTDIR_DRAW)zimage$(OBJ)

$(OUTDIR_DRAW)Draw.lib: $(OUTDIR_DRAW)DrawWin32$(OBJ) \
	$(OUTDIR_DRAW)DrawX11$(OBJ) \
	$(OUTDIR_DRAW)DrawText$(OBJ) \
	$(OUTDIR_DRAW)DrawTextWin32$(OBJ) \
	$(OUTDIR_DRAW)DrawTextX11$(OBJ) \
	$(OUTDIR_DRAW)Draw$(OBJ) \
	$(OUTDIR_DRAW)DrawOpWin32$(OBJ) \
	$(OUTDIR_DRAW)DrawOpX11$(OBJ) \
	$(OUTDIR_DRAW)Drawing$(OBJ) \
	$(OUTDIR_DRAW)MetaFile$(OBJ) \
	$(OUTDIR_DRAW)DrawUtil$(OBJ) \
	$(OUTDIR_DRAW)Display$(OBJ) \
	$(OUTDIR_DRAW)Debug$(OBJ) \
	$(OUTDIR_DRAW)PixelArray$(OBJ) \
	$(OUTDIR_DRAW)PixelBlt$(OBJ) \
	$(OUTDIR_DRAW)PixelPal$(OBJ) \
	$(OUTDIR_DRAW)PixelUtil$(OBJ) \
	$(OUTDIR_DRAW)Image$(OBJ) \
	$(OUTDIR_DRAW)ImageUtil$(OBJ) \
	$(OUTDIR_DRAW)io$(OBJ) \
	$(OUTDIR_DRAW)bmp$(OBJ) \
	$(OUTDIR_DRAW)zimage$(OBJ)
	lib -nologo -out:$(OUTDIR_DRAW)Draw.lib $(OUTDIR_DRAW)DrawWin32$(OBJ) $(OUTDIR_DRAW)DrawX11$(OBJ) $(OUTDIR_DRAW)DrawText$(OBJ) $(OUTDIR_DRAW)DrawTextWin32$(OBJ) $(OUTDIR_DRAW)DrawTextX11$(OBJ) $(OUTDIR_DRAW)Draw$(OBJ) $(OUTDIR_DRAW)DrawOpWin32$(OBJ) $(OUTDIR_DRAW)DrawOpX11$(OBJ) $(OUTDIR_DRAW)Drawing$(OBJ) $(OUTDIR_DRAW)MetaFile$(OBJ) $(OUTDIR_DRAW)DrawUtil$(OBJ) $(OUTDIR_DRAW)Display$(OBJ) $(OUTDIR_DRAW)Debug$(OBJ) $(OUTDIR_DRAW)PixelArray$(OBJ) $(OUTDIR_DRAW)PixelBlt$(OBJ) $(OUTDIR_DRAW)PixelPal$(OBJ) $(OUTDIR_DRAW)PixelUtil$(OBJ) $(OUTDIR_DRAW)Image$(OBJ) $(OUTDIR_DRAW)ImageUtil$(OBJ) $(OUTDIR_DRAW)io$(OBJ) $(OUTDIR_DRAW)bmp$(OBJ) $(OUTDIR_DRAW)zimage$(OBJ)

$(OUTDIR_CORE)Mt$(OBJ): $(UPPDIR)Core\Mt.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\Mt.cpp -Fo$(OUTDIR_CORE)Mt$(OBJ)

$(OUTDIR_CORE)Thread$(OBJ): $(UPPDIR)Core\Thread.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\Thread.cpp -Fo$(OUTDIR_CORE)Thread$(OBJ)

$(OUTDIR_CORE)heap$(OBJ): $(UPPDIR)Core\heap.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\heap.cpp -Fo$(OUTDIR_CORE)heap$(OBJ)

$(OUTDIR_CORE)String$(OBJ): $(UPPDIR)Core\String.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\String.cpp -Fo$(OUTDIR_CORE)String$(OBJ)

$(OUTDIR_CORE)WString$(OBJ): $(UPPDIR)Core\WString.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\WString.cpp -Fo$(OUTDIR_CORE)WString$(OBJ)

$(OUTDIR_CORE)StrUtil$(OBJ): $(UPPDIR)Core\StrUtil.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\StrUtil.cpp -Fo$(OUTDIR_CORE)StrUtil$(OBJ)

$(OUTDIR_CORE)CharSet$(OBJ): $(UPPDIR)Core\CharSet.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\CharSet.cpp -Fo$(OUTDIR_CORE)CharSet$(OBJ)

$(OUTDIR_CORE)Path$(OBJ): $(UPPDIR)Core\Path.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\Path.cpp -Fo$(OUTDIR_CORE)Path$(OBJ)

$(OUTDIR_CORE)App$(OBJ): $(UPPDIR)Core\App.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\App.cpp -Fo$(OUTDIR_CORE)App$(OBJ)

$(OUTDIR_CORE)Stream$(OBJ): $(UPPDIR)Core\Stream.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\Stream.cpp -Fo$(OUTDIR_CORE)Stream$(OBJ)

$(OUTDIR_CORE)Debug$(OBJ): $(UPPDIR)Core\Debug.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\Debug.cpp -Fo$(OUTDIR_CORE)Debug$(OBJ)

$(OUTDIR_CORE)util$(OBJ): $(UPPDIR)Core\util.cpp \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Core\lng_master.h \
	$(UPPDIR)Core\lng_version.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\util.cpp -Fo$(OUTDIR_CORE)util$(OBJ)

$(OUTDIR_CORE)mathutil$(OBJ): $(UPPDIR)Core\mathutil.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\mathutil.cpp -Fo$(OUTDIR_CORE)mathutil$(OBJ)

$(OUTDIR_CORE)Cont$(OBJ): $(UPPDIR)Core\Cont.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\Cont.cpp -Fo$(OUTDIR_CORE)Cont$(OBJ)

$(OUTDIR_CORE)Callback$(OBJ): $(UPPDIR)Core\Callback.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\Callback.cpp -Fo$(OUTDIR_CORE)Callback$(OBJ)

$(OUTDIR_CORE)Color$(OBJ): $(UPPDIR)Core\Color.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\Color.cpp -Fo$(OUTDIR_CORE)Color$(OBJ)

$(OUTDIR_CORE)Gtypes$(OBJ): $(UPPDIR)Core\Gtypes.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\Gtypes.cpp -Fo$(OUTDIR_CORE)Gtypes$(OBJ)

$(OUTDIR_CORE)TimeDate$(OBJ): $(UPPDIR)Core\TimeDate.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\TimeDate.cpp -Fo$(OUTDIR_CORE)TimeDate$(OBJ)

$(OUTDIR_CORE)Value$(OBJ): $(UPPDIR)Core\Value.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\Value.cpp -Fo$(OUTDIR_CORE)Value$(OBJ)

$(OUTDIR_CORE)Convert$(OBJ): $(UPPDIR)Core\Convert.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\Convert.cpp -Fo$(OUTDIR_CORE)Convert$(OBJ)

$(OUTDIR_CORE)Format$(OBJ): $(UPPDIR)Core\Format.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\Format.cpp -Fo$(OUTDIR_CORE)Format$(OBJ)

$(OUTDIR_CORE)Lang$(OBJ): $(UPPDIR)Core\Lang.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\Lang.cpp -Fo$(OUTDIR_CORE)Lang$(OBJ)

$(OUTDIR_CORE)parser$(OBJ): $(UPPDIR)Core\parser.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\parser.cpp -Fo$(OUTDIR_CORE)parser$(OBJ)

$(OUTDIR_CORE)Ptr$(OBJ): $(UPPDIR)Core\Ptr.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\Ptr.cpp -Fo$(OUTDIR_CORE)Ptr$(OBJ)

$(OUTDIR_CORE)Win32Com$(OBJ): $(UPPDIR)Core\Win32Com.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\Win32Com.cpp -Fo$(OUTDIR_CORE)Win32Com$(OBJ)

$(OUTDIR_CORE)Dli$(OBJ): $(UPPDIR)Core\Dli.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\Dli.cpp -Fo$(OUTDIR_CORE)Dli$(OBJ)

$(OUTDIR_CORE)Test$(OBJ): $(UPPDIR)Core\Test.cpp \
	$(UPPDIR)Core\Core.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)Core\Test.cpp -Fo$(OUTDIR_CORE)Test$(OBJ)

$(OUTDIR_CORE)Core.lib: $(OUTDIR_CORE)Mt$(OBJ) \
	$(OUTDIR_CORE)Thread$(OBJ) \
	$(OUTDIR_CORE)heap$(OBJ) \
	$(OUTDIR_CORE)String$(OBJ) \
	$(OUTDIR_CORE)WString$(OBJ) \
	$(OUTDIR_CORE)StrUtil$(OBJ) \
	$(OUTDIR_CORE)CharSet$(OBJ) \
	$(OUTDIR_CORE)Path$(OBJ) \
	$(OUTDIR_CORE)App$(OBJ) \
	$(OUTDIR_CORE)Stream$(OBJ) \
	$(OUTDIR_CORE)Debug$(OBJ) \
	$(OUTDIR_CORE)util$(OBJ) \
	$(OUTDIR_CORE)mathutil$(OBJ) \
	$(OUTDIR_CORE)Cont$(OBJ) \
	$(OUTDIR_CORE)Callback$(OBJ) \
	$(OUTDIR_CORE)Color$(OBJ) \
	$(OUTDIR_CORE)Gtypes$(OBJ) \
	$(OUTDIR_CORE)TimeDate$(OBJ) \
	$(OUTDIR_CORE)Value$(OBJ) \
	$(OUTDIR_CORE)Convert$(OBJ) \
	$(OUTDIR_CORE)Format$(OBJ) \
	$(OUTDIR_CORE)Lang$(OBJ) \
	$(OUTDIR_CORE)parser$(OBJ) \
	$(OUTDIR_CORE)Ptr$(OBJ) \
	$(OUTDIR_CORE)Win32Com$(OBJ) \
	$(OUTDIR_CORE)Dli$(OBJ) \
	$(OUTDIR_CORE)Test$(OBJ)
	lib -nologo -out:$(OUTDIR_CORE)Core.lib $(OUTDIR_CORE)Mt$(OBJ) $(OUTDIR_CORE)Thread$(OBJ) $(OUTDIR_CORE)heap$(OBJ) $(OUTDIR_CORE)String$(OBJ) $(OUTDIR_CORE)WString$(OBJ) $(OUTDIR_CORE)StrUtil$(OBJ) $(OUTDIR_CORE)CharSet$(OBJ) $(OUTDIR_CORE)Path$(OBJ) $(OUTDIR_CORE)App$(OBJ) $(OUTDIR_CORE)Stream$(OBJ) $(OUTDIR_CORE)Debug$(OBJ) $(OUTDIR_CORE)util$(OBJ) $(OUTDIR_CORE)mathutil$(OBJ) $(OUTDIR_CORE)Cont$(OBJ) $(OUTDIR_CORE)Callback$(OBJ) $(OUTDIR_CORE)Color$(OBJ) $(OUTDIR_CORE)Gtypes$(OBJ) $(OUTDIR_CORE)TimeDate$(OBJ) $(OUTDIR_CORE)Value$(OBJ) $(OUTDIR_CORE)Convert$(OBJ) $(OUTDIR_CORE)Format$(OBJ) $(OUTDIR_CORE)Lang$(OBJ) $(OUTDIR_CORE)parser$(OBJ) $(OUTDIR_CORE)Ptr$(OBJ) $(OUTDIR_CORE)Win32Com$(OBJ) $(OUTDIR_CORE)Dli$(OBJ) $(OUTDIR_CORE)Test$(OBJ)

$(OUTDIR_PLUGIN_ZLIB)zlibupp$(OBJ): $(UPPDIR)plugin\zlib\zlibupp.cpp \
	$(UPPDIR)core\core.h \
	$(UPPDIR)plugin\zlib\zlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)core\Core.lng \
	$(UPPDIR)plugin\zlib\zconf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)plugin\zlib\zlibupp.cpp -Fo$(OUTDIR_PLUGIN_ZLIB)zlibupp$(OBJ)

$(OUTDIR_PLUGIN_ZLIB)adler32$(OBJ): $(UPPDIR)plugin\zlib\adler32.c \
	$(UPPDIR)plugin\zlib\zlib.h \
	$(UPPDIR)plugin\zlib\zconf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)   $(UPPDIR)plugin\zlib\adler32.c -Fo$(OUTDIR_PLUGIN_ZLIB)adler32$(OBJ)

$(OUTDIR_PLUGIN_ZLIB)compress$(OBJ): $(UPPDIR)plugin\zlib\compress.c \
	$(UPPDIR)plugin\zlib\zlib.h \
	$(UPPDIR)plugin\zlib\zconf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)   $(UPPDIR)plugin\zlib\compress.c -Fo$(OUTDIR_PLUGIN_ZLIB)compress$(OBJ)

$(OUTDIR_PLUGIN_ZLIB)crc32$(OBJ): $(UPPDIR)plugin\zlib\crc32.c \
	$(UPPDIR)plugin\zlib\zlib.h \
	$(UPPDIR)plugin\zlib\zconf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)   $(UPPDIR)plugin\zlib\crc32.c -Fo$(OUTDIR_PLUGIN_ZLIB)crc32$(OBJ)

$(OUTDIR_PLUGIN_ZLIB)deflate$(OBJ): $(UPPDIR)plugin\zlib\deflate.c \
	$(UPPDIR)plugin\zlib\deflate.h \
	$(UPPDIR)plugin\zlib\zutil.h \
	$(UPPDIR)plugin\zlib\zlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	$(UPPDIR)plugin\zlib\zconf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)   $(UPPDIR)plugin\zlib\deflate.c -Fo$(OUTDIR_PLUGIN_ZLIB)deflate$(OBJ)

$(OUTDIR_PLUGIN_ZLIB)gzio$(OBJ): $(UPPDIR)plugin\zlib\gzio.c \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	$(UPPDIR)plugin\zlib\zutil.h \
	$(UPPDIR)plugin\zlib\zlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	$(UPPDIR)plugin\zlib\zconf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)   $(UPPDIR)plugin\zlib\gzio.c -Fo$(OUTDIR_PLUGIN_ZLIB)gzio$(OBJ)

$(OUTDIR_PLUGIN_ZLIB)infblock$(OBJ): $(UPPDIR)plugin\zlib\infblock.c \
	$(UPPDIR)plugin\zlib\zutil.h \
	$(UPPDIR)plugin\zlib\infblock.h \
	$(UPPDIR)plugin\zlib\inftrees.h \
	$(UPPDIR)plugin\zlib\infcodes.h \
	$(UPPDIR)plugin\zlib\infutil.h \
	$(UPPDIR)plugin\zlib\zlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	$(UPPDIR)plugin\zlib\zconf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)   $(UPPDIR)plugin\zlib\infblock.c -Fo$(OUTDIR_PLUGIN_ZLIB)infblock$(OBJ)

$(OUTDIR_PLUGIN_ZLIB)infcodes$(OBJ): $(UPPDIR)plugin\zlib\infcodes.c \
	$(UPPDIR)plugin\zlib\zutil.h \
	$(UPPDIR)plugin\zlib\inftrees.h \
	$(UPPDIR)plugin\zlib\infblock.h \
	$(UPPDIR)plugin\zlib\infcodes.h \
	$(UPPDIR)plugin\zlib\infutil.h \
	$(UPPDIR)plugin\zlib\inffast.h \
	$(UPPDIR)plugin\zlib\zlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	$(UPPDIR)plugin\zlib\zconf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)   $(UPPDIR)plugin\zlib\infcodes.c -Fo$(OUTDIR_PLUGIN_ZLIB)infcodes$(OBJ)

$(OUTDIR_PLUGIN_ZLIB)inffast$(OBJ): $(UPPDIR)plugin\zlib\inffast.c \
	$(UPPDIR)plugin\zlib\zutil.h \
	$(UPPDIR)plugin\zlib\inftrees.h \
	$(UPPDIR)plugin\zlib\infblock.h \
	$(UPPDIR)plugin\zlib\infcodes.h \
	$(UPPDIR)plugin\zlib\infutil.h \
	$(UPPDIR)plugin\zlib\inffast.h \
	$(UPPDIR)plugin\zlib\zlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	$(UPPDIR)plugin\zlib\zconf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)   $(UPPDIR)plugin\zlib\inffast.c -Fo$(OUTDIR_PLUGIN_ZLIB)inffast$(OBJ)

$(OUTDIR_PLUGIN_ZLIB)inflate$(OBJ): $(UPPDIR)plugin\zlib\inflate.c \
	$(UPPDIR)plugin\zlib\zutil.h \
	$(UPPDIR)plugin\zlib\infblock.h \
	$(UPPDIR)plugin\zlib\zlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	$(UPPDIR)plugin\zlib\zconf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)   $(UPPDIR)plugin\zlib\inflate.c -Fo$(OUTDIR_PLUGIN_ZLIB)inflate$(OBJ)

$(OUTDIR_PLUGIN_ZLIB)inftrees$(OBJ): $(UPPDIR)plugin\zlib\inftrees.c \
	$(UPPDIR)plugin\zlib\zutil.h \
	$(UPPDIR)plugin\zlib\inftrees.h \
	$(UPPDIR)plugin\zlib\zlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	$(UPPDIR)plugin\zlib\zconf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)   $(UPPDIR)plugin\zlib\inftrees.c -Fo$(OUTDIR_PLUGIN_ZLIB)inftrees$(OBJ)

$(OUTDIR_PLUGIN_ZLIB)infutil$(OBJ): $(UPPDIR)plugin\zlib\infutil.c \
	$(UPPDIR)plugin\zlib\zutil.h \
	$(UPPDIR)plugin\zlib\infblock.h \
	$(UPPDIR)plugin\zlib\inftrees.h \
	$(UPPDIR)plugin\zlib\infcodes.h \
	$(UPPDIR)plugin\zlib\infutil.h \
	$(UPPDIR)plugin\zlib\zlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	$(UPPDIR)plugin\zlib\zconf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)   $(UPPDIR)plugin\zlib\infutil.c -Fo$(OUTDIR_PLUGIN_ZLIB)infutil$(OBJ)

$(OUTDIR_PLUGIN_ZLIB)trees$(OBJ): $(UPPDIR)plugin\zlib\trees.c \
	$(UPPDIR)plugin\zlib\deflate.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	$(UPPDIR)plugin\zlib\zutil.h \
	$(UPPDIR)plugin\zlib\zlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	$(UPPDIR)plugin\zlib\zconf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)   $(UPPDIR)plugin\zlib\trees.c -Fo$(OUTDIR_PLUGIN_ZLIB)trees$(OBJ)

$(OUTDIR_PLUGIN_ZLIB)uncompr$(OBJ): $(UPPDIR)plugin\zlib\uncompr.c \
	$(UPPDIR)plugin\zlib\zlib.h \
	$(UPPDIR)plugin\zlib\zconf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)   $(UPPDIR)plugin\zlib\uncompr.c -Fo$(OUTDIR_PLUGIN_ZLIB)uncompr$(OBJ)

$(OUTDIR_PLUGIN_ZLIB)zutil$(OBJ): $(UPPDIR)plugin\zlib\zutil.c \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	$(UPPDIR)plugin\zlib\zutil.h \
	$(UPPDIR)plugin\zlib\zlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	$(UPPDIR)plugin\zlib\zconf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)   $(UPPDIR)plugin\zlib\zutil.c -Fo$(OUTDIR_PLUGIN_ZLIB)zutil$(OBJ)

$(OUTDIR_PLUGIN_ZLIB)zlib.lib: $(OUTDIR_PLUGIN_ZLIB)zlibupp$(OBJ) \
	$(OUTDIR_PLUGIN_ZLIB)adler32$(OBJ) \
	$(OUTDIR_PLUGIN_ZLIB)compress$(OBJ) \
	$(OUTDIR_PLUGIN_ZLIB)crc32$(OBJ) \
	$(OUTDIR_PLUGIN_ZLIB)deflate$(OBJ) \
	$(OUTDIR_PLUGIN_ZLIB)gzio$(OBJ) \
	$(OUTDIR_PLUGIN_ZLIB)infblock$(OBJ) \
	$(OUTDIR_PLUGIN_ZLIB)infcodes$(OBJ) \
	$(OUTDIR_PLUGIN_ZLIB)inffast$(OBJ) \
	$(OUTDIR_PLUGIN_ZLIB)inflate$(OBJ) \
	$(OUTDIR_PLUGIN_ZLIB)inftrees$(OBJ) \
	$(OUTDIR_PLUGIN_ZLIB)infutil$(OBJ) \
	$(OUTDIR_PLUGIN_ZLIB)trees$(OBJ) \
	$(OUTDIR_PLUGIN_ZLIB)uncompr$(OBJ) \
	$(OUTDIR_PLUGIN_ZLIB)zutil$(OBJ)
	lib -nologo -out:$(OUTDIR_PLUGIN_ZLIB)zlib.lib $(OUTDIR_PLUGIN_ZLIB)zlibupp$(OBJ) $(OUTDIR_PLUGIN_ZLIB)adler32$(OBJ) $(OUTDIR_PLUGIN_ZLIB)compress$(OBJ) $(OUTDIR_PLUGIN_ZLIB)crc32$(OBJ) $(OUTDIR_PLUGIN_ZLIB)deflate$(OBJ) $(OUTDIR_PLUGIN_ZLIB)gzio$(OBJ) $(OUTDIR_PLUGIN_ZLIB)infblock$(OBJ) $(OUTDIR_PLUGIN_ZLIB)infcodes$(OBJ) $(OUTDIR_PLUGIN_ZLIB)inffast$(OBJ) $(OUTDIR_PLUGIN_ZLIB)inflate$(OBJ) $(OUTDIR_PLUGIN_ZLIB)inftrees$(OBJ) $(OUTDIR_PLUGIN_ZLIB)infutil$(OBJ) $(OUTDIR_PLUGIN_ZLIB)trees$(OBJ) $(OUTDIR_PLUGIN_ZLIB)uncompr$(OBJ) $(OUTDIR_PLUGIN_ZLIB)zutil$(OBJ)

$(OUTDIR_DOCTYPES)ParaFormat$(OBJ): $(UPPDIR)DocTypes\ParaFormat.cpp \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)DocTypes\ParaFormat.cpp -Fo$(OUTDIR_DOCTYPES)ParaFormat$(OBJ)

$(OUTDIR_DOCTYPES)DocTypes$(OBJ): $(UPPDIR)DocTypes\DocTypes.cpp \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)DocTypes\DocTypes.cpp -Fo$(OUTDIR_DOCTYPES)DocTypes$(OBJ)

$(OUTDIR_DOCTYPES)DocTable$(OBJ): $(UPPDIR)DocTypes\DocTable.cpp \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)DocTypes\DocTable.cpp -Fo$(OUTDIR_DOCTYPES)DocTable$(OBJ)

$(OUTDIR_DOCTYPES)QtfParser$(OBJ): $(UPPDIR)DocTypes\QtfParser.cpp \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)DocTypes\QtfParser.cpp -Fo$(OUTDIR_DOCTYPES)QtfParser$(OBJ)

$(OUTDIR_DOCTYPES)QtfEncoder$(OBJ): $(UPPDIR)DocTypes\QtfEncoder.cpp \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)DocTypes\QtfEncoder.cpp -Fo$(OUTDIR_DOCTYPES)QtfEncoder$(OBJ)

$(OUTDIR_DOCTYPES)RtfDocOut$(OBJ): $(UPPDIR)DocTypes\RtfDocOut.cpp \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO)  /TP  $(UPPDIR)DocTypes\RtfDocOut.cpp -Fo$(OUTDIR_DOCTYPES)RtfDocOut$(OBJ)

$(OUTDIR_DOCTYPES)DocTypes.lib: $(OUTDIR_DOCTYPES)ParaFormat$(OBJ) \
	$(OUTDIR_DOCTYPES)DocTypes$(OBJ) \
	$(OUTDIR_DOCTYPES)DocTable$(OBJ) \
	$(OUTDIR_DOCTYPES)QtfParser$(OBJ) \
	$(OUTDIR_DOCTYPES)QtfEncoder$(OBJ) \
	$(OUTDIR_DOCTYPES)RtfDocOut$(OBJ)
	lib -nologo -out:$(OUTDIR_DOCTYPES)DocTypes.lib $(OUTDIR_DOCTYPES)ParaFormat$(OBJ) $(OUTDIR_DOCTYPES)DocTypes$(OBJ) $(OUTDIR_DOCTYPES)DocTable$(OBJ) $(OUTDIR_DOCTYPES)QtfParser$(OBJ) $(OUTDIR_DOCTYPES)QtfEncoder$(OBJ) $(OUTDIR_DOCTYPES)RtfDocOut$(OBJ)

$(OUTDIR_WORK)Main$(OBJ): $(UPPDIR)Work\Main.cpp \
	$(UPPDIR)CtrlLib\CtrlLib.h \
	$(UPPDIR)CtrlCore\CtrlCore.h \
	$(UPPDIR)DocTypes\DocTypes.h \
	$(UPPDIR)Draw\iml_header.h \
	$(UPPDIR)Core\lng_header.h \
	$(UPPDIR)CtrlLib\LabelBase.h \
	$(UPPDIR)CtrlLib\StaticCtrl.h \
	$(UPPDIR)CtrlLib\PushCtrl.h \
	$(UPPDIR)CtrlLib\ScrollBar.h \
	$(UPPDIR)CtrlLib\HeaderCtrl.h \
	$(UPPDIR)CtrlLib\EditCtrl.h \
	$(UPPDIR)CtrlLib\Bar.h \
	$(UPPDIR)CtrlLib\StatusBar.h \
	$(UPPDIR)CtrlLib\TabCtrl.h \
	$(UPPDIR)CtrlLib\ArrayCtrl.h \
	$(UPPDIR)CtrlLib\DropChoice.h \
	$(UPPDIR)CtrlLib\QtfCtrl.h \
	$(UPPDIR)CtrlLib\DocEdit.h \
	$(UPPDIR)CtrlLib\LineEdit.h \
	$(UPPDIR)CtrlLib\Splitter.h \
	$(UPPDIR)CtrlLib\ColumnList.h \
	$(UPPDIR)CtrlCore\lay.h \
	$(UPPDIR)CtrlLib\Progress.h \
	$(UPPDIR)CtrlLib\FileSel.h \
	$(UPPDIR)CtrlLib\CtrlUtil.h \
	$(UPPDIR)CtrlLib\Report.h \
	$(UPPDIR)CtrlLib\ctrl.iml \
	$(UPPDIR)CtrlLib\ctrl.lng \
	$(UPPDIR)CtrlLib\ctrl.lay \
	$(UPPDIR)Draw\Draw.h \
	$(UPPDIR)CtrlCore\Win32Keys.h \
	$(UPPDIR)CtrlCore\X11Keys.h \
	$(UPPDIR)CtrlCore\TopWindow.h \
	$(UPPDIR)Core\Core.h \
	$(UPPDIR)Draw\PixelArray.h \
	$(UPPDIR)Draw\Image.h \
	$(UPPDIR)Draw\DrawWin32.h \
	$(UPPDIR)Draw\Display.h \
	$(UPPDIR)Draw\ImageDraw.h \
	$(UPPDIR)Draw\Debug.h \
	$(UPPDIR)CtrlCore\MKeys.i \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\typeinfo \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\math.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\limits.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdlib.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdio.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\string.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stdarg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ctype.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\errno.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\types.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\sys\stat.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\time.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\fcntl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\memory.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\io.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wingdi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\windows.h \
	$(UPPDIR)Core\Defs.h \
	$(UPPDIR)Core\Mt.h \
	$(UPPDIR)Core\Global.h \
	$(UPPDIR)Core\Topt.h \
	$(UPPDIR)Core\String.h \
	$(UPPDIR)Core\CharSet.h \
	$(UPPDIR)Core\Path.h \
	$(UPPDIR)Core\Stream.h \
	$(UPPDIR)Core\Debug.h \
	$(UPPDIR)Core\Vcont.h \
	$(UPPDIR)Core\BiCont.h \
	$(UPPDIR)Core\Index.h \
	$(UPPDIR)Core\Map.h \
	$(UPPDIR)Core\Algo.h \
	$(UPPDIR)Core\Other.h \
	$(UPPDIR)Core\Vcont.hpp \
	$(UPPDIR)Core\Index.hpp \
	$(UPPDIR)Core\Callback.h \
	$(UPPDIR)Core\Util.h \
	$(UPPDIR)Core\TimeDate.h \
	$(UPPDIR)Core\Value.h \
	$(UPPDIR)Core\Convert.h \
	$(UPPDIR)Core\Gtypes.h \
	$(UPPDIR)Core\Color.h \
	$(UPPDIR)Core\Format.h \
	$(UPPDIR)Core\Thread.h \
	$(UPPDIR)Core\Parser.h \
	$(UPPDIR)Core\lang.h \
	$(UPPDIR)Core\ptr.h \
	$(UPPDIR)Core\App.h \
	$(UPPDIR)Core\Win32Com.h \
	$(UPPDIR)Core\Core.lng \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\exception \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winerror.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack1.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\poppack.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack4.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winresrc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\excpt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnls.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winver.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winreg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnetwk.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cderr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ddeml.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dlgs.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\lzexpand.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mmsystem.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\nb30.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\shellapi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winperf.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mswsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsock.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wincrypt.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commdlg.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winspool.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ole2.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winsvc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\mcx.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\imm.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\new \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comdef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\yvals.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cstddef \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\eh.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\basetsd.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\pshpack8.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winuser.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\commctrl.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\dde.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\winnt.rh \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\setjmp.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdce.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnterr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcasync.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\qos.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\prsht.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objbase.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleauto.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oleidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\olectl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\comutil.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\malloc.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\use_ansi.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\stddef.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcdcep.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcndr.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\wtypes.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\unknwn.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\objidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\cguid.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\urlmon.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oaidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\ocidl.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\rpcnsip.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\servprov.h \
	D:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\msxml.h
	$(CFLAGS) $(CINC) $(CMACRO) -DflagMAIN  /TP  $(UPPDIR)Work\Main.cpp -Fo$(OUTDIR_WORK)Main$(OBJ)

$(OUTDIR_WORK)Work.exe: $(OUTDIR_WORK)Main$(OBJ) \
	$(OUTDIR_CTRLLIB)CtrlLib.lib \
	$(OUTDIR_CTRLCORE)CtrlCore.lib \
	advapi32$(LIB) \
	comdlg32$(LIB) \
	comctl32$(LIB) \
	$(OUTDIR_DRAW)Draw.lib \
	user32.lib \
	gdi32.lib \
	$(OUTDIR_CORE)Core.lib \
	$(OUTDIR_PLUGIN_ZLIB)zlib.lib \
	$(OUTDIR_DOCTYPES)DocTypes.lib
	link -nologo -pdb:$(OUTDIR_WORK)Work.pdb -out:$(OUTDIR_WORK)Work.exe -machine:I386 -incremental:yes -debug -pdbtype:sept -subsystem:windows -LIBPATH:d:\mysql\lib\debug  $(OUTDIR_WORK)Main$(OBJ) $(OUTDIR_CTRLLIB)CtrlLib.lib $(OUTDIR_CTRLCORE)CtrlCore.lib advapi32.lib comdlg32.lib comctl32.lib $(OUTDIR_DRAW)Draw.lib user32.lib gdi32.lib $(OUTDIR_CORE)Core.lib $(OUTDIR_PLUGIN_ZLIB)zlib.lib $(OUTDIR_DOCTYPES)DocTypes.lib

