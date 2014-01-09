#ifndef _GraphCtrl_GraphCtrlBase_h_
#define _GraphCtrl_GraphCtrlBase_h_


struct GraphCtrl_Keys {
	static dword K_ZOOM;
	static dword K_AXIS_SELECT;
	static dword K_AXIS_ZOOM;
	static dword K_SCROLL;
	static dword K_AXIS_SCROLL;

	static dword K_ELEMENT_FLOAT_MOVERESIZE;
	static dword K_ELEMENT_FLOAT_MOVE;
	
	static void reset() {
		K_AXIS_SELECT = K_SHIFT;
		K_ZOOM        = K_CTRL;
		K_SCROLL      = 0;

		K_AXIS_ZOOM   = K_ZOOM   | K_AXIS_SELECT;
		K_AXIS_SCROLL = K_SCROLL | K_AXIS_SELECT;
		
		K_ELEMENT_FLOAT_MOVERESIZE = K_CTRL;
		K_ELEMENT_FLOAT_MOVE       = 0;
	}
};

#define TEST_GC_KEYS( keyflags, MASKS)  ( ((keyflags) & ~(K_MOUSEMIDDLE|K_MOUSELEFT|K_MOUSERIGHT) ) == (MASKS) )

// ============================================================================================
//                        LOOP DELEGATION CLASS
// ============================================================================================

class GraphCtrlLooper : public LocalLoop {
	private:
	Image cursorImg;
	virtual void  LeftUp(Point, dword)          { EndLoop(); }
	virtual void  RightUp(Point, dword)         { EndLoop(); }
	virtual void  MouseMove(Point p, dword f)   { 
		RLOGBLOCK_STR( true, "GraphCtrlLooper::MouseMove("<< &GetMaster() << ")");
		WhenMouseMove(p, f); };
	virtual Image CursorImage(Point p, dword keyflags) {
		RLOGBLOCK_STR( true, "GraphCtrlLooper::CursorImage("<< &GetMaster() << ")");
		if (cursorImg.IsNullInstance()) cursorImg = GetMaster().CursorImage(p,keyflags);
		return cursorImg;
	}
	
	public:
	GraphCtrlLooper(Ctrl& master) {
		SetMaster(master);
	} 
	GraphDraw_ns::MouseLocalLoopCB WhenMouseMove;
};


// ============================
//    CRTP_GraphCtrl_Base   CLASS
// ============================
typedef GraphDraw_ns::GraphElement* (GraphDraw_ns::GraphElement::*mouseCallBack)(Point,dword);

template<class TYPES, class DERIVED, template <class TYPES2, class DERIVED2> class GRAPHDRAW_BASE_CLASS >
class CRTP_GraphCtrl_Base : public GRAPHDRAW_BASE_CLASS<TYPES, DERIVED>, public Ctrl
{
	public:
	typedef CRTP_GraphCtrl_Base<TYPES, DERIVED, GRAPHDRAW_BASE_CLASS> CLASSNAME;

	private:
	typedef GraphDraw_ns::CRTP_XYGraphDraw<TYPES, DERIVED> _B;

	Image CaptureMouseMove_cursorImage;
	Point prevMousePoint;
	Point selectOriginPoint, selectEndPoint;
	int copyRatio;
	int autoWaitCursor_paint;
	int autoWaitCursor_saveClipBoard;
	int autoWaitCursor_saveToFile;
	bool useLocalSelectLoop;
	bool isXZoomAllowed;
	bool isYZoomAllowed;
	bool isXScrollAllowed;
	bool isYScrollAllowed;
	bool isZoomFromAxisAllowed;
	bool isScrollFromAxisAllowed;
	bool isZoomFromGraphAllowed;
	bool isScrollFromGraphAllowed;

	public:

