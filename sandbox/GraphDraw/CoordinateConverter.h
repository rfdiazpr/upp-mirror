#ifndef _GraphCtrl_CoordinateConverter_h_
#define _GraphCtrl_CoordinateConverter_h_


namespace GraphDraw_ns
{
	// ============================
	//    CoordinateConverter   CLASS
	// ============================
//	template <class SCREEN_COORD = Upp::int32, class GRAPH_COORD = double>
	template <class SCREEN_COORD, class GRAPH_COORD>
	class CoordinateConverter
	{
		public:
			typedef SCREEN_COORD TypeScreenCoord;
			typedef GRAPH_COORD  TypeGraphCoord;
			
		protected:
			long double _graphToScreenScale;
			long double _graphMin;
			long double _graphMax;
			long double _graphRange;
			long double _screenToGraphScale;
			long double _screenMin;
			long double _screenMax;
			long double _screenRange;

		public:
			CoordinateConverter()
			{
				update( 0, 100, 0, 100);
			}

			CoordinateConverter(GRAPH_COORD graphMin, GRAPH_COORD graphMax, SCREEN_COORD screenMin, SCREEN_COORD screenMax)
			{
				update( graphMin, graphMax, screenMin, screenMax);
			}

			inline SCREEN_COORD getScreenMin()        { return _screenMin; }
			inline SCREEN_COORD getScreenMax()        { return _screenMax; }
			inline SCREEN_COORD getScreenRange()      { return _screenRange; }
			inline GRAPH_COORD  getGraphMin()         { return _graphMin; }
			inline GRAPH_COORD  getGraphMax()         { return _graphMax; }
			inline GRAPH_COORD  getGraphRange()       { return _graphRange; }
			inline GRAPH_COORD  getSignedGraphRange() { return (_graphMax-_graphMin); }
			inline bool IsInGraphRange(TypeGraphCoord p) { return ((_graphMin <= p) && (p <= _graphMax)); }

			inline void Update() {
				update( getGraphMin(), getGraphMax(), getScreenMin(), getScreenMax() );
			}

			virtual void update(GRAPH_COORD graphMin, GRAPH_COORD graphMax, SCREEN_COORD screenMin, SCREEN_COORD screenMax)
			{
				//TRACE_INFO("CoordinateConverter.update(" << graphMin << ", " << graphMax  << ", " << screenMin<< ", " << screenMax << " )" );
				_graphMin = graphMin;
				_graphMax = graphMax;
				_screenMin = screenMin;
				_screenMax = screenMax;
				_graphRange =  fabs(_graphMax - _graphMin);
				_screenRange = fabs(_screenMax - _screenMin);
				_graphToScreenScale = (_screenMax - _screenMin) / (_graphMax - _graphMin);
				_screenToGraphScale = (_graphMax - _graphMin) / (_screenMax - _screenMin);
			}

			virtual void updateScreenSize(SCREEN_COORD screenMin, SCREEN_COORD screenMax)
			{
				//TRACE_INFO("CoordinateConverter.update(" << graphMin << ", " << graphMax  << ", " << screenMin<< ", " << screenMax << " )" );
				_screenMin = screenMin;
				_screenMax = screenMax;
				_screenRange = fabs(_screenMax - _screenMin);
				_graphToScreenScale = (_screenMax - _screenMin) / (_graphMax - _graphMin);
				_screenToGraphScale = (_graphMax - _graphMin) / (_screenMax - _screenMin);
			}

			virtual void updateGraphSize(GRAPH_COORD graphMin, GRAPH_COORD graphMax)
			{
				//TRACE_INFO("CoordinateConverter.update(" << graphMin << ", " << graphMax  << ", " << screenMin<< ", " << screenMax << " )" );
				_graphMin = graphMin;
				_graphMax = graphMax;
				_graphRange =  fabs(_graphMax - _graphMin);
				_graphToScreenScale = (_screenMax - _screenMin) / (_graphMax - _graphMin);
				_screenToGraphScale = (_graphMax - _graphMin) / (_screenMax - _screenMin);
			}

			virtual SCREEN_COORD toScreen(GRAPH_COORD v)  { return  ((v-_graphMin)*_graphToScreenScale + _screenMin); }
			virtual GRAPH_COORD  toGraph(SCREEN_COORD v)  { return  ((v-_screenMin)*_screenToGraphScale + _graphMin); }
			virtual const char* GetConversionType() { return "STD"; }
	};

