/*
 * GraphElement.h
 *
 *  Created on: 6 avr. 2012
 *      Author: didier
 */

#ifndef GRAPHELEMENT_H_
#define GRAPHELEMENT_H_



namespace GraphDraw_ns
{
	typedef enum {
		LEFT_OF_GRAPH  = 0x001,
		RIGHT_OF_GRAPH = 0x002,
		TOP_OF_GRAPH   = 0x010,
		BOTTOM_OF_GRAPH= 0x020,
		OVER_GRAPH     = 0x100
	} ElementPosition;

	enum {
		VERTICAL_MASK  = 0x00F,
		HORIZONTAL_MASK = 0x0F0,
		OVER_MASK       = 0xF00
	};

	typedef enum {
			REFRESH_FAST = 0,
			REFRESH_TOTAL
	} RefreshStrategy;


	class GraphElementParent {
		public:
			typedef GraphElementParent CLASSNAME;
			typedef int TypeScreenCoord;
			virtual CLASSNAME& ScrollX( TypeScreenCoord xOffset) = 0;
			virtual CLASSNAME& ScrollY( TypeScreenCoord yOffset) = 0;
			virtual CLASSNAME& ZoomX(TypeScreenCoord left, TypeScreenCoord right) = 0;
			virtual CLASSNAME& ZoomY(TypeScreenCoord top, TypeScreenCoord bottom) = 0;
			virtual void RefreshFromChild( RefreshStrategy doFastPaint ) = 0;

			GraphElementParent() {}
			virtual ~GraphElementParent() {}
	};


	// ============================
	//    GraphElementFrame   CLASS
	// ============================
	class GraphElementFrame {
		protected:
			Rect            _frame;  // Frame on which element is painted (absolute position in complete draw area)
			int             _width;  // width of GraphElement (in screen size)
			ElementPosition _pos;    // position to plot area

		public:
			GraphElementParent* _parent;
			Callback3<GraphElementFrame*, Point , dword > WhenLeftDown     ;
			Callback3<GraphElementFrame*, Point , dword > WhenLeftUp       ;
			Callback3<GraphElementFrame*, Point , dword > WhenLeftDouble   ;
			Callback3<GraphElementFrame*, Point , dword > WhenLeftDrag     ;
			Callback3<GraphElementFrame*, Point , dword > WhenRightDown    ;
			Callback3<GraphElementFrame*, Point , dword > WhenRightUp      ;
			Callback3<GraphElementFrame*, Point , dword > WhenRightDouble  ;
			Callback3<GraphElementFrame*, Point , dword > WhenRightDrag    ;
			Callback3<GraphElementFrame*, Point , dword > WhenMiddleDown   ;
			Callback3<GraphElementFrame*, Point , dword > WhenMiddleUp     ;

			GraphElementFrame() : _width(5), _pos(LEFT_OF_GRAPH) {}
			GraphElementFrame(GraphElementFrame* p)
			: _width(p->_width)
			, _pos(LEFT_OF_GRAPH)
			, _parent(p->_parent)
			, WhenLeftDown(p->WhenLeftDown)
			, WhenLeftDouble  (p->WhenLeftDouble  )
			, WhenLeftDrag    (p->WhenLeftDrag    )
			, WhenRightDown   (p->WhenRightDown   )
			, WhenRightDouble (p->WhenRightDouble )
			, WhenRightDrag   (p->WhenRightDrag   )
			, WhenMiddleDown  (p->WhenMiddleDown  )
			{}

			virtual ~GraphElementFrame() {}

			virtual bool Contains(Point p) const { return _frame.Contains(p); }
			inline const Rect& GetFrame() const { return _frame; }
			inline void SetFrame(Rect v) { _frame = v; }
			inline int GetElementWidth() { return _width; }
			inline ElementPosition GetElementPos() { return _pos; }

			inline bool IsVertical() const { return ((_pos & GraphDraw_ns::VERTICAL_MASK)!=0); }
			inline bool IsHorizontal() const { return ((_pos & GraphDraw_ns::HORIZONTAL_MASK)!=0); }
			inline bool IsOverGraph() const { return ((_pos & GraphDraw_ns::OVER_MASK)!=0); }

         // Paint element somewhere inside the graph area (legend, ...)
			// Offset and clipping are set with the '_frame' settings
			virtual void PaintOverGraph(Draw& dw, int scale) { };

			// Paint the element in his own area
			// There is no clipping ==> drawing can overlap plot or other elements
			virtual void PaintElement(Draw& dw, int scale) = 0;

			// Paint additionnal element stuff on PLOT AREA : grids, square zones,  anything you wan't
			// Painting zone is clipped so nothing can be drawn outside
			virtual void PaintOnPlot(Draw& dw, int otherWidth, int scale) {}
			virtual void Update() {}; // called when coordinates need update

