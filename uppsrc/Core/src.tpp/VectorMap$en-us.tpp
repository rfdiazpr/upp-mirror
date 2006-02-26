TITLE("VectorMap")
REF("::VectorMap::class")
REF("::VectorMap::Pop()")
REF("::VectorMap::VectorMap(const::VectorMap&,int)")
REF("::VectorMap::VectorMap(pick_::Index<K,HashFn>&,pick_::Vector<T>&)")
REF("::VectorMap::VectorMap(pick_::Vector<K>&,pick_::Vector<T>&)")
REF("::VectorMap::VectorMap()")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2;:`:`:VectorMap`:`:class: "
"VectorMap&][s10; template <class [*@4 K], class [*@4 T], class [*@4 HashFn] `= StdHash<[*@4 "
"K]> >&][s10;* [* class ]VectorMap&][s5; &][s12; [*C@4 K]-|Type of keys. K must have "
"deep copy constructor, be [*/^topic`:`/`/Core`/s")
TOPIC_TEXT(
"rcdoc`/Moveable`$en`-us^ moveable] and must have [*C operator`=`=] defined.&][s12; "
"[*C@4 T]-|Type of elements stored in Vector. T is required to be [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ "
"moveable][*/  ]and must have either [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ "
"deep copy constructor], [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick constructor] "
"or [*/ default constructor].&][s12; [")
TOPIC_TEXT(
"*C@4 HashFn]-|Hashing class. Must have defined [*C unsigned operator()(const K`& "
"x)] method returning hash value for elements.&][s3; Base classes&][s9; AMap< K, T, "
"Vector<T>, HashFn >  &][s0; &][s5; Vector flavor of map. Inherits most of its functionality "
"from [^topic`:`/`/Core`/src`/AMap`$en`-us^ AMap] and adds only members specific for "
"its flavor.&][s5; Like any other NTL container, VectorMap is")
TOPIC_TEXT(
" [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ "
"pick and optional deep copy] transfer semantics. Calling methods of picked VectorMap "
"is logic error with exception of&][s0;/+75 [/C void ][*/C operator`=][/C (pick`_ "
"VectorMap`& ][*/C@3 v][/C ) ](defined by composition)&][s0;/+75 [/C void ][*/C operator<<`=][/C "
"(const AMap`& ][*/C@")
TOPIC_TEXT(
"3 v][/C )][/  ](defined in AMap)&][s0;C+75 void [* Clear]()&][s0;C+75 bool [* IsPicked]() "
"const&][s5; Optional deep copy operator is inherited from AMap class. Pick operator "
"is implicitly defined by composition.&][s3; Members&][s0;3 &][s4;:`:`:VectorMap`:`:Pop`(`): "
"T [* Pop]()&][s6; Drops last element of VectorMap and returns its value.&][s1; [*/ "
"Return value]-|Value of dropped element.&][s0;3 &][")
TOPIC_TEXT(
"s4;:`:`:VectorMap`:`:VectorMap`(const`:`:VectorMap`&`,int`): [* VectorMap](const "
"VectorMap`& [*@3 s], int)&][s6; Optional deep copy constructor.&][s6;*1 Requires "
"T to have deep copy constructor or optional deep copy constructor.&][s1; [*C@3 s]-|Source "
"VectorMap.&][s0;3 &][s4;:`:`:VectorMap`:`:VectorMap`(pick`_`:`:Index`<K`,HashFn`>`&`,pick`_`:`:Vector`<T`>`&`): "
"[* VectorMap](pick`_ Index<K>`& [*@3")
TOPIC_TEXT(
" ndx], pick`_ Vector<T>`& [*@3 val])&][s6; Pick`-constructs VectorMap from Index "
"of keys and Vector of values. Both source containers should have the same number "
"of elements.&][s1; [*C@3 ndx]-|Source Index.&][s1; [*C@3 val]-|Source Vector.&][s0;3 "
"&][s4;:`:`:VectorMap`:`:VectorMap`(pick`_`:`:Vector`<K`>`&`,pick`_`:`:Vector`<T`>`&`): "
"[* VectorMap](pick`_ Vector<K>`& [*@3 ndx], pick`_ Vector<T>`& [*@")
TOPIC_TEXT(
"3 val])&][s6; Pick`-constructs VectorMap from Vector of keys and Vector of values. "
"Both source containers should have the same number of elements.&][s1; [*C@3 ndx]-|Source "
"Vector of keys.&][s1; [*C@3 val]-|Source Vector of values.&][s0;3 &][s4;:`:`:VectorMap`:`:VectorMap`(`): "
"[* VectorMap]()&][s6; Default constructor. Constructs empty VectorMap.]")
