#ifndef _GraphCtrl_GridStepManager_h_
#define _GraphCtrl_GridStepManager_h_

#include <limits>

namespace GraphDraw_ns
{

	// ============================
	//    GridStepManager   CLASS
	// ============================
	class  GridStepData {
		public:
			bool tickLevel;    // 0:Major tick    1:secondary tick    2: ....
			char drawTickText;
			Size_<Upp::uint16>    textSize;
			TypeGraphCoord stepGraphValue;
			String 		   text;
			Value          customParams; // general purpose step parameter holder : filled when calculating the steps
			
			GridStepData()
			: tickLevel(0)
			, drawTickText(true)
			{
			}
	};
	
	class GridStepIterator {
		private:
			int _nbSteps;     // current step range (according to grid step)
			int _currentStep; // current step number
			CoordinateConverter& _coordConverter;
			GridStepData* const  _stepData; // points to steps parameters array (pre-calculated by GridStepManager)

		public:
			typedef GridStepIterator CLASSNAME;

			GridStepIterator( CoordinateConverter& conv, int nbSteps, GridStepData* stepData, int currentStep)
			: _nbSteps(nbSteps)
			, _currentStep(currentStep)
			, _coordConverter(conv)
			, _stepData(stepData)
			{}

		
			GridStepIterator(const CLASSNAME& p)
			: _nbSteps(p._nbSteps)
			, _currentStep(p._currentStep)
			, _coordConverter(p._coordConverter)
			, _stepData(p._stepData)
			{}
		
		public:
			inline bool operator==( const CLASSNAME& v) const { return( _currentStep==v._currentStep ); }
			inline bool operator!=( const CLASSNAME& v) const { return( _currentStep!=v._currentStep ); }
			inline operator TypeScreenCoord() const { return _coordConverter.toScreen( getGraphValue() ); }
			inline operator Size() const            { return Size(_stepData[_currentStep].textSize.cx, _stepData[_currentStep].textSize.cy); }
			inline operator String() const          { return _stepData[_currentStep].text; }
			GridStepData*    operator->()  { return _stepData + _currentStep; }

			inline TypeGraphCoord getGraphValue()   const { return _stepData[_currentStep].stepGraphValue; }
			inline const Value& getCustomStepData() const { return _stepData[_currentStep].customParams; }
			inline unsigned int getTickLevel()      const { return _stepData[_currentStep].tickLevel; }
			inline bool drawTickText()              const { return _stepData[_currentStep].drawTickText; }
			inline TypeGraphCoord getGraphRange()   const { return _coordConverter.getSignedGraphRange(); }
			inline int getStepNum()                 const { return _currentStep; }
			inline bool isFirst()                   const { return (_currentStep==0); }
			inline bool isLast()                    const { return (_currentStep==(_nbSteps-1)); }

			inline CLASSNAME& toEnd()   { _currentStep = _nbSteps; return *this; }
			inline CLASSNAME& toBegin() { _currentStep = 0; return *this; }

			// ++X
			inline CLASSNAME&  operator++()
			{
				++_currentStep;
				return *this;
			}

			// X++
			CLASSNAME  operator++(int)
			{
				CLASSNAME tmp(*this);
				++_currentStep;
				return tmp;
			}
	};


	typedef Callback2< const GridStepIterator&, String&> TypeFormatTextCbk; // IN: valueIterator,  OUT: formated value

	class GridStepManager : public ChangeStatus {
		public:
			enum { NB_MAX_STEPS = 100 };
			typedef GridStepManager  CLASSNAME;
			typedef GridStepIterator Iterator;
			typedef Callback2<CLASSNAME&, CoordinateConverter&> TypeGridStepCalcCallBack;

		protected:
			double       _textSize;    // tick Text MAX Size  ( can be height or width )
			unsigned int _nbMaxSteps;  // steps range is : [0, _nbMaxSteps]
			unsigned int _nbSteps;     // current step range (according to grid step)
			unsigned int _currentStep; // current step number
			CoordinateConverter& _coordConverter;
			bool updateNeeded;
			
			GridStepData   _stepDrawingParams[NB_MAX_STEPS+1]; // general purpose step parameter holder : filled when calculating the steps

			TypeGridStepCalcCallBack _updateCbk;
			
			ChangeStatus  coordConvLastStatus;

			TypeGraphCoord  GetNormalizedStep(TypeGraphCoord  range)
			{
				int e = 0;
				double s = Upp::normalize(range/(_nbMaxSteps+1), e);
				double sign = 1;
				if (s<0) sign = -1;

				s = sign*s;
				if     ( (s>2) && (s<=5)  ) { s = 5; }
				else if( (s>1) && (s<=2)  ) { s = 2; }
				else if( (s>5) && (s<=10) ) { s = 1; e++; }
				else   { s = 1; }
				return sign * s * Upp::ipow10(e);
			}

