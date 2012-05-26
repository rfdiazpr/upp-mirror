#ifndef _GraphCtrl_CoordinateConverter_h_
#define _GraphCtrl_CoordinateConverter_h_


namespace GraphDraw_ns
{

	// ============================
	//    CoordinateConverter   CLASS
	// ============================
	class CoordinateConverter
	{
		protected:
			long double _graphToScreenScale;
			long double _graphMin;
			long double _graphMax;
			long double _graphRange;
			long double _screenToGraphScale;
			TypeScreenCoord _screenMin;
			TypeScreenCoord _screenMax;
			TypeScreenCoord _screenRange;

		public:
			CoordinateConverter()
			{
				update( 0, 100, 0, 100);
			}

			CoordinateConverter(TypeGraphCoord graphMin, TypeGraphCoord graphMax, TypeScreenCoord screenMin, TypeScreenCoord screenMax)
			{
				update( graphMin, graphMax, screenMin, screenMax);
			}

			inline TypeScreenCoord getScreenMin()   const { return _screenMin; }
			inline TypeScreenCoord getScreenMax()   const { return _screenMax; }
			inline TypeScreenCoord getScreenRange() const { return _screenRange; }
			inline TypeGraphCoord  getGraphMin()    const { return _graphMin; }
			inline TypeGraphCoord  getGraphMax()    const { return _graphMax; }
			inline TypeGraphCoord  getGraphRange()  const { return _graphRange; }
			inline TypeGraphCoord  getSignedGraphRange() const { return (_graphMax-_graphMin); }
			inline bool IsInGraphRange(TypeGraphCoord p) const { return ((_graphMin <= p) && (p <= _graphMax)); }

			inline void Update() {
				update( getGraphMin(), getGraphMax(), getScreenMin(), getScreenMax() );
			}

			virtual void update(TypeGraphCoord graphMin, TypeGraphCoord graphMax, TypeScreenCoord screenMin, TypeScreenCoord screenMax)
			{
				//TRACE_INFO("CoordinateConverter.update(" << graphMin << ", " << graphMax  << ", " << screenMin<< ", " << screenMax << " )" );
				_graphMin = graphMin;
				_graphMax = graphMax;
				_screenMin = screenMin;
				_screenMax = screenMax;
				_graphRange =  tabs(_graphMax - _graphMin);
				_screenRange = tabs(_screenMax - _screenMin);
				_graphToScreenScale = (_screenMax - _screenMin) / (_graphMax - _graphMin);
				_screenToGraphScale = (_graphMax - _graphMin) / (_screenMax - _screenMin);
			}

			virtual void updateScreenSize(TypeScreenCoord screenMin, TypeScreenCoord screenMax)
			{
				//TRACE_INFO("CoordinateConverter.update(" << graphMin << ", " << graphMax  << ", " << screenMin<< ", " << screenMax << " )" );
				_screenMin = screenMin;
				_screenMax = screenMax;
				_screenRange = tabs(_screenMax - _screenMin);
				_graphToScreenScale = (_screenMax - _screenMin) / (_graphMax - _graphMin);
				_screenToGraphScale = (_graphMax - _graphMin) / (_screenMax - _screenMin);
			}

			virtual void updateGraphSize(TypeGraphCoord graphMin, TypeGraphCoord graphMax)
			{
				//TRACE_INFO("CoordinateConverter.update(" << graphMin << ", " << graphMax  << ", " << screenMin<< ", " << screenMax << " )" );
				_graphMin = graphMin;
				_graphMax = graphMax;
				_graphRange =  tabs(_graphMax - _graphMin);
				_graphToScreenScale = (_screenMax - _screenMin) / (_graphMax - _graphMin);
				_screenToGraphScale = (_graphMax - _graphMin) / (_screenMax - _screenMin);
			}

			virtual void Scroll( TypeScreenCoord offset ) {
				updateGraphSize( toGraph( _screenMin - offset ),
								 toGraph( _screenMax - offset )
								 );
			}

			virtual void Zoom( double factor ) {
				TypeScreenCoord addRange=(TypeScreenCoord)((_screenRange*(factor-1.0))/2.0);
				updateGraphSize( toGraph( _screenMin - addRange ),
								 toGraph( _screenMax + addRange )
								);
			}


			virtual TypeScreenCoord toScreen(TypeGraphCoord v)  { return  (TypeScreenCoord)((v-_graphMin)*_graphToScreenScale + _screenMin); }
			virtual TypeGraphCoord  toGraph(TypeScreenCoord v)  { return  ((v-_screenMin)*_screenToGraphScale + _graphMin); }
			virtual const char* GetConversionType() { return "STD"; }
	};

	template<class STREAM>
	STREAM& operator<<(STREAM& str, CoordinateConverter v)
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
	class LogCoordinateConverter : public CoordinateConverter
	{
		private:
			typedef CoordinateConverter _B;
			long double _logGraphMin;
			long double _logGraphMax;

		public:
			LogCoordinateConverter()
			{
				update( 0, 100, 0, 100);
			}