	template<class STREAM, class SCREEN_COORD, class GRAPH_COORD>
	STREAM& operator<<(STREAM& str, CoordinateConverter<SCREEN_COORD, GRAPH_COORD> v)
	{
		str << "\n CoordinateConverter :   " << v.GetConversionType();
		str << "\n     - G[" << v.getGraphMin() << ", " << v.getGraphMax() << "] ==> range = " << v.getGraphRange();
		str << "\n     - S[" << v.getScreenMin() << ", " << v.getScreenMax()<< "]  ==> range = " << v.getScreenRange();

		str << "\n     - TEST :  G["<< v.getGraphMin() <<                          "] => S " << v.toScreen(v.getGraphMin());
		str << "\n               G["<< (v.getGraphMin()+v.getGraphMax())/2.0 <<    "] => S " << v.toScreen((v.getGraphMin()+v.getGraphMax())/2.0);
		str << "\n               G["<< v.getGraphMax() <<                          "] => S " << v.toScreen(v.getGraphMax());
		str << "\n               S["<< v.getScreenMin() <<                         "] => G " << v.toGraph(v.getScreenMin());
		str << "\n               S["<< (v.getScreenMin()+v.getScreenMax())/2.0 <<  "] => G " << v.toGraph((v.getScreenMin()+v.getScreenMax())/2.0);
		str << "\n               S["<< v.getScreenMax() <<                         "] => G " << v.toGraph(v.getScreenMax());

		return str;
	}

	// ============================
	//    CoordinateConverter   CLASS
	// ============================
	template <class SCREEN_COORD, class GRAPH_COORD>
	class LogCoordinateConverter : public CoordinateConverter<SCREEN_COORD, GRAPH_COORD>
	{
		public:
			typedef SCREEN_COORD TypeScreenCoord;
			typedef GRAPH_COORD  TypeGraphCoord;

		private:
			typedef CoordinateConverter<SCREEN_COORD, GRAPH_COORD> _B;
			long double _logGraphMin;
			long double _logGraphMax;

		public:
			LogCoordinateConverter()
			{
				update( 0, 100, 0, 100);
			}

			LogCoordinateConverter(GRAPH_COORD graphMin, GRAPH_COORD graphMax, SCREEN_COORD screenMin, SCREEN_COORD screenMax)
			{
				update( graphMin, graphMax, screenMin, screenMax);
			}

			virtual void update(GRAPH_COORD graphMin, GRAPH_COORD graphMax, SCREEN_COORD screenMin, SCREEN_COORD screenMax)
			{
				if (graphMin <=0) graphMin = 0.000000001;
				if (graphMax <=0) graphMax = 0.1;
				_B::update(graphMin, graphMax, screenMin, screenMax);

				_logGraphMin = log(graphMin);
				_logGraphMax = log(graphMax);
				_B::_graphToScreenScale = (screenMax - screenMin) / (_logGraphMax - _logGraphMin);
				_B::_screenToGraphScale = (_logGraphMax - _logGraphMin) / (screenMax - screenMin);
//				TRACE_INFO( "LogCoordinateConverter::Update(" << graphMin<<", "<< graphMax<<", "<< screenMin<<", "<< screenMax<<" )  ==> ["<< _logGraphMin << ", " << _logGraphMax << "]" );
			}

			virtual SCREEN_COORD toScreen(GRAPH_COORD v)
			{
				if (v<=0) return _B::_screenMin;
				return  ((log(v)-_logGraphMin)*_B::_graphToScreenScale + _B::_screenMin);
			}

			virtual GRAPH_COORD toGraph(SCREEN_COORD v)  { return  pow(10, (v-_B::_screenMin)*_B::_screenToGraphScale + _logGraphMin); }
			virtual const char* GetConversionType() { return "LOG"; }
	};

	template <class SCREEN_COORD, class GRAPH_COORD>
	class Pow10CoordinateConverter : public CoordinateConverter<SCREEN_COORD, GRAPH_COORD>
	{
		public:
			typedef SCREEN_COORD TypeScreenCoord;
			typedef GRAPH_COORD  TypeGraphCoord;

		private:
			typedef CoordinateConverter<SCREEN_COORD, GRAPH_COORD> _B;
			long double _pow10GraphMin;
			long double _pow10GraphMax;

