#ifndef _ScatterDraw_DataSource_h_
#define _ScatterDraw_DataSource_h_

//#include <Core/Core.h>

//using namespace Upp;

class DataSource {
public:
	DataSource() : isParam(false) {}
	virtual ~DataSource() {};	
	virtual double y(int id) 	{return Null;};
	virtual double x(int id) 	{return Null;};
	virtual int GetCount()		{return Null;};
	bool IsParam()				{return isParam;};
	virtual double MinY() {
		double minVal = -DOUBLE_NULL;
		for (int i = 0; i < GetCount(); ++i)
			if (minVal > y(i))
				minVal = y(i);
		return minVal;
	}
	virtual double MinX() {
		double minVal = -DOUBLE_NULL;
		for (int i = 0; i < GetCount(); ++i)
			if (minVal > x(i))
				minVal = x(i);
		return minVal;
	}
	virtual double MaxY() {
		double maxVal = DOUBLE_NULL;
		for (int i = 0; i < GetCount(); ++i)
			if (maxVal < y(i))
				maxVal = y(i);
		return maxVal;
	}	
	virtual double MaxX() {
		double maxVal = DOUBLE_NULL;
		for (int i = 0; i < GetCount(); ++i)
			if (maxVal < x(i))
				maxVal = x(i);
		return maxVal;
	}	
	virtual double AvgY() {
		double ret = 0;
		for (int i = 0; i < GetCount(); ++i)
			ret += y(i);
		return ret/GetCount();
	}
	virtual double AvgX() {
		double ret = 0;
		for (int i = 0; i < GetCount(); ++i)
			ret += x(i);
		return ret/GetCount();
	}
protected:
	bool isParam;
};

class CArrayY : public DataSource {
private:
	double *yData;
	int numData;
	double x0, deltaX;
	
public:
	CArrayY(double *yData, int numData, double x0, double deltaX) : yData(yData), numData(numData), x0(x0), deltaX(deltaX) {};
	virtual inline double y(int id) 	{return yData[id];};
	virtual inline double x(int id) 	{return id*deltaX + x0;};
	virtual inline int GetCount()		{return numData;};
};

class CArrayXY : public DataSource {
private:
	double *xData, *yData;
	int numData;

public:
	CArrayXY(double *xData, double *yData, int numData) : xData(xData), yData(yData), numData(numData) {};
	virtual inline double y(int id)	{return yData[id];};
	virtual inline double x(int id) {return xData[id];};
	virtual inline int GetCount()	{return numData;};
};

template <class Y>
class VectorY : public DataSource {
private:
	Vector<Y> *yData;

public:
	VectorY(Vector<Y> &yData) : yData(&yData) {};
	virtual inline double y(int id)	{return (*yData)[id];};
	virtual inline double x(int id) {return id;};
	virtual inline int GetCount()	{return yData->GetCount();};
};	

template <class Y>
class ArrayY : public DataSource {
private:
	Upp::Array<Y> *yData;

public:
	ArrayY(Upp::Array<Y> &yData) : yData(&yData) {};
	virtual inline double y(int id)	{return (*yData)[id];};
	virtual inline double x(int id) {return id;};
	virtual inline int GetCount()	{return yData->GetCount();};
};

class VectorDouble : public DataSource {
private:
	const Vector<double> *xData, *yData;

public:
	VectorDouble(const Vector<double> &xData, Vector<double> &yData) : xData(&xData), yData(&yData) {};
	virtual inline double y(int id)	{return (*yData)[id];};
	virtual inline double x(int id) {return (*xData)[id];};
	virtual inline int GetCount()	{return xData->GetCount();};
};

class ArrayDouble : public DataSource {
private:
	const Upp::Array<double> *xData, *yData;

public:
	ArrayDouble(const Upp::Array<double> &xData, Upp::Array<double> &yData) : xData(&xData), yData(&yData) {};
	virtual inline double y(int id)	{return (*yData)[id];};
	virtual inline double x(int id) {return (*xData)[id];};
	virtual inline int GetCount()	{return xData->GetCount();};
};

