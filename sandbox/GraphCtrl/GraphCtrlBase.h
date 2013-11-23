#ifndef _GraphCtrl_GraphCtrlBase_h_
#define _GraphCtrl_GraphCtrlBase_h_



// ============================================================================================
//                        LOOP DELEGATION CLASS
// ============================================================================================

class GraphCtrlLooper : public LocalLoop {
	private:
	Image cursorImg;
	
	virtual void  LeftUp(Point, dword) { EndLoop(); }
	virtual void  RightUp(Point, dword) { EndLoop(); }
	virtual void  MouseMove(Point p, dword f)  { WhenMouseMove(p, f); };
	virtual Image CursorImage(Point p, dword keyflags) {
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
	Image ctrlImgSave; // to save ctrl image
	Point prevMousePoint;
	int copyRatio;
	int autoWaitCursorAvg;
	
	public:

	CRTP_GraphCtrl_Base()
	: copyRatio(3)
	{
		SetModify();
		_B::setScreenSize( GetSize() );
	}

	CRTP_GraphCtrl_Base(const CRTP_GraphCtrl_Base& p)
	: _B(p)
	, copyRatio(p.copyRatio)
	{
		SetModify();
		_B::setScreenSize( GetSize() );
	}

	virtual Value GetParentCtrl() {
		return RawToValue(static_cast<Ctrl*>(this));
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
		_B::setScreenSize( GetSize() );
		if (_B::_drawMode == GraphDraw_ns::MD_DRAW) {
			ImageDraw ib(GetSize());
			_B::Paint(ib, 1);
			//ctrlImgSave = ib;
			w.DrawImage(0, 0, ib);
		}
		else {
			ImageBuffer ib(GetSize());
			BufferPainter bp(ib, _B::_drawMode);
			_B::Paint(bp, 1);
			//ctrlImgSave = ib;
			w.DrawImage(0, 0, ib);
		}
	}


	// Refresh called from child
	virtual void RefreshFromChild( GraphDraw_ns::RefreshStrategy strategy ) {
		if      (strategy == GraphDraw_ns::REFRESH_FAST)      _B::_doFastPaint = true;
		else if (strategy != GraphDraw_ns::REFRESH_KEEP_DATA)  _B::_PlotDrawImage.Clear();// _B::_keepDataPaint = true;
		SetModify();
		Refresh();
	};


	virtual void Paint(Draw& w) {
		if ( _B::_doFastPaint == false ) {
			//if ( IsModified() ) {
				AutoWaitCursor waitcursor(autoWaitCursorAvg);
				Paint2(w);
				ClearModify();
//			}
//			else {
//				w.DrawImage(0, 0, ctrlImgSave);
//			}
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
	
	void OpenSeriesPropertiesDlg(int c) {
		WithSeriesPropertiesDlgLayout<TopWindow> dlg;
		
		//dlg.lineDash.SetDrawMode(_B::GetDrawMode());
		//dlg.lineThickness.SetDrawMode(_B::GetDrawMode());
		
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
		GraphDraw_ns::GraphUndoData undo;
		undo.undoAction << _B::MakeRestoreGraphSizeCB();
			_B::FitToData( fitStrategy );
		undo.redoAction << _B::MakeRestoreGraphSizeCB();
		_B::AddUndoAction(undo);
	}


	void ContextMenu(Bar& bar)
	{
		bar.Add( t_("Copy"), GraphCtrlImg::COPY(), 		  THISBACK1(SaveToClipboard, false));//.Key(K_CTRL_C);
		bar.Add( t_("Save to file"), GraphCtrlImg::SAVE(), THISBACK1(SaveToFile, Null));

		bar.Separator();

		bar.Add( _B::_undoManager.CanUndo(), t_("Undo"), THISBACK(Undo));//.Key(K_CTRL_Z);
		bar.Add( _B::_undoManager.CanRedo(), t_("Redo"), THISBACK(Redo));//.Key(K_SHIFT_CTRL_Z);

		bar.Separator();

		bar.Add( t_("Fit To Data"),         THISBACK1(CtrlFitToData, GraphDraw_ns::ALL_SERIES));
		bar.Add( t_("Fit To Visible Data"), THISBACK1(CtrlFitToData, GraphDraw_ns::VISIBLE_SERIES_ONLY));

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
		else if ( _B::_plotRect.Contains(p) ) {
			if ( keyflags & K_CTRL )
			{
				// SELECT ZOOM
				GraphDraw_ns::GraphUndoData undo;
				undo.undoAction << _B::MakeRestoreGraphSizeCB(); // PREV size before  SELECT ZOOM
				RectTracker tracker(*this);
				Rect selectedZoomArea = tracker.Track( RectfC(p.x,p.y,0,0), ALIGN_NULL, ALIGN_NULL) - _B::_plotRect.TopLeft();
				if (selectedZoomArea.Width() !=0  && selectedZoomArea.Height() != 0) {
					_B::ZoomOnRect( selectedZoomArea );
					SetModify();
					undo.redoAction << _B::MakeRestoreGraphSizeCB(); // NEW size after  SELECT ZOOM
					_B::AddUndoAction(undo);
					Refresh();
				}
			}
			return;
		}
	}

	virtual void LeftDouble(Point p, dword keyflags) {
		if ( ProcessMouseCallBack(p, keyflags, &GraphDraw_ns::GraphElement::LeftDouble) ) {
			return;
		}
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
	void LoopedPlotScrollMouseMove(Point p, dword keyflags) {
				// LEFT SCROLL
				_B::_doFastPaint = true;
				_B::Scroll(p.x-prevMousePoint.x, p.y-prevMousePoint.y);
				prevMousePoint = p;
				Refresh();
	}
	
	public:
	virtual void MouseMove(Point p, dword keyflags) {
		if ( ProcessMouseCallBack(p, keyflags, &GraphDraw_ns::GraphElement::MouseMove)) {}
		else if ( _B::_plotRect.Contains(p) )  {
			if ( keyflags & K_MOUSELEFT ) {
				GraphDraw_ns::GraphUndoData undo;
				undo.undoAction << _B::MakeRestoreGraphSizeCB(); // PREV size before  MOVE
					DoLocalLoop( THISBACK(LoopedPlotScrollMouseMove) );
				undo.redoAction << _B::MakeRestoreGraphSizeCB(); // NEW size after  MOVE
				_B::AddUndoAction(undo);
			}
			else {
				if (_B::_doFastPaint) { // Do complete drawing when SCROLLING FINISHED
					_B::_doFastPaint = false;
					Refresh();
				}
			}
			return;
		}

		if (_B::_doFastPaint) {
			// Do complete drawing when nothing special to be done
			_B::_doFastPaint = false;
			Refresh();
		}
	}


	virtual Image  CursorImage(Point p, dword keyflags)
	{
		Image output;
		if ( ProcessMouseCallBack<Image>(p, keyflags, &GraphDraw_ns::GraphElement::CursorImage, output, GraphDrawImg::CROSS())) {
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
			if ( keyflags & K_CTRL ) // => WHEEL ZOOM
			{
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