			LogCoordinateConverter(TypeGraphCoord graphMin, TypeGraphCoord graphMax, TypeScreenCoord screenMin, TypeScreenCoord screenMax)
			{
				update( graphMin, graphMax, screenMin, screenMax);
			}

			virtual void update(TypeGraphCoord graphMin, TypeGraphCoord graphMax, TypeScreenCoord screenMin, TypeScreenCoord screenMax)
			{
				if (graphMin <=0) graphMin = 0.000000001;
				if (graphMax <=0) graphMax = 0.1;
				_B::update(graphMin, graphMax, screenMin, screenMax);

				_logGraphMin = log10(graphMin);
				_logGraphMax = log10(graphMax);
				_B::_graphToScreenScale = (screenMax - screenMin) / (_logGraphMax - _logGraphMin);
				_B::_screenToGraphScale = (_logGraphMax - _logGraphMin) / (screenMax - screenMin);
//				TRACE_INFO( "LogCoordinateConverter::Update(" << graphMin<<", "<< graphMax<<", "<< screenMin<<", "<< screenMax<<" )  ==> ["<< _logGraphMin << ", " << _logGraphMax << "]" );
			}

			virtual TypeScreenCoord toScreen(TypeGraphCoord v)
			{
				if (v<=0) return _B::_screenMin;
				return  ((TypeScreenCoord)((log10(v)-_logGraphMin)*_B::_graphToScreenScale + _B::_screenMin));
			}

			virtual TypeGraphCoord toGraph(TypeScreenCoord v)  { return  pow(10, (v-_B::_screenMin)*_B::_screenToGraphScale + _logGraphMin); }
			virtual const char* GetConversionType() { return "LOG"; }
	};

	class Pow10CoordinateConverter : public CoordinateConverter
	{
		private:
			typedef CoordinateConverter _B;
			long double _pow10GraphMin;
			long double _pow10GraphMax;

		public:
			Pow10CoordinateConverter() {
				update( 0, 100, 0, 100);
			}

			Pow10CoordinateConverter(TypeGraphCoord graphMin, TypeGraphCoord graphMax, TypeScreenCoord screenMin, TypeScreenCoord screenMax) {
				update( graphMin, graphMax, screenMin, screenMax);
			}

			virtual void update(TypeGraphCoord graphMin, TypeGraphCoord graphMax, TypeScreenCoord screenMin, TypeScreenCoord screenMax) {
				_B::update(graphMin, graphMax, screenMin, screenMax);
				_pow10GraphMin = pow(10, graphMin);
				_pow10GraphMax = pow(10, graphMax);
				_B::_graphToScreenScale = (screenMax - screenMin) / (_pow10GraphMax - _pow10GraphMin);
				_B::_screenToGraphScale = (_pow10GraphMax - _pow10GraphMin) / (screenMax - screenMin);
			}

			virtual TypeScreenCoord toScreen(TypeGraphCoord v) { return  ((TypeScreenCoord)((pow(10, v)-_pow10GraphMin)*_B::_graphToScreenScale + _B::_screenMin)); }
			virtual TypeGraphCoord toGraph(TypeScreenCoord v)  { return  log10((v-_B::_screenMin)*_B::_screenToGraphScale + _pow10GraphMin); }
			virtual const char* GetConversionType()      { return "10^x"; }
	};

	class GenericCoordinateConverter : public CoordinateConverter
	{
		public:
			typedef enum {
				AXIS_SCALE_STD = 0,
				AXIS_SCALE_LOG,
				AXIS_SCALE_POW10,
			} AxisScaleType;

		protected:
			typedef GenericCoordinateConverter CLASSNAME;
			typedef CoordinateConverter _B;
			long double _fctGraphMin;
			long double _fctGraphMax;

			typedef TypeGraphCoord (*TypeConvertFct) (TypeGraphCoord);
			TypeConvertFct _convertFct;
			TypeConvertFct _unConvertFct;
			AxisScaleType _scaleType;
			const char* _convTypeName;

			static TypeGraphCoord _defautFct(TypeGraphCoord v) { return v; }
			static TypeGraphCoord _logFct(TypeGraphCoord v) { if (v<=0) v=0.00001; return log10(v); }
			static TypeGraphCoord _pow10Fct(TypeGraphCoord v) { return pow(10.0,v); }
			static TypeGraphCoord _X2fct(TypeGraphCoord v) { return (v*v); }
			static TypeGraphCoord _sqrtFct(TypeGraphCoord v) { return sqrt(v); }

		public:
			GenericCoordinateConverter()
			: _convertFct(_defautFct)
			, _unConvertFct(_defautFct)
			, _scaleType(AXIS_SCALE_STD)
			, _convTypeName("")
			{
				update( 0, 100, 0, 100);
				SetConvStd();
			}

//			GenericCoordinateConverter(TypeConvertFct convertFct, TypeConvertFct unConvertFct, const char* convTypeName)
//			: _convertFct(convertFct)
//			, _unConvertFct(unConvertFct)
//			, _convTypeName(convTypeName)
//			{
//				SetConvStd();
//				update( 0, 100, 0, 100);
//			}
//
//			GenericCoordinateConverter(TypeConvertFct convertFct, TypeConvertFct unConvertFct, const char* convTypeName, TypeGraphCoord graphMin, TypeGraphCoord graphMax, TypeScreenCoord screenMin, TypeScreenCoord screenMax)
//			: _convertFct(convertFct)
//			, _unConvertFct(unConvertFct)
//			, _convTypeName(convTypeName)
//			{
//				SetConvStd();
//				update( graphMin, graphMax, screenMin, screenMax);
//			}