			virtual GraphElementFrame* Clone() = 0;


			virtual GraphElementFrame* LeftDown   (Point p, dword keyflags) { WhenLeftDown   (this, p, keyflags); return 0;}
			virtual GraphElementFrame* LeftUp     (Point p, dword keyflags) { WhenLeftUp     (this, p, keyflags); return 0;}
			virtual GraphElementFrame* LeftDouble (Point p, dword keyflags) { WhenLeftDouble (this, p, keyflags); return 0;}
			virtual GraphElementFrame* LeftDrag   (Point p, dword keyflags) { WhenLeftDrag   (this, p, keyflags); return 0;}
			virtual GraphElementFrame* RightDown  (Point p, dword keyflags) { WhenRightDown  (this, p, keyflags); return 0;}
			virtual GraphElementFrame* RightUp    (Point p, dword keyflags) { WhenRightUp    (this, p, keyflags); return 0;}
			virtual GraphElementFrame* RightDouble(Point p, dword keyflags) { WhenRightDouble(this, p, keyflags); return 0;}
			virtual GraphElementFrame* RightDrag  (Point p, dword keyflags) { WhenRightDrag  (this, p, keyflags); return 0;}
			virtual GraphElementFrame* MiddleDown (Point p, dword keyflags) { WhenMiddleDown (this, p, keyflags); return 0;}
			virtual GraphElementFrame* MiddleUp (Point p, dword keyflags)   { WhenMiddleUp   (this, p, keyflags); return 0;}
			virtual GraphElementFrame* MouseMove  (Point p, dword keyflags) { return 0;}
			virtual GraphElementFrame* MouseWheel (Point p, int zdelta, dword keyflags) { return 0; };
			virtual Image  CursorImage(Point p, dword keyflags) { return GraphDrawImg::CROSS(); }
	};

	template<class DERIVED>
	class CRTPGraphElementFrame : public GraphElementFrame {
		public:
			CRTPGraphElementFrame() {}
			CRTPGraphElementFrame(CRTPGraphElementFrame& p) : GraphElementFrame(p) {}
			virtual ~CRTPGraphElementFrame() {}

			inline DERIVED&  SetElementWidth(int v) { _width = v; return *static_cast<DERIVED*>(this); }
			inline DERIVED&  SetElementPos(ElementPosition v) { _pos = v; return *static_cast<DERIVED*>(this); }
	};


	class BlankAreaElement : public CRTPGraphElementFrame< BlankAreaElement >
	{
		typedef BlankAreaElement CLASSNAME;
		public:
			BlankAreaElement() {}
			BlankAreaElement( BlankAreaElement& p) : CRTPGraphElementFrame< BlankAreaElement >(p) {}
			virtual ~BlankAreaElement() {}
			virtual void PaintElement(Draw& dw, int scale) { /* do noting */}
			virtual CLASSNAME* Clone() { return new CLASSNAME(*this); }
	};


	// ============================
	//    LabelElement   CLASS
	// ============================
	class LabelElement : public CRTPGraphElementFrame< LabelElement >
	{
		private:
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

