TITLE("BiVector")
REF("::BiVector::AddHead()")
REF("::BiVector::AddTail()")
REF("::BiVector::AddHead(const T&)")
REF("::BiVector::AddTail(const T&)")
REF("::BiVector::AddHeadPick(pick_ T&)")
REF("::BiVector::AddTailPick(pick_ T&)")
REF("::BiVector::Head()")
REF("::BiVector::Tail()")
REF("::BiVector::Head()const")
REF("::BiVector::Tail()const")
REF("::BiVector::DropHead()")
REF("::BiVector::DropTail()")
REF("::BiVector::operator[](int)")
REF("::BiVector::operator[](int)const")
REF("::BiVector::GetCount()const")
REF("::BiVector::IsEmpty()const")
REF("::BiVector::Clear()")
REF("::BiVector::Shrink()")
REF("::BiVector::Reserve(int)")
REF("::BiVector::GetAlloc()const")
REF("friend BiVector& operator<<(BiVector& b, const T& x)")
REF("friend BiVector& operator>>(const T& x, BiVector& b)")
REF("::BiVector::Serialize(::Stream&)")
REF("::BiVector::IsPicked()")
REF("::BiVector::~BiVector()")
REF("::BiVector::BiVector(pick_::BiVector&)")
REF("::BiVector::operator=(pick_::BiVector&)")
REF("::BiVector::BiVector(const::BiVector&,int)")
REF("::BiVector::ValueType::typedef")
REF("::BiVector::Iterator::typedef")
REF("::BiVector::ConstIterator::typedef")
REF("::BiVector::Begin()const")
REF("::BiVector::End()const")
REF("::BiVector::GetIter(int)const")
REF("::BiVector::Begin()")
REF("::BiVector::End()")
REF("::BiVector::GetIter(int)")
REF("::BiVector::Swap(::BiVector&,::BiVector&)")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2;~~~64; BiVector&][s10;~~~64; "
"template <class [*@4 T]>&][s10;~~~64;* [* class ]BiVector&][s0; &][s1;l0;~~~.704; "
"[*C@4 T]-|Type of elements stored in the BiVector. T is required to be [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ "
"moveable][*/  ]and m")
TOPIC_TEXT(
"ust have either [*/ deep copy constructor], [*/ pick constructor] or [*/ default "
"constructor].&][s5; Vector flavor of bidirectional container. Allows adding elements "
"at both sides of sequence in constant amortized time.&][s5; Like any other NTL container, "
"BiVector is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][/  ]type with "
"[*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick and optio")
TOPIC_TEXT(
"nal deep copy] transfer semantics. Calling methods of picked a BiVector is logic "
"error with the exceptions of&][s0;C+75 void [* operator`=](pick`_ Vector`& [*@3 v])&][s0;/+75 "
"[/C void ][*/C operator<<`=][/C (const Vector`& ][*/C@3 v][/C )][/  ](defined using "
"DeepCopyOptionTemplate)&][s0;C+75 void [* Clear]()&][s0;C+75 bool [* IsPicked]() "
"const&][s5; Optional deep copy is implemented through DeepCo")
TOPIC_TEXT(
"pyOptionTemplate macro.&][s3; Members&][s0;3 &][s4;:`:`:BiVector`:`:AddHead`(`): "
"T`& [* AddHead]()&][s6; Adds a new default constructed element at the head of the "
"BiVector. The new element will be at position 0.&][s11; Requires T to have default "
"constructor.&][s11; Invalidates iterators and references to the BiVector.&][s1; [*/ "
"Return value]-|Reference to the newly added default constructed elemen")
TOPIC_TEXT(
"t.&][s0;3 &][s4;:`:`:BiVector`:`:AddTail`(`): T`& [* AddTail]()&][s6; Adds a new "
"default constructed element at the tail of the BiVector. The new element will be "
"at position GetCount() `- 1.&][s11; Requires T to have default constructor.&][s11; "
"Invalidates iterators and references to the BiVector.&][s1; [*/ Return value]-|Reference "
"to the newly added default constructed element.&][s0;3 &][s4;:`:`:")
TOPIC_TEXT(
"BiVector`:`:AddHead`(const T`&`): void [* AddHead](const T`& [*@3 x])&][s6; Adds "
"a new element with the specified value at the head of the BiVector. The new element "
"will be at position 0.&][s11; Requires T to have deep copy constructor.&][s11; Invalidates "
"iterators and references to the BiVector.&][s1; [*C@3 x]-|The value that is copied "
"to the newly created element.&][s0;3 &][s4;:`:`:BiVector`:`:A")
TOPIC_TEXT(
"ddTail`(const T`&`): void [* AddTail](const T`& [*@3 x])&][s6; Adds a new element "
"with the specified value at the tail of the BiVector. The new element will be at "
"position GetCount() `- 1.&][s11; Requires T to have deep copy constructor.&][s11; "
"Invalidates iterators and references to the BiVector.&][s1; [*C@3 x]-|The value that "
"is copied to the newly created element.&][s0;3 &][s4;:`:`:BiVector`:`:")
TOPIC_TEXT(
"AddHeadPick`(pick`_ T`&`): void [* AddHeadPick](pick`_ T`& [*@3 x])&][s6; Adds a "
"new element at the head of the BiVector and picks value of the parameter. The new "
"element will be at position 0.&][s11; Requires T to have pick constructor.&][s11; "
"Invalidates iterators and references to the BiVector.&][s1; [*C@3 x]-|Source instance "
"of T that is to be picked.&][s0;3 &][s4;:`:`:BiVector`:`:AddTailPick`")
TOPIC_TEXT(
"(pick`_ T`&`): void [* AddTailPick](pick`_ T`& [*@3 x])&][s6; Adds a new element "
"at the tail of the BiVector and picks value of the parameter. The new element will "
"be at position GetCount() `- 1.&][s11; Requires T to have pick constructor.&][s11; "
"Invalidates iterators and references to the BiVector.&][s1; [*C@3 x]-|Source instance "
"of T that is to be picked.&][s0;3 &][s4;:`:`:BiVector`:`:Head`(`): ")
TOPIC_TEXT(
"T`& [* Head]()&][s6; Returns a reference to the head of the BiVector. Same as operator`[`](0).&][s1; "
"[*/ Return value]-|Reference to the head of the BiVector.&][s0;3 &][s4;:`:`:BiVector`:`:Tail`(`): "
"T`& [* Tail]()&][s6; Returns a reference to the tail of the BiVector. Same as operator`[`](GetCount() "
"`- 1).&][s1; [*/ Return value]-|Reference to the tail of the BiVector.&][s0;3 &][s4;:`:`:BiVector`:")
TOPIC_TEXT(
"`:Head`(`)const: const T`& [* Head]() const&][s6; Returns a reference to the head "
"of the BiVector. Same as operator`[`](0).&][s1; [*/ Return value]-|Constant reference "
"to the head of the BiVector.&][s0;3 &][s4;:`:`:BiVector`:`:Tail`(`)const: const T`& "
"[* Tail]() const&][s6; Returns a reference to the tail of the BiVector. Same as operator`[`](GetCount() "
"`- 1).&][s1; [*/ Return value]-|Constant ref")
TOPIC_TEXT(
"erence to the tail of the BiVector.&][s0;3 &][s4;:`:`:BiVector`:`:DropHead`(`): "
"void [* DropHead]()&][s6; Removes the element at the head of the BiVector.&][s11; "
"Invalidates iterators and references to the BiVector.&][s0;3 &][s4;:`:`:BiVector`:`:DropTail`(`): "
"void [* DropTail]()&][s6; Removes the element at the tail of the BiVector.&][s11; "
"Invalidates iterators and references to the BiVector.&][s0")
TOPIC_TEXT(
";3 &][s4;:`:`:BiVector`:`:operator`[`]`(int`): T`& [* operator]`[`](int [*@3 i])&][s6; "
"Returns a reference to the element at the specified position.&][s1; [*C@3 i]-|Position "
"of the element.&][s1; [*/ Return value]-|Reference to the element.&][s0;3 &][s4;:`:`:BiVector`:`:operator`[`]`(int`)const: "
"const T`& [* operator]`[`](int [*@3 i]) const&][s6; Returns a reference to the element "
"at the specified")
TOPIC_TEXT(
" position.&][s1; [*C@3 i]-|Position of the element.&][s1; [*/ Return value]-|Constant "
"reference to the element.&][s0;3 &][s4;:`:`:BiVector`:`:GetCount`(`)const: int [* "
"GetCount]() const&][s6; Returns the number of elements in the BiVector.&][s1; [*/ "
"Return value]-|Actual number of elements.&][s0;3 &][s4;:`:`:BiVector`:`:IsEmpty`(`)const: "
"bool [* IsEmpty]() const&][s6; Tests whether the BiVector is")
TOPIC_TEXT(
" empty. Same as GetCount() `=`= 0.&][s1; [*/ Return value]-|true if Vector is empty, "
"false otherwise.&][s0;3 &][s4;:`:`:BiVector`:`:Clear`(`): void [* Clear]()&][s6; "
"Removes all elements from the BiVector.&][s0;3 &][s4;:`:`:BiVector`:`:Shrink`(`):~~~.992; "
"void [* Shrink]()&][s6; Minimizes memory consumption of the BiVector by minimizing "
"capacity.&][s0;3 &][s4;:`:`:BiVector`:`:Reserve`(int`): void ")
TOPIC_TEXT(
"[* Reserve](int [*@3 n])&][s6; Reserves capacity. If required capacity is greater "
"than the current capacity, capacity is increased to the required value.&][s1; [*C@3 "
"n]-|Required capacity.&][s0;3 &][s4;:`:`:BiVector`:`:GetAlloc`(`)const: int [* GetAlloc]() "
"const&][s6; Returns current capacity of the BiVector.&][s1; [*/ Return value]-|Capacity "
"of the BiVector.&][s0; &][s4;:friend BiVector`& operato")
TOPIC_TEXT(
"r`<`<`(BiVector`& b`, const T`& x`): friend BiVector`& [* operator]<<(BiVector`& "
"[*@3 b], const T`& [*@3 x])&][s6; Operator replacement of [* void AddTail(const T`&x)]. "
"By returning a reference to the BiVector it allows adding more elements in a single "
"expression, thus e.g. allowing to construct a temporary BiVector as part of an expression "
"like Foo(BiVector<int>() << 1 << 2 << 4).&][s11; Requires")
TOPIC_TEXT(
" T to have deep copy constructor.&][s11; Invalidates iterators and references to "
"the BiVector.&][s1; [*C@3 x]-|The value that is copied to the newly created element.&][s1; "
"[*/ Return value]-|Reference to the BiVector (that is `*this).&][s0; &][s4;:friend "
"BiVector`& operator`>`>`(const T`& x`, BiVector`& b`): friend BiVector`& [* operator]>>(const "
"T`& [*@3 x], BiVector`& [*@3 b])&][s6; Operator rep")
TOPIC_TEXT(
"lacement of [* void AddHead(const T`&x)]. By returning a reference to the BiVector "
"it allows adding more elements in a single expression, thus e.g. allowing to construct "
"a temporary BiVector as part of an expression like Foo(1 >> (2 >> BiVector<int>())).&][s11; "
"Requires T to have deep copy constructor.&][s11; Invalidates iterators and references "
"to the BiVector.&][s1; [*C@3 x]-|The value that is c")
TOPIC_TEXT(
"opied to the newly created element.&][s1; [*/ Return value]-|Reference to BiVector "
"(that is `*this).&][s0;3 &][s4;:`:`:BiVector`:`:Serialize`(`:`:Stream`&`): void [* "
"Serialize](Stream`& [*@3 s])&][s6; Serializes content of the BiVector to/from the "
"Stream. Works only if NTL is used as part of UPP.&][s11; Requires T to have serialization "
"operator defined.&][s1; [*C@3 s]-|Target/source stream.&][s0;3")
TOPIC_TEXT(
" &][s4;:`:`:BiVector`:`:IsPicked`(`): [* BiVector]()&][s6; Default constructor. "
"Constructs an empty BiVector.&][s0;3 &][s4;:`:`:BiVector`:`:`~BiVector`(`): `~[* "
"BiVector]()&][s6; Destructor. Invokes the destructor of every element in the BiVector.&][s0;3 "
"&][s4;:`:`:BiVector`:`:BiVector`(pick`_`:`:BiVector`&`): [* BiVector](pick`_ BiVector`& "
"[*@3 src])&][s6; Pick constructor. Transfers source BiVec")
TOPIC_TEXT(
"tor in low constant time, but destroys it by picking.&][s1; [*C@3 src]-|Source BiVector.&][s0;3 "
"&][s4;:`:`:BiVector`:`:operator`=`(pick`_`:`:BiVector`&`): void [* operator]`=(pick`_ "
"BiVector`& [*@3 src])&][s6; Pick operator. Transfers source BiVector in low constant "
"time, but destroys it by picking.&][s1; [*C@3 v]-|Source BiVector.&][s0;3 &][s4;:`:`:BiVector`:`:IsPicked`(`): "
"bool [* IsPicked]()&][")
TOPIC_TEXT(
"s6; Returns [* true] if BiVector is in picked state.&][s1; [*/ Return value]-|[* "
"true] if BiVector is in picked state, [* false] otherwise.&][s0;3 &][s4;:`:`:BiVector`:`:BiVector`(const`:`:BiVector`&`,int`): "
"[* BiVector](const BiVector`& [*@3 src], int)&][s6; Optional deep copy constructor.&][s11; "
"Requires T to have deep copy constructor or optional deep copy constructor.&][s1; "
"[*C@3 v]-|Source Bi")
TOPIC_TEXT(
"Vector.&][s0;3 &][s4;:`:`:BiVector`:`:ValueType`:`:typedef:* [* typedef T ]ValueType&][s6; "
"Typedef of T for use in templated algorithms.&][s0;3 &][s4;:`:`:BiVector`:`:Iterator`:`:typedef: "
"typedef [/ type] [* Iterator];&][s6; Iterator type.&][s0;3 &][s4;:`:`:BiVector`:`:ConstIterator`:`:typedef: "
"typedef [/ type] [* ConstIterator];&][s6; Constant iterator type.&][s0;3 &][s4;:`:`:BiVector`:`:Begin`(`")
TOPIC_TEXT(
")const: ConstIterator [* Begin]() const&][s6; Returns a constant iterator to the "
"first element in the BiVector.&][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:BiVector`:`:End`(`)const: "
"ConstIterator [* End]() const&][s6; Returns a constant iterator to the position just "
"beyond the last element in the BiVector.&][s1; [*/ Return value]-|Iterator.&][s0;3 "
"&][s4;:`:`:BiVector`:`:GetIter`(int`)cons")
TOPIC_TEXT(
"t: ConstIterator [* GetIter](int [*@3 pos]) const&][s6; Returns a constant iterator "
"to the element at the specified position. Same as [* Begin() `+ i]. The benefit of "
"this method is that [* pos] is range checked in debug mode.&][s1; [*C@3 i]-|Required "
"position.&][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:BiVector`:`:Begin`(`): "
"Iterator [* Begin]()&][s6; Returns a non`-constant iterator to")
TOPIC_TEXT(
" the first element in the BiVector.&][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:BiVector`:`:End`(`): "
"Iterator [* End]()&][s6; Returns non`-constant iterator to the position just beyond "
"the last element in the BiVector.&][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:BiVector`:`:GetIter`(int`): "
"Iterator [* GetIter](int [*@3 pos])&][s6; Returns a non`-constant iterator to the "
"element a")
TOPIC_TEXT(
"t the specified position. Same as [* Begin() `+ pos]. The benefit of this method "
"is that [* pos] is range checked in debug mode.&][s1; [*C@3 i]-|Required position.&][s1; "
"[*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:BiVector`:`:Swap`(`:`:BiVector`&`,`:`:BiVector`&`): "
"friend void [* Swap](BiVector`& [*@3 a], BiVector`& [*@3 b])&][s6; Specialization "
"of the generic [* Swap] for BiVectors. Swaps BiVe")
TOPIC_TEXT(
"ctors in low constant time operation.&][s1; [*C@3 a]-|First BiVector to swap.&][s1; "
"[*C@3 b]-|Second BiVector to swap.&][s0;3 ]")
