#ifndef _GraphCtrl_GrapCtrl_h
#define _GraphCtrl_GrapCtrl_h

#include <CtrlLib/CtrlLib.h>
#include <GraphDraw/GraphDraw.h>

using namespace Upp;


#define LAYOUTFILE <GraphCtrl/GraphCtrl.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS GraphCtrlImg
#define IMAGEFILE <GraphCtrl/GraphCtrl.iml>
#include <Draw/iml_header.h>

#include <plugin/jpg/jpg.h>
#include <plugin/tif/tif.h>


// ============================
//    CRTP_GraphCtrlBase   CLASS
// ============================
template<class TYPES, class DERIVED>
class CRTP_GraphCtrlBase : public GraphDraw_ns::CRTP_StdGraphDraw<TYPES, DERIVED>, public Ctrl
{
	public:
	typedef CRTP_GraphCtrlBase<TYPES, DERIVED> CLASSNAME;

	private:
	typedef GraphDraw_ns::CRTP_StdGraphDraw<TYPES, DERIVED> _B;

	GraphDraw_ns::GraphElementFrame* elementCapture_LeftDown   ;
	GraphDraw_ns::GraphElementFrame* elementCapture_LeftDouble ;
	GraphDraw_ns::GraphElementFrame* elementCapture_LeftDrag   ;
	GraphDraw_ns::GraphElementFrame* elementCapture_RightDown  ;
	GraphDraw_ns::GraphElementFrame* elementCapture_RightDouble;
	GraphDraw_ns::GraphElementFrame* elementCapture_RightDrag  ;
	GraphDraw_ns::GraphElementFrame* elementCapture_MiddleDown ;
	GraphDraw_ns::GraphElementFrame* elementCapture_MouseMove  ;
	GraphDraw_ns::GraphElementFrame* elementCapture_MouseWheel ;


	bool plotCapture_LeftDown   ;
	bool plotCapture_LeftDouble ;
	bool plotCapture_LeftDrag   ;
	bool plotCapture_RightDown  ;
	bool plotCapture_RightDouble;
	bool plotCapture_RightDrag  ;
	bool plotCapture_MiddleDown ;
	bool plotCapture_MouseMove  ;
	bool plotCapture_MouseWheel ;

	Image CaptureMouseMove_cursorImage;

	Image ctrlImgSave; // to save ctrl image

	Point prevMousePoint;
	int copyRatio;

	public:



	CRTP_GraphCtrlBase()
	: elementCapture_LeftDown   (0)
	, elementCapture_LeftDouble (0)
	, elementCapture_LeftDrag   (0)
	, elementCapture_RightDown  (0)
	, elementCapture_RightDouble(0)
	, elementCapture_RightDrag  (0)
	, elementCapture_MiddleDown (0)
	, elementCapture_MouseMove  (0)
	, elementCapture_MouseWheel (0)
	, plotCapture_LeftDown   (false)
	, plotCapture_LeftDouble (false)
	, plotCapture_LeftDrag   (false)
	, plotCapture_RightDown  (false)
	, plotCapture_RightDouble(false)
	, plotCapture_RightDrag  (false)
	, plotCapture_MiddleDown (false)
	, plotCapture_MouseMove  (false)
	, plotCapture_MouseWheel (false)
	, copyRatio(3)
	{
		SetModify();
		setScreenSize( GetSize() );
	}

	CRTP_GraphCtrlBase(const CRTP_GraphCtrlBase& p)
	: _B(p)
	, elementCapture_LeftDown   (0)
	, elementCapture_LeftDouble (0)
	, elementCapture_LeftDrag   (0)
	, elementCapture_RightDown  (0)
	, elementCapture_RightDouble(0)
	, elementCapture_RightDrag  (0)
	, elementCapture_MiddleDown (0)
	, elementCapture_MouseMove  (0)
	, elementCapture_MouseWheel (0)
	, plotCapture_LeftDown   (false)
	, plotCapture_LeftDouble (false)
	, plotCapture_LeftDrag   (false)
	, plotCapture_RightDown  (false)
	, plotCapture_RightDouble(false)
	, plotCapture_RightDrag  (false)
	, plotCapture_MiddleDown (false)
	, plotCapture_MouseMove  (false)
	, plotCapture_MouseWheel (false)
	, copyRatio(p.copyRatio)
	{
		SetModify();
		setScreenSize( GetSize() );
	}

