#ifndef _GraphDraw_StdElements_h_
#define _GraphDraw_StdElements_h_


#include "GraphElement.h"
#include "TickMark.h"

namespace GraphDraw_ns
{
	// ============================
	//    LabelElement   CLASS
	// ============================
	class LabelElement : public CRTPGraphElementFrame< LabelElement >
	{
		public:
		Upp::String _label;
		Font        _font;
		Color       _color;
		Color       _bckGndcolor;
		typedef CRTPGraphElementFrame< LabelElement >  _B;

		public:
		LabelElement() : _color( Blue() ), _bckGndcolor(Null) {}
		LabelElement(LabelElement& p) : _B(p), _color( p._color ), _bckGndcolor(p._bckGndcolor) {}
		virtual ~LabelElement() {}

		virtual LabelElement* Clone() { return new LabelElement(*this); };

		template<class T>	inline LabelElement& SetLabel(T& v) { _label = v; return *this; }
		template<class T>	inline LabelElement& SetFont(T& v) { _font = v; return *this; }
		template<class T>	inline LabelElement& SetTextColor(T v) { _color = v; return *this; }
		template<class T>	inline LabelElement& SetBckGndColor(T v) { _bckGndcolor = v; return *this; }

		inline const Upp::String& GetLabel() const { return _label; }

		virtual void PaintElement(Draw& dw, int scale)
		{
			if ( !_bckGndcolor.IsNullInstance() )
				dw.DrawRect( 0, 0, _B::GetFrame().GetWidth(), _B::GetFrame().GetHeight(), _bckGndcolor);

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
				case OVER_GRAPH:
					break;
			}
		}
	};
