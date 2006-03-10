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
REF("::ConvertDouble::class")
REF("::ConvertDouble::Pattern(const char*)")
REF("::ConvertDouble::MinMax(double,double)")
REF("::ConvertDouble::Min(double)")
REF("::ConvertDouble::Max(double)")
REF("::ConvertDouble::NotNull(bool)")
REF("::ConvertDouble::NoNotNull()")
REF("::ConvertDouble::GetMin()const")
REF("::ConvertDouble::GetMax()const")
REF("::ConvertDouble::IsNotNull()const")
REF("::ConvertDouble::::ConvertDouble(double,double,bool)")
REF("::ConvertInt::class")
REF("::ConvertInt::MinMax(int,int)")
REF("::ConvertInt::Min(int)")
REF("::ConvertInt::Max(int)")
REF("::ConvertInt::NotNull(bool)")
REF("::ConvertInt::NoNotNull()")
REF("::ConvertInt::GetMin()const")
REF("::ConvertInt::GetMax()const")
REF("::ConvertInt::IsNotNull()const")
REF("::ConvertInt::::ConvertInt(int,int,bool)")
REF("::ConvertString::class")
REF("::ConvertString::MaxLen(int)")
REF("::ConvertString::GetMaxLength()const")
REF("::ConvertString::NotNull(bool)")
REF("::ConvertString::NoNotNull()")
REF("::ConvertString::IsNotNull()const")
REF("::ConvertString::::ConvertString(int,bool)")
REF("::ConvertTime::class")
REF("::ConvertTime::MinMax(::Time,::Time)")
REF("::ConvertTime::NotNull(bool)")
REF("::ConvertTime::NoNotNull()")
REF("::ConvertTime::GetMin()const")
REF("::ConvertTime::GetMax()const")
REF("::ConvertTime::IsNotNull()const")
REF("::ConvertTime::::ConvertTime(::Time,::Time,bool)")
REF("::FormatConvert::class")
REF("::FormatConvert::SetFormat(const char*)")
REF("::JoinConvert::class")
REF("::JoinConvert::Add(const char*)")
REF("::JoinConvert::Add(int,const::Convert&)")
REF("::JoinConvert::Add(int)")
REF("::JoinConvert::Add(const::Convert&)")
REF("::JoinConvert::Add()")
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
"Nulls.&][s0; &][s0;3 &][s10;:`:`:ConvertDouble`:`:class:* [%00-00* class_][%00-00 "
"ConvertDouble]&][s6; Floating point number converter.&][s6; &][s9; [/ Derived from] "
"[%00-00 Convert]&][s0; &][s4;:`:`:ConvertDouble`:`:Pattern`(const char`*`): [%00-00 "
"ConvertDouble`&_][%00-00* Pattern][%00-00 (const_char_`*][%00-00")
TOPIC_TEXT(
"*@3 p][%00-00 )]&][s6; Formatting tag used for conversion to textual representation "
"(includes standard printf formatting tags, see Format function for description). "
"Default tag is [@4 %.10g].&][s1; [%00-00*C@3 p]-|Pattern.&][s1; [*/ Return value]-|`*this.&][s0; "
"&][s4;:`:`:ConvertDouble`:`:MinMax`(double`,double`): [%00-00 ConvertDouble`&_][%00-00* "
"MinMax][%00-00 (double_][%00-00*@3 `_min][%00-00 ,")
TOPIC_TEXT(
" double_][%00-00*@3 `_max][%00-00 )]&][s6; Sets minimal and maximal allowed numbers.&][s1; "
"[%00-00*C@3 `_min]-|Lower limit.&][s1; [%00-00*C@3 `_max]-|Upper limit.&][s1; [*/ "
"Return value]-|`*this.&][s0; &][s4;:`:`:ConvertDouble`:`:Min`(double`): [%00-00 ConvertDouble`&_][%00-00* "
"Min][%00-00 (double_][%00-00*@3 `_min][%00-00 )]&][s6; Sets minimal allowed number.&][s1; "
"[%00-00*C@3 `_min]-|Lower limit")
TOPIC_TEXT(
".&][s1; [*/ Return value]-|`*this.&][s0; &][s4;:`:`:ConvertDouble`:`:Max`(double`): "
"[%00-00 ConvertDouble`&_][%00-00* Max][%00-00 (double_][%00-00*@3 `_max][%00-00 )]&][s6; "
"Sets maximal allowed number.&][s1; [%00-00*C@3 `_max]-|Upper limit.&][s1; [*/ Return "
"value]-|`*this.&][s0; &][s4;:`:`:ConvertDouble`:`:NotNull`(bool`): [%00-00 ConvertDouble`&_][%00-00* "
"NotNull][%00-00 (bool_][%00-00*@3 b][%00-")
TOPIC_TEXT(
"00 _`=_true)]&][s6; Allows/disallows Nulls. (Nulls are represented by the empty "
"text).&][s1; [%00-00*C@3 b]-|true to disallow.&][s1; [*/ Return value]-|`*this.&][s0; "
"&][s4;:`:`:ConvertDouble`:`:NoNotNull`(`): [%00-00 ConvertDouble`&_][%00-00* NoNotNull][%00-00 "
"()]&][s6; Equivalent to NotNull(false).&][s0; &][s4;:`:`:ConvertDouble`:`:GetMin`(`)const: "
"[%00-00 double_][%00-00* GetMin][%00-00 ()_const")
TOPIC_TEXT(
"]&][s1; [*/ Return value]-|Lower limit.&][s0; &][s4;:`:`:ConvertDouble`:`:GetMax`(`)const: "
"[%00-00 double_][%00-00* GetMax][%00-00 ()_const]&][s1; [*/ Return value]-|Upper "
"limit.&][s0; &][s4;:`:`:ConvertDouble`:`:IsNotNull`(`)const: [%00-00 bool_][%00-00* "
"IsNotNull][%00-00 ()_const]&][s1; [*/ Return value]-|true is Nulls are disallowed.&][s0; "
"&][s4;:`:`:ConvertDouble`:`:`:`:ConvertDouble`(double`,")
TOPIC_TEXT(
"double`,bool`): [%00-00* ConvertDouble][%00-00 (double_][%00-00*@3 minval][%00-00 "
"_`=_DOUBLE`_NULL`_LIM, double_][%00-00*@3 maxval][%00-00 _`=_`-DOUBLE`_NULL`_LIM, "
"bool_][%00-00*@3 notnull][%00-00 _`=_false)]&][s6; Constructor.&][s1; [%00-00*C@3 "
"minval]-|Lower limit `- default value means there is no limit.&][s1; [%00-00*C@3 "
"maxval]-|Upper limit `- default value means there is no limit.&][s1; [%00")
TOPIC_TEXT(
"-00*C@3 notnull]-|If true, Nulls are not allowed.&][s0; &][s0; &][s0; &][s10;:`:`:ConvertInt`:`:class:*@3 "
"[%00-00*@0 class_][%00-00@0 ConvertInt][%00-00*@0 _:_public_][%00-00 Convert]&][s6; "
"Integer converter.&][s6; &][s9; [/ Derived from] [%00-00 Convert]&][s0; &][s4;:`:`:ConvertInt`:`:MinMax`(int`,int`): "
"[%00-00 ConvertInt`&_][%00-00* MinMax][%00-00 (int_][%00-00*@3 `_min][%00-00 , int_][%00-00*@")
TOPIC_TEXT(
"3 `_max][%00-00 )]&][s6; Sets minimal and maximal allowed numbers.&][s1; [%00-00*C@3 "
"`_min]-|Lower limit.&][s1; [%00-00*C@3 `_max]-|Upper limit.&][s1; [*/ Return value]-|`*this.&][s0; "
"&][s4;:`:`:ConvertInt`:`:Min`(int`): [%00-00 ConvertInt`&_][%00-00* Min][%00-00 (int_][%00-00*@3 "
"`_min][%00-00 )]&][s6; Sets minimal allowed number.&][s1; [%00-00*C@3 `_min]-|Lower "
"limit.&][s1; [*/ Return value]-|`*t")
TOPIC_TEXT(
"his.&][s0; &][s4;:`:`:ConvertInt`:`:Max`(int`): [%00-00 ConvertInt`&_][%00-00* Max][%00-00 "
"(int_][%00-00*@3 `_max][%00-00 )]&][s6; Sets maximal allowed number.&][s1; [%00-00*C@3 "
"`_max]-|Upper limit.&][s1; [*/ Return value]-|`*this.&][s0; &][s4;:`:`:ConvertInt`:`:NotNull`(bool`): "
"[%00-00 ConvertInt`&_][%00-00* NotNull][%00-00 (bool_][%00-00*@3 b][%00-00 _`=_true)]&][s6; "
"Allows/disallows Nulls. (Nul")
TOPIC_TEXT(
"ls are represented by the empty text).&][s1; [%00-00*C@3 b]-|true to disallow.&][s1; "
"[*/ Return value]-|`*this.&][s0; &][s4;:`:`:ConvertInt`:`:NoNotNull`(`): [%00-00 "
"ConvertInt`&_][%00-00* NoNotNull][%00-00 ()]&][s6; Equivalent to NotNull(false).&][s1; "
"[*/ Return value]-|`*this.&][s0; &][s4;:`:`:ConvertInt`:`:GetMin`(`)const: [%00-00 "
"int_][%00-00* GetMin][%00-00 ()_const]&][s1; [*/ Return value]-|")
TOPIC_TEXT(
"Lower limit.&][s0; &][s4;:`:`:ConvertInt`:`:GetMax`(`)const: [%00-00 int_][%00-00* "
"GetMax][%00-00 ()_const]&][s1; [*/ Return value]-|Upper limit.&][s0; &][s4;:`:`:ConvertInt`:`:IsNotNull`(`)const: "
"[%00-00 bool_][%00-00* IsNotNull][%00-00 ()_const]&][s1; [*/ Return value]-|true "
"if null are not allowed.&][s0; &][s4;:`:`:ConvertInt`:`:`:`:ConvertInt`(int`,int`,bool`): "
"[%00-00* ConvertInt][%00-00 (int")
TOPIC_TEXT(
"_][%00-00*@3 minval][%00-00 _`=_`-INT`_MAX, int_][%00-00*@3 maxval][%00-00 _`=_INT`_MAX, "
"bool_][%00-00*@3 notnull][%00-00 _`=_false)]&][s6; Constructor.&][s1; [%00-00*C@3 "
"minval]-|Lower limit. Default value results in no limit.&][s1; [%00-00*C@3 maxval]-|Upper "
"limit. Default value results in no limit.&][s1; [%00-00*C@3 notnull]-|If true, Nulls "
"are not allowed.&][s0; &][s0; &][s0; &][s10;:`:`:Conve")
TOPIC_TEXT(
"rtString`:`:class:*@3 [%00-00*@0 class_][%00-00@0 ConvertString][%00-00*@0 _:_public_][%00-00 "
"Convert]&][s6; String `\"converter`\". Of course, String already is textual representation "
"of itself, the real purpose of this class is to introduce constraints of String value.&][s6; "
"&][s9; [/ Derived from] [%00-00 Convert]&][s0; &][s4;:`:`:ConvertString`:`:MaxLen`(int`): "
"[%00-00 ConvertString`&_][%00-00* ")
TOPIC_TEXT(
"MaxLen][%00-00 (int_][%00-00*@3 `_maxlen][%00-00 )]&][s6; Sets maximum length of "
"String allowed.&][s1; [%00-00*C@3 `_maxlen]-|Length.&][s1; [*/ Return value]-|`*this.&][s0; "
"&][s4;:`:`:ConvertString`:`:GetMaxLength`(`)const: [%00-00 int_][%00-00* GetMaxLength][%00-00 "
"()_const]&][s1; [*/ Return value]-|Maximum length of String allowed.&][s0; &][s4;:`:`:ConvertString`:`:NotNull`(bool`): "
"[%00-00 Conve")
TOPIC_TEXT(
"rtString`&_][%00-00* NotNull][%00-00 (bool_][%00-00*@3 b][%00-00 _`=_true)]&][s6; "
"Disallows empty Strings.&][s1; [%00-00*C@3 b]-|true to disallow.&][s1; [*/ Return "
"value]-|`*this.&][s0; &][s4;:`:`:ConvertString`:`:NoNotNull`(`): [%00-00 ConvertString`&_][%00-00* "
"NoNotNull][%00-00 ()]&][s6; Same as NotNull(false).&][s0; &][s4;:`:`:ConvertString`:`:IsNotNull`(`)const: "
"[%00-00 bool_][%00-00* IsNotNul")
TOPIC_TEXT(
"l][%00-00 ()_const]&][s1; [*/ Return value]-|true mean empty Strings are not allowed.&][s0; "
"&][s4;:`:`:ConvertString`:`:`:`:ConvertString`(int`,bool`): [%00-00* ConvertString][%00-00 "
"(int_][%00-00*@3 maxlen][%00-00 _`=_INT`_MAX, bool_][%00-00*@3 notnull][%00-00 _`=_false)]&][s6; "
"Constructor.&][s1; [%00-00*C@3 maxlen]-|Maximum length `- default is unlimited.&][s1; "
"[%00-00*C@3 notnull]-|If true, emp")
TOPIC_TEXT(
"ty strings are not allowed.&][s0; &][s0; &][s0; &][s10;:`:`:ConvertTime`:`:class:* "
"[%00-00* class_][%00-00 ConvertTime]&][s6; Time converter.&][s6; &][s9; [/ Derived "
"from] [%00-00 Convert]&][s0; &][s4;:`:`:ConvertTime`:`:MinMax`(`:`:Time`,`:`:Time`): "
"[%00-00 ConvertTime`&_][%00-00* MinMax][%00-00 (Time_][%00-00*@3 `_min][%00-00 , "
"Time_][%00-00*@3 `_max][%00-00 )]&][s6; [%00-00 Time limeits.]&][s1;")
TOPIC_TEXT(
" [%00-00*C@3 `_min]-|Lower limit.&][s1; [%00-00*C@3 `_max]-|Upper limit.&][s1; [*/ "
"Return value]-|`*this.&][s0; &][s4;:`:`:ConvertTime`:`:NotNull`(bool`): [%00-00 ConvertTime`&_][%00-00* "
"NotNull][%00-00 (bool_][%00-00*@3 b][%00-00 _`=_true)]&][s6; Disallows empty Strings.&][s1; "
"[%00-00*C@3 b]-|true to disallow.&][s1; [*/ Return value]-|`*this.&][s0; &][s4;:`:`:ConvertTime`:`:NoNotNull`(`): "
"[%00-00")
TOPIC_TEXT(
" ConvertTime`&_][%00-00* NoNotNull][%00-00 ()]&][s6; Same as NotNull(false).&][s0; "
"&][s4;:`:`:ConvertTime`:`:GetMin`(`)const: [%00-00 Time_][%00-00* GetMin][%00-00 "
"()_const]&][s1; [*/ Return value]-|Lower limit.&][s0; &][s4;:`:`:ConvertTime`:`:GetMax`(`)const: "
"[%00-00 Time_][%00-00* GetMax][%00-00 ()_const]&][s1; [*/ Return value]-|Upper limit.&][s0; "
"&][s4;:`:`:ConvertTime`:`:IsNotNull`(`)const: [")
TOPIC_TEXT(
"%00-00 bool_][%00-00* IsNotNull][%00-00 ()_const]&][s1; [*/ Return value]-|true "
"means Nulls are disallowed.&][s0; &][s4;:`:`:ConvertTime`:`:`:`:ConvertTime`(`:`:Time`,`:`:Time`,bool`): "
"[%00-00* ConvertTime][%00-00 (Time_][%00-00*@3 minval][%00-00 _`=_Null, Time_][%00-00*@3 "
"maxval][%00-00 _`=_Null, bool_][%00-00*@3 notnull][%00-00 _`=_false)]&][s6; [%00-00 "
"Constructor.]&][s1; [%00-00*C@3 minval]-|L")
TOPIC_TEXT(
"ower limit. Default means there is no limit.&][s1; [%00-00*C@3 maxval]-|Upper limit. "
"Default means there is no limit.&][s1; [%00-00*C@3 notnull]-|true disallows Nulls.&][s0; "
"&][s0; &][s0; &][s10;:`:`:FormatConvert`:`:class:* [%00-00* class_][%00-00 FormatConvert]&][s6; "
"This unidirectional Convert class (only Format direction implemented) converts single "
"Value or ValueArray using Format function an")
TOPIC_TEXT(
"d specified formatter pattern.&][s9; [/ Derived from] [%00-00 Convert]&][s0; &][s4;:`:`:FormatConvert`:`:SetFormat`(const "
"char`*`): [%00-00 void_][%00-00* SetFormat][%00-00 (const_char_`*][%00-00*@3 fmt][%00-00 "
")]&][s6; Sets formatting pattern.&][s1; [%00-00*C@3 fmt]-|The pattern.&][s0; &][s0;3 "
"&][s0;3 &][s10;:`:`:JoinConvert`:`:class:*@3 [%00-00*@0 class_][%00-00@0 JoinConvert][%00-00*@0 "
"_:_publi")
TOPIC_TEXT(
"c_][%00-00 Convert]&][s6; This unidirectional Convert class (only Format direction "
"is implemented) converts ValueArray using a set of other Convert instances. Resulting "
"textual representation is created by joining a set of defined elements `- some of "
"them static texts, others referring to elements of input ValueArray, converted using "
"specified Convert.&][s6; &][s9; [/ Derived from] [%00-00 Convert")
TOPIC_TEXT(
"]&][s0; &][s4;:`:`:JoinConvert`:`:Add`(const char`*`): [%00-00 JoinConvert`&_][%00-00* "
"Add][%00-00 (const_char_`*][%00-00*@3 text][%00-00 )]&][s6; Adds static text to the "
"list of elements.&][s1; [%00-00*C@3 text]-|Text.&][s1; [*/ Return value]-|`*this.&][s0; "
"&][s4;:`:`:JoinConvert`:`:Add`(int`,const`:`:Convert`&`): [%00-00 JoinConvert`&_][%00-00* "
"Add][%00-00 (int_][%00-00*@3 pos][%00-00 , const_Co")
TOPIC_TEXT(
"nvert`&_][%00-00*@3 cv][%00-00 )]&][s6; Add element referring to input value, to "
"be converted using specified Convert.&][s1; [%00-00*C@3 pos]-|Index of input ValueArray "
"element.&][s1; [%00-00*C@3 cv]-|Convert.&][s1; [*/ Return value]-|`*this.&][s0; &][s4;:`:`:JoinConvert`:`:Add`(int`): "
"[%00-00 JoinConvert`&_][%00-00* Add][%00-00 (int_][%00-00*@3 pos][%00-00 )]&][s6; "
"Add element referring to input ")
TOPIC_TEXT(
"value, to be converted using default Convert.&][s1; [%00-00*C@3 pos]-|Index of input "
"ValueArray element.&][s1; [*/ Return value]-|`*this.&][s0; &][s4;:`:`:JoinConvert`:`:Add`(const`:`:Convert`&`): "
"[%00-00 JoinConvert`&_][%00-00* Add][%00-00 (const_Convert`&_][%00-00*@3 cv][%00-00 "
")]&][s6; Add element referring to input value, to be converted using specified Convert. "
"Index of input element is the i")
TOPIC_TEXT(
"ndex of previous input element plus 1.&][s1; [%00-00*C@3 cv]-|Convert.&][s1; [*/ "
"Return value]-|`*this.&][s0; &][s4;:`:`:JoinConvert`:`:Add`(`): [%00-00 JoinConvert`&_][%00-00* "
"Add][%00-00 ()]&][s6; Add element referring to input value, to be converted using "
"default Convert. Index of input element is the index of previous input element plus "
"1.&][s1; [*/ Return value]-|`*this.&][s0;3 &][s0;3 &][s0;")
TOPIC_TEXT(
"3 &][s3;3 Standard Converts&][s5; Standard converts are simple global functions "
"returning a constant reference to the single global variable representing the particular "
"Convert class. Following table lists names of these functions and respective constructors "
"of Convert classes used to create global variable&][s0;3 &][ {{3310:6690h1;@(204) "
"[s0;1 Function name]:: [s0;1 Definition]::@2 [s0;*C1 StdCon")
TOPIC_TEXT(
"vert]:: [s0;C1 Convert]:: [s0;*C1 StdConvertInt]:: [s0;C1 ConvertInt]:: [s0;*C1 "
"StdConvertIntNotNull]:: [s0;C1 ConvertInt(`-INT`_MAX, INT`_MAX, true)]:: [s0;*C1 "
"StdConvertDouble]:: [s0;C1 ConvertDouble]:: [s0;*C1 StdConvertDoubleNotNull]:: [s0;C1 "
"ConvertDouble(`-DOUBLE`_NULL`_LIM, DOUBLE`_NULL`_LIM, true)]:: [s0;*C1 StdConvertDate]:: "
"[s0;C1 ConvertDate]:: [s0;*C1 StdConvertDateNotNull]:: [s0;C1 Co")
TOPIC_TEXT(
"nvertDate(Date(0, 0, 0), Date(3000, 12, 31), true)]:: [s0;*C1 StdConvertTime]:: "
"[s0;C1 ConvertTime]:: [s0;*C1 StdConvertTimeNotNull]:: [s0;C1 ConvertTime(Null, Null, "
"true)]:: [s0;*C1 StdConvertString]:: [s0;C1 ConvertString]:: [s0;*C1 StdConvertStringNotNull]:: "
"[s0;C1 ConvertString(INT`_MAX, true)]}}&][s0;3 ]")