			Size sz = GetTextSize(_label, _font);
			switch(_B::GetElementPos()){
				case LEFT_OF_GRAPH:
					dw.DrawText( _B::GetElementWidth()/2-sz.cy/2, _B::GetFrame().GetHeight()/2+sz.cx/2 , 900, _label, _font, _color);
					break;
				case BOTTOM_OF_GRAPH:
					dw.DrawText( _B::GetFrame().GetWidth()/2-sz.cx/2, _B::GetElementWidth()/2-sz.cy/2 , _label, _font, _color);
					break;
				case TOP_OF_GRAPH:
					dw.DrawText( _B::GetFrame().GetWidth()/2-sz.cx/2, _B::GetElementWidth()/2-sz.cy/2 , _label, _font, _color);
					break;
				case RIGHT_OF_GRAPH:
					dw.DrawText( _B::GetElementWidth()/2+sz.cy/2, _B::GetFrame().GetHeight()/2-sz.cx/2 , 2700, _label, _font, _color);
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
//					//dw.DrawText( _B::GetElementWidth()/2-sz.cy/2, _B::GetFrame().GetHeight()/2+sz.cx/2 , 900, _label, _font, _color);
//					break;
//				case BOTTOM_OF_GRAPH:
//					//dw.DrawText( _B::GetFrame().GetWidth()/2-sz.cx/2, _B::GetElementWidth()/2-sz.cy/2 , _label, _font, _color);
//					break;
//				case TOP_OF_GRAPH:
//					_label.Paint(dw, 0, 0, 1000);
//					//dw.DrawText( _B::GetFrame().GetWidth()/2-sz.cx/2, _B::GetElementWidth()/2-sz.cy/2 , _label, _font, _color);
//					break;
//				case RIGHT_OF_GRAPH:
//					//dw.DrawText( _B::GetElementWidth()/2+sz.cy/2, _B::GetFrame().GetHeight()/2-sz.cx/2 , 2700, _label, _font, _color);
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
		private:
		RichText    _legend;
		Color       _bckGndcolor;
		RGBA        _bckGndRgba;
		bool        _isRgba;
		typedef LegendElement CLASSNAME;
		typedef CRTPGraphElementFrame< LegendElement > _B;
		typedef typename TYPES::TypeVectorSeries TypeVectorSeries;

		TypeVectorSeries* series;
		public:
		LegendElement() : _bckGndcolor(Null), series(0), _isRgba(false) {}
		LegendElement(LegendElement& p) : _B(p), _bckGndcolor(p._bckGndcolor), series(p.series) {}
		virtual ~LegendElement() {}

		virtual CLASSNAME* Clone() { return new CLASSNAME(*this); };

		template<class T>
		inline CLASSNAME& SetLegend(T& v) { _legend = ParseQTF(v); return *this; }
		//		inline const Upp::String& GetLegend() const { return AsQTF(_legend); }

		inline CLASSNAME& SetSeries(TypeVectorSeries& v) { series = &v; return *this; }

		inline CLASSNAME&  SetBackGndColor(Color v) { _bckGndcolor = v; _isRgba=false; return *this; }
		inline CLASSNAME&  SetBackGndColor(RGBA  v) { _bckGndRgba  = v; _isRgba=true;  return *this; }
		inline const Color& GetBackGndColor() const { return _bckGndcolor; }

		virtual void PaintElement(Draw& dw, int scale){
			if ( !_bckGndcolor.IsNullInstance() ) {
				dw.DrawRect( 0, 0, _B::GetFrame().GetWidth(), _B::GetFrame().GetHeight(), _bckGndcolor);
			}
			else if (_isRgba){
				Image img = CreateImage(_B::GetFrame().GetSize(), _bckGndRgba);
				dw.DrawImage( 0,0, img );
			}
			DrawLegend(dw, scale);
		}

		virtual void PaintOverGraph(Draw& dw, int scale){
			if ( !_bckGndcolor.IsNullInstance() )
				dw.DrawRect( 0, 0, _B::GetFrame().GetWidth(), _B::GetFrame().GetHeight(), _bckGndcolor);
			else if (_isRgba){
				Image img = CreateImage(_B::GetFrame().GetSize(), _bckGndRgba);
				dw.DrawImage( 0,0, img );
			}
			DrawLegend(dw, scale);
		}

		void DrawLegend(Draw& w, const int& scale) const
		{
			if (series==0) {
				String text = "This is the legend !!";
				w.DrawText( 0,0 , text, StdFont(), Black());
				return;
			}

			int legendWeight = 80;
			int nmr = fround(_B::_frame.GetSize().cx/legendWeight);	//max number of labels per row
			if (nmr <= 0)
				return;
			int nLab = (*series).GetCount();	//number of labels
			int Nc;							//number of complete rows
			int LCR;		  				//number of labels on complete row
			int R;							//number of remaining labels on incomplete row
			if(nmr > nLab) {
				Nc = 0;      	LCR = 0; 	R = nLab;
			} else if (nmr == nLab) {
				Nc = 1;      	LCR = nLab; R = 0;
			} else {
				Nc = nLab/nmr;  LCR = nmr;	R = nLab%nmr;
			}
			for(int j = 0; j <= Nc; j++) {
				int start = nLab - (j+1)*LCR;
				int end = nLab - j*LCR;
				if (j == Nc) {
					start = 0;
					end = R;
				}
				Font scaledFont;
				int txtHeight = scaledFont.Height(scale*StdFont().GetHeight()).GetHeight();
				for(int i = start; i < end; i++) {
					int x = scale*(i-start)*legendWeight;
					int y = scale*(txtHeight+txtHeight/2)*j;

					Vector <Point> vp;
					vp << Point(x,y+txtHeight/2) <<
							Point(x+scale*23, y+txtHeight/2);
					if ((*series)[i].opacity > 0 && (*series)[i].seriesPlot)
						DrawPolylineOpa(w, vp, scale, 1, scale*(*series)[i].thickness, (*series)[i].color, (*series)[i].dash);

					Point p(scale*(x+7),y+txtHeight/2);
					if ((*series)[i].markWidth >= 1 && (*series)[i].markPlot)
						(*series)[i].markPlot->Paint(w, scale, p, (*series)[i].markWidth, (*series)[i].markColor);

					DrawText(w, x+scale*25, y, 0, (*series)[i].legend, scaledFont, Black() );
				}
			}
		}
	};
}

#endif /* GRAPHELEMENT_H_ */
