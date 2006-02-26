TITLE("BiArray")
REF("::BiArray::class")
REF("::BiArray::AddHead()")
REF("::BiArray::AddTail()")
REF("::BiArray::AddHead(const T&)")
REF("::BiArray::AddTail(const T&)")
REF("::BiArray::AddHeadPick(pick_ T&)")
REF("::BiArray::AddTailPick(pick_ T&)")
REF("::BiArray::AddHead(T*)")
REF("::BiArray::AddTail(T*)")
REF("::BiArray::Head()")
REF("::BiArray::Tail()")
REF("::BiArray::Head()const")
REF("::BiArray::Tail()const")
REF("::BiArray::DropHead()")
REF("::BiArray::DropTail()")
REF("::BiArray::DetachHead()")
REF("::BiArray::DetachTail()")
REF("::BiArray::operator[](int)")
REF("::BiArray::operator[](int)const")
REF("::BiArray::GetCount()const")
REF("::BiArray::IsEmpty()const")
REF("::BiArray::Clear()")
REF("::BiArray::Shrink()")
REF("::BiArray::Reserve(int)")
REF("::BiArray::GetAlloc()const")
REF("friend BiArray& operator<<(BiArray& b, const T& x)")
REF("friend BiArray& operator>>(const T& x, BiArray& b)")
REF("::BiArray::Serialize(::Stream&)")
REF("::BiArray::IsPicked()const")
REF("::BiArray::BiArray(const::BiArray&,int)")
REF("::BiArray::BiArray(pick_::BiArray&)")
REF("::BiArray::operator=(pick_::BiArray&)")
REF("::BiArray::BiArray()")
REF("::BiArray::~BiArray()")
REF("::BiArray::ValueType::typedef")
REF("::BiArray::Iterator::typedef")
REF("::BiArray::ConstIterator::typedef")
REF("::BiArray::Begin()")
REF("::BiArray::End()")
REF("::BiArray::GetIter(int)")
REF("::BiArray::Begin()const")
REF("::BiArray::End()const")
REF("::BiArray::GetIter(int)const")
REF("::BiArray::Swap(::BiArray&,::BiArray&)")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2;~~~64; BiArray&][s10;~~~64; "
"template <class [*@4 T]>&][s0;3 &][s10;:`:`:BiArray`:`:class:~~~64;* [* class ]BiArray&][s0; "
"&][s12; [*C@4 T]-|Type or base class of elements stored in the BiArray. There is "
"no common requirement for T.&][s5; The universa")
TOPIC_TEXT(
"l form of bidirectional random access container. Its features are derived from the "
"fact that it is typically implemented using an indirect container of pointers to "
"T like BiVector<T`*>. Nevertheless it supports common concepts as ownership of contained "
"elements, reference (not pointer) access to elements and so on.&][s5; It provides "
"almost all operations of BiVector with the same semantics and alm")
TOPIC_TEXT(
"ost any BiVector can be directly replaced by BiArray. On the other hand, it provides "
"some special operations impossible for BiVector and most important, it never invalidates "
"references (that means C`+`+ references and pointers) to elements (it often invalidates "
"iterators, though).&][s5; BiArray can also be used to store polymorphic elements "
"`- stored elements could be derived from T. To store such")
TOPIC_TEXT(
" elements, you pass pointer to element previously created on the heap. Still, BiArray "
"takes over ownership of such element (it e.g. deletes it when appropriate). You can "
"also use this method to create BiArray of elements that do not have pick nor deep "
"copy constructor.&][s5; There are also operations that allow detaching an element "
"from BiArray, removing it but not destroying. Pointer to such elem")
TOPIC_TEXT(
"ent is returned from these operations and BiArray gives up ownership.&][s5; Disadvantage "
"of BiArray over BiVector is performance `- most operations are significantly slower "
"than with BiVectors (by factor up to 8, it depends on speed of malloc/free).&][s5; "
"As for memory, for small sized elements, BiArray memory consumption is significantly "
"higher than BiVector consumption. As the size of the elemen")
TOPIC_TEXT(
"ts grow, BiArray starts to be better than BiVector.&][s5;~~~.704; Iterators to BiArray "
"satisfy all C`+`+ standard library requirements for random access iterator plus they "
"allow assignment (and copy constructor) and testing for 0 (that is NULL) pointer.&][s5; "
"Like any other NTL container, BiArray is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ "
"moveable][*/  ]type with [*/^topic`:`/`/Core`/srcd")
TOPIC_TEXT(
"oc`/pick`_`$en`-us^ pick and optional deep copy] transfer semantics. Calling methods "
"of picked BiArray is logic error with the exceptions of&][s0;C+75 void [* operator`=](pick`_ "
"BiArray`& [*@3 v])&][s0;/+75 [/C void ][*/C operator<<`=][/C (const BiArray`& ][*/C@3 "
"v][/C )][/  ](defined using DeepCopyOptionTemplate)&][s0;C+75 void [* Clear]()&][s0;C+75 "
"bool [* IsPicked]() const&][s5; Optional deep c")
TOPIC_TEXT(
"opy is implemented through DeepCopyOptionTemplate macro.&][s3; Members&][s0;3 &][s4;:`:`:BiArray`:`:AddHead`(`): "
"T`& [* AddHead]()&][s6; Adds a new default constructed element at the head of the "
"BiArray. The new element will be at position 0.&][s11; Requires T to have default "
"constructor.&][s11; Invalidates iterators to the BiArray.&][s1; [*/ Return value]-|Reference "
"to the newly added default con")
TOPIC_TEXT(
"structed element.&][s0;3 &][s4;:`:`:BiArray`:`:AddTail`(`): T`& [* AddTail]()&][s6; "
"Adds a new default constructed element at the tail of BiArray. The new element will "
"be at position GetCount() `- 1.&][s11; Requires T to have default constructor.&][s11; "
"Invalidates iterators to the BiArray.&][s1; [*/ Return value]-|Reference to the newly "
"added default constructed element.&][s0;3 &][s4;:`:`:BiArray")
TOPIC_TEXT(
"`:`:AddHead`(const T`&`): void [* AddHead](const T`& [*@3 x])&][s6; Adds a new element "
"with the specified value at the head of BiArray. The new element will be at position "
"0.&][s11; Requires T to have deep copy constructor.&][s11; Invalidates iterators "
"to the BiArray.&][s1; [*C@3 x]-|The value that is copied to the newly created element.&][s0;3 "
"&][s4;:`:`:BiArray`:`:AddTail`(const T`&`): void [* A")
TOPIC_TEXT(
"ddTail](const T`& [*@3 x])&][s6; Adds a new element with the specified value at "
"the tail of BiArray. The new element will be at position GetCount() `- 1.&][s11; "
"Requires T to have deep copy constructor.&][s11; Invalidates iterators to the BiArray.&][s1; "
"[*C@3 x]-|The value that is copied to the newly created element.&][s0;3 &][s4;:`:`:BiArray`:`:AddHeadPick`(pick`_ "
"T`&`): void [* AddHeadPick](pick")
TOPIC_TEXT(
"`_ T`& [*@3 x])&][s6; Adds a new element at the head of BiArray and picks value "
"of the parameter. The new element will be at position 0.&][s11; Requires T to have "
"pick constructor.&][s11; Invalidates iterators to the BiArray.&][s1; [*C@3 x]-|Source "
"instance of T that is to be picked.&][s0;3 &][s4;:`:`:BiArray`:`:AddTailPick`(pick`_ "
"T`&`): void [* AddTailPick](pick`_ T`& [*@3 x])&][s6; Adds a new e")
TOPIC_TEXT(
"lement at the tail of BiArray and picks the value of the parameter. The new element "
"will be at position GetCount() `- 1.&][s11; Requires T to have pick constructor.&][s11; "
"Invalidates iterators to the BiArray.&][s1; [*C@3 x]-|Source instance of T that is "
"to be picked.&][s0;3 &][s4;:`:`:BiArray`:`:AddHead`(T`*`): void [* AddHead](T `*[*@3 "
"newt])&][s6; Adds a new element at the head of BiArray. Elem")
TOPIC_TEXT(
"ent is specified by a pointer to the object. BiArray takes over ownership of the "
"object. This variant allows use of BiArray as polymorphic container, because the "
"type of added element can also be derived from T as well. No constructor is applied. "
"The new element will be at position 0.&][s11; Invalidates iterators to the BiArray.&][s1; "
"[*C@3 newt]-|The object to be added.&][s1; [*/ Return value]-|R")
TOPIC_TEXT(
"eference to the object `= [* `*newt].&][s0;3 &][s4;:`:`:BiArray`:`:AddTail`(T`*`): "
"void [* AddTail](T `*[*@3 newt])&][s6; Adds new element at the tail of BiArray. Element "
"is specified by a pointer to the object. BiArray takes over ownership of this this "
"object. This variant allows use of BiArray as polymorphic container, because the "
"type of added element can also be derived from T as well. No cons")
TOPIC_TEXT(
"tructor is applied. The new element will be at position GetCount() `- 1.&][s11; "
"Invalidates iterators to the BiArray.&][s1; [*C@3 newt]-|The object to be added.&][s1; "
"[*/ Return value]-|Reference to the object `- [* `*newt].&][s0;3 &][s4;:`:`:BiArray`:`:Head`(`): "
"T`& [* Head]()&][s6; Returns reference to the head of the BiArray. Same as operator`[`](0).&][s1; "
"[*/ Return value]-|Reference to the he")
TOPIC_TEXT(
"ad of BiArray.&][s0;3 &][s4;:`:`:BiArray`:`:Tail`(`): T`& [* Tail]()&][s6; Returns "
"reference to the tail of the BiArray. Same as operator`[`](GetCount() `- 1).&][s1; "
"[*/ Return value]-|Reference to the tail of BiArray.&][s0;3 &][s4;:`:`:BiArray`:`:Head`(`)const: "
"const T`& [* Head]() const&][s6; Returns reference to the head of the BiArray. Same "
"as operator`[`](0).&][s1; [*/ Return value]-|Constant")
TOPIC_TEXT(
" reference to the head of BiArray.&][s0;3 &][s4;:`:`:BiArray`:`:Tail`(`)const: const "
"T`& [* Tail]() const&][s6; Returns reference to the tail of the BiArray. Same as "
"operator`[`](GetCount() `- 1).&][s1; [*/ Return value]-|Constant reference to the "
"tail of BiArray.&][s0;3 &][s4;:`:`:BiArray`:`:DropHead`(`): void [* DropHead]()&][s6; "
"Removes element at the head of the BiArray.&][s11; Invalidates ite")
TOPIC_TEXT(
"rators to the BiArray.&][s0;3 &][s4;:`:`:BiArray`:`:DropTail`(`): void [* DropTail]()&][s6; "
"Removes element at the tail of the BiArray.&][s11; Invalidates iterators to the BiArray.&][s0;3 "
"&][s4;:`:`:BiArray`:`:DetachHead`(`): T `*[* DetachHead]()&][s6; Removes element "
"at the head of the BiArray, giving up ownership. Client is responsible for deletion "
"of the element.&][s11; Invalidates iterators to")
TOPIC_TEXT(
" the BiArray.&][s1; [*/ Return value]-|Pointer to the element allocated on the heap.&][s0;3 "
"&][s4;:`:`:BiArray`:`:DetachTail`(`): T `*[* DetachTail]()&][s6; Removes element "
"at the tail of the BiArray, giving up ownership. Client is responsible for deletion "
"of the element.&][s11; Invalidates iterators to the BiArray.&][s1; [*/ Return value]-|Pointer "
"to the element allocated on the heap.&][s0;3 &][s")
TOPIC_TEXT(
"4;:`:`:BiArray`:`:operator`[`]`(int`): T`& [* operator`[`]](int [*@3 i])&][s6; Returns "
"a reference to the element at the specified position.&][s1; [*C@3 i]-|Position of "
"the element.&][s1; [*/ Return value]-|Reference to the element.&][s0;3 &][s4;:`:`:BiArray`:`:operator`[`]`(int`)const: "
"const T`& [* operator`[`]](int [*@3 i]) const&][s6; Returns a reference to the element "
"at the specified position")
TOPIC_TEXT(
".&][s1; [*C@3 i]-|Position of the element.&][s1; [*/ Return value]-|Constant reference "
"to the element.&][s0;3 &][s4;:`:`:BiArray`:`:GetCount`(`)const: int [* GetCount]() "
"const&][s6; Returns the number of elements in the BiArray.&][s1; [*/ Return value]-|Actual "
"number of elements.&][s0;3 &][s4;:`:`:BiArray`:`:IsEmpty`(`)const: bool [* IsEmpty]() "
"const&][s6; Tests whether the BiArray is empty. Same ")
TOPIC_TEXT(
"as GetCount() `=`= 0.&][s1; [*/ Return value]-|true if Vector is empty, false otherwise.&][s0;3 "
"&][s4;:`:`:BiArray`:`:Clear`(`): void [* Clear]()&][s6; Removes all elements from "
"the BiArray.&][s0;3 &][s4;:`:`:BiArray`:`:Shrink`(`): void [* Shrink]()&][s6; Minimizes "
"memory consumption of the BiArray by minimizing capacity.&][s0;3 &][s4;:`:`:BiArray`:`:Reserve`(int`): "
"void [* Reserve](int [*@3 n])&]")
TOPIC_TEXT(
"[s6; Reserves capacity. If required capacity is greater than the current capacity, "
"capacity is increased to the required value.&][s1; [*C@3 n]-|Required capacity.&][s0;3 "
"&][s4;:`:`:BiArray`:`:GetAlloc`(`)const: int [* GetAlloc]() const&][s6; Returns current "
"capacity of BiArray.&][s1; [*/ Return value]-|Capacity of the BiArray.&][s0; &][s4;:friend "
"BiArray`& operator`<`<`(BiArray`& b`, const T`& x`)")
TOPIC_TEXT(
": friend BiArray`& [* operator]<<(BiArray`& [*@3 b], const T`& [*@3 x])&][s6; Operator "
"replacement of [* void AddTail(const T`&x)]. By returning a reference to the BiArray "
"it allows adding more elements in a single expression, thus e.g. allowing to construct "
"a temporary BiArray as part of an expression like Foo(BiArray<int>() << 1 << 2 << "
"4).&][s11; Requires T to have deep copy constructor.&][s11;")
TOPIC_TEXT(
" Invalidates iterators to the BiArray.&][s1; [*C@3 x]-|The value that is copied "
"to the newly created element.&][s1; [*/ Return value]-|Reference to the BiArray (that "
"is `*this).&][s0; &][s4;:friend BiArray`& operator`>`>`(const T`& x`, BiArray`& b`): "
"friend BiArray`& [* operator]>>(const T`& [*@3 x], BiArray`& [*@3 b])&][s6; Operator "
"replacement of [* void AddHead(const T`&x)]. By returning a refe")
TOPIC_TEXT(
"rence to the BiArray it allows adding more elements in a single expression, thus "
"e.g. allowing to construct a temporary BiArray as part of an expression like Foo(1 "
">> (2 >> BiArray<int>())).&][s11; Requires T to have deep copy constructor.&][s11; "
"Invalidates iterators to the BiArray.&][s1; [*C@3 x]-|The value that is copied to "
"the newly created element.&][s1; [*/ Return value]-|Reference to the Bi")
TOPIC_TEXT(
"Array (that is `*this).&][s0;3 &][s4;:`:`:BiArray`:`:Serialize`(`:`:Stream`&`): "
"void [* Serialize](Stream`& [*@3 s])&][s6; Serializes the content of the BiArray "
"to/from a Stream. Works only if NTL is used as part of UPP.&][s11; Requires T to "
"have serialization operator defined.&][s1; [*C@3 s]-|Target/source stream.&][s0;3 "
"&][s4;:`:`:BiArray`:`:IsPicked`(`)const: bool [* IsPicked]() const&][s6; Ret")
TOPIC_TEXT(
"urns [* true] if BiArray is in picked state.&][s1; [*/ Return value]-|[* true] if "
"BiArray is in picked state, [* false] otherwise.&][s0;3 &][s4;:`:`:BiArray`:`:BiArray`(const`:`:BiArray`&`,int`): "
"[* BiArray](const [* BiArray]`& [*@3 v], int)&][s6; Optional deep copy constructor.&][s11; "
"Requires T to have deep copy constructor or optional deep copy constructor if Array "
"stores only objects of type T")
TOPIC_TEXT(
".&][s11; Requires polymorphic deep copy if Array stores also objects of type derived "
"from T.&][s1; [*C@3 v]-|Source Array.&][s0;3 &][s4;:`:`:BiArray`:`:BiArray`(pick`_`:`:BiArray`&`): "
"[* BiArray](pick`_ [* BiArray]`& [*@3 src])&][s6; Pick constructor. Transfers source "
"BiArray in low constant time, but destroys it by picking. &][s1; [*C@3 v]-|Source "
"BiArray.&][s0;3 &][s4;:`:`:BiArray`:`:operator`=`")
TOPIC_TEXT(
"(pick`_`:`:BiArray`&`): void [* operator]`=(pick`_ BiArray`& [*@3 src])&][s6; Pick "
"operator. Transfers source Array in low constant time, but destroys it by picking.&][s1; "
"[*C@3 v]-|Source Array.&][s0;3 &][s4;:`:`:BiArray`:`:BiArray`(`): [* BiArray]()&][s6; "
"Default constructor. Constructs empty BiArray.&][s0;3 &][s4;:`:`:BiArray`:`:`~BiArray`(`): "
"`~[* BiArray]()&][s6; Destructor. Invokes the destr")
TOPIC_TEXT(
"uctor of every element in the BiArray.&][s0;3 &][s4;:`:`:BiArray`:`:ValueType`:`:typedef:* "
"[* typedef T ]ValueType&][s6; Typedef of T for use in templated algorithms.&][s0;3 "
"&][s4;:`:`:BiArray`:`:Iterator`:`:typedef: typedef [/ type] [* Iterator];&][s6; Iterator "
"type.&][s0;3 &][s4;:`:`:BiArray`:`:ConstIterator`:`:typedef: typedef [/ type] [* "
"ConstIterator];&][s6; Constant iterator type.&][s0;3 &][")
TOPIC_TEXT(
"s4;:`:`:BiArray`:`:Begin`(`): Iterator [* Begin]()&][s6; Returns a non`-constant "
"iterator to the head of the BiArray. &][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:BiArray`:`:End`(`): "
"Iterator [* End]()&][s6; Returns a non`-constant iterator to the position just beyond "
"the tail of the BiArray.&][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:BiArray`:`:GetIter`(int`): "
"Iterator [* GetIt")
TOPIC_TEXT(
"er](int [*@3 pos])&][s6; Returns a non`-constant iterator to the element at the "
"specified position. Same as [* Begin() `+ pos]. The benefit of this method is that "
"[* pos] is range checked in debug mode.&][s1; [*C@3 pos]-|Required position.&][s1; "
"[*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:BiArray`:`:Begin`(`)const: ConstIterator "
"[* Begin]() const&][s6; Returns a constant iterator to the head of")
TOPIC_TEXT(
" the BiArray. &][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:BiArray`:`:End`(`)const: "
"ConstIterator [* End]() const&][s6; Returns a constant iterator to the position just "
"beyond the tail of the Array.&][s1; [*/ Return value]-|Iterator.-|&][s0;3 &][s4;:`:`:BiArray`:`:GetIter`(int`)const: "
"ConstIterator [* GetIter](int [*@3 pos]) const&][s6; Returns a constant iterator "
"to the element at the sp")
TOPIC_TEXT(
"ecified position. Same as [* Begin() `+ pos]. The benefit of this method is that "
"[* pos] is range checked in debug mode.&][s1; [*C@3 pos]-|Required position.&][s1; "
"[*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:BiArray`:`:Swap`(`:`:BiArray`&`,`:`:BiArray`&`): "
"friend void [* Swap](BiArray`& [*@3 a], BiArray`& [*@3 b])&][s6; Specialization of "
"the generic [* Swap] for BiArrays. Swaps BiArray in low c")
TOPIC_TEXT(
"onstant time operation.&][s1; [*C@3 a]-|First BiArray to swap.&][s1; [*C@3 b]-|Second "
"BiArray to swap.]")
