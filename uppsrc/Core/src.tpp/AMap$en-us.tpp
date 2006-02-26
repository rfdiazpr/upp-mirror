TITLE("AMap")
REF("::AMap::class")
REF("::AMap::Add(const K&,const T&)")
REF("::AMap::AddPick(const K&,pick_ T&)")
REF("::AMap::Add(const K&)")
REF("::AMap::FindAdd(const K&)")
REF("::AMap::Find(const K&)const")
REF("::AMap::Find(const K&,unsigned)const")
REF("::AMap::FindLast(const K&,unsigned)const")
REF("::AMap::FindLast(const K&)const")
REF("::AMap::FindPrev(int)const")
REF("::AMap::FindAdd(const K&,const T&)")
REF("::AMap::FindPutPick(const K&,pick_ T&)")
REF("::AMap::Unlink(int)")
REF("::AMap::Put(const K&,const T&)")
REF("::AMap::PutPick(const K&,pick_ T&)")
REF("::AMap::Put(const K&)")
REF("::AMap::FindPut(const K&)")
REF("::AMap::FindPut(const K&,const T&)")
REF("::AMap::Get(const K&)")
REF("::AMap::Get(const K&)const")
REF("::AMap::Get(const K&,const T&)const")
REF("::AMap::GetAdd(const K&)")
REF("::AMap::GetAdd(const K&,const T&)")
REF("::AMap::GetAddPick(const K&,pick_ T&)")
REF("::AMap::GetPut(const K&)")
REF("::AMap::GetPut(const K&,const T&)")
REF("::AMap::GetPutPick(const K&,pick_ T&)")
REF("::AMap::SetKey(int,const K&)")
REF("::AMap::FindPtr(const K&)")
REF("::AMap::FindPtr(const K&)const")
REF("::AMap::UnlinkKey(const K&,unsigned)")
REF("::AMap::UnlinkKey(const K&)")
REF("::AMap::IsUnlinked(int)const")
REF("::AMap::Insert(int,const K&)")
REF("::AMap::Insert(int,const K&,const T&)")
REF("::AMap::Remove(int)")
REF("::AMap::Remove(const int*,int)")
REF("::AMap::Remove(const::Vector<int>&)")
REF("::AMap::RemoveKey(const K&)")
REF("::AMap::operator[](int)const")
REF("::AMap::operator[](int)")
REF("::AMap::GetCount()const")
REF("::AMap::IsEmpty()const")
REF("::AMap::Clear()")
REF("::AMap::Shrink()")
REF("::AMap::Reserve(int)")
REF("::AMap::GetAlloc()const")
REF("::AMap::Drop(int)")
REF("::AMap::Top()")
REF("::AMap::TopKey()const")
REF("::AMap::PopKey()")
REF("::AMap::GetKey(int)const")
REF("::AMap::Serialize(::Stream&)")
REF("::AMap::GetIndex()const")
REF("::AMap::PickIndex()pick_")
REF("::AMap::GetKeys()const")
REF("::AMap::PickKeys()pick_")
REF("::AMap::GetValues()const")
REF("::AMap::PickValues()pick_")
REF("::AMap::AMap()")
REF("::AMap::AMap(const::AMap&,int)")
REF("::AMap::AMap(pick_::Index<K,HashFn>&,pick_ V&)")
REF("::AMap::AMap(pick_::Vector<K>&,pick_ V&)")
REF("::AMap::KeyType::typedef")
REF("::AMap::KeyConstIterator::typedef")
REF("::AMap::KeyBegin()const")
REF("::AMap::KeyEnd()const")
REF("::AMap::KeyGetIter(int)const")
REF("::AMap::ValueType::typedef")
REF("::AMap::ConstIterator::typedef")
REF("::AMap::Iterator::typedef")
REF("::AMap::Begin()")
REF("::AMap::End()")
REF("::AMap::GetIter(int)")
REF("::AMap::Begin()const")
REF("::AMap::End()const")
REF("::AMap::GetIter(int)const")
TOPIC_TEXT(
"[2 $$0,0#00000000000000000000000000000000:Default][l288;i1120;a17;O9;~~~.1408;2 "
"$$1,0#10431211400427159095818037425705:param][a83;*R6 $$2,5#31310162474203024125188417583966:caption][b83;*4 "
"$$3,5#07864147445237544204411237157677:title][i288;O9;C2 $$4,6#40027414424643823182269349404212:item][b42;a42;2 "
"$$5,5#45413000475342174754091244180557:text][l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc][l321;t246;C@5;1 "
"$$7,7#20902679421464641399138805415013:code][b2503; $$8,0#65142375456100023862071332075487:separator][*@(0.0.255)2 "
"$$9,0#83433469410354161042741608181528:base][t4167;C $$10,0#37138531426314131251341829483380:class][l288;a17;*1 "
"$$11,11#70004532496200323422659154056402:requirement][i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam][b167;C2 "
"$$13,13#92430459443460461911108080531343:item1][i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2][*@2$(0.128.128) "
"$$15,15#34511555403152284025741354420178:NewsDate][l321;*C$7;2 $$16,16#03451589433145915344929335295360:result][l321;b83;a83;*C$7;2 "
"$$17,17#07531550463529505371228428965313:result`-line][l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title][ "
"$$19,0#53580023442335529039900623488521:gap][t4167;C2 $$20,20#70211524482531209251820423858195:class`-nested][b50;2 "
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2;:`:`:AMap`:`:class:~~~64; "
"AMap&][s10;~~~64; template <class [*@4 K], class [*@4 T], class [*@4 V], class [*@4 "
"HashFn]>&][s0;3 &][s10;:`:`:AMap`:`:class:~~~64;* [* class ]AMap&][s0; &][s12; [*C@4 "
"K]-|Type of keys. K must have deep copy constructor, be [*/^dpp`:`/`/SourceDoc`/Containers`/Moveable^ "
"moveable] and must have [*C operator`=`=] defined.&][s12; [*C@4 T]-|Type of values. "
"T must satisfy requirements for container flavor identified by parameter V.&][s12; "
"[*C@4 V]-|Type of basic random access container.&][s12; [*C@4 HashFn]-|Hashing class. "
"Must have defined [*C unsigned operator()(const K`& x)] method returning hash value "
"for elements.&]")
TOPIC_TEXT(
"[s5;~~~.416; AMap is a class that combines Index of keys with basic random access "
"container of values, forming map flavors. It is used as base class for concrete map "
"flavors, [* VectorMap], [* ArrayMap] and [* SegtorMap].&][s5;~~~.416; Like any other "
"NTL container, AMap is [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/ "
" ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick and optional deep copy] "
"transfer semantics, although these features are more important in derived concrete "
"index flavors.&][s3; Members&][s0;3 &][s4;:`:`:AMap`:`:Add`(const K`&`,const T`&`): "
"void [* Add](const K`& [*@3 k], const T`& [*@3 x])&][s6; Adds a key and value pair "
"to the AMap.&][s11; T must have deep copy constructor.&][s11; Invalidates iterators "
"to AMap.&][s11; Invalidates references to keys.&][s11; Invalidates references to "
"VectorMap values.&][s1; [*C@3 k]-|Key.&][s1; [*C@3 x]-|Value.&][s0;3 &][s4;:`:`:AMap`:`:AddPick`(const "
"K`&`,pick`_ T`&`): void [* AddPick](const K`& [*@3 k], pick`_ T`& [*@3 x])&][s6; "
"Adds a key and value pair to the AMap. Value is transfered by pick constructor in "
"low constant time, but the source value is destroyed.&][s11; T must have pick constructor.&][s11; "
"Invalidates iterators to AMap.&][s11; Invalidates references to keys.&][s11; Invalidates "
"references to VectorMap values.&][s1; [*C@3 k]-|Key.&][s1; [*C@3 x]-|-|Value.&][s0;3 "
"&][s4;:`:`:AMap`:`:Add`(const K`&`): T`& [* Add](const K`& [*@3 k])&][s6; Adds a "
"key to the AMap and returns a reference to the corresponding default constructed "
"value.&][s11; T must have default constructor.&][s11; Invalidates iterators to AMap.&][s11; "
"Invalidates references to keys.&][s11; Invalidates references to VectorMap values.&][s1; "
"[*C@3 k]-|Key.&][s1; [*/ Return value]-|Reference to value.&][s0;3 &][s4;:`:`:AMap`:`:FindAdd`(const "
"K`&`): int [* Find](const K`& [*@3 k], unsigned [*@3 h]) const&][s6; Retrieves the "
"position of first element with the specified key in AMap, using a precomputed hash "
"value. The precom")
TOPIC_TEXT(
"puted hash value must be the same as the hash value that would be the result of "
"HashFn. If multi`-key ordering is not broken and more than one element with the same "
"value exists in AMap, the lowest position is retrieved. If the element does not exist "
"in AMap, a negative number is returned. Unlinked elements are ignored.&][s1; [*C@3 "
"x]-|Key to find.&][s1; [*C@3 h]-|Precomputed hash value.&][s1; [*/ Return value]-|Position "
"of element or a negative value if element is not in AMap.&][s0;3 &][s4;:`:`:AMap`:`:Find`(const "
"K`&`)const:~~~.992; int [* Find](const K`& [*@3 k]) const&][s6; Retrieves the position "
"of first element with the specified key in AMap. If multi`-key ordering is not broken "
"and more than one element with the same value exists in AMap, the lowest position "
"is retrieved. If the element does not exist in AMap, a negative number is returned. "
"Unlinked elements are ignored.&][s1; [*C@3 x]-|Key to find.&][s1; [*/ Return value]-|Position "
"of element or a negative value if element is not in AMap.&][s0;3 &][s4;:`:`:AMap`:`:Find`(const "
"K`&`,unsigned`)const: int [* FindNext](int [*@3 i]) const&][s6; Retrieves the position "
"of next element with the same key as element at the specified position. If multi`-key "
"ordering is not broken and more than one element with that value exists in AMap, "
"the lowest position greater than specified one is retrieved (so that positions got "
"by subsequent calls to FindNext are in ascending order). When there are no more elements "
"with required key, negative number is returned. Unlinked elements are ignored.&][s1; "
"[*C@3 i]-|Position of element.&][s1; [*/ Return value]-|Position of next element "
"with same value.&][s0;3 &][s4;:`:`:AMap`:`:FindLast`(const K`&`,unsigned`)const: "
"int [* FindLast](const K`& [*@3 k], unsigned [*@3 h]) const&][s6; Retrieves position "
"of last element with the specified key in AMap, using a precomputed hash value. The "
"precomputed hash value must be the same as the hash value that would be the result "
"of HashFn. If multi`-key ")
TOPIC_TEXT(
"ordering is not broken and more than one element with the same value exists in AMap, "
"the greatest position is retrieved. If element does not exist in AMap, a negative "
"number is returned. Unlinked elements are ignored.&][s1; [*C@3 x]-|Key to find.&][s1; "
"[*C@3 h]-|Precomputed hash value.&][s1; [*/ Return value]-|Position of element or "
"a negative value if element is not in AMap.&][s0;3 &][s4;:`:`:AMap`:`:FindLast`(const "
"K`&`)const: int [* FindLast](const K`& [*@3 k]) const&][s6; Retrieves the position "
"of last element with the specified key in AMap. If multi`-key ordering is not broken "
"and more than one element with the same value exists in AIndex, the greatest position "
"is retrieved. If element does not exist in AMap, a negative number is returned. Unlinked "
"elements are ignored.&][s1; [*C@3 x]-|Element to find.&][s1; [*/ Return value]-|Position "
"of element or a negative value if element is not in AMap.&][s0;3 &][s4;:`:`:AMap`:`:FindPrev`(int`)const: "
"int [* FindPrev](int [*@3 i]) const&][s6; Retrieves the position of previous element "
"with the same key as element at the specified position. If multi`-key ordering is "
"not broken and more than one element with that value exists in AMap, the greatest "
"position lower than specified one is retrieved (so that positions got by subsequent "
"calls to FindNext are in descending order). When there are no more elements with "
"required key, a negative number is returned. Unlinked elements are ignored.&][s1; "
"[*C@3 i]-|Position of element.&][s1; [*/ Return value]-|Position of previous element "
"with same value.&][s0;3 &][s4;:`:`:AMap`:`:FindAdd`(const K`&`):~~~.992; int [* FindAdd](const "
"K`& [*@3 k])&][s6; Retrieves the position of first element with the specified key "
"in AMap. If multi`-key ordering is not broken and more than one element with the "
"same value exists in AMap, lowest position is retrieved. If the element does not "
"exist in AMap, adds new default constructed element at the end of AMap and returns "
"its position. Unlinked elements are ig")
TOPIC_TEXT(
"nored.&][s11; T must have default constructor.&][s11; Invalidates iterators to AMap.&][s11; "
"Invalidates references to keys.&][s11; Invalidates references to VectorMap values.&][s1; "
"[*C@3 k]-|Key to find.&][s1; [*/ Return value]-|Position of element (either found "
"or added).&][s0;3 &][s4;:`:`:AMap`:`:FindAdd`(const K`&`,const T`&`): int [* FindAdd](const "
"K`& [*@3 k], const T`& [*@3 init])&][s6; Retrieves the position of first element "
"with the specified key in AMap. If multi`-key ordering is not broken and more than "
"one element with the same value exists in AMap, the lowest position is retrieved. "
"Unlinked elements are ignored. If the element does not exist in AMap, adds new element, "
"deep copy constructed from [*/ init], at the end of AMap and returns its position.&][s11; "
"T must have deep copy constructor.&][s11; Invalidates iterators to AMap.&][s11; Invalidates "
"references to keys.&][s11; Invalidates references to VectorMap values.&][s1; [*C@3 "
"k]-|Key to find.&][s1; [*C@3 init]-|Value to add if key is not in AMap yet.&][s1; "
"[*/ Return value]-|Position of element (either found or added).&][s0;3 &][s4;:`:`:AMap`:`:FindPutPick`(const "
"K`&`,pick`_ T`&`): int [* FindAddPick](const K`& [*@3 k], pick`_ T`& [*@3 init])&][s6; "
"Retrieves the position of first element with the specified key in AMap. If multi`-key "
"ordering is not broken and more than one element with the same value exists in AMap, "
"the lowest position is retrieved. Unlinked elements are ignored. If the element does "
"not exist in AMap, adds new element, pick constructed from [*/ init], at the end "
"of AMap and returns its position.&][s11; T must have pick constructor.&][s11; Invalidates "
"iterators to AMap.&][s11; Invalidates references to keys.&][s11; Invalidates references "
"to VectorMap values.&][s1; [*C@3 k]-|Key to find.&][s1; [*C@3 init]-|Value to add "
"if key is not in AMap yet.&][s1; [*/ Return value]-|Position of element (either found "
"or added).&][s0;3 &][s4;:`:`:AMap`:`:Unlink`(int`): void [* Unlink](int [*@3 i])&][s6")
TOPIC_TEXT(
"; Unlinks element at the specified position. Unlinked item stays in AMap but is "
"ignored by any Find operation.&][s1; [*C@3 i]-|Position of element to unlink.&][s0;3 "
"&][s4;:`:`:AMap`:`:Put`(const K`&`,const T`&`): void [* Put](const K`& [*@3 k], const "
"T`& [*@3 x])&][s6; If there are any unlinked elements in AMap, one of them is replaced "
"by the specified key/value pair. If there is are unlinked elements, the key/value "
"pair is added to the end of AIndex using [* Add]. Value is transfered using deep "
"copy constructor.&][s11; T must have deep copy constructor.&][s11; Invalidates multi`-key "
"ordering.&][s11; Invalidates iterators to AMap.&][s11; Invalidates references to "
"keys.&][s11; Invalidates references to VectorMap values.&][s1; [*C@3 k]-|Key.&][s1; "
"[*C@3 x]-|Value.&][s0;3 &][s4;:`:`:AMap`:`:PutPick`(const K`&`,pick`_ T`&`): void "
"[* PutPick](const K`& [*@3 k], pick`_ T`& [*@3 x])&][s6; If there are any unlinked "
"elements in AMap, one of them is replaced by the specified key/value pair. If there "
"is none unlinked element, the key/value pair is added at the end of AIndex using "
"[* Add]. Value is transfered using pick constructor.&][s11; T must have pick constructor.&][s11; "
"Invalidates multi`-key ordering.&][s11; Invalidates iterators to AMap.&][s11; Invalidates "
"references to keys.&][s11; Invalidates references to VectorMap values.&][s1; [*C@3 "
"k]-|Key.&][s1; [*C@3 x]-|Value.&][s0;3 &][s4;:`:`:AMap`:`:Put`(const K`&`): T`& [* "
"Put](const K`& [*@3 k])&][s6; If there is any unlinked element in AMap, it is replaced "
"by the specified key and reference to the value is returned. If there is none unlinked "
"element, key is added at the end of AIndex using [* Add ]and a reference to corresponding "
"default constructed Value is returned.&][s11; T must have default constructor.&][s11; "
"Invalidates multi`-key ordering.&][s11; Invalidates iterators to AMap.&][s11; Invalidates "
"references to keys.&][s11; Invalidates references to VectorMap values.&][s1; [*C@3 "
"k]-|Key.&][s1; [*/ Return value]-|Refe")
TOPIC_TEXT(
"rence to the corresponding value.&][s0;3 &][s4;:`:`:AMap`:`:FindPut`(const K`&`): "
"int [* FindPut](const K`& [*@3 k])&][s6; Retrieves the position of first element "
"with the specified key in AMap. Unlinked elements are ignored. If the element does "
"not exist in AMap, puts new default constructed element into AMap using [* Put] and "
"returns its position. &][s11; T must have default constructor.&][s11; Invalidates "
"iterators to AMap.&][s11; Invalidates references to keys.&][s11; Invalidates references "
"to VectorMap values.&][s1; [*C@3 k]-|Key to find.&][s1; [*/ Return value]-|Position "
"of element (either found or added).&][s0;3 &][s4;:`:`:AMap`:`:FindPut`(const K`&`,const "
"T`&`): int [* FindPut](const K`& [*@3 k], const T`& [*@3 init])&][s6; Retrieves the "
"position of first element with the specified key in AMap. Unlinked elements are ignored. "
"If the element does not exist in AMap, puts new element, deep copy constructed from "
"[* init], using [* Put] and returns its position. &][s11; T must have deep copy constructor.&][s11; "
"Invalidates iterators to AMap.&][s11; Invalidates references to keys.&][s11; Invalidates "
"references to VectorMap values.&][s1; [*C@3 k]-|Key to find.&][s1; [*C@3 init]-|Value "
"to add if key is not in AMap yet.&][s1; [*/ Return value]-|Position of element (either "
"found or added).&][s0;3 &][s4;:`:`:AMap`:`:FindPutPick`(const K`&`,pick`_ T`&`): "
"int [* FindPutPick](const K`& [*@3 k], pick`_ T`& [*@3 init])&][s6; Retrieves the "
"position of first element with the specified key in AMap. Unlinked elements are ignored. "
"If the element does not exist in AMap, puts new element, pick constructed from [* "
"init], using [* Put] and returns its position. &][s11; T must have deep copy constructor.&][s11; "
"Invalidates iterators to AMap.&][s11; Invalidates references to keys.&][s11; Invalidates "
"references to VectorMap values.&][s1; [*C@3 k]-|Key to find.&][s1; [*C@3 init]-|Value "
"to add if key is not in AMap yet.&][s1; [*/ Return value]-|Position of element (either "
"found or added).")
TOPIC_TEXT(
"&][s0;3 &][s4;:`:`:AMap`:`:Get`(const K`&`): T`& [* Get](const K`& [*@3 k])&][s6; "
"Retrieves a reference to the first element with the specified key. If multi`-key "
"ordering is not broken and more than one element with the same value exists in AMap, "
"the lowest position element is retrieved. Unlinked elements are ignored. Required "
"key must be in AMap, otherwise it is logic error (asserted in debug mode).&][s1; "
"[*C@3 k]-|Key to find.&][s1; [*/ Return value]-|Reference to corresponding value.&][s0;3 "
"&][s4;:`:`:AMap`:`:Get`(const K`&`)const: const T`& [* Get](const K`& [*@3 k]) const&][s6; "
"Retrieves a constant reference the the first element with the specified key. If multi`-key "
"ordering is not broken and more than one element with the same value exists in AMap, "
"the lowest position element is retrieved. Unlinked elements are ignored. Required "
"key must be in AMap, otherwise it is logic error (asserted in debug mode).&][s1; "
"[*C@3 k]-|Key to find.&][s1; [*/ Return value]-|Reference to corresponding value.&][s0;3 "
"&][s4;:`:`:AMap`:`:Get`(const K`&`,const T`&`)const: const T`& [* Get](const K`& "
"[*@3 k], const T`& [*@3 d]) const&][s6; Retrieves a constant reference value of the "
"first element with the specified key. If multi`-key ordering is not broken and more "
"than one element with the same value exists in AMap, the lowest position element "
"is retrieved. Unlinked elements are ignored. If the required key is not in the AMap, "
"constant reference to the specified value is returned instead.&][s1; [*C@3 k]-|Key "
"to find.&][s1; [*C@3 d]-|Value to be returned if key is not found.&][s1; [*/ Return "
"value]-|Reference to found element or supplied value.&][s0;3 &][s4;:`:`:AMap`:`:GetAdd`(const "
"K`&`): T`& [* GetAdd](const K`& [*@3 k])&][s6; Retrieves a constant reference value "
"of the first element with the specified key. If multi`-key ordering is not broken "
"and more than one element with the same value exists in AMap, the lowest position "
"element is retrieved. Unlinked elements are ignored. If r")
TOPIC_TEXT(
"equired key is not in the AMap, adds new default constructed element at the end "
"of AMap and returns a reference to it.&][s11; T must have default constructor.&][s11; "
"Invalidates iterators to AMap.&][s11; Invalidates references to keys.&][s11; Invalidates "
"references to VectorMap values.&][s1; [*C@3 k]-|Key to find.&][s1; [*/ Return value]-|Reference "
"to corresponding value.&][s0;3 &][s4;:`:`:AMap`:`:GetAdd`(const K`&`,const T`&`): "
"T`& [* GetAdd](const K`& [*@3 k], const T`& [*@3 x])&][s6; Retrieves a constant reference "
"to the first element with the specified key. If multi`-key ordering is not broken "
"and more than one element with the same value exists in AMap, the lowest position "
"element is retrieved. Unlinked elements are ignored. If required key is not in the "
"AMap, adds new element, deep copy constructed from [* x],[*  ]at the end of AMap "
"and returns a reference to it.&][s11; T must have deep copy constructor.&][s11; Invalidates "
"iterators to AMap.&][s11; Invalidates references to keys.&][s11; Invalidates references "
"to VectorMap values.&][s1; [*C@3 k]-|Key to find.&][s1; [*C@3 x]-|Value to add if "
"key is not in AMap.&][s1; [*/ Return value]-|Reference to corresponding value.&][s0;3 "
"&][s4;:`:`:AMap`:`:GetAddPick`(const K`&`,pick`_ T`&`):~~~.992; T`& [* GetAddPick](const "
"K`& [*@3 k], pick`_ T`& [*@3 x])&][s6; Retrieves a constant reference to the first "
"element with the specified key. If multi`-key ordering is not broken and more than "
"one element with the same value exists in AMap, the lowest position element is retrieved. "
"Unlinked elements are ignored. If required key is not in the AMap, adds new element, "
"pick constructed from [* x],[*  ]at the end of AMap and returns a reference to it.&][s11; "
"T must have pick constructor.&][s11; Invalidates iterators to AMap.&][s11; Invalidates "
"references to keys.&][s11; Invalidates references to VectorMap values.&][s1; [*C@3 "
"k]-|Key to find.&][s1; [*C@3 x]-|Value to add if key is not in AMap.&][s1; [*/ Return "
"value]-|Reference to corr")
TOPIC_TEXT(
"esponding value.&][s0;3 &][s4;:`:`:AMap`:`:GetPut`(const K`&`): T`& [* GetPut](const "
"K`& [*@3 k])&][s6; Retrieves a constant reference value of the first element with "
"the specified key. If multi`-key ordering is not broken and more than one element "
"with the same value exists in AMap, the lowest position element is retrieved. Unlinked "
"elements are ignored. If required key is not in the AMap, puts new default constructed "
"element into the AMap using [* Put] and returns a reference to it.&][s11; T must "
"have default constructor.&][s11; Invalidates iterators to AMap.&][s11; Invalidates "
"references to keys.&][s11; Invalidates references to VectorMap values.&][s1; [*C@3 "
"k]-|Key to find.&][s1; [*/ Return value]-|Reference to corresponding value.&][s0;3 "
"&][s4;:`:`:AMap`:`:GetPut`(const K`&`,const T`&`): T`& [* GetPut](const K`& [*@3 "
"k], const T`& [*@3 x])&][s6; Retrieves a constant reference value of the first element "
"with the specified key. If multi`-key ordering is not broken and more than one element "
"with the same value exists in AMap, lowest position element is retrieved. Unlinked "
"elements are ignored. If required key is not in the AMap, puts new element, deep "
"copy constructed from [* x],[*  ]into the AMap using [* Put] and returns a reference "
"to it.&][s11; T must have deep copy constructor.&][s11; Invalidates iterators to "
"AMap.&][s11; Invalidates references to keys.&][s11; Invalidates references to VectorMap "
"values.&][s1; [*C@3 k]-|Key to find.&][s1; [*C@3 x]-|Value to add if key is not in "
"AMap.&][s1; [*/ Return value]-|Reference to corresponding value.&][s0;3 &][s4;:`:`:AMap`:`:GetPutPick`(const "
"K`&`,pick`_ T`&`): T`& [* GetPutPick](const K`& [*@3 k], pick`_ T`& [*@3 x])&][s6; "
"Retrieves a constant reference value of the first element with the specified key. "
"If multi`-key ordering is not broken and more than one element with the same value "
"exists in AMap, lowest position element is retrieved. Unlinked elements are ignored. "
"If required key is not in the AMap, puts new elem")
TOPIC_TEXT(
"ent, pick constructed from [* x], into the AMap using [* Put] and returns a reference "
"to it.&][s11; T must have pick constructor.&][s11; Invalidates iterators to AMap.&][s11; "
"Invalidates references to keys.&][s11; Invalidates references to VectorMap values.&][s1; "
"[*C@3 k]-|Key to find.&][s1; [*C@3 x]-|Value to add if key is not in AMap.&][s1; "
"[*/ Return value]-|Reference to corresponding value.&][s0;3 &][s4;:`:`:AMap`:`:SetKey`(int`,const "
"K`&`): void [* SetKey](int [*@3 i], const K`& [*@3 k])&][s6; Replaces key of element "
"at the specified position.&][s1; [*C@3 i]-|Position of element.&][s1; [*C@3 k]-|New "
"key value.&][s0;3 &][s4;:`:`:AMap`:`:FindPtr`(const K`&`): T `*[* FindPtr](const "
"K`& [*@3 k])&][s6; Retrieves a pointer to value of first element with the specified "
"key in AMap. If multi`-key ordering is not broken and more than one element with "
"the same value exists in AMap, the lowest position is retrieved. If the element does "
"not exist in AMap, NULL pointer is returned. Unlinked elements are ignored.&][s1; "
"[*C@3 k]-|Key to find.&][s1; [*/ Return value]-|Pointer to value or NULL pointer "
"if element is not in AMap.&][s0;3 &][s4;:`:`:AMap`:`:FindPtr`(const K`&`)const: const "
"T `*[* FindPtr](const K`& [*@3 k]) const&][s6; Retrieves a constant pointer to value "
"of first element with the specified key in AMap. If multi`-key ordering is not broken "
"and more than one element with the same value exists in AMap, the lowest position "
"is retrieved. If the element does not exist in AMap, NULL pointer is returned. Unlinked "
"elements are ignored.&][s1; [*C@3 k]-|Key to find.&][s1; [*/ Return value]-|Pointer "
"to value or NULL pointer if element is not in AMap.&][s0;3 &][s4;:`:`:AMap`:`:UnlinkKey`(const "
"K`&`,unsigned`): int [* UnlinkKey](const K`& [*@3 k], unsigned [*@3 h])&][s6; Unlinks "
"all elements with the specified key using precomputed hash`-value. Unlinked elements "
"stay in AIndex but are ignored by any Find operations. The precomputed hash value "
"must be the same as the hash value ")
TOPIC_TEXT(
"that would be the result of HashFn. &][s1; [*C@3 k]-|Key to unlink.&][s1; [*C@3 "
"h]-|Precomputed hash value.&][s1; [*/ Return value]-|Number of elements unlinked.&][s0;3 "
"&][s4;:`:`:AMap`:`:UnlinkKey`(const K`&`): int [* UnlinkKey](const K`& [*@3 k])&][s6; "
"Unlinks all elements with the specified key. Unlinked elements stay in AIndex but "
"are ignored by any Find operations.&][s1; [*C@3 k]-|Key to unlink.&][s1; [*/ Return "
"value]-|Number of elements unlinked.&][s0;3 &][s4;:`:`:AMap`:`:IsUnlinked`(int`)const: "
"bool [* IsUnlinked](int [*@3 i]) const&][s6; Tests whether element at the specified "
"position is unlinked.&][s1; [*C@3 i]-|Position.&][s1; [*/ Return value]-|true if "
"element is unlinked.&][s0;3 &][s4;:`:`:AMap`:`:Insert`(int`,const K`&`): T`& [* Insert](int "
"[*@3 i], const K`& [*@3 k])&][s6; Inserts an element with the specified key and default "
"constructed value at the specified position. This is a slow operation, especially "
"when combined with any search operations.&][s11; Requires T to have default constructor.&][s11; "
"Invalidates iterators to AMap.&][s11; Invalidates references to keys.&][s11; Invalidates "
"references to VectorMap values.&][s1; [*C@3 i]-|Insert position.&][s1; [*C@3 k]-|Key "
"to insert.&][s0;3 &][s4;:`:`:AMap`:`:Insert`(int`,const K`&`,const T`&`): void [* "
"Insert](int [*@3 i], const K`& [*@3 k], const T`& [*@3 x])&][s6; Inserts an element "
"with the specified key and value copy constructed from [*/ x] at the specified position. "
"This is a slow operation, especially when combined with any search operations.&][s11; "
"Requires T to have deep copy constructor.&][s11; Invalidates iterators to AMap.&][s11; "
"Invalidates references to keys.&][s11; Invalidates references to VectorMap values.&][s1; "
"[*C@3 i]-|Insert position.&][s1; [*C@3 k]-|Key to insert.&][s1; [*C@3 x]-|Value to "
"insert.&][s0;3 &][s4;:`:`:AMap`:`:Remove`(int`): void [* Remove](int [*@3 i])&][s6; "
"Removes the element at the specified position. This is a slow operation, especially "
"when combined with any sear")
TOPIC_TEXT(
"ch operations.&][s11; Invalidates iterators to AMap.&][s11; Invalidates references "
"to keys.&][s11; Invalidates references to VectorMap values.&][s1; [*C@3 i]-|Position "
"of element to remove.&][s0;3 &][s4;:`:`:AMap`:`:Remove`(const int`*`,int`): void "
"[* Remove](const int `*[*@3 sl], int [*@3 n])&][s6; Removes number of elements from "
"AMap. Time of operation only slightly depends on the number of removed elements. "
"This is a slow operation, especially when combined with any search operations.&][s11; "
"Invalidates iterators to AMap.&][s11; Invalidates references to keys.&][s11; Invalidates "
"references to VectorMap values.&][s1; [*C@3 i]-|Position of element to remove.&][s1; "
"[*C@3 sl]-|Pointer to array of the positions to remove, in ascending order.&][s1; "
"[*C@3 n]-|Number of elements to remove. &][s0;3 &][s4;:`:`:AMap`:`:Remove`(const`:`:Vector`<int`>`&`): "
"void [* Remove](const Vector<int>`& [*@3 sl])&][s6; Removes number of elements from "
"AMap. Same as Remove(sorted`_list, sorted`_list.GetCount()).&][s11; Invalidates iterators "
"to AMap.&][s11; Invalidates references to keys.&][s11; Invalidates references to "
"VectorMap values.&][s1; [*C@3 sl]-|Sorted Vector of positions to remove.&][s0;3 &][s4;:`:`:AMap`:`:RemoveKey`(const "
"K`&`): int [* RemoveKey](const K`& [*@3 k])&][s6; Removes all elements with the specified "
"value. This is a slow operation, especially when combined with any search operations.&][s1; "
"[*C@3 k]-|Key to remove.&][s0;3 &][s4;:`:`:AMap`:`:operator`[`]`(int`)const: const "
"T`& [* operator]`[`](int [*@3 i]) const&][s6; Returns a constant reference to the "
"element at the specified position.&][s1; [*C@3 i]-|Position.&][s1; [*/ Return value]-|Constant "
"reference to the element at the specified position.&][s0;3 &][s4;:`:`:AMap`:`:operator`[`]`(int`): "
"T`& [* operator]`[`](int [*@3 i])&][s6; Returns a reference to the element at the "
"specified position.&][s1; [*C@3 i]-|Position.&][s1; [*/ Return value]-|Constant reference "
"to the element at the specified position.&][s0;3 &][s4;:`")
TOPIC_TEXT(
":`:AMap`:`:GetCount`(`)const: int [* GetCount]() const&][s6; Returns the number "
"of elements in AMap.&][s1; [*/ Return value]-|Actual number of elements.&][s0;3 &][s4;:`:`:AMap`:`:IsEmpty`(`)const: "
"bool [* IsEmpty]() const&][s6; Tests whether AMap is empty. Same as GetCount() `=`= "
"0.&][s1; [*/ Return value]-|true if AMap is empty, false otherwise.&][s0;3 &][s4;:`:`:AMap`:`:Clear`(`): "
"void [* Clear]()&][s6; Removes all elements from AMap.&][s0;3 &][s4;:`:`:AMap`:`:Shrink`(`): "
"void [* Shrink]()&][s6; Minimizes memory consumption of AMap by decreasing the capacity "
"to the number of elements.&][s0;3 &][s4;:`:`:AMap`:`:Reserve`(int`): void [* Reserve](int "
"[*@3 xtra])&][s6; Reserves capacity. If the required capacity is greater than current "
"capacity, capacity is increased to the required value.&][s1; [*C@3 n]-|Required capacity.&][s0;3 "
"&][s4;:`:`:AMap`:`:GetAlloc`(`)const: int [* GetAlloc]() const&][s6; Returns the "
"current capacity of Array.&][s1; [*/ Return value]-|Capacity of Array.&][s0;3 &][s4;:`:`:AMap`:`:Drop`(int`): "
"void [* Drop](int [*@3 n] `= 1)&][s6; Drops the specified number of elements at the "
"end of the AMap.&][s1; [*C@3 n]-|Number of elements.&][s0;3 &][s4;:`:`:AMap`:`:Top`(`): "
"T`& [* Top]()&][s6; Returns a reference to the value of the last element of AMap.&][s1; "
"[*/ Return value]-|Reference to the value of the last element.&][s0;3 &][s4;:`:`:AMap`:`:Top`(`): "
"const T`& [* Top]() const&][s6; Returns a constant reference to the value of the "
"last element of AMap.&][s1; [*/ Return value]-|Reference to the value of the last "
"element.&][s0;3 &][s4;:`:`:AMap`:`:TopKey`(`)const: const K`& [* TopKey]() const&][s6; "
"Returns a constant reference to the key of the last element of AMap.&][s1; [*/ Return "
"value]-|Reference to the key of the last element.&][s0;3 &][s4;:`:`:AMap`:`:PopKey`(`): "
"K [* PopKey]()&][s6; Drops the last element of AMap and returns the key of the dropped "
"element.&][s1; [*/ Return value]-|Key of the element dropped at the end of AMap.&][s0;3 "
"&][s4;:`:`:A")
TOPIC_TEXT(
"Map`:`:GetKey`(int`)const: const K`& [* GetKey](int [*@3 i]) const&][s6; Returns "
"a constant reference to the key of element at the specified position.&][s1; [*C@3 "
"i]-|Position.&][s1; [*/ Return value]-|Constant reference to the key.&][s0;3 &][s4;:`:`:AMap`:`:Serialize`(`:`:Stream`&`): "
"void [* Serialize](Stream`& [*@3 s])&][s6; Serializes the content of AMap to/from "
"Stream. Works only if NTL is used as part of UPP.&][s11; Requires T to have serialization "
"operator defined.&][s1; [*C@3 s]-|Target/source stream.&][s0;3 &][s4;:`:`:AMap`:`:GetIndex`(`)const: "
"const Index<K>`& [* GetIndex]() const&][s6; Returns a constant reference to the internal "
"Index of keys.&][s1; [*/ Return value]-|Constant reference to the Index of keys.&][s0;3 "
"&][s4;:`:`:AMap`:`:PickIndex`(`)pick`_: Index<K> [* PickIndex]() pick`_&][s6; Returns "
"Index of keys. Destroys AMap by picking.&][s1; [*/ Return value]-|Index of keys.&][s0;3 "
"&][s4;:`:`:AMap`:`:GetKeys`(`)const: const Vector<K>`& [* GetKeys]() const&][s6; "
"Returns a constant reference to the Vector of keys.&][s1; [*/ Return value]-|Constant "
"reference to the Vector of keys.&][s0;3 &][s4;:`:`:AMap`:`:PickKeys`(`)pick`_: Vector<K> "
"[* PickKeys]() pick`_&][s6; Returns Vector of keys. Destroys AMap by picking.&][s1; "
"[*/ Return value]-|Vector of keys.&][s0;3 &][s4;:`:`:AMap`:`:GetValues`(`)const: "
"const V`& [* GetValues]() const&][s6; Returns a constant reference to the basic random "
"access container of values. Destroys AIndex by picking.&][s1; [*/ Return value]-|Constant "
"reference to the basic random access container of values.&][s0;3 &][s4;:`:`:AMap`:`:PickValues`(`)pick`_: "
"V [* PickValues]()&][s6; Returns basic random access container of values. Destroys "
"AIndex by picking.&][s1; [*/ Return value]-|Basic random access container of values.&][s0;3 "
"&][s4;:`:`:AMap`:`:AMap`(`): [* AMap]()&][s6; Constructor. Constructs an empty AMap.&][s0;3 "
"&][s4;:`:`:AMap`:`:AMap`(const`:`:AMap`&`,int`): [* AMap](const [* AMap]`& [*@3 s], "
"int)&][s6; Optional deep copy const")
TOPIC_TEXT(
"ructor.&][s11; Requires T to have deep copy constructor or optional deep copy constructor.&][s1; "
"[*C s]-|Source AMap.&][s0;3 &][s4;:`:`:AMap`:`:AMap`(pick`_`:`:Index`<K`,HashFn`>`&`,pick`_ "
"V`&`): [* AMap](pick`_ Index<K>`& [*@3 ndx], pick`_ V`& [*@3 val])&][s6; This form "
"of constructors creates AMap by picking Index of keys and basic random access container "
"of values. Both containers must have same number of elements.&][s1; [*C@3 ndx]-|Keys.&][s1; "
"[*C@3 val]-|Values.&][s0;3 &][s4;:`:`:AMap`:`:AMap`(pick`_`:`:Vector`<K`>`&`,pick`_ "
"V`&`): [* AMap](pick`_ Vector<K>`& [*@3 ndx], pick`_ V`& [*@3 val])&][s6; This form "
"of constructors creates AMap by picking Vector of keys and basic random access container "
"of values. Both containers must have same number of elements.&][s1; [*C@3 ndx]-|Keys.&][s1; "
"[*C@3 val]-|Values.&][s0;3 &][s4;:`:`:AMap`:`:KeyType`:`:typedef:* [* typedef ]K[* "
" ]KeyType&][s6; Typedef of K for use in templated algorithms.&][s0;3 &][s4;:`:`:AMap`:`:KeyConstIterator`:`:typedef:* "
"[* typedef ][*/ type][*  ]KeyConstIterator&][s6; Key iterator type.&][s0;3 &][s4;:`:`:AMap`:`:KeyBegin`(`)const: "
"KeyConstIterator [* KeyBegin]() const&][s6; Returns a constant iterator to the first "
"key in AMap.&][s1; [*/ Return value]-|Constant key iterator.&][s0;3 &][s4;:`:`:AMap`:`:KeyEnd`(`)const: "
"KeyConstIterator [* KeyEnd]() const&][s6; Returns a constant iterator to the key "
"just beyond the last key in AMap.&][s1; [*/ Return value]-|Constant key iterator.&][s0;3 "
"&][s4;:`:`:AMap`:`:KeyGetIter`(int`)const: KeyConstIterator [* KeyGetIter](int [*@3 "
"pos]) const&][s6; Returns a constant iterator to the key at the specified position. "
"Same as [* KeyBegin() `+ i]. The benefit of this method is that [* pos] is range "
"checked in debug mode. &][s1; [*C@3 i]-|Required position.&][s1; [*/ Return value]-|Constant "
"key iterator.&][s0;3 &][s4;:`:`:AMap`:`:ValueType`:`:typedef:* [* typedef T ]ValueType&][s6; "
"Typedef of T for use in templated algorithms.&][s0;3 &][s4;:`:`:AMap`:`:ConstIterator`:`:ty")
TOPIC_TEXT(
"pedef:* [* typedef ][*/ type][*  ]ConstIterator&][s6; Constant value iterator type.&][s0;3 "
"&][s4;:`:`:AMap`:`:Iterator`:`:typedef:* [* typedef ][*/ type][*  ]Iterator&][s6; "
"Value iterator type.&][s0;3 &][s4;:`:`:AMap`:`:Begin`(`): Iterator [* Begin]()&][s6; "
"Returns an iterator to the first value in AMap.&][s1; [*/ Return value]-|Value iterator.&][s0;3 "
"&][s4;:`:`:AMap`:`:End`(`): Iterator [* End]()&][s6; Returns a constant iterator "
"to the value just beyond the last key in AMap.&][s1; [*/ Return value]-|Value iterator.&][s0;3 "
"&][s4;:`:`:AMap`:`:GetIter`(int`): Iterator [* GetIter](int [*@3 pos])&][s6; Returns "
"an iterator to the value at the specified position. Same as [* Begin() `+ i]. The "
"benefit of this method is that [* pos] is range checked in debug mode.&][s1; [*C@3 "
"i]-|Required position.&][s1; [*/ Return value]-|Value iterator.&][s0;3 &][s4;:`:`:AMap`:`:Begin`(`)const: "
"ConstIterator [* Begin]() const&][s6; Returns a constant iterator to the first value "
"in AMap.&][s1; [*/ Return value]-|Constant value iterator.&][s0;3 &][s4;:`:`:AMap`:`:End`(`)const: "
"ConstIterator [* End]() const&][s6; Returns a constant iterator to the value just "
"beyond the last value in AMap.&][s1; [*/ Return value]-|Constant value iterator.&][s0;3 "
"&][s4;:`:`:AMap`:`:GetIter`(int`)const: ConstIterator [* GetIter](int [*@3 pos]) "
"const&][s6; Returns a constant iterator to the value at the specified position. Same "
"as [* Begin() `+ i]. Benefit of this methods is that in debug mode [* pos] is range "
"checked.&][s1; [*C@3 i]-|Required position.&][s1; [*/ Return value]-|Constant value "
"iterator.&][s4; friend void [* Swap](AMap`& [*@3 a], AMap`& [*@3 b])&][s6; Specialization "
"of the generic [* Swap] for AMap. Swaps array in simple constant time operation.&][s1; "
"[*C@3 a]-|First AMap to swap.&][s1; [*C@3 b]-|Second AMap to swap.]")