	CRTP_GraphCtrl_Base()
	: copyRatio(3)
	, autoWaitCursor_paint(0)
	, autoWaitCursor_saveClipBoard(0)
	, autoWaitCursor_saveToFile(0)
	, useLocalSelectLoop(true)
	, isXZoomAllowed(true)
	, isYZoomAllowed(true)
	, isXScrollAllowed(true)
	, isYScrollAllowed(true)
	, isZoomFromAxisAllowed(true)
	, isScrollFromAxisAllowed(true)
	, isZoomFromGraphAllowed(true)
	, isScrollFromGraphAllowed(true)
	{
		Transparent();
		BackPaint();
		SetModify();
		_B::setScreenSize( GetSize() );
		_B::_selectStyle = GraphCtrlImg::GSELECT();
	}
	private:
	CRTP_GraphCtrl_Base(const CRTP_GraphCtrl_Base& p)
	: _B(p)
	, copyRatio(p.copyRatio)
	, autoWaitCursor_paint(0)
	, autoWaitCursor_saveClipBoard(0)
	, autoWaitCursor_saveToFile(0)
	, useLocalSelectLoop(true)
	, isXZoomAllowed(true)
	, isYZoomAllowed(true)
	, isXScrollAllowed(true)
	, isYScrollAllowed(true)
	, isZoomFromAxisAllowed(true)
	, isScrollFromAxisAllowed(true)
	, isZoomFromGraphAllowed(true)
	, isScrollFromGraphAllowed(true)
	{
		Transparent();
		BackPaint();
		SetModify();
		_B::setScreenSize( GetSize() );
		_B::_selectStyle = GraphCtrlImg::GSELECT();
	}

	public:
	virtual Value GetParentCtrl() {
		return RawToValue(static_cast<Ctrl*>(this));
	}

	virtual void Refresh() {
		_B::updateSizes();
		Ctrl::Refresh();
	};

	void LayoutRefresh() {
		_B::_doFastPaint = false;
		Ctrl::Refresh();
	}
	
	virtual void Layout() {
		enum { 
			TIMEID_LAYOUT_REFRESH = Ctrl::TIMEID_COUNT,
		    TIMEID_COUNT
		};
		_B::_doFastPaint = true;
		KillSetTimeCallback(300, THISBACK(LayoutRefresh), TIMEID_LAYOUT_REFRESH );
		SetModify();
	}

	virtual bool   IsModified() const {
		return ( Ctrl::IsModified() || _B::IsModifiedData());
	}

	virtual void   ClearModify() {
		Ctrl::ClearModify();
		_B::ClearModifyData();
	}

	DERIVED&  UseLocalSelectLoop(bool p = true) {
		useLocalSelectLoop = p;
		return *static_cast<DERIVED*>(this);
	}

	private:
	void UpdateScrollZoomAuthorisations() {
		for (int j = 0; j < _B::_xConverters.GetCount(); j++) {
			_B::_xConverters[j]->AllowZoom(isXZoomAllowed && isZoomFromAxisAllowed );
			_B::_xConverters[j]->AllowScroll(isXScrollAllowed && isScrollFromAxisAllowed );
		}
		for (int j = 0; j < _B::_yConverters.GetCount(); j++) {
			_B::_yConverters[j]->AllowZoom(isYZoomAllowed && isZoomFromAxisAllowed );
			_B::_yConverters[j]->AllowScroll(isYScrollAllowed && isScrollFromAxisAllowed );
		}
	}
	public:
	
	DERIVED& DisableGraphZoom(bool p = false) {
		isZoomFromGraphAllowed= !p;
		return *static_cast<DERIVED*>(this);
	}

	DERIVED& DisableGraphScroll(bool p = false) {
		isScrollFromGraphAllowed= !p;
		return *static_cast<DERIVED*>(this);
	}

	DERIVED& DisableAxisZoom(bool p = false) {
		isZoomFromAxisAllowed= !p;
		UpdateScrollZoomAuthorisations();
		return *static_cast<DERIVED*>(this);
	}

	DERIVED& DisableXZoom(bool p = false) {
		isXZoomAllowed = !p;
		UpdateScrollZoomAuthorisations();
		return *static_cast<DERIVED*>(this);
	}

	DERIVED& DisableYZoom(bool p = false) {
		isYZoomAllowed = !p;
		UpdateScrollZoomAuthorisations();
		return *static_cast<DERIVED*>(this);
	}

	DERIVED& DisableAxisScroll(bool p = false) {
		isScrollFromAxisAllowed = !p;
		UpdateScrollZoomAuthorisations();
		return *static_cast<DERIVED*>(this);
	}

	DERIVED& DisableXScroll(bool p = false) {
		isXScrollAllowed = !p;
		UpdateScrollZoomAuthorisations();
		return *static_cast<DERIVED*>(this);
	}

	DERIVED& DisableYScroll(bool p = false) {
		isYScrollAllowed = !p;
		UpdateScrollZoomAuthorisations();
		return *static_cast<DERIVED*>(this);
	}

	inline bool IsZoomFromGraphEnabled() { return (isZoomFromGraphAllowed && (isXZoomAllowed || isYZoomAllowed)); }
	inline bool IsScrollFromGraphEnabled() { return (isScrollFromGraphAllowed && (isXScrollAllowed || isYScrollAllowed)); }



