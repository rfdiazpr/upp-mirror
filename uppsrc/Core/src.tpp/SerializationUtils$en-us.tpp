TITLE("Serialization utilities")
REF("::Pack16(::Stream&,int&)")
REF("::Pack16(::Stream&,int&,int&)")
REF("::Pack16(::Stream&,int&,int&,int&)")
REF("::Pack16(::Stream&,int&,int&,int&,int&)")
REF("::Pack16(::Stream&,int&,int&,int&,int&,int&)")
REF("::Load(::Callback1<::Stream&>,::Stream&)")
REF("::Store(::Callback1<::Stream&>,::Stream&)")
REF("::LoadFromFile(::Callback1<::Stream&>,const char*)")
REF("::StoreToFile(::Callback1<::Stream&>,const char*)")
REF("::Load(T&,::Stream&)")
REF("::Store(T&,::Stream&)")
REF("::LoadFromFile(T&,const char*)")
REF("::StoreToFile(T&,const char*)")
REF("::StoreAsString(T&)")
REF("::LoadFromString(T&,const::String&)")
REF("::RegisterGlobalConfig(const char*)")
REF("::RegisterGlobalConfig(const char*,::Callback)")
REF("::LoadFromGlobal(T&,const char*)")
REF("::StoreToGlobal(T&,const char*)")
REF("::SerializeGlobalConfigs(::Stream&)")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2; Serialization utilities&][s0;3 "
"&][s4;:`:`:Pack16`(`:`:Stream`&`,int`&`): [%00-00 Stream`&_][%00-00* Pack16][%00-00 "
"(Stream`&_][%00-00*@3 s][%00-00 , int`&_][%00-00*@3 i][%00-00 )]&][s6; Serializes "
"32`-bit data, optimizing it for 16`-bit values. If ")
TOPIC_TEXT(
"value is in `-32767`-32767 range, it is serialized as 2 bytes, otherwise 6 bytes "
"are used.&][s1; [%00-00*C@3 s]-|Stream.&][s1; [%00-00*C@3 i]-|Data to serialize.&][s1; "
"[*/ Return value]-|s for chaining.&][s0;3 &][s13;:`:`:Pack16`(`:`:Stream`&`,int`&`,int`&`): "
"Stream`&_[* Pack16](Stream`&_[*@3 s], int`&_[*@3 i1], int`&_[*@3 i2])&][s14;a0;:`:`:Pack16`(`:`:Stream`&`,int`&`,int`&`,int`&`): "
"Stream`&_[*")
TOPIC_TEXT(
" Pack16](Stream`&_[*@3 s], int`&_[*@3 i1], int`&_[*@3 i2], int`&_[*@3 i3])&][s14;a0;:`:`:Pack16`(`:`:Stream`&`,int`&`,int`&`,int`&`,int`&`): "
"Stream`&_[* Pack16](Stream`&_[*@3 s], int`&_[*@3 i1], int`&_[*@3 i2], int`&_[*@3 "
"i3], int`&_[*@3 i4])&][s14;:`:`:Pack16`(`:`:Stream`&`,int`&`,int`&`,int`&`,int`&`,int`&`): "
"Stream`&_[* Pack16](Stream`&_[*@3 s], int`&_[*@3 i1], int`&_[*@3 i2], int`&_[*@3 "
"i3], i")
TOPIC_TEXT(
"nt`&_[*@3 i4], int`&_[*@3 i5])&][s6; Calls Pack16 for each of individual 32`-bit "
"integer parameters.&][s0;3 &][s4;:`:`:Load`(`:`:Callback1`<`:`:Stream`&`>`,`:`:Stream`&`): "
"[%00-00 bool_][%00-00* Load][%00-00 (Callback1<Stream`&>_][%00-00*@3 serialize][%00-00 "
", Stream`&_][%00-00*@3 stream][%00-00 )]&][s6; Smart serialization restore. Data "
"to serialize are represented by Callback. First, backup of c")
TOPIC_TEXT(
"urrent state of data is performed and stored in memory. Then data are loaded from "
"the specified stream. If restoring of data is successful (no LoadingError exception "
"is thrown), Load returns true. If LoadingError is thrown, it is caught by Load, data "
"are restored from backup copy and Load returns false.&][s1; [%00-00*C@3 serialize]-|Callback "
"to the serialization function.&][s1; [%00-00*C@3 stream]")
TOPIC_TEXT(
"-|Stream.&][s1; [*/ Return value]-|true indicates success.&][s0;3 &][s4;:`:`:Store`(`:`:Callback1`<`:`:Stream`&`>`,`:`:Stream`&`): "
"[%00-00 bool_][%00-00* Store][%00-00 (Callback1<Stream`&>_][%00-00*@3 serialize][%00-00 "
", Stream`&_][%00-00*@3 stream][%00-00 )]&][s6; Serialization save. Data to serialize "
"are represented by Callback.&][s1; [%00-00*C@3 serialize]-|Callback to the serialization "
"functio")
TOPIC_TEXT(
"n.&][s1; [%00-00*C@3 stream]-|Stream.&][s1; [*/ Return value]-|true if data were "
"properly stored to the stream.&][s0;3 &][s4;:`:`:LoadFromFile`(`:`:Callback1`<`:`:Stream`&`>`,const "
"char`*`): [%00-00 bool_][%00-00* LoadFromFile][%00-00 (Callback1<Stream`&>_][%00-00*@3 "
"serialize][%00-00 , const_char_`*][%00-00*@3 file][%00-00 _`=_NULL)]&][s6; Using "
"[^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`")
TOPIC_TEXT(
"#`:`:Load`(Callback1`<Stream`&`>`,Stream`&`)^ Load], data are restored from the "
"file.&][s1; [%00-00*C@3 serialize]-|Callback to the serialization function.&][s1; "
"[%00-00*C@3 file]-|Filename.&][s1; [*/ Return value]-|true indicates success.&][s0;3 "
"&][s4;:`:`:StoreToFile`(`:`:Callback1`<`:`:Stream`&`>`,const char`*`): [%00-00 bool_][%00-00* "
"StoreToFile][%00-00 (Callback1<Stream`&>_][%00-00*@3 serial")
TOPIC_TEXT(
"ize][%00-00 , const_char_`*][%00-00*@3 file][%00-00 _`=_NULL)]&][s6; Using [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:Store`(Callback1`<Stream`&`>`,Stream`&`)^ "
"Store] data are saved to the file.&][s1; [%00-00*C@3 serialize]-|Callback to the "
"serialization function.&][s1; [%00-00*C@3 file]-|Filename.&][s1; [*/ Return value]-|true "
"indicates success.&][s0;3 &][s4;:`:`:Load`(T`&`,`:`:Strea")
TOPIC_TEXT(
"m`&`): template <class [*@4 T]> [%00-00 bool_][%00-00* Load][%00-00 (][%00-00*@4 "
"T][%00-00 `&_][%00-00*@3 x][%00-00 , Stream`&_][%00-00*@3 s][%00-00 )]&][s6; Calls "
"[^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:Load`(Callback1`<Stream`&`>`,Stream`&`)^ "
"Load] with callback(`&[@3 x], `&T`::Serialize) as serialization callback. In other "
"words, [@3 x] is restored using its Serialize method.&][")
TOPIC_TEXT(
"s1; [*C@4 T]-|Type of object to restore.&][s1; [%00-00*C@3 x]-|Reference to the "
"object.&][s1; [%00-00*C@3 s]-|Stream.&][s1; [*/ Return value]-|true indicates success.&][s0;3 "
"&][s4;:`:`:Store`(T`&`,`:`:Stream`&`): template <class [*@4 T]> [%00-00 bool_][%00-00* "
"Store][%00-00 (][%00-00*@4 T][%00-00 `&_][%00-00*@3 x][%00-00 , Stream`&_][%00-00*@3 "
"s][%00-00 )]&][s6; Calls [^topic`:`/`/Core`/src`/Seria")
TOPIC_TEXT(
"lizationUtils`$en`-us`#`:`:Store`(T`&`,Stream`&`)^ Store] with callback(`&[@3 x], "
"`&T`::Serialize) as serialization callback. In other words, [@3 x] is stored using "
"its Serialize method.&][s1; [*C@4 T]-|Type of object to restore.&][s1; [%00-00*C@3 "
"x]-|Reference to the object.&][s1; [%00-00*C@3 s]-|Stream.&][s1; [*/ Return value]-|true "
"indicates success.&][s0;3 &][s4;:`:`:LoadFromFile`(T`&`,const c")
TOPIC_TEXT(
"har`*`): template <class [*@4 T]> [%00-00 bool_][%00-00* LoadFromFile][%00-00 (][%00-00*@4 "
"T][%00-00 `&_][%00-00*@3 x][%00-00 , const_char_`*][%00-00*@3 name][%00-00 _`=_NULL)]&][s6; "
"Calls [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:LoadFromFile`(Callback1`<Stream`&`>`,const "
"char`*`)^ LoadFromFile] with callback(`&[@3 x], `&T`::Serialize) as serialization "
"callback. In other words, [@3 ")
TOPIC_TEXT(
"x] is restored from the file using its Serialize method.&][s1; [*C@4 T]-|Type of "
"object to restore.&][s1; [%00-00*C@3 x]-|Reference to the object.&][s1; [%00-00*C@3 "
"name]-|Filename.&][s1; [*/ Return value]-|true indicates success.&][s0;3 &][s4;:`:`:StoreToFile`(T`&`,const "
"char`*`): template <class [*@4 T]> [%00-00 bool_][%00-00* StoreToFile][%00-00 (][%00-00*@4 "
"T][%00-00 `&_][%00-00*@3 x][%00-00 ,")
TOPIC_TEXT(
" const_char_`*][%00-00*@3 name][%00-00 _`=_NULL)]&][s6; Calls [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:StoreToFile`(Callback1`<Stream`&`>`,const "
"char`*`)^ StoreToFile] with callback(`&[@3 x], `&T`::Serialize) as serialization "
"callback. In other words, [@3 x] is stored to the file using its Serialize method.&][s1; "
"[*C@4 T]-|Type of object to store.&][s1; [%00-00*C@3 x]-|Reference to ")
TOPIC_TEXT(
"the object.&][s1; [%00-00*C@3 name]-|Filename.&][s1; [*/ Return value]-|true indicates "
"success.&][s0;3 &][s4;:`:`:StoreAsString`(T`&`): template <class [*@4 T]> [%00-00 "
"String_][%00-00* StoreAsString][%00-00 (][%00-00*@4 T][%00-00 `&_][%00-00*@3 x][%00-00 "
")]&][s6; Stores [@3 x] using its Serialize method and StringStream.&][s1; [*C@4 T]-|Type "
"of object to store.&][s1; [%00-00*C@3 x]-|Reference to ")
TOPIC_TEXT(
"the object.&][s1; [*/ Return value]-|String containing serialized [@3 x].&][s0;3 "
"&][s4;:`:`:LoadFromString`(T`&`,const`:`:String`&`): template <class [*@4 T]> [%00-00 "
"bool_][%00-00* LoadFromString][%00-00 (][%00-00*@4 T][%00-00 `&_][%00-00*@3 x][%00-00 "
", const_String`&_][%00-00*@3 s][%00-00 )]&][s6; Restores serialized data from the "
"String (e.g. previously stored by StoreAsString).&][s1; [*C@4 T]-")
TOPIC_TEXT(
"|Type of object to restore.&][s1; [%00-00*C@3 x]-|Reference to object.&][s1; [%00-00*C@3 "
"s]-|String containing serialized [@3 x].&][s1; [*/ Return value]-|true indicates "
"success.&][s0; &][s0; &][s3; Global modular serialization support&][s5; Modular serialization "
"is a viable option for storing configuration of applications consisting of many modules. "
"It allows individual storing/loading data for g")
TOPIC_TEXT(
"iven global configuration key and also serialization of all such data with single "
"stream.&][s0;3 &][s4;:`:`:RegisterGlobalConfig`(const char`*`): [%00-00 void_][%00-00* "
"RegisterGlobalConfig][%00-00 (const_char_`*][%00-00*@3 name][%00-00 )]&][s6; Registers "
"name as global configuration key.&][s1; [%00-00*C@3 name]-|Key.&][s0;3 &][s4;:`:`:RegisterGlobalConfig`(const "
"char`*`,`:`:Callback`): [%00-00 vo")
TOPIC_TEXT(
"id_][%00-00* RegisterGlobalConfig][%00-00 (const_char_`*][%00-00*@3 name][%00-00 "
", Callback_][%00-00*@3 WhenFlush][%00-00 )]&][s6; Registers name as global configuration "
"key, with flush callback.&][s1; [%00-00*C@3 name]-|Key.&][s1; [%00-00*C@3 WhenFlush]-|This "
"callback is called before storing of all configuration keys is performed by SerializeGlobalConfigs "
"`- this is useful when StoreToGlobal has")
TOPIC_TEXT(
" to be explicitly triggered before storing configuration.&][s0;3 &][s4;:`:`:LoadFromGlobal`(T`&`,const "
"char`*`): template <class [*@4 T]> [%00-00 bool_][%00-00* LoadFromGlobal][%00-00 "
"(][%00-00*@4 T][%00-00 `&_][%00-00*@3 x][%00-00 , const_char_`*][%00-00*@3 name][%00-00 "
")]&][s6; Loads [@3 x] from global configuration key, using smart backup methods provided "
"by [^topic`:`/`/Core`/src`/Serializatio")
TOPIC_TEXT(
"nUtils`$en`-us`#`:`:Load`(Callback1`<Stream`&`>`,Stream`&`)^ Load].&][s1; [*C@4 "
"T]-|Type of serialized object.&][s1; [%00-00*C@3 x]-|Reference to serialized object.&][s1; "
"[%00-00*C@3 name]-|Configuration key.&][s1; [*/ Return value]-|true indicates success.&][s0;3 "
"&][s4;:`:`:StoreToGlobal`(T`&`,const char`*`): template <class [*@4 T]> [%00-00 void_][%00-00* "
"StoreToGlobal][%00-00 (][%00-00*@4 T][%0")
TOPIC_TEXT(
"0-00 `&_][%00-00*@3 x][%00-00 , const_char_`*][%00-00*@3 name][%00-00 )]&][s6; Stores "
"[@3 x] to global configuration key.&][s1; [*C@4 T]-|Type of serialized object.&][s1; "
"[%00-00*C@3 x]-|Reference to serialized object.&][s1; [%00-00*C@3 name]-|Configuration "
"key.&][s0;3 &][s4;:`:`:SerializeGlobalConfigs`(`:`:Stream`&`): [%00-00 void_][%00-00* "
"SerializeGlobalConfigs][%00-00 (Stream`&_][%00-00*@3 s][")
TOPIC_TEXT(
"%00-00 )]&][s6; Serializes all global configuration data from/to single stream.&][s1; "
"[%00-00*C@3 s]-|Stream.&][s0;3 ]")
