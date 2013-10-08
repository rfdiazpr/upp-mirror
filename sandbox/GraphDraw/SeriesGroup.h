#ifndef _GraphCtrl_SeriesGroup_h_
#define _GraphCtrl_SeriesGroup_h_


namespace GraphDraw_ns
{

/*
 * Graph Series managemet class
 */
template<class TYPES, class DERIVED>
	class SeriesGroup {
	public:
		typedef SeriesGroup<TYPES, DERIVED> CLASSNAME;
		typename TYPES::TypeVectorSeries    series;
		typename TYPES::TypeCoordConverter* _currentXConverter;
		typename TYPES::TypeCoordConverter* _currentYConverter;

		bool _setDefaultStylesOnCreate;
		bool _isDataModified;

	public:

		SeriesGroup() : _currentXConverter(0), _currentYConverter(0), _setDefaultStylesOnCreate(false), _isDataModified(true) {}
		virtual ~SeriesGroup() {}

		inline void SetModifyData()   { _isDataModified = true;  }
		inline void ClearModifyData() { _isDataModified = false; }
		inline bool IsModifiedData() const { return _isDataModified;  }

		inline typename TYPES::TypeSeriesConfig& GetSeriesConfig(int id) { return series[id]; }

		inline int GetCount() 	{return series.GetCount();}
		inline bool IsEmpty()	{return series.IsEmpty();}

		DERIVED&  SetDefaultStylesSeriesAdd (bool v = true) {
			_setDefaultStylesOnCreate = v;
			return *static_cast<DERIVED*>(this);
		}

		DERIVED& SetDataColor(const int j,const Color& color){
			ASSERT(IsValid(j));
			series[j].color = color;
			_isDataModified = true;
//			static_cast<DERIVED*>(this)->Refresh();
			return *static_cast<DERIVED*>(this);
		}

		DERIVED& SetDataColor(const Color& color){
			series[series.GetCount()-1].color = color;
			_isDataModified = true;
//			static_cast<DERIVED*>(this)->Refresh();
			return *static_cast<DERIVED*>(this);
		}

		Color GetDataColor (const int j) const{
			ASSERT(IsValid(j));
			return series[j].color;
		}

		DERIVED&  SetSequential(const int j, bool v){
			ASSERT(IsValid(j));
			series[j].sequential = v;
			_isDataModified = true;
//			static_cast<DERIVED*>(this)->Refresh();
			return *static_cast<DERIVED*>(this);
		}
		DERIVED&  SetSequential(bool v){
			return SetSequential(series.GetCount()-1, v);
		}
		DERIVED&  SetSequential(){
			return SetSequential( true);
		}

		DERIVED&  SetDataThickness(const int j, const double& thickness){
			ASSERT(IsValid(j));
			series[j].thickness = thickness;
			_isDataModified = true;
//			static_cast<DERIVED*>(this)->Refresh();
			return *static_cast<DERIVED*>(this);
		}
		DERIVED& SetDataThickness(const double& thickness){
			return SetDataThickness(series.GetCount()-1, thickness);
		}

		void GetDataThickness(const int j) const{
			ASSERT(IsValid(j));
			return series[j].thickness;
		}
		void SetFillColor(const int j, const Color& color){
			ASSERT(IsValid(j));
			series[j].fillColor = color;
			_isDataModified = true;
//			static_cast<DERIVED*>(this)->Refresh();
		}

		Color GetFillColor(const int j) const{
			ASSERT(IsValid(j));
			return series[j].fillColor;
		}


		DERIVED& SetMarkWidth(const int j, const double& width){
			ASSERT(IsValid(j));
			series[j].markWidth = width;
			_isDataModified = true;
//			static_cast<DERIVED*>(this)->Refresh();
			return *static_cast<DERIVED*>(this);
		}
		DERIVED& SetMarkWidth(const double& width){
			return SetMarkWidth( series.GetCount()-1, width);
		}

