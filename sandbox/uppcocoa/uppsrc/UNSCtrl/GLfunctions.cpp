#include "UNSCtrl.h"

NAMESPACE_UPP
void uglTriangle1(Rect rect) //adapted from apple site
{
	
	//	glClearColor(0,0,0,0);  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	
//    glViewport( 0,0,[glview frame].size.width,[glview frame].size.height );  
    glViewport(rect.left, rect.top, rect.Width(),rect.Height() );  
    glMatrixMode(GL_PROJECTION);   glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);    glLoadIdentity();
//good above
// glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); glLoadIdentity(); 
// glTranslatef( -1.5f, 0.0f, -6.0f );
	glRotatef( 10.5f, 10.5f, 10.5f, 10.5f ); 
// glBegin( GL_TRIANGLES ); 
// { glColor3f( 1.0f, 0.0f, 0.0f ); glVertex3f( 0.0f, 1.0f, 0.0f ); glColor3f( 0.0f, 1.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, 1.0f ); glColor3f( 0.0f, 0.0f, 1.0f ); glVertex3f( 1.0f, -1.0f, 1.0f ); glColor3f( 1.0f, 0.0f, 0.0f ); glVertex3f( 0.0f, 1.0f, 0.0f ); glColor3f( 0.0f, 0.0f, 1.0f ); glVertex3f( 1.0f, -1.0f, 1.0f ); glColor3f( 0.0f, 1.0f, 0.0f ); glVertex3f( 1.0f, -1.0f, -1.0f ); glColor3f( 1.0f, 0.0f, 0.0f ); glVertex3f( 0.0f, 1.0f, 0.0f ); glColor3f( 0.0f, 1.0f, 0.0f ); glVertex3f( 1.0f, -1.0f, -1.0f ); glColor3f( 0.0f, 0.0f, 1.0f ); glVertex3f( -1.0f, -1.0f, -1.0f ); glColor3f( 1.0f, 0.0f, 0.0f ); glVertex3f( 0.0f, 1.0f, 0.0f ); glColor3f( 0.0f, 0.0f, 1.0f ); glVertex3f( -1.0f, -1.0f, -1.0f ); glColor3f( 0.0f, 1.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, 1.0f ); } 
// glEnd();

	glColor3f(1.0f, 0.85f, 0.35f);

	glBegin(GL_TRIANGLES);

	{
		glVertex3f(  0.0,  0.6, 0.0);
		glVertex3f( -0.2, -0.3, 0.0);
		glVertex3f(  0.2, -0.3 ,0.0);
	}
//    glTranslatef(0,0,-10);
	glRotatef(30, 31, 0, 0);
////  glRotatef(alpha, 0, 1, 0);
//
	glEnd();    

//	glColor3f(1,1,1);
//	glutWireTeapot(1);
	
//	glFlush();
//	[glcontext update];	
	
}

void uglPyramid() //copied from internet
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	 glLoadIdentity(); glTranslatef( -1.5f, 0.0f, -6.0f );
	  glRotatef( 50, 0.0f, 1.0f, 0.0f ); 
	  glBegin( GL_TRIANGLES );
	   { glColor3f( 1.0f, 0.0f, 0.0f ); glVertex3f( 0.0f, 1.0f, 0.0f );
	    glColor3f( 0.0f, 1.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, 1.0f );
	     glColor3f( 0.0f, 0.0f, 1.0f ); glVertex3f( 1.0f, -1.0f, 1.0f );
	      glColor3f( 1.0f, 0.0f, 0.0f ); glVertex3f( 0.0f, 1.0f, 0.0f );
	       glColor3f( 0.0f, 0.0f, 1.0f ); glVertex3f( 1.0f, -1.0f, 1.0f );
	        glColor3f( 0.0f, 1.0f, 0.0f ); glVertex3f( 1.0f, -1.0f, -1.0f );
	         glColor3f( 1.0f, 0.0f, 0.0f ); glVertex3f( 0.0f, 1.0f, 0.0f );
	          glColor3f( 0.0f, 1.0f, 0.0f ); glVertex3f( 1.0f, -1.0f, -1.0f );
	           glColor3f( 0.0f, 0.0f, 1.0f ); glVertex3f( -1.0f, -1.0f, -1.0f );
	            glColor3f( 1.0f, 0.0f, 0.0f ); glVertex3f( 0.0f, 1.0f, 0.0f );
	             glColor3f( 0.0f, 0.0f, 1.0f ); glVertex3f( -1.0f, -1.0f, -1.0f );
	              glColor3f( 0.0f, 1.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, 1.0f ); }
	 glEnd();
	return;

	glColor3f(1.0f, 0.85f, 0.35f);


	glBegin(GL_TRIANGLES);
	{
		glVertex3f(  0.0,  0.6, 0.0);
		glVertex3f( -0.2, -0.3, 0.0);
		glVertex3f(  0.2, -0.3 ,0.0);
	}
//    glTranslatef(0,0,-10);
	glRotatef(30, 31, 0, 0);
////  glRotatef(alpha, 0, 1, 0);
	glEnd();    	
}

