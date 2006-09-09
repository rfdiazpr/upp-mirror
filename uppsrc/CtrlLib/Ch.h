enum {
	CH_STYLE_DETECT,
	CH_STYLE_STD,
	CH_STYLE_CLASSIC,
};

void   ChSetStyle(int q);
int    ChGetStyle();

int    ChGetStyleCount();
String ChGetStyleName(int q);

void ChSysInit();

void ChDetectStyle();
void ChSetStyleStd();
void ChSetStyleClassic();

void ChSync();