	virtual void Refresh() {
		_B::updateSizes();
		Ctrl::Refresh();
	};

	virtual void Layout() {
		SetModify();
	}

	virtual bool   IsModified() const {
		return ( Ctrl::IsModified() || _B::IsModifiedData());
	}

	virtual void   ClearModify() {
		Ctrl::ClearModify();
		_B::ClearModifyData();
	}

	void Paint2(Draw& w) {
		setScreenSize( GetSize() );
		if (_B::_mode == GraphDraw_ns::MD_DRAW) {
			ImageDraw ib(GetSize());
			_B::Paint(ib, 1);
			ctrlImgSave = ib;
			w.DrawImage(0, 0, ctrlImgSave);
		}
		else {
			ImageBuffer ib(GetSize());
			BufferPainter bp(ib, _B::_mode);
			_B::Paint(bp, 1);
			ctrlImgSave = ib;
			w.DrawImage(0, 0, ctrlImgSave);
		}
	}

	// Refresh called from child
	virtual void RefreshFromChild( GraphDraw_ns::RefreshStrategy doFastPaint ) {
		if (doFastPaint == GraphDraw_ns::REFRESH_FAST) {
			_B::_doFastPaint = true;
		}
		SetModify();
		Refresh();
	};


	virtual void Paint(Draw& w) {
		if ( _B::_doFastPaint == false )	{
			if ( IsModified() ) {
				Image m = OverrideCursor(GraphCtrlImg::SAND());
				Paint2(w);
				OverrideCursor(m);
				ClearModify();
			}
			else {
				w.DrawImage(0, 0, ctrlImgSave);
			}
		}
		else {
			Paint2(w);
			SetModify();
		}
	}


	int GetCopyRatio() { return copyRatio; }
	DERIVED& SetCopyRatio(int ratio) {
		copyRatio = ratio;
		return *static_cast<DERIVED*>(this);
	}

#ifdef PLATFORM_WIN32

	void SaveAsMetafile(const char* file)
	{
		GuiLock __;
		WinMetaFileDraw wmfd;
		wmfd.Create(copyRatio*GetSize().cx, copyRatio*GetSize().cy, "GraphCtrl", "chart", file);
		_B::Paint<Draw>(wmfd, copyRatio);
		wmfd.Close();
	}

	void SaveToClipboard(bool saveAsMetafile)
	{
		GuiLock __;
		
/*		if (saveAsMetafile) {
			WinMetaFileDraw wmfd;
			wmfd.Create(copyRatio*GetSize().cx, copyRatio*GetSize().cy, "GraphCtrl", "chart");
			SetDrawing<Draw>(wmfd, copyRatio);
			WinMetaFile wmf = wmfd.Close();
			wmf.WriteClipboard();
		} else {*/
			Image img = GetImage(copyRatio);
			WriteClipboardImage(img);
//		}
	}
#else

	void SaveToClipboard(bool)
	{
		GuiLock __;
		Image img = _B::GetImage(copyRatio);
		WriteClipboardImage(img);
	}
#endif

	void SaveToFile(String fileName)
	{
		GuiLock __;
		if (IsNull(fileName)) {
			FileSel fs;

			fs.Type("PNG file", "*.png");
			fs.Type("JPEG file", "*.jpg");
			fs.Type("TIFF file", "*.tif");
		    if(!fs.ExecuteSaveAs(t_("Saving plot to PNG, JPEG or TIFF file"))) {
		        Exclamation(t_("Plot has not been saved"));
		        return;
		    }
	        fileName = fs;
		}
		if (GetFileExt(fileName) == ".png") {
			PNGEncoder encoder;
			encoder.SaveFile(fileName, _B::GetImage(copyRatio));
		} else if (GetFileExt(fileName) == ".jpg") {
			JPGEncoder encoder(90);
			encoder.SaveFile(fileName, _B::GetImage(copyRatio));
		} else if (GetFileExt(fileName) == ".tif") {
			TIFEncoder encoder;
			encoder.SaveFile(fileName, _B::GetImage(copyRatio));
		} else
			Exclamation(Format(t_("File format \"%s\" not found"), GetFileExt(fileName)));
	}


