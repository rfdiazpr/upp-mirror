/*
 * GraphElement.h
 *
 *  Created on: 6 avr. 2012
 *      Author: didier
 */

#ifndef GRAPHELEMENT_H_
#define GRAPHELEMENT_H_

#include "GraphUndo.h"

namespace GraphDraw_ns
{
	typedef enum ElementPosition {
		LEFT_OF_GRAPH  = 0x001,
		RIGHT_OF_GRAPH = 0x002,
		TOP_OF_GRAPH   = 0x010,
		BOTTOM_OF_GRAPH= 0x020,
		FLOAT_OVER_GRAPH     = 0x100
	} ElementPosition;

	enum {
		VERTICAL_MASK   = 0x00F,
		HORIZONTAL_MASK = 0x0F0,
		OVER_MASK       = 0xF00
	};

	typedef enum RefreshStrategy {
			REFRESH_FAST = 0,
			REFRESH_TOTAL,
			REFRESH_KEEP_DATA
	} RefreshStrategy;

	typedef enum FitToDataStrategy {
		ALL_SERIES,
		VISIBLE_SERIES_ONLY
	} FitToDataStrategy;


	typedef Callback2< Point, dword >  MouseLocalLoopCB;

	class GraphElementParent {
		public:
			typedef GraphElementParent CLASSNAME;
			virtual void ScrollX( TypeScreenCoord xOffset) = 0;
			virtual void ScrollY( TypeScreenCoord yOffset) = 0;
			virtual void ZoomX(TypeScreenCoord left, TypeScreenCoord right) = 0;
			virtual void ZoomY(TypeScreenCoord top, TypeScreenCoord bottom) = 0;
			virtual void RefreshFromChild( RefreshStrategy doFastPaint ) = 0;
			virtual Value GetSeries() = 0;
			virtual Value GetParentCtrl() = 0;
			virtual void AddUndoAction(GraphUndoData& CB) = 0;
			virtual Callback MakeRestoreGraphSizeCB() = 0;
			
			virtual void DoLocalLoop(MouseLocalLoopCB  CB) {};

			GraphElementParent() {}
			virtual ~GraphElementParent() {}
	};


	// ============================
	//    GraphElement   CLASS
	// ============================
	class GraphElement {
		public:
			Rect            _frame;     // Frame on which element is painted (absolute position in complete ctrl area)
			Rect            _floatFrame;// Frame size used as SCALE=1 reference frame  when element is FLOATING
			int             _width;     // width of GraphElement (in screen size)
			ElementPosition _pos;       // position in plot area
			int             _allowedPosMask;
			bool            _hide;      // true: is not drawn
			int             _stackingPriority; // 
			String          _name;
			Value           _backgndStyle; //

		public:
			GraphElementParent* _parent;

			GraphElement()
			: _frame( 0, 0, 5, 5 )
			, _floatFrame(_frame)
			, _width(5)
			, _pos(LEFT_OF_GRAPH)
			, _allowedPosMask(LEFT_OF_GRAPH | RIGHT_OF_GRAPH| BOTTOM_OF_GRAPH | TOP_OF_GRAPH | FLOAT_OVER_GRAPH )
			, _hide(false)
			, _stackingPriority(100)
			, _name("NAME NOT SET")
			, _parent(0)
			{}
			
			GraphElement(GraphElement* p)
			: _frame( 0, 0, 5, 5 )
			, _floatFrame(p->_floatFrame)
			, _width(p->_width)
			, _pos(LEFT_OF_GRAPH)
			, _allowedPosMask(p->_allowedPosMask)
			, _hide(p->_hide)
			, _stackingPriority(p->_stackingPriority)
			, _name("NAME NOT SET")
			, _backgndStyle(p->_backgndStyle)
			, _parent(p->_parent)
			{}

			void AdjustToPlotRect(const Rect& plotRect)
			{
				if ( !IsFloat() ) {
					int xCenter = (plotRect.left + plotRect.right)/2;
					if ( (_frame.left < xCenter) && (xCenter < _frame.right) ) {
						_frame.left = plotRect.left;
						_frame.right = plotRect.right;
					}
					else {
						_frame.top = plotRect.top;
						_frame.bottom = plotRect.bottom;
					}
					Update();
				}
			}

			virtual ~GraphElement() {}

