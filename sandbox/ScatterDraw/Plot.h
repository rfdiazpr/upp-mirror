#ifndef _ScatterDraw_Brush_h_
#define _ScatterDraw_Brush_h_

using namespace Upp;

class SeriesPlot {
public:
	virtual ~SeriesPlot() 	{};	
	virtual void Paint(Draw& w, Vector<Point> &p, const int &scale, const double opacity, 
				double thick, const Color &color, String pattern, const Color &background, 
				const Color &fillColor, double fx, double fy, int y0) const {};
	virtual void Paint(Painter& w, Vector<Point> &p, const int &scale, const double opacity, 
				double thick, const Color &color, String pattern, const Color &background, 
				const Color &fillColor, double fx, double fy, int y0) const {};
	template<class T>
	static void Register(const String& name)
	{
		classMap().FindAdd(name, __Create<T>);
		typeMap().FindAdd(typeid(T).name(),name);
	}
	static void Unregister(const String& name)
	{
		int i=TypeIndex(name);
		ASSERT(i>=0);
		classMap().Remove(i);
		typeMap().Remove(i);
	}
	static String         TypeName(int i)               {return typeMap()[i];}
	static int            TypeIndex(const String& name) {return max(classMap().Find(name),0);}
	static int            GetCount()                    {return classMap().GetCount();}
	static SeriesPlot*	  Create(int i)                 {return classMap()[i]();}
	static Vector<String> GetTypes()                    {return VectorMap<String,String>(typeMap(),1).GetValues();}
	int GetType()const {return typeMap().Find(typeid(*this).name());}
	SeriesPlot* Copy()const;
	       	
protected:
	typedef SeriesPlot* (*CreateFunc)();
	template<class T>	
	static SeriesPlot*                      __Create() {return new T;}
	static VectorMap<String, CreateFunc>& classMap() {static VectorMap<String, CreateFunc> cMap; return cMap;}
	static VectorMap<String, String>&     typeMap()  {static VectorMap<String, String> tMap; 	 return tMap;}
};

class LineSeriesPlot : public SeriesPlot {
private:
	template <class T>
	void DoPaint(T& w, Vector<Point> &p, const int &scale, const double opacity, 
				double thick, const Color &color, String pattern, const Color &background, 
				const Color &fillColor, int y0) const 
	{
		if (!IsNull(fillColor)) {
			int x = p[0].x;
			p.Insert(0, Point(x, y0));
			x = p[p.GetCount()-1].x;
			p.Add(Point(x, y0));
			FillPolylineOpa(w, p, scale, opacity, background, fillColor);
			p.Remove(0);
			p.Remove(p.GetCount()-1);
		}
		DrawPolylineOpa(w, p, scale, 1, thick, color, pattern, background);		
	}
	
public:
	void Paint(Draw& w, Vector<Point> &p, const int &scale, const double opacity, 
				double thick, const Color &color, String pattern, const Color &background, 
				const Color &fillColor, double fx, double fy, int y0) const 
	{
		DoPaint(w, p, scale, opacity, fround(thick), color, pattern, background, fillColor, y0);		
	}
	void Paint(Painter& w, Vector<Point> &p, const int &scale, const double opacity, 
				double thick, const Color &color, String pattern, const Color &background, 
				const Color &fillColor, double fx, double fy, int y0) const 
	{
		DoPaint(w, p, scale, opacity, fround(thick), color, pattern, background, fillColor, y0);		
	}
};