			void SetConvFct(TypeConvertFct convertFct, TypeConvertFct unConvertFct, const char* convTypeName) {
				_convTypeName = convTypeName;
				_convertFct = convertFct;
				_unConvertFct = unConvertFct;
				_B::Update();
			}
			void SetConvStd() {
				SetConvFct(_defautFct, _defautFct, "");
				_scaleType = AXIS_SCALE_STD;
			}
			void SetConvLog()   {
				SetConvFct(_logFct,    _pow10Fct,  "LOG");
				_scaleType = AXIS_SCALE_LOG;
				TRACE_INFO("SetConvLog()::");
				updateGraphSize(_B::getGraphMin(), _B::getGraphMax());
			}
			void SetConvPow10() {
				SetConvFct(_pow10Fct,  _logFct,    "10^x");
				_scaleType = AXIS_SCALE_POW10;
			}

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

			virtual bool isRangeValid(TypeGraphCoord& graphMin, TypeGraphCoord& graphMax) {
				bool res = true;
				if (_scaleType == AXIS_SCALE_LOG) {
					TypeGraphCoord range = graphMax - graphMin;
					if (graphMin <=0) { graphMin = 0.000000001; graphMax = graphMin + range; res = false; }
					if (graphMax <=0) { graphMax = 0.000000001; graphMin = graphMax - range; res = false; }
				}
				return res;
			}

			virtual void update(TypeGraphCoord graphMin, TypeGraphCoord graphMax, TypeScreenCoord screenMin, TypeScreenCoord screenMax) {
				isRangeValid( graphMin, graphMax );
				_B::update(graphMin, graphMax, screenMin, screenMax);

				_fctGraphMin = _convertFct(graphMin);
				_fctGraphMax = _convertFct(graphMax);
				_B::_graphToScreenScale = (screenMax - screenMin) / (_fctGraphMax - _fctGraphMin);
				_B::_screenToGraphScale = (_fctGraphMax - _fctGraphMin) / (screenMax - screenMin);
//				if (_scaleType == AXIS_SCALE_LOG) {
//					TRACE_INFO("LOG GenericCoordinateConverter::update()::   graphMin="<< graphMin << "    graphMax="<< graphMax
//					           << "      [ _fctGraphMin="<<_fctGraphMin<< "  _fctGraphMax="<<_fctGraphMax
//					           << " ]    [ _screenMin="<<_B::_screenMin<< "  _screenMax="<<_B::_screenMax
//					           << " ]    [ g2s="<<_B::_graphToScreenScale<< "  s2g="<<_B::_screenToGraphScale
//					           << " ]");
//				}
			}

			virtual void updateScreenSize(TypeScreenCoord screenMin, TypeScreenCoord screenMax)
			{
				_B::updateScreenSize(screenMin, screenMax);

				_B::_graphToScreenScale = (screenMax - screenMin) / (_fctGraphMax - _fctGraphMin);
				_B::_screenToGraphScale = (_fctGraphMax - _fctGraphMin) / (screenMax - screenMin);
			}

			virtual void updateGraphSize(TypeGraphCoord graphMin, TypeGraphCoord graphMax)
			{
				isRangeValid( graphMin, graphMax );
				_B::updateGraphSize(graphMin, graphMax);

				_fctGraphMin = _convertFct(graphMin);
				_fctGraphMax = _convertFct(graphMax);
				_B::_graphToScreenScale = (_B::_screenMax - _B::_screenMin) / (_fctGraphMax - _fctGraphMin);
				_B::_screenToGraphScale = (_fctGraphMax - _fctGraphMin) / (_B::_screenMax - _B::_screenMin);
//				if (_scaleType == AXIS_SCALE_LOG) {
//					TRACE_INFO("LOG GenericCoordinateConverter::updateGraphSize( graphMin="<< graphMin << "    graphMax="<< graphMax << " ) : "
//					           << "      [ _fctGraphMin="<<_fctGraphMin<< "      _fctGraphMax="<<_fctGraphMax
//					           << " ]    [ _screenMin="<<_B::_screenMin<< "      _screenMax="<<_B::_screenMax
//					           << " ]    [ g2s="<<_B::_graphToScreenScale<< "      s2g="<<_B::_screenToGraphScale
//					           << " ]");
//				}
			}

			virtual TypeScreenCoord toScreen(TypeGraphCoord v)	{ return  (TypeScreenCoord)((_convertFct(v)-_fctGraphMin)*_B::_graphToScreenScale + _B::_screenMin); }
			virtual TypeGraphCoord toGraph(TypeScreenCoord v)  { return  _unConvertFct((v-_B::_screenMin)/_B::_graphToScreenScale + _fctGraphMin); }
	};

};

#endif