		public:
			Pow10CoordinateConverter()
			{
				update( 0, 100, 0, 100);
			}

			Pow10CoordinateConverter(GRAPH_COORD graphMin, GRAPH_COORD graphMax, SCREEN_COORD screenMin, SCREEN_COORD screenMax)
			{
				update( graphMin, graphMax, screenMin, screenMax);
			}

			virtual void update(GRAPH_COORD graphMin, GRAPH_COORD graphMax, SCREEN_COORD screenMin, SCREEN_COORD screenMax)
			{
				_B::update(graphMin, graphMax, screenMin, screenMax);

				_pow10GraphMin = pow(10, graphMin);
				_pow10GraphMax = pow(10, graphMax);
				_B::_graphToScreenScale = (screenMax - screenMin) / (_pow10GraphMax - _pow10GraphMin);
				_B::_screenToGraphScale = (_pow10GraphMax - _pow10GraphMin) / (screenMax - screenMin);
//				TRACE_INFO( "Pow10CoordinateConverter::Update(" << graphMin<<", "<< graphMax<<", "<< screenMin<<", "<< screenMax<<" )  ==> ["<< _pow10GraphMin << ", " << _pow10GraphMax << "]" );
			}

			virtual SCREEN_COORD toScreen(GRAPH_COORD v)
			{
				return  ((pow(10, v)-_pow10GraphMin)*_B::_graphToScreenScale + _B::_screenMin);
			}

			virtual GRAPH_COORD toGraph(SCREEN_COORD v)  { return  log((v-_B::_screenMin)*_B::_screenToGraphScale + _pow10GraphMin); }
			virtual const char* GetConversionType() { return "10^x"; }
	};

	template <class SCREEN_COORD, class GRAPH_COORD>
	class GenericCoordinateConverter : public CoordinateConverter<SCREEN_COORD, GRAPH_COORD>
	{
		public:
			typedef SCREEN_COORD TypeScreenCoord;
			typedef GRAPH_COORD  TypeGraphCoord;

			typedef enum {
				AXIS_SCALE_STD = 0,
				AXIS_SCALE_LOG,
				AXIS_SCALE_POW10,
			} AxisScaleType;

		protected:
			typedef CoordinateConverter<SCREEN_COORD, GRAPH_COORD> _B;
			long double _fctGraphMin;
			long double _fctGraphMax;

			typedef GRAPH_COORD (*TypeConvertFct) (GRAPH_COORD);
			TypeConvertFct _convertFct;
			TypeConvertFct _unConvertFct;
			int _scaleType; // AxisScaleType
			const char* _convTypeName;

			static GRAPH_COORD _defautFct(GRAPH_COORD v) { return v; }
			static GRAPH_COORD _logFct(GRAPH_COORD v) { if (v<=0) v=0.00001; return log(v); }
			static GRAPH_COORD _pow10Fct(GRAPH_COORD v) { return pow(10,v); }
			static GRAPH_COORD _X2fct(GRAPH_COORD v) { return (v*v); }
			static GRAPH_COORD _sqrtFct(GRAPH_COORD v) { return sqrt(v); }


		public:
			GenericCoordinateConverter()
			: _convertFct(_defautFct)
			, _unConvertFct(_defautFct)
			, _convTypeName("")
			{
				update( 0, 100, 0, 100);
			}

			GenericCoordinateConverter(TypeConvertFct convertFct, TypeConvertFct unConvertFct, const char* convTypeName)
			: _convertFct(convertFct)
			, _unConvertFct(unConvertFct)
			, _convTypeName(convTypeName)
			{
				update( 0, 100, 0, 100);
			}

			GenericCoordinateConverter(TypeConvertFct convertFct, TypeConvertFct unConvertFct, const char* convTypeName, GRAPH_COORD graphMin, GRAPH_COORD graphMax, SCREEN_COORD screenMin, SCREEN_COORD screenMax)
			: _convertFct(convertFct)
			, _unConvertFct(unConvertFct)
			, _convTypeName(convTypeName)
			{
				update( graphMin, graphMax, screenMin, screenMax);
			}