	void ContextMenu(Bar& bar)
	{
//		if (mouseHandlingX || mouseHandlingY) {
//			bar.Add(t_("Fit to data"), 	ScatterImg::ShapeHandles(), THISBACK1(FitToData, mouseHandlingY));
//			bar.Add(t_("Zoom +"), 		ScatterImg::ZoomPlus(), 	THISBACK3(Zoom, 1/1.2, true, mouseHandlingY));
//			bar.Add(t_("Zoom -"), 		ScatterImg::ZoomMinus(), 	THISBACK3(Zoom, 1.2, true, mouseHandlingY));
//			bar.Add(t_("Scroll Left"), 	ScatterImg::LeftArrow(), 	THISBACK2(ScatterDraw::Scroll, -0.2, 0));
//			bar.Add(t_("Scroll Right"), ScatterImg::RightArrow(), 	THISBACK2(ScatterDraw::Scroll, 0.2, 0));
//			if (mouseHandlingY) {
//				bar.Add(t_("Scroll Up"), 	ScatterImg::UpArrow(), 	THISBACK2(ScatterDraw::Scroll, 0, -0.2));
//				bar.Add(t_("Scroll Down"), 	ScatterImg::DownArrow(), THISBACK2(ScatterDraw::Scroll, 0, 0.2));
//			}
//			bar.Separator();
//		}
		bar.Add(t_("Copy"), GraphCtrlImg::COPY(), 		  THISBACK1(SaveToClipboard, false)).Key(K_CTRL_C);
		bar.Add(t_("Save to file"), GraphCtrlImg::SAVE(), THISBACK1(SaveToFile, Null));
	}


	typedef GraphDraw_ns::GraphElementFrame* (GraphDraw_ns::GraphElementFrame::*mouseCallBack)(Point,dword);

	template<class RESULT, class CBCK>
	bool ProcessMouseCallBack(Point p, dword keyflags, CBCK cbck, RESULT& output ,RESULT defaultRes = 0)
	{
		for (int j = 0; j < _B::_overElements.GetCount(); j++)
		{
			if (_B::_overElements[j]->Contains(p)) {
				output = ((_B::_overElements[j]->*cbck)(p, keyflags));
				return true;
			}
		}
		for (int j = 0; j < _B::_leftElements.GetCount(); j++)
		{
			if (_B::_leftElements[j]->Contains(p)) {
				output = ((_B::_leftElements[j]->*cbck)(p, keyflags));
				return true;
			}
		}

		for (int j = 0; j < _B::_bottomElements.GetCount(); j++)
		{
			if (_B::_bottomElements[j]->Contains(p)) {
				output = ((_B::_bottomElements[j]->*cbck)(p, keyflags));
				return true;
			}
		}

		for (int j = 0; j < _B::_rightElements.GetCount(); j++)
		{
			if (_B::_rightElements[j]->Contains(p)) {
				output = ((_B::_rightElements[j]->*cbck)(p, keyflags));
				return true;
			}
		}

		for (int j = 0; j < _B::_topElements.GetCount(); j++)
		{
			if (_B::_topElements[j]->Contains(p)) {
				output = ((_B::_topElements[j]->*cbck)(p, keyflags));
				return true;
			}
		}
		return false;
	}


	virtual void LeftDown(Point p, dword keyflags) {
		prevMousePoint = p;
		if ( elementCapture_LeftDown != 0) {
			elementCapture_LeftDown = elementCapture_LeftDown->LeftDown(p, keyflags);
			return;
		}
		if ( ProcessMouseCallBack<GraphDraw_ns::GraphElementFrame*>(p, keyflags, &GraphDraw_ns::GraphElementFrame::LeftDown, elementCapture_LeftDown) )
		{
			return;
		}
		else if ( _B::_plotRect.Contains(p) ) {
			if ( keyflags & K_CTRL )
			{
				// SELECT ZOOM
				RectTracker tracker(*this);
				Rect selectedZoomArea = tracker.Track( RectfC(p.x,p.y,0,0), ALIGN_NULL, ALIGN_NULL) - _B::_PlotTopLeft;
				if (selectedZoomArea.Width() !=0  && selectedZoomArea.Height() != 0) {
					_B::ZoomOnRect( selectedZoomArea );
					SetModify();
				}
			}
			return;
		}
	}

