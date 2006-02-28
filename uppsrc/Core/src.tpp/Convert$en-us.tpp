TITLE("Convert")
REF("::Convert::class")
REF("::Convert::Format(const::Value&)const")
REF("::Convert::Scan(const::Value&)const")
REF("::Convert::Filter(int)const")
REF("::Convert::operator()(const::Value&)const")
REF("::ConvertDate::class")
REF("::ConvertDate::Scan(const::Value&)const")
REF("::ConvertDate::Min(::Date)")
REF("::ConvertDate::Max(::Date)")
REF("::ConvertDate::NotNull(bool)")
REF("::ConvertDate::NoNotNull()")
REF("::ConvertDate::GetMin()const")
REF("::ConvertDate::GetMax()const")
REF("::ConvertDate::IsNotNull()const")
REF("::ConvertDate::::ConvertDate(::Date,::Date,bool)")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2; Convert&][s10;:`:`:Convert`:`:class:* "
"[%00-00* class_][%00-00 Convert]&][s6; Convert`-derived classes serve as bidirectional "
"Value`-Value converters. One direction is represented by Format method and usually "
"converts the Value to the textual repres")
TOPIC_TEXT(
"entation. Reverse direction is represented by the Scan method.&][s6; If the conversion "
"is not possible (e.g. due to invalid textual representation), convert methods should "
"return `\"ErrorValue`\" to indicate the problem.&][s0; &][s4;:`:`:Convert`:`:Format`(const`:`:Value`&`)const: "
"[%00-00 virtual Value_][%00-00* Format][%00-00 (const_Value`&_][%00-00*@3 q][%00-00 "
")_const]&][s6; [%00-00 Converts the ")
TOPIC_TEXT(
"Value. Default ]implementation uses Value`::ToString to convert the Value.&][s1; "
"[%00-00*C@3 q]-|Value.&][s1; [*/ Return value]-|Converted Value.&][s0; &][s4;:`:`:Convert`:`:Scan`(const`:`:Value`&`)const: "
"[%00-00 virtual Value_][%00-00* Scan][%00-00 (const_Value`&_][%00-00*@3 text][%00-00 "
")_const]&][s6; Converts the Value in reverse direction. Default implementation returns "
"[@3 text] (no conversio")
TOPIC_TEXT(
"n is performed).&][s1; [%00-00*C@3 text]-|Value.&][s1; [*/ Return value]-|Converted "
"Value.&][s0; &][s4;:`:`:Convert`:`:Filter`(int`)const: [%00-00 virtual int_][%00-00* "
"Filter][%00-00 (int_][%00-00*@3 chr][%00-00 )_const]&][s6; Adjusts characters allowed "
"in textual representation of Value.&][s1; [%00-00*C@3 chr]-|Character to adjust.&][s1; "
"[*/ Return value]-|Adjusted character. Zero indicates that")
TOPIC_TEXT(
" character is rejected. Filter is allowed to alter the input character (e.g. by "
"upper`-casing it, changing `',`' to `'.`' etc..). Default implementation returns "
"unaltered [@3 chr].&][s0; &][s4;:`:`:Convert`:`:operator`(`)`(const`:`:Value`&`)const: "
"[%00-00 Value_][%00-00* operator()][%00-00 (const_Value`&_][%00-00*@3 q][%00-00 )_const]&][s6; "
"Invokes the Format method. Helper functor`-like syntax.&]")
TOPIC_TEXT(
"[s1; [%00-00*C@3 q]-|Value.&][s1; [*/ Return value]-|Converted Value.&][s0; &][s0;3 "
"&][s10;:`:`:ConvertDate`:`:class:* [%00-00* class_][%00-00 ConvertDate]&][s6; Date "
"Converter. Textual date format depends on actual regional settings.&][s9; [/ Derived "
"from] [%00-00 Convert]&][s0; &][s4;:`:`:ConvertDate`:`:Scan`(const`:`:Value`&`)const: "
"[%00-00 ConvertDate`&_][%00-00* MinMax][%00-00 (Date_][%00-00*")
TOPIC_TEXT(
"@3 `_min][%00-00 , Date_][%00-00*@3 `_max][%00-00 )]&][s6; Sets the range of Dates "
"allowed.&][s1; [%00-00*C@3 `_min]-|Minimal Date.&][s1; [%00-00*C@3 `_max]-|Maximal "
"Date.&][s1; [*/ Return value]-|`*this.&][s0; &][s4;:`:`:ConvertDate`:`:Min`(`:`:Date`): "
"[%00-00 ConvertDate`&_][%00-00* Min][%00-00 (Date_][%00-00*@3 `_min][%00-00 )]&][s6; "
"Sets minimal allowed Date.&][s1; [%00-00*C@3 `_min]-|Minimal ")
TOPIC_TEXT(
"Date.&][s1; [*/ Return value]-|`*this.&][s0; &][s4;:`:`:ConvertDate`:`:Max`(`:`:Date`): "
"[%00-00 ConvertDate`&_][%00-00* Max][%00-00 (Date_][%00-00*@3 `_max][%00-00 )]&][s6; "
"Sets maximal allowed Date.&][s1; [%00-00*C@3 `_max]-|Maximal Date.&][s1; [*/ Return "
"value]-|`*this.&][s0; &][s4;:`:`:ConvertDate`:`:NotNull`(bool`): [%00-00 ConvertDate`&_][%00-00* "
"NotNull][%00-00 (bool_][%00-00*@3 b][%00-00 _`")
TOPIC_TEXT(
"=_true)]&][s6; Allows/disallows Null dates. (Null dates are represented by the empty "
"text).&][s1; [%00-00*C@3 b]-|true to disallow Null dates.&][s1; [*/ Return value]-|`*this.&][s0; "
"&][s4;:`:`:ConvertDate`:`:NoNotNull`(`): [%00-00 ConvertDate`&_][%00-00* NoNotNull][%00-00 "
"()]&][s6; Same as NotNull(false).&][s0; &][s4;:`:`:ConvertDate`:`:GetMin`(`)const: "
"[%00-00 Date_][%00-00* GetMin][%00-00 ()_con")
TOPIC_TEXT(
"st]&][s1; [*/ Return value]-|Minimal allowed date.&][s0; &][s4;:`:`:ConvertDate`:`:GetMax`(`)const: "
"[%00-00 Date_][%00-00* GetMax][%00-00 ()_const]&][s1; [*/ Return value]-|Maximal "
"allowed date.&][s0; &][s4;:`:`:ConvertDate`:`:IsNotNull`(`)const: [%00-00 bool_][%00-00* "
"IsNotNull][%00-00 ()_const]&][s1; [*/ Return value]-|true is Nulls are disallowed.&][s0; "
"&][s4;:`:`:ConvertDate`:`:`:`:ConvertDate")
TOPIC_TEXT(
"`(`:`:Date`,`:`:Date`,bool`): [%00-00* ConvertDate][%00-00 (Date_][%00-00*@3 minval][%00-00 "
"_`=_Date(][%00-00@3 0][%00-00 , ][%00-00@3 0][%00-00 , ][%00-00@3 0][%00-00 ), Date_][%00-00*@3 "
"maxval][%00-00 _`=_Date(][%00-00@3 9999][%00-00 , ][%00-00@3 12][%00-00 , ][%00-00@3 "
"31][%00-00 ), bool_][%00-00*@3 notnull][%00-00 _`=_false)]&][s6; Constructor.&][s1; "
"[%00-00*C@3 minval]-|Minimum Date allowed.&")
TOPIC_TEXT(
"][s1; [%00-00*C@3 maxval]-|Maximum Date allowed.&][s1; [%00-00*C@3 notnull]-|Disallow "
"Nulls.&][s0; &][s0;3 ]")