		double GetMarkWidth(const int j) const{
			ASSERT(IsValid(j));
			return series[j].markWidth;
		}

		void SetMarkColor(const int j, const Color& color){
			ASSERT(IsValid(j));
			series[j].markColor = color;
			_isDataModified = true;
//			static_cast<DERIVED*>(this)->Refresh();
		}
		void SetMarkColor(const Color& color){
			ASSERT(series.GetCount()-1);
			series[series.GetCount()-1].markColor = color;
			_isDataModified = true;
//			static_cast<DERIVED*>(this)->Refresh();
		}

		::Color GetMarkColor (const int j) const{
			ASSERT(IsValid(j));
			return series[j].markColor;
		}
		void NoMark(const int j){
			ASSERT(IsValid(j));
			series[j].markWidth = 0;
		}
		bool IsShowMark(const int j) const throw (Exc){
			ASSERT(IsValid(j));
			return series[j].markWidth > 0;
		}

		DERIVED& SetXconverter(typename TYPES::TypeCoordConverter& conv) {
			series[series.GetCount() - 1].xConverter = &conv;
			return *static_cast<DERIVED*>(this);
		}

		DERIVED& SetYconverter(typename TYPES::TypeCoordConverter& conv) {
			series[series.GetCount() - 1].yConverter = &conv;
			_isDataModified = true;
			return *static_cast<DERIVED*>(this);
		}


		DERIVED& AddSeries(double *yData, int numData, double x0 = 0, double deltaX = 1)       {return AddSeries<CArray>(yData, numData, x0, deltaX); }
		DERIVED& AddSeries(double *xData, double *yData, int numData)                          {return AddSeries<CArray>(xData, yData, numData);}
		DERIVED& AddSeries(Vector<double> &xData, Vector<double> &yData)                       {return AddSeries<VectorDouble>(xData, yData);}
		DERIVED& AddSeries(Upp::Array<double> &xData, Upp::Array<double> &yData)               {return AddSeries<ArrayDouble>(xData, yData);}
		DERIVED& AddSeries(Vector<Pointf> &points)                                             {return AddSeries<VectorPointf>(points);}
		DERIVED& AddSeries(Upp::Array<Pointf> &points)                                         {return AddSeries<ArrayPointf>(points);}
		DERIVED& AddSeries(double (*function)(double))                                         {return AddSeries<FuncSource>(function);}
		DERIVED& AddSeries(Pointf (*function)(double), int np, double from = 0, double to = 1) {return AddSeries<FuncSourcePara>(function, np, from, to);}
		DERIVED& AddSeries(PlotExplicFunc &function)                                           {return AddSeries<PlotExplicFuncSource>(function);}
		DERIVED& AddSeries(PlotParamFunc function, int np, double from = 0, double to = 1)     {return AddSeries<PlotParamFuncSource>(function, np, from, to);}
		DERIVED& AddSeries(DataSource &data) {
			typename TYPES::TypeSeriesConfig &s = series.Add();
			s.Init(series.GetCount()-1, _setDefaultStylesOnCreate);
			s.SetDataSource(&data, false);
			ASSERT(_currentXConverter!=0);
			s.xConverter = _currentXConverter;
			ASSERT(_currentYConverter!=0);
			s.yConverter = _currentYConverter;
			_isDataModified = true;
//			static_cast<DERIVED*>(this)->Refresh();
			return *static_cast<DERIVED*>(this);
		}
		
		DERIVED& _AddSeries(DataSource *data) {
			typename TYPES::TypeSeriesConfig &s = series.Add();
			s.Init(series.GetCount()-1, _setDefaultStylesOnCreate);
			s.SetDataSource(data);
			ASSERT(_currentXConverter!=0);
			s.xConverter = _currentXConverter;
			ASSERT(_currentYConverter!=0);
			s.yConverter = _currentYConverter;
			_isDataModified = true;
//			static_cast<DERIVED*>(this)->Refresh();
			return *static_cast<DERIVED*>(this);
		}