	virtual void LeftDouble(Point p, dword keyflags) {
		if ( elementCapture_LeftDouble != 0) {
			elementCapture_LeftDouble = elementCapture_LeftDouble->LeftDouble(p, keyflags);
			return;
		}
		if ( ProcessMouseCallBack<GraphDraw_ns::GraphElementFrame*>(p, keyflags, &GraphDraw_ns::GraphElementFrame::LeftDouble, elementCapture_LeftDouble) ) {
			return;
		}
		if ( _B::_plotRect.Contains(p) ) {
			return;
		}
	}

	virtual void LeftDrag(Point p, dword keyflags) {
		if ( elementCapture_LeftDrag != 0) {
			elementCapture_LeftDrag = elementCapture_LeftDrag->LeftDrag(p, keyflags);
			return;
		}
		if ( ProcessMouseCallBack<GraphDraw_ns::GraphElementFrame*>(p, keyflags, &GraphDraw_ns::GraphElementFrame::LeftDrag, elementCapture_LeftDrag)) {
			return;
		}
		if ( _B::_plotRect.Contains(p) ) {
			// do PLOT SCROLLING
			return;
		}
	}

	virtual void RightDown(Point p, dword keyflags) {
		if ( elementCapture_RightDown != 0) {
			elementCapture_RightDown = elementCapture_RightDown->RightDown(p, keyflags);
			return;
		}
		if ( ProcessMouseCallBack<GraphDraw_ns::GraphElementFrame*>(p, keyflags, &GraphDraw_ns::GraphElementFrame::RightDown, elementCapture_RightDown)) {
			return;
		}
		if ( _B::_plotRect.Contains(p) ) {
			MenuBar::Execute(THISBACK(ContextMenu));
			return;
		}
	}

	virtual void RightDouble(Point p, dword keyflags) {
		if ( elementCapture_RightDouble != 0) {
			elementCapture_RightDouble = elementCapture_RightDouble->RightDouble(p, keyflags);
			return;
		}
		if ( ProcessMouseCallBack<GraphDraw_ns::GraphElementFrame*>(p, keyflags, &GraphDraw_ns::GraphElementFrame::RightDouble, elementCapture_RightDouble) ) {
			return;
		}
		if ( _B::_plotRect.Contains(p) ) {
			return;
		}

	}

	virtual void MiddleDown(Point p, dword keyflags) {
		if ( elementCapture_MiddleDown != 0) {
			elementCapture_MiddleDown = elementCapture_MiddleDown->MiddleDown(p, keyflags);
			return;
		}
		if ( ProcessMouseCallBack<GraphDraw_ns::GraphElementFrame*>(p, keyflags, &GraphDraw_ns::GraphElementFrame::MiddleDown, elementCapture_MiddleDown) ) {
			return;
		}
		if ( _B::_plotRect.Contains(p) ) {
			// do PLOT  CENTER  at point
			return;
		}
	}

	virtual void MouseMove(Point p, dword keyflags) {
		if ( elementCapture_MouseMove != 0) {
			elementCapture_MouseMove = elementCapture_MouseMove->MouseMove(p, keyflags);
			return;
		}

		if ( (plotCapture_MouseMove==0) && ProcessMouseCallBack<GraphDraw_ns::GraphElementFrame*>(p, keyflags, &GraphDraw_ns::GraphElementFrame::MouseMove, elementCapture_MouseMove)) {

		}
		else if (( _B::_plotRect.Contains(p) ) || plotCapture_MouseMove)  {
			if ( keyflags & K_MOUSELEFT ) {
				// LEFT SCROLL
				_B::_doFastPaint = true;
				plotCapture_MouseMove = true;
				CaptureMouseMove_cursorImage = CursorImage(p, keyflags);
				_B::Scroll(p.x-prevMousePoint.x, p.y-prevMousePoint.y);
				prevMousePoint = p;
				Refresh();
			}
			else {
				plotCapture_MouseMove = false;
				CaptureMouseMove_cursorImage = Null;
				if (_B::_doFastPaint) { // Do complete drawing when SCROLLING FINISHED
					_B::_doFastPaint = false;
					Refresh();
				}
			}
			return;
		}

		if (( elementCapture_MouseMove == 0) && (_B::_doFastPaint)) {
			// Do complete drawing when nothing special to be done
			_B::_doFastPaint = false;
			Refresh();
		}

		CaptureMouseMove_cursorImage = Null;
		if ( elementCapture_MouseMove)
		{
			CaptureMouseMove_cursorImage = CursorImage(p, keyflags);
		}
	}


