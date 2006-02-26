TITLE("ArrayMap")
REF("::ArrayMap::class")
REF("::ArrayMap::Add(const K&,const T&)")
REF("::ArrayMap::Add(const K&)")
REF("::ArrayMap::Add(const K&,T*)")
REF("::ArrayMap::Set(int,T*)")
REF("::ArrayMap::PopDetach()")
REF("::ArrayMap::ArrayMap(const::ArrayMap&,int)")
REF("::ArrayMap::ArrayMap(pick_::Index<K,HashFn>&,pick_::Array<T>&)")
REF("::ArrayMap::ArrayMap(pick_::Vector<K>&,pick_::Array<T>&)")
REF("::ArrayMap::ArrayMap()")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2;:`:`:ArrayMap`:`:class: "
"ArrayMap&][s10; &][s10; template <class [*@4 K], class [*@4 T], class [*@4 HashFn] "
"`= StdHash<[*@4 K]> >&][s0;3 &][s10;:`:`:ArrayMap`:`:class:* [* class ]ArrayMap&][s5; "
"&][s12; [*C@4 K]-|Type of keys. K must have deep copy co")
TOPIC_TEXT(
"nstructor, be [*/^dpp`:`/`/SourceDoc`/Containers`/Moveable^ moveable] and must have "
"[*C operator`=`=] defined.&][s12; [*C@4 T]-|Type or base class of the values stored "
"in ArrayMap. There is no common requirement for T.&][s12; [*C@4 HashFn]-|Hashing "
"class. Must have defined [*C unsigned operator()(const K`& x)] method returning hash "
"value for elements.&][s3;~~~.416; Base classes&][s9;^topic`:`/`/Co")
TOPIC_TEXT(
"re`/src`/AMap`:EN`-US^ AMap< K, T, Array<T>, HashFn >&][s0; &][s5;~~~.416; Array "
"flavor of map. Inherits most of its functionality from [^topic`:`/`/Core`/src`/AMap`$en`-us^ "
"AMap] and adds only members specific for its flavor.&][s5;~~~.416; Like any other "
"NTL container, ArrayMap is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/ "
" ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us")
TOPIC_TEXT(
"^ pick and optional deep copy] transfer semantics. Calling methods of a picked VectorMap "
"is logic error with the exceptions of&][s0;/+75 [/C void ][*/C operator`=][/C (pick`_ "
"ArrayMap`& ][*/C@3 v][/C ) ](defined by composition)&][s0;/+75 [/C void ][*/C operator<<`=][/C "
"(const AMap`& ][*/C@3 v][/C )][/  ](defined in AMap)&][s0;C+75 void [* Clear]()&][s0;C+75 "
"bool [* IsPicked]() const&][s5; Optional")
TOPIC_TEXT(
" deep copy operator is inherited from AMap class. Pick operator is implicitly defined "
"by composition.&][s3; Members&][s0;3 &][s4;:`:`:ArrayMap`:`:Add`(const K`&`,const "
"T`&`): void [* Add](const K`& [*@3 k], const T`& [*@3 x])&][s6; Adds a key and value "
"pair to the ArrayMap. This method has to be reimplemented in ArrayMap (using simple "
"forwarding) due to overloading of [* Add] in other forms.&][s11")
TOPIC_TEXT(
"; T must have deep copy constructor.&][s11; Invalidates iterators to the ArrayMap.&][s11; "
"Invalidates references to keys.&][s1; [*C@3 k]-|Key.&][s1; [*C@3 x]-|Value.&][s0;3 "
"&][s4;:`:`:ArrayMap`:`:Add`(const K`&`): T`& [* Add](const K`& [*@3 k])&][s6; Adds "
"a key to the ArrayMap and returns a reference to the corresponding default constructed "
"value. This method has to be reimplemented in ArrayMap (u")
TOPIC_TEXT(
"sing simple forwarding) due to overloading of [* Add] in other forms.&][s11; T must "
"have default constructor.&][s11; Invalidates iterators to the ArrayMap.&][s11; Invalidates "
"references to keys.&][s1; [*C@3 k]-|Key.&][s1; [*/ Return value]-|Reference to value.&][s0;3 "
"&][s4;:`:`:ArrayMap`:`:Add`(const K`&`,T`*`): void [* Add](const K`& [*@3 k], T `*[*@3 "
"ptr])&][s6; Adds a key`-value pair to the Arr")
TOPIC_TEXT(
"ayMap. The value is specified by a pointer to the object. ArrayMap takes over ownership "
"of this object. This variant allows use of ArrayMap as polymorphic container, because "
"type of added element can be also derived from T as well. No constructor is applied.&][s1; "
"[*C@3 k]-|Key.&][s1; [*C@3 ptr]-|Value.&][s0;3 &][s4;:`:`:ArrayMap`:`:Set`(int`,T`*`): "
"void_[* Set](int_[*@3 i], T_`*[*@3 ptr])&][s6; S")
TOPIC_TEXT(
"ets value at specified index. Value is specified by a pointer to the object. ArrayMap "
"takes over ownership of this object.&][s1; [*C@3 i]-|Index.&][s1; [*C@3 ptr]-|Value.&][s0;3 "
"&][s4;:`:`:ArrayMap`:`:PopDetach`(`): T_`*[* PopDetach]()&][s6; Drops last element "
"in the ArrayMap, giving up ownership of value. Client is responsible for deletion "
"of the element.&][s11; Invalidates iterators to ArrayMap.")
TOPIC_TEXT(
"&][s1; [* Return value]-|Value.&][s0;3 &][s4;:`:`:ArrayMap`:`:ArrayMap`(const`:`:ArrayMap`&`,int`): "
"[* ArrayMap](const ArrayMap`& [*@3 s], int)&][s6; Optional deep copy constructor.&][s6;*1 "
"Requires T to have deep copy constructor or optional deep copy constructor if Array "
"stores only objects of type T.&][s11; Requires polymorphic deep copy if Array also "
"stores objects of type derived from T.&][s1")
TOPIC_TEXT(
"; [*C@3 s]-|Source ArrayMap.&][s0;3 &][s4;:`:`:ArrayMap`:`:ArrayMap`(pick`_`:`:Index`<K`,HashFn`>`&`,pick`_`:`:Array`<T`>`&`): "
"[* ArrayMap](pick`_ Index<K>`& [*@3 ndx], pick`_ Array<T>`& [*@3 val])&][s6; Pick`-constructs "
"ArrayMap from Index of keys and Array of values. The source containers should have "
"equal number of elements.&][s1; [*C@3 ndx]-|Source Index.&][s1; [*C@3 val]-|Source "
"Array.&][s0;3")
TOPIC_TEXT(
" &][s4;:`:`:ArrayMap`:`:ArrayMap`(pick`_`:`:Vector`<K`>`&`,pick`_`:`:Array`<T`>`&`): "
"[* ArrayMap](pick`_ Vector<K>`& [*@3 ndx], pick`_ Array<T>`& [*@3 val])&][s6; Pick`-constructs "
"ArrayMap from Vector of keys and Array of values. The source containers should have "
"equal number of elements.&][s1; [*C@3 ndx]-|Source Index.&][s1; [*C@3 val]-|Source "
"Array.&][s0;3 &][s4;:`:`:ArrayMap`:`:ArrayMap`(`): [*")
TOPIC_TEXT(
" ArrayMap]()&][s6; Default constructor. Creates empty ArrayMap.]")
