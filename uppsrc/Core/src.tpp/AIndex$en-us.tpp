TITLE("AIndex")
REF("::AIndex::class")
REF("::AIndex::Add(const T&,unsigned)")
REF("::AIndex::Add(const T&)")
REF("::AIndex::Find(const T&,unsigned)const")
REF("::AIndex::Find(const T&)const")
REF("::AIndex::FindNext(int)const")
REF("::AIndex::FindLast(const T&,unsigned)const")
REF("::AIndex::FindLast(const T&)const")
REF("::AIndex::FindPrev(int)const")
REF("::AIndex::FindAdd(const T&,unsigned)")
REF("::AIndex::FindAdd(const T&)")
REF("::AIndex::operator<<(const T&)")
REF("::AIndex::Unlink(int)")
REF("::AIndex::Put(const T&,unsigned)")
REF("::AIndex::Put(const T&)")
REF("::AIndex::FindPut(const T&,unsigned)")
REF("::AIndex::FindPut(const T&)")
REF("::AIndex::Set(int,const T&,unsigned)")
REF("::AIndex::Set(int,const T&)")
REF("::AIndex::operator[](int)const")
REF("::AIndex::GetCount()const")
REF("::AIndex::IsEmpty()const")
REF("::AIndex::Clear()")
REF("::AIndex::ClearIndex()const")
REF("::AIndex::UnlinkKey(const T&,unsigned)")
REF("::AIndex::UnlinkKey(const T&)")
REF("::AIndex::IsUnlinked(int)const")
REF("::AIndex::Sweep()")
REF("::AIndex::Insert(int,const T&,unsigned)")
REF("::AIndex::Insert(int,const T&)")
REF("::AIndex::Remove(int)")
REF("::AIndex::Remove(const int*,int)")
REF("::AIndex::Remove(const::Vector<int>&)")
REF("::AIndex::RemoveKey(const T&,unsigned)")
REF("::AIndex::RemoveKey(const T&)")
REF("::AIndex::Trim(int)")
REF("::AIndex::Drop(int)")
REF("::AIndex::Top()const")
REF("::AIndex::Reserve(int)")
REF("::AIndex::Shrink()")
REF("::AIndex::GetAlloc()const")
REF("::AIndex::Serialize(::Stream&)")
REF("::AIndex::PickKeys()pick_")
REF("::AIndex::GetKeys()const")
REF("::AIndex::operator=(pick_ V&)")
REF("::AIndex::operator<<=(const V&)")
REF("::AIndex::AIndex(pick_ V&)")
REF("::AIndex::AIndex(const V&,int)")
REF("::AMap::ValueType::typedef")
REF("::AIndex::ConstIterator::typedef")
REF("::AIndex::Begin()const")
REF("::AIndex::End()const")
REF("::AIndex::GetIter(int)const")
REF("::AIndex::AIndex()")
REF("::AIndex::AIndex(const::AIndex&,int)")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2;:`:`:AIndex`:`:class:t292; "
"AIndex&][s10;:`:`:AIndex`:`:class:t292; template <class [*@4 T], class [*@4 V], class "
"[*@4 HashFn]>&][s10;t292;* [* class ]AIndex&][s0; &][s12; [*C@4 T]-|Type of elements "
"to store. T must satisfy requirements for container flavor identified by parameter "
"V and must have [*C operator`=`=] defined.&][s12; [*C@4 V]-|Basic random access container.&][s12; "
"[*C@4 HashFn]-|Hashing class. Must have defined [*C unsigned operator()(const T`& "
"x)] method returning hash value for elements.&][s5; This template class adds associative "
"capabilities to basic random access containers, forming flavors of Index. It is used "
"as base class ")
TOPIC_TEXT(
"for concrete index flavors, [* Index] and [* ArrayIndex].&][s5; It allows adding "
"elements at the end of sequence in constant amortized time like basic random container. "
"Additionally, it also allows fast retrieval of a position of the element with specified "
"value. Hashing is used for this operation. AIndex stores hash`-values of elements, "
"so it has no sense to cache them externally.&][s5; Building of internal hash maps "
"of AIndex is always deferred till search operation. This effectively avoids unneeded "
"remapping if large number of elements is added.&][s5; Removing elements from an AIndex "
"causes an interesting problem. While it is possible to simply remove (or insert) "
"an element at a specified position, such operation has to move a lot of elements "
"and also scratches internal hash maps. Thus removing elements this way is slow, especially "
"when combined with searching.&][s5; The solution for this problem is [*/ unlinking] "
"of elements. Unlinked elements are not removed from index, but they are [*/ ignored][/ "
" ]by search operations. Unlinking is a simple, constant time, fast operation. Further, "
"it is possible to place an element at the first available unlinked position (rather "
"than to the end of sequence) using the [* Put] method, reusing unlinked position "
"in short constant time.&][s5; The only problem of unlinking is that it breaks the "
"so`-called [* multi`-key ordering]. This term means that if there are more elements "
"with the same value in the index and they are iterated through using the FindNext "
"method, their positions (got as the result of Find and subsequent FindNext methods) "
"are in ascending order. The problem is that it is impossible to implement placing "
"elements at unlinked positions in short time while preserving this ordering. On the "
"other hand, usage scenarios for indexes show that need for unlinking elements and "
"multi`-key ordering is almost always disjunct. For the rest of the cases, it is always "
"possible to restore ordering by the [* Reindex] or the [* Sweep")
TOPIC_TEXT(
"] method.&][s5; Like any other NTL container, AIndex is [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ "
"moveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ pick and "
"optional deep copy] transfer semantics, although these features are more important "
"in derived concrete index flavors.&][s3; Members&][s0;3 &][s4;:`:`:AIndex`:`:Add`(const "
"T`&`,unsigned`): void [* Add](const T`& [*@3 x], unsigned [*@3 `_hash])&][s6; Adds "
"a new element with a precomputed hash value.The precomputed hash value must be the "
"same as the hash value that would be the result of HashFn. The benefit of this variant "
"is that sometimes you can compute hash`-value as the part of an other process, like "
"fetching strings from an input stream.&][s11; Requires T to have deep copy constructor.&][s11; "
"Invalidates iterators to AIndex.&][s11; Invalidates references to Index.&][s1; [*C@3 "
"x]-|Element to add.&][s1; [*C@3 `_hash]-|Precomputed hash value.&][s0;3 &][s4;:`:`:AIndex`:`:Add`(const "
"T`&`): void [* Add](const T`& [*@3 x])&][s6; Adds a new element to AIndex.&][s11; "
"Requires T to have deep copy constructor.&][s11; Invalidates iterators to AIndex.&][s11; "
"Invalidates references to Index.&][s1; [*C@3 x]-|Element to add.&][s0;3 &][s4;:`:`:AIndex`:`:Find`(const "
"T`&`,unsigned`)const: int [* Find](const T`& [*@3 x], unsigned [*@3 `_hash]) const&][s6; "
"Retrieves the position of the first element with the specified value in AIndex, using "
"a precomputed hash value. The precomputed hash value must be the same as the hash "
"value that would be result of HashFn. If multi`-key ordering is not broken and more "
"than one element with the same value exists in AIndex, the lowest position is retrieved. "
"If the specified value does not exist in AIndex, a negative number is returned. Unlinked "
"elements are ignored.&][s1; [*C@3 x]-|Element to find.&][s1; [*C@3 `_hash]-|Precomputed "
"hash value.&][s1; [*/ Return value]-|Position of the element or negative value if "
"value is not in AIndex.&][s0;3 &][s4;:`:`:AIndex`:`:Find`(c")
TOPIC_TEXT(
"onst T`&`)const: int [* Find](const T`& [*@3 x]) const&][s6; Retrieves the position "
"of the first element with the specified value in AIndex. If multi`-key ordering is "
"not broken and more than one element with the same value exists in AIndex, lowest "
"position is retrieved. If the specified value does not exist in AIndex, a negative "
"number is returned. Unlinked elements are ignored.&][s1; [*C@3 x]-|Element to find.&][s1; "
"[*/ Return value]-|Position of the element or negative value if value is not in AIndex.&][s0;3 "
"&][s4;:`:`:AIndex`:`:FindNext`(int`)const:~~~.992; int [* FindNext](int [*@3 i]) "
"const&][s6; Retrieves the position of the next element with the same value as the "
"element at specified position. If multi`-key ordering is not broken and more than "
"one element with that value exists in AIndex, the lowest position greater than specified "
"one is retrieved, so positions returned by subsequent calls to FindNext are in ascending "
"order. When there are no more elements with the required value, a negative number "
"is returned. Unlinked elements are ignored.&][s1; [*C@3 i]-|Position of the element.&][s1; "
"[*/ Return value]-|Position of the next element with same value.&][s0;3 &][s4;:`:`:AIndex`:`:FindLast`(const "
"T`&`,unsigned`)const: int [* FindLast](const T`& [*@3 x], unsigned [*@3 `_hash]) "
"const&][s6; Retrieves the position of the last element with specified value in AIndex, "
"using a precomputed hash value. The precomputed hash value must be the same as the "
"hash value that would be the result of HashFn. If multi`-key ordering is not broken "
"and more than one element with the same value exists in AIndex, the greatest position "
"is retrieved. If the specified value does not exist in AIndex, a negative number "
"is returned. Unlinked elements are ignored.&][s1; [*C@3 x]-|Element to find.&][s1; "
"[*C@3 `_hash]-|Precomputed hash value.&][s1; [*/ Return value]-|Position of the element "
"or negative number if value is not in AIndex.&][s0;3 &][s4;:`:`:AIndex`:`:FindLast`(const "
"T`&`)const: int")
TOPIC_TEXT(
" [* FindLast](const T`& [*@3 x]) const&][s6; Retrieves the position of the last "
"element with specified value in AIndex. If multi`-key ordering is not broken and "
"more than one element with the same value exists in AIndex, the greatest position "
"is retrieved. If element does not exist in AIndex, a negative number is returned. "
"Unlinked elements are ignored.&][s1; [*C@3 x]-|Element to find.&][s1; [*C@3 `_hash]-|Precomputed "
"hash value.&][s1; [*/ Return value]-|Position of the element or negative number if "
"value is not in AIndex.&][s0;3 &][s4;:`:`:AIndex`:`:FindPrev`(int`)const: int [* "
"FindPrev](int [*@3 i]) const&][s6; Retrieves the position of the previous element "
"with the same value as the element at the specified position. If multi`-key ordering "
"is not broken and more than one element with that value exists in AIndex, the greatest "
"position lower than specified one is retrieved (so that positions got by subsequent "
"calls to FindNext are in descending order). When there are no more elements with "
"required value, negative number is returned. Unlinked elements are ignored.&][s1; "
"[*C@3 i]-|Position of the element.&][s1; [*/ Return value]-|Position of the previous "
"element with same value.&][s0;3 &][s4;:`:`:AIndex`:`:FindAdd`(const T`&`,unsigned`): "
"int [* FindAdd](const T`& [*@3 key], unsigned [*@3 `_hash])&][s6; Retrieves position "
"of first element with specified value in AIndex, using a precomputed hash value. "
"Precomputed hash value must be same as hash value that would be result of HashFn. "
"If multi`-key ordering is not broken and more than one element with the same value "
"exists in AIndex, the lowest position is retrieved. If element does not exist in "
"AIndex, it is added to AIndex and position of this newly added element is returned. "
"Unlinked elements are ignored.&][s1; [*C@3 key]-|Element to find or add.&][s1; [*C@3 "
"`_hash]-|Precomputed hash value.&][s1; [*/ Return value]-|Position of the found or "
"added element.&][s0;3 &][s4;:`:`:AIndex`:`:FindAdd`(const T`&`): int [* FindAdd")
TOPIC_TEXT(
"](const T`& [*@3 key])&][s6; Retrieves position of first element with specified "
"value in AIndex. If multi`-key ordering is not broken and more than one element with "
"the same value exists in AIndex, lowest position is retrieved. If element does not "
"exist in AIndex, it is added to AIndex and position of this newly added element is "
"returned. Unlinked elements are ignored.&][s1; [*C@3 key]-|Element to find or add.&][s1; "
"[*/ Return value]-|Position of the found or added element.&][s0;3 &][s4;:`:`:AIndex`:`:operator`<`<`(const "
"T`&`): AIndex`& [* operator]<<(const T`& [*@3 x])&][s6; Operator replacement of [* "
"void Add(const T`& x)]. By returning reference to AIndex it allows adding multiple "
"elements in a single expression, thus e.g. allowing to construct a temporary Index "
"as part of an expression like Foo((Index<int>() << 1 << 2)).&][s11; Requires T to "
"have deep copy constructor.&][s11; Invalidates iterators to AIndex.&][s11; Invalidates "
"references to Index.&][s1; [*C@3 newt]-|Element to be added.&][s1; [*/ Return value]-|Reference "
"to AIndex.&][s0;3 &][s4;:`:`:AIndex`:`:Unlink`(int`): void [* Unlink](int [*@3 i])&][s6; "
"Unlinks the element at the specified position. The unlinked item stays in AIndex "
"but is ignored by any Find operation.&][s1; [*C@3 i]-|Position of item to unlink.&][s0;3 "
"&][s4;:`:`:AIndex`:`:Put`(const T`&`,unsigned`):~~~.992; int [* Put](const T`& [*@3 "
"x], unsigned [*@3 `_hash])&][s6; If there are any unlinked elements in AIndex, one "
"of them is replaced by specified value. If there are no unlinked elements, the element "
"with the specified value is appended to the end of AIndex using [* Add]. The precomputed "
"hash should be same as the result of HashFn. The position of the placed element is "
"returned.&][s11; Invalidates multi`-key ordering.&][s11; Requires T to have deep "
"copy constructor.&][s11; Invalidates iterators to AIndex.&][s11; Invalidates references "
"to Index.&][s1; [*C@3 x]-|Element to put into AIndex.&][s1; [*C@3 `_hash]-|Precomputed "
"hash value.&][s1; ")
TOPIC_TEXT(
"[*/ Return value]-|Position where the element was placed.&][s0;3 &][s4;:`:`:AIndex`:`:Put`(const "
"T`&`): int [* Put](const T`& [*@3 x])&][s6; If there are any unlinked elements in "
"AIndex, one of them is replaced by specified value. If there are no unlinked elements, "
"the element with the specified value is appended to the end of AIndex using [* Add]. "
"The position of the placed element is returned.&][s11; Invalidates multi`-key ordering.&][s11; "
"Requires T to have deep copy constructor.&][s11; Invalidates iterators to AIndex.&][s1; "
"[*C@3 x]-|Element to put into AIndex.&][s1; [*/ Return value]-|Position where element "
"is placed.&][s0;3 &][s4;:`:`:AIndex`:`:FindPut`(const T`&`,unsigned`): int [* FindPut](const "
"T`& [*@3 key], unsigned [*@3 `_hash])&][s6; Retrieves the position of the first element "
"with the specified value in AIndex, using a precomputed hash value. The precomputed "
"hash value must be the same as the hash value that would be the result of HashFn. "
"If the specified value does not exist in the AIndex, it is placed to it using [* "
"Put(const T`& x, unsigned `_hash).] The position of the found or placed element is "
"returned.&][s11;~~~.992; Invalidates multi`-key ordering.&][s11; Requires T to have "
"deep copy constructor.&][s11; Invalidates iterators to AIndex.&][s11; Invalidates "
"references to Index.&][s1; [*C@3 key]-|Element to find or put.&][s1; [*C@3 `_hash]-|Precomputed "
"hash value.&][s1; [*/ Return value]-|Position of the found or placed element.&][s0;3 "
"&][s4;:`:`:AIndex`:`:FindPut`(const T`&`): int [* FindPut](const T`& [*@3 key])&][s6; "
"Retrieves the position of the first element with the specified value in AIndex. If "
"the element does not exist in the AIndex, it is placed to it using [* Put(const T`& "
"x).] The position of the found or placed element is returned.&][s11; Invalidates "
"multi`-key ordering.&][s11; Requires T to have deep copy constructor.&][s11; Invalidates "
"iterators to AIndex.&][s11; Invalidates references to Index.&][s1; [*C@3 key]-|Element "
"to find or p")
TOPIC_TEXT(
"ut.&][s1; [*C@3 `_hash]-|Precomputed hash value.&][s1; [*/ Return value]-|Position "
"of the found or placed element.&][s0;3 &][s4;:`:`:AIndex`:`:Set`(int`,const T`&`,unsigned`): "
"void [* Set](int [*@3 i], const T`& [*@3 x], unsigned [*@3 `_hash])&][s6; Replaces "
"the element at the specified position with a new element with the specified value, "
"using a precomputed hash`-value. Speed of this operation depends on the total number "
"of elements with the same value as the specified one.&][s11; Requires T to have deep "
"copy constructor.&][s11; Invalidates iterators to AIndex.&][s11; Invalidates references "
"to Index.&][s1; [*C@3 i]-|Position of the element.&][s1; [*C@3 x]-|Value to set.&][s1; "
"[*C@3 `_hash]-|Precomputed hash value.&][s0;3 &][s4;:`:`:AIndex`:`:Set`(int`,const "
"T`&`): void [* Set](int [*@3 i], const T`& [*@3 x])&][s6; Replaces the element at "
"the specified position with a new element with the specified value. Speed of this "
"operation depends on total number of elements with the same value as specified the "
"specified one.&][s11; Requires T to have deep copy constructor.&][s11; Invalidates "
"iterators to AIndex.&][s11; Invalidates references to Index.&][s1; [*C@3 i]-|Position "
"of the element.&][s1; [*C@3 x]-|Value to set.&][s0;3 &][s4;:`:`:AIndex`:`:operator`[`]`(int`)const: "
"const T`& [* operator]`[`](int [*@3 i]) const&][s6; Returns the element at the specified "
"position.&][s1; [*C@3 i]-|Position of the element.&][s1; [*/ Return value]-|Constant "
"reference to element.&][s0;3 &][s4;:`:`:AIndex`:`:GetCount`(`)const: int [* GetCount]() "
"const&][s6; Returns number of elements in AIndex.&][s1; [*/ Return value]-|Actual "
"number of elements.&][s0;3 &][s4;:`:`:AIndex`:`:IsEmpty`(`)const: bool [* IsEmpty]() "
"const&][s6; Tests whether AIndex is empty. Same as GetCount() `=`= 0.&][s1; [*/ Return "
"value]-|true if AIndex is empty, false otherwise.&][s0;3 &][s4;:`:`:AIndex`:`:Clear`(`): "
"void [* Clear]()&][s6; Removes all elements from AIndex.&][s0;3 &][s4;:`:`:AIndex`:`:ClearIndex`(`)const: "
"voi")
TOPIC_TEXT(
"d [* Reindex]() const&][s6; Restores multi`-key ordering.&][s0;3 &][s4;:`:`:AIndex`:`:UnlinkKey`(const "
"T`&`,unsigned`): int [* UnlinkKey](const T`& [*@3 k], unsigned [*@3 h])&][s6; Unlinks "
"all elements with specified value using precomputed hash`-value. Unlinked elements "
"stay in AIndex but are ignored by any Find operations. Precomputed hash value must "
"be same as hash value that would be result of HashFn. &][s1; [*C@3 k]-|Value of elements "
"to unlink.&][s1; [*C@3 h]-|Precomputed hash value.&][s1; [*/ Return value]-|Number "
"of elements unlinked.&][s0;3 &][s4;:`:`:AIndex`:`:UnlinkKey`(const T`&`): int [* "
"UnlinkKey](const T`& [*@3 k])&][s6; Unlinks all elements with specified value. Unlinked "
"elements remain in the AIndex but are ignored by any Find operations.&][s1; [*C@3 "
"k]-|Value of elements to unlink.&][s1; [*/ Return value]-|Number of elements unlinked.&][s0;3 "
"&][s4;:`:`:AIndex`:`:IsUnlinked`(int`)const: bool [* IsUnlinked](int [*@3 i]) const&][s6; "
"Tests whether the element at the specified position is unlinked.&][s1; [*C@3 i]-|Position.&][s1; "
"[*/ Return value]-|true if element is unlinked.&][s0;3 &][s4;:`:`:AIndex`:`:Sweep`(`): "
"void [* Sweep]()&][s6; Removes all unlinked elements from AIndex. Complexity of the "
"operation depends on the number of elements in AIndex, not on the number of unlinked "
"elements. Also restores multi`-key ordering.&][s0;3 &][s4;:`:`:AIndex`:`:Insert`(int`,const "
"T`&`,unsigned`): void [* Insert](int [*@3 i], const T`& [*@3 k], unsigned [*@3 h])&][s6; "
"Inserts an element with the specified value at the specified position, using a precomputed "
"hash value. The precomputed hash value must be the same as the hash value that would "
"be the result of HashFn. This is a slow operation, especially when combined with "
"any search operations.&][s11; Requires T to have deep copy constructor.&][s11; Invalidates "
"iterators to AIndex.&][s11; Invalidates references to Index.&][s1; [*C@3 i]-|Insert "
"position.&][s1; [*C@3 k]-|Element to insert.&][s1; [*C@3 h]-|Precomputed")
TOPIC_TEXT(
" hash value.&][s0;3 &][s4;:`:`:AIndex`:`:Insert`(int`,const T`&`): void [* Insert](int "
"[*@3 i], const T`& [*@3 k])&][s6; Inserts an element with the specified value at "
"the specified position. This is a slow operation, especially when combined with any "
"search operations.&][s11; Requires T to have deep copy constructor.&][s11; Invalidates "
"iterators to AIndex.&][s11; Invalidates references to Index.&][s1; [*C@3 i]-|Insert "
"position.&][s1; [*C@3 k]-|Element to insert.&][s0;3 &][s4;:`:`:AIndex`:`:Remove`(int`): "
"void [* Remove](int [*@3 i])&][s6; Removes the element at the specified position. "
"This is a slow operation, especially when combined with any search operations.&][s11; "
"Invalidates iterators to AIndex.&][s11; Invalidates references to Index.&][s1; [*C@3 "
"i]-|Position of the element to remove.&][s0;3 &][s4;:`:`:AIndex`:`:Remove`(const "
"int`*`,int`): void [* Remove](const int `*[*@3 sorted`_list], int [*@3 count])&][s6; "
"Removes multiple elements from AIndex. Time of operation only slightly depends on "
"the number of removed elements. This is a slow operation, especially when combined "
"with any search operations.&][s11; Invalidates iterators to AIndex.&][s11; Invalidates "
"references to Index.&][s1; [*C@3 sorted`_list]-|Pointer to array of positions to "
"remove, in ascending order.&][s1; [*C@3 count]-|Number of elements to remove.&][s0;3 "
"&][s4;:`:`:AIndex`:`:Remove`(const`:`:Vector`<int`>`&`): void [* Remove](const Vector<int>`& "
"[*@3 sorted`_list])&][s6; Removes multiple elements from AIndex. Same as Remove(sorted`_list, "
"sorted`_list.GetCount()).&][s11; Invalidates iterators to AIndex.&][s11; Invalidates "
"references to Index.&][s1; [*C@3 sorted`_list]-|Sorted Vector of positions to remove.&][s0;3 "
"&][s4;:`:`:AIndex`:`:RemoveKey`(const T`&`,unsigned`): int [* RemoveKey](const T`& "
"[*@3 k], unsigned [*@3 h])&][s6; Removes all elements with the specified value using "
"a precomputed hash`-value. The precomputed hash value must be the same as the hash "
"value that would be the result of Ha")
TOPIC_TEXT(
"shFn. This is a slow operation, especially when combined with any search operations.&][s1; "
"[*C@3 k]-|Value of the elements to remove.&][s1; [*C@3 h]-|Precomputed hash value.&][s0;3 "
"&][s4;:`:`:AIndex`:`:RemoveKey`(const T`&`): int [* RemoveKey](const T`& [*@3 k])&][s6; "
"Removes all elements with the specified value. This is a slow operation, especially "
"when combined with any search operations.&][s1; [*C@3 k]-|Value of the elements to "
"remove.&][s0;3 &][s4;:`:`:AIndex`:`:Trim`(int`): void [* Trim](int [*@3 n])&][s6; "
"Reduces the number of elements in AIndex to the specified number. Requested number "
"must be less than or equal to actual number of elements in AIndex.&][s1; [*C@3 n]-|Requested "
"number of elements.&][s0;3 &][s4;:`:`:AIndex`:`:Drop`(int`): void [* Drop](int [*@3 "
"n] `= 1)&][s6; Drops the specified number of elements from the end of the AIndex "
"(same as Trim(GetCount() `- n)).&][s1; [*C@3 n]-|Number of elements.&][s0;3 &][s4;:`:`:AIndex`:`:Top`(`)const: "
"const [* T]`& [* Top]() const&][s6; Returns a reference to the last element in the "
"AIndex.&][s1; [*/ Return value]-|Reference of thr last element in the AIndex.&][s0;3 "
"&][s4;:`:`:AIndex`:`:Reserve`(int`): void [* Reserve](int [*@3 n])&][s6; Reserves "
"capacity. If the requested capacity is greater than current capacity, capacity is "
"increased to the requested value.&][s1; [*C@3 n]-|Requested capacity.&][s0;3 &][s4;:`:`:AIndex`:`:Shrink`(`): "
"void [* Shrink]()&][s6; Minimizes the memory consumption of AIndex by decreasing "
"the capacity to the number of elements.&][s0;3 &][s4;:`:`:AIndex`:`:GetAlloc`(`)const: "
"int [* GetAlloc]() const&][s6; Returns the current capacity of AIndex.&][s1; [*/ "
"Return value]-|Capacity of AIndex.&][s0;3 &][s4;:`:`:AIndex`:`:Serialize`(`:`:Stream`&`): "
"void [* Serialize](Stream`& [*@3 s])&][s6; Serializes content of AIndex to/from Stream. "
"Works only if NTL is used as part of UPP.&][s11; Requires T to have serialization "
"operator defined.&][s1; [*C@3 s]-|Target/source stream.&][s0;3 &][s4;:`:`:AInde")
TOPIC_TEXT(
"x`:`:PickKeys`(`)pick`_: V [* PickKeys]() pick&][s6; Returns a basic random access "
"container of elements. Destroys AIndex by picking.&][s1; [*/ Return value]-|Basic "
"random access container of elements in AIndex.&][s0;3 &][s4;:`:`:AIndex`:`:GetKeys`(`)const: "
"const V`& [* GetKeys]() const&][s6; Returns a constant reference to basic random "
"access container of elements.&][s1; [*/ Return value]-|Constant reference to a basic "
"random access container of elements.&][s0;3 &][s4;:`:`:AIndex`:`:operator`=`(pick`_ "
"V`&`): AIndex`& [* operator]`=(pick`_ V`& [*@3 s])&][s6; Assigns basic random access "
"container to AIndex. Transfers the source container in short constant time, but destroys "
"it by picking.&][s1; [*C@3 s]-|Source container.&][s0;3 &][s4;:`:`:AIndex`:`:operator`<`<`=`(const "
"V`&`): AIndex`& [* operator]<<`=(const V`& [*@3 s])&][s6; Assigns the basic random "
"access container to AIndex, while preserving the value of the source container.&][s11; "
"Requires T to have deep copy constructor or optional deep copy.&][s1; [*C@3 s]-|Source "
"container.&][s0;3 &][s4;:`:`:AIndex`:`:AIndex`(pick`_ V`&`): [* AIndex](pick`_ V`& "
"[*@3 s])&][s6; Pick`-constructs AIndex from a basic random access container. Transfers "
"the source container in short constant time, but destroys it by picking.&][s1; [*C@3 "
"s]-|Source basic random access container.&][s0;3 &][s4;:`:`:AIndex`:`:AIndex`(const "
"V`&`,int`): [* AIndex](const V`& [*@3 s], int)&][s6; Deep`-copy constructs AIndex "
"from basic random access container.&][s11; Requires T to have deep copy constructor "
"or optional deep copy constructor.&][s1; [*C@3 s]-|Source AIndex.&][s0;3 &][s4;:`:`:AMap`:`:ValueType`:`:typedef:~~~.992;* "
"[* typedef T ]ValueType&][s6; Typedef of T for use in templated algorithms.&][s0;3 "
"&][s4;:`:`:AIndex`:`:ConstIterator`:`:typedef: typedef [/ type] [* ConstIterator];&][s6; "
"Constant iterator type.&][s0;3 &][s4;:`:`:AIndex`:`:Begin`(`)const: ConstIterator "
"[* Begin]() const&][s6; Returns a constant iterator to the first element in AInd")
TOPIC_TEXT(
"ex.&][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:AIndex`:`:End`(`)const: "
"ConstIterator [* End]() const&][s6; Returns a constant iterator to the position just "
"beyond the last element in AIndex.&][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:AIndex`:`:GetIter`(int`)const: "
"ConstIterator [* GetIter](int [*@3 pos]) const&][s6; Returns a constant iterator "
"to the element at specified position. Same as [* Begin() `+ i]. The benefit of this "
"method is that [* pos] is range checked in debug mode.&][s1; [*C@3 pos]-|Required "
"position.&][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:AIndex`:`:AIndex`(`): "
"[* AIndex]()&][s6; Constructor. Constructs an empty AIndex.&][s0;3 &][s4;:`:`:AIndex`:`:AIndex`(const`:`:AIndex`&`,int`): "
"[* AIndex](const [* AIndex]`& [*@3 s], int)&][s6; Optional deep copy constructor.&][s11; "
"Requires T to have deep copy constructor or optional deep copy constructor.&][s1; "
"[*C s]-|Source AIndex.&][s0;3 ]")