void uglUppExample(Point point)  //copied from upp OpenGL example
{
	
//	double t = 13 * point.x;
	double t = 13 * 100;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	
//	t = 13 * point.y;
	t = 13 * 100;
	
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
}

void uglCube()//FIXME 
{
	static float angle = 0;
		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glTranslatef(0, 0, -10);
	glRotatef(30, 1, 0, 0);
	glRotatef(angle, 0, 1, 0);
	
	glBegin(GL_QUADS);
	
	glColor3f(0, 0, 0); glVertex3f(-1, -1, -1);
	glColor3f(0, 0, 1); glVertex3f(-1, -1,  1);
	glColor3f(0, 1, 1); glVertex3f(-1,  1,  1);
	glColor3f(0, 1, 0); glVertex3f(-1,  1, -1);
	
	glColor3f(1, 0, 0); glVertex3f( 1, -1, -1);
	glColor3f(1, 0, 1); glVertex3f( 1, -1,  1);
	glColor3f(1, 1, 1); glVertex3f( 1,  1,  1);
	glColor3f(1, 1, 0); glVertex3f( 1,  1, -1);
	
	glColor3f(0, 0, 0); glVertex3f(-1, -1, -1);
	glColor3f(0, 0, 1); glVertex3f(-1, -1,  1);
	glColor3f(1, 0, 1); glVertex3f( 1, -1,  1);
	glColor3f(1, 0, 0); glVertex3f( 1, -1, -1);
	
	glColor3f(0, 1, 0); glVertex3f(-1,  1, -1);
	glColor3f(0, 1, 1); glVertex3f(-1,  1,  1);
	glColor3f(1, 1, 1); glVertex3f( 1,  1,  1);
	glColor3f(1, 1, 0); glVertex3f( 1,  1, -1);
	
	glColor3f(0, 0, 0); glVertex3f(-1, -1, -1);
	glColor3f(0, 1, 0); glVertex3f(-1,  1, -1);
	glColor3f(1, 1, 0); glVertex3f( 1,  1, -1);
	glColor3f(1, 0, 0); glVertex3f( 1, -1, -1);
	
	glColor3f(0, 0, 1); glVertex3f(-1, -1,  1);
	glColor3f(0, 1, 1); glVertex3f(-1,  1,  1);
	glColor3f(1, 1, 1); glVertex3f( 1,  1,  1);
	glColor3f(1, 0, 1); glVertex3f( 1, -1,  1);
	
	glEnd();
	
	//rotation
	angle = angle + 0.1;
	//   glFlush();
	
}


void drawCube()  //FIXME doesn't work
{
	glPushMatrix();
	glTranslatef(-2, 2, 0); //move to topleft corner?
	glBegin(GL_QUADS);
		// face v0-v1-v2-v3
		glNormal3f(0,0,1);
		glColor3f(1,1,1);
		glVertex3f(1,1,1);
		glColor3f(1,1,0);
		glVertex3f(-1,1,1);
		glColor3f(1,0,0);
		glVertex3f(-1,-1,1);
		glColor3f(1,0,1);
		glVertex3f(1,-1,1);

		// face v0-v3-v4-v6
		glNormal3f(1,0,0);
		glColor3f(1,1,1);
		glVertex3f(1,1,1);
		glColor3f(1,0,1);
		glVertex3f(1,-1,1);
		glColor3f(0,0,1);
		glVertex3f(1,-1,-1);
		glColor3f(0,1,1);
		glVertex3f(1,1,-1);

		// face v0-v5-v6-v1
		glNormal3f(0,1,0);
		glColor3f(1,1,1);
		glVertex3f(1,1,1);
		glColor3f(0,1,1);
		glVertex3f(1,1,-1);
		glColor3f(0,1,0);
		glVertex3f(-1,1,-1);
		glColor3f(1,1,0);
		glVertex3f(-1,1,1);

		// face  v1-v6-v7-v2
		glNormal3f(-1,0,0);
		glColor3f(1,1,0);
		glVertex3f(-1,1,1);
		glColor3f(0,1,0);
		glVertex3f(-1,1,-1);
		glColor3f(0,0,0);
		glVertex3f(-1,-1,-1);
		glColor3f(1,0,0);
		glVertex3f(-1,-1,1);

		// face v7-v4-v3-v2
		glNormal3f(0,-1,0);
		glColor3f(0,0,0);
		glVertex3f(-1,-1,-1);
		glColor3f(0,0,1);
		glVertex3f(1,-1,-1);
		glColor3f(1,0,1);
		glVertex3f(1,-1,1);
		glColor3f(1,0,0);
		glVertex3f(-1,-1,1);

		// face v4-v7-v6-v5
		glNormal3f(0,0,-1);
		glColor3f(0,0,1);
		glVertex3f(1,-1,-1);
		glColor3f(0,0,0);
		glVertex3f(-1,-1,-1);
		glColor3f(0,1,0);
		glVertex3f(-1,1,-1);
		glColor3f(0,1,1);
		glVertex3f(1,1,-1);
	glEnd();

	glPopMatrix();	
}

END_UPP_NAMESPACE
