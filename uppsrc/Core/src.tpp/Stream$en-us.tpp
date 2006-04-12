TITLE("Streams")
REF("::Stream::class")
REF("::Stream::pos")
REF("::Stream::buffer")
REF("::Stream::ptr")
REF("::Stream::rdlim")
REF("::Stream::wrlim")
REF("::Stream::_Put(int)")
REF("::Stream::_Term()")
REF("::Stream::_Get()")
REF("::Stream::_Put(const void*,::dword)")
REF("::Stream::_Get(void*,::dword)")
REF("::Stream::Seek(::int64)")
REF("::Stream::GetSize()const")
REF("::Stream::SetSize(::int64)")
REF("::Stream::Flush()")
REF("::Stream::Close()")
REF("::Stream::IsOpen()const")
REF("::Stream::IsError()const")
REF("::Stream::IsOK()const")
REF("::Stream::SetError(int)")
REF("::Stream::SetLastError()")
REF("::Stream::GetError()const")
REF("::Stream::ClearError()")
REF("::Stream::GetPos()const")
REF("::Stream::IsEof()const")
REF("::Stream::GetLeft()const")
REF("::Stream::SeekEnd(::int64)")
REF("::Stream::SeekCur(::int64)")
REF("::Stream::Put(int)")
REF("::Stream::Term()")
REF("::Stream::Get()")
REF("::Stream::Put(const void*,::dword)")
REF("::Stream::Get(void*,::dword)")
REF("::Stream::LoadThrowing()")
REF("::Stream::LoadError()")
REF("::Stream::GetAll(void*,::dword)")
REF("::Stream::Get8()")
REF("::Stream::Get16()")
REF("::Stream::Get32()")
REF("::Stream::Get64()")
REF("::Stream::Get16le()")
REF("::Stream::Get32le()")
REF("::Stream::Get64le()")
REF("::Stream::Get16be()")
REF("::Stream::Get32be()")
REF("::Stream::Get64be()")
REF("::Stream::GetLine()")
REF("::Stream::GetUtf8()")
REF("::Stream::Put16(word)")
REF("::Stream::Put32(int)")
REF("::Stream::Put64(::int64)")
REF("::Stream::Put16le(word)")
REF("::Stream::Put32le(int)")
REF("::Stream::Put64le(::int64)")
REF("::Stream::Put16be(word)")
REF("::Stream::Put32be(int)")
REF("::Stream::Put64be(::int64)")
REF("::Stream::PutUtf8(int)")
REF("::Stream::Put(const char*)")
REF("::Stream::Put(const::String&)")
REF("::Stream::Put(int,int)")
REF("::Stream::Put0(int)")
REF("::Stream::PutW(const::wchar*,int)")
REF("::Stream::GetW(::wchar*,int)")
REF("::Stream::GetAllW(::wchar*,int)")
REF("::Stream::PutCrLf()")
REF("::Stream::PutEol()")
REF("::Stream::PutLine(const char*)")
REF("::Stream::PutLine(const::String&)")
REF("::Stream::Put(::Stream&,::int64,::dword)")
REF("::Stream::Putf(int)")
REF("::Stream::Putf(const char*)")
REF("::Stream::Putf(const::String&)")
REF("::Stream::Begin()")
REF("::Stream::End()")
REF("::Stream::operator<<(BeginIndentEnum)")
REF("::Stream::operator<<(EndIndentEnum)")
REF("::Stream::SetLoading()")
REF("::Stream::SetStoring()")
REF("::Stream::IsLoading()")
REF("::Stream::IsStoring()")
REF("::Stream::SerializeRaw(::byte*,::dword)")
REF("::Stream::SerializeRaw(word*,::dword)")
REF("::Stream::SerializeRaw(::dword*,::dword)")
REF("::Stream::SerializeRaw(::uint64*,::dword)")
REF("::Stream::operator%(bool&)")
REF("::Stream::operator%(char&)")
REF("::Stream::operator%(signed char&)")
REF("::Stream::operator%(unsigned char&)")
REF("::Stream::operator%(short&)")
REF("::Stream::operator%(unsigned short&)")
REF("::Stream::operator%(int&)")
REF("::Stream::operator%(unsigned int&)")
REF("::Stream::operator%(long&)")
REF("::Stream::operator%(unsigned long&)")
REF("::Stream::operator%(float&)")
REF("::Stream::operator%(double&)")
REF("::Stream::operator%(::int64&)")
REF("::Stream::operator%(::uint64&)")
REF("::Stream::operator%(::String&)")
REF("::Stream::operator/(::String&)")
REF("::Stream::operator%(::WString&)")
REF("::Stream::operator/(::WString&)")
REF("::Stream::Pack(::dword&)")
REF("::Stream::operator/(int&)")
REF("::Stream::operator/(unsigned int&)")
REF("::Stream::Magic(::dword)")
REF("::Stream::Pack(bool&,bool&,bool&,bool&,bool&,bool&,bool&,bool&)")
REF("::Stream::Pack(bool&,bool&,bool&,bool&,bool&,bool&,bool&)")
REF("::Stream::Pack(bool&,bool&,bool&,bool&,bool&,bool&)")
REF("::Stream::Pack(bool&,bool&,bool&,bool&,bool&)")
REF("::Stream::Pack(bool&,bool&,bool&,bool&)")
REF("::Stream::Pack(bool&,bool&,bool&)")
REF("::Stream::Pack(bool&,bool&)")
REF("::Stream::Stream()")
REF("::Stream::~Stream()")
REF("::StringStream::class")
REF("::StringStream::Open(const::String&)")
REF("::StringStream::Create()")
REF("::StringStream::GetResult()")
REF("::StringStream::operator::String()")
REF("::StringStream::StringStream()")
REF("::StringStream::StringStream(const::String&)")
REF("::MemStream::class")
REF("::MemStream::MemStream(void*,int)")
REF("::MemReadStream::class")
REF("::MemReadStream::MemReadStream(const void*,int)")
REF("::BlockStream::class")
REF("::BlockStream::READ")
REF("::BlockStream::CREATE")
REF("::BlockStream::APPEND")
REF("::BlockStream::READWRITE")
REF("::BlockStream::NOWRITESHARE")
REF("::BlockStream::DELETESHARE")
REF("::BlockStream::NOREADSHARE")
REF("::BlockStream::Read(::int64,void*,::dword)")
REF("::BlockStream::Write(::int64,const void*,::dword)")
REF("::BlockStream::SetStreamSize(::int64)")
REF("::BlockStream::GetMediaSize()const")
REF("::BlockStream::OpenInit(::dword,::int64)")
REF("::BlockStream::SetBufferSize(::dword)")
REF("::BlockStream::GetBufferSize()const")
REF("::FileStream::class")
REF("::FileStream::FileStream(const char*,::dword)")
REF("::FileStream::FileStream(const char*,::dword,mode_t)")
REF("::FileStream::FileStream(int)")
REF("::FileStream::FileStream()")
REF("::FileStream::operator bool()const")
REF("::FileStream::GetTime()const")
REF("::FileStream::SetTime(const::FileTime&)")
REF("::FileStream::Open(const char*,::dword,mode_t)")
REF("::FileStream::GetHandle()const")
REF("::FileIn::class")
REF("::FileIn::FileIn(const char*)")
REF("::FileIn::FileIn()")
REF("::FileIn::Open(const char*)")
REF("::FileOut::class")
REF("::FileOut::FileOut(const char*)")
REF("::FileOut::FileOut()")
REF("::FileOut::Open(const char*,mode_t)")
REF("::FileAppend::class")
REF("::FileAppend::FileAppend(const char*)")
REF("::FileAppend::FileAppend()")
REF("::FileAppend::Open(const char*)")
REF("::SizeStream::class")
REF("::SizeStream::SizeStream()")
REF("::SizeStream::_Put(int)")
REF("::SizeStream::Open()")
REF("::CompareStream::class")
REF("::CompareStream::CompareStream()")
REF("::CompareStream::CompareStream(::Stream&)")
REF("::CompareStream::_Put(int)")
REF("::CompareStream::IsEqual()")
REF("::CompareStream::operator bool()")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2; Streams&][ {{993:993:993:993:995:996:987:1013:1016:1021f0;g33;GNl17/25r17/4t17/0b17/0@7-2 "
"[s0;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:Stream`:`:class Stream^1 Stream]::l0/25r0/25t0/15b0/15@2-1 "
"[s0; ]:: [s0; ]::r/4t/0b/0 [s0;1 ]:: [s0;1 ]:: [s0;1")
TOPIC_TEXT(
" ]:: [s0;1 ]:: [s0;1 ]:: [s5;1 ]:: [s5;1 ]:: [s0;1 ]::l17/25r17/4t17/0b17/0@7-2 "
"[s0;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:StringStream`:`:class StringStream^1 "
"StringStream]::l0/25r0/25t0/15b0/15@2-1 [s0; ]:: [s0; ]::r/4t/0b/0 [s0;1 ]:: [s0;1 "
"]:: [s0;1 ]:: [s0;1 ]:: [s5;1 ]:: [s5;1 ]:: [s0;1 ]::l17/25r17/4t17/0b17/0@7-2 [s0;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:MemStream`:`:class "
"MemStrea")
TOPIC_TEXT(
"m^1 MemStream]::l0/25r0/25t0/15b0/15@2-1 [s0; ]:: [s0; ]::r/4t/0b/0 [s0;1 ]:: [s0;1 "
"]:: [s0;1 ]:: [s0;1 ]:: [s5;1 ]:: [s5;1 ]:: [s0;1 ]:: [s0;1 ]::l17/25r17/4t17/0b17/0@7-2 "
"[s0;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:MemReadStream`:`:class MemReadStream^1 "
"MemReadStream]::l0/25r0/25t0/15b0/15@2-1 [s0; ]:: [s0; ]::r/4t/0b/0 [s0;1 ]:: [s0;1 "
"]:: [s0;1 ]:: [s5;1 ]:: [s5;1 ]:: [s0;1 ]::l17/25r17/4t17")
TOPIC_TEXT(
"/0b17/0@7-2 [s0;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:BufferStream`:`:class "
"BufferStream^1 BufferStream]::l0/25r0/25t0/15b0/15@2-1 [s0; ]:: [s0; ]::r/4t/0b/0 "
"[s0;1 ]:: [s0;1 ]:: [s0;1 ]:: [s0;1 ]:: [s5;1 ]:: [s5;1 ]:: [s0;1 ]:: [s0;1 ]::l17/25r17/4t17/0b17/0@7-2 "
"[s0;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:BlockStream`:`:class BlockStream^1 "
"BlockStream]::l0/25r0/25t0/15b0/15@2-1 [s0; ]:: [s")
TOPIC_TEXT(
"0; ]::r/4t/0b/0 [s0;1 ]:: [s0;1 ]:: [s0;1 ]:: [s5;1 ]:: [s5;1 ]:: [s0;1 ]:: [s0;1 "
"]:: [s0;1 ]::l17/25r17/4t17/0b17/0@7-2 [s0;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:FileStream`:`:class "
"FileStream^1 FileStream]::l0/25r0/25t0/15b0/15@2-1 [s0; ]:: [s0; ]::r/4t/0b/0 [s0;1 "
"]:: [s0;1 ]:: [s5;1 ]:: [s5;1 ]:: [s0;1 ]:: [s0;1 ]:: [s0;1 ]:: [s0;1 ]::l17/25r17/4t17/0b17/0@7-2 "
"[s0;^topic`:`/`/Core`/src`/St")
TOPIC_TEXT(
"ream`$en`-us`#`:`:FileIn`:`:class FileIn^1 FileIn]::l0/25r0/25t0/15b0/15@2-1 [s0; "
"]:: [s0; ]::r/4t/0b/0 [s0;1 ]:: [s5;1 ]:: [s5;1 ]:: [s0;1 ]:: [s0;1 ]:: [s0;1 ]:: "
"[s0;1 ]::l17/25r17/4t17/0b17/0@7-2 [s0;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:FileOut`:`:class "
"FileOut^1 FileOut]::l0/25r0/25t0/15b0/15@2-1 [s0; ]:: [s0; ]::r/4t/0b/0 [s0;1 ]:: "
"[s5;1 ]:: [s5;1 ]:: [s0;1 ]:: [s0;1 ]:: [s0;1 ]:: [s0;1")
TOPIC_TEXT(
" ]::l17/25r17/4t17/0b17/0@7-2 [s0;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:FileAppend`:`:class "
"FileAppend^1 FileAppend]::l0/25r0/25t0/15b0/15@2-1 [s0; ]:: [s0; ]::r/4t/0b/0 [s0;1 "
"]:: [s5;1 ]:: [s5;1 ]:: [s0;1 ]:: [s0;1 ]::l17/25r17/4t17/0b17/0@7-2 [s0;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:SizeStream`:`:class "
"SizeStream^1 SizeStream]::l0/25r0/25t0/15b0/15@2-1 [s0; ]:: [s0; ]::r/4t/0b/0 [s0;1")
TOPIC_TEXT(
" ]:: [s0;1 ]:: [s0;1 ]:: [s5;1 ]:: [s5;1 ]:: [s0;1 ]:: [s0;1 ]::l17/25r17/4t17/0b17/0@7-2 "
"[s0;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:CompareStream`:`:class CompareStream^1 "
"CompareStream]::l0/25r0/25t0/15b0/15@2-1 [s0; ]:: [s0; ]::r/4t/0b/0 [s0;1 ]:: [s0;1 "
"]:: [s0;1 ]:: [s5;1 ]:: [s5;1 ]}}&][s5; &][s0;3 &][s10;:`:`:Stream`:`:class:* [%00-00* "
"class_][%00-00 Stream]&][s6; &][s6; Stream is the bas")
TOPIC_TEXT(
"e class of all U`+`+ streams.&][s6; U`+`+ streams generally serve in three different "
"roles:&][s6;i150;O0; As basic raw binary streams.&][s6;i150;O0; As text output with "
"enhanced indentation feature. This is realized by storing indentation level in the "
"stream and inserting the corresponding number of `'`\\t`' characters at the beginning "
"of each line (Putf method must be used instead of Put).&][s6;i1")
TOPIC_TEXT(
"50;O0; As binary serialization target/source. This mainly needs to store flag indicating "
"serialization direction.&][s6; In order to achieve optimal performance of buffered "
"stream operations, the most frequent operations have a little bit more complicated "
"implementation that demands proper definition of virtual methods, as they must correctly "
"adjust some protected data members of Stream. Therefore ")
TOPIC_TEXT(
"the implementation of some method can be considered a part of interface definition:&][s7; "
"void  Stream`::Put(int c)&][s7; `{&][s7; -|if(ptr < wrlim)&][s7; -|-|`*ptr`+`+ `= "
"c;&][s7; -|else&][s7; -|-|`_Put(c);&][s7; `}&][s7; &][s7; int  Stream`::Term()&][s7; "
"`{&][s7; -|return ptr < rdlim ? `*ptr : `_Term();&][s7; `}&][s7; &][s7; int  Stream`::Get()&][s7; "
"`{&][s7; -|return ptr < rdlim ? `*ptr`+`+ : `")
TOPIC_TEXT(
"_Get();&][s7; `}&][s7; &][s7; int64 Stream`::GetPos() const&][s7; `{&][s7; -|return "
"dword(ptr `- buffer) `+ pos;&][s7; `}&][s7; &][s0;3 &][s3; [%00-00 Protected members "
"for Stream implementation]&][s4; &][s4;:`:`:Stream`:`:pos:* [%00-00* int64_][%00-00 "
"pos]&][s11; [%00-00 Protected.]&][s6; Position of buffer in the stream.&][s0;3 &][s4;:`:`:Stream`:`:buffer:* "
"[%00-00* byte_`*][%00-00 buffer]&][s11")
TOPIC_TEXT(
"; Protected.&][s6; Pointer to beginning of buffer.&][s0;3 &][s4;:`:`:Stream`:`:ptr:* "
"[%00-00* byte_`*][%00-00 ptr]&][s11; Protected.&][s6; Pointer to current input/output "
"byte.&][s0;3 &][s4;:`:`:Stream`:`:rdlim:* [%00-00* byte_`*][%00-00 rdlim]&][s11; "
"Protected.&][s6; Read limit. [* Get] method returns values from buffer as long as "
"[* ptr < rdlim].&][s0;3 &][s4;:`:`:Stream`:`:wrlim:* [%00-00* byte")
TOPIC_TEXT(
"_`*][%00-00 wrlim]&][s11; Protected.&][s6; Write limit. [* Put] method returns values "
"from buffer as long as [* ptr < wrlim].&][s0;3 &][s4;:`:`:Stream`:`:`_Put`(int`): "
"[%00-00 virtual void_][%00-00* `_Put][%00-00 (int_][%00-00*@3 w][%00-00 )]&][s6; "
"Called by [* Put] method in case that output byte cannot be stored into buffer ([* "
"ptr >`= wrlim]). If Stream wants to use buffering, this method shoul")
TOPIC_TEXT(
"d adjust [* buffer], [* ptr] and [* wrlim].&][s1; [%00-00*C@3 w]-|Byte to store.&][s0;3 "
"&][s4;:`:`:Stream`:`:`_Term`(`): [%00-00 virtual int_][%00-00* `_Term][%00-00 ()]&][s6; "
"Called by [* Term] method in case that input byte is not in buffer ([* ptr >`= rdlim]). "
"If Stream wants to use buffering, this method should adjust [* buffer], [* ptr] and "
"[* rdlim].&][s1; [*/ Return value]-|Value at current")
TOPIC_TEXT(
" position in the stream. Current position is not advanced.&][s0;3 &][s4;:`:`:Stream`:`:`_Get`(`): "
"[%00-00 virtual int_][%00-00* `_Get][%00-00 ()]&][s6; Called by [* Get] method in "
"case that input byte is not in buffer ([* ptr >`= rdlim]). If Stream wants to use "
"buffering, this method should adjust [* buffer], [* ptr] and [* rdlim].&][s1; [*/ "
"Return value]-|Value read from the stream.&][s0;3 &][s4;")
TOPIC_TEXT(
":`:`:Stream`:`:`_Put`(const void`*`,`:`:dword`): [%00-00 virtual void_][%00-00* "
"`_Put][%00-00 (const_void_`*][%00-00*@3 data][%00-00 , dword_][%00-00*@3 size][%00-00 "
")]&][s6; Directly called by [* Put] method. Writes a block of binary data.&][s1; "
"[%00-00*C@3 data]-|Pointer to data.&][s1; [%00-00*C@3 size]-|Size.&][s0;3 &][s4;:`:`:Stream`:`:`_Get`(void`*`,`:`:dword`): "
"[%00-00 virtual dword_][%00-00")
TOPIC_TEXT(
"* `_Get][%00-00 (void_`*][%00-00*@3 data][%00-00 , dword_][%00-00*@3 size][%00-00 "
")]&][s6; Directly called by [* Get] method. &][s1; [%00-00*C@3 data]-|&][s1; [%00-00*C@3 "
"size]-|&][s1; [*/ Return value]-|&][s6; &][s3; Raw stream&][s0;3 &][s4;:`:`:Stream`:`:Seek`(`:`:int64`): "
"[%00-00 virtual void_][%00-00* Seek][%00-00 (int64_][%00-00*@3 pos][%00-00 )]&][s6; "
"Seeks to given position.&][s1; [%00-00*C")
TOPIC_TEXT(
"@3 pos]-|Position.&][s0;3 &][s4;:`:`:Stream`:`:GetSize`(`)const: [%00-00 virtual "
"int64_][%00-00* GetSize][%00-00 ()_const]&][s1; [*/ Return value]-|Size of stream.&][s0;3 "
"&][s4;:`:`:Stream`:`:SetSize`(`:`:int64`): [%00-00 virtual void_][%00-00* SetSize][%00-00 "
"(int64_][%00-00*@3 size][%00-00 )]&][s6; Alters the size of the stream.&][s1; [%00-00*C@3 "
"size]-|New size.&][s0;3 &][s4;:`:`:Stream`:`:Flus")
TOPIC_TEXT(
"h`(`): [%00-00 virtual void_][%00-00* Flush][%00-00 ()]&][s6; If stream has any "
"internal buffers (like FileStream), writes these bufers to OS.&][s0;3 &][s4;:`:`:Stream`:`:Close`(`): "
"[%00-00 virtual void_][%00-00* Close][%00-00 ()]&][s6; Closes stream.&][s0;3 &][s4;:`:`:Stream`:`:IsOpen`(`)const:@3 "
"[%00-00@0 virtual bool_][%00-00*@0 IsOpen][%00-00@0 ()_const_`=_][%00-00 0]&][s1; "
"[*/ Return value]-|")
TOPIC_TEXT(
"true if stream is open.&][s0;3 &][s4;:`:`:Stream`:`:IsError`(`)const: [%00-00 bool_][%00-00* "
"IsError][%00-00 ()_const]&][s1; [*/ Return value]-|true if error was encountered "
"during stream operations since opening it or last ClearError call `- error code is "
"non`-zero.&][s0;3 &][s4;:`:`:Stream`:`:IsOK`(`)const: [%00-00 bool_][%00-00* IsOK][%00-00 "
"()_const]&][s1; [*/ Return value]-|!IsError().&][s0;3")
TOPIC_TEXT(
" &][s4;:`:`:Stream`:`:SetError`(int`): [%00-00 void_][%00-00* SetError][%00-00 (int_][%00-00*@3 "
"c][%00-00 _`=_][%00-00@3 0][%00-00 )]&][s6; Sets stream error code.&][s1; [%00-00*C@3 "
"c]-|Error code.&][s0;3 &][s4;:`:`:Stream`:`:SetLastError`(`): [%00-00 void_][%00-00* "
"SetLastError][%00-00 ()]&][s6; Sets stream error to last OS`-specific error (obtained "
"e.g. by GetLastError call in Win32 or in errno ")
TOPIC_TEXT(
"in Posix). This error can be interpreted by GetErrorMessage function.&][s0;3 &][s4;:`:`:Stream`:`:GetError`(`)const: "
"[%00-00 int_][%00-00* GetError][%00-00 ()_const]&][s1; [*/ Return value]-|Current "
"error`-code. Zero indicates no error.&][s0;3 &][s4;:`:`:Stream`:`:ClearError`(`): "
"[%00-00 void_][%00-00* ClearError][%00-00 ()]&][s6; Clears error code.&][s0;3 &][s4;:`:`:Stream`:`:GetPos`(`)const: "
"[%0")
TOPIC_TEXT(
"0-00 int64_][%00-00* GetPos][%00-00 ()_const]&][s1; [*/ Return value]-|Current position "
"in the stream.&][s0;3 &][s4;:`:`:Stream`:`:IsEof`(`)const: [%00-00 bool_][%00-00* "
"IsEof][%00-00 ()_const]&][s1; [*/ Return value]-|There are no more byte to be read "
"from the stream. Is also true in case of error.&][s0;3 &][s4;:`:`:Stream`:`:GetLeft`(`)const: "
"[%00-00 int64_][%00-00* GetLeft][%00-00 ()_const]&][s")
TOPIC_TEXT(
"1; [*/ Return value]-|Bytes between current position and the end of stream `- equivalent "
"to GetSize() `- GetPos().&][s0;3 &][s4;:`:`:Stream`:`:SeekEnd`(`:`:int64`): [%00-00 "
"void_][%00-00* SeekEnd][%00-00 (int64_][%00-00*@3 rel][%00-00 _`=_][%00-00@3 0][%00-00 "
")]&][s6; Sets current position in the stream relative to the end of stream. Same "
"as Seek(GetSize() `+ rel).&][s1; [%00-00*C@3 rel]-|Position")
TOPIC_TEXT(
" `- should be less or equal to zero.&][s0;3 &][s4;:`:`:Stream`:`:SeekCur`(`:`:int64`): "
"[%00-00 void_][%00-00* SeekCur][%00-00 (int64_][%00-00*@3 rel][%00-00 )]&][s6; Sets "
"current position in the stream relative to the current position. Same as Seek(GetPos() "
"`+ rel).&][s1; [%00-00*C@3 rel]-|Relative offset.&][s0;3 &][s4;:`:`:Stream`:`:Put`(int`): "
"[%00-00 void_][%00-00* Put][%00-00 (int_][%00-00*@3 ")
TOPIC_TEXT(
"c][%00-00 )]&][s6; Puts single byte into the output stream.&][s1; [%00-00*C@3 c]-|Byte.&][s0;3 "
"&][s4;:`:`:Stream`:`:Term`(`): [%00-00 int_][%00-00* Term][%00-00 ()]&][s6; Peeks "
"byte from input stream not advancing current position. If there are no more bytes "
"in input stream or error occurred, negative value is returned.&][s1; [*/ Return value]-|Byte "
"at current position in the stream.&][s0;3 &][s4;")
TOPIC_TEXT(
":`:`:Stream`:`:Get`(`): [%00-00 int_][%00-00* Get][%00-00 ()]&][s6; Reads single "
"byte from input stream, advancing current position. If there are no more bytes in "
"input stream or error occurred, negative value is returned.&][s1; [*/ Return value]-|Byte "
"read from input stream.&][s0;3 &][s4;:`:`:Stream`:`:Put`(const void`*`,`:`:dword`): "
"[%00-00 void_][%00-00* Put][%00-00 (const_void_`*][%00-00*@3 da")
TOPIC_TEXT(
"ta][%00-00 , dword_][%00-00*@3 size][%00-00 )]&][s6; Writes a block of raw binary "
"data to the output stream.&][s1; [%00-00*C@3 data]-|Pointer to data.&][s1; [%00-00*C@3 "
"size]-|Number of bytes to write.&][s0;3 &][s4;:`:`:Stream`:`:Get`(void`*`,`:`:dword`): "
"[%00-00 dword_][%00-00* Get][%00-00 (void_`*][%00-00*@3 data][%00-00 , dword_][%00-00*@3 "
"size][%00-00 )]&][s6; Reads a block of raw binary data ")
TOPIC_TEXT(
"from the input stream.&][s1; [%00-00*C@3 data]-|Pointer to buffer to receive the "
"data.&][s1; [%00-00*C@3 size]-|Number of bytes to read.&][s1; [*/ Return value]-|Number "
"of bytes actually read (lower or equal to the requested [*@3 size]).&][s0;3 &][s4;:`:`:Stream`:`:LoadThrowing`(`): "
"void_[* LoadThrowing]()&][s6; Sets stream into the mode that throws LoadingError "
"exception when LoadError is invoked")
TOPIC_TEXT(
". This mode is typical for serialization usage of stream.&][s0;3 &][s4;:`:`:Stream`:`:LoadError`(`): "
"void_[* LoadError]()&][s6; Performs SetError(ERROR`_LOADING`_FAILED). If Stream set "
"to the LoadThrowing mode (by LoadThrowing() method), LoadingError exception is thrown.&][s0;3 "
"&][s4;:`:`:Stream`:`:GetAll`(void`*`,`:`:dword`): [%00-00 bool_][%00-00* GetAll][%00-00 "
"(void_`*][%00-00*@3 data][%00-00 ")
TOPIC_TEXT(
", dword_][%00-00*@3 size][%00-00 )]&][s6; Reads a block of raw binary data from "
"the stream. If there is not enough data in the stream, LoadError is invoked (that "
"in turn might throw an exception).&][s1; [%00-00*C@3 data]-|Pointer to buffer to "
"receive the data.&][s1; [%00-00*C@3 size]-|Number of bytes to read.&][s1; [*/ Return "
"value]-|true if required number of bytes was read.&][s0;3 &][s4;:`:`:Str")
TOPIC_TEXT(
"eam`:`:Get8`(`): [%00-00 int_][%00-00* Get8][%00-00 ()]&][s6; Reads a single byte "
"from the stream. If there is not enough data in the stream, LoadError is invoked "
"(that in turn might throw an exception) `- this is the difference from Get() method.&][s1; "
"[*/ Return value]-|Byte from stream or `-1.&][s0;3 &][s4;:`:`:Stream`:`:Get16`(`): "
"[%00-00 int_][%00-00* Get16][%00-00 ()]&][s6; Reads 16`-bit val")
TOPIC_TEXT(
"ue from the stream in platform specific format (either little`-endian or big`-endian). "
"If there is not enough data in the stream, LoadError is invoked (that in turn might "
"throw an exception).&][s1; [*/ Return value]-|16`-bit value.&][s0;3 &][s4;:`:`:Stream`:`:Get32`(`): "
"[%00-00 int_][%00-00* Get32][%00-00 ()]&][s6; Reads 32`-bit value from the stream "
"in platform specific format (either little`-end")
TOPIC_TEXT(
"ian or big`-endian). If there is not enough data in the stream, LoadError is invoked "
"(that in turn might throw an exception).&][s1; [*/ Return value]-|32`-bit value.&][s0;3 "
"&][s4;:`:`:Stream`:`:Get64`(`): [%00-00 int64_][%00-00* Get64][%00-00 ()]&][s6; Reads "
"64`-bit value from the stream in platform specific format (either little`-endian "
"or big`-endian). If there is not enough data in the stream, ")
TOPIC_TEXT(
"LoadError is invoked (that in turn might throw an exception).&][s1; [*/ Return value]-|64`-bit "
"value.&][s0;3 &][s4;:`:`:Stream`:`:Get16le`(`): [%00-00 int_][%00-00* Get16le][%00-00 "
"()]&][s6; Reads 16`-bit value from the stream in the little`-endian mode. If there "
"is not enough data in the stream, LoadError is invoked (that in turn might throw "
"an exception).&][s1; [*/ Return value]-|16`-bit value.&")
TOPIC_TEXT(
"][s0;3 &][s4;:`:`:Stream`:`:Get32le`(`): [%00-00 int_][%00-00* Get32le][%00-00 ()]&][s6; "
"Reads 32`-bit value from the stream in the little`-endian mode. If there is not enough "
"data in the stream, LoadError is invoked (that in turn might throw an exception).&][s1; "
"[*/ Return value]-|32`-bit value.&][s0;3 &][s4;:`:`:Stream`:`:Get64le`(`): [%00-00 "
"int64_][%00-00* Get64le][%00-00 ()]&][s6; Reads 64`-b")
TOPIC_TEXT(
"it value from the stream in the little`-endian mode. If there is not enough data "
"in the stream, LoadError is invoked (that in turn might throw an exception).&][s1; "
"[*/ Return value]-|64`-bit value.&][s0;3 &][s4;:`:`:Stream`:`:Get16be`(`): [%00-00 "
"int_][%00-00* Get16be][%00-00 ()]&][s6; Reads 16`-bit value from the stream in the "
"big`-endian mode. If there is not enough data in the stream, LoadError")
TOPIC_TEXT(
" is invoked (that in turn might throw an exception).&][s1; [*/ Return value]-|16`-bit "
"value.&][s0;3 &][s4;:`:`:Stream`:`:Get32be`(`): [%00-00 int_][%00-00* Get32be][%00-00 "
"()]&][s6; Reads 32`-bit value from the stream in the big`-endian mode. If there is "
"not enough data in the stream, LoadError is invoked (that in turn might throw an "
"exception).&][s1; [*/ Return value]-|32`-bit value.&][s0;3 &][s4")
TOPIC_TEXT(
";:`:`:Stream`:`:Get64be`(`): [%00-00 int64_][%00-00* Get64be][%00-00 ()]&][s6; Reads "
"32`-bit value from the stream in the big`-endian mode. If there is not enough data "
"in the stream, LoadError is invoked (that in turn might throw an exception).&][s1; "
"[*/ Return value]-|64`-bit value.&][s0;3 &][s4;:`:`:Stream`:`:GetLine`(`): [%00-00 "
"String_][%00-00* GetLine][%00-00 ()]&][s6; Reads single line from ")
TOPIC_TEXT(
"the stream. Line is delimited by `'`\\n`' character or the end of file (or error). "
"`'`\\r`' characters are ignored.&][s1; [*/ Return value]-|Line from the stream.&][s0;3 "
"&][s4;:`:`:Stream`:`:GetUtf8`(`): int_[* GetUtf8]()&][s6; Reads single utf`-8 encoded "
"value from the stream.&][s1; [*/ Return value]-|Utf`-8 value.&][s0;3 &][s4;:`:`:Stream`:`:Put16`(word`): "
"[%00-00 void_][%00-00* Put16][%00-00 (wor")
TOPIC_TEXT(
"d_][%00-00*@3 q][%00-00 )]&][s6; Writes 16`-bit value in platform specific format "
"(either little`-endian or big`-endian).&][s1; [%00-00*C@3 q]-|Value to write.&][s0;3 "
"&][s4;:`:`:Stream`:`:Put32`(int`): [%00-00 void_][%00-00* Put32][%00-00 (int_][%00-00*@3 "
"q][%00-00 )]&][s6; Writes 32`-bit value in platform specific format (either little`-endian "
"or big`-endian).&][s1; [%00-00*C@3 q]-|Value to write")
TOPIC_TEXT(
".&][s0;3 &][s4;:`:`:Stream`:`:Put64`(`:`:int64`): [%00-00 void_][%00-00* Put64][%00-00 "
"(int64_][%00-00*@3 q][%00-00 )]&][s6; Writes 64`-bit value in platform specific format "
"(either little`-endian or big`-endian).&][s1; [%00-00*C@3 q]-|Value to write.&][s0;3 "
"&][s4;:`:`:Stream`:`:Put16le`(word`): [%00-00 void_][%00-00* Put16le][%00-00 (word_][%00-00*@3 "
"q][%00-00 )]&][s6; Writes 16`-bit value in lit")
TOPIC_TEXT(
"tle`-endian mode.&][s1; [%00-00*C@3 q]-|Value to write.&][s0;3 &][s4;:`:`:Stream`:`:Put32le`(int`): "
"[%00-00 void_][%00-00* Put32le][%00-00 (int_][%00-00*@3 q][%00-00 )]&][s6; Writes "
"32`-bit value in little`-endian mode.&][s1; [%00-00*C@3 q]-|Value to write.&][s0;3 "
"&][s4;:`:`:Stream`:`:Put64le`(`:`:int64`): [%00-00 void_][%00-00* Put64le][%00-00 "
"(int64_][%00-00*@3 q][%00-00 )]&][s6; Writes 64`-bit ")
TOPIC_TEXT(
"value in little`-endian mode.&][s1; [%00-00*C@3 q]-|Value to write.&][s0;3 &][s4;:`:`:Stream`:`:Put16be`(word`): "
"[%00-00 void_][%00-00* Put16be][%00-00 (word_][%00-00*@3 q][%00-00 )]&][s6; Writes "
"16`-bit value in big`-endian mode.&][s1; [%00-00*C@3 q]-|Value to write.&][s0;3 &][s4;:`:`:Stream`:`:Put32be`(int`): "
"[%00-00 void_][%00-00* Put32be][%00-00 (int_][%00-00*@3 q][%00-00 )]&][s6; Writes "
"32`-b")
TOPIC_TEXT(
"it value in big`-endian mode.&][s1; [%00-00*C@3 q]-|Value to write.&][s0;3 &][s4;:`:`:Stream`:`:Put64be`(`:`:int64`): "
"[%00-00 void_][%00-00* Put64be][%00-00 (int64_][%00-00*@3 q][%00-00 )]&][s6; Writes "
"64`-bit value in big`-endian mode.&][s1; [%00-00*C@3 q]-|Value to write.&][s0;3 &][s4;:`:`:Stream`:`:PutUtf8`(int`): "
"[%00-00 void_][%00-00* PutUtf8][%00-00 (int_][%00-00*@3 c][%00-00 )]&][s6; Puts a")
TOPIC_TEXT(
" single utf`-8 value to the stream.&][s1; [%00-00*C@3 c]-|Value.&][s0; &][s0;3 &][s4;:`:`:Stream`:`:Put`(const "
"char`*`): [%00-00 void_][%00-00* Put][%00-00 (const_char_`*][%00-00*@3 s][%00-00 "
")]&][s6; Writes zero terminated string to the stream (zero is not written).&][s1; "
"[%00-00*C@3 s]-|String to write.&][s0;3 &][s4;:`:`:Stream`:`:Put`(const`:`:String`&`): "
"[%00-00 void_][%00-00* Put][%00-00 (con")
TOPIC_TEXT(
"st_String`&_][%00-00*@3 s][%00-00 )]&][s6; Writes String to the the stream. String "
"can contain zero characters.&][s1; [%00-00*C@3 s]-|String to write.&][s0;3 &][s4;:`:`:Stream`:`:Put`(int`,int`): "
"[%00-00 void_][%00-00* Put][%00-00 (int_][%00-00*@3 c][%00-00 , int_][%00-00*@3 count][%00-00 "
")]&][s6; Writes single byte to the stream requested number of times.&][s1; [%00-00*C@3 "
"c]-|Byte to write.&][s1")
TOPIC_TEXT(
"; [%00-00*C@3 count]-|Repeat count.&][s0;3 &][s4;:`:`:Stream`:`:Put0`(int`): [%00-00 "
"void_][%00-00* Put0][%00-00 (int_][%00-00*@3 count][%00-00 )]&][s6; Writes zero byte "
"to the stream requested number of times. This variation is mainly provided because "
"Put(0, count) causes ambiguity as 0 is considered as NULL pointer too...&][s1; [%00-00*C@3 "
"count]-|Repeat count.&][s0; &][s0;3 &][s4;:`:`:Stream`:`")
TOPIC_TEXT(
":PutW`(const`:`:wchar`*`,int`): [%00-00 void_][%00-00* PutW][%00-00 (const_wchar_`*][%00-00*@3 "
"s][%00-00 , int_][%00-00*@3 count][%00-00 )]&][s6; Writes a specified number of wchars "
"(16`-bit character values) to the stream in platform specific format (little`-endian "
"or big`-endian).&][s1; [%00-00*C@3 s]-|String to write.&][s1; [%00-00*C@3 count]-|Length "
"of string.&][s0;3 &][s4;:`:`:Stream`:`:GetW`")
TOPIC_TEXT(
"(`:`:wchar`*`,int`): [%00-00 dword_][%00-00* GetW][%00-00 (wchar_`*][%00-00*@3 s][%00-00 "
", int_][%00-00*@3 count][%00-00 )]&][s6; Reads a specified number of wchars (16`-bit "
"character values) from the stream in platform specific format.&][s1; [%00-00*C@3 "
"s]-|Pointer to buffer to receive wchars.&][s1; [%00-00*C@3 count]-|Number of wchars.&][s1; "
"[*/ Return value]-|Number of wchars actually read.&][s")
TOPIC_TEXT(
"0;3 &][s4;:`:`:Stream`:`:GetAllW`(`:`:wchar`*`,int`): [%00-00 bool_][%00-00* GetAllW][%00-00 "
"(wchar_`*][%00-00*@3 s][%00-00 , int_][%00-00*@3 count][%00-00 )]&][s6; Reads a specified "
"number of wchars (16`-bit character values) from the stream in platform specific "
"format. If there is not enough data in the stream, LoadError is invoked (that in "
"turn might throw an exception).&][s1; [%00-00*C@3 s]-|&")
TOPIC_TEXT(
"][s1; [%00-00*C@3 count]-|&][s1; [*/ Return value]-|&][s0;3 &][s4;:`:`:Stream`:`:PutCrLf`(`): "
"[%00-00 void_][%00-00* PutCrLf][%00-00 ()]&][s6; Writes CR`-LF pair to the stream. "
"&][s0;3 &][s4;:`:`:Stream`:`:PutEol`(`): [%00-00 void_][%00-00* PutEol][%00-00 ()]&][s6; "
"Writes platform specific `\"end of line`\" to the stream. It is CR`-LF pair on Win32 "
"platform or single LF on Posix platform.&][s0;3 &][")
TOPIC_TEXT(
"s4;:`:`:Stream`:`:PutLine`(const char`*`): [%00-00 void_][%00-00* PutLine][%00-00 "
"(const_char_`*][%00-00*@3 s][%00-00 )]&][s6; Writes a line to stream `- equivalent "
"of Put(s); PutEol();&][s1; [%00-00*C@3 s]-|Zero`-terminated string.&][s0;3 &][s4;:`:`:Stream`:`:PutLine`(const`:`:String`&`): "
"[%00-00 void_][%00-00* PutLine][%00-00 (const_String`&_][%00-00*@3 s][%00-00 )]&][s6; "
"Writes a line to stream")
TOPIC_TEXT(
" `- equivalent of Put(s); PutEol();&][s1; [%00-00*C@3 s]-|String.&][s0;3 &][s4;:`:`:Stream`:`:Put`(`:`:Stream`&`,`:`:int64`,`:`:dword`): "
"[%00-00 void_][%00-00* Put][%00-00 (Stream`&_][%00-00*@3 s][%00-00 , int64_][%00-00*@3 "
"size][%00-00 _`=_INT64`_MAX, dword_][%00-00*@3 click][%00-00 _`=_][%00-00@3 4096][%00-00 "
")]&][s6; Writes a content of specified stream to the stream.&][s1; [%00-00*C@3 s]-|Inpu")
TOPIC_TEXT(
"t stream.&][s1; [%00-00*C@3 size]-|Maximum number of bytes to write. Default value "
"INT64`_MAX means whole input stream from current position to the end is written.&][s1; "
"[%00-00*C@3 click]-|Size of buffer used for copying.&][s3; Formated output&][s0;3 "
"&][s4;:`:`:Stream`:`:Putf`(int`): [%00-00 void_][%00-00* Putf][%00-00 (int_][%00-00*@3 "
"c][%00-00 )]&][s6; Formated output `- each start of line (tha")
TOPIC_TEXT(
"t is, byte passed by Putf after `'`\\n`') is indented using `'`\\t`' characters, "
"as controlled by Begin/End methods. Also, `'`\\n`' are translated by PutEol and `'`\\r`' "
"are ignored.&][s1; [%00-00*C@3 c]-|Byte to write.&][s0;3 &][s4;:`:`:Stream`:`:Putf`(const "
"char`*`): [%00-00 void_][%00-00* Putf][%00-00 (const_char_`*][%00-00*@3 s][%00-00 "
")]&][s6; Outputs zero terminated string using Putf method.&][s")
TOPIC_TEXT(
"1; [%00-00*@3 s]-|String to write.&][s0;3 &][s4;:`:`:Stream`:`:Putf`(const`:`:String`&`): "
"[%00-00 void_][%00-00* Putf][%00-00 (const_String`&_][%00-00*@3 s][%00-00 )]&][s6; "
"Outputs String using Putf.&][s1; [%00-00*C@3 s]-|String.&][s0;3 &][s4;:`:`:Stream`:`:Begin`(`): "
"[%00-00 void_][%00-00* Begin][%00-00 ()]&][s6; Increases Putf line indentation level "
"by 1.&][s0;3 &][s4;:`:`:Stream`:`:End`(`): [%0")
TOPIC_TEXT(
"0-00 void_][%00-00* End][%00-00 ()]&][s6; Decreases Putf line indentation level "
"by 1.&][s0;3 &][s4;:`:`:Stream`:`:operator`<`<`(BeginIndentEnum`): [%00-00 Stream`&_][%00-00* "
"operator<<][%00-00 (BeginIndentEnum)]&][s6; Calls Begin() `- BeginIndentEnum has "
"single value `- `\"BeginIndent`\".&][s1; [*/ Return value]-|Stream for operator<< "
"chaining.&][s0;3 &][s4;:`:`:Stream`:`:operator`<`<`(EndIndentEnum")
TOPIC_TEXT(
"`): [%00-00 Stream`&_][%00-00* operator<<][%00-00 (EndIndentEnum)]&][s6; Calls End() "
"`- EndIndentEnum has single value `- `\"EndIndent`\".&][s1; [*/ Return value]-|Stream "
"for operator<< chaining.&][s0; &][s3; Serialization support&][s0;3 &][s4;:`:`:Stream`:`:SetLoading`(`): "
"[%00-00 void_][%00-00* SetLoading][%00-00 ()]&][s6; Sets the stream into the loading "
"mode.&][s0;3 &][s4;:`:`:Stream`:`:SetStori")
TOPIC_TEXT(
"ng`(`): [%00-00 void_][%00-00* SetStoring][%00-00 ()]&][s6; Sets the stream into "
"the storing mode.&][s0;3 &][s4;:`:`:Stream`:`:IsLoading`(`): [%00-00 bool_][%00-00* "
"IsLoading][%00-00 ()]&][s1; [*/ Return value]-|true if stream is in loading mode.&][s0;3 "
"&][s4;:`:`:Stream`:`:IsStoring`(`): [%00-00 bool_][%00-00* IsStoring][%00-00 ()]&][s1; "
"[*/ Return value]-|true if stream is in storing mode.&][s0;")
TOPIC_TEXT(
"3 &][s4;:`:`:Stream`:`:SerializeRaw`(`:`:byte`*`,`:`:dword`): [%00-00 void_][%00-00* "
"SerializeRaw][%00-00 (byte_`*][%00-00*@3 data][%00-00 , dword_][%00-00*@3 count][%00-00 "
")]&][s6; Serializes raw 8`-bit data. Might invoke LoadError if there is not enough "
"data to load.&][s1; [%00-00*C@3 data]-|Pointer to data to store or buffer to receive "
"loaded data.&][s1; [%00-00*C@3 count]-|Number of bytes to l")
TOPIC_TEXT(
"oad/store.&][s0;3 &][s4;:`:`:Stream`:`:SerializeRaw`(word`*`,`:`:dword`): [%00-00 "
"void_][%00-00* SerializeRaw][%00-00 (word_`*][%00-00*@3 data][%00-00 , dword_][%00-00*@3 "
"count][%00-00 )]&][s6; Serializes raw 16`-bit data. Might invoke LoadError if there "
"is not enough data to load.&][s1; [%00-00*C@3 data]-|Pointer to data to store or "
"buffer to receive loaded data.&][s1; [%00-00*C@3 count]-|Number ")
TOPIC_TEXT(
"of values to load/store.&][s0;3 &][s4;:`:`:Stream`:`:SerializeRaw`(`:`:dword`*`,`:`:dword`): "
"[%00-00 void_][%00-00* SerializeRaw][%00-00 (dword_`*][%00-00*@3 data][%00-00 , dword_][%00-00*@3 "
"count][%00-00 )]&][s6; Serializes raw 32`-bit data. Might invoke LoadError if there "
"is not enough data to load.&][s1; [%00-00*C@3 data]-|Pointer to data to store or "
"buffer to receive loaded data.&][s1; [%00-00")
TOPIC_TEXT(
"*C@3 count]-|Number of values to load/store.&][s0;3 &][s4;:`:`:Stream`:`:SerializeRaw`(`:`:uint64`*`,`:`:dword`): "
"[%00-00 void_][%00-00* SerializeRaw][%00-00 (uint64_`*][%00-00*@3 data][%00-00 , "
"dword_][%00-00*@3 count][%00-00 )]&][s6; Serializes raw 64`-bit data. Might invoke "
"LoadError if there is not enough data to load.&][s1; [%00-00*C@3 data]-|Pointer to "
"data to store or buffer to receive load")
TOPIC_TEXT(
"ed data.&][s1; [%00-00*C@3 count]-|Number of values to load/store.&][s0;3 &][s4;:`:`:Stream`:`:SerializeRaw`(word`*`,`:`:dword`): "
"[%00-00 void_][%00-00* SerializeRLE][%00-00 (byte_`*][%00-00*@3 data][%00-00 , dword_][%00-00*@3 "
"count][%00-00 )]&][s6; Serializes raw 8`-bit data. Simple RLE compression is used. "
"Might invoke LoadError if there is not enough data to load or input data are invalid.&][s1")
TOPIC_TEXT(
"; [%00-00*C@3 data]-|Pointer to data to store or buffer to receive loaded data.&][s1; "
"[%00-00*C@3 count]-|Number of bytes to load/store.&][s0;3 &][s4;:`:`:Stream`:`:operator`%`(bool`&`): "
"[%00-00 Stream`&_][%00-00* operator%][%00-00 (bool`&_][%00-00*@3 d][%00-00 )]&][s6; "
"Serializes bool variable. Might invoke LoadError if there is not enough data to load.&][s1; "
"[%00-00*C@3 d]-|Variable to serialize")
TOPIC_TEXT(
".&][s1; [*/ Return value]-|`*this for chaining.&][s0;3 &][s4;:`:`:Stream`:`:operator`%`(char`&`): "
"[%00-00 Stream`&_][%00-00* operator%][%00-00 (char`&_][%00-00*@3 d][%00-00 )]&][s6; "
"Serializes char variable. Might invoke LoadError if there is not enough data to load.&][s1; "
"[%00-00*C@3 d]-|Variable to serialize.&][s1; [*/ Return value]-|`*this for chaining.&][s0;3 "
"&][s4;:`:`:Stream`:`:operator`%`(s")
TOPIC_TEXT(
"igned char`&`): [%00-00 Stream`&_][%00-00* operator%][%00-00 (signed_char`&_][%00-00*@3 "
"d][%00-00 )]&][s6; Serializes signed char variable. Might invoke LoadError if there "
"is not enough data to load.&][s1; [%00-00*C@3 d]-|Variable to serialize.&][s1; [*/ "
"Return value]-|`*this for chaining.&][s0;3 &][s4;:`:`:Stream`:`:operator`%`(unsigned "
"char`&`): [%00-00 Stream`&_][%00-00* operator%][%00-00 (unsi")
TOPIC_TEXT(
"gned_char`&_][%00-00*@3 d][%00-00 )]&][s6; Serializes unsigned char variable. Might "
"invoke LoadError if there is not enough data to load.&][s1; [%00-00*C@3 d]-|Variable "
"to serialize.&][s1; [*/ Return value]-|`*this for chaining.&][s0;3 &][s4;:`:`:Stream`:`:operator`%`(short`&`): "
"[%00-00 Stream`&_][%00-00* operator%][%00-00 (short`&_][%00-00*@3 d][%00-00 )]&][s6; "
"Serializes short variable. Might in")
TOPIC_TEXT(
"voke LoadError if there is not enough data to load.&][s1; [%00-00*C@3 d]-|Variable "
"to serialize.&][s1; [*/ Return value]-|`*this for chaining.&][s0;3 &][s4;:`:`:Stream`:`:operator`%`(unsigned "
"short`&`): [%00-00 Stream`&_][%00-00* operator%][%00-00 (unsigned_short`&_][%00-00*@3 "
"d][%00-00 )]&][s6; Serializes unsigned short variable. Might invoke LoadError if "
"there is not enough data to load.&][s1; [")
TOPIC_TEXT(
"%00-00*C@3 d]-|Variable to serialize.&][s1; [*/ Return value]-|`*this for chaining.&][s0;3 "
"&][s4;:`:`:Stream`:`:operator`%`(int`&`): [%00-00 Stream`&_][%00-00* operator%][%00-00 "
"(int`&_][%00-00*@3 d][%00-00 )]&][s6; Serializes int variable. Might invoke LoadError "
"if there is not enough data to load.&][s1; [%00-00*C@3 d]-|Variable to serialize.&][s1; "
"[*/ Return value]-|`*this for chaining.&][s0;3 &")
TOPIC_TEXT(
"][s4;:`:`:Stream`:`:operator`%`(unsigned int`&`): [%00-00 Stream`&_][%00-00* operator%][%00-00 "
"(unsigned_int`&_][%00-00*@3 d][%00-00 )]&][s6; Serializes unsigned int variable. "
"Might invoke LoadError if there is not enough data to load.&][s1; [%00-00*C@3 d]-|Variable "
"to serialize.&][s1; [*/ Return value]-|`*this for chaining.&][s0;3 &][s4;:`:`:Stream`:`:operator`%`(long`&`): "
"[%00-00 Stream`&_][%00-")
TOPIC_TEXT(
"00* operator%][%00-00 (long`&_][%00-00*@3 d][%00-00 )]&][s6; Serializes long variable. "
"Might invoke LoadError if there is not enough data to load.&][s1; [%00-00*C@3 d]-|Variable "
"to serialize.&][s1; [*/ Return value]-|`*this for chaining.&][s0;3 &][s4;:`:`:Stream`:`:operator`%`(unsigned "
"long`&`): [%00-00 Stream`&_][%00-00* operator%][%00-00 (unsigned_long`&_][%00-00*@3 "
"d][%00-00 )]&][s6; Serializes")
TOPIC_TEXT(
" unsigned long variable. Might invoke LoadError if there is not enough data to load.&][s1; "
"[%00-00*C@3 d]-|Variable to serialize.&][s1; [*/ Return value]-|`*this for chaining.&][s0;3 "
"&][s4;:`:`:Stream`:`:operator`%`(float`&`): [%00-00 Stream`&_][%00-00* operator%][%00-00 "
"(float`&_][%00-00*@3 d][%00-00 )]&][s6; Serializes float variable. Might invoke LoadError "
"if there is not enough data to load.&]")
TOPIC_TEXT(
"[s1; [%00-00*C@3 d]-|Variable to serialize.&][s1; [*/ Return value]-|`*this for "
"chaining.&][s0;3 &][s4;:`:`:Stream`:`:operator`%`(double`&`): [%00-00 Stream`&_][%00-00* "
"operator%][%00-00 (double`&_][%00-00*@3 d][%00-00 )]&][s6; Serializes double variable. "
"Might invoke LoadError if there is not enough data to load.&][s1; [%00-00*C@3 d]-|Variable "
"to serialize.&][s1; [*/ Return value]-|`*this for cha")
TOPIC_TEXT(
"ining.&][s0;3 &][s4;:`:`:Stream`:`:operator`%`(`:`:int64`&`): [%00-00 Stream`&_][%00-00* "
"operator%][%00-00 (int64`&_][%00-00*@3 d][%00-00 )]&][s6; Serializes int64 variable. "
"Might invoke LoadError if there is not enough data to load.&][s1; [%00-00*C@3 d]-|Variable "
"to serialize.&][s1; [*/ Return value]-|`*this for chaining.&][s0;3 &][s4;:`:`:Stream`:`:operator`%`(`:`:uint64`&`): "
"[%00-00 Stream`&_][")
TOPIC_TEXT(
"%00-00* operator%][%00-00 (uint64`&_][%00-00*@3 d][%00-00 )]&][s6; Serializes uint64 "
"variable. Might invoke LoadError if there is not enough data to load.&][s1; [%00-00*C@3 "
"d]-|Variable to serialize.&][s1; [*/ Return value]-|`*this for chaining.&][s0;3 &][s4;:`:`:Stream`:`:operator`%`(`:`:String`&`): "
"[%00-00 Stream`&_][%00-00* operator%][%00-00 (String`&_][%00-00*@3 s][%00-00 )]&][s6; "
"Serializes S")
TOPIC_TEXT(
"tring variable. Might invoke LoadError if there is not enough data to load or input "
"data are invalid.&][s1; [%00-00*C@3 d]-|Variable to serialize.&][s1; [*/ Return value]-|`*this "
"for chaining.&][s0;3 &][s4;:`:`:Stream`:`:operator`/`(`:`:String`&`): [%00-00 Stream`&_][%00-00* "
"operator/][%00-00 (String`&_][%00-00*@3 s][%00-00 )]&][s6; Serializes String variable "
"using RLE compression and packed forma")
TOPIC_TEXT(
"t for length. Might invoke LoadError if there is not enough data to load or input "
"data are invalid.&][s1; [%00-00*C@3 s]-|Variable to serialize.&][s1; [*/ Return value]-|`*this "
"for chaining.&][s0;3 &][s4;:`:`:Stream`:`:operator`%`(`:`:WString`&`): [%00-00 Stream`&_][%00-00* "
"operator%][%00-00 (WString`&_][%00-00*@3 s][%00-00 )]&][s6; Serializes WString variable. "
"Might invoke LoadError if there is n")
TOPIC_TEXT(
"ot enough data to load or input data are invalid.&][s1; [%00-00*C@3 d]-|Variable "
"to serialize.&][s1; [*/ Return value]-|`*this for chaining.&][s0;3 &][s4;:`:`:Stream`:`:operator`/`(`:`:WString`&`): "
"[%00-00 Stream`&_][%00-00* operator/][%00-00 (WString`&_][%00-00*@3 s][%00-00 )]&][s6; "
"Serializes String variable using RLE compression and packed format for length. Might "
"invoke LoadError if there is n")
TOPIC_TEXT(
"ot enough data to load or input data are invalid.&][s1; [%00-00*C@3 s]-|Variable "
"to serialize.&][s1; [*/ Return value]-|`*this for chaining.&][s0;3 &][s4;:`:`:Stream`:`:Pack`(`:`:dword`&`): "
"[%00-00 void_][%00-00* Pack][%00-00 (dword`&_][%00-00*@3 i][%00-00 )]&][s6; Serializes "
"dword value using format optimized for storing small values. Values 0..254 are stored "
"as serializes as single byte, other v")
TOPIC_TEXT(
"alues result in 5 bytes. Might invoke LoadError if there is not enough data to load "
"or input data are invalid.&][s1; [%00-00*C@3 i]-|Variable to serialize.&][s0;3 &][s4;:`:`:Stream`:`:operator`/`(int`&`): "
"[%00-00 Stream`&_][%00-00* operator/][%00-00 (int`&_][%00-00*@3 i][%00-00 )]&][s6; "
"Serializes int value using format optimized for storing small values. Might invoke "
"LoadError if there is not eno")
TOPIC_TEXT(
"ugh data to load or input data are invalid.&][s1; [%00-00*C@3 i]-|Variable to serialize.&][s1; "
"[*/ Return value]-|`*this for chaining.&][s0;3 &][s4;:`:`:Stream`:`:operator`/`(unsigned "
"int`&`): [%00-00 Stream`&_][%00-00* operator/][%00-00 (unsigned_int`&_][%00-00*@3 "
"i][%00-00 )]&][s6; Serializes unsigned int value using format optimized for storing "
"small values. Might invoke LoadError if there is n")
TOPIC_TEXT(
"ot enough data to load or input data are invalid.&][s1; [%00-00*C@3 i]-|Variable "
"to serialize.&][s1; [*/ Return value]-|`*this for chaining.&][s0;3 &][s4;:`:`:Stream`:`:Magic`(`:`:dword`): "
"[%00-00 void_][%00-00* Magic][%00-00 (dword_][%00-00*@3 magic][%00-00 _`=_][%00-00@3 "
"0][%00-00 x7d674d7b)]&][s6; Serializes `\"magic value`\" to ensure stream integrity. "
"When loading, this value is loaded and chec")
TOPIC_TEXT(
"ked `- mismatch results in invoking LoadError.&][s1; [%00-00*C@3 magic]-|Magic value.&][s0;3 "
"&][s4;:`:`:Stream`:`:Pack`(bool`&`,bool`&`,bool`&`,bool`&`,bool`&`,bool`&`,bool`&`,bool`&`): "
"[%00-00 void_][%00-00* Pack][%00-00 (bool`&_][%00-00*@3 a][%00-00 , bool`&_][%00-00*@3 "
"b][%00-00 , bool`&_][%00-00*@3 c][%00-00 , bool`&_][%00-00*@3 d][%00-00 , bool`&_][%00-00*@3 "
"e][%00-00 , bool`&_][%00-00*@3 f][")
TOPIC_TEXT(
"%00-00 , bool`&_][%00-00*@3 g][%00-00 , bool`&_][%00-00*@3 h][%00-00 )]&][s6; Serializes "
"a set of boolean values compressed into single byte.&][s0;3 &][s4;:`:`:Stream`:`:Pack`(bool`&`,bool`&`,bool`&`,bool`&`,bool`&`,bool`&`,bool`&`): "
"[%00-00 void_][%00-00* Pack][%00-00 (bool`&_][%00-00*@3 a][%00-00 , bool`&_][%00-00*@3 "
"b][%00-00 , bool`&_][%00-00*@3 c][%00-00 , bool`&_][%00-00*@3 d][%00-00 , bool`")
TOPIC_TEXT(
"&_][%00-00*@3 e][%00-00 , bool`&_][%00-00*@3 f][%00-00 , bool`&_][%00-00*@3 g][%00-00 "
")]&][s6; Serializes a set of boolean values compressed into single byte.&][s0;3 &][s4;:`:`:Stream`:`:Pack`(bool`&`,bool`&`,bool`&`,bool`&`,bool`&`,bool`&`): "
"[%00-00 void_][%00-00* Pack][%00-00 (bool`&_][%00-00*@3 a][%00-00 , bool`&_][%00-00*@3 "
"b][%00-00 , bool`&_][%00-00*@3 c][%00-00 , bool`&_][%00-00*@3 d][%00-0")
TOPIC_TEXT(
"0 , bool`&_][%00-00*@3 e][%00-00 , bool`&_][%00-00*@3 f][%00-00 )]&][s6; Serializes "
"a set of boolean values compressed into single byte.&][s0;3 &][s4;:`:`:Stream`:`:Pack`(bool`&`,bool`&`,bool`&`,bool`&`,bool`&`): "
"[%00-00 void_][%00-00* Pack][%00-00 (bool`&_][%00-00*@3 a][%00-00 , bool`&_][%00-00*@3 "
"b][%00-00 , bool`&_][%00-00*@3 c][%00-00 , bool`&_][%00-00*@3 d][%00-00 , bool`&_][%00-00*@3 "
"e][%00-")
TOPIC_TEXT(
"00 )]&][s6; Serializes a set of boolean values compressed into single byte.&][s0;3 "
"&][s4;:`:`:Stream`:`:Pack`(bool`&`,bool`&`,bool`&`,bool`&`): [%00-00 void_][%00-00* "
"Pack][%00-00 (bool`&_][%00-00*@3 a][%00-00 , bool`&_][%00-00*@3 b][%00-00 , bool`&_][%00-00*@3 "
"c][%00-00 , bool`&_][%00-00*@3 d][%00-00 )]&][s6; Serializes a set of boolean values "
"compressed into single byte.&][s0;3 &][s4;:`:`:Stream")
TOPIC_TEXT(
"`:`:Pack`(bool`&`,bool`&`,bool`&`): [%00-00 void_][%00-00* Pack][%00-00 (bool`&_][%00-00*@3 "
"a][%00-00 , bool`&_][%00-00*@3 b][%00-00 , bool`&_][%00-00*@3 c][%00-00 )]&][s6; "
"Serializes a set of boolean values compressed into single byte.&][s0;3 &][s4;:`:`:Stream`:`:Pack`(bool`&`,bool`&`): "
"[%00-00 void_][%00-00* Pack][%00-00 (bool`&_][%00-00*@3 a][%00-00 , bool`&_][%00-00*@3 "
"b][%00-00 )]&][s6; Seria")
TOPIC_TEXT(
"lizes a set of boolean values compressed into single byte.&][s0;3 &][s4;:`:`:Stream`:`:Stream`(`): "
"[* Stream]()&][s6; Constructor. Sets stream into Loading serialization mode and zero "
"level indentation. All protected variables are set to 0 / NULL.&][s0;3 &][s4;:`:`:Stream`:`:`~Stream`(`): "
"`~[* Stream]()&][s6; Destructor.&][s0;3 &][s0;3 &][s0;3 &][s0;3 &][s10;:`:`:StringStream`:`:class:* "
"[%00-00* c")
TOPIC_TEXT(
"lass_][%00-00 StringStream]&][s0;* &][s6; StringStream is stream that uses String "
"as storage medium.&][s6; &][s9;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:Stream`:`:class "
"Stream^ [/^^ Derived from][^^  ][%00-00 Stream]&][s0;3 &][s4;:`:`:StringStream`:`:Open`(const`:`:String`&`): "
"[%00-00 void_][%00-00* Open][%00-00 (const_String`&_][%00-00*@3 data][%00-00 )]&][s6; "
"[%00-00 Sets the content of strea")
TOPIC_TEXT(
"m to specified String and sets it into the Loading serialization mode.]&][s1; [%00-00*C@3 "
"data]-|Content of stream.&][s0;3 &][s4;:`:`:StringStream`:`:Create`(`): [%00-00 void_][%00-00* "
"Create][%00-00 ()]&][s6; Creates empty StringStream and sets it into Storing serialization "
"mode.&][s0;3 &][s4;:`:`:StringStream`:`:GetResult`(`): [%00-00 String_][%00-00* GetResult][%00-00 "
"()]&][s6; Returns resultin")
TOPIC_TEXT(
"g String.&][s1; [*/ Return value]-|Result.&][s0;3 &][s4;:`:`:StringStream`:`:operator`:`:String`(`): "
"[%00-00* operator_String][%00-00 ()]&][s1; [*/ Return value]-|GetResult().&][s0;3 "
"&][s4;:`:`:StringStream`:`:StringStream`(`): [%00-00* StringStream][%00-00 ()]&][s6; "
"Consructs empty stream and sets it into Storing serialization mode (like [* Create]).&][s0;3 "
"&][s4;:`:`:StringStream`:`:StringStream")
TOPIC_TEXT(
"`(const`:`:String`&`): [%00-00* StringStream][%00-00 (const_String`&_][%00-00*@3 "
"data][%00-00 )]&][s6; Constructs stream with specified content and sets it into Loading "
"serialization mode (like [* Open]).&][s1; [%00-00*C@3 data]-|Content of stream.&][s0;3 "
"&][s0;3 &][s0;3 &][s0;3 &][s10;:`:`:MemStream`:`:class:* [%00-00* class_][%00-00 "
"MemStream]&][s6; &][s6; Stream that is using raw memory as its ")
TOPIC_TEXT(
"content.&][s6; &][s9;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:Stream`:`:class "
"Stream^ [/^^ Derived from][^^  ][%00-00 Stream]&][s0;3 &][s4;:`:`:MemStream`:`:MemStream`(void`*`,int`): "
"[%00-00* MemStream][%00-00 (void_`*][%00-00*@3 data][%00-00 , int_][%00-00*@3 size][%00-00 "
")]&][s6; Constructs [* MemStream] at specified memory buffer.&][s1; [%00-00*C@3 data]-|Pointer "
"to the stream content.&][s1; ")
TOPIC_TEXT(
"[%00-00*C@3 size]-|Size.&][s0;3 &][s0;3 &][s0;3 &][s0;3 &][s10;:`:`:MemReadStream`:`:class:* "
"[%00-00* class_][%00-00 MemReadStream]&][s6; &][s6; Read`-only stream using raw memory "
"as content.&][s6; &][s9;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:MemStream`:`:class "
"MemStream^ [/^^ Derived from][^^  ][%00-00 MemStream]&][s0;3 &][s4;:`:`:MemReadStream`:`:MemReadStream`(const "
"void`*`,int`): [%00-00* ")
TOPIC_TEXT(
"MemReadStream][%00-00 (const_void_`*][%00-00*@3 data][%00-00 , int_][%00-00*@3 size][%00-00 "
")]&][s6; Constructs [* MemStream] at specified memory buffer.&][s1; [%00-00*C@3 data]-|Pointer "
"to the stream content.&][s1; [%00-00*C@3 size]-|Size.&][s0;3 &][s0;3 &][s0;3 &][s0;3 "
"&][s10;:`:`:BlockStream`:`:class:* [%00-00* class_][%00-00 BlockStream]&][s6; &][s6; "
"BlockStream implements operations needed to")
TOPIC_TEXT(
" manage streams that are able to read or write a block of data at random positon. "
"BlockStream provides buffered implementation of such stream. It implements all virtual "
"methods of Stream, with exception of [* IsOpen] and [* Close], using new virtual "
"methods [* Read], [* Write] and [* SetStreamSize].&][s6; &][s9;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:BufferStream`:`:class "
"BufferStream^ [/^^ Der")
TOPIC_TEXT(
"ived from][^^  ][%00-00 BufferStream]&][s0;3 &][s4; [%00-00 enum_`{_][%00-00*@3 "
"READ][%00-00 , ][%00-00*@3 CREATE][%00-00 , ][%00-00*@3 APPEND][%00-00 , ][%00-00*@3 "
"READWRITE][%00-00 , ][%00-00*@3 NOWRITESHARE][%00-00 , ][%00-00*@3 DELETESHARE][%00-00 "
", ][%00-00*@3 NOREADSHARE][%00-00 , ][%00-00*@3 SHAREMASK][%00-00 _`}]&][s6; This "
"enum defines basic operation modes of BlockStream (used combined w")
TOPIC_TEXT(
"ith binary or).&][s1;:`:`:BlockStream`:`:READ: [%00-00*C@3 READ]-|Read mode.&][s1;:`:`:BlockStream`:`:CREATE: "
"[%00-00*C@3 CREATE]-|Write mode.&][s1;:`:`:BlockStream`:`:APPEND: [%00-00*C@3 APPEND]-|Append "
"mode `- means that initial position in the stream is at the end of it.&][s1;:`:`:BlockStream`:`:READWRITE: "
"[%00-00*C@3 READWRITE]-|Enables subsequent streams full access to stream.&][s1;:`:`:Block")
TOPIC_TEXT(
"Stream`:`:NOWRITESHARE: [%00-00*C@3 NOWRITESHARE]-|Disables subsequent streams to "
"write to the stream.&][s1;:`:`:BlockStream`:`:DELETESHARE: [%00-00*C@3 DELETESHARE]-|Enables "
"subsequent streams to delete the stream.&][s1;:`:`:BlockStream`:`:NOREADSHARE: [%00-00*C@3 "
"NOREADSHARE]-|Disables subsequent streams to read the stream.&][s0;3 &][s4;:`:`:BlockStream`:`:Read`(`:`:int64`,void`*`,`:`:dword`): "
"[")
TOPIC_TEXT(
"%00-00 virtual dword_][%00-00* Read][%00-00 (int64_][%00-00*@3 at][%00-00 , void_`*][%00-00*@3 "
"ptr][%00-00 , dword_][%00-00*@3 size][%00-00 )]&][s11; [%00-00 Protected.]&][s6; "
"Implementation of this virtual method in derived class should read a block of data "
"at specified position in media.&][s1; [%00-00*C@3 at]-|Position in media.&][s1; [%00-00*C@3 "
"ptr]-|Pointer to buffer to receive data.&][s1; [%")
TOPIC_TEXT(
"00-00*C@3 size]-|Requested size of data.&][s1; [*/ Return value]-|Size of data read.&][s0;3 "
"&][s4;:`:`:BlockStream`:`:Write`(`:`:int64`,const void`*`,`:`:dword`): [%00-00 virtual "
"void_][%00-00* Write][%00-00 (int64_][%00-00*@3 at][%00-00 , const_void_`*][%00-00*@3 "
"data][%00-00 , dword_][%00-00*@3 size][%00-00 )]&][s11; Protected.&][s6; Implementation "
"of this virtual method in derived method should")
TOPIC_TEXT(
" write a block of data at specified position in media.&][s1; [%00-00*C@3 at]-|Position "
"in media.&][s1; [%00-00*C@3 data]-|Pointer to  data.&][s1; [%00-00*C@3 size]-|Size "
"of data.&][s0;3 &][s4;:`:`:BlockStream`:`:SetStreamSize`(`:`:int64`): [%00-00 virtual "
"void_][%00-00* SetStreamSize][%00-00 (int64_][%00-00*@3 size][%00-00 )]&][s11; Protected.&][s6; "
"Implementation of this virtual method in derived")
TOPIC_TEXT(
" class should adjust the size of media.&][s1; [%00-00*C@3 size]-|Requested new size "
"of media.&][s0;3 &][s4;:`:`:BlockStream`:`:GetMediaSize`(`)const: [%00-00 int64_][%00-00* "
"GetMediaSize][%00-00 ()_const]&][s11; Protected.&][s6; Returns current media size. "
"Note that this might be different from current stream`-size `- size of media adjusting "
"can be deffered to flushing the buffer.&][s1; [*/ Return")
TOPIC_TEXT(
" value]-|Current media size.&][s0;3 &][s4;:`:`:BlockStream`:`:OpenInit`(`:`:dword`,`:`:int64`): "
"[%00-00 void_][%00-00* OpenInit][%00-00 (dword_][%00-00*@3 mode][%00-00 , int64_][%00-00*@3 "
"file`_size][%00-00 )]&][s11; Protected.&][s6; Initializes the BlockStream to specified "
"mode and actual media size.&][s1; [%00-00*C@3 mode]-|Mode.&][s1; [%00-00*C@3 file`_size]-|Actual "
"media size.&][s0;3 &][s4;:`:")
TOPIC_TEXT(
"`:BlockStream`:`:SetBufferSize`(`:`:dword`): [%00-00 void_][%00-00* SetBufferSize][%00-00 "
"(dword_][%00-00*@3 newsize][%00-00 )]&][s6; Sets a new size of internal buffer.&][s1; "
"[%00-00*C@3 newsize]-|The new size of buffer.&][s0;3 &][s4;:`:`:BlockStream`:`:GetBufferSize`(`)const: "
"[%00-00 dword_][%00-00* GetBufferSize][%00-00 ()_const]&][s1; [*/ Return value]-|Size "
"of buffer.&][s0;3 &][s0;3 &][s0;3 &")
TOPIC_TEXT(
"][s0;3 &][s10;:`:`:FileStream`:`:class:* [%00-00* class_][%00-00 FileStream]&][s6; "
"&][s6; Classical file stream.&][s6; &][s9;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:BlockStream`:`:class "
"BlockStream^ [/^^ Derived from][^^  ][%00-00 BlockStream]&][s0;3 &][s4;:`:`:FileStream`:`:FileStream`(const "
"char`*`,`:`:dword`): [%00-00* FileStream][%00-00 (const_char_`*][%00-00*@3 filename][%00-00 "
", dword_][%")
TOPIC_TEXT(
"00-00*@3 mode][%00-00 )]&][s6; Opens file stream in specified mode (as defined in "
"BlockStream).&][s1; [%00-00*C@3 filename]-|The name of the file.&][s1; [%00-00*C@3 "
"mode]-|Open mode.&][s0;3 &][s4;:`:`:FileStream`:`:FileStream`(const char`*`,`:`:dword`,mode`_t`): "
"[%00-00* FileStream][%00-00 (const_char_`*][%00-00*@3 filename][%00-00 , dword_][%00-00*@3 "
"mode][%00-00 , mode`_t_][%00-00*@3 acm][%00-00")
TOPIC_TEXT(
" _`=_][%00-00@3 0644][%00-00 )]&][s11; [%00-00 Posix specific.]&][s6; Opens file "
"stream in specified mode (as defined in BlockStream) and specific POSIX access rights.&][s1; "
"[%00-00*C@3 filename]-|The name of the file.&][s1; [%00-00*C@3 mode]-|Open mode.&][s1; "
"[%00-00*C@3 acm]-|Access rights.&][s0;3 &][s4;:`:`:FileStream`:`:FileStream`(int`): "
"[%00-00* FileStream][%00-00 (int_][%00-00*@3 std`_handl")
TOPIC_TEXT(
"e][%00-00 )]&][s11; Posix specific.&][s6; Assigns existing file handle to FileStream.&][s1; "
"[%00-00*C@3 std`_handle]-|File handle of open file. FileStream takes ownership of "
"this handle.&][s0;3 &][s4;:`:`:FileStream`:`:FileStream`(`): [%00-00* FileStream][%00-00 "
"()]&][s6; Creates empty unopened FileStream.&][s0;3 &][s4;:`:`:FileStream`:`:operator "
"bool`(`)const: [%00-00* operator_bool][%00-00 ()_co")
TOPIC_TEXT(
"nst]&][s1; [*/ Return value]-|True if stream is open.&][s0;3 &][s4;:`:`:FileStream`:`:GetTime`(`)const: "
"[%00-00 FileTime_][%00-00* GetTime][%00-00 ()_const]&][s6; Returns last`-write time "
"of stream.&][s1; [*/ Return value]-|Time.&][s0;3 &][s4;:`:`:FileStream`:`:SetTime`(const`:`:FileTime`&`): "
"[%00-00 bool_][%00-00* Open][%00-00 (const_char_`*][%00-00*@3 filename][%00-00 , "
"dword_][%00-00*@3 mode][%")
TOPIC_TEXT(
"00-00 )]&][s6; Opens file stream in specified mode (as defined in BlockStream).&][s1; "
"[%00-00*C@3 filename]-|The name of the file.&][s6; [%00-00*C@3 mode]-|Open mode.&][s0;3 "
"&][s4;:`:`:FileStream`:`:Open`(const char`*`,`:`:dword`,mode`_t`): [%00-00 bool_][%00-00* "
"Open][%00-00 (const_char_`*][%00-00*@3 filename][%00-00 , dword_][%00-00*@3 mode][%00-00 "
", mode`_t_][%00-00*@3 acm][%00-00 _`=_][%00-00@")
TOPIC_TEXT(
"3 0644][%00-00 )]&][s11; [%00-00 Posix specific.]&][s6; Opens file stream in specified "
"mode (as defined in BlockStream) and specific POSIX access rights.&][s1; [%00-00*C@3 "
"filename]-|The name of the file.&][s1; [%00-00*C@3 mode]-|Open mode.&][s6; [%00-00*C@3 "
"acm]-|Access rights.&][s0;3 &][s4;:`:`:FileStream`:`:GetHandle`(`)const: [%00-00 "
"HANDLE_][%00-00* GetHandle][%00-00 ()_const]&][s11; Win32 sp")
TOPIC_TEXT(
"ecific.&][s1; [*/ Return value]-|File handle.&][s0;3 &][s4;:`:`:FileStream`:`:GetHandle`(`)const: "
"[%00-00 int_][%00-00* GetHandle][%00-00 ()_const]&][s11; Posix specific.&][s1; [*/ "
"Return value]-|File handle.&][s0;3 &][s0;3 &][s0;3 &][s0;3 &][s10;:`:`:FileIn`:`:class:* "
"[%00-00* class_][%00-00 FileIn]&][s6; &][s6; Simple helper class that represents "
"FileStream in read mode.&][s6; &][s9;^topic`:`/`/")
TOPIC_TEXT(
"Core`/src`/Stream`$en`-us`#`:`:FileStream`:`:class FileStream^ [/^^ Derived from][^^ "
" ][%00-00 FileStream]&][s0;3 &][s4;:`:`:FileIn`:`:FileIn`(const char`*`): [%00-00* "
"FileIn][%00-00 (const_char_`*][%00-00*@3 fn][%00-00 )]&][s6; Opens file for reading.&][s1; "
"[%00-00*C@3 fn]-|File name.&][s0;3 &][s4;:`:`:FileIn`:`:FileIn`(`): [%00-00* FileIn][%00-00 "
"()]&][s6; Constructs empty FileStream.&][s0;3 &][")
TOPIC_TEXT(
"s4;:`:`:FileIn`:`:Open`(const char`*`): [%00-00 bool_][%00-00* Open][%00-00 (const_char_`*][%00-00*@3 "
"fn][%00-00 )]&][s6; Opens file for reading.&][s1; [%00-00*C@3 fn]-|File name.&][s1; "
"[*/ Return value]-|True if open was successful.&][s0;3 &][s0;3 &][s0;3 &][s0;3 &][s10;:`:`:FileOut`:`:class:* "
"[%00-00* class_][%00-00 FileOut]&][s6; &][s6; Simple helper class that represents "
"FileStream in write mo")
TOPIC_TEXT(
"de.&][s6; &][s9;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:FileStream`:`:class FileStream^ "
"[/^^ Derived from][^^  ][%00-00 FileStream]&][s0;3 &][s4;:`:`:FileOut`:`:FileOut`(const "
"char`*`): [%00-00* FileOut][%00-00 (const_char_`*][%00-00*@3 fn][%00-00 )]&][s6; "
"Opens file for writing.&][s1; [%00-00*C@3 fn]-|File name.&][s0;3 &][s4;:`:`:FileOut`:`:FileOut`(`): "
"[%00-00* FileOut][%00-00 ()]&][s6; [%00-")
TOPIC_TEXT(
"00 Constructs non`-opened FileStream.]&][s0;3 &][s4;:`:`:FileOut`:`:Open`(const "
"char`*`,mode`_t`): [%00-00 bool_][%00-00* Open][%00-00 (const_char_`*][%00-00*@3 "
"fn][%00-00 )]&][s6; Opens file for writing.&][s1; [%00-00*C@3 fn]-|File name.&][s1; "
"[*/ Return value]-|True if open was successful.&][s0;3 &][s0;3 &][s0;3 &][s0;3 &][s10;:`:`:FileAppend`:`:class:* "
"[%00-00* class_][%00-00 FileAppend]&][s6; ")
TOPIC_TEXT(
"&][s6; Simple helper class that represents FileStream in append mode `- that in "
"fact means in write mode with current position at the end of the file.&][s6; &][s9; "
"[/ Derived from] [%00-00 FileStream]&][s0;3 &][s4;:`:`:FileAppend`:`:FileAppend`(const "
"char`*`): [%00-00* FileAppend][%00-00 (const_char_`*][%00-00*@3 fn][%00-00 )]&][s6; "
"Opens file in append mode.&][s1; [%00-00*C@3 fn]-|File name.&][s0")
TOPIC_TEXT(
";3 &][s4;:`:`:FileAppend`:`:FileAppend`(`): [%00-00* FileAppend][%00-00 ()]&][s6; "
"Constructs empty FileStream.&][s0;3 &][s4;:`:`:FileAppend`:`:Open`(const char`*`): "
"[%00-00 bool_][%00-00* Open][%00-00 (const_char_`*][%00-00*@3 fn][%00-00 )]&][s6; "
"Opens file in append mode.&][s1; [%00-00*C@3 fn]-|File name.&][s1; [*/ Return value]-|true "
"when Open was successful.&][s0;3 &][s0;3 &][s0;3 &][s0;3 &][s1")
TOPIC_TEXT(
"0;:`:`:SizeStream`:`:class:* [%00-00* class_][%00-00 SizeStream]&][s6; &][s6; Special "
"output stream that in fact does not store output data, only counts the total number "
"of bytes written.&][s6; &][s9;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:BufferStream`:`:class "
"BufferStream^ [/^^ Derived from][^^  ][%00-00 BufferStream]&][s0;3 &][s4;:`:`:SizeStream`:`:SizeStream`(`): "
"[%00-00* SizeStream][%00-00")
TOPIC_TEXT(
" ()]&][s6; Constructor `- co&][s0;3 &][s4;:`:`:SizeStream`:`:`_Put`(int`): [%00-00* "
"operator_int64][%00-00 ()_const]&][s1; [*/ Return value]-|Current number of bytes "
"written.&][s0;3 &][s4;:`:`:SizeStream`:`:Open`(`): [%00-00 void_][%00-00* Open][%00-00 "
"()]&][s6; Reopens data `- resets the counter of output bytes.&][s0;3 &][s0;3 &][s0;3 "
"&][s0;3 &][s10;:`:`:CompareStream`:`:class:* [%00-00* class_][")
TOPIC_TEXT(
"%00-00 CompareStream]&][s6; &][s6; Special output stream that instead of storing "
"data performs their comparison to the data of another stream.&][s6; &][s9;^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:BufferStream`:`:class "
"BufferStream^ [/^^ Derived from][^^  ][%00-00 BufferStream]&][s0;3 &][s4;:`:`:CompareStream`:`:CompareStream`(`): "
"[%00-00* CompareStream][%00-00 ()]&][s6; Constructs closed Compare")
TOPIC_TEXT(
"Stream.&][s0;3 &][s4;:`:`:CompareStream`:`:CompareStream`(`:`:Stream`&`): [%00-00* "
"CompareStream][%00-00 (Stream`&_][%00-00*@3 aStream][%00-00 )]&][s6; Constructors "
"CompareStream opened for comparison with specified stream.&][s1; [%00-00*C@3 aStream]-|Stream "
"to compare with.&][s0;3 &][s4;:`:`:CompareStream`:`:`_Put`(int`): [%00-00 void_][%00-00* "
"Open][%00-00 (Stream`&_][%00-00*@3 aStream][%00-00 )")
TOPIC_TEXT(
"]&][s6; Opens CompareStream for comparison with the specified stream.&][s1; [%00-00*C@3 "
"aStream]-|Stream to compare with.&][s0;3 &][s4;:`:`:CompareStream`:`:IsEqual`(`): "
"[%00-00 bool_][%00-00* IsEqual][%00-00 ()]&][s1; [*/ Return value]-|true if all bytes "
"written so far match those in comparison stream.&][s0;3 &][s4;:`:`:CompareStream`:`:operator "
"bool`(`): [%00-00* operator_bool][%00-00 ()]&][s1; ")
TOPIC_TEXT(
"[*/ Return value]-|IsEqual().&][s0;3 ]")