	virtual Image  CursorImage(Point p, dword keyflags)
	{
		if ( !CaptureMouseMove_cursorImage.IsNullInstance() ) return CaptureMouseMove_cursorImage;
		Image output;
		if ( ProcessMouseCallBack<Image>(p, keyflags, &GraphDraw_ns::GraphElementFrame::CursorImage, output, GraphDrawImg::CROSS())) {
			return output;
		}
		if ( _B::_plotRect.Contains(p) ) {
			if ( keyflags & K_CTRL ) return GraphCtrlImg::ZOOM();
			return GraphCtrlImg::SCROLL();
		}
		return GraphDrawImg::CROSS();
	}



	virtual void MouseWheel(Point p, int zdelta, dword keyflags)
	{
		for (int j = 0; j < _B::_overElements.GetCount(); j++)
		{
			if (_B::_overElements[j]->Contains(p)) {
				(_B::_overElements[j]->MouseWheel)(p, zdelta, keyflags);
				return;
			}
		}

		for (int j = 0; j < _B::_leftElements.GetCount(); j++)
		{
			if (_B::_leftElements[j]->Contains(p)) {
				(_B::_leftElements[j]->MouseWheel)(p, zdelta, keyflags);
				return;
			}
		}

		for (int j = 0; j < _B::_bottomElements.GetCount(); j++)
		{
			if (_B::_bottomElements[j]->Contains(p)) {
				(_B::_bottomElements[j]->MouseWheel)(p, zdelta, keyflags);
				return;
			}
		}

		for (int j = 0; j < _B::_rightElements.GetCount(); j++)
		{
			if (_B::_rightElements[j]->Contains(p)) {
				(_B::_rightElements[j]->MouseWheel)(p, zdelta, keyflags);
				return ;
			}
		}

		for (int j = 0; j < _B::_topElements.GetCount(); j++)
		{
			if (_B::_topElements[j]->Contains(p)) {
				(_B::_topElements[j]->MouseWheel)(p, zdelta, keyflags);
				return;
			}
		}

		if ( _B::_plotRect.Contains(p) ) {
			if ( keyflags & K_CTRL ) // => WHEEL ZOOM
			{
				if (zdelta < 0) _B::ApplyZoomFactor(1.2);
				else            _B::ApplyInvZoomFactor(1.2);
				_B::_doFastPaint = true;

			}
			return;
		}
	}
};

// Default GridAxisPropertiesDlg  class
// it opens the minimal dialog for the CoordConverter beeing used
template <class GRIDAXISDRAW, class COORDCONVERTER>
struct GridAxisPropertiesDlg {
		void openDialog(GRIDAXISDRAW& gda, COORDCONVERTER& converter) {
			CtrlRetriever r;
			double gMin = converter.getGraphMin();
			double gMax = converter.getGraphMax();
			int majorTickLength = gda.GetMajorTickLength();
			int minorTickLength = gda.GetMinorTickLength();

			WithGridAxisPropertiesLayout<TopWindow> dlg;

			dlg.scaleType.Hide();

			r ( dlg.axisLineColor, gda._axisColor)
			  ( dlg.axisTextColor, gda._axisTextColor)
			  ( dlg.axisTickColor, gda._axisTickColor)
			  ( dlg.axisMajorTickLength, majorTickLength)
			  ( dlg.axisMinorTickLength, minorTickLength)
			  ( dlg.gridColor, gda._gridColor)
			  ( dlg.min, gMin)
			  ( dlg.max, gMax)
			  ;

			CtrlLayoutOKCancel(dlg, "");

			if ( dlg.Execute() == IDOK ) {
				r.Retrieve();
				gda.SetMajorTickLength(majorTickLength);
				gda.SetMinorTickLength(majorTickLength);
				converter.updateGraphSize(gMin, gMax);
				gda._parent->RefreshFromChild( GraphDraw_ns::REFRESH_TOTAL);
			}
		}
};


