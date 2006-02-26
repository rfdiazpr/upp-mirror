TITLE("Design decisions and tradeoffs")
TOPIC_TEXT(
"[2 $$0,0#00000000000000000000000000000000:Default][l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param][a83;*R6 "
"$$2,5#31310162474203024125188417583966:caption][b83;* $$3,5#07864147445237544204411237157677:title][i288;b167;a42;O9;C2 "
"$$4,6#40027414424643823182269349404212:item][b42;a42;2 $$5,5#45413000475342174754091244180557:text][l288;a17;2 "
"$$6,6#2752174848137824262002072514382")
TOPIC_TEXT(
"5:desc][l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code][b2503; $$8,0#65142375456100023862071332075487:separator][*@(0.0.255)2 "
"$$9,0#83433469410354161042741608181528:base][t4167;C $$10,0#37138531426314131251341829483380:class][l288;a17;*1 "
"$$11,11#70004532496200323422659154056402:requirement][i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam][b167;C2 "
"$$13,13#9243")
TOPIC_TEXT(
"0459443460461911108080531343:item1][i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2][*@2$(0.128.128) "
"$$15,15#34511555403152284025741354420178:NewsDate][l321;*C$7;2 $$16,16#03451589433145915344929335295360:result][l321;b83;a83;*C$7;2 "
"$$17,17#07531550463529505371228428965313:result`-line][l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title][{_}%EN-US "
"[s2; Design de")
TOPIC_TEXT(
"cisions and trade`-offs&][s0;i150;b33;O0; At the moment, we strongly favor static "
"linking of U`+`+ packages. If nothing else, it makes U`+`+ apps self`-contained (together "
"with system of .lay and .iml files) and also much faster to load in Linux (just compare "
"start times of UWord with some KDE based word`-processor). Shared builds were implemented "
"mainly to improve build times in debug mode.&][s0;")
TOPIC_TEXT(
"i150;b33;O0; CPU has to be either little`-endian or big`-endian (exotic architectures "
"are not supported), in flat 32`-bit mode or in 64`-bit mode with sizeof(int) `=`= "
"4 (that is quite standard) (Actually, at the moment, only little`-endian can be considered "
"as stable). Currently supported CPUs architectures are i386 and AMD64, support is "
"planned for PowerPC and ARM.&][s0;i150;b33;O0; CPU and comp")
TOPIC_TEXT(
"iler are supposed to support Moveable (true for any gcc platform and MSVC`+`+ as "
"well).&][s0;i150;b33;O0; We decided to ignore possibility of `\"out`-of`-memory`\" "
"exceptions and recovery. If U`+`+ application goes out of memory, it simply prints "
"the error message and terminates. This is quite pragmatic resolution `- our experience "
"is that it is quite hard and annoying to achieve robustness here and")
TOPIC_TEXT(
" it cannot be reliably tested. Also, most platforms with virtual memory will almost "
"freeze long before out`-of`-memory problem due to intensive page swapping. Connected "
"issue `- default and copy constructors are not allowed to throw exceptions in U`+`+ "
"(the common reason to throw exception here was out`-of`-memory condition).&][s0;i150;b33;O0; "
"Maximum number of elements in any of U`+`+ containers ")
TOPIC_TEXT(
"(including GUI elements, like number of rows in DropList) is 2[` 31 ]`- 1 `= [%00-00 "
"2,147,483,647.]&][s0;i150;b33;O0; GUI is single threaded. That does not mean you "
"cannot do multi`-threaded GUI apps `- just use only one thread to perform GUI and "
"communicate with others via timer queue (I guess this method is far safer anyway).&][s0;i150;b33;O0; "
"U`+`+ does not use OS`-native widgets. The main rea")
TOPIC_TEXT(
"son for this is to achieve widget interface model optimal for rapid development. "
"The less important reason is that this way it is much easier to make U`+`+ cross`-platform. "
"On the other hand, we are determined to achieve native look`&feel of emulated widgets "
"(already true on Windows platform).&][s0;3 &][s2; Multithreading guarantee&][s0; "
"U`+`+ is designed around a simple MT guarantee:&][s0; &][s0;")
TOPIC_TEXT(
"i150;O2; At one time, only a single thread is allowed to call methods of particular "
"instance. If two or more threads are about to [%00-00 work with the same instance], "
"they must serialize access to it via some form of lock like CriticalSection.&][s0; "
"&][s0; Note also that this includes both non`-const [*/ and const][/  ]methods, so "
"even if the method represents a reading operation, it still has to")
TOPIC_TEXT(
" be serialized (this allows objects to use mutable caches).&][s0; &][s0; This is "
"the minimal guarantee that is valid across most U`+`+ classes. Some of them however, "
"e.g. synchronization primitives, might provide higher level of MT guarantees.&][s0; "
"]")
