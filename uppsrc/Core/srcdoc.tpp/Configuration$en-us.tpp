TITLE("About storing configuration")
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
"[s2;+184 About storing")
TOPIC_TEXT(
" configuration&][s0; If you want to go on using registry functions (we don`'t like "
"them very much, honestly), you`'ll find their wrappers in Core/Win32Com.h:&][s0; "
"&][s7; [%00-00 String GetWinRegString(const char `*value, const char `*path, HKEY "
"base`_key `=]&][s7; [%00-00 HKEY`_LOCAL`_MACHINE);]&][s7; [%00-00 int    GetWinRegInt(const "
"char `*value, const char `*path, HKEY base`_key `=]&][s7; [%00")
TOPIC_TEXT(
"-00 HKEY`_LOCAL`_MACHINE);]&][s7; [%00-00 bool   SetWinRegString(const String`& "
"string, const char `*value, const char]&][s7; [%00-00 `*path, HKEY base`_key `= HKEY`_LOCAL`_MACHINE);]&][s7; "
"[%00-00 bool   SetWinRegInt(int data, const char `*value, const char `*path, HKEY]&][s7; "
"[%00-00 base`_key `= HKEY`_LOCAL`_MACHINE);]&][s7; [%00-00 void   DeleteWinReg(const "
"String`& key, HKEY base `= HKEY`_LOC")
TOPIC_TEXT(
"AL`_MACHINE);]&][s0; &][s0; However if you want to use your own configuration files, "
"you have basically two possibilities:&][s0; &][s0; 1) text`-oriented configuration "
"(something like key`=value pairs, or perhaps some xml)&][s0; 2) binary configuration&][s0; "
"&][s0; Text`-oriented configuration is somewhat `'cleaner`' in the sense that you "
"(or anyone else) can look into the file and possibly find o")
TOPIC_TEXT(
"ut what went wrong, add or modify its contents using a simple text editor. Sometimes "
"this is useful, sometimes this is exactly what we try to avoid. It depends on your "
"context. Binary configuration, on the other hand, is much simpler to generate / parse, "
"because the data can directly flow between the configuration and the program objects "
"using the serialization functions.&][s0; &][s3; 1) Text conf")
TOPIC_TEXT(
"iguration&][s0; &][s0; If you want simple key`=value pairs, you could use the function&][s0; "
"&][s7; VectorMap<String, String> LoadIniFile(const char `*filename);&][s0; &][s0; "
"defined in Core/Util.cpp. For some strange reason its declaration is missing in Core/Util.h "
"but I`'ll fix that immediately. This function parses a given file and turns it into "
"a key`-value map. In an application, you can then")
TOPIC_TEXT(
" write e.g.:&][s0; &][s7; VectorMap<String, String> cfg `= LoadIniFile(`\"myapp.cfg`\");&][s7; "
"String recentdir `= cfg.Get(`\"RECENTDIR`\", Null);&][s7; int id `= ScanInt(cfg.Get(`\"ID`\", "
"Null));&][s0; etc.&][s0; &][s0; You have to write such file manually, but it`'s no "
"big problem, just do something like&][s0; &][s7; String cfg;&][s7; cfg << `\"RECENTDIR`=`\" "
"<< appobj.recentdir << `\"`\\n`\"&][s7; -|`\"ID`")
TOPIC_TEXT(
"=`\" << appobj.id << `\"`\\n`\";&][s7; if(!SaveFile(`\"myapp.cfg`\", cfg))&][s7; "
"-|Exclamation(`\"Error saving configuration!`\");&][s0; &][s0; Of course the situation "
"would get much more complicated if you wanted to stored more complex data in the "
"configuration. Basically you have to parse the file by yourself, but you can make "
"use of many text parsing utilities present in uppsrc (like CParser or XMLPars")
TOPIC_TEXT(
"er).&][s0; &][s0; 2) Binary configuration&][s0; &][s0; For the sake of simplicity "
"let`'s initially assume you keep the whole configuration in a single object cfgobj "
"of type AppConfig. Your main application function could then look something like:&][s0; "
"&][s7; GUI`_APP`_MAIN&][s7; `{&][s7;     String cfgfile `= ConfigFile();&][s7;   "
"  AppConfig cfgobj;&][s7;     if(FileExists(cfgfile) `&`& !LoadFro")
TOPIC_TEXT(
"mFile(cfgobj, cfgfile))&][s7;         Exclamation(`\"Error loading configuration "
"file!`\");&][s7;     RunMyApp(cfgobj);&][s7;     if(!StoreToFile(cfgobj, cfgfile))&][s7; "
"        Exclamation(`\"Error updating configuration file!`\");&][s7; `}&][s0; &][s0; "
"Note the functions LoadFromFile and StoreToFile, which use the AppConfig`::Serialize "
"method to transfer the data between the object and its linear (s")
TOPIC_TEXT(
"tream) representation. You have to implement this function and so define how the "
"object is to be linearized. For example:&][s0; &][s7; struct AppConfig `{&][s7;  "
"   AppConfig(); // you usually want to initialize the default configuration&][s7; "
"here&][s7; &][s7;     void Serialize(Stream`& stream);&][s7; &][s7;     String recentdir;&][s7; "
"    int id;&][s7; `};&][s7; &][s7; void AppConfig`::Serializ")
TOPIC_TEXT(
"e(Stream`& stream)&][s7; `{&][s7;     stream % recentdir % id;&][s7; `}&][s7; &][s0; "
"This is sufficient to make the basic thing working. If you wanted to dig deeper into "
"this, you can try to take a look at the following:&][s0; &][s0; a) There are many "
"utility functions for serialization, see e.g. Core/Util.h, functions like Load, Store, "
"LoadFromString / File, StoreToString / File.&][s0; &][s0; b) ")
TOPIC_TEXT(
"Some support is present for the so called `\"distributed configuration`\" where "
"different parts of the application register their own configuration objects, not "
"necessarily knowing about configuration objects belonging to different parts of the "
"application. See Core/Util.h, functions&][s0; &][s7; void             RegisterGlobalConfig(const "
"char `*name);&][s7; void             RegisterGlobalConfig(co")
TOPIC_TEXT(
"nst char `*name, Callback WhenFlush);&][s7; String`&          GlobalConfigData(const "
"char `*name);&][s7; CriticalSection`& GlobalConfigLock();&][s7; bool LoadFromGlobal(T`& "
"x, const char `*name);&][s7; void StoreToGlobal(T`& x, const char `*name);&][s7; "
"void SerializeGlobalConfigs(Stream`& s);&][s0;3 &][s0;3 ]")
