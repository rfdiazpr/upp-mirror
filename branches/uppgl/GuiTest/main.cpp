#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>
#include <DropGrid/DropGrid.h>
#include <Skulpture/Skulpture.h>
#include <plugin/glew/glew.h>

#define IMAGECLASS Img
#define IMAGEFILE <GuiTest/main.iml>
#include <Draw/iml.h>

using namespace Upp;

struct MyButton : Button
{
	float diff;
	int dir;
	typedef MyButton CLASSNAME;
	
	MyButton()
	{
		diff = 1.f;
		dir = 0;
	}
	
	virtual void ApplyTransform(TransformState state)
	{
		if(state == TS_BEFORE_CTRL_PAINT)
		{
			Rect r = GetSize();
			GetTopRect(r, InFrame());
			
			float dx = (float) (r.left + r.right) / 2.f;
			float dy = (float) (r.top + r.bottom) / 2.f;
		
			glTranslatef(dx, dy, 0);
			glScalef(diff, diff, 0);
			glTranslatef(-dx, -dy, 0);
		}
	}

	virtual void Paint(Draw& w)
	{
		ChangeSize();
		Button::Paint(w);
		Size sz = GetSize();
		Size isz = sz - 5;

		w.DrawImage((sz.cx - isz.cx) / 2, (sz.cy - isz.cy) / 2, isz.cx, isz.cy, Img::ChromeIcon());	
	}
		
	void ChangeSize()
	{
		if(dir == 0)
			return;
		
		diff += dir / 30.f;
		if(diff > 2.12f)
		{
			diff = 2.12f;
			dir = 0;
		}
		else if(diff < 1.f)
		{
			diff = 1.f;
			dir = 0;
		}
		
	}
	
	virtual void MouseEnter(Point p, dword keys)
	{
		dir = 1;	
	}
	
	virtual void MouseLeave()
	{
		dir = -1;
	}
};

#define LAYOUTFILE <GuiTest/main.lay>
#include <CtrlCore/lay.h>

struct App : TopWindow
{
	Point point;

	Splitter spl;
	WithTopLayout<ParentCtrl> top;
	WithBottomLayout<ParentCtrl> bottom;
	WithTab0Layout<ParentCtrl> tab0;
	WithTab1Layout<ParentCtrl> tab1;
	MenuBar menubar;
	ToolBar toolbar;
	Button b;
	
	typedef App CLASSNAME;
	App()
	{
		CtrlLayout(top);
		CtrlLayout(bottom);
		AddFrame(menubar);
		AddFrame(TopSeparatorFrame());
		//AddFrame(toolbar);
		menubar.Set(THISBACK(SetMenu));
		toolbar.Set(THISBACK(SetTool));
		Add(spl.SizePos());
		spl.Vert(top, bottom);
		
		top.grid.AddColumn("Column 0");
		top.grid.AddColumn("Column 1");
		top.grid.AddColumn("Column 2");
		
		top.b8.SetImage(Img::ChromeIcon());
		
		for(int i = 0; i < 25; i++)
			top.grid.Add(Format("Value %d", i), Format("Longer text %d", i), Format("%.4d", i + 1000));
		
		for(int i = 0; i < 50; i++)
		{
			tab0.list.Add(Format("Drop list item %d", i));
			tab0.list2.Add(Format("Drop grid item %d", i));
		}
		
		CtrlLayout(tab0);
		CtrlLayout(tab1);
		
		tab0.slider.SetData(50);
		
		top.tab.Add(tab0.SizePos(), "First tab");
		top.tab.Add(tab1.SizePos(), "Second tab");
		top.tab.Add("Third tab");
		top.tab.Add("Fourth tab");

		bottom.array.AddColumn("Column 0");
		bottom.array.AddColumn("Column 1");
		bottom.array.AddColumn("Column 2");
		bottom.array.SetCount(100);
		bottom.progress.Set(50, 100);

		point.x = 50;
		point.y = 50;
		Sizeable().Zoomable();
		SetTimeCallback(-10, THISBACK(Repaint));
	}
	