		template <class C>
		DERIVED& AddSeries() 	{return _AddSeries(new C());}
		template <class C, class T1>
		DERIVED& AddSeries(T1 &arg1)                                                    {return _AddSeries(new C(arg1));}
		template <class C, class T1, class T2>
		DERIVED& AddSeries(T1 &arg1, T2 &arg2)                                          {return _AddSeries(new C(arg1, arg2));}
		template <class C, class T1, class T2, class T3>
		DERIVED& AddSeries(T1 &arg1, T2 &arg2, T3 &arg3)                                {return _AddSeries(new C(arg1, arg2, arg3));}
		template <class C, class T1, class T2, class T3, class T4>
		DERIVED& AddSeries(T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4)                      {return _AddSeries(new C(arg1, arg2, arg3, arg4));}
		template <class C, class T1, class T2, class T3, class T4, class T5>
		DERIVED& AddSeries(T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4, T5 &arg5)            {return _AddSeries(new C(arg1, arg2, arg3, arg4, arg5));}
		template <class C, class T1, class T2, class T3, class T4, class T5, class T6>
		DERIVED& AddSeries(T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4, T5 &arg5, T6 &arg6)  {return _AddSeries(new C(arg1, arg2, arg3, arg4, arg5, arg6));}

		template <class Y>
		DERIVED& AddSeries(Vector<Y> &yData)		{return _AddSeries(new VectorY<Y>(yData));}
		template <class Y>
		DERIVED& AddSeries(Upp::Array<Y> &yData)	{return _AddSeries(new ArrayY<Y>(yData));}
		template <class X, class Y>
		DERIVED& AddSeries(VectorMap<X, Y> &data)	{return _AddSeries(new VectorMapXY<X, Y>(data));}
		template <class X, class Y>
		DERIVED& AddSeries(ArrayMap<X, Y> &data)	{return _AddSeries(new ArrayMapXY<X, Y>(data));}

		void InsertSeries(int id, double *yData, int numData, double x0 = 0, double deltaX = 1)       {InsertSeries<CArray>(id, yData, numData, x0, deltaX);}
		void InsertSeries(int id, double *xData, double *yData, int numData)                          {InsertSeries<CArray>(id, xData, yData, numData);}
		void InsertSeries(int id, Vector<double> &xData, Vector<double> &yData)                       {InsertSeries<VectorDouble>(id, xData, yData);}
		void InsertSeries(int id, Upp::Array<double> &xData, Upp::Array<double> &yData)               {InsertSeries<ArrayDouble>(id, xData, yData);}
		void InsertSeries(int id, Vector<Pointf> &points)                                             {InsertSeries<VectorPointf>(id, points);}
		void InsertSeries(int id, Upp::Array<Pointf> &points)                                         {InsertSeries<ArrayPointf>(id, points);}
		void InsertSeries(int id, double (*function)(double))                                         {InsertSeries<FuncSource>(id, function);}
		void InsertSeries(int id, Pointf (*function)(double), int np, double from = 0, double to = 1) {InsertSeries<FuncSourcePara>(id, function, np, from, to);}
		void InsertSeries(int id, PlotExplicFunc &function)                                           {InsertSeries<PlotExplicFuncSource>(id, function);}
		void InsertSeries(int id, PlotParamFunc function, int np, double from = 0, double to = 1)     {InsertSeries<PlotParamFuncSource>(id, function, np, from, to);}
		void _InsertSeries(int id, DataSource *data) {
			ASSERT(IsValid(id));
			typename TYPES::TypeSeriesConfig &s = series.Insert(id);
			s.Init(id, _setDefaultStylesOnCreate);
			s.SetDataSource(data);
			ASSERT(_currentXConverter!=0);
			s.xConverter = _currentXConverter;
			ASSERT(_currentYConverter!=0);
			s.yConverter = _currentYConverter;
			_isDataModified = true;
//			static_cast<DERIVED*>(this)->Refresh();
		}


