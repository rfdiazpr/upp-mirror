//#include "UNSCtrl.h"

#ifdef flagTEST_UNS
#include <UNSCtrl/UNSCtrl.h>

NAMESPACE_UPP

#define LLOG(x) //LOG(x)

void Ctrl::PaintGL(Rect r)                        { glrect = r;}

void Ctrl::GLStdView()
{
	glShadeModel( GL_SMOOTH );
	
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); //last 0 transparent
//	glDepthMask(false);
	glClearDepth( 1.0f );
	
//	 glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_TEXTURE_2D);
//	glDepthMask(GL_TRUE);

//	glEnable (GL_BLEND); 
//	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
//	glEnable( GL_DEPTH_TEST );
//	glDepthFunc( GL_LEQUAL );
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
	
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	return;
	
	//below copied from upp GLCtrl but it didn't work - jut black sceen
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

//	Size sz = GetSize();
	Size sz = glrect.GetSize(); //aris002

	glViewport(0, 0, (GLsizei)sz.cx, (GLsizei)sz.cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)(sz.cx)/(GLfloat)(sz.cy), 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//copied from upp GLCtrl
}

END_UPP_NAMESPACE

#endif