	void Nothing()
	{
	}
	
	void SetMenu(Bar& bar)
	{
		bar.Add("Menu item 0", THISBACK(SubMenu));
		bar.Add("Menu item 1", THISBACK(SubMenu));
		bar.Add("Menu item 2", THISBACK(SubMenu));
		bar.Add("Menu item 3", THISBACK(Nothing));
	}
	
	void SubMenu(Bar& bar)
	{
		bar.Add("Menu item 0", THISBACK(SubMenu2));
		bar.Add("Menu item 1", THISBACK(Nothing));
		bar.Add("Menu item 2", THISBACK(Nothing));
		bar.Add("Menu item 3", THISBACK(Nothing));
		bar.Add("Menu item 0", THISBACK(SubMenu2));
		bar.Add("Menu item 1", THISBACK(Nothing));
		bar.Add("Menu item 2", THISBACK(Nothing));
		bar.Add("Menu item 3", THISBACK(Nothing));
	}
	
	void SubMenu2(Bar& bar)
	{
		bar.Add("Menu item 0", THISBACK(Nothing));
		bar.Add("Menu item 1", THISBACK(Nothing));
		bar.Add("Menu item 2", THISBACK(Nothing));
		bar.Add("Menu item 3", THISBACK(Nothing));
	}
	
	void SetTool(Bar& bar)
	{
	}
	
	virtual void Paint(Draw& w)
	{
		Size sz = GetSize();
		
		glShadeModel(GL_SMOOTH);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (GLfloat)(sz.cx)/(GLfloat)(sz.cy), 1.0f, 100.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		double t = 13 * point.x;
		
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glDisable(GL_SCISSOR_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		
		glLoadIdentity();
		glPushMatrix();
			glTranslatef(0, 0, -6);
			glRotated(t / 30.0, 0, 1, 0);
			glRotated(t / 40.0, 1, 0, 0);
			glRotated(t / 5.0, 0, 0, 1);
			glBegin(GL_TRIANGLE_FAN);
				glColor4d(0.8, 0.4, 0.2, 1);
				glVertex3f(0.0f, 0.0f, 0.0f);
				for(int i = 0; i < 50; i++) {
					double u = M_2PI * i / 49;
					glColor4d(i / 50.0, 0.5 + i / 100.0, 0.7 - i / 150.0, 1);
					glVertex3d(sin(u), cos(u), 1.0f);
				}
			glEnd();
			glBegin(GL_TRIANGLE_FAN);
				glColor4d(0.8, 0.4, 0.2, 1);
				glVertex3f(0.0f, 0.0f, 3.0f);
				for(int i = 0; i < 50; i++) {
					double u = M_2PI * i / 49;
					glColor4d(0.7, 0.5 - i / 100.0, 0.7 + i / 150.0, 1);
					glVertex3d(sin(u), cos(u), 1.0f);
				}
			glEnd();
		glPopMatrix();
		t = 13 * point.y;
		float sn = (float)sin(t / 300);
		float cs = (float)cos(t / 300);
		glTranslatef(sn + cs, sn, -6 - cs);
		glRotated(t / 12.0, 0, 0, 1);
		glRotated(t / 15.0, 0, 1, 0);
		glRotated(t / 17.0, 1, 0, 0);
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glColor4d(1, 0, 0, 0.5);
			glVertex3d(-1, 0, 0);
			glVertex3d(1, 0, 0);
			glVertex3d(0, 0.87, 0);
			glColor4d(0, 1, 0, 0.5);
			glVertex3d(0, 0, 1.67);
		glEnd();

		++point.x;

		glEnable(GL_SCISSOR_TEST);
		((OpenGLDraw&) w).FlatView();
	}
};

GUI_APP_MAIN
{
	//Skulpture sk;	
	//ChHostSkin();
	//ChClassicSkin();
	ChStdSkin();
	App().Run();
}
