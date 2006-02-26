TITLE("Segtor")
REF("::Segtor::class")
REF("::Segtor::Add()")
REF("::Segtor::Add(const T&)")
REF("::Segtor::AddPick(pick_ T&)")
REF("::Segtor::operator[](int)")
REF("::Segtor::operator[](int)const")
REF("::Segtor::GetCount()const")
REF("::Segtor::IsEmpty()const")
REF("::Segtor::SetCount(int)")
REF("::Segtor::SetCount(int,const T&)")
REF("::Segtor::Clear()")
REF("::Segtor::At(int)")
REF("::Segtor::At(int,const T&)")
REF("::Segtor::Shrink()")
REF("::Segtor::Reserve(int)")
REF("::Segtor::GetAlloc()const")
REF("::Segtor::Set(int,const T&,int)")
REF("::Segtor::Drop(int)")
REF("::Segtor::Top()")
REF("::Segtor::Top()const")
REF("::Segtor::Pop()")
REF("::Segtor::operator<<(const T&)")
REF("::Segtor::operator|(pick_ T&)")
REF("::Segtor::Serialize(::Stream&)")
REF("::Segtor::IsPicked()const")
REF("::Segtor::Segtor()")
REF("::Segtor::Segtor(pick_::Segtor&)")
REF("::Segtor::Segtor(const::Segtor&,int)")
REF("::Segtor::~Segtor()")
REF("::Segtor::ValueType::typedef")
REF("::Segtor::Iterator::typedef")
REF("::Segtor::ConstIterator::typedef")
REF("::Segtor::Begin()const")
REF("::Segtor::End()const")
REF("::Segtor::GetIter(int)const")
REF("::Segtor::Begin()")
REF("::Segtor::End()")
REF("::Segtor::GetIter(int)")
REF("::Segtor::Swap(::Segtor&,::Segtor&)")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2;~~~64; Segtor&][s10;~~~64; "
"template <class [*@4 T], int [*@4 NBLK] `= 16>&][s0;3 &][s10;:`:`:Segtor`:`:class:~~~64;* "
"[* class ]Segtor&][s12; [*C@4 T]-|Type of elements stored in Segtor. T  must have "
"either [*/ deep copy constructor], [*/ pick constr")
TOPIC_TEXT(
"uctor] or [*/ default constructor].&][s12; [*C@4 NBLK]-|Size of segments.&][s5;~~~.416; "
"Segtor is special flavor of random access container that can be sometimes used to "
"improve memory consumption. It never invalidates iterators nor references to elements, "
"but it also lacks operations to insert and remove elements at any position. Segtor "
"is designed to be used in situations where size of each elem")
TOPIC_TEXT(
"ent is small, but it is either non`-moveable or references to elements must be never "
"invalidated.&][s5;~~~.416; Typical implementation stores elements in segments of "
"fixed size. This fixed size is determined by template parameter NBLK.&][s5;~~~.416; "
"Iterators to Segtor satisfy all C`+`+ standard library requirements for random access "
"iterator plus they allow assignment (and copy constructor) and t")
TOPIC_TEXT(
"esting for 0 (that is NULL) pointer.&][s5; Like any other NTL container, Segtor "
"is [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ "
"pick and optional deep copy] transfer semantics. Calling methods of picked Segtor "
"is logic error with exception of&][s0;C+75 void [* operator`=](pick`_ Array`& [*@3 "
"v])&][s0;/+75 [/C void ][*/C operat")
TOPIC_TEXT(
"or<<`=][/C (const Array`& ][*/C@3 v][/C )][/  ](defined using DeepCopyOptionTemplate)&][s0;C+75 "
"void [* Clear]()&][s0;C+75 bool [* IsPicked]() const&][s0;b42;a42; Optional deep "
"copy is implemented through DeepCopyOptionTemplate macro.&][s3; Members&][s0;3 &][s4;:`:`:Segtor`:`:Add`(`): "
"T`& [* Add]()&][s6; Adds a new default constructed element to Segtor.&][s11; Requires "
"T to have default constructo")
TOPIC_TEXT(
"r.&][s1; [*/ Return value]-|Reference to the newly added default constructed element.&][s0;3 "
"&][s4;:`:`:Segtor`:`:Add`(const T`&`): void [* Add](const T`& [*@3 x])&][s6; Adds "
"a new element with the specified value to Segtor.&][s11; Requires T to have deep "
"copy constructor.&][s1; [*C@3 x]-|The value that is copied to the newly created element.&][s0;3 "
"&][s4;:`:`:Segtor`:`:AddPick`(pick`_ T`&`): void")
TOPIC_TEXT(
" [* AddPick](pick`_ T`& [*@3 x])&][s6; Adds a new element to Segtor and picks value "
"of parameter x. &][s11; Requires T to have pick constructor.&][s1; [*C@3 x]-|Source "
"instance of T that is to be picked.&][s0;3 &][s4;:`:`:Segtor`:`:operator`[`]`(int`): "
"T`& [* operator]`[`](int [*@3 i])&][s6; Returns a reference to the element at the "
"specified position.&][s1; [*C@3 i]-|Position of the element.&][s1")
TOPIC_TEXT(
"; [*/ Return value]-|Reference to the element.&][s0;3 &][s4;:`:`:Segtor`:`:operator`[`]`(int`)const: "
"const T`& [* operator]`[`](int [*@3 i]) const&][s6; Returns a reference to the element "
"at the specified position.&][s1; [*C@3 i]-|Position of the element.&][s1; [*/ Return "
"value]-|Constant reference to the element.&][s0;3 &][s4;:`:`:Segtor`:`:GetCount`(`)const: "
"int [* GetCount]() const&][s6; Return")
TOPIC_TEXT(
" the number of elements in Segtor.&][s1; [*/ Return value]-|Actual number of elements.&][s0;3 "
"&][s4;:`:`:Segtor`:`:IsEmpty`(`)const: bool [* IsEmpty]() const&][s6; Tests whether "
"Segtor is empty. Same as GetCount() `=`= 0.&][s1; [*/ Return value]-|true if Segtor "
"is empty, false otherwise.&][s0;3 &][s4;:`:`:Segtor`:`:SetCount`(int`): void [* SetCount](int "
"[*@3 n])&][s6; Changes count of elements in ")
TOPIC_TEXT(
"Segtor to specified value. If required number of elements is greater than actual "
"number, newly added elements are default constructed.&][s11; Requires T to have default "
"constructor.&][s1; [*C@3 n]-|Required number of elements.&][s0;3 &][s4;:`:`:Segtor`:`:SetCount`(int`,const "
"T`&`): void [* SetCount](int [*@3 n], const T`& [*@3 init])&][s6; Changes count of "
"elements in Segtor to specified value. If")
TOPIC_TEXT(
" required number of elements is greater than actual number, newly added elements "
"are initialized to the specified value using copy constructor.&][s11; Requires T "
"to have deep copy constructor.&][s1; [*C@3 n]-|Required number of elements.&][s1; "
"[*C@3 init]-|Initialization value of newly added elements.&][s0;3 &][s4;:`:`:Segtor`:`:Clear`(`): "
"void [* Clear]()&][s6; Removes all elements from Segtor. C")
TOPIC_TEXT(
"apacity is also cleared to zero.&][s0;3 &][s4;:`:`:Segtor`:`:At`(int`): T`& [* At](int "
"[*@3 i])&][s6; If specified position is lower than number of elements in Segtor ([* "
"i < GetCount()]), returns reference to element at specified position. Otherwise increases "
"number of elements in Segtor to [* i `+ 1]. Newly added elements are default constructed. "
"&][s11; Requires T to have default constructor.&]")
TOPIC_TEXT(
"[s1; [*C@3 i]-|Position of required element.&][s1; [*/ Return value]-|Reference "
"to required element.&][s0;3 &][s4;:`:`:Segtor`:`:At`(int`,const T`&`): T`& [* At](int "
"[*@3 i], const T`& [*@3 x])&][s6; If the specified position is lower than number "
"of elements in Segtor ([* i < GetCount()]), returns reference to element at specified "
"position. Otherwise increases number of elements in Segtor to [* i ")
TOPIC_TEXT(
"`+ 1]. Newly added elements are deep`-copy constructed from [* x].&][s11; Requires "
"T to have deep copy constructor.&][s1; [*C@3 i]-|Position of the required element.&][s1; "
"[*C@3 x]-|Initialization value of the newly added elements.&][s1; [*/ Return value]-|Reference "
"to the required element.&][s0;3 &][s4;:`:`:Segtor`:`:Shrink`(`): void [* Shrink]()&][s6; "
"Minimizes memory consumption of Segtor.&][s1")
TOPIC_TEXT(
"; [*/ Return value]-|&][s0;3 &][s4;:`:`:Segtor`:`:Reserve`(int`): void [* Reserve](int "
"[*@3 xtra])&][s6; Reserves capacity. If the required capacity is greater than the "
"current capacity, capacity is increased to the required value.&][s1; [*C@3 xtra]-|Required "
"capacity.&][s0;3 &][s4;:`:`:Segtor`:`:GetAlloc`(`)const: int [* GetAlloc]() const&][s6; "
"Returns current capacity of Segtor.&][s1; [*/ Return")
TOPIC_TEXT(
" value]-|Capacity of Segtor.&][s0;3 &][s4;:`:`:Segtor`:`:Set`(int`,const T`&`,int`): "
"void [* Set](int [*@3 i], const T`& [*@3 x], int [*@3 count] `= 1)&][s6; Sets requested "
"number of elements starting at the position [* i] to the specified value. If required "
"number of elements exceeds existing elements of Segtor then elements are added to "
"Segtor.&][s11; Requires T to have deep copy constructor.&][")
TOPIC_TEXT(
"s1; [*C@3 i]-|Starting position.&][s1; [*C@3 x]-|Value.&][s1; [*C@3 count]-|Number "
"of elements.&][s0;3 &][s4;:`:`:Segtor`:`:Drop`(int`): void [* Drop](int [*@3 n] `= "
"1)&][s6; Drops specified number of last elements in the Segtor.&][s1; [*C@3 n]-|Number "
"of elements.&][s0;3 &][s4;:`:`:Segtor`:`:Top`(`): [* T]`& [* Top]()&][s6; Returns "
"reference to the last element in the Segtor.&][s1; [*/ Return val")
TOPIC_TEXT(
"ue]-|Reference of last element in the Segtor.&][s0;3 &][s4;:`:`:Segtor`:`:Top`(`)const: "
"const [* T]`& [* Top]() const&][s6; Returns a constant reference to the last element "
"in the Segtor.&][s1; [*/ Return value]-|Reference of last element in the Segtor.&][s0;3 "
"&][s4;:`:`:Segtor`:`:Pop`(`): T [* Pop]()&][s6; Drops the last element of the Segtor "
"and returns its value.&][s11; Requires T to have deep ")
TOPIC_TEXT(
"copy constructor.&][s1; [*/ Return value]-|Value of the dropped element.&][s0;3 "
"&][s4;:`:`:Segtor`:`:operator`<`<`(const T`&`): Segtor`& [* operator]<<(const T`& "
"[*@3 x])&][s6; Operator variant of [* void Add(const T`&x)]. Returning a reference "
"to the Segtor enables adding more elements in a single expression, thus e.g. allowing "
"to construct a temporary Segtor as part of an expression like Foo((Se")
TOPIC_TEXT(
"gtor<int>() << 1 << 2 << 4)).&][s11; Requires T to have deep copy constructor.&][s1; "
"[*C@3 x]-|The value that is copied to the newly created element.&][s1; [*/ Return "
"value]-|Reference to Segtor (`*this).&][s0;3 &][s4;:`:`:Segtor`:`:operator`|`(pick`_ "
"T`&`): Segtor`& [* operator]`|(pick`_ T`& [*@3 x])&][s6; Operator replacement of "
"[* void AddPick(pick`_ T`&x)]. By returning reference to to Segtor ")
TOPIC_TEXT(
"allows adding more elements in single expression, thus e.g. allowing to construct "
"temporary Segtor as part of expression.&][s11; Requires T to have pick constructor.&][s1; "
"[*C@3 x]-|Source instance of T that is to be picked.&][s1; [*/ Return value]-|Reference "
"to Segtor (`*this).&][s0;3 &][s4;:`:`:Segtor`:`:Serialize`(`:`:Stream`&`): void [* "
"Serialize](Stream`& [*@3 s])&][s6; Serializes the content")
TOPIC_TEXT(
" of the Segtor to/from the Stream. Works only if NTL is used as part of UPP.&][s11; "
"Requires T to have serialization operator defined.&][s1; [*C@3 s]-|Target/source "
"stream.&][s0;3 &][s4;:`:`:Segtor`:`:IsPicked`(`)const:~~~.992; bool [* IsPicked]() "
"const&][s6; Returns [* true] if Segtor is in picked state.&][s1; [*/ Return value]-|[* "
"true] if Segtor is in picked state, [* false] otherwise.&][s0;3 &")
TOPIC_TEXT(
"][s4;:`:`:Segtor`:`:Segtor`(`): [* Segtor]()&][s6; Default constructor. Constructs "
"an empty Segtor.&][s0;3 &][s4;:`:`:Segtor`:`:Segtor`(pick`_`:`:Segtor`&`): [* Segtor](pick`_ "
"[* Segtor]`& [*@3 s])&][s6; Pick constructor. Transfers source Segtor in low constant "
"time, but destroys it by picking.&][s1; [*C@3 v]-|Source Segtor.&][s0;3 &][s4;:`:`:Segtor`:`:Segtor`(const`:`:Segtor`&`,int`): "
"[* Segtor](")
TOPIC_TEXT(
"const [* Segtor]`& [*@3 s], int)&][s6; Optional deep copy constructor.&][s11; Requires "
"T to have deep copy constructor or optional deep copy constructor.&][s1; [*C@3 v]-|Source "
"array.&][s0;3 &][s4;:`:`:Segtor`:`:`~Segtor`(`): `~[* Segtor]()&][s6; Destructor. "
"Invokes the destructor of all elements in the Segtor.&][s0;3 &][s4;:`:`:Segtor`:`:ValueType`:`:typedef:* "
"[* typedef T ]ValueType&][s6; Typede")
TOPIC_TEXT(
"f of T for use in templated algorithms.&][s0;3 &][s4;:`:`:Segtor`:`:Iterator`:`:typedef: "
"typedef [/ type] [* Iterator];&][s6; Iterator type.&][s0;3 &][s4;:`:`:Segtor`:`:ConstIterator`:`:typedef: "
"typedef [/ type] [* ConstIterator];&][s6; Constant iterator type.&][s0;3 &][s4;:`:`:Segtor`:`:Begin`(`)const: "
"ConstIterator [* Begin]() const&][s6; Returns a constant iterator to the first element "
"in Segto")
TOPIC_TEXT(
"r.&][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:Segtor`:`:End`(`)const: "
"ConstIterator [* End]() const&][s6; Returns a constant iterator to the position just "
"beyond the last element in Segtor.&][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:Segtor`:`:GetIter`(int`)const: "
"ConstIterator [* GetIter](int [*@3 i]) const&][s6; Returns a constant iterator to "
"the element at the specified posit")
TOPIC_TEXT(
"ion. Same as [* Begin() `+ pos]. The benefit of this method is that in debug mode "
"[* pos] is range checked.&][s1; [*C@3 i]-|Required position.&][s1; [*/ Return value]-|Iterator.&][s0;3 "
"&][s4;:`:`:Segtor`:`:Begin`(`): Iterator [* Begin]()&][s6; Returns a non`-constant "
"iterator to the first element in Segtor.&][s1; [*/ Return value]-|Iterator.&][s0;3 "
"&][s4;:`:`:Segtor`:`:End`(`): Iterator [* End]()&")
TOPIC_TEXT(
"][s6; Returns a non`-constant iterator to the position just beyond the last element "
"in Segtor.&][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:Segtor`:`:GetIter`(int`): "
"Iterator [* GetIter](int [*@3 i])&][s6; Returns a non`-constant iterator to the element "
"at specified position. Same as [* Begin() `+ pos]. The benefit of this methods is "
"that in debug mode [* pos] is range checked.&][s1; [*C@3")
TOPIC_TEXT(
" i]-|Required position.&][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:Segtor`:`:Swap`(`:`:Segtor`&`,`:`:Segtor`&`): "
"friend void [* Swap](Segtor`& [*@3 a], Segtor`& [*@3 b])&][s6; Specialization of "
"generic [* Swap] for Segtor. Swaps array in simple constant time operation.&][s1; "
"[*C@3 a]-|First Segtor to swap.&][s1; [*C@3 b]-|Second Segtor to swap.]")
