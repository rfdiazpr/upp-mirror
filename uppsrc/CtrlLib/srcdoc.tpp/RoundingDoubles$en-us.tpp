TITLE("About rounding double values")
TOPIC_TEXT(
"[2 $$0,0#00000000000000000000000000000000:Default][l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param][a83;*R6 "
"$$2,5#31310162474203024125188417583966:caption][b83;* $$3,5#07864147445237544204411237157677:title][b167;a42;C2 "
"$$4,6#40027414424643823182269349404212:item][b42;a42;2 $$5,5#45413000475342174754091244180557:text][l288;a17;2 "
"$$6,6#27521748481378242620020725143825:desc][")
TOPIC_TEXT(
"l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code][b2503; $$8,0#65142375456100023862071332075487:separator][*@(0.0.255)2 "
"$$9,0#83433469410354161042741608181528:base][t4167;C $$10,0#37138531426314131251341829483380:class][l288;a17;*1 "
"$$11,11#70004532496200323422659154056402:requirement][i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam][b167;C2 "
"$$13,13#924304594434")
TOPIC_TEXT(
"60461911108080531343:item1][i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2][*@2$(0.128.128) "
"$$15,15#34511555403152284025741354420178:NewsDate][l321;*C$7;2 $$16,16#03451589433145915344929335295360:result][l321;b83;a83;*C$7;2 "
"$$17,17#07531550463529505371228428965313:result`-line][l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title][{_}%EN-US "
"[s2; About rounding do")
TOPIC_TEXT(
"uble values&][s0; Standard C math library defines two rounding functions:&][s0; "
"&][s0; floor `- rounds to the nearest non`-greater float without decimal digits, "
"i.e.&][s0; &][s0; floor(`-2.8) `= floor(`-2.4) `= `-3, floor(8) `= floor(8.2) `= "
"floor(8.8) `= 8&][s0; &][s0; ceil `- rounds to the nearest non`-lower float without "
"decimal digits, i.e.&][s0; &][s0; ceil(`-2.8) `= ceil(`-2.4) `= `-2, ceil(")
TOPIC_TEXT(
"8.2) `= ceil(8.8) `= ceil(9) `= 9&][s0; &][s0; In addition to these standard functions, "
"U`+`+ defines (in Core/util.h) a few of its own:&][s0; &][s7; [%00-00 int fround(double "
"x);]&][s7; [%00-00 int ffloor(double x);]&][s7; [%00-00 int fceil(double x);]&][s0; "
"&][s0; These combine rounding (to the nearest integer, to the nearest non`-greater, "
"to the nearest non`-lower integer, respectively) with tr")
TOPIC_TEXT(
"uncation of float to integer.&][s0; &][s7; [%00-00 int ilog10(double d);]&][s7; "
"[%00-00 double ipow10(int i);]&][s7; [%00-00 double normalize(double d, int`& exponent);]&][s0; "
"&][s0; These functions can be used to manipulate the order magnitude of floats. ilog10 "
"returns the greatest integer i such that 10`^i <`= d, ipow10 calculates 10`^i (should "
"be relatively fast, at least for reasonable exponen")
TOPIC_TEXT(
"ts), normalize transforms a float d to the form mantissa `* 10`^exponent, where "
"mantissa (the returned double) lies in the semi`-closed interval `[1, 10).&][s0; "
"&][s7; [%00-00 double roundr(double d, int digits);]&][s7; [%00-00 double floorr(double "
"d, int digits);]&][s7; [%00-00 double ceilr(double d, int digits);]&][s0; &][s0; "
"These functions round given float to the given number of `_`_valid`_`_")
TOPIC_TEXT(
" digits (i.e. relative to its magnitude).&][s0; &][s0; All in all, the simple rounding "
"you are asking for is currently not present, but it`'s relatively easy to implement, "
"e.g.:&][s0; &][s0; round(d, n) `= floor(d / ipow10(n) `+ 0.5) `* ipow10(n)&][s0; "
"&][s0; But your example (where you are using the rounding function to initialize "
"an edit field), is still something a bit different, what you are d")
TOPIC_TEXT(
"emonstrating here is formatting a float to the given number of digits. If you look "
"into Core/Format.h, you`'ll find the following functions:&][s0; &][s7; [%00-00 enum]&][s7; "
"[%00-00 `{]&][s7; [%00-00 -|FD`_SIGN     `= 0x01, // always prepend sign (`+10)]&][s7; "
"[%00-00 -|FD`_REL      `= 0x02, // relative decimal places (valid digits)]&][s7; "
"[%00-00 -|FD`_SIGN`_EXP `= 0x04, // always prepend sign to")
TOPIC_TEXT(
" exponent (1e`+2)]&][s7; [%00-00 -|FD`_CAP`_E    `= 0x08, // capital E for exponent "
"(1E10)]&][s7; [%00-00 -|FD`_ZERO     `= 0x10, // keep trailing zeros (1.25000)]&][s7; "
"[%00-00 -|FD`_FIX      `= 0x20, // always use fixed notation (FormatDouble)]&][s7; "
"[%00-00 -|FD`_EXP      `= 0x40, // always use exponential notation (FormatDouble)]&][s7; "
"[%00-00 `};]&][s7; [%00-00 String         FormatDoubleDigi")
TOPIC_TEXT(
"ts(double d, int digits, int flags, int`& exponent);]&][s7; [%00-00 String      "
"   FormatDouble(double d, int digits, int flags `= 0, int fill`_exp `= 0);]&][s7; "
"[%00-00 String         FormatDoubleFix(double d, int digits, int flags `= 0);]&][s7; "
"[%00-00 String         FormatDoubleExp(double d, int digits, int flags `= 0, int "
"fill`_exp `= 0);]&][s0; &][s0; You can use these to perform almost any i")
TOPIC_TEXT(
"maginable (reasonable) float formatting. For a start try FormatDouble(x, digits), "
"I suppose this should do something similar to your MyRoundDouble.&][s0; &][s0; There`'s "
"still another option using EditDouble. It has a method called Pattern, which sets "
"the formatting string for NFormat which is used to generate the textual representation "
"of the number. To round to a fixed number of decimal digits, ")
TOPIC_TEXT(
"use e.g.&][s0; &][s7; [%00-00 EditDouble dedit;]&][s7; [%00-00 dedit.Pattern(`\"%4n`\");]&][s0; "
"&][s0; (You can also use the more traditional C`-style formatters, like %4g or %3f, "
"but the %n formatter is more flexible). &][s0; &][s0; Note that Pattern is actually "
"method of ConvertDouble, which is one of base classes for EditDouble.&][s0;3 ]")
