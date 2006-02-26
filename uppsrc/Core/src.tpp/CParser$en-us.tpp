TITLE("CParser")
REF("::CParser::class")
REF("::CParser::Error::struct")
REF("::CParser::Error::Error(const char*)")
REF("::CParser::Pos::struct")
REF("::CParser::Pos::Pos(const char*,int,::String)")
REF("::CParser::Pos::ptr")
REF("::CParser::Pos::line")
REF("::CParser::Pos::fn")
REF("::CParser::CParser(const char*)")
REF("::CParser::CParser(const char*,const char*,int)")
REF("::CParser::CParser()")
REF("::CParser::ThrowError(const char*)")
REF("::CParser::NoSkipSpaces()")
REF("::CParser::SkipSpaces()")
REF("::CParser::Spaces()")
REF("::CParser::PeekChar()")
REF("::CParser::GetChar()")
REF("::CParser::IsChar(char)")
REF("::CParser::IsChar2(char,char)")
REF("::CParser::IsChar3(char,char,char)")
REF("::CParser::Char(char)")
REF("::CParser::Char2(char,char)")
REF("::CParser::Char3(char,char,char)")
REF("::CParser::PassChar(char)throw(::CParser::Error)")
REF("::CParser::PassChar2(char,char)throw(::CParser::Error)")
REF("::CParser::PassChar3(char,char,char)throw(::CParser::Error)")
REF("::CParser::Id(const char*)")
REF("::CParser::PassId(const char*)throw(::CParser::Error)")
REF("::CParser::IsId()")
REF("::CParser::ReadId()throw(::CParser::Error)")
REF("::CParser::ReadIdt()throw(::CParser::Error)")
REF("::CParser::IsInt()")
REF("::CParser::ReadInt()throw(::CParser::Error)")
REF("::CParser::IsNumber()")
REF("::CParser::IsNumber(int)")
REF("::CParser::ReadNumber(int)throw(::CParser::Error)")
REF("::CParser::IsDouble()")
REF("::CParser::ReadDouble()throw(::CParser::Error)")
REF("::CParser::IsString()")
REF("::CParser::ReadOneString(bool)throw(::CParser::Error)")
REF("::CParser::ReadString(bool)throw(::CParser::Error)")
REF("::CParser::ReadOneString(int,bool)throw(::CParser::Error)")
REF("::CParser::ReadString(int,bool)throw(::CParser::Error)")
REF("::CParser::SkipTerm()")
REF("::CParser::GetPtr()")
REF("::CParser::GetPos()")
REF("::CParser::SetPos(const::CParser::Pos&)")
REF("::CParser::IsEof()const")
REF("::CParser::operator bool()const")
REF("::CParser::GetLine()const")
REF("::CParser::GetFileName()const")
REF("::AsCString(const char*,const char*,int,const char*,bool)")
REF("::AsCString(const char*,int,const char*,bool)")
REF("::AsCString(const::String&,int,const char*,bool)")
TOPIC_TEXT(
"[2 $$0,0#00000000000000000000000000000000:Default][l288;i1120;a17;O9;~~~.1408;2 "
"$$1,0#10431211400427159095818037425705:param][a83;*R6 $$2,5#31310162474203024125188417583966:caption][b83;*+117 "
"$$3,5#07864147445237544204411237157677:title][i288;b167;a42;O9;C2 $$4,6#40027414424643823182269349404212:item][b42;a42;2 "
"$$5,5#45413000475342174754091244180557:text][l288;a17;2 $$6,6#2752174848137824262002072")
TOPIC_TEXT(
"5143825:desc][l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code][b2503; "
"$$8,0#65142375456100023862071332075487:separator][*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base][t4167;C "
"$$10,0#37138531426314131251341829483380:class][l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement][i417;b42;a42;O9;~~~.416;2 "
"$$12,12#10566046415157235020018451313112:tparam][b167;C2 $$13,1")
TOPIC_TEXT(
"3#92430459443460461911108080531343:item1][i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2][*@2$(0.128.128) "
"$$15,15#34511555403152284025741354420178:NewsDate][l321;*C$7;2 $$16,16#03451589433145915344929335295360:result][l321;b83;a83;*C$7;2 "
"$$17,17#07531550463529505371228428965313:result`-line][l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title][ "
"$$19,0#5358002344")
TOPIC_TEXT(
"2335529039900623488521:gap][t4167;C2 $$20,20#70211524482531209251820423858195:class`-nested][{_}%EN-US "
"[s2;:`:`:CParser`:`:class: [%00-00 CParser]&][s10;* [%00-00* class_][%00-00 CParser]&][s6; "
"CParser is simple but very useful lexical analyzer suitable for building descent "
"parsers of languages with C`-like syntax.&][s6; CParser works on `'`\\0`' terminated "
"text in memory (not Stream), so to parse ")
TOPIC_TEXT(
"the file you have to load it into the memory first. Text has to exist during the "
"time it is processed by CParser (no copy is made).&][s6; Many CParser methods can "
"throw CParser`::Error exception to indicate the failure to parse required symbol. "
"When using CParser to build concrete parser, it is common to use this exception (preferably "
"via ThrowError method) to indicate errors as well.&][s6; CParse")
TOPIC_TEXT(
"r objects cannot be transfered (copied).&][s6; &][s0;3 &][s20;:`:`:CParser`:`:Error`:`:struct:* "
"[%00-00* struct_][%00-00 CParser`::Error]&][s20;* &][s6; Type used as CParser exception. "
"Contains single String with error description.&][s6; &][s9;l288;^topic`:`/`/Core`/src`/util`$en`-us`#`:`:Exc`:`:class "
"Exc^ [/^^ Derived from][^^  ][%00-00 Exc]&][s0;3 &][s4;l288;:`:`:CParser`:`:Error`:`:Error`(const")
TOPIC_TEXT(
" char`*`): [%00-00* Error][%00-00 (const_char_`*][%00-00*@3 s][%00-00 )]&][s6;l448; "
"Constructor.&][s1;l448; [%00-00*C@3 s]-|Error message.&][s6; &][s0;3 &][s20;:`:`:CParser`:`:Pos`:`:struct:* "
"[%00-00* struct_][%00-00 CParser`::Pos]&][s6; &][s6; Position in parsed text.&][s0;3 "
"&][s4;l288;:`:`:CParser`:`:Pos`:`:Pos`(const char`*`,int`,`:`:String`): [%00-00* "
"Pos][%00-00 (const_char_`*][%00-00*@3 ptr]")
TOPIC_TEXT(
"[%00-00 _`=_NULL, int_][%00-00*@3 line][%00-00 _`=_][%00-00@3 1][%00-00 , String_][%00-00*@3 "
"fn][%00-00 _`=_Null)]&][s6;l480; Constructor.&][s1;l480; [%00-00*C@3 ptr]-|Pointer "
"to the position in the input text &][s1;l480; [%00-00*C@3 line]-|Line number.&][s1;l480; "
"[%00-00*C@3 fn]-|Filename.&][s0;3 &][s4;l288;:`:`:CParser`:`:Pos`:`:ptr:* [%00-00* "
"const_char_`*][%00-00 ptr]&][s6; Pointer to the posi")
TOPIC_TEXT(
"tion in the input text &][s0;3 &][s4;l288;:`:`:CParser`:`:Pos`:`:line:* [%00-00* "
"int_][%00-00 line]&][s6;l480; Line number.&][s0;3 &][s4;l288;:`:`:CParser`:`:Pos`:`:fn:* "
"[%00-00* String_][%00-00 fn]&][s6;l480; Filename.&][s6; &][s0;3 &][s4;:`:`:CParser`:`:CParser`(const "
"char`*`): [%00-00* CParser][%00-00 (const_char_`*][%00-00*@3 ptr][%00-00 )]&][s6; "
"Constructs the CParser.&][s1; [%00-00*C@3 ptr]-")
TOPIC_TEXT(
"|Pointer to the input text.&][s0;3 &][s4;:`:`:CParser`:`:CParser`(const char`*`,const "
"char`*`,int`): [%00-00* CParser][%00-00 (const_char_`*][%00-00*@3 ptr][%00-00 , const_char_`*][%00-00*@3 "
"fn][%00-00 , int_][%00-00*@3 line][%00-00 _`=_][%00-00@3 1][%00-00 )]&][s6; Constructs "
"the CParser, with additional information for the text. The additional info can be "
"used when reporting error.&][s1; [%00-00")
TOPIC_TEXT(
"*C@3 ptr]-|Pointer to the input text.&][s1; [%00-00*C@3 fn]-|The name of file (in "
"fact, can be anything, value is just stored).&][s1; [%00-00*C@3 line]-|First line "
"number.&][s0;3 &][s4;:`:`:CParser`:`:CParser`(`): [%00-00* CParser][%00-00 ()]&][s6; "
"Constructs the CParser. Input text has to be assigned using the SetPos method.&][s0;3 "
"&][s4;:`:`:CParser`:`:ThrowError`(const char`*`): [%00-00 void_][")
TOPIC_TEXT(
"%00-00* ThrowError][%00-00 (const_char_`*][%00-00*@3 s][%00-00 )]&][s6; Throws CParser`::Error.&][s1; "
"[%00-00*C@3 s]-|Error message.&][s0;3 &][s4;:`:`:CParser`:`:NoSkipSpaces`(`): [%00-00 "
"void_][%00-00* NoSkipSpaces][%00-00 ()]&][s6; Sets CParser to the mode where white`-spaces "
"are not automatically skipped, but have to be skipped by Spaces method.&][s0;3 &][s4;:`:`:CParser`:`:SkipSpaces`(`): "
"[%00")
TOPIC_TEXT(
"-00 void_][%00-00* SkipSpaces][%00-00 ()]&][s6; Sets CParser to the mode where white`-spaces "
"are automatically skipped. First skip is performed when position in input text is "
"assigned via constructor or SetPos, then the skip is performed after any symbol.&][s0;3 "
"&][s4;:`:`:CParser`:`:Spaces`(`): [%00-00 bool_][%00-00* Spaces][%00-00 ()]&][s6; "
"Skips white`-spaces.&][s1; [*/ Return value]-|true if t")
TOPIC_TEXT(
"here were white`-space to skip.&][s0;3 &][s4;:`:`:CParser`:`:PeekChar`(`): [%00-00 "
"char_][%00-00* PeekChar][%00-00 ()]&][s6; Returns the current single character.&][s1; "
"[*/ Return value]-|Current character.&][s0;3 &][s4;:`:`:CParser`:`:GetChar`(`): [%00-00 "
"char_][%00-00* GetChar][%00-00 ()]&][s6; Advances the position in the input text "
"by one character.&][s1; [*/ Return value]-|Character at positi")
TOPIC_TEXT(
"on before advancing it.&][s0;3 &][s4;:`:`:CParser`:`:IsChar`(char`): [%00-00 bool_][%00-00* "
"IsChar][%00-00 (char_][%00-00*@3 c][%00-00 )]&][s6; Tests whether there is a specific "
"character at the current position.&][s1; [%00-00*C@3 c]-|Character to test.&][s1; "
"[*/ Return value]-|true on match.&][s0;3 &][s4;:`:`:CParser`:`:IsChar2`(char`,char`): "
"[%00-00 bool_][%00-00* IsChar2][%00-00 (char_][%00-00*")
TOPIC_TEXT(
"@3 c1][%00-00 , char_][%00-00*@3 c2][%00-00 )]&][s6; Tests whether there is a specific "
"character pair at the current position.&][s1; [%00-00*C@3 c1]-|First character of "
"pair.&][s1; [%00-00*C@3 c2]-|Second character of pair.&][s1; [*/ Return value]-|true "
"on match.&][s0;3 &][s4;:`:`:CParser`:`:IsChar3`(char`,char`,char`): [%00-00 bool_][%00-00* "
"IsChar3][%00-00 (char_][%00-00*@3 c1][%00-00 , char_][%")
TOPIC_TEXT(
"00-00*@3 c2][%00-00 , char_][%00-00*@3 c3][%00-00 )]&][s6; Test for a specific character "
"triplet at the current position.&][s1; [%00-00*C@3 c1]-|First character of triplet.&][s1; "
"[%00-00*C@3 c2]-|Second character of triplet.&][s1; [%00-00*C@3 c3]-|Third character "
"of triplet.&][s1; [*/ Return value]-|true on match.&][s0;3 &][s4;:`:`:CParser`:`:Char`(char`): "
"[%00-00 bool_][%00-00* Char][%00-00 (char")
TOPIC_TEXT(
"_][%00-00*@3 c][%00-00 )]&][s6; Tests for a single character at the current position. "
"If there is match, position is advanced.&][s1; [%00-00*C@3 c]-|Character to test.&][s1; "
"[*/ Return value]-|true on match.&][s0;3 &][s4;:`:`:CParser`:`:Char2`(char`,char`): "
"[%00-00 bool_][%00-00* Char2][%00-00 (char_][%00-00*@3 c1][%00-00 , char_][%00-00*@3 "
"c2][%00-00 )]&][s6; Tests for a character pair at the cur")
TOPIC_TEXT(
"rent position. If there is match, position is advanced by two characters.&][s1; "
"[%00-00*C@3 c1]-|First character of pair.&][s1; [%00-00*C@3 c2]-|Second character "
"of pair.&][s1; [*/ Return value]-|true on match.&][s0;3 &][s4;:`:`:CParser`:`:Char3`(char`,char`,char`): "
"[%00-00 bool_][%00-00* Char3][%00-00 (char_][%00-00*@3 c1][%00-00 , char_][%00-00*@3 "
"c2][%00-00 , char_][%00-00*@3 c3][%00-00 )]&][s6")
TOPIC_TEXT(
"; Tests for a character triplet at the current position. If there is match, position "
"is advanced by three characters.&][s1; [%00-00*C@3 c1]-|First character of triplet.&][s1; "
"[%00-00*C@3 c2]-|Second character of triplet.&][s1; [%00-00*C@3 c3]-|Third character "
"of triplet.&][s1; [*/ Return value]-|true on match.&][s4;:`:`:CParser`:`:PassChar`(char`)throw`(`:`:CParser`:`:Error`): "
"[%00-00 void_][%00-0")
TOPIC_TEXT(
"0* PassChar][%00-00 (char_][%00-00*@3 c][%00-00 )_throw(Error)]&][s6; Calls Char(c). "
"If it returns false, throws error.&][s1; [%00-00*C@3 c]-|Character to test.&][s0;3 "
"&][s4;:`:`:CParser`:`:PassChar2`(char`,char`)throw`(`:`:CParser`:`:Error`): [%00-00 "
"void_][%00-00* PassChar2][%00-00 (char_][%00-00*@3 c1][%00-00 , char_][%00-00*@3 "
"c2][%00-00 )_throw(Error)]&][s6; Calls Char2(c1, c2). If it returns")
TOPIC_TEXT(
" false, throws error.&][s1; [%00-00*C@3 c1]-|First character of pair.&][s1; [%00-00*C@3 "
"c2]-|Second character of pair.&][s0;3 &][s4;:`:`:CParser`:`:PassChar3`(char`,char`,char`)throw`(`:`:CParser`:`:Error`): "
"[%00-00 void_][%00-00* PassChar3][%00-00 (char_][%00-00*@3 c1][%00-00 , char_][%00-00*@3 "
"c2][%00-00 , char_][%00-00*@3 c3][%00-00 )_throw(Error)]&][s6; Calls Char2(c1, c2, "
"c3). If it returns f")
TOPIC_TEXT(
"alse, throws the Error.&][s1; [%00-00*C@3 c1]-|First character of triplet.&][s1; "
"[%00-00*C@3 c2]-|Second character of triplet.&][s1; [%00-00*C@3 c3]-|Third character "
"of triplet.&][s0;3 &][s4;:`:`:CParser`:`:Id`(const char`*`): [%00-00 bool_][%00-00* "
"Id][%00-00 (const_char_`*][%00-00*@3 s][%00-00 )]&][s6; Tests for given C`-like identifier. "
"If there is match, advances position by strlen(s) characte")
TOPIC_TEXT(
"rs.&][s1; [%00-00*C@3 s]-|Identifier.&][s1; [*/ Return value]-|true on match.&][s0;3 "
"&][s4;:`:`:CParser`:`:PassId`(const char`*`)throw`(`:`:CParser`:`:Error`): [%00-00 "
"void_][%00-00* PassId][%00-00 (const_char_`*][%00-00*@3 s][%00-00 )_throw(Error)]&][s6; "
"Invokes the Id method with s as parameter. If it returns false, throws the Error.&][s1; "
"[%00-00*C@3 s]-|Identifier.&][s0;3 &][s4;:`:`:CParser`:`")
TOPIC_TEXT(
":IsId`(`): [%00-00 bool_][%00-00* IsId][%00-00 ()]&][s6; Tests whether there is "
"any C`-like identifier at the current position.&][s1; [*/ Return value]-|true if "
"there is identifier.&][s0;3 &][s4;:`:`:CParser`:`:ReadId`(`)throw`(`:`:CParser`:`:Error`): "
"[%00-00 String_][%00-00* ReadId][%00-00 ()_throw(Error)]&][s6; Reads C`-like identifier "
"from the current position. If there is none, an Error is thr")
TOPIC_TEXT(
"own.&][s1; [*/ Return value]-|Identifier.&][s0;3 &][s4;:`:`:CParser`:`:ReadIdt`(`)throw`(`:`:CParser`:`:Error`): "
"[%00-00 String_][%00-00* ReadIdt][%00-00 ()_throw(Error)]&][s6; Special variant of "
"ReadId that considers different non`-alphanumeric characters to be the part of identifier "
"as long as they form C`+`+ normal or template based type.&][s1; [*/ Return value]-|Identifier.&][s0;3 "
"&][s4;:`:`:C")
TOPIC_TEXT(
"Parser`:`:IsInt`(`): [%00-00 bool_][%00-00* IsInt][%00-00 ()]&][s6; Test for integer "
"at current position `- there either must be digit, or `'`+`' or `'`-`' sign followed "
"by any number of spaces and digit.&][s1; [*/ Return value]-|true if there is integer.&][s0;3 "
"&][s4;:`:`:CParser`:`:ReadInt`(`)throw`(`:`:CParser`:`:Error`): [%00-00 int_][%00-00* "
"ReadInt][%00-00 ()_throw(Error)]&][s6; Reads the in")
TOPIC_TEXT(
"teger from the current position. If IsInt is false, throws an Error.&][s1; [*/ Return "
"value]-|Integer.&][s0;3 &][s4;:`:`:CParser`:`:IsNumber`(`): [%00-00 bool_][%00-00* "
"IsNumber][%00-00 ()]&][s6; Tests for sign`-less number at current position `- there "
"must be digit at current position.&][s1; [*/ Return value]-|true if there is number.&][s0;3 "
"&][s4;:`:`:CParser`:`:IsNumber`(int`): [%00-00 bool_][%")
TOPIC_TEXT(
"00-00* IsNumber][%00-00 (int_][%00-00*@3 base][%00-00 )]&][s6; Tests for sign`-less "
"number with given base `- there must be digit or letter `'A`' `- `'Z`' or `'a`' `- "
"`'z`', where range is limit by acutal base (e.g. for base 12 letters `'a`' `'A`' "
"`'b`' `'B`' are allowed).&][s1; [%00-00*C@3 base]-|Numeric base.&][s1; [*/ Return "
"value]-|true if there is number with given numeric base.&][s0;3 &][s4;")
TOPIC_TEXT(
":`:`:CParser`:`:ReadNumber`(int`)throw`(`:`:CParser`:`:Error`): [%00-00 uint32_][%00-00* "
"ReadNumber][%00-00 (int_][%00-00*@3 base][%00-00 _`=_][%00-00@3 10][%00-00 )_throw(Error)]&][s6; "
"Reads a number with the given numeric base. If IsNumber(base) is false, throws an "
"Error.&][s1; [%00-00*C@3 base]-|Numeric base.&][s1; [*/ Return value]-|Number.&][s0;3 "
"&][s4;:`:`:CParser`:`:IsDouble`(`): [%00-00 bo")
TOPIC_TEXT(
"ol_][%00-00* IsDouble][%00-00 ()]&][s6; Test for floating point number at current "
"position `- there either must be digit, or `'`+`' or `'`-`' sign followed by any "
"number of spaces and digit.&][s1; [*/ Return value]-|true if there is the floating "
"point number.&][s0;3 &][s4;:`:`:CParser`:`:ReadDouble`(`)throw`(`:`:CParser`:`:Error`): "
"[%00-00 double_][%00-00* ReadDouble][%00-00 ()_throw(Error)]&][s6;")
TOPIC_TEXT(
" Reads a floating point number with C based lexical rules.&][s1; [*/ Return value]-|Floating "
"point number.&][s0;3 &][s4;:`:`:CParser`:`:IsString`(`): [%00-00 bool_][%00-00* IsString][%00-00 "
"()]&][s6; Tests for C`-like string literal at the current position. Same as IsChar(`'`\\`\"`');&][s1; "
"[*/ Return value]-|true when there is string literal.&][s0;3 &][s4;:`:`:CParser`:`:ReadOneString`(bool`)throw`")
TOPIC_TEXT(
"(`:`:CParser`:`:Error`): [%00-00 String_][%00-00* ReadOneString][%00-00 (bool_][%00-00*@3 "
"chkend][%00-00 _`=_false)_throw(Error)]&][s6; Reads C`-like string literal from current "
"position (follow C lexical rules, including escape codes). Literals on different "
"lines are not concatenated (unlike C).&][s1; [%00-00*C@3 chkend]-|When false, ReadOneString "
"is more permissive as it allows unterminated stri")
TOPIC_TEXT(
"ng literals `- string is then also delimited by end of line or text.&][s1; [*/ Return "
"value]-|String literal.&][s0;3 &][s4;:`:`:CParser`:`:ReadString`(bool`)throw`(`:`:CParser`:`:Error`): "
"[%00-00 String_][%00-00* ReadString][%00-00 (bool_][%00-00*@3 chkend][%00-00 _`=_false)_throw(Error)]&][s6; "
"Reads C`-like string literal from current position (follow C lexical rules, including "
"escape codes). Lit")
TOPIC_TEXT(
"erals on different lines are concatenated (as in C).&][s1; [%00-00*C@3 chkend]-|When "
"false, ReadOneString is more permissive as it allows unterminated string literals "
"`- string is then also delimited by end of line or text.&][s1; [*/ Return value]-|String "
"literal.&][s0;3 &][s4;:`:`:CParser`:`:ReadOneString`(int`,bool`)throw`(`:`:CParser`:`:Error`): "
"[%00-00 String_][%00-00* ReadOneString][%00-00 (i")
TOPIC_TEXT(
"nt_][%00-00*@3 delim][%00-00 , bool_][%00-00*@3 chkend][%00-00 _`=_false)_throw(Error)]&][s6; "
"Reads C`-like string literal from current position (follow C lexical rules, including "
"escape codes) with different delimiter than `'`\\`\"`'. Literals on different lines "
"are not concatenated (unlike C).&][s1; [%00-00*C@3 delim]-|Delimiter.&][s1; [%00-00*C@3 "
"chkend]-|When false, ReadOneString is more permiss")
TOPIC_TEXT(
"ive as it allows unterminated string literals `- string is then also delimited by "
"end of line or text.&][s1; [*/ Return value]-|String literal.&][s0;3 &][s4;:`:`:CParser`:`:ReadString`(int`,bool`)throw`(`:`:CParser`:`:Error`): "
"[%00-00 String_][%00-00* ReadString][%00-00 (int_][%00-00*@3 delim][%00-00 , bool_][%00-00*@3 "
"chkend][%00-00 _`=_false)_throw(Error)]&][s6; Reads C`-like string literal from")
TOPIC_TEXT(
" current position (follow C lexical rules, including escape codes). with different "
"delimiter than `'`\\`\"`'. Literals on different lines are concatenated (as in C).&][s1; "
"[%00-00*C@3 delim]-|Delimiter.&][s1; [%00-00*C@3 chkend]-|When false, ReadOneString "
"is more permissive as it allows unterminated string literals `- string is then also "
"delimited by end of line or text.&][s1; [*/ Return value]-|Str")
TOPIC_TEXT(
"ing literal.&][s0;3 &][s4;:`:`:CParser`:`:SkipTerm`(`): [%00-00 void_][%00-00* SkipTerm][%00-00 "
"()]&][s6; Skips a single symbol. Decimal numbers, identifiers and string literals "
"are skipped as whole symbols, otherwise input position is advanced by 1 character.&][s0;3 "
"&][s4;:`:`:CParser`:`:GetPtr`(`): [%00-00 const_char_`*][%00-00* GetPtr][%00-00 ()]&][s6; "
"Gets a pointer to the current position.&][")
TOPIC_TEXT(
"s1; [*/ Return value]-|Pointer to current position.&][s0;3 &][s4;:`:`:CParser`:`:GetPos`(`): "
"[%00-00 Pos_][%00-00* GetPos][%00-00 ()]&][s6; Gets the current position,.&][s1; "
"[*/ Return value]-|Current position. It contains the pointer as well as the line "
"number and the filename.&][s0;3 &][s4;:`:`:CParser`:`:SetPos`(const`:`:CParser`:`:Pos`&`): "
"[%00-00 void_][%00-00* SetPos][%00-00 (const_Pos`&_][%")
TOPIC_TEXT(
"00-00*@3 pos][%00-00 )]&][s6; Sets the current position.&][s1; [%00-00*C@3 pos]-|New "
"current position. Can be in different text than previously used in CParser.&][s0;3 "
"&][s4;:`:`:CParser`:`:IsEof`(`)const: [%00-00 bool_][%00-00* IsEof][%00-00 () const]&][s6; "
"Test for the end of input text.&][s1; [*/ Return value]-|true when current position "
"is a the end of input text (`'`\\0`' character).&][s0;3 &]")
TOPIC_TEXT(
"[s4;:`:`:CParser`:`:operator bool`(`)const: [%00-00 operator_bool()_const]&][s1; "
"[*/ Return value]-|!IsEof().&][s0; &][s4;:`:`:CParser`:`:GetLine`(`)const: [%00-00 "
"int_][%00-00* GetLine][%00-00 ()_const]&][s1; [*/ Return value]-|Current line.&][s0;3 "
"&][s4;:`:`:CParser`:`:GetFileName`(`)const: [%00-00 String_][%00-00* GetFileName][%00-00 "
"()_const]&][s1; [*/ Return value]-|Actual filename.&][s0; &][")
TOPIC_TEXT(
"s0; &][s0; &][s0; &][s3; C`-like string literal formatting&][s5; &][s5; AsCString "
"routines produce C`-like literals (compatible with CParser) from character data:&][s0;3 "
"&][s4;:`:`:AsCString`(const char`*`,const char`*`,int`,const char`*`,bool`): [%00-00 "
"String_][%00-00* AsCString][%00-00 (const_char_`*][%00-00*@3 s][%00-00 , const_char_`*][%00-00*@3 "
"end][%00-00 , int_][%00-00*@3 linemax][%00-00 _")
TOPIC_TEXT(
"`=_INT`_MAX, const_char_`*][%00-00*@3 linepfx][%00-00 _`=_NULL, bool_][%00-00*@3 "
"smart][%00-00 _`=_false)]&][s6; Creates C`-like literal.&][s1; [%00-00*C@3 s]-|Pointer "
"to characters.&][s1; [%00-00*C@3 end]-|End of characters array (`'`\\0`' characters "
"are allowed inside data).&][s1; [%00-00*C@3 linemax]-|Maximal length of line. If "
"this is exceeded, ending `\"`\\`\"`\\n`\" and [@3 linepfx] is inserted an")
TOPIC_TEXT(
"d literal continues on the new line.&][s1; [%00-00*C@3 linepfx]-|Pointer to zero`-terminated "
"text to be inserted at the beginning of the line when the line length is exceeded.&][s1; "
"[%00-00*C@3 smart]-|true activates smarter breaking into lines `- breaks at spaces "
"are preferred.&][s1; [*/ Return value]-|C`-like literal.&][s0;3 &][s4;:`:`:AsCString`(const "
"char`*`,int`,const char`*`,bool`): [%00-00 ")
TOPIC_TEXT(
"String_][%00-00* AsCString][%00-00 (const_char_`*][%00-00*@3 s][%00-00 , int_][%00-00*@3 "
"linemax][%00-00 _`=_INT`_MAX, const_char_`*][%00-00*@3 linepfx][%00-00 _`=_NULL, "
"bool_][%00-00*@3 smart][%00-00 _`=_false)]&][s6; Creates C`-like literal from zero "
"terminated character data. Same as AsCString([@3 s], [@3 s] `+ strlen([@3 s]), [@3 "
"linemax], [@3 linepfx], [@3 smart]).&][s0;3 &][s4;:`:`:AsCString")
TOPIC_TEXT(
"`(const`:`:String`&`,int`,const char`*`,bool`): [%00-00 String_][%00-00* AsCString][%00-00 "
"(const_String`&_][%00-00*@3 s][%00-00 , int_][%00-00*@3 linemax][%00-00 _`=_INT`_MAX, "
"const_char_`*][%00-00*@3 linepfx][%00-00 _`=_NULL, bool_][%00-00*@3 smart][%00-00 "
"_`=_false)]&][s6; Creates C`-like literal from String. String can contain zero characters. "
"Same as AsCString([@3 s].Begin(), [@3 s].End(), [@")
TOPIC_TEXT(
"3 linemax], [@3 linepfx], [@3 smart]).&][s0;3 ]")
