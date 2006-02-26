TITLE("Ultimate++ installation guide")
TOPIC_TEXT(
"[2 $$0,0#00000000000000000000000000000000:Default][l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param][a83;*R6 "
"$$2,5#31310162474203024125188417583966:caption][b83;* $$3,5#07864147445237544204411237157677:title][b167;a42;C2 "
"$$4,6#40027414424643823182269349404212:item][b42;a42;2 $$5,5#45413000475342174754091244180557:text][l288;a17;2 "
"$$6,6#27521748481378242620020725143825:desc][")
TOPIC_TEXT(
"l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code][b2503; $$8,0#65142375456100023862071332075487:separator][*@(0.0.255) "
"$$9,0#83433469410354161042741608181528:base][t4167;C $$10,0#37138531426314131251341829483380:class][l288;a17;*1 "
"$$11,11#70004532496200323422659154056402:requirement][i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam][b167;C2 "
"$$13,13#9243045944346")
TOPIC_TEXT(
"0461911108080531343:item1][i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2][*@2$(0.128.128) "
"$$15,15#34511555403152284025741354420178:NewsDate][l321;*C$7;2 $$16,16#03451589433145915344929335295360:result][l321;b83;a83;*C$7;2 "
"$$17,17#07531550463529505371228428965313:result`-line][l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title][{_}%EN-US "
"[s0;=*+184 Ultimate`+`+")
TOPIC_TEXT(
" Win32 installation guide&][s0;3 &][s0; You can use Ultimate`+`+ with either MinGW "
"compiler system or with Microsoft[*`+75 \302\256] free C`+`+ compiler `- Microsoft[*` "
"\302\256] Visual C`+`+\342\204\242 Toolkit 2003 (recommended).&][s0; &][s0; If you "
"want TheIDE to perform automatic setup of build methods for these compilers, you "
"should install them before continuing this dialog (if you press exit now, automatic "
"setu")
TOPIC_TEXT(
"p will be performed next time you will launch TheIDE).&][s0; &][s0;*+117 MinGW&][s0; "
"&][s0; Using MinGW is quite easy `- all you need is to download it here:&][s0; &][s0;^http`:`/`/prdownloads`.sf`.net`/mingw`/MinGW`-3`.1`.0`-1`.exe`?download^1 "
"http://prdownloads.sf.net/mingw/MinGW`-3.1.0`-1.exe?download&][s0; &][s0; and install "
"it prior continuing this dialog. TheIDE should then detect MinGW dire")
TOPIC_TEXT(
"ctory and install and setup appropriate build method.&][s0; &][s0; Ultimate`+`+ "
"also contains more effective mingw linker and librarian replacements (ld.exe and "
"ar.exe files in mingw/bin directory). You can compile them yourself `- they come "
"in uppsrc/coff/uld and uppsrc/coff/ar packages. Backup mingw originals and put new "
"compiled replacements to bin directory.&][s0; &][s0; If you have downloaded")
TOPIC_TEXT(
" upp`-mingw release, you do not have to download/install/compile anything, as that "
"release already comes with complete mingw C`+`+ compiler including our linker.&][s0; "
"&][s0; Note: Recently we have encountered problems with mingw gdb from regular mingw "
"releases. That is why we have to recommend using our upp`-mingw package in case you "
"want to use mingw instead of separate download.&][s0; &][s0;*+1")
TOPIC_TEXT(
"17 Microsoft[` \302\256 ]Visual C`+`+[* \342\204\242] Toolkit 2003&][s0; &][s0; "
"Visual C`+`+ Toolkit is a little bit more tricky because you have to download and "
"install three different parts:&][s0; &][s0; Visual C`+`+ Toolkit itself:&][s0; &][s0;^http`:`/`/msdn`.microsoft`.com`/visualc`/vctoolkit2003`/^1 "
"http://msdn.microsoft.com/visualc/vctoolkit2003/&][s0; &][s0; Microsoft Win32 SDK "
"(installing minimum 32bit ")
TOPIC_TEXT(
"core is all you need for most projects):&][s0; &][s0;^http`:`/`/www`.microsoft`.com`/downloads`/details`.aspx`?FamilyId`=A55B6B43`-E24F`-4EA3`-A93E`-40C0EC4F68E5`&displaylang`=en^1 "
"http://www.microsoft.com/downloads/details.aspx?FamilyId`=A55B6B43`-E24F`-4EA3`-A93E`-40C0EC4F68E5`&displaylang`=en&][s0; "
"&][s0; And finally, there is missing cvtres.exe utility in Visual C`+`+ Toolkit, "
"you can download")
TOPIC_TEXT(
" it here:&][s0; &][s0;^http`:`/`/support`.microsoft`.com`/default`.aspx`?scid`=kb`;en`-us`;q187280^1 "
"http://support.microsoft.com/default.aspx?scid`=kb;en`-us;q187280&][s0; &][s0; and "
"then please unpack and install to the bin directory of toolkit. TheIDE should then "
"detect all required directories and setup build methods.&][s0; &][s0; Above steps "
"should install enough components needed to compile ")
TOPIC_TEXT(
"most of U`+`+ examples (and TheIDE itself), anyway some project will need more libraries "
"that were not included with toolkit. Please refer to this page:&][s0; &][s0;^http`:`/`/root`.cern`.ch`/root`/Procedure`/Procedure`%20to`%20install`%20the`%20free`%20Microsoft`%20Visual`%20C`.htm^1 "
"http://root.cern.ch/root/Procedure/Procedure%20to%20install%20the%20free%20Microsoft%20Visual%20C.htm&][s0;3 "
"&][s0")
TOPIC_TEXT(
"; Note that that with TheIDE you do not need to set up environment variables.&][s0;3 "
"]")
