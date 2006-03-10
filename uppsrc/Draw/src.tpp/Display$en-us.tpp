TITLE("class Display")
REF("::Display::class")
REF("::Display::FOCUS")
REF("::Display::SELECT")
REF("::Display::READONLY")
REF("::Display::Paint(::Draw&,const::Rect&,const::Value&,::Color,::Color,::dword)const")
REF("::Display::PaintBackground(::Draw&,const::Rect&,const::Value&,::Color,::Color,::dword)const")
REF("::Display::GetStdSize(const::Value&)const")
REF("::Display::RatioSize(const::Value&,int,int)const")
REF("::AttrText::struct")
REF("::AttrText::text")
REF("::AttrText::font")
REF("::AttrText::ink")
REF("::AttrText::paper")
REF("::AttrText::Ink(::Color)")
REF("::AttrText::Paper(::Color)")
REF("::AttrText::SetFont(::Font)")
REF("::AttrText::operator::Value()const")
REF("::AttrText::::AttrText(const char*)")
REF("::AttrText::::AttrText(const::wchar*)")
REF("::AttrText::::AttrText(const::WString&)")
REF("::PaintRect::class")
REF("::PaintRect::Paint(::Draw&,const::Rect&,::Color,::Color,::dword)const")
REF("::PaintRect::Paint(::Draw&,int,int,int,int,::Color,::Color,::dword)const")
REF("::PaintRect::GetStdSize()const")
REF("::PaintRect::RatioSize(int,int)const")
REF("::PaintRect::RatioSize(::Size)const")
REF("::PaintRect::SetDisplay(const::Display&)")
REF("::PaintRect::SetValue(const::Value&)")
REF("::PaintRect::Set(const::Display&,const::Value&)")
REF("::PaintRect::Clear()")
REF("::PaintRect::GetValue()const")
REF("::PaintRect::GetDisplay()const")
REF("::PaintRect::operator bool()const")
REF("::PaintRect::::PaintRect()")
REF("::PaintRect::::PaintRect(const::Display&)")
REF("::PaintRect::::PaintRect(const::Display&,const::Value&)")
TOPIC_TEXT(
"[2 $$0,0#00000000000000000000000000000000:Default][l288;i1120;a17;O9;~~~.1408;2 "
"$$1,0#10431211400427159095818037425705:param][a83;*R6 $$2,5#31310162474203024125188417583966:caption][b83;*4 "
"$$3,5#07864147445237544204411237157677:title][i288;O9;C2 $$4,6#40027414424643823182269349404212:item][b42;a42;2 "
"$$5,5#45413000475342174754091244180557:text][l288;b17;a17;2 $$6,6#27521748481378242620020725143825:")
TOPIC_TEXT(
"desc][l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code][b2503; $$8,0#65142375456100023862071332075487:separator][*@(0.0.255)2 "
"$$9,0#83433469410354161042741608181528:base][t4167;C $$10,0#37138531426314131251341829483380:class][l288;a17;*1 "
"$$11,11#70004532496200323422659154056402:requirement][i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam][b167;C2 "
"$$13,13#924304")
TOPIC_TEXT(
"59443460461911108080531343:item1][i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2][*@2$(0.128.128) "
"$$15,15#34511555403152284025741354420178:NewsDate][l321;*C$7;2 $$16,16#03451589433145915344929335295360:result][l321;b83;a83;*C$7;2 "
"$$17,17#07531550463529505371228428965313:result`-line][l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title][ "
"$$19,0#535800234423355290")
TOPIC_TEXT(
"39900623488521:gap][t4167;C2 $$20,20#70211524482531209251820423858195:class`-nested][b50;2 "
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s10;:`:`:Display`:`:class:* "
"[%00-00* class_][%00-00 Display]&][s6; Display and Display`-derived classes render "
"Value into the specified rectangular area. References to Displays are used in many "
"widgets as attributes affecting the rendering of widget Va")
TOPIC_TEXT(
"lues.&][s6; Base Display class implements rendering of Value converted to text and "
"also rendering of special AttrText type (text with font and color attributes).&][s0; "
"&][s0; [* Visual style constants ]are used as `\"style`\" parameter bit flags of "
"rendering methods and provide additional information about required visual appearance:&][s0;* "
"&][s0;l288;~~~.1408; [%00-00*C CURSOR][%00-00* -|][%00-00 G")
TOPIC_TEXT(
"ui element is current (`\"has cursor`\").]&][s4;l288;i0;:`:`:Display`:`:FOCUS:~~~.1408;A "
"[%00-00*C FOCUS][* -|]Gui element has focus.&][s4;l288;:`:`:Display`:`:SELECT:~~~.1408;A "
"[%00-00*C SELECT][%00-00 -|Gui element is selected.]&][s4;l288;:`:`:Display`:`:READONLY:~~~.1408;A "
"[%00-00*C READONLY][%00-00 -|Gui element is read`-only.]&][s0; &][s4;:`:`:Display`:`:Paint`(`:`:Draw`&`,const`:`:Rect`&`,cons")
TOPIC_TEXT(
"t`:`:Value`&`,`:`:Color`,`:`:Color`,`:`:dword`)const: [%00-00 virtual void_][%00-00* "
"Paint][%00-00 (Draw`&_][%00-00*@3 w][%00-00 , const_Rect`&_][%00-00*@3 r][%00-00 "
", const_Value`&_][%00-00*@3 q][%00-00 , Color_][%00-00*@3 ink][%00-00 , Color_][%00-00*@3 "
"paper][%00-00 , dword_][%00-00*@3 style][%00-00 )_const]&][s6; This virtual method "
"is used to paint rectangle content according to specified Val")
TOPIC_TEXT(
"ue. The base class implementation tries to convert the Value into the textual representation "
"using AsString functions and display it as text, unless it is of AttrText type. Note "
"that it is OK for derived class to understand just Value types it was designed for "
"(and crash otherwise) `- it is client code responsibility to use the correct Display.&][s1; "
"[%00-00*C@3 w]-|Draw.&][s1; [%00-00*C@3 r]-|Tar")
TOPIC_TEXT(
"get rectangle.&][s1; [%00-00*C@3 q]-|Value to be painted.&][s1; [%00-00*C@3 ink]-|Suggested "
"foreground color.&][s1; [%00-00*C@3 paper]-|Suggested background color.&][s1; [%00-00*C@3 "
"style]-|Visual style.&][s0; &][s4;:`:`:Display`:`:PaintBackground`(`:`:Draw`&`,const`:`:Rect`&`,const`:`:Value`&`,`:`:Color`,`:`:Color`,`:`:dword`)const: "
"[%00-00 virtual void_][%00-00* PaintBackground][%00-00 (Draw`&_]")
TOPIC_TEXT(
"[%00-00*@3 w][%00-00 , const_Rect`&_][%00-00*@3 r][%00-00 , const_Value`&_][%00-00*@3 "
"q][%00-00 , Color_][%00-00*@3 ink][%00-00 , Color_][%00-00*@3 paper][%00-00 , dword_][%00-00*@3 "
"style][%00-00 )_const]&][s6; This virtual method is used to paint areas of GUI element "
"that are outside of rectangle specified in Paint method, but should have color related "
"somewhat to current Display class `- usually")
TOPIC_TEXT(
" this color is the same as background painted in Paint. (Note that Paint method "
"must clear the background separately, although base Display class is defined to use "
"PaintBackground for this task).&][s1; [%00-00*C@3 w]-|Draw.&][s1; [%00-00*C@3 r]-|Target "
"rectangle.&][s1; [%00-00*C@3 q]-|Value to be painted.&][s1; [%00-00*C@3 ink]-|Suggested "
"foreground color.&][s1; [%00-00*C@3 paper]-|Suggested backg")
TOPIC_TEXT(
"round color.&][s1; [%00-00*C@3 style]-|Visual style.&][s1;*C@3 &][s4;:`:`:Display`:`:GetStdSize`(const`:`:Value`&`)const: "
"[%00-00 virtual Size_][%00-00* GetStdSize][%00-00 (const_Value`&_][%00-00*@3 q][%00-00 "
")_const]&][s6; Should return standard size for given value and Display. E.g. if Display "
"is rendering Images, it should return the Size of the Image in pixels. Base Display "
"returns the size of")
TOPIC_TEXT(
" textual representation of the Value.&][s1; [%00-00*C@3 q]-|Value.&][s1; [*/ Return "
"value]-|Size of Value for Display.&][s4;:`:`:Display`:`:RatioSize`(const`:`:Value`&`,int`,int`)const: "
"&][s4; [%00-00 virtual Size_][%00-00* RatioSize][%00-00 (const_Value`&_][%00-00*@3 "
"q][%00-00 , int_][%00-00*@3 cx][%00-00 , int_][%00-00*@3 cy][%00-00 )_const]&][s6; "
"Returns size for defined value keeping the aspec")
TOPIC_TEXT(
"t ratio.&][s1; [%00-00*C@3 q]-|Value.&][s1; [%00-00*C@3 cx]-|Required width. If "
"zero, it should be computed to keep aspect ratio with cy.&][s1; [%00-00*C@3 cy]-|Required "
"height. If zero, it should be computed to keep aspect ratio with cx.&][s1; [*/ Return "
"value]-|Size of Value for Display.&][s0; &][s0; &][s0; &][s10;:`:`:AttrText`:`:struct:* "
"[%00-00* struct_][%00-00 AttrText]&][s6; Simple helper c")
TOPIC_TEXT(
"lass convertible to the Value. Standard base Display detects whether Value passed "
"in is of AttrText type and handles it differently by adopting non`-null attributes "
"for the text painted.&][s6; &][s4;:`:`:AttrText`:`:text:* [%00-00* WString_][%00-00 "
"text]&][s6; Text to be displayed.&][s0; &][s4;:`:`:AttrText`:`:font:* [%00-00* Font_][%00-00 "
"font]&][s6; Font of text.&][s0; &][s4;:`:`:AttrText`:`:ink")
TOPIC_TEXT(
":* [%00-00* Color_][%00-00 ink]&][s6; Text color.&][s0; &][s4;:`:`:AttrText`:`:paper:* "
"[%00-00* Color_][%00-00 paper]&][s6; Background color&][s0; &][s4;:`:`:AttrText`:`:Ink`(`:`:Color`): "
"[%00-00 AttrText`&_][%00-00* Ink][%00-00 (Color_][%00-00*@3 c][%00-00 )]&][s6; Sets "
"the text color.&][s1; [%00-00*C@3 c]-|The color.&][s1; [*/ Return value]-|`*this.&][s0; "
"&][s4;:`:`:AttrText`:`:Paper`(`:`:Color`")
TOPIC_TEXT(
"): [%00-00 AttrText`&_][%00-00* Paper][%00-00 (Color_][%00-00*@3 c][%00-00 )]&][s6; "
"Sets the paper color.&][s1; [%00-00*C@3 c]-|The color.&][s1; [*/ Return value]-|`*this.&][s0; "
"&][s4;:`:`:AttrText`:`:SetFont`(`:`:Font`): [%00-00 AttrText`&_][%00-00* SetFont][%00-00 "
"(Font_][%00-00*@3 f][%00-00 )]&][s6; Sets the font.&][s1; [%00-00*C@3 f]-|The font.&][s1; "
"[*/ Return value]-|`*this.&][s0; &][s4;:`:`")
TOPIC_TEXT(
":AttrText`:`:operator`:`:Value`(`)const: [%00-00 operator_Value()_const]&][s1; [*/ "
"Return value]-|AttrText as raw Value.&][s0; &][s4;:`:`:AttrText`:`:`:`:AttrText`(const "
"char`*`): [%00-00* AttrText][%00-00 (const_char_`*][%00-00*@3 text][%00-00 )]&][s6; "
"Constructs AttrText, assigning the text attribute and all other attributes to zero.&][s0; "
"&][s4;:`:`:AttrText`:`:`:`:AttrText`(const`:`:wchar`*`):")
TOPIC_TEXT(
" [%00-00* AttrText][%00-00 (const_wchar_`*][%00-00*@3 text][%00-00 )]&][s6; Constructs "
"AttrText, assigning the text attribute and all other attributes to zero.&][s0; &][s4;:`:`:AttrText`:`:`:`:AttrText`(const`:`:WString`&`): "
"[%00-00* AttrText][%00-00 (const_WString`&_][%00-00*@3 text][%00-00 )]&][s6; Constructs "
"AttrText, assigning the text attribute and all other attributes to zero.&][s0;3 &][s0;3")
TOPIC_TEXT(
" &][s0;3 &][s0;3 Standard Displays&][s5; Standard Displays are implemented as `\"functional "
"globals`\" `- functions returning constant reference to single global Display instance.&][s0; "
"&][ {{3784:6216h1;@(204) [s0; Display name]:: [s0; Description]::^@2 [s0;* StdDisplay]::= "
"[s0; Base Display. Displays Value as text, unless it is AttrText (see above)]::^ "
"[s0;* ColorDisplay]::= [s0; Displays Color (r")
TOPIC_TEXT(
"equired) `- simply paints background using the Value passed in.]::^ [s0;* SizeTextDisplay]::= "
"[s0; Similar to StdDisplay, but stretches the text size to fill whole display area.]::^ "
"[s0;* ImageDisplay]::= [s0; Displays Image passed in as Value, aligns it to the top`-left "
"corner.]::^ [s0;* FittedImageDisplay]::= [s0; Displays Image scaled to fit the rectangle.]::^ "
"[s0;* CenteredImageDisplay]::= [s0")
TOPIC_TEXT(
"; Displays Image centered in the rectangle.]::^ [s0;* CenteredHighlightImageDisplay]::= "
"[s0; Displays Image centered in the rectangle with 1 pixel wide white border.]::^ "
"[s0;* DrawingDisplay]::= [s0; Displays Drawing scaled to fit the rectangle.]}}&][s0;3 "
"&][s0; &][s0; &][s10;:`:`:PaintRect`:`:class:* [%00-00* class_][%00-00 PaintRect]&][s6; "
"PaintRect is a simple helper class that combines Value a")
TOPIC_TEXT(
"nd a reference to the Display to provide `\"visual content`\" of rectangle. It is "
"[%00-00*@(0.0.255) Moveable].&][s0;*@(128.0.255) &][s4;:`:`:PaintRect`:`:Paint`(`:`:Draw`&`,const`:`:Rect`&`,`:`:Color`,`:`:Color`,`:`:dword`)const: "
"[%00-00 void_][%00-00* Paint][%00-00 (Draw`&_][%00-00*@3 w][%00-00 , const_Rect`&_][%00-00*@3 "
"r][%00-00 , Color_][%00-00*@3 ink][%00-00 _`=_SBlack, Color_][%00-00*@3 paper")
TOPIC_TEXT(
"][%00-00 _`=_SWhite, dword_][%00-00*@3 style][%00-00 _`=_][%00-00@3 0][%00-00 )_const]&][s6; "
"Invokes Paint of contained Display for contained Value.&][s1; [%00-00*C@3 w]-|Draw.&][s1; "
"[%00-00*C@3 r]-|Target rectangle.&][s1; [%00-00*C@3 ink]-|Suggested foreground color.&][s1; "
"[%00-00*C@3 paper]-|Suggested background color.&][s1; [%00-00*C@3 style]-|Visual "
"style.&][s0; &][s4;:`:`:PaintRect`:`:Paint`(")
TOPIC_TEXT(
"`:`:Draw`&`,int`,int`,int`,int`,`:`:Color`,`:`:Color`,`:`:dword`)const: [%00-00 "
"void_][%00-00* Paint][%00-00 (Draw`&_][%00-00*@3 w][%00-00 , int_][%00-00*@3 x][%00-00 "
", int_][%00-00*@3 y][%00-00 , int_][%00-00*@3 cx][%00-00 , int_][%00-00*@3 cy][%00-00 "
", Color_][%00-00*@3 ink][%00-00 _`=_SBlack, Color_][%00-00*@3 paper][%00-00 _`=_SWhite, "
"dword_][%00-00*@3 style][%00-00 _`=_][%00-00@3 0][%00-00 )_")
TOPIC_TEXT(
"const]&][s6; Invokes contained Display with contained Value.&][s1; [%00-00*C@3 w]-|Draw.&][s1; "
"[%00-00*C@3 x, y, cx, cy]-|Target rectangle (left, top, width, height).&][s1; [%00-00*C@3 "
"ink]-|Suggested foreground color.&][s1; [%00-00*C@3 paper]-|Suggested background "
"color.&][s1; [%00-00*C@3 style]-|Visual style.&][s0; &][s4;:`:`:PaintRect`:`:GetStdSize`(`)const: "
"[%00-00 Size_][%00-00* GetStdSize][%")
TOPIC_TEXT(
"00-00 ()_const]&][s6; Invokes GetStdSize of contained Display for contained Value.&][s1; "
"[*/ Return value]-|Preferred Size of Value.&][s0; &][s4;:`:`:PaintRect`:`:RatioSize`(int`,int`)const: "
"[%00-00 Size_][%00-00* RatioSize][%00-00 (int_][%00-00*@3 cx][%00-00 , int_][%00-00*@3 "
"cy][%00-00 )_const]&][s6; Invokes RatioSize of contained Display for contained Value.&][s1; "
"[%00-00*C@3 cx]-|Required widt")
TOPIC_TEXT(
"h. If zero, it should be computed to keep aspect ratio with cy.&][s1; [%00-00*C@3 "
"cy]-|Required height. If zero, it should be computed to keep aspect ratio with cx.&][s1; "
"[*/ Return value]-|Size of Value for Display.&][s0; &][s4;:`:`:PaintRect`:`:RatioSize`(`:`:Size`)const: "
"[%00-00 Size_][%00-00* RatioSize][%00-00 (Size_][%00-00*@3 sz][%00-00 )_const]&][s6; "
"Equivalent to RatioSize(sz.cx, sz.cy).&]")
TOPIC_TEXT(
"[s0; &][s4;:`:`:PaintRect`:`:SetDisplay`(const`:`:Display`&`): [%00-00 void_][%00-00* "
"SetDisplay][%00-00 (const_Display`&_][%00-00*@3 d][%00-00 )]&][s6; Sets the Display.&][s1; "
"[%00-00*C@3 d]-|Display.&][s0; &][s4;:`:`:PaintRect`:`:SetValue`(const`:`:Value`&`): "
"[%00-00 void_][%00-00* SetValue][%00-00 (const_Value`&_][%00-00*@3 v][%00-00 )]&][s6; "
"Sets the Value.&][s1; [%00-00*C@3 v]-|Value.&][s0; &")
TOPIC_TEXT(
"][s4;:`:`:PaintRect`:`:Set`(const`:`:Display`&`,const`:`:Value`&`): [%00-00 void_][%00-00* "
"Set][%00-00 (const_Display`&_][%00-00*@3 d][%00-00 , const_Value`&_][%00-00*@3 v][%00-00 "
")]&][s6; Sets the Display and the Value.&][s1; [%00-00*C@3 d]-|Display.&][s1; [%00-00*C@3 "
"v]-|Value.&][s0; &][s4;:`:`:PaintRect`:`:Clear`(`): [%00-00 void_][%00-00* Clear][%00-00 "
"()]&][s6; Removes the Display `- subseque")
TOPIC_TEXT(
"nt calls to Paint act as `\"no operation`\", calls to GetStdSize and RatioSize return "
"Size(0, 0).&][s0; &][s4;:`:`:PaintRect`:`:GetValue`(`)const: [%00-00 const_Value`&_][%00-00* "
"GetValue][%00-00 ()_const]&][s6; Returns the Value.&][s1; [*/ Return value]-|Value.&][s0; "
"&][s4;:`:`:PaintRect`:`:GetDisplay`(`)const: [%00-00 const_Display`&_][%00-00* GetDisplay][%00-00 "
"()_const]&][s6; Returns the Display")
TOPIC_TEXT(
".&][s1; [*/ Return value]-|Display.&][s0; &][s4;:`:`:PaintRect`:`:operator bool`(`)const: "
"[%00-00 operator_bool()_const]&][s1; [*/ Return value]-|true if Display is set.&][s0; "
"&][s4;:`:`:PaintRect`:`:`:`:PaintRect`(`): [%00-00* PaintRect][%00-00 ()]&][s6; Constructs "
"empty PaintRect, with no Display assigned.&][s0; &][s4;:`:`:PaintRect`:`:`:`:PaintRect`(const`:`:Display`&`): "
"[%00-00* PaintRect][%00")
TOPIC_TEXT(
"-00 (const_Display`&_][%00-00*@3 display][%00-00 )]&][s6; Constructs PaintRect with "
"specified Display.&][s1; [%00-00*C@3 display]-|Display.&][s0; &][s4;:`:`:PaintRect`:`:`:`:PaintRect`(const`:`:Display`&`,const`:`:Value`&`): "
"[%00-00* PaintRect][%00-00 (const_Display`&_][%00-00*@3 display][%00-00 , const_Value`&_][%00-00*@3 "
"val][%00-00 )]&][s6; Constructs PaintRext with specified Display and Value.")
TOPIC_TEXT(
"&][s1; [%00-00*C@3 display]-|Display.&][s1; [%00-00*C@3 val]-|Value.&][s0; &][s0;3 "
"]")
