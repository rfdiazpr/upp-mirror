TITLE("LabelBase - basic label routines")
REF("::DeAmp(const char*)")
REF("::GetSmartTextSize(::Draw&,const char*,::Font")
REF("::GetSmartTextHeight(::Draw&,const char*,int,::Font")
REF("::DrawSmartText(::Draw&,int,int,int,const char*,::Font")
REF("::ExtractAccessKey(const char*,::String&)")
REF("::CompareAccessKey(::byte,::dword)")
REF("::ChooseAccessKey(const char*,::dword)")
REF("::DrawLabel::struct")
REF("::DrawLabel::DrawLabel()")
REF("::DrawLabel::GetSize()const")
REF("::DrawLabel::Paint(::Draw&,const::Rect&,bool)const")
REF("::DrawLabel::Paint(::Draw&,int,int,int,int,bool)const")
REF("::DrawLabel::push")
REF("::DrawLabel::focus")
REF("::DrawLabel::disabled")
REF("::DrawLabel::paintrect")
REF("::DrawLabel::limg")
REF("::DrawLabel::lcolor")
REF("::DrawLabel::lspc")
REF("::DrawLabel::text")
REF("::DrawLabel::font")
REF("::DrawLabel::ink")
REF("::DrawLabel::rimg")
REF("::DrawLabel::rcolor")
REF("::DrawLabel::rspc")
REF("::DrawLabel::align")
REF("::DrawLabel::valign")
REF("::DrawLabel::accesskey")
REF("::LabelBase::class")
REF("::LabelBase::LabelUpdate()")
REF("::LabelBase::SetLeftImage(const::Image&,int)")
REF("::LabelBase::SetPaintRect(const::PaintRect&)")
REF("::LabelBase::SetText(const char*)")
REF("::LabelBase::SetFont(::Font)")
REF("::LabelBase::SetInk(::Color)")
REF("::LabelBase::SetRightImage(const::Image&,int)")
REF("::LabelBase::SetAlign(int)")
REF("::LabelBase::SetVAlign(int)")
REF("::LabelBase::SetImage(const::Image&,int)")
REF("::LabelBase::GetAlign()const")
REF("::LabelBase::GetVAlign()const")
REF("::LabelBase::GetPaintRect()const")
REF("::LabelBase::GetText()const")
REF("::LabelBase::GetFont()const")
REF("::LabelBase::GetInk()const")
REF("::LabelBase::PaintLabel(::Draw&,const::Rect&,bool,bool,bool,bool)")
REF("::LabelBase::PaintLabel(::Draw&,int,int,int,int,bool,bool,bool,bool)")
REF("::LabelBase::GetLabelSize()const")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2; LabelBase `- basic label "
"routines&][s5; LabelBase.h contains functions and simple helper classes used to implement "
"Ctrls with text, images and accelerator keys.&][s5; Generally, format of text used "
"by LabelBase facilities is either simple multi`-li")
TOPIC_TEXT(
"ne or [^topic`:`/`/RichText`/srcdoc`/QTF`$en`-us^ QTF]:&][s5;i150;O2; If first character "
"of text is `'`\\1`', text is QTF (starting with next character).&][s5;i150;O2; If "
"first character is NOT `'`\\1`', text is simple multi`-line text with `'`\\n`' used "
"as line separator.&][s5; In both cases, `'`&`' or `'`\\b`' are used as access`-key "
"flags. If `'`&`' should be the part of text, it must be escaped as")
TOPIC_TEXT(
" `\"`&`&`\". This format is formally named `\"SmartText`\".&][s5; Note also that "
"U`+`+ normally distributes access`-keys automatically. Providing access`-key is thus "
"usually unnecessary.&][s0;3 &][s4;:`:`:DeAmp`(const char`*`): [%00-00 String_][%00-00* "
"DeAmp][%00-00 (const_char_`*][%00-00*@3 s][%00-00 )]&][s6; This routine escapes all "
"occurrences of `'`&`' character in source string with `\"`&`&`\" stri")
TOPIC_TEXT(
"ng to avoid using it as access`-key flag.&][s1; [%00-00*C@3 s]-|Input string.&][s1; "
"[*/ Return value]-|Escaped string.&][s0;3 &][s4;:`:`:GetSmartTextSize`(`:`:Draw`&`,const "
"char`*`,`:`:Font: [%00-00 Size_][%00-00* GetSmartTextSize][%00-00 (Draw`&_][%00-00*@3 "
"w][%00-00 , const_char_`*][%00-00*@3 text][%00-00 , Font_][%00-00*@3 font][%00-00 "
"_`=_StdFont())]&][s6; Returns the minimal size of SmartText")
TOPIC_TEXT(
". If text is multi`-paragraph QTF text, width of text is the width of widest paragraph "
"without doing any line breaks (paragraphs are formatted for infinite width).&][s1; "
"[%00-00*C@3 w]-|Draw.&][s1; [%00-00*C@3 text]-|SmartText string.&][s1; [%00-00*C@3 "
"font]-|Font for non`-QTF text.&][s1; [*/ Return value]-|Size of SmartText.&][s0;3 "
"&][s4;:`:`:GetSmartTextHeight`(`:`:Draw`&`,const char`*`,int`,`:`")
TOPIC_TEXT(
":Font: [%00-00 int_][%00-00* GetSmartTextHeight][%00-00 (Draw`&_][%00-00*@3 w][%00-00 "
", const_char_`*][%00-00*@3 s][%00-00 , int_][%00-00*@3 cx][%00-00 , Font_][%00-00*@3 "
"font][%00-00 _`=_StdFont())]&][s6; Returns the height of given SmartText for given "
"width. QTF paragraphs are formatted for this width.&][s1; [%00-00*C@3 w]-|Draw.&][s1; "
"[%00-00*C@3 s]-|SmartText string.&][s1; [%00-00*C@3 cx]-|Req")
TOPIC_TEXT(
"uired width.&][s1; [%00-00*C@3 font]-|Font for non`-QTF text.&][s1; [*/ Return value]-|Height "
"of SmartText.&][s0;3 &][s4;:`:`:DrawSmartText`(`:`:Draw`&`,int`,int`,int`,const char`*`,`:`:Font: "
"[%00-00 void_][%00-00* DrawSmartText][%00-00 (Draw`&_][%00-00@3 w][%00-00 , int_][%00-00@3 "
"x][%00-00 , int_][%00-00@3 y][%00-00 , int_][%00-00@3 cx][%00-00 , const_char_`*][%00-00@3 "
"text][%00-00 , Font_][%00-")
TOPIC_TEXT(
"00@3 font][%00-00 _`=_StdFont(), Color_][%00-00@3 ink][%00-00 _`=_SBlack, int_][%00-00@3 "
"accesskey][%00-00 _`=_][%00-00@3 0][%00-00 )]&][s6; Paints SmartText to target Draw.&][s1; "
"[%00-00*C@3 w]-|Draw.&][s1; [%00-00*C@3 x, y]-|Position.&][s1; [%00-00*C@3 cx]-|Required "
"width.&][s1; [%00-00*C@3 text]-|SmartText.&][s1; [%00-00*C@3 font]-|Font for non`-QTF "
"SmartText.&][s1; [%00-00*C@3 ink]-|Text color")
TOPIC_TEXT(
" for non`-QTF SmartText.&][s1; [%00-00*C@3 accesskey]-|Access`-key `- first corresponding "
"character will be painted with underline.&][s0;3 &][s4;:`:`:ExtractAccessKey`(const "
"char`*`,`:`:String`&`): [%00-00 byte_][%00-00* ExtractAccessKey][%00-00 (const_char_`*][%00-00*@3 "
"s][%00-00 , String`&_][%00-00*@3 label][%00-00 )]&][s6; Scans input string for access`-key.&][s1; "
"[%00-00*C@3 s]-|Input string.&")
TOPIC_TEXT(
"][s1; [%00-00*C@3 label]-|Output string `- with access`-key removed.&][s1; [*/ Return "
"value]-|Access`-key.&][s0;3 &][s4;:`:`:CompareAccessKey`(`:`:byte`,`:`:dword`): [%00-00 "
"bool_][%00-00* CompareAccessKey][%00-00 (byte_][%00-00*@3 accesskey][%00-00 , dword_][%00-00*@3 "
"key][%00-00 )]&][s6; Compares access key to U`+`+ event key value as received by "
"Key virtual method.&][s1; [%00-00*C@3 accesskey]-")
TOPIC_TEXT(
"|Access`-key.&][s1; [%00-00*C@3 key]-|U`+`+ event key value.&][s1; [*/ Return value]-|True "
"if values match.&][s0;3 &][s4;:`:`:ChooseAccessKey`(const char`*`,`:`:dword`): [%00-00 "
"byte_][%00-00* ChooseAccessKey][%00-00 (const_char_`*][%00-00*@3 s][%00-00 , dword_][%00-00*@3 "
"used][%00-00 )]&][s6; Scans input string for suitable access`-key.&][s1; [%00-00*C@3 "
"s]-|Input string.&][s1; [%00-00*C@3 used]-")
TOPIC_TEXT(
"|Bit`-set of already used access`-keys. Bit`-mask of specific access`-key is defined "
"by [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:AccessKeyBit`(byte`)^ Ctrl`::AccessKeyBit] "
"class method.&][s1; [*/ Return value]-|Access`-key or 0 if no suitable access`-key "
"found.&][s5; &][s5; &][s0;3 &][s10;:`:`:DrawLabel`:`:struct:* [%00-00* struct_][%00-00 "
"DrawLabel]&][s6; &][s6; This structure in fac")
TOPIC_TEXT(
"t just encapsulates generic label painting routine that otherwise would have way "
"too much parameters.&][s0;3 &][s4;:`:`:DrawLabel`:`:DrawLabel`(`): [%00-00* DrawLabel][%00-00 "
"()]&][s6; Default constructor. Default values are documented in attributes descriptions.&][s0;3 "
"&][s4;:`:`:DrawLabel`:`:GetSize`(`)const: [%00-00 Size_][%00-00* GetSize][%00-00 "
"()_const]&][s6; Returns the size of label based ")
TOPIC_TEXT(
"on current set of attributes.&][s1; [*/ Return value]-|Size of label.&][s0;3 &][s4;:`:`:DrawLabel`:`:Paint`(`:`:Draw`&`,const`:`:Rect`&`,bool`)const: "
"[%00-00 Size_][%00-00* Paint][%00-00 (Draw`&_][%00-00*@3 w][%00-00 , const_Rect`&_][%00-00*@3 "
"r][%00-00 , bool_][%00-00*@3 visibleacckey][%00-00 _`=_true)_const]&][s6; Paints "
"the label in given rectangle.&][s1; [%00-00*C@3 w]-|Draw.&][s1; [%00-00*C@3")
TOPIC_TEXT(
" r]-|Rectangle.&][s1; [%00-00*C@3 visibleacckey]-|Access`-keys should be graphically "
"highlighted (underlined).&][s1; [*/ Return value]-|Size of label.&][s0;3 &][s4;:`:`:DrawLabel`:`:Paint`(`:`:Draw`&`,int`,int`,int`,int`,bool`)const: "
"[%00-00 Size_][%00-00* Paint][%00-00 (Draw`&_][%00-00*@3 w][%00-00 , int_][%00-00*@3 "
"x][%00-00 , int_][%00-00*@3 y][%00-00 , int_][%00-00*@3 cx][%00-00 , int_][%00-00")
TOPIC_TEXT(
"*@3 cy][%00-00 , bool_][%00-00*@3 visibleacckey][%00-00 _`=_true)_const]&][s6; Paints "
"the label in given rectangle.&][s1; [%00-00*C@3 w]-|Draw.&][s1; [%00-00*C@3 x, y, "
"cx, cy]-|Position and size of rectangle.&][s1; [%00-00*C@3 visibleacckey]-|Access`-keys "
"should be graphically highlighted (underlined).&][s1; [*/ Return value]-|Size of "
"label.&][s0;3 &][s4;:`:`:DrawLabel`:`:push:* [%00-00* bool_][%0")
TOPIC_TEXT(
"0-00 push]&][s6; Label should be painted as `\"pushed`\" (means paint offset one "
"pixel right and down). Default is false.&][s0;3 &][s4;:`:`:DrawLabel`:`:focus:* [%00-00* "
"bool_][%00-00 focus]&][s6; Label should be painted as `\"with input focus`\" (light "
"blue rectangle is drawn around label). Default is false.&][s0;3 &][s4;:`:`:DrawLabel`:`:disabled:* "
"[%00-00* bool_][%00-00 disabled]&][s6; Label should")
TOPIC_TEXT(
" be painted as `\"disable`\" (affects the way how images and text are painted, disabled "
"mean they are gray).&][s0;3 &][s4;:`:`:DrawLabel`:`:paintrect:* [%00-00* PaintRect_][%00-00 "
"paintrect]&][s6; This paintrect is painted behind the label text. If label text is "
"empty, size of text area is determined as paintrect.GetSize().&][s0;3 &][s4;:`:`:DrawLabel`:`:limg:* "
"[%00-00* Image_][%00-00 limg]&][s6; Le")
TOPIC_TEXT(
"ft image.&][s0;3 &][s4;:`:`:DrawLabel`:`:lcolor:* [%00-00* Color_][%00-00 lcolor]&][s6; "
"Color of left monochromatic image. Null means that the image is color.&][s0;3 &][s4;:`:`:DrawLabel`:`:lspc:* "
"[%00-00* int_][%00-00 lspc]&][s6; Space between left image and text. If Null, image "
"is placed at left edge of Label.&][s0;3 &][s4;:`:`:DrawLabel`:`:text:* [%00-00* String_][%00-00 "
"text]&][s6; Text of lab")
TOPIC_TEXT(
"el.&][s0;3 &][s4;:`:`:DrawLabel`:`:font:* [%00-00* Font_][%00-00 font]&][s6; Font "
"used to paint non`-QTF text.&][s0;3 &][s4;:`:`:DrawLabel`:`:ink:* [%00-00* Color_][%00-00 "
"ink]&][s6; Color used to paint non`-QTF text.&][s0;3 &][s4;:`:`:DrawLabel`:`:rimg:* "
"[%00-00* Image_][%00-00 rimg]&][s6; Right image.&][s0;3 &][s4;:`:`:DrawLabel`:`:rcolor:* "
"[%00-00* Color_][%00-00 rcolor]&][s6; Color of monochro")
TOPIC_TEXT(
"matic right image. Null means that the image is color.&][s0;3 &][s4;:`:`:DrawLabel`:`:rspc:* "
"[%00-00* int_][%00-00 rspc]&][s6; Space between the right image and text. If Null, "
"image is placed at the right size of Label.&][s0;3 &][s4;:`:`:DrawLabel`:`:align:* "
"[%00-00* int_][%00-00 align]&][s6; Horizontal alignment. Can be ALIGN`_LEFT, ALIGN`_RIGHT "
"or ALIGN`_CENTER.&][s0;3 &][s4;:`:`:DrawLabel`:`:va")
TOPIC_TEXT(
"lign:* [%00-00* int_][%00-00 valign]&][s6; Vertical alignment. Can be ALIGN`_TOP, "
"ALIGN`_BOTTOM or ALIGN`_CENTER.&][s0;3 &][s4;:`:`:DrawLabel`:`:accesskey:* [%00-00* "
"int_][%00-00 accesskey]&][s6; Access`-key.&][s5; &][s5; &][s0;3 &][s10;:`:`:LabelBase`:`:class:* "
"[%00-00* class_][%00-00 LabelBase]&][s6; &][s6; This class encapsulates basic DrawLabel "
"struct into form suitable to play a role of base ")
TOPIC_TEXT(
"class of GUI elements.&][s0;3 &][s4;:`:`:LabelBase`:`:LabelUpdate`(`): [%00-00 virtual "
"void_][%00-00* LabelUpdate][%00-00 ()]&][s6; This virtual method is called each time "
"when any of attributes changes.&][s0;3 &][s4;:`:`:LabelBase`:`:SetLeftImage`(const`:`:Image`&`,int`): "
"[%00-00 LabelBase`&_][%00-00* SetLeftImage][%00-00 (const_Image`&_][%00-00*@3 bmp1][%00-00 "
", int_][%00-00*@3 spc][%00-00 _`=_]")
TOPIC_TEXT(
"[%00-00@3 0][%00-00 )]&][s6; Sets the left image.&][s1; [%00-00*C@3 bmp1]-|Image.&][s1; "
"[%00-00*C@3 spc]-|Space between left image and text. If Null, image is placed at "
"left size of Label.&][s1; [*/ Return value]-|`*this for chaining.&][s0;3 &][s4;:`:`:LabelBase`:`:SetPaintRect`(const`:`:PaintRect`&`): "
"[%00-00 LabelBase`&_][%00-00* SetPaintRect][%00-00 (const_PaintRect`&_][%00-00*@3 "
"pr][%00-00 )]&")
TOPIC_TEXT(
"][s6; Sets PaintRect to be drawn behind or instead of label text.&][s1; [%00-00*C@3 "
"pr]-|PaintRect&][s1; [*/ Return value]-|`*this for chaining.&][s0;3 &][s4;:`:`:LabelBase`:`:SetText`(const "
"char`*`): [%00-00 LabelBase`&_][%00-00* SetText][%00-00 (const_char_`*][%00-00*@3 "
"text][%00-00 )]&][s6; Sets text of the label.&][s1; [%00-00*C@3 text]-|Text.&][s1; "
"[*/ Return value]-|`*this for chaining.&][s0")
TOPIC_TEXT(
";3 &][s4;:`:`:LabelBase`:`:SetFont`(`:`:Font`): [%00-00 LabelBase`&_][%00-00* SetFont][%00-00 "
"(Font_][%00-00*@3 font][%00-00 )]&][s6; Sets font of non`-QTF text.&][s1; [%00-00*C@3 "
"font]-|Font.&][s1; [*/ Return value]-|`*this for chaining.&][s0;3 &][s4;:`:`:LabelBase`:`:SetInk`(`:`:Color`): "
"[%00-00 LabelBase`&_][%00-00* SetInk][%00-00 (Color_][%00-00*@3 color][%00-00 )]&][s6; "
"Sets the color of non`")
TOPIC_TEXT(
"-QTF text .&][s1; [%00-00*C@3 color]-|Color.&][s1; [*/ Return value]-|`*this for "
"chaining.&][s0;3 &][s4;:`:`:LabelBase`:`:SetRightImage`(const`:`:Image`&`,int`): "
"[%00-00 LabelBase`&_][%00-00* SetRightImage][%00-00 (const_Image`&_][%00-00*@3 bmp2][%00-00 "
", int_][%00-00*@3 spc][%00-00 _`=_][%00-00@3 0][%00-00 )]&][s6; Sets the right image.&][s1; "
"[%00-00*C@3 bmp2]-|Image.&][s1; [%00-00*C@3 spc]-|Spac")
TOPIC_TEXT(
"e between left image and text. If Null, image is placed at left size of Label.&][s1; "
"[*/ Return value]-|`*this for chaining.&][s0;3 &][s4;:`:`:LabelBase`:`:SetAlign`(int`): "
"[%00-00 LabelBase`&_][%00-00* SetAlign][%00-00 (int_][%00-00*@3 align][%00-00 )]&][s6; "
"Sets horizontal alignment.&][s1; [%00-00*C@3 align]-|One of ALIGN`_TOP, ALIGN`_BOTTOM "
"or ALIGN`_CENTER.&][s1; [*/ Return value]-|`*this for ")
TOPIC_TEXT(
"chaining.&][s0;3 &][s4;:`:`:LabelBase`:`:SetVAlign`(int`): [%00-00 LabelBase`&_][%00-00* "
"SetVAlign][%00-00 (int_][%00-00*@3 align][%00-00 )]&][s6; Sets vertical alignment.&][s1; "
"[%00-00*C@3 align]-|One of ALIGN`_TOP, ALIGN`_BOTTOM or ALIGN`_CENTER.&][s1; [*/ "
"Return value]-|`*this for chaining.&][s0;3 &][s4;:`:`:LabelBase`:`:SetImage`(const`:`:Image`&`,int`): "
"[%00-00 LabelBase`&_][%00-00* SetImage]")
TOPIC_TEXT(
"[%00-00 (const_Image`&_][%00-00*@3 bmp][%00-00 , int_][%00-00*@3 spc][%00-00 _`=_][%00-00@3 "
"0][%00-00 )]&][s6; Same as SetLeftImage(bmp, spc).&][s0;3 &][s4;:`:`:LabelBase`:`:GetAlign`(`)const: "
"[%00-00 int_][%00-00* GetAlign][%00-00 ()_const]&][s1; [*/ Return value]-|Current "
"horizontal alignment.&][s0;3 &][s4;:`:`:LabelBase`:`:GetVAlign`(`)const: [%00-00 "
"int_][%00-00* GetVAlign][%00-00 ()_const]&][")
TOPIC_TEXT(
"s1; [*/ Return value]-|Current vertical alignment.&][s0;3 &][s4;:`:`:LabelBase`:`:GetPaintRect`(`)const: "
"[%00-00 PaintRect_][%00-00* GetPaintRect][%00-00 ()_const]&][s1; [*/ Return value]-|Current "
"PaintRect.&][s0;3 &][s4;:`:`:LabelBase`:`:GetText`(`)const: [%00-00 String_][%00-00* "
"GetText][%00-00 ()_const]&][s1; [*/ Return value]-|Current label text.&][s0;3 &][s4;:`:`:LabelBase`:`:GetFont`(`)const")
TOPIC_TEXT(
": [%00-00 Font_][%00-00* GetFont][%00-00 ()_const]&][s1; [*/ Return value]-|Current "
"font.&][s0;3 &][s4;:`:`:LabelBase`:`:GetInk`(`)const: [%00-00 Color_][%00-00* GetInk][%00-00 "
"()_const]&][s1; [*/ Return value]-|Current text color.&][s0;3 &][s4;:`:`:LabelBase`:`:PaintLabel`(`:`:Draw`&`,const`:`:Rect`&`,bool`,bool`,bool`,bool`): "
"[%00-00 Size_][%00-00* PaintLabel][%00-00 (Draw`&_][%00-00*@3 w][%00-0")
TOPIC_TEXT(
"0 , const_Rect`&_][%00-00*@3 r][%00-00 , bool_][%00-00*@3 disabled][%00-00 _`=_false, "
"bool_][%00-00*@3 push][%00-00 _`=_false, bool_][%00-00*@3 focus][%00-00 _`=_false, "
"bool_][%00-00*@3 vak][%00-00 _`=_true)]&][s6; Paints label in the given rectangle.&][s1; "
"[%00-00*C@3 w]-|Draw.&][s1; [%00-00*C@3 r]-|Rectangle.&][s1; [%00-00*C@3 disabled]-|Disable "
"flag.&][s1; [%00-00*C@3 push]-|Push flag.&][s1; [%")
TOPIC_TEXT(
"00-00*C@3 focus]-|Focus flag.&][s1; [%00-00*C@3 vak]-|If true, accelerator should "
"be highlighted.&][s1; [*/ Return value]-|Size of label.&][s0;3 &][s4;:`:`:LabelBase`:`:PaintLabel`(`:`:Draw`&`,int`,int`,int`,int`,bool`,bool`,bool`,bool`): "
"[%00-00 Size_][%00-00* PaintLabel][%00-00 (Draw`&_][%00-00*@3 w][%00-00 , int_][%00-00*@3 "
"x][%00-00 , int_][%00-00*@3 y][%00-00 , int_][%00-00*@3 cx][%00-00 , in")
TOPIC_TEXT(
"t_][%00-00*@3 cy][%00-00 , bool_][%00-00*@3 disabled][%00-00 _`=_false, bool_][%00-00*@3 "
"push][%00-00 _`=_false, bool_][%00-00*@3 focus][%00-00 _`=_false, bool_][%00-00*@3 "
"vak][%00-00 _`=_true)]&][s6; Paints label in the given rectangle.&][s1; [%00-00*C@3 "
"w]-|Draw.&][s1; [%00-00*C@3 x, y, cx, cy]-|Rectangle.&][s1; [%00-00*C@3 disabled]-|Disable "
"flag.&][s1; [%00-00*C@3 push]-|Push flag.&][s1; [%00-")
TOPIC_TEXT(
"00*C@3 focus]-|Focus flag.&][s1; [%00-00*C@3 vak]-|If true, accelerator should be "
"highlighted.&][s1; [*/ Return value]-|Size of label.&][s0;3 &][s4;:`:`:LabelBase`:`:GetLabelSize`(`)const: "
"[%00-00 Size_][%00-00* GetLabelSize][%00-00 ()_const]&][s1; [*/ Return value]-|Size "
"of label.&][s0;3 ]")