			template <class T>
			T  GetNormalizedStep(TypeGraphCoord  range, const Vector<T> stepValues)
			{
				double s = range/(_nbMaxSteps+1);
				T sign = 1;
				if (s<0) sign = -1;

				s = sign*s;
				int c=0;
				while (c < stepValues.GetCount()-1) {
					if ( (s>stepValues[c]) && (s<=stepValues[c+1])  ) {
						s = stepValues[c+1];
						return sign*s;
					}
					++c;
				}
				if (c==stepValues.GetCount()) s = stepValues[0];
				return sign*s;
			}

			TypeGraphCoord  GetGridStartValue(TypeGraphCoord  pstepValue, TypeGraphCoord pgraphMin)
			{
				TypeGraphCoord res;
				long double stepValue = pstepValue;
				long double graphMin = pgraphMin;
				if (graphMin>=0) {
					res = ((Upp::int64)(graphMin/stepValue + 1.0 - std::numeric_limits<TypeGraphCoord>::epsilon()) ) * stepValue;
				} else {
					res = (((Upp::int64)(graphMin/stepValue) ) * stepValue);
				}
				if (res < pgraphMin) res = pgraphMin;
				return res;
			}

		public:
			GridStepManager(CoordinateConverter& coordConv)
			: _textSize(30)
			, _nbMaxSteps( Upp::min(15, (int)NB_MAX_STEPS) )
			, _currentStep( 0 )
			, _coordConverter( coordConv )
			{
				setStdGridSteps();
				UpdateGridSteps();
			}

			GridStepManager(int nbSteps, int currStep, CoordinateConverter& coordConv)
			: _textSize(30)
			, _nbMaxSteps( Upp::max(5, Upp::min(nbSteps, (int)NB_MAX_STEPS)) )
			, _currentStep( currStep )
			, _coordConverter( coordConv )
			{
				setStdGridSteps();
				UpdateGridSteps();
			}
			
		private:
			// explicitly forbidden to use
			CLASSNAME& operator=( const CLASSNAME& v) { return *this; }
			//GridStepManager()
			//: _textSize(30)
			//, _nbMaxSteps( 0)//CLASSNAME::NB_MAX_STEPS) )
			//, _currentStep( 0 )
			//, _coordConverter()
			//{}

		public:
			void setGridStepCalcBack(TypeGridStepCalcCallBack cbk) { _updateCbk = cbk; incChangeStatus(); updateNeeded = true; }
			void setStdGridSteps()                                 { _updateCbk = THISBACK(stdGridStepCalcCbk);   incChangeStatus(); updateNeeded = true; }
			void setLogGridSteps()                                 { _updateCbk = THISBACK(log10GridStepCalcCbk); incChangeStatus(); updateNeeded = true; }
			void setTimeGridSteps()                                { _updateCbk = THISBACK(timeGridStepCalcCbk);  incChangeStatus(); updateNeeded = true; }
			void setDateGridSteps()                                { _updateCbk = THISBACK(dateGridStepCalcCbk);  incChangeStatus(); updateNeeded = true; }
 
			virtual ~GridStepManager() {}


			virtual void UpdateGridSteps()
			{
				//RLOGBLOCK("UpdateGridSteps()");
				if ( updateNeeded || coordConvLastStatus.hasChangedUpdate(_coordConverter) ) {
					if (_textSize > 1.0) _nbMaxSteps = _coordConverter.getScreenRange()/_textSize;
					_updateCbk(*this, _coordConverter);
					updateNeeded = false;
				}
			}

			void SetNbSteps(int nbSteps)
			{
				//_nbMaxSteps = nbSteps;
			}
			
			int GetNbSteps() { return _nbSteps; }

			bool SetTextMaxSize(double textSize)
			{
				textSize *= 2;
				if ( _textSize < textSize) {
					if ( textSize/_textSize > 1.2) {
						//RLOG("> SetTextMaxSize(>1.2)(" << textSize << ")  old  _textSize=" << _textSize  <<  "  ---UPDATED---");
						_textSize = textSize;
						updateNeeded = true;
						incChangeStatus();
						return true;
					}
				} else {
					if (textSize/_textSize < 0.5) {
						//RLOG("> SetTextMaxSize(<0.5)(" << textSize << ")  old  _textSize=" << _textSize  <<  "  ---UPDATED---");
						_textSize = textSize;
						incChangeStatus();
						updateNeeded = true;
						return true;
					}
				}
				return false;
			}

			Iterator End() {
				return GridStepIterator( _coordConverter, _nbSteps+1, _stepDrawingParams, _nbSteps+1);
			}

			Iterator Begin() {
				return GridStepIterator( _coordConverter, _nbSteps+1, _stepDrawingParams, 0);
			}

			void stdGridStepCalcCbk(CLASSNAME& gridStepManager, CoordinateConverter& coordConv );
			void log10GridStepCalcCbk(CLASSNAME& gridStepManager, CoordinateConverter& coordConv );
			void dateGridStepCalcCbk(CLASSNAME& gridStepManager, CoordinateConverter& coordConv );
			void timeGridStepCalcCbk(CLASSNAME& gridStepManager, CoordinateConverter& coordConv );
	};



};
#endif
