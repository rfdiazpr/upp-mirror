#ifndef _GraphCtrl_SeriesConfig_h_
#define _GraphCtrl_SeriesConfig_h_

#include <ScatterDraw/SeriesPlot.h>
#include <ScatterDraw/MarkPlot.h>
#include <DataSource/DataSource.h>

namespace GraphDraw_ns
{

	template <class TYPES>
	class SeriesConfigBase {
		public:
			bool sequential;
			unsigned int sequentialPointsRangeMin;
			unsigned int sequentialPointsRangeMax;

			unsigned int nbVisiblePoints;

			One<typename TYPES::TypeSeriesPlot> seriesPlot;
			double thickness;
			Color color;
			String dash;

			One<typename TYPES::TypeMarkPlot> markPlot;
			double markWidth;
			Color markColor;
			Color fillColor;

			String legend;

			double opacity;
			int id;
			bool trendLine;

			typename TYPES::TypeCoordConverter* xConverter;
			typename TYPES::TypeCoordConverter* yConverter;


			SeriesConfigBase()
			{
				color = Null;
				thickness = 3;
				legend = "";
				opacity = 1;
				sequential = true;
				sequentialPointsRangeMin = 0;
				sequentialPointsRangeMax = 0;
				nbVisiblePoints = 0;
				dash = LINE_SOLID;
				seriesPlot;// = new LineSeriesPlot();
				markPlot;  // = new CircleMarkPlot();
				markWidth = 8;
				markColor = Null;
				fillColor = Null;
				xConverter = 0;
				yConverter = 0;
			}

			void Init(int id)
			{
				color = GetNewColor(id);
				markColor = Color(max(color.GetR()-30, 0), max(color.GetG()-30, 0), max(color.GetB()-30, 0));
				//dash = GetNewDash(id);
			}
	};

	template <class TYPES>
	class SeriesConfig : public Moveable< SeriesConfig<TYPES> >, public SeriesConfigBase<TYPES> {
		public:
		SeriesConfig()	{pD = 0;}
		void SetDataSource(typename TYPES::TypeDataSource *pointsData, bool ownsData = true) {pD = pointsData; owns = ownsData;}
		inline typename TYPES::TypeDataSource *PointsData()	{return pD;}
		~SeriesConfig()	{if(pD && owns) delete pD;}

		private:
		typename TYPES::TypeDataSource *pD;
		bool owns;
	};
};
#endif