// Specialized 'GridAxisPropertiesDlg'  class
// it opens A dedicated dialog for the 'GenericCoordinateConverter' beeing used
template <class GRIDAXISDRAW>
struct GridAxisPropertiesDlg<GRIDAXISDRAW, GraphDraw_ns::GenericCoordinateConverter> {
		void openDialog(GRIDAXISDRAW& gda, GraphDraw_ns::GenericCoordinateConverter& converter) {
			CtrlRetriever r;
			double gMin = converter.getGraphMin();
			double gMax = converter.getGraphMax();
			int majorTickLength = gda.GetMajorTickLength();
			int minorTickLength = gda.GetMinorTickLength();
			int scaleType = converter.GetScaleType();

			WithGridAxisPropertiesLayout<TopWindow> dlg;

			dlg.scaleType.Add(GraphDraw_ns::GenericCoordinateConverter::AXIS_SCALE_STD,   t_("Std new") );
			dlg.scaleType.Add(GraphDraw_ns::GenericCoordinateConverter::AXIS_SCALE_LOG,   t_("Log") );
			dlg.scaleType.Add(GraphDraw_ns::GenericCoordinateConverter::AXIS_SCALE_POW10, t_("Pow10") );
			if ( gMin < 0) {
				dlg.scaleType.DisableValue( GraphDraw_ns::GenericCoordinateConverter::AXIS_SCALE_LOG);
			}

			r ( dlg.axisLineColor, gda._axisColor)
			( dlg.axisTextColor, gda._axisTextColor)
			( dlg.axisTickColor, gda._axisTickColor)
			( dlg.axisMajorTickLength, majorTickLength)
			( dlg.axisMinorTickLength, minorTickLength)
			( dlg.gridColor, gda._gridColor)
			( dlg.min, gMin)
			( dlg.max, gMax)
			( dlg.scaleType, scaleType)
			;

			CtrlLayoutOKCancel(dlg, "");

			if ( dlg.Execute() == IDOK ) {
				r.Retrieve();
				gda.SetMajorTickLength(majorTickLength);
				gda.SetMinorTickLength(majorTickLength);
				converter.SetScaleType(scaleType);
				converter.updateGraphSize(gMin, gMax);
				gda._parent->RefreshFromChild( GraphDraw_ns::REFRESH_TOTAL);
			}
		}
};



/************************************************************
 *
 ************************************************************/

template <class TYPES>
class StdGridAxisDrawCtrl : public GraphDraw_ns::GridAxisDraw<TYPES>
{
	typedef StdGridAxisDrawCtrl<TYPES> CLASSNAME;
	typedef GraphDraw_ns::GridAxisDraw<TYPES> _B;

	Point prevMousePoint;


	public:
	StdGridAxisDrawCtrl(typename TYPES::TypeCoordConverter& conv) : _B(conv) {}
	StdGridAxisDrawCtrl( StdGridAxisDrawCtrl& p) : _B(p) {}
	~StdGridAxisDrawCtrl() {}


	virtual Image  CursorImage(Point p, dword keyflags) {
		if ( keyflags & K_CTRL ){
			if (keyflags & K_SHIFT) {
				if (_B::IsVertical() ) return GraphCtrlImg::AXIS_ZOOM_Y();
				else                   return GraphCtrlImg::AXIS_ZOOM_X();
			}
			if (_B::IsVertical() ) return GraphCtrlImg::ZOOM_Y();
			else                   return GraphCtrlImg::ZOOM_X();
		}

		if (keyflags & K_SHIFT) {
			if (_B::IsVertical() ) return GraphCtrlImg::AXIS_SCROLL_Y();
			else                   return GraphCtrlImg::AXIS_SCROLL_X();
		}
		if (_B::IsVertical() ) return GraphCtrlImg::SCROLL_Y();
		else                   return GraphCtrlImg::SCROLL_X();
	}

