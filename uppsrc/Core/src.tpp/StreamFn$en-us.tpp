TITLE("Stream utilities")
REF("::LoadStream(::Stream&)")
REF("::SaveStream(::Stream&,const::String&)")
REF("::CopyStream(::Stream&,::Stream&,::int64)")
REF("::CopyStream(::Stream&,::Stream&)")
REF("::Cout()")
REF("::ReadStdIn()")
REF("::NilStream()")
REF("::LoadFile(const char*)")
REF("::SaveFile(const char*,const::String&)")
REF("::operator%(::Stream&,T&)")
REF("::operator<<(::Stream&,const char*)")
REF("::operator<<(::Stream&,char*)")
REF("::operator<<(::Stream&,const::String&)")
REF("::operator<<(::Stream&,char)")
REF("::operator<<(::Stream&,const void*)")
REF("::operator<<(::Stream&,void*)")
REF("::operator<<(::Stream&,const T&)")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2; Stream utilities&][s0;3 "
"&][s4;:`:`:LoadStream`(`:`:Stream`&`): [%00-00 String_][%00-00* LoadStream][%00-00 "
"(Stream`&_][%00-00*@3 in][%00-00 )]&][s6; Reads the stream starting with the current "
"position till the end is reached and returns data in Str")
TOPIC_TEXT(
"ing.&][s1; [%00-00*C@3 in]-|Stream.&][s1; [*/ Return value]-|Content of stream.&][s0;3 "
"&][s4;:`:`:SaveStream`(`:`:Stream`&`,const`:`:String`&`): [%00-00 bool_][%00-00* "
"SaveStream][%00-00 (Stream`&_][%00-00*@3 out][%00-00 , const_String`&_][%00-00*@3 "
"data][%00-00 )]&][s6; Writes data to stream.&][s1; [%00-00*C@3 out]-|Output stream.&][s1; "
"[%00-00*C@3 data]-|Data to write.&][s1; [*/ Return value]-|T")
TOPIC_TEXT(
"rue if all data were successfully written to the stream.&][s0;3 &][s4;:`:`:CopyStream`(`:`:Stream`&`,`:`:Stream`&`,`:`:int64`): "
"[%00-00 int64_][%00-00* CopyStream][%00-00 (Stream`&_][%00-00*@3 dest][%00-00 , Stream`&_][%00-00*@3 "
"src][%00-00 , int64_][%00-00*@3 count][%00-00 )]&][s6; Copies data from source to "
"destination stream.&][s1; [%00-00*C@3 dest]-|Destination stream.&][s1; [%00-00*C@3 "
"src]-|")
TOPIC_TEXT(
"Source stream.&][s1; [%00-00*C@3 count]-|Number of bytes to copy.&][s1; [*/ Return "
"value]-|Number of bytes actually copies.&][s0;3 &][s4;:`:`:CopyStream`(`:`:Stream`&`,`:`:Stream`&`): "
"[%00-00 int64_][%00-00* CopyStream][%00-00 (Stream`&_][%00-00*@3 dest][%00-00 , Stream`&_][%00-00*@3 "
"src][%00-00 )]&][s6; Copies all data from source stream starting with current position "
"to the destination stream.&]")
TOPIC_TEXT(
"[s1; [%00-00*C@3 dest]-|Destination stream.&][s1; [%00-00*C@3 src]-|Source stream.&][s1; "
"[*/ Return value]-|Number of bytes copied.&][s0;3 &][s4;:`:`:Cout`(`): [%00-00 Stream`&_][%00-00* "
"Cout][%00-00 ()]&][s6; Returns special output stream representing console output. "
"Data written to this stream are displayed as characters in console.&][s1; [*/ Return "
"value]-|Console stream.&][s0;3 &][s4;:`:`:Read")
TOPIC_TEXT(
"StdIn`(`): [%00-00 String_][%00-00* ReadStdIn][%00-00 ()]&][s6; Reads one line of "
"input data from the console.&][s1; [*/ Return value]-|Console input.&][s0;3 &][s4;:`:`:NilStream`(`): "
"[%00-00 Stream`&_][%00-00* NilStream][%00-00 ()]&][s6; Returns special stream that "
"is always in IsEof state and simply discards all data written to it.&][s1; [*/ Return "
"value]-|`\"Black hole`\" stream.&][s0;3 &][s4;:`:")
TOPIC_TEXT(
"`:LoadFile`(const char`*`): [%00-00 String_][%00-00* LoadFile][%00-00 (const_char_`*][%00-00*@3 "
"filename][%00-00 )]&][s6; Loads the content of specified file.&][s1; [%00-00*C@3 "
"filename]-|File name.&][s1; [*/ Return value]-|Content of file.&][s0;3 &][s4;:`:`:SaveFile`(const "
"char`*`,const`:`:String`&`): [%00-00 bool_][%00-00* SaveFile][%00-00 (const_char_`*][%00-00*@3 "
"filename][%00-00 , const_Strin")
TOPIC_TEXT(
"g`&_][%00-00*@3 data][%00-00 )]&][s6; Saves data as the file (overwrites existing).&][s1; "
"[%00-00*C@3 filename]-|File name.&][s1; [%00-00*C@3 data]-|Data to write.&][s1; [*/ "
"Return value]-|true if file was successfully written.&][s0;3 &][s4;:`:`:operator`%`(`:`:Stream`&`,T`&`): "
"[%00-00 template <class ][%00-00*@4 T][%00-00 > Stream`&_][%00-00* operator%][%00-00 "
"(Stream`&_][%00-00*@3 s][%00-00 , ][")
TOPIC_TEXT(
"%00-00*@4 T][%00-00 `&_][%00-00*@3 x][%00-00 )]&][s6; Serialization operator. Simply "
"invokes x.Serialize(s);&][s1; [*C@4 T]-|Type of variable to be serialized.&][s1; "
"[%00-00*C@3 s]-|Stream.&][s1; [%00-00*C@3 x]-|Variable to be serialized.&][s1; [*/ "
"Return value]-|s for chaining.&][s0;3 &][s4;:`:`:operator`<`<`(`:`:Stream`&`,const "
"char`*`): [%00-00 Stream`&_][%00-00* operator<<][%00-00 (Stream`&_][")
TOPIC_TEXT(
"%00-00*@3 s][%00-00 , const_char_`*][%00-00*@3 x][%00-00 )]&][s6; Overload of stream "
"insertion operator to get simple case work. Insertion operator uses formatted stream "
"output (Putf).&][s1; [%00-00*C@3 s]-|Output stream.&][s1; [%00-00*C@3 x]-|Data to "
"write `- all characters are written to the stream.&][s1; [*/ Return value]-|s for "
"chaining.&][s0;3 &][s4;:`:`:operator`<`<`(`:`:Stream`&`,char`*`): ")
TOPIC_TEXT(
"[%00-00 Stream`&_][%00-00* operator<<][%00-00 (Stream`&_][%00-00*@3 s][%00-00 , "
"char_`*][%00-00*@3 x][%00-00 )]&][s6; Overload of stream insertion operator to get "
"simple case work. Insertion operator uses formatted stream output (Putf).&][s1; [%00-00*C@3 "
"s]-|Output stream.&][s1; [%00-00*C@3 x]-|Data to write `- all characters are written "
"to the stream.&][s1; [*/ Return value]-|s for chaining.&][s0")
TOPIC_TEXT(
";3 &][s4;:`:`:operator`<`<`(`:`:Stream`&`,const`:`:String`&`): [%00-00 Stream`&_][%00-00* "
"operator<<][%00-00 (Stream`&_][%00-00*@3 s][%00-00 , const_String_`&][%00-00*@3 x][%00-00 "
")]&][s6; Overload of stream insertion operator to get simple case work. Insertion "
"operator uses formatted stream output (Putf).&][s1; [%00-00*C@3 s]-|Output stream.&][s1; "
"[%00-00*C@3 x]-|Data to write `- all characters a")
TOPIC_TEXT(
"re written to the stream.&][s1; [*/ Return value]-|s for chaining.&][s0;3 &][s4;:`:`:operator`<`<`(`:`:Stream`&`,char`): "
"[%00-00 Stream`&_][%00-00* operator<<][%00-00 (Stream`&_][%00-00*@3 s][%00-00 , char_][%00-00*@3 "
"x][%00-00 )]&][s6; Overload of stream insertion operator to get simple case work. "
"Insertion operator uses formatted stream output (Putf).&][s1; [%00-00*C@3 s]-|Output "
"stream.&][s1; [")
TOPIC_TEXT(
"%00-00*C@3 x]-|Data to write `- it is written as single character.&][s1; [*/ Return "
"value]-|s for chaining.&][s0;3 &][s4;:`:`:operator`<`<`(`:`:Stream`&`,const void`*`): "
"[%00-00 Stream`&_][%00-00* operator<<][%00-00 (Stream`&_][%00-00*@3 s][%00-00 , const_void_`*][%00-00*@3 "
"x][%00-00 )]&][s6; Overload of stream insertion operator to get simple case work. "
"Insertion operator uses formatted stream ou")
TOPIC_TEXT(
"tput (Putf).&][s1; [%00-00*C@3 s]-|Output stream.&][s1; [%00-00*C@3 x]-|Data to "
"write `- pointer is formatted as hexadecimal value.&][s1; [*/ Return value]-|s for "
"chaining.&][s0;3 &][s4;:`:`:operator`<`<`(`:`:Stream`&`,void`*`): [%00-00 Stream`&_][%00-00* "
"operator<<][%00-00 (Stream`&_][%00-00*@3 s][%00-00 , void_`*][%00-00*@3 x][%00-00 "
")]&][s6; Overload of stream insertion operator to get simple c")
TOPIC_TEXT(
"ase work. Insertion operator uses formatted stream output (Putf).&][s1; [%00-00*C@3 "
"s]-|Output stream.&][s1; [%00-00*C@3 x]-|Data to write `- pointer is formatted as "
"hexadecimal value.&][s1; [*/ Return value]-|s for chaining.&][s0;3 &][s4;:`:`:operator`<`<`(`:`:Stream`&`,const "
"T`&`): template <class [*@4 T]> [%00-00 Stream`&_][%00-00* operator<<][%00-00 (Stream`&_][%00-00*@3 "
"s][%00-00 , const_][%0")
TOPIC_TEXT(
"0-00*@4 T][%00-00 `&_][%00-00*@3 x][%00-00 )]&][s6; Global stream insertion operator. "
"Calls AsString for x and stores the result to the stream. Global AsString version "
"in turn calls ToString method of x.&][s1; [*C@4 T]-|Type of data to write to the "
"stream.&][s1; [%00-00*C@3 s]-|Output stream.&][s1; [%00-00*C@3 x]-|Data to write.&][s1; "
"[*/ Return value]-|s for chaining.]")
