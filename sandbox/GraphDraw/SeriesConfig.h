#ifndef _GraphCtrl_SeriesConfig_h_
#define _GraphCtrl_SeriesConfig_h_

#include <ScatterDraw/SeriesPlot.h>
#include <ScatterDraw/MarkPlot.h>
#include <ScatterDraw/DataSource.h>

namespace GraphDraw_ns
{

	class SeriesConfigBase {
		public:
			bool sequential;
			unsigned int sequentialPointsRangeMin;
			unsigned int sequentialPointsRangeMax;

			unsigned int nbVisiblePoints;

			String legend;
			int id;
			CoordinateConverter* xConverter;
			CoordinateConverter* yConverter;
			bool show;
			
			// plot
			One<SeriesPlot> seriesPlot;
			double thickness;
			Color color;
			String dash;

			// Mark
			One<MarkPlot> markPlot;
			double markWidth;
			Color markColor;
			
			// Filling
			Color fillColor;
			double opacity;
			
			Image serieIcon; // image shown in LEGENDs & MENUs

			SeriesConfigBase()
			{
				color = Null;
				thickness = 1;
				legend = "";
				opacity = 1;
				sequential = true;
				sequentialPointsRangeMin = 0;
				sequentialPointsRangeMax = 0;
				nbVisiblePoints = 0;
				dash = LINE_SOLID;
//				seriesPlot;// = new LineSeriesPlot();
//				markPlot;  // = new CircleMarkPlot();
				markWidth = 8;
				markColor = Null;
				fillColor = Null;
				xConverter = 0;
				yConverter = 0;
				show = true;
			}

			void Init(int id, bool setDefaultStyles)
			{
				color = GetNewColor(id);
				markColor = Color(max(color.GetR()-30, 0), max(color.GetG()-30, 0), max(color.GetB()-30, 0));
				if (setDefaultStyles) SetDefaultStyles(id);
			}

			void SetDefaultStyles(int id)
			{
				dash = GetNewDash(id);
				markPlot = GetNewMarkPlot(id);
				seriesPlot = GetNewPlotStyle(id);
			}

			Image MakeSerieIcon( int width=16 ) {
				RGBA bckgColor;   bckgColor.r = 0; bckgColor.g = 0; bckgColor.b = 0; bckgColor.a = 0;
				
				ImageBuffer ib( Size(width,width) );
				Fill( ib.Begin(), bckgColor, ib.GetLength() );

				Vector<Point> p1;
				const int scale=1;
				int painterMode = MODE_ANTIALIASED;
				if ( !seriesPlot.IsEmpty() && !color.IsNullInstance() ) {
					// draw multiple points ONLY if lines are drawn 
					p1 << Point((width*2)/32,(width*2)/32) << Point((width*10)/32,(width*20)/32) << Point((width*20)/32,(width*12)/32) << Point((width*30)/32,(width*30)/32);
				}
				else {
					// lines not drawn ==> only one point in serie ( CENTERED )
					p1 << Point(width/2,width/2);
					painterMode = MODE_SUBPIXEL;
				}

				BufferPainter dw(ib, painterMode );
				
				// Draw lines
				if ( !seriesPlot.IsEmpty() ) {
					seriesPlot->Paint(dw,
	                                 p1,
	                                 scale,
	                                 opacity,
	                                 fround(thickness),
	                                 color,
	                                 dash,
	                                 Null,
	                                 fillColor,
	                                 1,
	                                 1,
	                                 32
					                 );
				}
				// Draw marks
				if ( markWidth >= 1 && markPlot && !markPlot.IsEmpty()) {
					for (int c=0; c<p1.GetCount(); ++c)
					{
						markPlot->Paint(dw,
		                               scale,
		                               p1[c],
		                               markWidth,
		                               markColor);
					}
				}
				return ib;
			}
			
			const Image& UpdateSerieImage() {
				serieIcon = MakeSerieIcon();
				return serieIcon;
			}
	
			inline const Image& GetSerieIcon() { return serieIcon; }
	};

	class SeriesConfig : public Moveable< SeriesConfig >, public SeriesConfigBase {
		public:
		SeriesConfig()	{pD = 0;}
		void SetDataSource(DataSource *pointsData, bool ownsData = true) {pD = pointsData; owns = ownsData;}
		inline DataSource *PointsData()	{return pD;}
		~SeriesConfig()	{if(pD && owns) delete pD;}

		private:
		DataSource* pD;
		bool owns;
	};
	
	typedef Vector<SeriesConfig>  TypeVectorSeries;
};
#endif