	virtual GraphDraw_ns::GraphElementFrame* MouseWheel (Point p, int zdelta, dword keyflags) {
		typename TYPES::TypeCoordConverter& converter = _B::GetCoordConverter();
		if (zdelta < 0) zdelta = -1;
		else            zdelta =  1;
		if (_B::IsVertical())  zdelta = -zdelta;
		zdelta *= abs(converter.getScreenRange()) / 5;

		if ( (keyflags & (K_CTRL|K_SHIFT)) == (K_CTRL|K_SHIFT) ) // => ZOOM on wheel (this axis only)
		{
			converter.updateGraphSize( converter.toGraph( converter.getScreenMin() - zdelta ),
			                           converter.toGraph( converter.getScreenMax() + zdelta ));
			_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
		}
		else if ( keyflags & K_CTRL )
		{ // => ZOOM on wheel ( whole graph )
			if (_B::IsVertical() ) {
				_B::_parent->ZoomY(converter.getScreenMax() + zdelta, converter.getScreenMin() - zdelta);
			} else {
				_B::_parent->ZoomX(converter.getScreenMin() - zdelta, converter.getScreenMax() + zdelta);
			}
			_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
		}
		else if ( keyflags & K_SHIFT )
		{
			// => SCROLL on wheel ( on axis )
			converter.updateGraphSize( converter.toGraph( converter.getScreenMin() - zdelta ),
			                           converter.toGraph( converter.getScreenMax() - zdelta ));
			_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
			return this; // Capture MouseCtrl
		}
		else
		{
			// => SCROLL on wheel ( whole graph )
			if (_B::IsVertical() ) {
				// Vertical drag
				_B::_parent->ScrollY(zdelta);
			} else {
				// Horizontal drag
				_B::_parent->ScrollX(zdelta);
			}
			_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
			return this; // Capture MouseCtrl
		}
		return 0; // no need to capture MouseCtrl
	}

	virtual GraphDraw_ns::GraphElementFrame*  LeftDown   (Point p, dword keyflags) {
		prevMousePoint = p;
		return 0; // no need to capture MouseCtrl
	}

	virtual GraphDraw_ns::GraphElementFrame* MouseMove (Point p, dword keyflags) {
		if (keyflags & K_MOUSELEFT)
		{
			if ( keyflags & K_SHIFT )
			{
				// ==> LEFT SCROLL (this axis only)
				int delta=0;
				if (_B::IsVertical() ) {
					// Vertical drag
					delta = p.y-prevMousePoint.y;
				} else {
					// Horizontal drag
					delta = p.x-prevMousePoint.x;
				}
				typename TYPES::TypeCoordConverter& converter = _B::GetCoordConverter();
				converter.Scroll( delta );
				prevMousePoint = p;
				_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
				return this; // Capture MouseCtrl
			}
			else // if (keyflags & K_SHIFT)
			{
				// ==> LEFT SCROLL ( whole graph )
				if (_B::IsVertical() ) {
					// Vertical drag
					_B::_parent->ScrollY(p.y-prevMousePoint.y);
				} else {
					// Horizontal drag
					_B::_parent->ScrollX(p.x-prevMousePoint.x);
				}
				prevMousePoint = p;
				_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
				return this; // Capture MouseCtrl
			}
		}
		return 0; // no need to capture MouseCtrl
	}

	virtual CLASSNAME* Clone() { return new CLASSNAME(*this); }


	virtual void OpenGridAxisPropertiesDlg()
	{
		GridAxisPropertiesDlg<CLASSNAME,  typename TYPES::TypeCoordConverter >().openDialog(*this,  _B::GetCoordConverter() );
	}

	virtual GraphDraw_ns::GraphElementFrame* LeftDouble (Point p, dword keyflags) {
		OpenGridAxisPropertiesDlg();
		return 0;
	}
};