		template <class C>
		void InsertSeries(int id) 		{_InsertSeries(id, new C());}
		template <class C, class T1>
		void InsertSeries(int id, T1 &arg1)
										{_InsertSeries(id, new C(arg1));}
		template <class C, class T1, class T2>
		void InsertSeries(int id, T1 &arg1, T2 &arg2)
										{_InsertSeries(id, new C(arg1, arg2));}
		template <class C, class T1, class T2, class T3>
		void InsertSeries(int id, T1 &arg1, T2 &arg2, T3 &arg3)
										{_InsertSeries(id, new C(arg1, arg2, arg3));}
		template <class C, class T1, class T2, class T3, class T4>
		void InsertSeries(int id, T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4)
										{_InsertSeries(id, new C(arg1, arg2, arg3, arg4));}
		template <class C, class T1, class T2, class T3, class T4, class T5>
		void InsertSeries(int id, T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4, T5 &arg5)
										{_InsertSeries(id, new C(arg1, arg2, arg3, arg4, arg5));}
		template <class C, class T1, class T2, class T3, class T4, class T5, class T6>
		void InsertSeries(int id, T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4, T5 &arg5, T6 &arg6)
										{_InsertSeries(id, new C(arg1, arg2, arg3, arg4, arg5, arg6));}

		template <class Y>
		void InsertSeries(int id, Vector<Y> &yData)		{_InsertSeries(id, new VectorY<Y>(yData));}
		template <class Y>
		void InsertSeries(int id, Upp::Array<Y> &yData)	{_InsertSeries(id, new ArrayY<Y>(yData));}
		template <class X, class Y>
		void InsertSeries(int id, VectorMap<X, Y> &data){_InsertSeries(id, new VectorMapXY<X, Y>(data));}
		template <class X, class Y>
		void InsertSeries(int id, ArrayMap<X, Y> &data)	{_InsertSeries(id, new ArrayMapXY<X, Y>(data));}


		void RemoveSeries(const int j){
			ASSERT(IsValid(j));
			series.Remove(j);
			_isDataModified = true;
//			static_cast<DERIVED*>(this)->Refresh();
		}

		void RemoveAllSeries(){
			series.Clear();
			_isDataModified = true;
//			static_cast<DERIVED*>(this)->Refresh();
		}




		DERIVED& PlotStyle()                             {return PlotStyle(0);};
		template <class C>
		DERIVED& PlotStyle()                             {return PlotStyle(new C());};
		template <class C, class T1>
		DERIVED& PlotStyle(T1 arg1)                     {return PlotStyle(new C(arg1));};
		template <class C, class T1, class T2>
		DERIVED& PlotStyle(T1 &arg1, T2 &arg2)           {return PlotStyle(new C(arg1, arg2));};
		template <class C, class T1, class T2, class T3>
		DERIVED& PlotStyle(T1 &arg1, T2 &arg2, T3 &arg3) {return PlotStyle(new C(arg1, arg2, arg3));};
		DERIVED& NoPlot()	{return PlotStyle();};
		DERIVED& PlotStyle(SeriesPlot *data) {
			int id = series.GetCount() - 1;
			series[id].seriesPlot = data;
			_isDataModified = true;
			return *static_cast<DERIVED*>(this);
		}

		DERIVED& MarkStyle()                             {return MarkStyle(0);};
		template <class C>
		DERIVED& MarkStyle()                             {return MarkStyle(new C());};
		template <class C, class T1>
		DERIVED& MarkStyle(T1 &arg1)                     {return MarkStyle(new C(arg1));};
		template <class C, class T1, class T2>
		DERIVED& MarkStyle(T1 &arg1, T2 &arg2)           {return MarkStyle(new C(arg1, arg2));};
		template <class C, class T1, class T2, class T3>
		DERIVED& MarkStyle(T1 &arg1, T2 &arg2, T3 &arg3) {return MarkStyle(new C(arg1, arg2, arg3));};
		DERIVED& NoMark()	{return MarkStyle();};
		DERIVED& MarkStyle(MarkPlot *data) {
			int id = series.GetCount() - 1;
			series[id].markPlot = data;
			_isDataModified = true;
			return *static_cast<DERIVED*>(this);
		}

