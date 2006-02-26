#include <CtrlLib/CtrlLib.h>

struct App : TopWindow {
	ArrayCtrl  log;
	
	void Log(const String& s)
	{
//		if(!IsShutdown()) { // to prevent "ChildLostFocus" Log caused by destroying log itself
			log.Add(s);
			log.GoEnd();
//		}
	}

	virtual void   Paint(Draw& w)
	{
		Size sz = GetSize();
		w.DrawRect(sz, SLtGray);
		Log("Paint");
	}

	virtual void   Activate()
	{
		Log("Activate");
	}
	
	virtual void   Deactivate()
	{
		Log("Deactivate");
	}
	
	virtual Image  FrameMouseEvent(int event, Point p, int zdelta, dword keyflags)
	{
		if(event != CURSORIMAGE)
			Log(Format("FrameMouseEvent(%d, Point(%d, %d), %d, %x)",
			           event, p.x, p.y, zdelta, (int)keyflags));
		return TopWindow::FrameMouseEvent(event, p, zdelta, keyflags);
	}
	
	virtual Image  MouseEvent(int event, Point p, int zdelta, dword keyflags)
	{
		if(event != CURSORIMAGE)
			Log(Format("MouseEvent(%d, Point(%d, %d), %d, %x)",
			           event, p.x, p.y, zdelta, (int)keyflags));
		return TopWindow::MouseEvent(event, p, zdelta, keyflags);
	}

	virtual void   MouseEnter(Point p, dword keyflags)
	{
		Log(Format("MouseEnter(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   MouseMove(Point p, dword keyflags)
	{
		Log(Format("MouseMove(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   LeftDown(Point p, dword keyflags)
	{
		Log(Format("LeftDown(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
		SetFocus();
	}

	virtual void   LeftDouble(Point p, dword keyflags)
	{
		Log(Format("LeftDouble(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   LeftRepeat(Point p, dword keyflags)
	{
		Log(Format("LeftRepeat(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   LeftUp(Point p, dword keyflags)
	{
		Log(Format("LeftUp(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   RightDown(Point p, dword keyflags)
	{
		Log(Format("RightDown(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   RightDouble(Point p, dword keyflags)
	{
		Log(Format("RightDouble(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   RightRepeat(Point p, dword keyflags)
	{
		Log(Format("RightRepeat(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   RightUp(Point p, dword keyflags)
	{
		Log(Format("RightUp(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   MouseWheel(Point p, int zdelta, dword keyflags)
	{
		Log(Format("MouseWheel(Point(%d, %d), %d, %x)", p.x, p.y, zdelta, (int)keyflags));
	}

	virtual void   MouseLeave()
	{
		Log("MouseLeave");
	}

	virtual Image  CursorImage(Point p, dword keyflags)
	{
		return keyflags & K_CTRL ? CtrlImg::swap_color_cursor() : Image::Arrow();
	}

	virtual bool   Key(dword key, int count)
	{
		Log(Format("Key(%x, %d) ", (int)key, count) + GetKeyDesc(key));
		return false;
	}

	virtual bool   WKey(dword key, int count)
	{
		Log(Format("WKey(%x, %d) ", (int)key, count) + GetKeyDesc(key));
		return false;
	}

	virtual void   GotFocus()
	{
		Log("GotFocus");
	}

	virtual void   LostFocus()
	{
		Log("LostFocus");
	}

	virtual bool   HotKey(dword key)
	{
		Log(Format("HotKey(%x) ", (int)key) + GetKeyDesc(key));
		return false;
	}

	virtual void   ChildGotFocus()
	{
		Log("ChildGotFocus");
	}

	virtual void   ChildLostFocus()
	{
		Log("ChildLostFocus");
	}
	
	virtual void   Layout()
	{
		Log("Layout");
	}

	App()
	{
		SetFrame(InsetFrame());
		AddFrame(InsetFrame());
		AddFrame(InsetFrame());
		AddFrame(InsetFrame());
		log.AddColumn("");
		log.NoHeader();
		Add(log.HSizePos().BottomPos(0, 200));
	}
	
	~App()
	{
		Shutdown();
	}
};

GUI_APP_MAIN
{
	App().Run();
}