template <class TYPES, class LEGEND_DRAW_CLASS >
class CtrlElement_MoveResize : public LEGEND_DRAW_CLASS
{
	typedef CtrlElement_MoveResize<TYPES,LEGEND_DRAW_CLASS> CLASSNAME;
	typedef LEGEND_DRAW_CLASS _B;

	Point prevMousePoint;
	Ctrl* parentCtrl;

	public:
	CtrlElement_MoveResize() : parentCtrl(0) {}
	CtrlElement_MoveResize(Ctrl& p) : parentCtrl(&p) {}
	CtrlElement_MoveResize( CtrlElement_MoveResize& p) : _B(p), parentCtrl(p.parentCtrl)  {}
	~CtrlElement_MoveResize() {}

	virtual bool Contains(Point p) const { return (_B::_frame.Contains(p) && _B::IsOverGraph()); }

	inline CtrlElement_MoveResize& SetParentCtrl(Ctrl& p) { parentCtrl = &p; return *this; }

	virtual Image  CursorImage(Point p, dword keyflags) {
		if ( keyflags & K_CTRL ){
			return GraphCtrlImg::ELEMENT_RESIZE();
		}
		return GraphCtrlImg::ELEMENT_MOVE();
	}
	virtual GraphDraw_ns::GraphElementFrame* MouseMove (Point p, dword keyflags) {
		if (parentCtrl != 0) {
			if (keyflags & K_MOUSELEFT)
			{
				RectTracker tracker(*parentCtrl);
				if(keyflags & K_CTRL) {
					tracker.Dashed().Animation();
					_B::_frame = tracker.Track(_B::_frame, ALIGN_NULL, ALIGN_NULL);
					_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_TOTAL );
					return 0;
				}
				tracker.Dashed().Animation();
				_B::_frame = tracker.Track(_B::_frame, ALIGN_CENTER, ALIGN_CENTER);
				_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_TOTAL );
				return 0;
			}
		}
		return 0; // no need to capture MouseCtrl
	}

	virtual CLASSNAME* Clone() { return new CLASSNAME(*this); }
};


template<class TYPES, class LEGENDDRAW>
class StdLegendCtrl : public CtrlElement_MoveResize<TYPES, LEGENDDRAW> {
	public:
	StdLegendCtrl() {}
	virtual ~StdLegendCtrl() {}

	StdLegendCtrl(Ctrl& ctrl) : CtrlElement_MoveResize<TYPES, LEGENDDRAW>(ctrl) {}
	StdLegendCtrl(StdLegendCtrl& p) : CtrlElement_MoveResize<TYPES, LEGENDDRAW>(p) {}
};
// ===============================================================================================================================
// ===============================================================================================================================
//
//
//
// ===============================================================================================================================
// ===============================================================================================================================
struct GraphCtrlDefaultTypes {
		typedef DataSource                                                      TypeDataSource;
		typedef SeriesPlot                                                      TypeSeriesPlot;
		typedef MarkPlot                                                        TypeMarkPlot;
		typedef GraphDraw_ns::GenericCoordinateConverter                        TypeCoordConverter;
		typedef StdGridAxisDrawCtrl<GraphCtrlDefaultTypes>                      TypeGridAxisDraw;
		typedef GraphDraw_ns::GridStepManager<>                                 TypeGridStepManager;
		typedef GraphDraw_ns::SeriesConfig<GraphCtrlDefaultTypes>               TypeSeriesConfig;
		typedef Vector<TypeSeriesConfig>                                        TypeVectorSeries;
};



template<class TYPES, class DERIVED >
class CRTP_StdGraphCtrl :	public CRTP_GraphCtrlBase< TYPES, DERIVED >
{
	public:
	typedef CRTP_StdGraphCtrl<TYPES, DERIVED>  CLASSNAME;
	typedef CRTP_GraphCtrlBase<TYPES, DERIVED> _B;
	typedef TYPES                              Types;

	private:

	public:
	CRTP_StdGraphCtrl() {
	}

};

template<class TYPES = GraphCtrlDefaultTypes >
class StdGraphCtrl :	public CRTP_StdGraphCtrl< TYPES, StdGraphCtrl<TYPES> >
{
};





#endif
