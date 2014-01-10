#ifndef _GraphDraw_StdElements_h_
#define _GraphDraw_StdElements_h_


#include "GraphElement.h"
#include "TickMark.h"

namespace GraphDraw_ns
{
	// ============================
	//    LabelElement   CLASS
	// ============================
	class LabelElement : public GraphElement
	{
		public:
		Upp::String _label;
		Font        _font;
		Color       _color;
		typedef GraphElement  _B;

		public:
		LabelElement() : _color( Blue() ) {}
		LabelElement(LabelElement& p) : _B(p), _color( p._color ) {}
		virtual ~LabelElement() {}

//		virtual LabelElement* Clone() { return new LabelElement(*this); };

		template<class T>	inline LabelElement& SetLabel(T& v) { _label = v; return *this; }
		template<class T>	inline LabelElement& SetFont(T& v) { _font = v; return *this; }
		template<class T>	inline LabelElement& SetTextColor(T v) { _color = v; return *this; }

		inline const Upp::String& GetLabel() const { return _label; }

		virtual void PaintElement(Draw& dw, int scale)
		{
			_B::PaintElementBckGround(dw, _B::GetFrame().GetSize() );
			
			Font scaledFont( _font );
			scaledFont.Height(scale*scaledFont.GetHeight());
			Size sz = GetTextSize(_label, scaledFont);
			switch(_B::GetElementPos()){
				case LEFT_OF_GRAPH:
					dw.DrawText( _B::GetElementWidth()*scale/2-sz.cy/2, _B::GetFrame().GetHeight()/2+sz.cx/2 , 900, _label, scaledFont, _color);
					break;
				case BOTTOM_OF_GRAPH:
					dw.DrawText( _B::GetFrame().GetWidth()/2-sz.cx/2, _B::GetElementWidth()*scale/2-sz.cy/2 , _label, scaledFont, _color);
					break;
				case TOP_OF_GRAPH:
					dw.DrawText( _B::GetFrame().GetWidth()/2-sz.cx/2, _B::GetElementWidth()*scale/2-sz.cy/2 , _label, scaledFont, _color);
					break;
				case RIGHT_OF_GRAPH:
					dw.DrawText( _B::GetElementWidth()*scale/2+sz.cy/2, _B::GetFrame().GetHeight()/2-sz.cx/2 , 2700, _label, scaledFont, _color);
					break;
				case FLOAT_OVER_GRAPH:
					break;
			}
		}
	};
//
//	// ============================
//	//    LabelElement   CLASS
//	// ============================
//	class RichLabelElement : public GraphElement
//	{
//		private:
//		RichText    _label;
//		Color       _bckGndcolor;
//		typedef RichLabelElement                          CLASSANME;
//		typedef GraphElement _B;
//
//		public:
//		RichLabelElement() : _bckGndcolor(Null) {}
//		RichLabelElement(RichLabelElement& p) : _B(p), _bckGndcolor(p._bckGndcolor) {}
//		virtual ~RichLabelElement() {}
//
//		virtual CLASSANME* Clone() { return new CLASSANME(*this); };
//
//		template<class T>
//		inline void SetLabel(T& v) { _label = ParseQTF(v); }
////		inline const Upp::String& GetLabel() const { return _label.; }
//
//		virtual void PaintElement(Draw& dw, int scale)
//		{
//			if ( !_bckGndcolor.IsNullInstance() )
//				dw.DrawRect( 0, 0, _B::GetFrame().GetWidth(), _B::GetFrame().GetHeight(), _bckGndcolor);
//
//			//Size sz(0,0);
//			switch(_B::GetElementPos()){
//				case LEFT_OF_GRAPH:
//
//					//dw.DrawText( _B::GetElementWidth()*scale/2-sz.cy/2, _B::GetFrame().GetHeight()/2+sz.cx/2 , 900, _label, _font, _color);
//					break;
//				case BOTTOM_OF_GRAPH:
//					//dw.DrawText( _B::GetFrame().GetWidth()/2-sz.cx/2, _B::GetElementWidth()*scale/2-sz.cy/2 , _label, _font, _color);
//					break;
//				case TOP_OF_GRAPH:
//					_label.Paint(dw, 0, 0, 1000);
//					//dw.DrawText( _B::GetFrame().GetWidth()/2-sz.cx/2, _B::GetElementWidth()*scale/2-sz.cy/2 , _label, _font, _color);
//					break;
//				case RIGHT_OF_GRAPH:
//					//dw.DrawText( _B::GetElementWidth()*scale/2+sz.cy/2, _B::GetFrame().GetHeight()/2-sz.cx/2 , 2700, _label, _font, _color);
//					break;
//			}
//		}
//	};



	// ============================
	//    LegendElement   CLASS
	// ============================
	template<class TYPES >
	class LegendElement : public GraphElement
	{
		public:
		typedef LegendElement CLASSNAME;
		typedef GraphElement _B;