class StaggeredSeriesPlot : public SeriesPlot {
private:
	template <class T>
	void DoPaint(T& w, Vector<Point> &p, const int &scale, const double opacity, 
				double thick, const Color &color, String pattern, const Color &background,
				const Color &fillColor, int y0) const 
	{
		ASSERT(p.GetCount() > 1);
		Vector<Point> ps;
		ps << Pointf(p[0].x - (p[1].x - p[0].x)/2., p[0].y);
		for (int i = 1; i < p.GetCount(); ++i) {
			double x = (p[i].x + p[i-1].x)/2.;
			ps << Pointf(x, p[i-1].y);
			ps << Pointf(x, p[i].y);
		}
		ps << Pointf(p[p.GetCount() - 1].x + (p[p.GetCount() - 1].x - p[p.GetCount() - 2].x)/2., p[p.GetCount() - 1].y);
		if (!IsNull(fillColor)) {
			int x = ps[0].x;
			ps.Insert(0, Point(x, y0));
			x = ps[ps.GetCount()-1].x;
			ps.Add(Point(x, y0));
			FillPolylineOpa(w, ps, scale, opacity, background, fillColor);
			ps.Remove(0);
			ps.Remove(ps.GetCount()-1);
		}
		DrawPolylineOpa(w, ps, scale, 1, fround(thick), color, pattern, background);
	}
	
public:
	void Paint(Draw& w, Vector<Point> &p, const int &scale, const double opacity, 
				double thick, const Color &color, String pattern, const Color &background, 
				const Color &fillColor, double fx, double fy, int y0) const 
	{
		DoPaint(w, p, scale, opacity, thick, color, pattern, background, fillColor, y0);
	}
	void Paint(Painter& w, Vector<Point> &p, const int &scale, const double opacity, 
				double thick, const Color &color, String pattern, const Color &background, 
				const Color &fillColor, double fx, double fy, int y0) const 
	{
		DoPaint(w, p, scale, opacity, thick, color, pattern, background, fillColor, y0);
	}
};

class BarSeriesPlot : public SeriesPlot {
private:
	template <class T>
	void DoPaint(T& w, Vector<Point> &p, const int &scale, const double opacity, 
				double thick, const Color &color, String pattern, const Color &background, 
				const Color &fillColor, double fx, int y0) const 
	{
		for (int i = 0; i < p.GetCount(); ++i) {
			FillRectangleOpa(w, p[i].x-width*fx, y0, p[i].x+width*fx, p[i].y, scale, opacity, background, fillColor);
			Vector<Point> ps;
			ps << Pointf(fround(p[i].x-width*fx), y0) << Pointf(fround(p[i].x-width*fx), p[i].y) 
			   << Pointf(fround(p[i].x+width*fx), p[i].y) << Pointf(fround(p[i].x+width*fx), y0);
			DrawPolylineOpa(w, ps, scale, 1, fround(thick), color, pattern, background);
		}
	}
	double width;
	
public:
	BarSeriesPlot(double width) : width(width) {};
	BarSeriesPlot() {};
	void Paint(Draw& w, Vector<Point> &p, const int &scale, const double opacity, 
				double thick, const Color &color, String pattern, const Color &background, 
				const Color &fillColor, double fx, double fy, int y0) const 
	{
		DoPaint(w, p, scale, opacity, thick, color, pattern, background, fillColor, fx, y0);
	}
	void Paint(Painter& w, Vector<Point> &p, const int &scale, const double opacity, 
				double thick, const Color &color, String pattern, const Color &background, 
				const Color &fillColor, double fx, double fy, int y0) const 
	{
		DoPaint(w, p, scale, opacity, thick, color, pattern, background, fillColor, fx, y0);
	}
};