			void SetConvFct(TypeConvertFct convertFct, TypeConvertFct unConvertFct, const char* convTypeName) {
				_convTypeName = convTypeName;
				_convertFct = convertFct;
				_unConvertFct = unConvertFct;
				_B::Update();
			}
			void SetConvStd()   { SetConvFct(_defautFct, _defautFct, ""); _scaleType = AXIS_SCALE_STD; }
			void SetConvLog()   {
				SetConvFct(_logFct,    _pow10Fct,  "LOG"); _scaleType = AXIS_SCALE_LOG;
				TRACE_INFO("SetConvLog()::");
				updateGraphSize(_B::getGraphMin(), _B::getGraphMax());
			}
			void SetConvPow10() { SetConvFct(_pow10Fct,  _logFct,    "10^x"); _scaleType = AXIS_SCALE_POW10; }

			void SetScaleType(AxisScaleType t)	{
				switch(t) {
					case AXIS_SCALE_STD:
						SetConvStd();
						break;
					case AXIS_SCALE_LOG:
						SetConvLog();
						break;
					case AXIS_SCALE_POW10:
						SetConvPow10();
						break;
				}
			}

			AxisScaleType GetScaleType() { return _scaleType; }
			virtual const char* GetConversionType() { return _convTypeName; }


			virtual void update(GRAPH_COORD graphMin, GRAPH_COORD graphMax, SCREEN_COORD screenMin, SCREEN_COORD screenMax)
			{
				if (_scaleType == AXIS_SCALE_LOG) {
					if (graphMin <=0) graphMin = 0.000000001;
					if (graphMax <=0) graphMax = 0.1;
				}

				_B::update(graphMin, graphMax, screenMin, screenMax);

				_fctGraphMin = _convertFct(graphMin);
				_fctGraphMax = _convertFct(graphMax);
				_B::_graphToScreenScale = (screenMax - screenMin) / (_fctGraphMax - _fctGraphMin);
				_B::_screenToGraphScale = (_fctGraphMax - _fctGraphMin) / (screenMax - screenMin);
				if (_scaleType == AXIS_SCALE_LOG) {
					TRACE_INFO("update()::   graphMin="<< graphMin << "    graphMax="<< graphMax
					           << "      _fctGraphMin="<<_fctGraphMin<< "      _fctGraphMax"<<_fctGraphMax
					           << "      g2s="<<_B::_graphToScreenScale<< "      s2g"<<_B::_screenToGraphScale
					           );
				}
			}

			virtual void updateScreenSize(SCREEN_COORD screenMin, SCREEN_COORD screenMax)
			{
				_B::updateScreenSize(screenMin, screenMax);

				_B::_graphToScreenScale = (screenMax - screenMin) / (_fctGraphMax - _fctGraphMin);
				_B::_screenToGraphScale = (_fctGraphMax - _fctGraphMin) / (screenMax - screenMin);
			}

			virtual void updateGraphSize(GRAPH_COORD graphMin, GRAPH_COORD graphMax)
			{
				if (_scaleType == AXIS_SCALE_LOG) {
					if (graphMin <=0) graphMin = 0.000000001;
					if (graphMax <=0) graphMax = 0.1;
				}
				_B::updateGraphSize(graphMin, graphMax);

				_fctGraphMin = _convertFct(graphMin);
				_fctGraphMax = _convertFct(graphMax);
				_B::_graphToScreenScale = (_B::_screenMax - _B::_screenMin) / (_fctGraphMax - _fctGraphMin);
				_B::_screenToGraphScale = (_fctGraphMax - _fctGraphMin) / (_B::_screenMax - _B::_screenMin);
				if (_scaleType == AXIS_SCALE_LOG) {
					TRACE_INFO("updateGraphSize()::  graphMin="<< graphMin << "    graphMax="<< graphMax
					           << "      _fctGraphMin="<<_fctGraphMin<< "      _fctGraphMax"<<_fctGraphMax
					           << "      g2s="<<_B::_graphToScreenScale<< "      s2g"<<_B::_screenToGraphScale
					           );
				}
			}

			virtual SCREEN_COORD toScreen(GRAPH_COORD v)
			{
				return  ((_convertFct(v)-_fctGraphMin)*_B::_graphToScreenScale + _B::_screenMin);
			}

			virtual GRAPH_COORD toGraph(SCREEN_COORD v)  { return  _unConvertFct((v-_B::_screenMin)*_B::_screenToGraphScale + _fctGraphMin); }
	};

};

#endif
