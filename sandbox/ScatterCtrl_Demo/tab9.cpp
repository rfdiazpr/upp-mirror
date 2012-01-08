#include "ScatterCtrl_Demo.h"


Tab9::Tab9()
{
	CtrlLayout(*this);	
	HSizePos().VSizePos();
	scatter.SetRange(100000,40).SetXYMin(0, -20);
	scatter.SetMouseHandling(true).SetMaxZoom(500000).SetMinZoom(2);
	for (int t = 0; t < 100000; ++t) {
		s1 <<Pointf(t,20*sin(2*M_PI*t/100000));
		s2 <<Pointf(t,15*cos(2*M_PI*t/100000));
	}
	scatter.AddSeries(s1).Legend("series1");
	scatter.AddSeries(s2).Legend("series2");
	scatter.ShowInfo();	
	fastView.WhenAction = THISBACK(OnFastView);
	sequentialX.WhenAction = THISBACK(OnSequentialX);
	fastView = true;
	sequentialX = true;
	OnFastView();
	OnSequentialX();
}