#ifndef _ScatterCtrl_Demo_ScatterCtrl_Demo_h
#define _ScatterCtrl_Demo_ScatterCtrl_Demo_h

#include <ScatterCtrl/ScatterCtrl.h>

using namespace Upp;

#define LAYOUTFILE <ScatterCtrl_Demo/ScatterCtrl_Demo.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS MyImages
#define IMAGEFILE <ScatterCtrl_Demo/ScatterCtrl_Demo.iml>
#include <Draw/iml_header.h>


struct ScatterDemo : ParentCtrl {
	virtual ScatterCtrl &Scatter() = 0;
};

void RegisterExample(const char *name, ScatterDemo *ctrl);

class Tab1 : public WithTab1<ScatterDemo> {
public:
	typedef Tab1 CLASSNAME;
	Tab1();
	virtual ScatterCtrl &Scatter()	{return scatter;};

private:
	Vector<Pointf> s1;
	double s2y[5];
	double s3x[5], s3y[5];
	Vector<double> s4x, s4y;
	VectorMap<double, double> s5;
};

class Tab2 : public WithTab2<ScatterDemo> {
public:
	typedef Tab2 CLASSNAME;
	Tab2();
	virtual ScatterCtrl &Scatter()	{return scatter;};
	
	Callback back1, back2, back3;
};

class Tab3 : public WithTab3<ScatterDemo> {
public:
	typedef Tab3 CLASSNAME;
	Tab3();
	virtual ScatterCtrl &Scatter()	{return scatter;};
};

class Tab4 : public WithTab4<ScatterDemo> {
public:
	typedef Tab4 CLASSNAME;
	Tab4();
	virtual ScatterCtrl &Scatter()	{return scatter;};
	
private:
	Vector<Pointf> s1, s2;
};

class Tab5 : public WithTab5<ScatterDemo> {
public:
	typedef Tab5 CLASSNAME;
	Tab5();
	void Animate();
	void Start();
	void Stop();
	void Reset();
	void PgDown();
	void PgUp();
	void Plus();
	void Minus();
	virtual ScatterCtrl &Scatter()	{return scatter;};
	
private:
	double t;
	Vector<Pointf> s1, s2;
};

class Tab6 : public WithTab6<ScatterDemo> {
public:
	typedef Tab6 CLASSNAME;
	Tab6();
	void FormatX(String& s, int i, double d);	
	virtual ScatterCtrl &Scatter()	{return scatter;};

private:
	Vector<Pointf> s1;
};

class Tab7 : public WithTab7<ScatterDemo> {
public:
	typedef Tab7 CLASSNAME;
	
	Tab7();		
	void AddSeries();
	void RemoveFirstSeries();
	void RemoveLastSeries();
	void RemoveAllSeries();
	virtual ScatterCtrl &Scatter()	{return scatter;};

private:
	Array<Vector<Pointf> > series;
};

class Tab8 : public WithTab8<ScatterDemo> {
public:
	typedef Tab8 CLASSNAME;

	Tab8();
	void MyFormat(String& s, int i, double d)
	{
		s=Format("%Mon",fround(d)); 
		if (!s.IsEmpty())
			s << "\n2011";
	}
	void MyFormatDelta(String& s, int i, double d)
	{
		s = FormatDouble(d, 1) + " " + t_("months");
	}
	virtual ScatterCtrl &Scatter()	{return scatter;};

private:
	Vector<Pointf> s1,s2, s3;
};

class Tab9 : public WithTab9<ScatterDemo> {
public:
	typedef Tab9 CLASSNAME;
	Tab9();
	void OnFastView()		{scatter.SetFastViewX(fastView);};
	void OnSequentialX()	{scatter.SetSequentialXAll(sequentialX);};
	virtual ScatterCtrl &Scatter()	{return scatter;};

private:
	Vector<Pointf> s1,s2;
};

class ScatterCtrl_Demo : public WithScatterCtrl_Demo<TopWindow> {
	typedef ScatterCtrl_Demo CLASSNAME;
	
public:
	ScatterCtrl_Demo();
	
	void Preview();
	void SavePNG();
	void SaveJPG();
#ifdef PLATFORM_WIN32
	void SaveEMF();
#endif
	void CopyClipboard();
	void SetMode();
};

#endif