		String _legend;
		Font   _font;
		int    _xSeparation; // separation between two legends
		int    _legendStyleLength;
		int    _legendWeight;
		TypeVectorSeries* v_series;

		public:
		LegendElement()
		: _font(StdFont())
		, _xSeparation(20)
		, _legendStyleLength(23)
		, v_series(0)
		{}
		
		LegendElement(LegendElement& p)
		: _B(p)
		, _font(p._font)
		, _xSeparation(p._xSeparation)
		, _legendStyleLength(p._legendStyleLength)
		, v_series(p.v_series)
		{ Update(); }

		virtual ~LegendElement() {}

		template<class T>
		inline CLASSNAME& SetLegend(T& v) { _legend = v; return *this; }

		inline CLASSNAME&  SetFont(Font  v)         { _font  = v;  return *this; }

		virtual void PaintElement(Draw& dw, int scale) {
			_B::PaintElementBckGround(dw, _B::GetFrame().GetSize() );
			DrawLegend(dw, scale);
		}

		virtual void PaintFloatElement(Draw& dw, int scale){
			_B::PaintElementBckGround(dw, _B::GetFloatFrame().GetSize()*scale );
			DrawLegend(dw, scale);
		}

		virtual void Update() {
			if (v_series==0) {
				v_series = &_B::_parent->GetSeries();
			}

			_legendWeight = 0;
			for (int c=0; c<(*v_series).GetCount(); ++c) {
				int textLength = GetTextSize((*v_series)[c].legend, _font).cx;
				if(_legendWeight < textLength) _legendWeight = textLength;
			}
			_legendWeight += _legendStyleLength + _xSeparation;
		}

		void DrawLegend(Draw& w, const int scale) const
		{
			if (v_series==0) {
				String text = "LEGEND TEXT : no series defined";
				w.DrawText( 0,0 , text, _font, Black());
				return;
			}

			int nmr = fround(_B::_frame.GetSize().cx/(_legendWeight*scale));	//max number of labels per row
			if (nmr <= 0) nmr = 1;
			int nLab = (*v_series).GetCount();	//number of labels
			int idx=0;
			int ix=0;
			int iy=0;
			Font scaledFont( _font );
			int txtHeight = scaledFont.Height(scale*_font.GetHeight()).GetHeight();
			while (idx<nLab) {
				while ((idx<nLab) && ((*v_series)[idx].show == false )) { ++idx; }
				
				if (idx<nLab) {
					int x = scale*ix*_legendWeight + txtHeight/2;
					int y = iy*txtHeight + txtHeight/2;
					Image img = (*v_series)[idx].MakeSerieIcon(txtHeight, scale);
					w.DrawImage(x,y, img);
					DrawText(w, x+scale*(_legendStyleLength+2), y, 0, (*v_series)[idx].legend, scaledFont,
					         (  ((*v_series)[idx].seriesPlot.IsEmpty()) ? (*v_series)[idx].markColor : (*v_series)[idx].color ) );
					++idx;
				}
				++ix;
				if (ix>=nmr) { ix=0; ++iy; }
			}
		}
	};
	
	// ============================
	//    MarkerElement   CLASS
	// ============================
	class MarkerElementData : Moveable<MarkerElementData> {
		private:
		TickMark* tickMark;
		TypeGraphCoord pos;
		int ID;
		
		MarkerElementData(const MarkerElementData& p) : tickMark(0), pos(p.pos), ID(-1) {} // explicitally forbidden
		
		public:
		MarkerElementData() : tickMark(0), ID(-1) {}
		~MarkerElementData() {
			if (tickMark) delete tickMark;
			tickMark = 0;
			ID=-1;
		}
		
		TickMark& GetTickMark() const { return *tickMark; }
		TypeGraphCoord GetPos() const { return pos; }
		
		operator TypeGraphCoord() const { return pos; }
		operator TickMark&()      const { return *tickMark;}
		
		MarkerElementData& operator=(const TypeGraphCoord v) { pos = v; return *this; }
		int GetID() { return ID; }
		
		template <class T>
		T& Create(int v) {
			ID = v;
			if (tickMark) delete tickMark;
			T& tick = * new T();
			tickMark = &tick;
			return tick;
		}
	};
	
	typedef ArrayMap<int, MarkerElementData> MarkerPosList;
	typedef Callback2< const MarkerPosList&, int> TypeMarkerMoveCbk;

	template<class TYPES>
	class MarkerElement : public GraphElement
	{
		public:
		CoordinateConverter& _coordConverter;
		Font        _font;
		Color       _color;
		int         _axisWidth;
		typedef GraphElement  _B;
		typedef MarkerElement<TYPES>  CLASSNAME;
		MarkerPosList markers;
		

		public:

		MarkerElement(CoordinateConverter& coordconv)
		: _coordConverter(coordconv)
		, _color( Red() )
		, _axisWidth(2)
		{
			_B::DisablePos(FLOAT_OVER_GRAPH);
		}
		
