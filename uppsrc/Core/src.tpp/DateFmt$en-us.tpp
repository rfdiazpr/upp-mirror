TITLE("Date formatting and scanning")
REF("::StrToDate(::Date&,const char*)")
REF("::Format(::Date)")
REF("::CharFilterDate(int)")
REF("::SetDateScan(const char*)")
REF("::SetDateFilter(const char*)")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2; [%00-00 Date formatting "
"and scanning]&][s0;3 &][s4;:`:`:StrToDate`(`:`:Date`&`,const char`*`): [%00-00 const_char_`*][%00-00* "
"StrToDate][%00-00 (Date`&_][%00-00*@3 d][%00-00 , const_char_`*][%00-00*@3 s][%00-00 "
")]&][s6; Scans a string for a Date. O")
TOPIC_TEXT(
"rder of day, month and year is specified using SetDateScan. In place of month both "
"number and text is accepted `- text must match abbreviated or full name of month.&][s1; "
"[%00-00*C@3 d]-|Found date.&][s1; [%00-00*C@3 s]-|String to scan.&][s1; [*/ Return "
"value]-|NULL if no date is found in s or position right after the date.&][s0;3 &][s4;:`:`:Format`(`:`:Date`): "
"[%00-00 String_][%00-00* Format][%00")
TOPIC_TEXT(
"-00 (Date_][%00-00*@3 date][%00-00 )]&][s6; Formats date. Date is formated using "
"the standard Format function, where the string set by SetDateFormat is used as formating "
"string, and the three supplied integer value arguments are year, month and day.&][s1; "
"[%00-00*C@3 date]-|Date to format.&][s1; [*/ Return value]-|Formatted date.&][s0;3 "
"&][s4;:`:`:CharFilterDate`(int`): [%00-00 int_][%00-00* CharF")
TOPIC_TEXT(
"ilterDate][%00-00 (int_][%00-00*@3 c][%00-00 )]&][s6; Character filter for dates. "
"Its behaviour is specified by SetDateFilter function.&][s1; [%00-00*C@3 c]-|Character "
"to filter.&][s1; [*/ Return value]-|Filtered character.&][s0; &][s4; [%00-00 void_][%00-00* "
"SetDateFormat][%00-00 (const_char_`*][%00-00*@3 fmt][%00-00 )]&][s6; Sets formating "
"string for dates.&][s1; [%00-00*C@3 fmt]-|Formating stri")
TOPIC_TEXT(
"ng.&][s0;3 &][s4;:`:`:SetDateScan`(const char`*`): [%00-00 void_][%00-00* SetDateScan][%00-00 "
"(const_char_`*][%00-00*@3 scan][%00-00 )]&][s6; Sets date scan string `- this string "
"represents order of day, month and year for StrToDate function.&][s1; [%00-00*C@3 "
"scan]-|Date scan string.&][s0;3 &][s4;:`:`:SetDateFilter`(const char`*`): [%00-00 "
"void_][%00-00* SetDateFilter][%00-00 (const_char_`*][%00-")
TOPIC_TEXT(
"00*@3 seps][%00-00 )]&][s6; Specifies CharFilterDate behaviour. Digits are always "
"allowed in CharFilterDate. If there is `\"l`\" at the beginning of seps string, letters "
"are allowed. If there is `\"L`\", letters are allowed and converted to upper`-case. "
"After this optional character, list of characters allowed as separator follows. `\"`\\a`\" "
"after character designates `\"amend`\" character `- if used, all")
TOPIC_TEXT(
" characters up to next `\"amend`\" character are replaced by it.&][s1; [%00-00*C@3 "
"seps]-|Filter`-string.&][s0;3 &][s0;3 ]")
