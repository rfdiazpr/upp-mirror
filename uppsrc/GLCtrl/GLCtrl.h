#ifndef _GLCtrl_GLCtrl_h
#define _GLCtrl_GLCtrl_h

#include <CtrlLib/CtrlLib.h>

#include <GL/gl.h>
#include <GL/glu.h>

class GLCtrl : public DHCtrl {
public:
	virtual void    State(int reason);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	HDC   hDC;
	HGLRC hRC;
	
	void OpenGL();
	void CloseGL();

public:
	virtual void GLPaint();

	Callback WhenGLPaint;

	void    StdView();
	
	HDC     GetDC() const            { return hDC; }
	HGLRC   GetHGLRC() const         { return hRC; }
	
	GLCtrl();
	~GLCtrl();
};


#endif

