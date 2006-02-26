TITLE("Color")
REF("::Color::class")
REF("::Color::GetRaw()const")
REF("::Color::GetR()const")
REF("::Color::GetG()const")
REF("::Color::GetB()const")
REF("::Color::IsNullInstance()const")
REF("::Color::GetHashValue()const")
REF("::Color::operator==(::Color)const")
REF("::Color::operator!=(::Color)const")
REF("::Color::Serialize(::Stream&)")
REF("::Color::Color()")
REF("::Color::Color(int,int,int)")
REF("::Color::Color(const::Nuller&)")
REF("::Color::operator::Value()const")
REF("::Color::Color(const::Value&)")
REF("::Color::Color(::Color(*)())")
REF("::Color::FromRaw(::dword)")
REF("::Color::operator COLORREF()const")
REF("::Color::FromCR(COLORREF)")
REF("::Color::operator::dword()const")
REF("::GrayColor::struct")
REF("::GrayColor::GrayColor(int)")
REF("::GetRValue(::dword)")
REF("::GetGValue(::dword)")
REF("::GetBValue(::dword)")
REF("::RGB(::byte,::byte,::byte)")
REF("::GetHashValue(::Color)")
REF("::Nvl(::Color,::Color)")
REF("::AsString(const::Color&)")
REF("::RGBtoHSV(double,double,double,double&,double&,double&)")
REF("::HSVtoRGB(double,double,double,double&,double&,double&)")
REF("::HsvColorf(double,double,double)")
REF("::Blend(::Color,::Color,int)")
REF("::ColorToHtml(::Color)")
REF("::Black()")
REF("::Gray()")
REF("::Yellow()")
REF("::LtRed()")
REF("::LtGreen()")
REF("::LtYellow()")
REF("::LtBlue()")
REF("::LtMagenta()")
REF("::LtCyan()")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2;:`:`:Color`:`:class: "
"[%00-00 Color]&][s10;:`:`:Color`:`:class:* [%00-00* class_][%00-00 Color]&][s6; Type "
"representing RGB color. All components are in the range 0 through 255. Null value "
"is defined for Color. Color is Value compatible as a Rich`-Va")
TOPIC_TEXT(
"lue type (supports Value comparison, serialization etc.).&][s9;/ &][s4;:`:`:Color`:`:GetRaw`(`)const: "
"[%00-00 dword_][%00-00* GetRaw][%00-00 ()_const]&][s1; [*/ Return value]-|Platform "
"specific dword representing the color.&][s0; &][s4;:`:`:Color`:`:GetR`(`)const: [%00-00 "
"int_][%00-00* GetR][%00-00 ()_const]&][s1; [*/ Return value]-|The red component.&][s0; "
"&][s4;:`:`:Color`:`:GetG`(`)const: [%00-")
TOPIC_TEXT(
"00 int_][%00-00* GetG][%00-00 ()_const]&][s1; [*/ Return value]-|The green component.&][s0; "
"&][s4;:`:`:Color`:`:GetB`(`)const: [%00-00 int_][%00-00* GetB][%00-00 ()_const]&][s1; "
"[*/ Return value]-|The blue component.&][s0; &][s4;:`:`:Color`:`:IsNullInstance`(`)const: "
"[%00-00 bool_][%00-00* IsNullInstance][%00-00 ()_const]&][s1; [*/ Return value]-|True "
"if Color is Null. Used to implement IsNull for")
TOPIC_TEXT(
" Color.&][s0; &][s4;:`:`:Color`:`:GetHashValue`(`)const: [%00-00 unsigned_][%00-00* "
"GetHashValue][%00-00 ()_const]&][s1; [*/ Return value]-|Hash`-value for Color.&][s0; "
"&][s4;:`:`:Color`:`:operator`=`=`(`:`:Color`)const: [%00-00 bool_][%00-00* operator`=`=][%00-00 "
"(Color_][%00-00*@3 c][%00-00 )_const]&][s1; [%00-00*C@3 c]-|Color to compare with.&][s1; "
"[*/ Return value]-|True if Color is equal to [")
TOPIC_TEXT(
"@3 c].&][s0; &][s4;:`:`:Color`:`:operator`!`=`(`:`:Color`)const: [%00-00 bool_][%00-00* "
"operator!`=][%00-00 (Color_][%00-00*@3 c][%00-00 )_const]&][s1; [%00-00*C@3 c]-|Color "
"to compare with.&][s1; [*/ Return value]-|True if Color is equal to [@3 c].&][s0; "
"&][s4;:`:`:Color`:`:Serialize`(`:`:Stream`&`): [%00-00 void_][%00-00* Serialize][%00-00 "
"(Stream`&_][%00-00*@3 s][%00-00 )]&][s6; Serialization.&")
TOPIC_TEXT(
"][s1; [%00-00*C@3 s]-|Stream.&][s0; &][s4;:`:`:Color`:`:Color`(`): [%00-00* Color][%00-00 "
"()]&][s6; Default constructor. Leaves component values uninitialized.&][s0; &][s4;:`:`:Color`:`:Color`(int`,int`,int`): "
"[%00-00* Color][%00-00 (int_][%00-00*@3 r][%00-00 , int_][%00-00*@3 g][%00-00 , int_][%00-00*@3 "
"b][%00-00 )]&][s6; Constructor.&][s1; [%00-00*C@3 r]-|Red channel.&][s1; [%00-00*C@3 "
"g]-|Green")
TOPIC_TEXT(
" channel.&][s1; [%00-00*C@3 b]-|Blue channel.&][s0; &][s4;:`:`:Color`:`:Color`(const`:`:Nuller`&`): "
"[%00-00* Color][%00-00 (const_Nuller`&)]&][s6; Null constructor.&][s0; &][s4;:`:`:Color`:`:operator`:`:Value`(`)const: "
"[%00-00* operator_Value][%00-00 ()_const]&][s6; Conversion to Value.&][s1; [*/ Return "
"value]-|Value.&][s0; &][s4;:`:`:Color`:`:Color`(const`:`:Value`&`): [%00-00* Color][%00-00 "
"(con")
TOPIC_TEXT(
"st_Value`&_][%00-00*@3 q][%00-00 )]&][s6; Conversion from Value.&][s1; [%00-00*C@3 "
"q]-|Value.&][s0; &][s4;:`:`:Color`:`:Color`(`:`:Color`(`*`)`(`)`): [%00-00* Color][%00-00 "
"(][%00-00* Color][%00-00 _(`*][%00-00*@3 fn][%00-00 )())]&][s6; This constructor "
"allows using a pointer to a function returning Colors instead of Colors in many places. "
"Global functions returning Color are needed instead of glo")
TOPIC_TEXT(
"bal Color constants on many platforms, because often global variables are not allowed "
"when using dynamic libraries.&][s1; [%00-00*C@3 fn]-|Pointer to a function returning "
"Color.&][s0; &][s4;:`:`:Color`:`:FromRaw`(`:`:dword`): [%00-00 static Color_][%00-00* "
"FromRaw][%00-00 (dword_][%00-00*@3 co][%00-00 )]&][s6; Creates a Color based on a "
"platform specific raw dword value.&][s1; [%00-00*C@3 co]-|Raw")
TOPIC_TEXT(
" dword value.&][s1; [*/ Return value]-|Color.&][s0; &][s4;:`:`:Color`:`:operator "
"COLORREF`(`)const: [%00-00* operator_COLORREF][%00-00 ()_const]&][s11; [%00-00 Win32 "
"specific.]&][s6; Return COLORREF for Color.&][s1; [*/ Return value]-|COLORREF.&][s0; "
"&][s4;:`:`:Color`:`:FromCR`(COLORREF`): [%00-00 static Color_][%00-00* FromCR][%00-00 "
"(COLORREF_][%00-00*@3 cr][%00-00 )]&][s11; Win32 specific.&][s6")
TOPIC_TEXT(
"; Creates Color from COLORREF.&][s1; [%00-00*C@3 cr]-|COLORREF.&][s1; [*/ Return "
"value]-|Color.&][s0; &][s4;:`:`:Color`:`:operator`:`:dword`(`)const: [%00-00* operator_dword][%00-00 "
"()_const]&][s1; [*/ Return value]-|GetRaw().&][s0;3 &][s0;3 &][s10;:`:`:GrayColor`:`:struct:* "
"[%00-00* struct_][%00-00 GrayColor]&][s6; &][s6; This simple utility class provides "
"constructor that sets all components to ")
TOPIC_TEXT(
"the same value producing the gray color.&][s6; &][s9; [/ Derived from] [%00-00 Color]&][s0; "
"&][s4;:`:`:GrayColor`:`:GrayColor`(int`): [%00-00* GrayColor][%00-00 (int_][%00-00*@3 "
"level][%00-00 )]&][s6; Constructor.&][s1; [%00-00*C@3 level]-|Gray intensity.&][s0; "
"&][s3;3 Utility functions&][s4;:`:`:GetRValue`(`:`:dword`): &][s4; [%00-00 int_][%00-00* "
"GetRValue][%00-00 (dword_][%00-00*@3 c][%00-00 )]")
TOPIC_TEXT(
"&][s6; Returns red component from a platform specific value.&][s1; [%00-00*C@3 c]-|Platform "
"specific value.&][s1; [*/ Return value]-|Red component.&][s0; &][s4;:`:`:GetGValue`(`:`:dword`): "
"[%00-00 int_][%00-00* GetGValue][%00-00 (dword_][%00-00*@3 c][%00-00 )]&][s6; Returns "
"green component from a platform specific value.&][s1; [%00-00*C@3 c]-|Platform specific "
"value.&][s1; [*/ Return value]-|Green")
TOPIC_TEXT(
" component.&][s0; &][s4;:`:`:GetBValue`(`:`:dword`): [%00-00 int_][%00-00* GetBValue][%00-00 "
"(dword_][%00-00*@3 c][%00-00 )]&][s6; Returns blue component from a platform specific "
"value.&][s1; [%00-00*C@3 c]-|Platform specific value.&][s1; [*/ Return value]-|Blue "
"component.&][s0; &][s4;:`:`:RGB`(`:`:byte`,`:`:byte`,`:`:byte`): [%00-00 dword_][%00-00* "
"RGB][%00-00 (byte_][%00-00*@3 r][%00-00 , byte_]")
TOPIC_TEXT(
"[%00-00*@3 g][%00-00 , byte_][%00-00*@3 b][%00-00 )]&][s6; Calculates platform specific "
"value from components.&][s1; [%00-00*C@3 r]-|Red component.&][s1; [%00-00*C@3 g]-|Green "
"component.&][s1; [%00-00*C@3 b]-|Blue component.&][s1; [*/ Return value]-|Platform "
"specific value.&][s0; &][s4;:`:`:GetHashValue`(`:`:Color`): [%00-00 unsigned_][%00-00* "
"GetHashValue][%00-00 (Color_][%00-00*@3 c][%00-00 )]&]")
TOPIC_TEXT(
"[s6; Returns a hash value for the color.&][s1; [%00-00*C@3 c]-|Color.&][s1; [*/ "
"Return value]-|Hash value.&][s0; &][s4;:`:`:Nvl`(`:`:Color`,`:`:Color`): [%00-00 "
"Color_][%00-00* Nvl][%00-00 (Color_][%00-00*@3 a][%00-00 , Color_][%00-00*@3 b][%00-00 "
")]&][s6; Returns first parameter if it is not Null, second otherwise.&][s1; [%00-00*C@3 "
"a]-|Color.&][s1; [%00-00*C@3 b]-|Color.&][s1; [*/ Return value]-")
TOPIC_TEXT(
"|a if not Null, b otherwise.&][s0; &][s4;:`:`:AsString`(const`:`:Color`&`): [%00-00 "
"template_<>]&][s4;:`:`:AsString`(const`:`:Color`&`): [%00-00 String_][%00-00* AsString][%00-00 "
"(const_Color`&_][%00-00*@3 c][%00-00 )]&][s6; Converts Color to textual form.&][s1; "
"[%00-00*C@3 c]-|Color.&][s1; [*/ Return value]-|Textual form.&][s0; &][s4;:`:`:RGBtoHSV`(double`,double`,double`,double`&`,double`&`,doub")
TOPIC_TEXT(
"le`&`): [%00-00 void_][%00-00* RGBtoHSV][%00-00 (double_][%00-00*@3 r][%00-00 , "
"double_][%00-00*@3 g][%00-00 , double_][%00-00*@3 b][%00-00 , double`&_][%00-00*@3 "
"h][%00-00 , double`&_][%00-00*@3 s][%00-00 , double`&_][%00-00*@3 v][%00-00 )]&][s6; "
"Transforms an `[R,G,B`] color triplet (each of the components is supposed to be in "
"the range `[0..1`]) into an `[H,S,V`] triplet (again all components a")
TOPIC_TEXT(
"re in the range `[0..1`]). This effectively transforms the RGB cube to a cone in "
"which color selection more intuitively matches our our visual perception ([/ hue] "
"corresponds to the tint of the color, [/ saturation] to the richness of the color "
"hue and [/ value] corresponds to luminance or brightness). &][s6; &][s6; The mathematical "
"formula used to convert between the two color spaces are:&][s6; &")
TOPIC_TEXT(
"][s6; v `= max(r, g, b)&][s6; s `= 1 `- min(r, g, b) / v&][s6; h `= (x `- min(r, "
"g, b)) / (max(r, g, b) `- min(r, g, b))&][s6; &][s6; where [/ x] is the color component "
"which is neither minimum nor maximum. There are six combinations for this, in every "
"even h is taken, in every odd 1`-h. The six combinations correspond to six points "
"on the color wheel forming the base of the HSV cone `- pure red, ")
TOPIC_TEXT(
"yellow, green, cyan, blue, and magenta. The color component ranges along the circumference "
"are the following (capital letters denote maximum component value, minuscule letters "
"minimum): Rgb `-> RGb `-> rGb `-> rGB `-> rgB `-> RgB.&][s6; &][s6; An easy way to "
"visualize the HSV transform based on the RGB cube is: imagine a slight deformation "
"of the cube where the tip of the cone corresponds to the b")
TOPIC_TEXT(
"lack RBG cube vertex, the opposite (white) RGB cube vertex corresponds to the center "
"of the base circle of the HSV cone and the remaining six RGB cube vertices get projected "
"on the circumference of the cone base as explained in the preceding paragraph.&][s6; "
"&][s1; [%00-00*C@3 r]-|input red component `[0..1`]&][s1; [%00-00*C@3 g]-|input green "
"component `[0..1`]&][s1; [%00-00*C@3 b]-|input blue com")
TOPIC_TEXT(
"ponent `[0..1`]&][s1; [%00-00*C@3 h]-|output hue `[0..1`], i.e. color tint&][s1; "
"[%00-00*C@3 s]-|output saturation `[0..1`]. Maximum saturation (1) corresponds to "
"the `\"purest`\" basic color (one of the six), whereas minimum saturation (0) is "
"gray or white.&][s1; [%00-00*C@3 v]-|output brightness `[0..1`].&][s0; &][s0; &][s4;:`:`:HSVtoRGB`(double`,double`,double`,double`&`,double`&`,double`&`): "
"[%0")
TOPIC_TEXT(
"0-00 void_][%00-00* HSVtoRGB][%00-00 (double_][%00-00*@3 h][%00-00 , double_][%00-00*@3 "
"s][%00-00 , double_][%00-00*@3 v][%00-00 , double`&_][%00-00*@3 r][%00-00 , double`&_][%00-00*@3 "
"g][%00-00 , double`&_][%00-00*@3 b][%00-00 )]&][s6; Transformes an HSV triplet (each "
"of the components is supposed to lie in the interval `[0..1`]) into an RGB triplet "
"(again with component ranges `[0..1`]). This pr")
TOPIC_TEXT(
"ojects the HSV cone (as explained in the preceding section) back onto the RGB cube.&][s1; "
"[%00-00*C@3 h]-|input hue, i.e. color tint `[0..1`]&][s1; [%00-00*C@3 s]-|input saturation, "
"i.e. color `\"purity`\" `[0..1`]&][s1; [%00-00*C@3 v]-|value `- luminance or brightness "
"`[0..1`]&][s1; [%00-00*C@3 r]-|output red component `[0..1`]&][s1; [%00-00*C@3 g]-|output "
"green component `[0..1`]&][s1; [%00-00*C@3")
TOPIC_TEXT(
" b]-|output blue component `[0..1`]&][s0; &][s4;:`:`:HsvColorf`(double`,double`,double`): "
"[%00-00 Color_][%00-00* HsvColorf][%00-00 (double_][%00-00*@3 h][%00-00 , double_][%00-00*@3 "
"s][%00-00 , double_][%00-00*@3 v][%00-00 )]&][s6; Converts an HSV color triplet into "
"a [* Color] object. The H, S, and V components are supposed to be in the range `[0..1`].&][s1; "
"[%00-00*C@3 h]-|input hue `[0..1`]&][")
TOPIC_TEXT(
"s1; [%00-00*C@3 s]-|input saturation `[0..1`]&][s1; [%00-00*C@3 v]-|input value "
"aka brightness `[0..1`]&][s1; [*/ Return value]-|the [* Color] object with the given "
"HSV components&][s0; &][s4;:`:`:Blend`(`:`:Color`,`:`:Color`,int`): [%00-00 Color_][%00-00* "
"Blend][%00-00 (Color_][%00-00*@3 c1][%00-00 , Color_][%00-00*@3 c2][%00-00 , int_][%00-00*@3 "
"alpha][%00-00 _`=_][%00-00@3 128][%00-00 )]&][s6; ")
TOPIC_TEXT(
"Blends two colors.&][s1; [%00-00*C@3 c1]-|First color.&][s1; [%00-00*C@3 c2]-|Second "
"color.&][s1; [%00-00*C@3 alpha]-|Blending factor in the range 0..255.&][s1; [*/ Return "
"value]-|Blended color `- (255 `- alpha) / 255.0 `* c1 `+ alpha / 255.0 `* c2.&][s0; "
"&][s4;:`:`:ColorToHtml`(`:`:Color`): [%00-00 String_][%00-00* ColorToHtml][%00-00 "
"(Color_][%00-00*@3 color][%00-00 )]&][s6; Converts Color to th")
TOPIC_TEXT(
"e textual format used in HTML (into hexadecimal form like #ffffff for white).&][s1; "
"[%00-00*C@3 color]-|Color.&][s1; [*/ Return value]-|HTML text.&][s0; &][s5;3 &][s3;3 "
"Color constants&][s5; Predefined colors are represented by functions that return "
"the predefined color value.&][s4;:`:`:Black`(`): [%00-00 Color_][%00-00* Black][%00-00 "
"()]&][s4;:`:`:Gray`(`): [%00-00 Color_][%00-00* Gray][%00-00 ()")
TOPIC_TEXT(
"]&][s4; [%00-00 Color_][%00-00* LtGray][%00-00 ()]&][s4; [%00-00 Color_][%00-00* "
"WhiteGray][%00-00 ()]&][s4; [%00-00 Color_][%00-00* White][%00-00 ()]&][s4; [%00-00 "
"Color_][%00-00* Red][%00-00 ()]&][s4; [%00-00 Color_][%00-00* Green][%00-00 ()]&][s4; "
"[%00-00 Color_][%00-00* Brown][%00-00 ()]&][s4; [%00-00 Color_][%00-00* Blue][%00-00 "
"()]&][s4; [%00-00 Color_][%00-00* Magenta][%00-00 ()]&][s4; [%00")
TOPIC_TEXT(
"-00 Color_][%00-00* Cyan][%00-00 ()]&][s4;:`:`:Yellow`(`): [%00-00 Color_][%00-00* "
"Yellow][%00-00 ()]&][s4;:`:`:LtRed`(`): [%00-00 Color_][%00-00* LtRed][%00-00 ()]&][s4;:`:`:LtGreen`(`): "
"[%00-00 Color_][%00-00* LtGreen][%00-00 ()]&][s4;:`:`:LtYellow`(`): [%00-00 Color_][%00-00* "
"LtYellow][%00-00 ()]&][s4;:`:`:LtBlue`(`): [%00-00 Color_][%00-00* LtBlue][%00-00 "
"()]&][s4;:`:`:LtMagenta`(`): [%00-00 C")
TOPIC_TEXT(
"olor_][%00-00* LtMagenta][%00-00 ()]&][s4;:`:`:LtCyan`(`): [%00-00 Color_][%00-00* "
"LtCyan][%00-00 ()]&][s0;3 ]")