		DERIVED& Stroke(double thickness = 3, Color color = Null) {
			int id = series.GetCount() - 1;
			if (IsNull(color))
				color = GetNewColor(id);
			series[id].color = color;
			series[id].thickness = thickness;
			//series[id].dash = GetNewDash(id);

			_isDataModified = true;
//			static_cast<DERIVED*>(this)->Refresh();
			return *static_cast<DERIVED*>(this);
		}
		DERIVED& Dash(const char *dash) {
			int id = series.GetCount() - 1;
			series[id].dash = dash;
			_isDataModified = true;
//			static_cast<DERIVED*>(this)->Refresh();
			return *static_cast<DERIVED*>(this);
		}

		DERIVED& Fill(Color color = Null) {
			int id = series.GetCount() - 1;
			if (IsNull(color)) {
				color = GetNewColor(id);
				color = Color(min(color.GetR()+30, 255), min(color.GetG()+30, 255), min(color.GetB()+30, 255));
			}
			series[id].fillColor = color;
			_isDataModified = true;
//			static_cast<DERIVED*>(this)->Refresh();
			return *static_cast<DERIVED*>(this);
		}

		DERIVED& MarkColor(Color color = Null) {
			int id = series.GetCount() - 1;
			if (IsNull(color)) {
				color = GetNewColor(id);
				color = Color(max(color.GetR()-30, 0), max(color.GetG()-30, 0), max(color.GetB()-30, 0));
			}
			series[id].markColor = color;
//			static_cast<DERIVED*>(this)->Refresh();
			_isDataModified = true;
			return *static_cast<DERIVED*>(this);
		}
		
		DERIVED& MarkWidth(const double& markWidth = 8) {
			int id = series.GetCount() - 1;
			series[id].markWidth = markWidth;
			_isDataModified = true;
//			static_cast<DERIVED*>(this)->Refresh();
			return *static_cast<DERIVED*>(this);
		}
		DERIVED& Hide() {
			series[series.GetCount() - 1].show = false;
			_isDataModified = true;
			return *static_cast<DERIVED*>(this);
		}

		DERIVED& Opacity(double opacity = 1) {
			series[series.GetCount() - 1].opacity = opacity;
			_isDataModified = true;
			return *static_cast<DERIVED*>(this);
		}

		DERIVED& Legend(const String legend) {
			int id = series.GetCount() - 1;
			series[id].legend = legend;
			_isDataModified = true;
			return *static_cast<DERIVED*>(this);
		}


		DERIVED& Id(int id)
		{
			return Id(series.GetCount()-1, id);
		}

		DERIVED& Id(const int j, int id)
		{
			ASSERT(IsValid(j));
			series[j].id = id;
			return *static_cast<DERIVED*>(this);
		}

		int GetId(const int j)
		{
			ASSERT(IsValid(j));
			return series[j].id;
		}

		void Show(const int j, const bool& show) {
			ASSERT(IsValid(j));
			series[j].show = show;
			_isDataModified = true;
//			static_cast<DERIVED*>(this)->Refresh();
		}

		bool IsVisible(const int j) {
			ASSERT(IsValid(j));
			return series[j].show;
		}

		DERIVED &ShowAll(const bool& show) {
			for (int i = 0; i < series.GetCount(); ++i)
				series[i].show = true;
			_isDataModified = true;
			return *static_cast<DERIVED*>(this);
		}

		inline bool IsValid(const int j) const {return (j >= 0 && j < series.GetCount());}
};
};
#endif
