TITLE("Index")
REF("::Index::class")
REF("::Index::Pop()")
REF("::Index::Index()")
REF("::Index::Index(pick_::Index&)")
REF("::Index::operator=(pick_::Vector<T>&)")
REF("::Index::Index(const::Index&,int)")
REF("::Index::Index(pick_::Vector<T>&)")
REF("::Index::Index(const::Vector<T>&,int)")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2;~~~64; Index&][s10;~~~64; "
"template <class [*@4 T], class [*@4 HashFn] `= StdHash<[*@4 T]> >&][s0;3 &][s10;:`:`:Index`:`:class:~~~64;* "
"[* class ]Index&][s0; &][s12; [*C@4 T]-|Type of elements stored in Index. T is required "
"to be [*/^topic`:`/`/Core`/")
TOPIC_TEXT(
"srcdoc`/Moveable`$en`-us^ moveable][*/  ]and must have either [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ "
"deep copy constructor], [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick constructor] "
"or [*/ default constructor].&][s12; [*C@4 HashFn]-|Hashing class. Must have [*C unsigned "
"operator()(const T`& x)] method defined, returning hash value for elements. Defaults "
"to[*  ][*C StdHash<T>] which req")
TOPIC_TEXT(
"uires [*C unsigned GetHashValue(const T`&)][C  ]function returning hash value of "
"elements to be defined.&][s3; Base class&][s9; [^topic`:`/`/Core`/src`/AIndex`$en`-us^ "
"AIndex<T, Vector<T>, HashFn> ] &][s5; &][s5; Vector flavor of index. Inherits most "
"of its functionality from [^topic`:`/`/Core`/src`/AIndex`$en`-us^ AIndex] and adds "
"only members specific for its flavor.&][s5; Like any other NTL con")
TOPIC_TEXT(
"tainer, Index is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/  "
"]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick and optional deep copy] "
"transfer semantics. Calling methods of picked Index is logic error with exception "
"of&][s0;/+75 [/C void ][*/C operator`=][/C (pick`_ Index`& ][*/C@3 v][/C ) ](defined "
"by composition)&][s0;/+75 [/C void ][*/C operator<<`=][/C (const AIn")
TOPIC_TEXT(
"dex`& ][*/C@3 v][/C )][/  ](defined in AIndex)&][s0;C+75 void [* Clear]()&][s0;C+75 "
"bool [* IsPicked]() const&][s5; Optional deep copy operator is inherited from AIndex "
"class. Pick operator is implicitly defined by composition.&][s3; Members&][s0;3 &][s4;:`:`:Index`:`:Pop`(`): "
"T [* Pop]()&][s6; Drops last element of the Index and returns its value.&][s11; Requires "
"T to have deep copy constructor.&")
TOPIC_TEXT(
"][s1; [*/ Return value]-|Value of the dropped element.&][s0;3 &][s4;:`:`:Index`:`:Index`(`): "
"[* Index]()&][s6; Constructs empty Index.&][s0;3 &][s4;:`:`:Index`:`:Index`(pick`_`:`:Index`&`): "
"[* Index](pick`_ Index`& [*@3 s])&][s6; Pick constructor. Transfers source Index "
"in low constant time, but destroys it by picking.&][s1; [*C@3 s]-|Source Index.&][s0;3 "
"&][s4;:`:`:Index`:`:operator`=`(pick`_`:`:")
TOPIC_TEXT(
"Vector`<T`>`&`): Index`& [* operator]`=(pick`_ Vector<T>`& [*@3 x])&][s6; Pick operator. "
"Transfers source Vector to Index in low constant time, but destroys it by picking.&][s1; "
"[*C@3 x]-|Source Vector.&][s0;3 &][s4;:`:`:Index`:`:Index`(const`:`:Index`&`,int`): "
"[* Index](const Index`& [*@3 s], int)&][s6; Optional deep copy constructor.&][s11; "
"Requires T to have deep copy constructor or optional de")
TOPIC_TEXT(
"ep copy constructor.&][s1; [*C@3 s]-|Source Index.&][s0;3 &][s4;:`:`:Index`:`:Index`(pick`_`:`:Vector`<T`>`&`): "
"[* Index](pick`_ Vector<T>`& [*@3 s])&][s6; Pick`-constructs Index from Vector. Transfers "
"source container in low constant time, but destroys it by picking.&][s1; [*C@3 s]-|Source "
"Vector.&][s0;3 &][s4;:`:`:Index`:`:Index`(const`:`:Vector`<T`>`&`,int`): [* Index](const "
"Vector<T>`& [*@3 s]")
TOPIC_TEXT(
", int)&][s6; Deep`-copy constructs Index from Vector.&][s11; Requires T to have "
"deep copy constructor or optional deep copy constructor.&][s1; [*C@3 s]-|Source Vector.&][s0;3 "
"]")