	virtual void AddXConverter(GraphDraw_ns::CoordinateConverter* conv) {
		_B::AddXConverter(conv);
		conv->AllowScroll(isXScrollAllowed);
		conv->AllowZoom(isXZoomAllowed);
	}

	virtual void AddYConverter(GraphDraw_ns::CoordinateConverter* conv) {
		_B::AddYConverter(conv);
		conv->AllowScroll(isYScrollAllowed);
		conv->AllowZoom(isYZoomAllowed);
	}


	void Paint2(Draw& w) {
		RLOGBLOCK_STR( _B::debugTrace, "CRTP_GraphCtrl_Base::Paint2(" << this << ")");
		_B::setScreenSize( GetSize() );
		_B::Paint(w, 1);
		
//		if(HasFocus())
//			DrawFocus(w, Rect(GetSize()).Deflated(5));//st->focusmargin));

	}


	// Refresh called from child
	virtual void RefreshFromChild( GraphDraw_ns::RefreshStrategy strategy ) {
		RLOGBLOCK_STR( _B::debugTrace, "CRTP_GraphCtrl_Base::RefreshFromChild(" << this << ")");
		if      (strategy == GraphDraw_ns::REFRESH_FAST)   _B::_doFastPaint = true;
		else if (strategy == GraphDraw_ns::REFRESH_FULL)  _B::_PlotDrawImage.Clear();// _B::_keepDataPaint = true;
		SetModify();
		Refresh();
	};