		MarkerElement(MarkerElement& p)
		: _B(p)
		, _coordConverter(p._coordConverter)
		, _color( p._color )
		, _axisWidth(p._axisWidth)
		{
			_B::DisablePos(FLOAT_OVER_GRAPH);
		}
		
		virtual ~MarkerElement() {}

//		virtual MarkerElement* Clone() { return new MarkerElement(*this); };

		template<class T>	inline MarkerElement& SetFont(T& v)       { _font = v; return *this; }
		template<class T>	inline MarkerElement& SetTextColor(T v)   { _color = v; return *this; }

		template <class MARKER_TYPE>
		MARKER_TYPE&  AddMarker(int markerId, TypeGraphCoord pos) {
			MarkerElementData& markData = markers.Add(markerId);
			markData = pos;
			return markData.Create<MARKER_TYPE>(markerId);
		}

		void ClearMarkers() {
			markers.Clear();
		}

		virtual void  SetElementPos(ElementPosition v) {
			if (v==LEFT_OF_GRAPH || v==RIGHT_OF_GRAPH) {
				_B::DisablePos(BOTTOM_OF_GRAPH);
				_B::DisablePos(TOP_OF_GRAPH);
			} else {
				_B::DisablePos(LEFT_OF_GRAPH);
				_B::DisablePos(RIGHT_OF_GRAPH);
			}
			_B::SetElementPos(v);
		 }


		virtual void PaintHLine(Draw& dw, int range)
		{
			if ( !_color.IsNullInstance())
			{
				MarkerPosList::Iterator iter = markers.Begin();
				MarkerPosList::ConstIterator endIter =  markers.End();
				while( iter != endIter)
				{
					MarkerElementData& markerData = *iter;
					if (_coordConverter.IsInGraphVisibleRange(markerData)) {
						dw.DrawLineOp( 0, _coordConverter.toScreen(markerData), range, _coordConverter.toScreen(markerData), 1, _color );
					}
					++iter;
				}
			}
		}

		virtual void PaintVLine(Draw& dw, int range)
		{
			if ( !_color.IsNullInstance())
			{
				MarkerPosList::Iterator iter = markers.Begin();
				MarkerPosList::ConstIterator endIter =  markers.End();
				while( iter != endIter)
				{
					MarkerElementData& markerData = *iter;
					if (_coordConverter.IsInGraphVisibleRange(markerData)) {
						dw.DrawLineOp( _coordConverter.toScreen(markerData), 0, _coordConverter.toScreen(markerData), range, 1, _color );
					}
					++iter;
				}
			}
		}

		virtual void PaintElement(Draw& dw, int scale)
		{
			_B::PaintElementBckGround(dw, _B::GetFrame().GetSize() );

			//dw.DrawLineOp(_B::GetElementWidth()*scale, _coordConverter.getScreenMin(), _B::GetElementWidth()*scale, _coordConverter.getScreenMax(), _axisWidth*scale, _color );
			MarkerPosList::Iterator iter = markers.Begin();
			MarkerPosList::ConstIterator endIter = markers.End();
			int c=0;
			while ( iter != endIter ) {
					MarkerElementData& markerData = *iter;
				if (_coordConverter.IsInGraphVisibleRange(markerData)) {
					switch( _B::GetElementPos() ) {
						case LEFT_OF_GRAPH:
							markerData.GetTickMark().Paint(dw, _B::GetElementPos(), scale, _B::GetElementWidth()*scale, _coordConverter.toScreen(markerData), _color);
							break;
						case BOTTOM_OF_GRAPH:
							markerData.GetTickMark().Paint(dw, _B::GetElementPos(), scale, _coordConverter.toScreen(markerData), 0, _color );
							break;
						case TOP_OF_GRAPH:
							markerData.GetTickMark().Paint(dw, _B::GetElementPos(), scale,  _coordConverter.toScreen(markerData), _B::GetElementWidth()*scale, _color );
							break;
						case RIGHT_OF_GRAPH:
							markerData.GetTickMark().Paint(dw, _B::GetElementPos(), scale, 0, _coordConverter.toScreen(markerData), _color );
							break;
						case FLOAT_OVER_GRAPH:
							break;
					}
				}
				++iter;
				++c;
			}
		}
		
		virtual void PaintOnPlot_overData(Draw& dw, int otherWidth, int scale)
		{
			switch( _B::GetElementPos() )
			{
				case LEFT_OF_GRAPH:
				case RIGHT_OF_GRAPH:
					PaintHLine(dw, otherWidth);
					break;
				case TOP_OF_GRAPH:
				case BOTTOM_OF_GRAPH:
					PaintVLine(dw, otherWidth);
					break;
				case FLOAT_OVER_GRAPH:
					break;
			}
			
		}
	};
}
#endif