//
//	// ============================
//	//    LabelElement   CLASS
//	// ============================
//	class RichLabelElement : public CRTPGraphElementFrame< RichLabelElement >
//	{
//		private:
//		RichText    _label;
//		Color       _bckGndcolor;
//		typedef RichLabelElement                          CLASSANME;
//		typedef CRTPGraphElementFrame< RichLabelElement > _B;
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
	class LegendElement : public CRTPGraphElementFrame< LegendElement<TYPES> >
	{
		public:
		typedef LegendElement CLASSNAME;
		typedef CRTPGraphElementFrame< LegendElement > _B;
		typedef typename TYPES::TypeVectorSeries TypeVectorSeries;

		String _legend;
		RGBA   _bckGndRgba;
		bool   _isRgba;
		Font   _font;
		int    _xSeparation; // separation between two legends
		int    _legendStyleLength;
		int    _legendWeight;
		TypeVectorSeries* v_series;

		public:
		LegendElement()
		: _isRgba(false)
		, _font(StdFont())
		, _xSeparation(20)
		, _legendStyleLength(23)
		, v_series(0)
		{}
		
		LegendElement(LegendElement& p)
		: _B(p)
		, _bckGndRgba(p._bckGndRgba)
		, _isRgba(p._isRgba)
		, _font(p._font)
		, _xSeparation(p._xSeparation)
		, _legendStyleLength(p._legendStyleLength)
		, v_series(p.v_series)
		{ Update(); }

		virtual ~LegendElement() {}

		virtual CLASSNAME* Clone() { return new CLASSNAME(*this); };

		template<class T>
		inline CLASSNAME& SetLegend(T& v) { _legend = v; return *this; }

		inline CLASSNAME&  SetBackGndColor(Color v) { _B::SetBackgndStyle(v); _isRgba=false; return *this; }
		inline CLASSNAME&  SetBackGndColor(RGBA  v) { _bckGndRgba = v; _isRgba=true;  return *this; }
		inline CLASSNAME&  SetFont(Font  v)         { _font  = v;  return *this; }

		virtual void PaintElement(Draw& dw, int scale) {
			if      ( !_B::_backgndStyle.IsNull() ) {
				ChPaint(dw, Rect(0, 0, _B::GetFrame().GetWidth(), _B::GetFrame().GetHeight()), _B::_backgndStyle );
			}
			else if (_isRgba){
				Image img = CreateImage(_B::GetFrame().GetSize(), _bckGndRgba); // no SCALING needed here
				dw.DrawImage( 0,0, img );
			}
			DrawLegend(dw, scale);
		}

		virtual void PaintFloatElement(Draw& dw, int scale){
			if      ( !_B::_backgndStyle.IsNull() ) {
				ChPaint(dw, Rect(0, 0, _B::GetFrame().GetWidth(), _B::GetFrame().GetHeight()), _B::_backgndStyle );
			}
			else if (_isRgba){
				Image img = CreateImage(_B::GetFloatFrame(scale).GetSize(), _bckGndRgba);
				dw.DrawImage( 0,0, img );
			}
			DrawLegend(dw, scale);
		}

		virtual void Update() {
			if (v_series==0) {
				typedef typename TYPES::TypeVectorSeries*  PtrTypeVectorSeries;
				v_series = _B::_parent->GetSeries().template To<PtrTypeVectorSeries>();
				// ###### BUG DE CONVERSION ARM ######
			}

			_legendWeight = 0;
			for (int c=0; c<(*v_series).GetCount(); ++c) {
				int textLength = GetTextSize((*v_series)[c].legend, _font).cx;
				if(_legendWeight < textLength) _legendWeight = textLength;
			}
			_legendWeight += _legendStyleLength + _xSeparation;
		}

		void DrawLegend(Draw& w, const int& scale) const
		{
			if (v_series==0) {
				String text = "This is the legend !!";
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
					Image img = (*v_series)[idx].MakeSerieIcon(txtHeight);
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
		
		MarkerElementData(const MarkerElementData& p) : tickMark(0), pos(p.pos) {} // prohibited
		
		public:
		MarkerElementData() : tickMark(0) {}

		TypeGraphCoord pos;
		
		TickMark& GetTickMark() { return *tickMark; }
		
		template <class T>
		T& Create() {
			if (tickMark) delete tickMark;
			T& tick = * new T();
			tickMark = &tick;
			return tick;
		}
	};
	typedef Array<MarkerElementData> MarkerPosList;

	template<class TYPES>
	class MarkerElement : public CRTPGraphElementFrame< MarkerElement<TYPES> >
	{
		public:
		typedef typename TYPES::TypeCoordConverter   TypeCoordConverter;
		TypeCoordConverter& _coordConverter;
		Font        _font;
		Color       _color;
		Color       _bckGndcolor;
		int         _axisWidth;
		typedef CRTPGraphElementFrame< MarkerElement >  _B;
		typedef MarkerElement<TYPES>  CLASSNAME;
		
		MarkerPosList markers;
		

		public:
		MarkerElement(TypeCoordConverter& coordconv)
		: _coordConverter(coordconv)
		, _color( Red() )
		, _bckGndcolor(Null)
		, _axisWidth(2)
		{
			_B::DisablePos(OVER_GRAPH);
		}
		
		MarkerElement(MarkerElement& p)
		: _B(p)
		, _coordConverter(p._coordConverter)
		, _color( p._color )
		, _bckGndcolor(p._bckGndcolor)
		, _axisWidth(p._axisWidth)
		{
			_B::DisablePos(OVER_GRAPH);
		}
		
		virtual ~MarkerElement() {}

		virtual MarkerElement* Clone() { return new MarkerElement(*this); };

		template<class T>	inline MarkerElement& SetFont(T& v)       { _font = v; return *this; }
		template<class T>	inline MarkerElement& SetTextColor(T v)   { _color = v; return *this; }
		template<class T>	inline MarkerElement& SetBckGndColor(T v) { _bckGndcolor = v; return *this; }

		template <class MARKER_TYPE>
		MARKER_TYPE&  AddMarker(TypeGraphCoord pos) {
			MarkerElementData& markData = markers.Add();
			markData.pos = pos;
			return markData.Create<MARKER_TYPE>();
		}

		void ClearMarkers() {
			markers.Clear();
		}

		virtual CLASSNAME&  SetElementPos(ElementPosition v) {
			if (v==LEFT_OF_GRAPH || v==RIGHT_OF_GRAPH) {
				_B::DisablePos(BOTTOM_OF_GRAPH);
				_B::DisablePos(TOP_OF_GRAPH);
			} else {
				_B::DisablePos(LEFT_OF_GRAPH);
				_B::DisablePos(RIGHT_OF_GRAPH);
			}
			return _B::SetElementPos(v);
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
					if (_coordConverter.IsInGraphVisibleRange(markerData.pos)) {
						dw.DrawLineOp( 0, _coordConverter.toScreen(markerData.pos), range, _coordConverter.toScreen(markerData.pos), 2, _color );
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
					if (_coordConverter.IsInGraphVisibleRange(markerData.pos)) {
						dw.DrawLineOp( _coordConverter.toScreen(markerData.pos), 0, _coordConverter.toScreen(markerData.pos), range, 2, _color );
					}
					++iter;
				}
			}
		}

		virtual void PaintElement(Draw& dw, int scale)
		{
			if ( !_bckGndcolor.IsNullInstance() ) {
				dw.DrawRect( 0, 0, _B::GetFrame().GetWidth(), _B::GetFrame().GetHeight(), _bckGndcolor);
			}

			//dw.DrawLineOp(_B::GetElementWidth()*scale, _coordConverter.getScreenMin(), _B::GetElementWidth()*scale, _coordConverter.getScreenMax(), _axisWidth*scale, _color );
			MarkerPosList::Iterator iter = markers.Begin();
			MarkerPosList::ConstIterator endIter = markers.End();
			int c=0;
			while ( iter != endIter ) {
					MarkerElementData& markerData = *iter;
				if (_coordConverter.IsInGraphVisibleRange(markerData.pos)) {
					switch( _B::GetElementPos() ) {
						case LEFT_OF_GRAPH:
							markerData.GetTickMark().Paint(dw, _B::GetElementPos(), scale, _B::GetElementWidth(), _coordConverter.toScreen(markerData.pos), _color);
							break;
						case BOTTOM_OF_GRAPH:
							markerData.GetTickMark().Paint(dw, _B::GetElementPos(), scale, _coordConverter.toScreen(markerData.pos), 0, _color );
							break;
						case TOP_OF_GRAPH:
							markerData.GetTickMark().Paint(dw, _B::GetElementPos(), scale,  _coordConverter.toScreen(markerData.pos), _B::GetElementWidth(), _color );
							break;
						case RIGHT_OF_GRAPH:
							markerData.GetTickMark().Paint(dw, _B::GetElementPos(), scale, 0, _coordConverter.toScreen(markerData.pos), _color );
							break;
						case OVER_GRAPH:
							break;
					}
				}
				++iter;
				++c;
			}
		}
		
		virtual void PaintOnPlot(Draw& dw, int otherWidth, int scale)
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
				case OVER_GRAPH:
					break;
			}
			
		}
	};
}
#endif