	virtual void Paint(Draw& w) {
		RLOGBLOCK_STR( _B::debugTrace, "CRTP_GraphCtrl_Base::Paint(" << this << ")   [ FastPaint , PlotImgEmpty ] => [ " << _B::_doFastPaint << " , " << _B::_PlotDrawImage.IsEmpty() << " ]");
		if ( _B::_doFastPaint == false ) {
			if (_B::_PlotDrawImage.IsEmpty()) {
				AutoWaitCursor waitcursor(autoWaitCursor_paint);
				Paint2(w);
			}
			else {
				Paint2(w);
			}
			ClearModify();
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

	void SaveToClipboard(bool saveAsMetafile)
	{
		RLOGBLOCK_STR( _B::debugTrace, "CRTP_GraphCtrl_Base::SaveToClipboard(" << this << ")");
//		if (saveAsMetafile) {
//			WinMetaFileDraw wmfd;
//			wmfd.Create(copyRatio*GetSize().cx, copyRatio*GetSize().cy, "GraphCtrl", "chart");
//			SetDrawing<Draw>(wmfd, copyRatio);
//			WinMetaFile wmf = wmfd.Close();
//			wmf.WriteClipboard();
//		} else {
			AutoWaitCursor waitcursor(autoWaitCursor_saveClipBoard);
			Image img = GetImage(copyRatio);
			WriteClipboardImage(img);
//		}
	}
#else

	void SaveToClipboard(bool)
	{
		RLOGBLOCK_STR( _B::debugTrace, "CRTP_GraphCtrl_Base::SaveToClipboard(" << this << ")");
		AutoWaitCursor waitcursor(autoWaitCursor_saveClipBoard);
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
			AutoWaitCursor waitcursor(autoWaitCursor_saveToFile);
			encoder.SaveFile(fileName, _B::GetImage(copyRatio));
		} else if (GetFileExt(fileName) == ".jpg") {
			JPGEncoder encoder(90);
			AutoWaitCursor waitcursor(autoWaitCursor_saveToFile);
			encoder.SaveFile(fileName, _B::GetImage(copyRatio));
		} else if (GetFileExt(fileName) == ".tif") {
			TIFEncoder encoder;
			AutoWaitCursor waitcursor(autoWaitCursor_saveToFile);
			encoder.SaveFile(fileName, _B::GetImage(copyRatio));
		} else
			Exclamation(Format(t_("File format \"%s\" not found"), GetFileExt(fileName)));
	}
	
	void OpenSeriesPropertiesDlg(int c) {
		WithSeriesPropertiesDlgLayout<TopWindow> dlg;
		
		GraphDraw_ns::SeriesConfig& s = _B::series[c];
		CtrlRetriever r;
		r ( dlg.fillColor, s.fillColor)
		  //( dlg.markShape, s.markShape)
		  ( dlg.opacity, s.opacity)
		  ( dlg.markColor, s.markColor)
		  ( dlg.lineDash, s.dash )
		  ( dlg.lineColor, s.color)
		  ( dlg.lineThickness, s.thickness)
		  ( dlg.fillColor, s.fillColor)
		  ( dlg.markWidth, s.markWidth)
		  ( dlg.show,   s.show)
		  ( dlg.legend, s.legend)
		  ;

		CtrlLayoutOKCancel(dlg, t_("Edit properties"));
		if(dlg.Execute() == IDOK) {
			r.Retrieve();
			_B::ClearPlotDrawImg();
			SetModify();
			Refresh();
		}
	}

	void HideSeries(int c) {
		_B::Show(c, _B::IsVisible(c) ? false : true);
		_B::ClearPlotDrawImg();
		SetModify();
		Refresh();
	}
	
	void ShowAllSeries(bool show=true) {
		_B::ShowAll(show);
		_B::ClearPlotDrawImg();
		SetModify();
		Refresh();
	}

	void CtrlFitToData(GraphDraw_ns::FitToDataStrategy fitStrategy = GraphDraw_ns::ALL_SERIES) {
		// No need to verify Authorisations ==> the menu isn't available
		GraphDraw_ns::GraphUndoData undo;
		undo.undoAction << _B::MakeRestoreGraphSizeCB();
			_B::FitToData( fitStrategy );
		undo.redoAction << _B::MakeRestoreGraphSizeCB();
		_B::AddUndoAction(undo);
	}


	void ContextMenu(Bar& bar)
	{
		
		bar.Add( t_("Copy"), GraphCtrlImg::COPY(),  THISBACK1(SaveToClipboard, false));//.Key(K_CTRL_C);
		//bar.Add( t_("Copy_post"), GraphCtrlImg::COPY(), THISBACK2(PostCallback, THISBACK1(SaveToClipboard, false), 0));
		//bar.Add( t_("Copy_100ms"), GraphCtrlImg::COPY(), THISBACK3(SetTimeCallback, 100, THISBACK1(SaveToClipboard, false), 0));
		bar.Add( t_("Save to file"), GraphCtrlImg::SAVE(), THISBACK1(SaveToFile, Null));

		bar.Separator();

		bar.Add( _B::_undoManager.CanUndo(), t_("Undo"), THISBACK(Undo));//.Key(K_CTRL_Z);
		bar.Add( _B::_undoManager.CanRedo(), t_("Redo"), THISBACK(Redo));//.Key(K_SHIFT_CTRL_Z);

		bar.Separator();

		bar.Add( IsScrollFromGraphEnabled() && IsZoomFromGraphEnabled(), t_("Fit To Data"),         THISBACK1(CtrlFitToData, GraphDraw_ns::ALL_SERIES));
		bar.Add( IsScrollFromGraphEnabled() && IsZoomFromGraphEnabled(), t_("Fit To Visible Data"), THISBACK1(CtrlFitToData, GraphDraw_ns::VISIBLE_SERIES_ONLY));

		bar.Separator();

		bar.Add( t_("Show ALL"), THISBACK1(ShowAllSeries, true) );
		bar.Add( t_("Hide ALL"), THISBACK1(ShowAllSeries, false) );
		for (int c=0; c < _B::series.GetCount(); c++) {
			String txt = t_("Show");
			 _B::series[c].UpdateSerieImage();
			txt += " : " + _B::series[c].legend;
			//bar.Add( txt, _B::series[c].serieIcon, THISBACK1(HideSeries, c)).Check(_B::IsVisible(c));
			bar.Add( txt, THISBACK1(HideSeries, c)).Check(_B::IsVisible(c));
		}
		
		bar.Separator();
		for (int c=0; c < _B::series.GetCount(); c++) {
			String txt = t_("Edit");
			txt += " : " + _B::series[c].legend;
			bar.Add(txt,  _B::series[c].GetSerieIcon(), THISBACK1(OpenSeriesPropertiesDlg, c));
		}
	}

	virtual void DoLocalLoop(GraphDraw_ns::MouseLocalLoopCB CB)
	{
		GraphCtrlLooper looper(*this);
		looper.WhenMouseMove << CB;
		looper.Run();
	}


	template<class RESULT, class CBCK>
	bool ProcessMouseCallBack(Point p, dword keyflags, CBCK cbck, RESULT& output ,RESULT defaultRes = 0)
	{
		for (int j = 0; j < _B::_drawElements.GetCount(); j++)
		{
			if ( !_B::_drawElements[j]->IsHidden() ) {
				if ( _B::_drawElements[j]->IsFloat() ) {
					if (_B::_drawElements[j]->Contains(p)) {
						output = ((_B::_drawElements[j]->*cbck)(p, keyflags));
						return true;
					}
				}
			}
		}
		for (int j = 0; j < _B::_drawElements.GetCount(); j++)
		{
			if ( !_B::_drawElements[j]->IsHidden() ) {
				if ( !_B::_drawElements[j]->IsFloat() ) {
					if (_B::_drawElements[j]->Contains(p)) {
						output = ((_B::_drawElements[j]->*cbck)(p, keyflags));
						return true;
					}
				}
			}
		}
		return false;
	}

	template<class CBCK>
	bool ProcessMouseCallBack(Point p, dword keyflags, CBCK cbck)
	{
		for (int j = 0; j < _B::_drawElements.GetCount(); j++)
		{
			if ( !_B::_drawElements[j]->IsHidden() ) {
				if ( _B::_drawElements[j]->IsFloat() ) {
					if (_B::_drawElements[j]->Contains(p)) {
						((_B::_drawElements[j]->*cbck)(p, keyflags));
						return true;
					}
				}
			}
		}
		for (int j = 0; j < _B::_drawElements.GetCount(); j++)
		{
			if ( !_B::_drawElements[j]->IsHidden() ) {
				if ( !_B::_drawElements[j]->IsFloat() ) {
					if (_B::_drawElements[j]->Contains(p)) {
						((_B::_drawElements[j]->*cbck)(p, keyflags));
						return true;
					}
				}
			}
		}
		return false;
	}


	virtual void LeftDown(Point p, dword keyflags) {
		prevMousePoint = p;
		if ( ProcessMouseCallBack(p, keyflags, &GraphDraw_ns::GraphElement::LeftDown) )
		{
			return;
		}
		if ( _B::_plotRect.Contains(p) ) {
			GraphDraw_ns::GraphUndoData undo;
			undo.undoAction << _B::MakeRestoreGraphSizeCB(); // PREV size before  SELECT ZOOM
			if ( TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_ZOOM) ) {
				// SELECT ZOOM
				if (!IsZoomFromGraphEnabled()) return;
				if (useLocalSelectLoop) {
					selectEndPoint = selectOriginPoint = p - _B::_plotRect.TopLeft();
					DoLocalLoop( THISBACK(LoopedPlotSelectCB) );
					_B::ZoomOnRect( _B::_selectRect );
					_B::_selectRect.Clear();
				}
				else {
					// ------------------------------------				
					RectTracker tracker(*this);
					Rect selectedZoomArea = tracker.Track( RectfC(p.x,p.y,0,0), ALIGN_NULL, ALIGN_NULL) - _B::_plotRect.TopLeft();
					if ( !selectedZoomArea.IsEmpty() ) {
						_B::ZoomOnRect( selectedZoomArea );
						Refresh();
					}
				}
				if ( p != GetMouseViewPos() ) {
					undo.redoAction << _B::MakeRestoreGraphSizeCB(); // NEW size after  MOVE
					_B::AddUndoAction(undo);
				}
			}
			else if ( TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_SCROLL ) ) {
				if (!IsScrollFromGraphEnabled()) return;
				DoLocalLoop( THISBACK(LoopedPlotScrollCB) );
				if ( p != GetMouseViewPos() ) {
					undo.redoAction << _B::MakeRestoreGraphSizeCB(); // NEW size after  MOVE
					_B::AddUndoAction(undo);
				}
			}
		}
	}

	virtual void LeftDouble(Point p, dword keyflags) {
		if ( ProcessMouseCallBack(p, keyflags, &GraphDraw_ns::GraphElement::LeftDouble) ) return;
		if ( _B::_plotRect.Contains(p) ) {
			return;
		}
	}

	virtual void RightDown(Point p, dword keyflags) {
		if ( ProcessMouseCallBack(p, keyflags, &GraphDraw_ns::GraphElement::RightDown)) {
			return;
		}
		if ( _B::_plotRect.Contains(p) ) {
			MenuBar::Execute(THISBACK(ContextMenu));
			return;
		}
	}

	virtual void RightDouble(Point p, dword keyflags) {
		if ( ProcessMouseCallBack(p, keyflags, &GraphDraw_ns::GraphElement::RightDouble) ) {
			return;
		}
		if ( _B::_plotRect.Contains(p) ) {
			return;
		}
	}

	virtual void MiddleDown(Point p, dword keyflags) {
		if ( ProcessMouseCallBack(p, keyflags, &GraphDraw_ns::GraphElement::MiddleDown) ) {
			return;
		}
		if ( _B::_plotRect.Contains(p) ) {
			// do PLOT  CENTER  at point
			return;
		}
	}

	private:
	void LoopedPlotScrollCB(Point p, dword keyflags) {
		_B::_doFastPaint = true;
		if (p != prevMousePoint) {
			if ( !isXScrollAllowed ) p.x = prevMousePoint.x;
			if ( !isYScrollAllowed ) p.y = prevMousePoint.y;
			_B::Scroll(p.x-prevMousePoint.x, p.y-prevMousePoint.y);
			SetModify();
		}
		prevMousePoint = p;
		Refresh();
	}
	
	void LoopedPlotSelectCB(Point p, dword keyflags) {
		selectEndPoint = p - _B::_plotRect.TopLeft();
		if (p != prevMousePoint) {
			_B::_selectRect.left   = Upp::min(selectEndPoint.x, selectOriginPoint.x);
			_B::_selectRect.right  = Upp::max(selectEndPoint.x, selectOriginPoint.x);
			_B::_selectRect.top    = Upp::min(selectEndPoint.y, selectOriginPoint.y);
			_B::_selectRect.bottom = Upp::max(selectEndPoint.y, selectOriginPoint.y);
		}

		if ( !isXZoomAllowed ) {
			_B::_selectRect.left = 0;
			_B::_selectRect.right = _B::_plotRect.GetWidth();
		}
		
		if ( !isYZoomAllowed ) {
			_B::_selectRect.top = 0;
			_B::_selectRect.bottom = _B::_plotRect.GetHeight();
		}
		

		Refresh();
	}
	
	public:
	virtual void MouseMove(Point p, dword keyflags) {
		RLOGBLOCK_STR( _B::debugTrace, "CRTP_GraphCtrl_Base::MouseMove(" << this << ")");
		if ( ProcessMouseCallBack(p, keyflags, &GraphDraw_ns::GraphElement::MouseMove)) {}
		if (_B::_doFastPaint) {
			// Do complete drawing when nothing special to be done
			_B::_doFastPaint = false;
			Refresh();
		}
	}


	virtual Image  CursorImage(Point p, dword keyflags)
	{
		RLOGBLOCK_STR( _B::debugTrace, "CRTP_GraphCtrl_Base::CursorImage(" << this << ")");
		Image output;
		if ( ProcessMouseCallBack<Image>(p, keyflags, &GraphDraw_ns::GraphElement::CursorImage, output, GraphDrawImg::CROSS())) {
			return output;
		}
		if ( _B::_plotRect.Contains(p) ) {
			if      ( TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_ZOOM)   && IsZoomFromGraphEnabled() )   return GraphCtrlImg::ZOOM();
			else if ( TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_SCROLL) && IsScrollFromGraphEnabled() ) return GraphCtrlImg::SCROLL();
		}
		return GraphDrawImg::CROSS();
	}



	virtual void MouseWheel(Point p, int zdelta, dword keyflags)
	{
		// Process FLOAT elements FIRST
		for (int j = 0; j < _B::_drawElements.GetCount(); j++)
		{
			if ( _B::_drawElements[j]->IsFloat() ) {
				if (_B::_drawElements[j]->Contains(p)) {
					(_B::_drawElements[j]->MouseWheel)(p, zdelta, keyflags);
					return;
				}
			}
		}
		for (int j = 0; j < _B::_drawElements.GetCount(); j++)
		{
			if ( !_B::_drawElements[j]->IsFloat() ) {
				if (_B::_drawElements[j]->Contains(p)) {
					(_B::_drawElements[j]->MouseWheel)(p, zdelta, keyflags);
					return;
				}
			}
		}
		if ( _B::_plotRect.Contains(p) ) {
			if (  TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_ZOOM) ) // => WHEEL ZOOM
			{
				if ( !IsZoomFromGraphEnabled() ) return;
				GraphDraw_ns::GraphUndoData undo;
				undo.undoAction << _B::MakeRestoreGraphSizeCB(); // PREV size before  MOVE
					if (zdelta < 0) _B::ApplyZoomFactor(1.2);
					else            _B::ApplyInvZoomFactor(1.2);
					_B::_doFastPaint = true;
				undo.redoAction << _B::MakeRestoreGraphSizeCB(); // NEW size after  MOVE
				_B::AddUndoAction(undo);
			}
			return;
		}
	}
};

#endif