class MarkPlot {
public:
	virtual ~MarkPlot() {};	
	virtual void Paint(Draw &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const {};
	virtual void Paint(Painter &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const {};
	template<class T>
	static void Register(const String& name)
	{
		classMap().FindAdd(name, __Create<T>);
		typeMap().FindAdd(typeid(T).name(),name);
	}
	static void Unregister(const String& name)
	{
		int i=TypeIndex(name);
		ASSERT(i>=0);
		classMap().Remove(i);
		typeMap().Remove(i);
	}
	static String         TypeName(int i)               {return typeMap()[i];}
	static int            TypeIndex(const String& name) {return max(classMap().Find(name),0);}
	static int            GetCount()                    {return classMap().GetCount();}
	static MarkPlot*   	  Create(int i)                 {return classMap()[i]();}
	static Vector<String> GetTypes()                    {return VectorMap<String,String>(typeMap(),1).GetValues();}
	int GetType() const {return typeMap().Find(typeid(*this).name());}
	MarkPlot* Copy()const;
	       	
protected:
	typedef MarkPlot* (*CreateFunc)();
	template<class T>	
	static MarkPlot*                      __Create() {return new T;}
	static VectorMap<String, CreateFunc>& classMap() {static VectorMap<String, CreateFunc> cMap; return cMap;}
	static VectorMap<String, String>&     typeMap()  {static VectorMap<String, String> tMap; 	 return tMap;}
};

class CircleMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const int& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		int radius = fround(scale*size);		
		int radius2 = radius/2;
		w.DrawEllipse(cp.x-radius2, cp.y-radius2, radius, radius, markColor, 1, markColor);
	}

public:
	void Paint(Draw &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const  
	{
		DoPaint(p, scale, cp, size, markColor);
	}
	void Paint(Painter &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const  
	{
		DoPaint(p, scale, cp, size, markColor);
	}
};

class SquareMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const int& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		Vector <Point> p;
		int side2l = int((size*scale)/2.);
		int side2r = int(size*scale - side2l);
		p << Point(cp.x - side2l, cp.y - side2l) << Point(cp.x + side2r, cp.y - side2l)
		  << Point(cp.x + side2r, cp.y + side2r) << Point(cp.x - side2l, cp.y + side2r)
		  << Point(cp.x - side2l, cp.y - side2l);
		w.DrawPolygon(p, markColor, scale/2, markColor); 	
	}

public:
	void Paint(Draw &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const  
	{
		DoPaint(p, scale, cp, size, markColor);
	}
	void Paint(Painter &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const  
	{
		DoPaint(p, scale, cp, size, markColor);
	}
};

class TriangleMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const int& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		Vector <Point> p;
		int xl = int((size*scale)/2.);
		int xr = int(size*scale - xl);
		int yt = int(size*scale*2/3.);
		int yb = int(size*scale - yt);
		p << Point(cp.x - xl, cp.y + yb) << Point(cp.x + xr, cp.y + yb)
		  << Point(cp.x, cp.y - yt) << Point(cp.x - xl, cp.y + yb);
		w.DrawPolygon(p, markColor, scale/2, markColor); 	
	}

public:
	void Paint(Draw &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const 
	{
		DoPaint(p, scale, cp, size, markColor);
	}
	void Paint(Painter &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const  
	{
		DoPaint(p, scale, cp, size, markColor);
	}
};

class CrossMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const int& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		int side2l = int((size*scale)/2.);
		int side2r = int(size*scale - side2l);
		w.DrawLine(cp.x - side2l, cp.y, cp.x + side2r, cp.y, scale, markColor);
		w.DrawLine(cp.x, cp.y - side2l, cp.x, cp.y + side2r, scale, markColor);
	}

public:
	void Paint(Draw &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const  
	{
		DoPaint(p, scale, cp, size, markColor);
	}
	void Paint(Painter &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const 
	{
		DoPaint(p, scale, cp, size, markColor);
	}
};	
	
class XMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const int& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		int side2l = int((size*scale)/2.);
		int side2r = int(size*scale - side2l);
		w.DrawLine(cp.x - side2l, cp.y - side2l, cp.x + side2r, cp.y + side2r, scale, markColor);
		w.DrawLine(cp.x + side2r, cp.y - side2l, cp.x - side2l, cp.y + side2r, scale, markColor);
	}

public:
	void Paint(Draw &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const  
	{
		DoPaint(p, scale, cp, size, markColor);
	}
	void Paint(Painter &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		DoPaint(p, scale, cp, size, markColor);
	}
};	
	
class RhombMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const int& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		Vector <Point> p;
		int side2l = int((size*scale)/2.);
		int side2r = int(size*scale - side2l);
		p << Point(cp.x, cp.y - side2l) << Point(cp.x + side2r, cp.y)
		  << Point(cp.x, cp.y + side2r) << Point(cp.x - side2l, cp.y)
		  << Point(cp.x, cp.y - side2l);
		w.DrawPolygon(p, markColor, scale/2, markColor); 
	}

public:
	void Paint(Draw &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const 
	{
		DoPaint(p, scale, cp, size, markColor);
	}
	void Paint(Painter &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const 
	{
		DoPaint(p, scale, cp, size, markColor);
	}
};	

#endif