class VectorPointf : public DataSource {
private:
	const Vector<Pointf> *data;

public:
	VectorPointf(const Vector<Pointf> &data) : data(&data) 	{};
	VectorPointf(Vector<Pointf> *data) : data(data) 		{};
	virtual inline double y(int id)	{return (*data)[id].y;};
	virtual inline double x(int id) {return (*data)[id].x;};
	virtual inline int GetCount()	{return data->GetCount();};
};	

class ArrayPointf : public DataSource {
private:
	Upp::Array<Pointf> *data;

public:
	ArrayPointf(Upp::Array<Pointf> &data) : data(&data) {};
	virtual inline double y(int id)	{return (*data)[id].y;};
	virtual inline double x(int id) {return (*data)[id].x;};
	virtual inline int GetCount()	{return data->GetCount();};
};	

template <class X, class Y>
class VectorMapXY : public DataSource {
private:
	VectorMap<X, Y> *data;

public:
	VectorMapXY(VectorMap<X, Y> &data) : data(&data) {};
	virtual inline double y(int id)	{return (*data)[id];};
	virtual inline double x(int id) {return (*data).GetKey(id);};
	virtual inline int GetCount()	{return data->GetCount();};
};	

template <class X, class Y>
class ArrayMapXY : public DataSource {
private:
	ArrayMap<X, Y> *data;

public:
	ArrayMapXY(ArrayMap<X, Y> &data) : data(&data) {};
	virtual inline double y(int id)	{return (*data)[id];};
	virtual inline double x(int id) {return (*data).GetKey(id);};
	virtual inline int GetCount()	{return data->GetCount();};
};		

class FuncSource : public DataSource {
private:
	double (*function)(double);

public:
	FuncSource(double (*function)(double)) : function(function) {};
	virtual inline double y(int t)	{return function(t);};
	virtual inline double x(int t) 	{return t;};
	virtual inline int GetCount()	{return Null;};
};

class FuncSourcePara : public DataSource {
private:
	Pointf (*function)(double);
	Pointf lastPointf;
	double lastT;
	int numPoints;
	double minT, maxT;
	
public:
	FuncSourcePara(Pointf (*function)(double), int np, double from, double to) : 
									function(function), numPoints(np), minT(from), maxT(to) {
		lastT = Null;
		isParam = true;
	}
	virtual inline double y(int t) {
		if (IsNull(lastT) || t != lastT) {
			lastPointf = function(minT + t*(maxT-minT)/numPoints);
			lastT = t;
		}
		return lastPointf.y;
	}
	virtual inline double x(int t) {
		if (IsNull(lastT) || t != lastT) {
			lastPointf = function(minT + t*(maxT-minT)/numPoints);
			lastT = t;
		}
		return lastPointf.x;
	}
	virtual inline int GetCount()	{return numPoints;};
};	

typedef Callback2<double&, double> PlotFunc; 
typedef Callback2<Pointf&, double> PlotParamFunc;

class PlotFuncSource : public DataSource {
private:
	PlotFunc function;

public:
	PlotFuncSource() 										{};
	PlotFuncSource(PlotFunc &function) : function(function) {};
	virtual inline double y(int t)	{double y; function(y, t); return y;};
	virtual inline double x(int t) 	{return t;};
	virtual inline int GetCount()	{return Null;};
};	

class PlotParamFuncSource : public DataSource {
private:
	PlotParamFunc function;
	Pointf lastPointf;
	double lastT;
	int numPoints;
	double minT, maxT;

public:
	PlotParamFuncSource(PlotParamFunc function, int np, double from, double to) : 
									function(function), numPoints(np), minT(from), maxT(to) {
		lastT = Null;
		isParam = true;
	}
	virtual inline double y(int t) {
		if (IsNull(lastT) || t != lastT) {
			function(lastPointf, minT + t*(maxT-minT)/numPoints);
			lastT = t;
		}
		return lastPointf.y;
	}
	virtual inline double x(int t) {
		if (IsNull(lastT) || t != lastT) {
			function(lastPointf, minT + t*(maxT-minT)/numPoints);
			lastT = t;
		}
		return lastPointf.x;
	}
	virtual inline int GetCount()	{return numPoints;};
};	

#endif
