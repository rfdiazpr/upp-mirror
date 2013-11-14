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
			Value _graphMaxLimit;
			Value _graphMinLimit;

		public:
		typedef CoordinateConverter CLASSNAME;
			CoordinateConverter()
			{
				update( 0, 100, 0, 100);
			}

			CoordinateConverter(TypeGraphCoord graphMin, TypeGraphCoord graphMax, TypeScreenCoord screenMin, TypeScreenCoord screenMax)
			{
				update( graphMin, graphMax, screenMin, screenMax);
			}

			inline TypeScreenCoord getScreenMin()        const { return _screenMin; }
			inline TypeScreenCoord getScreenMax()        const { return _screenMax; }
			inline TypeScreenCoord getScreenRange()      const { return _screenRange; }
			inline TypeGraphCoord  getGraphMin()         const { return _graphMin; }
			inline TypeGraphCoord  getGraphMax()         const { return _graphMax; }
			inline Value           getGraphMinLimit()    const { return _graphMinLimit; }
			inline Value           getGraphMaxLimit()    const { return _graphMaxLimit; }
			inline TypeGraphCoord  getGraphRange()       const { return _graphRange; }
			inline TypeGraphCoord  getSignedGraphRange() const { return (_graphMax-_graphMin); }
			inline bool IsInGraphVisibleRange(TypeGraphCoord p) const { return ((_graphMin <= p) && (p <= _graphMax)); }

			Callback MakeRestoreAxisMinMaxCB() {
				return THISBACK2(updateGraphSize, _graphMin, _graphMax);
			}
			inline void setGraphMaxRangeLimit(TypeGraphCoord v) { _graphMaxLimit = v; }
			inline void setGraphMinRangeLimit(TypeGraphCoord v) { _graphMinLimit = v; }
			inline void resetGraphRangeLimits() { _graphMaxLimit = Null; _graphMinLimit = Null; }
			virtual TypeGraphCoord applyRangeLimits(TypeGraphCoord v) {
				if (!_graphMaxLimit.IsNull()) {
					if (v > ValueTo<TypeGraphCoord>(_graphMaxLimit)) v = _graphMaxLimit;
				}
				if (!_graphMinLimit.IsNull()) {
					if (v < ValueTo<TypeGraphCoord>(_graphMinLimit)) v = _graphMinLimit;
				}
				return v;
			}

			virtual bool isInRangeLimits(TypeGraphCoord v) {
				if (!_graphMaxLimit.IsNull()) {
					if (v > ValueTo<TypeGraphCoord>(_graphMaxLimit)) return false;;
				}
				if (!_graphMinLimit.IsNull()) {
					if (v < ValueTo<TypeGraphCoord>(_graphMinLimit)) return false;;
				}
				return true;
			}

			
			inline void Update() {
				update( getGraphMin(), getGraphMax(), getScreenMin(), getScreenMax() );
			}

			inline void SetGraphMin(TypeGraphCoord v) { update( v, getGraphMax(), getScreenMin(), getScreenMax() ); }
			inline void SetGraphMax(TypeGraphCoord v) { update( getGraphMin(), v, getScreenMin(), getScreenMax() ); }
			virtual void update(TypeGraphCoord graphMin, TypeGraphCoord graphMax, TypeScreenCoord screenMin, TypeScreenCoord screenMax)
			{
				if (screenMin!=_screenMin  || screenMax!=_screenMax || _graphMin!=graphMin || _graphMax!=graphMax ) {
					//TRACE_INFO("CoordinateConverter.update(" << graphMin << ", " << graphMax  << ", " << screenMin<< ", " << screenMax << " )" );
					_graphMin = applyRangeLimits(graphMin);
					_graphMax = applyRangeLimits(graphMax);
					_screenMin = screenMin;
					_screenMax = screenMax;
					_graphRange =  tabs(_graphMax - _graphMin);
					_screenRange = tabs(_screenMax - _screenMin);
					_graphToScreenScale = (_screenMax - _screenMin) / (_graphMax - _graphMin);
					_screenToGraphScale = (_graphMax - _graphMin) / (_screenMax - _screenMin);
				}
			}

			virtual void updateScreenSize(TypeScreenCoord screenMin, TypeScreenCoord screenMax)
			{
				if (screenMin!=_screenMin  || screenMax!=_screenMax ) {
					//TRACE_INFO("CoordinateConverter.update( "<< screenMin<< ", " << screenMax << " )" );
					_screenMin = screenMin;
					_screenMax = screenMax;
					_screenRange = tabs(_screenMax - _screenMin);
					_graphToScreenScale = (_screenMax - _screenMin) / (_graphMax - _graphMin);
					_screenToGraphScale = (_graphMax - _graphMin) / (_screenMax - _screenMin);
				}
			}

			virtual void updateGraphSize(TypeGraphCoord graphMin, TypeGraphCoord graphMax)
			{
				if ( _graphMin!=graphMin || _graphMax!=graphMax ) {
					//TRACE_INFO("CoordinateConverter.update(" << graphMin << ", " << graphMax  << " )" );
					_graphMin = applyRangeLimits(graphMin);
					_graphMax = applyRangeLimits(graphMax);
					_graphRange =  tabs(_graphMax - _graphMin);
					_graphToScreenScale = (_screenMax - _screenMin) / (_graphMax - _graphMin);
					_screenToGraphScale = (_graphMax - _graphMin) / (_screenMax - _screenMin);
				}
			}

			virtual void Scroll( TypeScreenCoord offset ) {
				TypeGraphCoord graphMin = toGraph( _screenMin - offset );
				TypeGraphCoord graphMax = toGraph( _screenMax - offset );
				if ( isInRangeLimits(graphMin) && isInRangeLimits(graphMax)) {
					updateGraphSize( graphMin, graphMax );
				}
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
				setGraphMinRangeLimit(0.0000000001);
				update( 0.1, 100, 0.1, 100);
			}

			LogCoordinateConverter(TypeGraphCoord graphMin, TypeGraphCoord graphMax, TypeScreenCoord screenMin, TypeScreenCoord screenMax)
			{
				setGraphMinRangeLimit(0.0000000001);
				update( graphMin, graphMax, screenMin, screenMax);
			}

			virtual void update(TypeGraphCoord graphMin, TypeGraphCoord graphMax, TypeScreenCoord screenMin, TypeScreenCoord screenMax)
			{
				_B::update(graphMin, graphMax, screenMin, screenMax);

				_logGraphMin = log10(_graphMin);
				_logGraphMax = log10(_graphMax);
				_B::_graphToScreenScale = (screenMax - screenMin) / (_logGraphMax - _logGraphMin);
				_B::_screenToGraphScale = (_logGraphMax - _logGraphMin) / (screenMax - screenMin);
			}

			virtual void updateScreenSize(TypeScreenCoord screenMin, TypeScreenCoord screenMax)
			{
				_B::updateScreenSize(screenMin, screenMax);

				_B::_graphToScreenScale = (screenMax - screenMin) / (_logGraphMax - _logGraphMin);
				_B::_screenToGraphScale = (_logGraphMax - _logGraphMin) / (screenMax - screenMin);
			}


			virtual void updateGraphSize(TypeGraphCoord graphMin, TypeGraphCoord graphMax)
			{
				_B::updateGraphSize(graphMin, graphMax);

				_logGraphMin = log10(_graphMin);
				_logGraphMax = log10(_graphMax);
				_B::_graphToScreenScale = (_B::_screenMax - _B::_screenMin) / (_logGraphMax - _logGraphMin);
				_B::_screenToGraphScale = (_logGraphMax - _logGraphMin) / (_B::_screenMax - _B::_screenMin);
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
				_pow10GraphMin = pow(10, _graphMin);
				_pow10GraphMax = pow(10, _graphMax);
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

			void SetConvFct(TypeConvertFct convertFct, TypeConvertFct unConvertFct, const char* convTypeName) {
				_convTypeName = convTypeName;
				_convertFct = convertFct;
				_unConvertFct = unConvertFct;
				resetGraphRangeLimits();
				_B::Update();
			}
			void SetConvStd() {
				SetConvFct(_defautFct, _defautFct, "");
				_scaleType = AXIS_SCALE_STD;
			}
			void SetConvLog()   {
				setGraphMinRangeLimit(0.00000000000001);
				Update();
				SetConvFct(_logFct,    _pow10Fct,  "LOG");
				_scaleType = AXIS_SCALE_LOG;
				updateGraphSize(_B::getGraphMin(), _B::getGraphMax());
			}
			void SetConvPow10() {
				SetConvFct(_pow10Fct,  _logFct,    "10^x");
				_scaleType = AXIS_SCALE_POW10;
			}

			void SetScaleType(unsigned int t)	{
				if ( t <=  AXIS_SCALE_POW10) {
					SetScaleType( (AxisScaleType) t );
				}

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

			virtual void update(TypeGraphCoord graphMin, TypeGraphCoord graphMax, TypeScreenCoord screenMin, TypeScreenCoord screenMax) {
				_B::update(graphMin, graphMax, screenMin, screenMax);

				_fctGraphMin = _convertFct(_graphMin);
				_fctGraphMax = _convertFct(_graphMax);
				_B::_graphToScreenScale = (screenMax - screenMin) / (_fctGraphMax - _fctGraphMin);
				_B::_screenToGraphScale = (_fctGraphMax - _fctGraphMin) / (screenMax - screenMin);
			}

			virtual void updateScreenSize(TypeScreenCoord screenMin, TypeScreenCoord screenMax)
			{
				_B::updateScreenSize(screenMin, screenMax);

				_B::_graphToScreenScale = (screenMax - screenMin) / (_fctGraphMax - _fctGraphMin);
				_B::_screenToGraphScale = (_fctGraphMax - _fctGraphMin) / (screenMax - screenMin);
			}

			virtual void updateGraphSize(TypeGraphCoord graphMin, TypeGraphCoord graphMax)
			{
				_B::updateGraphSize(graphMin, graphMax);

				_fctGraphMin = _convertFct(_graphMin);
				_fctGraphMax = _convertFct(_graphMax);
				_B::_graphToScreenScale = (_B::_screenMax - _B::_screenMin) / (_fctGraphMax - _fctGraphMin);
				_B::_screenToGraphScale = (_fctGraphMax - _fctGraphMin) / (_B::_screenMax - _B::_screenMin);
			}

			virtual TypeScreenCoord toScreen(TypeGraphCoord v)	{ return  (TypeScreenCoord)((_convertFct(v)-_fctGraphMin)*_B::_graphToScreenScale + _B::_screenMin); }
			virtual TypeGraphCoord toGraph(TypeScreenCoord v)  { return  _unConvertFct((v-_B::_screenMin)/_B::_graphToScreenScale + _fctGraphMin); }
	};

};

typedef GraphDraw_ns::GenericCoordinateConverter GraphGConv; // Needed by '.USC' file

#endif