			inline void SetName(const char* name) { _name = name; };
			virtual bool Contains(Point p) const { return _frame.Contains(p); }
			inline const Rect& GetFrame() const { return _frame; }
			inline void SetFrame(Rect v) { _frame = v; Update(); }
			inline const Rect& GetFloatFrame() const { return _floatFrame; }
			inline const Rect GetFloatFrame(int scale) const { Rect f=_floatFrame; f.Set(f.TopLeft()*scale, f.BottomRight()*scale) ; return f; }
			inline void SetFloatFrame(Rect v) { _floatFrame = v; }
			inline int GetElementWidth() { return _width; }
			inline ElementPosition GetElementPos() { return _pos; }
			inline void SetAllowedPosMask( int v ) { _allowedPosMask = v; }
			inline void DisablePos( int v ) { _allowedPosMask &= ~v; }
			inline void SetStackingPriority( int v ) { _stackingPriority = v; }
			inline int GetStackingPriority()  { return _stackingPriority; }

			inline bool IsVertical() const { return ((_pos & GraphDraw_ns::VERTICAL_MASK)!=0); }
			inline bool IsHorizontal() const { return ((_pos & GraphDraw_ns::HORIZONTAL_MASK)!=0); }
			inline bool IsFloat() const { return ((_pos & GraphDraw_ns::OVER_MASK)!=0); }
			inline bool IsHidden() const { return _hide; }
			inline void Hide( bool v=true ) { _hide = v; }
			
			bool operator<(const GraphElement& b) const { return (_stackingPriority < b._stackingPriority); };
			bool operator>(const GraphElement& b) const { return (_stackingPriority > b._stackingPriority); };

            // Paint element somewhere inside the graph area as a FLOATING element (legend, ...)
			// Offset and clipping are set with the '_floatFrame' settings
			virtual void PaintFloatElement(Draw& dw, int scale) { };

			// Paint the element in his own area
			// There is no clipping ==> drawing can overlap plot or other elements
			virtual void PaintElement(Draw& dw, int scale) = 0;

			// Paint additionnal element stuff on PLOT AREA : grids, square zones,  anything you wan't
			// Painting zone is clipped so nothing can be drawn outside
			virtual void PaintOnPlot_underData(Draw& dw, int otherWidth, int scale) {}
			virtual void PaintOnPlot_overData(Draw& dw, int otherWidth, int scale) {}
			virtual void Update() {}; // called when coordinates need update

			virtual GraphElement* Clone() = 0;


			virtual void LeftDown   (Point p, dword keyflags) { };
			virtual void LeftDouble (Point p, dword keyflags) { };
			virtual void RightDown  (Point p, dword keyflags) { };
			virtual void RightDouble(Point p, dword keyflags) { };
			virtual void MiddleDown (Point p, dword keyflags) { };
			virtual void MouseMove  (Point p, dword keyflags) { };
			virtual void MouseWheel (Point p, int zdelta, dword keyflags) { };
			virtual Image  CursorImage(Point p, dword keyflags) { return GraphDrawImg::CROSS(); }
			
			virtual void FitToData(FitToDataStrategy fitStrategy) {}

	};

	inline bool compareGraphElementFrame(const GraphElement* a, const GraphElement* b) { return *a < *b; }

	template<class DERIVED>
	class CRTPGraphElement : public GraphElement {
		public:
			CRTPGraphElement() {}
			CRTPGraphElement(CRTPGraphElement& p) : GraphElement(p) {}
			virtual ~CRTPGraphElement() {}

			inline  DERIVED&  SetElementWidth(int v) { _width = v; return *static_cast<DERIVED*>(this); }
			virtual DERIVED&  SetElementPos(ElementPosition v) { _pos = v; return *static_cast<DERIVED*>(this); }
			inline  DERIVED&  SetBackgndStyle(const Value& v) { _backgndStyle = v; return *static_cast<DERIVED*>(this); }
	};


	class BlankAreaElement : public CRTPGraphElement< BlankAreaElement >
	{
		typedef BlankAreaElement CLASSNAME;
		public:
			BlankAreaElement() {}
			BlankAreaElement( BlankAreaElement& p) : CRTPGraphElement< BlankAreaElement >(p) {}
			virtual ~BlankAreaElement() {}
			virtual void PaintElement(Draw& dw, int scale) { /* do noting */}
			virtual CLASSNAME* Clone() { return new CLASSNAME(*this); }
	};


}

#endif /* GRAPHELEMENT_H_ */
