#ifndef _GraphCtrl_GridStepManager_h_
#define _GraphCtrl_GridStepManager_h_

#include <limits>

namespace GraphDraw_ns
{

	// ============================
	//    GridStepManager   CLASS
	// ============================

	template<class COORDCONVERTER>
	class GridStepIterator {
		private:
			int _nbSteps;     // current step range (according to grid step)
			int _currentStep; // current step number
			COORDCONVERTER& _coordConverter;
			typename COORDCONVERTER::TypeGraphCoord* const _graphStepValues; // points to steps array (pre-calculated by GridStepManager)

		public:
			typedef GridStepIterator<COORDCONVERTER> CLASSNAME;

			GridStepIterator( COORDCONVERTER& conv, int nbSteps, typename COORDCONVERTER::TypeGraphCoord* stepValues, int currentStep)
			: _nbSteps(nbSteps)
			, _currentStep(currentStep)
			, _coordConverter(conv)
			, _graphStepValues(stepValues)
			{}

			GridStepIterator(const CLASSNAME& p)
			: _nbSteps(p._nbSteps)
			, _currentStep(p._currentStep)
			, _coordConverter(p._coordConverter)
			, _graphStepValues(p._graphStepValues)
			{}

			inline bool operator==( const CLASSNAME& v) { return( _currentStep==v._currentStep );  }
			inline bool operator!=( const CLASSNAME& v) { return( _currentStep!=v._currentStep );  }
			inline typename COORDCONVERTER::TypeScreenCoord  operator*() { return getStepValue(); }

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

			inline typename COORDCONVERTER::TypeGraphCoord getValue() // methode that calculates the grid step position value
			{
				return _graphStepValues[_currentStep];
			}

			inline typename COORDCONVERTER::TypeScreenCoord getStepValue() // methode that calculates the grid step position value
			{
				return _coordConverter.toScreen( getValue() );
			}
	};


	template<class COORDCONVERTER, int NB_MAX_STEPS=100>
	class GridStepManager {
		public:
			typedef COORDCONVERTER TypeCoordConverter;
			typedef GridStepManager<COORDCONVERTER>  CLASSNAME;
			typedef GridStepIterator<COORDCONVERTER> Iterator;

		protected:
			unsigned int _nbMaxSteps;  // steps range is : [0, _nbMaxSteps]
			unsigned int _nbSteps;     // current step range (according to grid step)
			unsigned int _currentStep; // current step number
			TypeCoordConverter& _coordConverter;
			typename COORDCONVERTER::TypeGraphCoord _gridStartValue;
			typename COORDCONVERTER::TypeGraphCoord _gridStepValue;
			
			typename COORDCONVERTER::TypeGraphCoord _stepValues[NB_MAX_STEPS];
		public:
			GridStepManager(TypeCoordConverter& coordConv)
			: _nbMaxSteps( 15 )
			, _currentStep( 0 )
			, _coordConverter( coordConv )
			{
				Update();
			}

			GridStepManager(int nbSteps, int currStep, TypeCoordConverter& coordConv)
			: _nbMaxSteps( nbSteps )
			, _currentStep( currStep )
			, _coordConverter( coordConv )
			{
				Update();
			}
			
			GridStepManager( const CLASSNAME& v )
			: _nbMaxSteps( v._nbMaxSteps )
			, _nbSteps( v._nbSteps )
			, _currentStep( v._currentStep )
			, _coordConverter( v._coordConverter )
			, _gridStartValue( v._gridStartValue )
			, _gridStepValue( v._gridStepValue )
			{  }

			virtual ~GridStepManager() {}

			virtual void Update()
			{
				_gridStepValue = GetNormalizedStep( _coordConverter.getSignedGraphRange() );
				_gridStartValue = GetGridStartValue( _gridStepValue, _coordConverter.getGraphMin() );
				_nbSteps =  fabs((_coordConverter.getGraphMax() - _gridStartValue) / _gridStepValue);
				
				if (_nbSteps > _nbMaxSteps) // ZOOM LIMIT REACHED
				{
					_nbSteps = _nbMaxSteps;
					TRACE_ERROR("-------------ZOOM LIMIT REACHED : -----------------"
							<<"\n     - _gridStepValue = " << _gridStepValue
							<<"\n     - _gridStartValue="<<_gridStartValue
							<<"\n     - range="<<_coordConverter.getSignedGraphRange()
							<<"\n     - graphMin="<<_coordConverter.getGraphMin()
							<<"\n     - graphMax="<<_coordConverter.getGraphMax()
							<<"\n     - graphMax - _gridStartValue = " << _coordConverter.getGraphMax() - _gridStartValue
							);

				}

//				TRACE_INFO(" Zoom update "
//						<<"\n     - _gridStepValue = " << _gridStepValue
//						<<"\n     - _gridStartValue="<<_gridStartValue
//						<<"\n     - range="<<_coordConverter.getSignedGraphRange()
//						<<"\n     - graphMin="<<_coordConverter.getGraphMin()
//						<<"\n     - graphMax="<<_coordConverter.getGraphMax()
//						);


				for (int c=0; c<_nbSteps+1; ++c)
				{
					_stepValues[c] = _gridStartValue + _gridStepValue*c;
				}
			}

			void SetNbSteps(int nbSteps)
			{
				_nbMaxSteps = nbSteps;
				Update();
			}

			Iterator End() {
				return GridStepIterator<COORDCONVERTER>( _coordConverter, _nbSteps+1, _stepValues, _nbSteps+1);
			}

			Iterator Begin() {
				return GridStepIterator<COORDCONVERTER>( _coordConverter, _nbSteps+1, _stepValues, 0);
			}

			typename COORDCONVERTER::TypeGraphCoord  GetNormalizedStep(typename COORDCONVERTER::TypeGraphCoord  range)
			{
				int e = 0;
				double s = Upp::normalize(range/(_nbMaxSteps+1), e);
				if (s>0)
				{
					if     ( (s>2) && (s<=5)  ) { s = 5; }
					else if( (s>1) && (s<=2)  ) { s = 2; }
					else if( (s>5) && (s<=10) ) { s = 1; e++; }
					else   { s = 1; }
					return s * Upp::ipow10(e);
				}
				else
				{
					s = -s;
					if     ( (s>2) && (s<=5)  ) { s = 5; }
					else if( (s>1) && (s<=2)  ) { s = 2; }
					else if( (s>5) && (s<=10) ) { s = 1; e++; }
					else   { s = 1; }
					return -s * Upp::ipow10(e);
				}
			}

			
			typename COORDCONVERTER::TypeGraphCoord  GetGridStartValue(typename COORDCONVERTER::TypeGraphCoord  pstepValue, typename COORDCONVERTER::TypeGraphCoord pgraphMin)
			{
				typename COORDCONVERTER::TypeGraphCoord res;
				long double stepValue = pstepValue;
				long double graphMin = pgraphMin;
				if (graphMin>=0) {
					res = ((int)(graphMin/stepValue + 1 - std::numeric_limits<typename COORDCONVERTER::TypeGraphCoord>::epsilon()) ) * stepValue;
				} else {
					res = (((int)(graphMin/stepValue) ) * stepValue);
				}
				if (res < pgraphMin) res = pgraphMin;
				return res;
			}


		private:
			// explicitly forbidden to use
			CLASSNAME& operator=( const CLASSNAME& v) {}
			GridStepManager() {}

	};
};
#endif
