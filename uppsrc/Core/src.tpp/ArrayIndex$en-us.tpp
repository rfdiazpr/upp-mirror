TITLE("ArrayIndex")
REF("::ArrayIndex::class")
REF("::ArrayIndex::Add(const T&,unsigned)")
REF("::ArrayIndex::Add(const T&)")
REF("::ArrayIndex::Set(int,const T&,unsigned)")
REF("::ArrayIndex::Set(int,const T&)")
REF("::ArrayIndex::Add(T*,unsigned)")
REF("::ArrayIndex::Add(T*)")
REF("::ArrayIndex::Set(int,T*,unsigned)")
REF("::ArrayIndex::Set(int,T*)")
REF("::ArrayIndex::ArrayIndex()")
REF("::ArrayIndex::ArrayIndex(pick_::ArrayIndex&)")
REF("::ArrayIndex::ArrayIndex(const::ArrayIndex&,int)")
REF("::ArrayIndex::ArrayIndex(pick_::Array<T>&)")
REF("::ArrayIndex::ArrayIndex(const::Array<T>&,int)")
REF("::ArrayIndex::operator=(pick_::Array<T>&)")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2;:`:`:ArrayIndex`:`:class:~~~64; "
"ArrayIndex&][s10;~~~64; template <class [*@4 T], class [*@4 HashFn] `= StdHash<[*@4 "
"T]> >&][s0;3 &][s10;:`:`:ArrayIndex`:`:class:~~~64;* [* class ]ArrayIndex&][s0; &][s12;i480;~~~.480; "
"[*C@4 T]-|Type or base class of ")
TOPIC_TEXT(
"elements stored in Array. There is no common requirement for T.&][s12;i480;~~~.480; "
"[*C@4 HashFn]-|Hashing class. Must have defined [*C unsigned operator()(const T`& "
"x)] method returning hash value for elements. Defaults to[*  ][*C StdHash<T>] which "
"requires [*C unsigned GetHashValue(const T`&)][C  ]function returning hash value "
"of elements to be defined.&][s3; Base class&][s9;^topic`:`/`/Core`/sr")
TOPIC_TEXT(
"c`/AIndex`$en`-us^ AIndex<T, Array<T>, HashFn>&][s0; &][s5; Array flavor of index. "
"Inherits most of its functionality from [^topic`:`/`/Core`/src`/AIndex`$en`-us^ AIndex] "
"and adds only members specific for its flavor.&][s5; Like any other NTL container, "
"ArrayIndex is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/  ]type "
"with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick and opti")
TOPIC_TEXT(
"onal deep copy] transfer semantics. Calling methods of a picked ArrayIndex is logic "
"error with the exceptions of&][s0;/+75 [/C void ][*/C operator`=][/C (pick`_ ArrayIndex`& "
"][*/C@3 v][/C ) ](defined by composition)&][s0;/+75 [/C void ][*/C operator<<`=][/C "
"(const AIndex`& ][*/C@3 v][/C )][/  ](defined in AIndex)&][s0;C+75 void [* Clear]()&][s0;C+75 "
"bool [* IsPicked]() const&][s5; Optional deep co")
TOPIC_TEXT(
"py operator is inherited from AIndex class. Pick operator is implicitly defined "
"by composition.&][s0;3 &][s4;:`:`:ArrayIndex`:`:Add`(const T`&`,unsigned`): void "
"[* Add](const T`& [*@3 x], unsigned [*@3 `_hash])&][s6; Adds a new element with a "
"precomputed hash value. The precomputed hash value must be the same as the hash value "
"that would be the result of HashFn. The benefit of using a precomputed ")
TOPIC_TEXT(
"hash value is that sometimes you can compute hash`-value as the part of an other "
"process, like fetching a string from an input stream. This method has to be reimplemented "
"in ArrayIndex (using simple forwarding) due to overloading of [* Add] in other forms.&][s11; "
"Requires T to have deep copy constructor.&][s11; Invalidates iterators to the ArrayIndex.&][s1; "
"[*C@3 x]-|Element to add.&][s1; [*C@3 `_")
TOPIC_TEXT(
"hash]-|Precomputed hash value.&][s0;3 &][s4;:`:`:ArrayIndex`:`:Add`(const T`&`): "
"void [* Add](const T`& [*@3 x])&][s6; Adds a new element to Index. This method has "
"to be reimplemented in ArrayIndex (using simple forwarding) due to overloading of "
"[* Add] in other forms.&][s11; Requires T to have deep copy constructor.&][s11; Invalidates "
"iterators to the ArrayIndex.&][s1; [*C@3 x]-|Element to add.&]")
TOPIC_TEXT(
"[s0;3 &][s4;:`:`:ArrayIndex`:`:Set`(int`,const T`&`,unsigned`): void [* Set](int "
"[*@3 i], const T`& [*@3 x], unsigned [*@3 `_hash])&][s6; Replaces the element at "
"the specified position with a new element with the specified value, using a precomputed "
"hash`-value. Speed of this operation depends on the total number of elements with "
"the same value as the specified one in ArrayIndex. This method has t")
TOPIC_TEXT(
"o be reimplemented in ArrayIndex (using simple redirection) due to overloading of "
"[* Set] in other forms.&][s11; Requires T to have deep copy constructor.&][s11; Invalidates "
"iterators to the ArrayIndex.&][s1; [*C@3 i]-|Position of element.&][s1; [*C@3 x]-|Value "
"to set.&][s1; [*C@3 `_hash]-|Precomputed hash value.&][s0;3 &][s4;:`:`:ArrayIndex`:`:Set`(int`,const "
"T`&`): void [* Set](int [*@3 i], cons")
TOPIC_TEXT(
"t T`& [*@3 x])&][s6; Replaces element at specified position with new element with "
"specified value. Speed of this operation depends on the total number of elements "
"with the same value as the specified one in ArrayIndex. This method has to be reimplemented "
"in ArrayIndex (using simple redirection) due to overloading of [* Set] in other forms.&][s11; "
"Requires T to have deep copy constructor.&][s11; In")
TOPIC_TEXT(
"validates iterators to the ArrayIndex.&][s1; [*C@3 i]-|Position of element.&][s1; "
"[*C@3 x]-|Value to set.&][s0;3 &][s4;:`:`:ArrayIndex`:`:Add`(T`*`,unsigned`): void "
"[* Add](T `*[*@3 newt], unsigned [*@3 `_hash])&][s6; Adds a new element created on "
"the heap to the ArrayIndex using a precomputed hash value. The element is specified "
"by a pointer to the object. ArrayIndex takes over ownership of the p")
TOPIC_TEXT(
"ointed element. This variant allows the use of an ArrayIndex as a polymorphic container, "
"because the type of the added element can be either T or a type derived from T. No "
"constructor is applied. &][s1; [*C@3 newt]-|Element created on the heap.&][s0;3 &][s4;:`:`:ArrayIndex`:`:Add`(T`*`): "
"void [* Add](T `*[*@3 newt])&][s6; Adds a new element to the ArrayIndex. The element "
"is specified by a pointer ")
TOPIC_TEXT(
"to the object. ArrayIndex takes over ownership of the pointed element. This variant "
"allows the use of an ArrayIndex as a polymorphic container, because the type of the "
"added element can be either T or a type derived from T. No constructor is applied. "
"&][s1; [*C@3 newt]-|Object to be added.&][s0;3 &][s4;:`:`:ArrayIndex`:`:Set`(int`,T`*`,unsigned`): "
"void [* Set](int [*@3 i], T `*[*@3 newt], unsigned")
TOPIC_TEXT(
" [*@3 `_hash])&][s6; &][s1; [*C@3 i]-|Position.&][s1; [*C@3 newt]-|New element created "
"on heap.&][s1; [*C@3 `_hash]-|Hash value&][s0;3 &][s4;:`:`:ArrayIndex`:`:Set`(int`,T`*`): "
"void [* Set](int [*@3 i], T `*[*@3 newt])&][s6; Replaces the element at the specified "
"position by an element previously created on the heap. ArrayIndex takes over ownership "
"of the element.&][s0; &][s1; [*C@3 i]-|Position.&]")
TOPIC_TEXT(
"[s1; [*C@3 newt]-|New element created on heap.&][s1; [*/ Return value]-|&][s0;3 "
"&][s4;:`:`:ArrayIndex`:`:ArrayIndex`(`): [* ArrayIndex]()&][s6; Constructor. Constructs "
"an empty ArrayIndex.&][s0;3 &][s4;:`:`:ArrayIndex`:`:ArrayIndex`(pick`_`:`:ArrayIndex`&`): "
"[* ArrayIndex](pick`_ [* ArrayIndex]`& [*@3 s])&][s6; Pick constructor. Transfers "
"source Index in low constant time, but destroys it by picki")
TOPIC_TEXT(
"ng.&][s1; [*C@3 s]-|Source ArrayIndex.&][s0;3 &][s4;:`:`:ArrayIndex`:`:ArrayIndex`(const`:`:ArrayIndex`&`,int`): "
"[* ArrayIndex](const [* ArrayIndex]`& [*@3 s], int)&][s6; Optional deep copy constructor.&][s11; "
"Requires T to have deep copy constructor or optional deep copy constructor.&][s1; "
"[*C@3 s]-|Source ArrayIndex.&][s0;3 &][s4;:`:`:ArrayIndex`:`:ArrayIndex`(pick`_`:`:Array`<T`>`&`): "
"[* ArrayI")
TOPIC_TEXT(
"ndex](pick`_ [* Array]<T>`& [*@3 s])&][s6; Pick`-constructs ArrayIndex from an Array. "
"Transfers source container in low constant time, but destroys it by picking.&][s1; "
"[*C@3 s]-|Source Array.&][s0;3 &][s4;:`:`:ArrayIndex`:`:ArrayIndex`(const`:`:Array`<T`>`&`,int`): "
"[* ArrayIndex](const [* Array]<T>`& [*@3 s], int)&][s6; Deep`-copy constructs ArrayIndex "
"from Array.&][s11; Requires T to have deep c")
TOPIC_TEXT(
"opy constructor or optional deep copy constructor.&][s1; [*C@3 s]-|Source Vector.&][s0;3 "
"&][s4;:`:`:ArrayIndex`:`:operator`=`(pick`_`:`:Array`<T`>`&`): ArrayIndex`& [* operator]`=(pick`_ "
"Array<T>`& [*@3 x])&][s6; Pick operator. Transfers source Array to the ArrayIndex, "
"but destroys it by picking.&][s1; [*C@3 x]-|Source Array.]")
