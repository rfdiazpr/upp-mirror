#ifndef _Docking_DockableCtrl_h_
#define _Docking_DockableCtrl_h_

#include "WindowFrame.h"
#include "TabBar.h"

class DockBase;
class DockableCtrl;

class DockableCtrl : public ParentCtrl
{
public:
	typedef DockableCtrl CLASSNAME;
	
	struct Style : ChStyle<Style> {
		CtrlFrame *frame;
		Value handle[2];
		bool handle_vert;
		Font title_font;
		Color title_ink[2];
		Value close[4];
		Value windowpos[4];
		Value autohide[4];
		int btnsize;
	};	
	static const Style& StyleDefault();		
	
private:
	Size			minsize;
	Size			maxsize;
	Size			stdsize;
	Image 			icon;
	WString 		title;

	const Style *	style;
public:
	virtual void WindowMenu(Bar &bar) { }

	const Image &	GetIcon()						{ return icon; }
	DockableCtrl &	Icon(const Image& m)			{ icon = m; return *this; }
	DockableCtrl &	Title(const char *_title)		{ title = _title; return *this; }
	DockableCtrl &	Title(const WString& _title)	{ title = _title; return *this; }
	const WString & GetTitle()						{ return title; }

	DockableCtrl &	SizeHint(const Size &min, const Size &max = Null, const Size &std = Null);
	void       		SetMinSize(Size sz) 			{ minsize = sz; }
	void       		SetMaxSize(Size sz) 			{ maxsize = sz; }	
	void 			SetStdSize(Size sz) 			{ stdsize = sz; }
	virtual Size	GetMinSize() const				{ return minsize; }
	virtual Size	GetMaxSize() const				{ return maxsize.IsNullInstance() ? Ctrl::GetMaxSize() : maxsize; }	
	virtual Size	GetStdSize() const				{ return stdsize.IsNullInstance() ? minsize : stdsize; }
		
	DockableCtrl &	SetStyle(Style &s)				{ style = &s; }
	const Style &	GetStyle()						{ return style ? *style : StyleDefault(); }
	
	DockableCtrl();
};

#endif

