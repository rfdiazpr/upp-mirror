TITLE("Array")
REF("::Array::class")
REF("::Array::Add()")
REF("::Array::Add(const T&)")
REF("::Array::AddPick(pick_ T&)")
REF("::Array::Add(T*)")
REF("::Array::operator[](int)const")
REF("::Array::operator[](int)")
REF("::Array::GetCount()const")
REF("::Array::IsEmpty()const")
REF("::Array::Trim(int)")
REF("::Array::SetCount(int)")
REF("::Array::SetCount(int,const T&)")
REF("::Array::SetCountR(int)")
REF("::Array::SetCountR(int,const T&)")
REF("::Array::Clear()")
REF("::Array::At(int)")
REF("::Array::At(int,const T&)")
REF("::Array::Shrink()")
REF("::Array::Reserve(int)")
REF("::Array::GetAlloc()const")
REF("::Array::Set(int,const T&,int)")
REF("::Array::Remove(int,int)")
REF("::Array::Remove(const int*,int)")
REF("::Array::Remove(const::Vector<int>&)")
REF("::Array::InsertN(int,int)")
REF("::Array::Insert(int)")
REF("::Array::Insert(int,const T&,int)")
REF("::Array::Insert(int,const::Array&)")
REF("::Array::Insert(int,const::Array&,int,int)")
REF("::Array::Append(const::Array&)")
REF("::Array::Append(const::Array&,int,int)")
REF("::Array::InsertPick(int,pick_::Array&)")
REF("::Array::AppendPick(pick_::Array&)")
REF("::Array::Swap(int,int)")
REF("::Array::Move(int,int)")
REF("::Array::Detach(int)")
REF("::Array::Set(int,T*)")
REF("::Array::Insert(int,T*)")
REF("::Array::Drop(int)")
REF("::Array::Top()")
REF("::Array::Top()const")
REF("::Array::PopDetach()")
REF("::Array::Swap(::Array&)")
REF("::Array::operator<<(const T&)")
REF("::Array::operator<<(T*)")
REF("Array& operator|(pick_ T& x)")
REF("::Array::IsPicked()const")
REF("::Array::Serialize(::Stream&)")
REF("::Array::Array()")
REF("::Array::~Array()")
REF("::Array::Array(pick_::Array&)")
REF("::Array::operator=(pick_::Array&)")
REF("::Array::Array(const::Array&,int)")
REF("::DeepCopyOption::operator<<=(T&,const T&)")
REF("::Array::ValueType::typedef")
REF("::Array::Iterator::class")
REF("::Array::ConstIterator::class")
REF("::Array::Begin()")
REF("::Array::End()")
REF("::Array::GetIter(int)")
REF("::Array::Begin()const")
REF("::Array::End()const")
REF("::Array::GetIter(int)const")
REF("::Array::Swap(::Array&,::Array&)")
REF("::Array::IterSwap(::Array::Iterator,::Array::Iterator)")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2;~~~64; Array&][s10;~~~64; "
"template <class [*@4 T]>&][s0;3 &][s10;:`:`:Array`:`:class:~~~64;* [* class ]Array&][s0; "
"&][s12; [*C@4 T]-|Type or base class of elements stored in the Array. There is no "
"common requirement for T.&][s5; The most universal f")
TOPIC_TEXT(
"orm of basic random access container. Its features are derived from fact that typically "
"it is  implemented using indirect container of pointers to T like Vector<T`*>. Nevertheless "
"it supports common concepts as ownership of contained elements, reference (not pointer) "
"access to elements and so on.&][s5; It provides almost all operations of Vector with "
"the same semantics and almost any Vector can be")
TOPIC_TEXT(
" directly replaced by Array. On the other hand, it provides some special operations "
"impossible for Vector and most important, it never invalidates references (that means "
"C`+`+ references and pointers) to elements (it often invalidates iterators, though).&][s5; "
"Array can be also used to store polymorphic elements `- stored elements could be "
"derived from T. To store such elements, you pass a pointer")
TOPIC_TEXT(
" to an element previously created on the heap. Still, Array takes over ownership "
"of such element (it e.g. deletes it when appropriate). You can also use this method "
"to create an Array of elements that do not have either pick, deep copy constructor "
"nor default constructor.&][s5; There are also operations that allow detaching an "
"element from an Array, removing it but not destroying. Pointer to such ")
TOPIC_TEXT(
"element is returned from this operation and the Array gives up ownership.&][s5; "
"Disadvantage of Array over Vector is performance `- most operations are significantly "
"slower than with Vectors (by factor up to 8, it depends on the speed of malloc/free). "
"Still, some operations might be even faster, e.g. Insert and Remove operations usually "
"move much less memory.&][s5; As for memory, for small size of")
TOPIC_TEXT(
" elements, Array memory consumption is significantly higher than Vector consumption. "
"As the size of the elements grow, Array starts to be better than Vector.&][s5;~~~.704; "
"Iterators to Array satisfy all C`+`+ standard library requirements for random access "
"iterator plus they allow assignment (and copy constructor) and testing for 0 (that "
"is NULL) pointer.&][s5; Like any other NTL container, Array ")
TOPIC_TEXT(
"is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ "
"pick and optional deep copy] transfer semantics. Calling methods of a picked Array "
"is logic error with the exceptions of&][s0;C+75 void [* operator`=](pick`_ Array`& "
"[*@3 v])&][s0;/+75 [/C void ][*/C operator<<`=][/C (const Array`& ][*/C@3 v][/C )][/ "
" ](defined using DeepCopyOp")
TOPIC_TEXT(
"tionTemplate)&][s0;C+75 void [* Clear]()&][s0;C+75 bool [* IsPicked]() const&][s5; "
"Optional deep copy is implemented through DeepCopyOptionTemplate macro.&][s3; Members&][s0;3 "
"&][s4;:`:`:Array`:`:Add`(`): T`& [* Add]()&][s6; Adds a new default constructed element "
"to the Array.&][s11; Requires T to have default constructor.&][s11; Invalidates iterators "
"to the Array.&][s1; [*/ Return value]-|Referen")
TOPIC_TEXT(
"ce to newly added default constructed element.&][s0;3 &][s4;:`:`:Array`:`:Add`(const "
"T`&`): void [* Add](const T`& [*@3 x])&][s6; Adds a new element with specified value "
"to the Array.&][s11; Requires T to have deep copy constructor.&][s11; Invalidates "
"iterators to the Array.&][s1; [*C@3 x]-|The value that is copied to newly created "
"element.&][s0;3 &][s4;:`:`:Array`:`:AddPick`(pick`_ T`&`): void [*")
TOPIC_TEXT(
" AddPick](pick`_ T`& [*@3 x])&][s6; Adds new element to the Array and picks value "
"of parameter to it. &][s11; Requires T to have pick constructor.&][s11; Invalidates "
"iterators to the Array.&][s1; [*C@3 x]-|Source instance of T that is to be picked.&][s0;3 "
"&][s4;:`:`:Array`:`:Add`(T`*`): T`& [* Add](T `*[*@3 newt])&][s6; Adds a new element "
"to the Array. Element is specified by pointer to an object ")
TOPIC_TEXT(
"created using operator new. Array takes over ownership of this object. This variant "
"allows the use of an Array as a polymorphic container, because the type of the added "
"element can be either T or a type derived from T. No constructor is applied. &][s1; "
"[*C@3 newt]-|Object to be added.&][s1; [*/ Return value]-|Reference to the new element "
" (that is [* `*newt]).&][s0; &][s4; [%00-00* template<class ")
TOPIC_TEXT(
"TT>][%00-00*@4  TT][%00-00 `&_][%00-00* Create][%00-00 ()]&][s6; Creates and adds "
"a new element to the Array.&][s11; Requires T to have default constructor.&][s1; "
"[*C@4 TT]-|Type of the new element.&][s1; [*/ Return value]-|Reference to the newly "
"created element.&][s0;3 &][s4;:`:`:Array`:`:operator`[`]`(int`)const:~~~.992; const "
"T`& [* operator`[`]](int [*@3 i]) const&][s6; Returns a reference to ")
TOPIC_TEXT(
"the element at the specified position.&][s1; [*C@3 i]-|Position of the element.&][s1; "
"[*/ Return value]-|Constant reference to the element.&][s0;3 &][s4;:`:`:Array`:`:operator`[`]`(int`): "
"T`& [* operator`[`]](int [*@3 i])&][s6; Returns a reference to the element at the "
"specified position.&][s1; [*C@3 i]-|Position of element.&][s1; [*/ Return value]-|Reference "
"to the element.&][s0;3 &][s4;:`:`:Arra")
TOPIC_TEXT(
"y`:`:GetCount`(`)const: int [* GetCount]() const&][s6; Return the number of elements "
"in the Array.&][s1; [*/ Return value]-|Actual number of elements.&][s0;3 &][s4;:`:`:Array`:`:IsEmpty`(`)const: "
"bool [* IsEmpty]() const&][s6; Tests whether Array is empty. Same as GetCount() `=`= "
"0.&][s1; [*/ Return value]-|true if Array is empty, false otherwise.&][s0;3 &][s4;:`:`:Array`:`:Trim`(int`): "
"void [* Tr")
TOPIC_TEXT(
"im](int [*@3 n])&][s6; Reduces the number of elements in the Array to the specified "
"number. Required number must be less than or equal to the actual number of elements "
"in the Array.&][s1; [*C@3 n]-|Required number of elements.&][s0;3 &][s4;:`:`:Array`:`:SetCount`(int`): "
"void [* SetCount](int [*@3 n])&][s6; Changes the count of elements in the Array to "
"the specified value. If the required number of")
TOPIC_TEXT(
" elements is greater than actual number, the newly added elements are default constructed. "
"If the Array`'s capacity has to be increased, the new capacity will exactly match "
"the required number of elements (unlike [* SetCountR]).&][s11; Requires T to have "
"default constructor.&][s11; Invalidates iterators to the Array.&][s1; [*C@3 n]-|Required "
"number of elements.&][s0;3 &][s4;:`:`:Array`:`:SetCount`")
TOPIC_TEXT(
"(int`,const T`&`): void [* SetCount](int [*@3 n], const T`& [*@3 init])&][s6; Changes "
"the count of elements in the Array to the specified value. If the required number "
"of elements is greater than the actual number, the newly added elements are initialized "
"to the specified value using copy constructor. If the Array`'s capacity has to be "
"increased, the new capacity will exactly match the required nu")
TOPIC_TEXT(
"mber of elements (unlike [* SetCountR]).&][s11; Requires T to have deep copy constructor.&][s11; "
"Invalidates iterators to the Array.&][s1; [*C@3 n]-|Required number of elements.&][s1; "
"[*C@3 init]-|Initialization value of the newly added elements.&][s0;3 &][s4;:`:`:Array`:`:SetCountR`(int`): "
"void [* SetCountR](int [*@3 n])&][s6; Changes the count of elements in the Array "
"to the specified value. If ")
TOPIC_TEXT(
"the required number of elements is greater than the actual number, the newly added "
"elements are default constructed. If the Array`'s capacity has to be increased, the "
"new capacity will be greater than the required number of elements (unlike [* SetCount]) "
"to allow adding other elements without further increasing capacity.&][s11; Requires "
"T to have default constructor.&][s11; Invalidates iterators t")
TOPIC_TEXT(
"o the Array.&][s1; [*C@3 n]-|Required number of elements.&][s0;3 &][s4;:`:`:Array`:`:SetCountR`(int`,const "
"T`&`):~~~.992; void [* SetCountR](int [*@3 n], const T`& [*@3 init])&][s6; Changes "
"the count of elements in the Array to the specified value. If the required number "
"of elements is greater than the actual number, the newly added elements are initialized "
"to the specified value using copy constr")
TOPIC_TEXT(
"uctor. If the Array`'s capacity has to be increased, he new capacity will be greater "
"than the required number of elements (unlike [* SetCount]) to allow adding other "
"elements without further increasing capacity.&][s11; Requires T to have deep copy "
"constructor.&][s11; Invalidates iterators to the Array.&][s1; [*C@3 n]-|Required "
"number of elements.&][s1; [*C@3 init]-|Initialization value of the newl")
TOPIC_TEXT(
"y added elements.&][s0;3 &][s4;:`:`:Array`:`:Clear`(`):~~~.992; void [* Clear]()&][s6; "
"Removes all elements from the Array. Capacity is also cleared to zero.&][s11; Invalidates "
"iterators to the Array.&][s0;3 &][s4;:`:`:Array`:`:At`(int`):~~~.992; T`& [* At](int "
"[*@3 i])&][s6; If the specified position is lower than the number of elements in "
"the Array ([* i < GetCount()]), returns a reference to th")
TOPIC_TEXT(
"e element at the specified position. Otherwise increases the number of elements "
"in the Array to [* i `+ 1]. Newly added elements are default constructed. If the "
"Array`'s capacity has to be increased, the new capacity will be greater than the "
"required number of elements to allow adding other elements without further increasing "
"capacity.&][s11; Requires T to have default constructor.&][s11; Invalida")
TOPIC_TEXT(
"tes iterators to the Array.&][s1; [*C@3 i]-|Position of the required element.&][s1; "
"[*/ Return value]-|Reference to the required element.&][s0;3 &][s4;:`:`:Array`:`:At`(int`,const "
"T`&`): T`& [* At](int [*@3 i], const T`& [*@3 x])&][s6; If specified position is "
"lower than number of elements in the Array ([* i < GetCount()]), returns a reference "
"to the element at the specified position. Otherwise in")
TOPIC_TEXT(
"creases the number of elements in the Array to [* i `+ 1]. Newly added elements "
"are copy constructed from [* x]. If the Array`'s capacity has to be increased, the "
"new capacity will be greater than the required number of elements to allow adding "
"other elements without further increasing capacity.&][s11; Requires T to have deep "
"copy constructor.&][s11; Invalidates iterators to the Array.&][s1; [*C@3")
TOPIC_TEXT(
" i]-|Position of the required element.&][s1; [*C@3 x]-|Initialization value of newly "
"added elements.&][s1; [*/ Return value]-|Reference to the required element.&][s0;3 "
"&][s4;:`:`:Array`:`:Shrink`(`): void [* Shrink]()&][s6; Minimizes memory consumption "
"of the Array by decreasing the capacity to the number of elements.&][s0;3 &][s4;:`:`:Array`:`:Reserve`(int`):~~~.992; "
"void [* Reserve](int [*@3 xtr")
TOPIC_TEXT(
"a])&][s6; Reserves capacity. If required capacity is greater than the current capacity, "
"capacity is increased to the required value.&][s1; [*C@3 xtra]-|Required capacity.&][s0;3 "
"&][s4;:`:`:Array`:`:GetAlloc`(`)const:~~~.992; int [* GetAlloc]() const&][s6; Returns "
"current capacity of the Array.&][s1; [*/ Return value]-|Capacity of the Array.&][s0;3 "
"&][s4;:`:`:Array`:`:Set`(int`,const T`&`,int`): vo")
TOPIC_TEXT(
"id [* Set](int [*@3 i], const T`& [*@3 x], int [*@3 count] `= 1)&][s6; Sets the "
"requested number of elements starting at the position [* i] to the specified value. "
"If the required number of elements exceeds existing elements of the Array then elements "
"are added to the Array.&][s11; Requires T to have deep copy constructor.&][s11; Invalidates "
"iterators to the Array.&][s1; [*C@3 i]-|Starting positio")
TOPIC_TEXT(
"n.&][s1; [*C@3 x]-|Value.&][s1; [*C@3 count]-|Number of elements.&][s0;3 &][s4;:`:`:Array`:`:Remove`(int`,int`): "
"void [* Remove](int [*@3 i], int [*@3 count] `= 1)&][s6; Removes requested number "
"of elements.&][s11; Invalidates iterators to the Array.&][s1; [*C@3 i]-|Position.&][s1; "
"[*C@3 count]-|Number of elements to remove.&][s0;3 &][s4;:`:`:Array`:`:Remove`(const "
"int`*`,int`): void [* Remove](co")
TOPIC_TEXT(
"nst int `*[*@3 sorted`_list], int [*@3 n])&][s6; Removes number of elements from "
"the Array. Time of operation almost does not depend on number of elements.&][s11; "
"Invalidates iterators to the Array.&][s1; [*C@3 sorted`_list]-|Pointer to the Array "
"of positions to remove. It must be sorted from lowest to greatest value.&][s1; [*C@3 "
"n]-|Number of elements to remove.&][s0;3 &][s4;:`:`:Array`:`:Remove`")
TOPIC_TEXT(
"(const`:`:Vector`<int`>`&`): void [* Remove](const Vector<int>`& [*@3 sorted`_list])&][s6; "
"Removes number of elements from the Array. Same as [* Remove(sorted`_list, sorted`_list.GetCount())].&][s11; "
"Invalidates iterators to the Array.&][s1; [*C@3 sorted`_list]-|Sorted Vector of positions "
"to remove.&][s0;3 &][s4;:`:`:Array`:`:InsertN`(int`,int`): void [* InsertN](int [*@3 "
"i], int [*@3 count] `= 1)")
TOPIC_TEXT(
"&][s6; Inserts a specified number of default constructed elements at the specified "
"position.&][s11; Requires T to have default constructor.&][s11; Invalidates iterators "
"to the Array.&][s1; [*C@3 i]-|Position.&][s1; [*C@3 count]-|Number of elements to "
"insert.&][s0;3 &][s4;:`:`:Array`:`:Insert`(int`):~~~.992; T`& [* Insert](int [*@3 "
"i])&][s6; Inserts one default constructed element at the specified ")
TOPIC_TEXT(
"position.&][s1;*1 Requires T to have default constructor.&][s11; Invalidates iterators "
"to the Array.&][s1; [*C@3 i]-|Position.&][s0;3 &][s4;:`:`:Array`:`:Insert`(int`,const "
"T`&`,int`): void [* Insert](int [*@3 i], const T`& [*@3 x], int [*@3 count] `= 1)&][s6; "
"Inserts a specified number of elements, setting them to a specified value.&][s11;~~~.992; "
"Requires T to have deep copy constructor.&][s11; ")
TOPIC_TEXT(
"Invalidates iterators to the Array.&][s1; [*C@3 i]-|Position.&][s1; [*C@3 x]-|Value "
"of the inserted elements.&][s1; [*C@3 count]-|Number of elements to insert.&][s0;3 "
"&][s4;:`:`:Array`:`:Insert`(int`,const`:`:Array`&`):~~~.992; void [* Insert](int "
"[*@3 i], const Array`& [*@3 x])&][s6; Inserts all elements from another Array.&][s11;~~~.992; "
"Requires T to have deep copy constructor.&][s11; Invalidat")
TOPIC_TEXT(
"es iterators to the Array.&][s1; [*C@3 i]-|Position.&][s1; [*C@3 x]-|Source Array.&][s0;3 "
"&][s4;:`:`:Array`:`:Insert`(int`,const`:`:Array`&`,int`,int`):~~~.992; void [* Insert](int "
"[*@3 i], const Array`& [*@3 x], int [*@3 offset], int [*@3 count])&][s6; Inserts "
"a range of elements from another Array.&][s11;~~~.992; Requires T to have deep copy "
"constructor.&][s11; Invalidates iterators to the Array")
TOPIC_TEXT(
".&][s1; [*C@3 i]-|Insertion position.&][s1; [*C@3 x]-|Source Array.&][s1; [*C@3 "
"offset]-|Position of first element in source Array to be inserted.&][s1; [*C@3 count]-|Number "
"of elements to insert.&][s0;3 &][s4;:`:`:Array`:`:Append`(const`:`:Array`&`): void "
"[* Append](const Array`& [*@3 x])&][s6; Appends all elements of source Array.&][s11;~~~.992; "
"Requires T to have deep copy constructor.&][s11; I")
TOPIC_TEXT(
"nvalidates iterators to the Array.&][s1; [*C@3 x]-|Source Array.&][s0;3 &][s4;:`:`:Array`:`:Append`(const`:`:Array`&`,int`,int`): "
"void [* Append](const Array`& [*@3 x], int [*@3 o], int [*@3 c])&][s6; Appends a "
"range of elements from source Array.&][s11;~~~.992; Requires T to have deep copy "
"constructor.&][s11; Invalidates iterators to the Array.&][s1; [*C@3 x]-|Source Array.&][s1; "
"[*C@3 o]-|Positi")
TOPIC_TEXT(
"on of the first element in source Array to be inserted.&][s1; [*C@3 c]-|Number of "
"elements to insert.&][s0;3 &][s4;:`:`:Array`:`:InsertPick`(int`,pick`_`:`:Array`&`): "
"void [* InsertPick](int [*@3 i], pick`_ Array`& [*@3 x])&][s6; Inserts the source "
"Array at the specified position using pick transfer semantics. It is faster than "
"deep copy insert, does not use deep copy constructor for T, but destro")
TOPIC_TEXT(
"ys the source Array by picking.&][s11; Invalidates iterators to the Array.&][s1; "
"[*C@3 i]-|Insertion position.&][s1; [*C@3 x]-|Source Array.&][s0;3 &][s4;:`:`:Array`:`:AppendPick`(pick`_`:`:Array`&`): "
"void [* AppendPick](pick`_ Array`& [*@3 x])&][s6; Appends source Array using pick "
"transfer semantics. It is faster than deep copy insert, does not use deep copy constructor "
"for T, but destroys the so")
TOPIC_TEXT(
"urce Array by picking.&][s11; Invalidates iterators to the Array.&][s1; [*C@3 x]-|Source "
"Array.&][s0;3 &][s4;:`:`:Array`:`:Swap`(int`,int`): void [* Swap](int [*@3 i1], int "
"[*@3 i2])&][s6; Swaps elements without using copy constructor.&][s11; Invalidates "
"iterators to the Array.&][s1; [*C@3 i1]-|Position of the first element.&][s1; [*C@3 "
"i2]-|Position of the second element.&][s0;3 &][s4;:`:`:Array`")
TOPIC_TEXT(
":`:Move`(int`,int`): void [* Move](int [*@3 i1], int [*@3 i2])&][s6; Removes element "
"at position [* i1] and inserts it at [* i2], without using copy constructor of T.&][s11; "
"Invalidates iterators to the Array.&][s1; [*C@3 i1]-|Position of the element to move.&][s1; "
"[*C@3 i2]-|Target position.&][s0;3 &][s4;:`:`:Array`:`:Detach`(int`): T `*[* Detach](int "
"[*@3 i])&][s6; Removes the element at positio")
TOPIC_TEXT(
"n [* i], giving up ownership. Client is responsible for deletion of the element.&][s11; "
"Invalidates iterators to the Array.&][s1; [*C@3 i]-|Position of the element to remove.&][s1; "
"[*/ Return value]-|Pointer to the element allocated on the heap.&][s0;3 &][s4;:`:`:Array`:`:Set`(int`,T`*`): "
"T`& [* Set](int [*@3 i], T `*[*@3 newt])&][s6; Replaces element at the specified "
"position by an element previo")
TOPIC_TEXT(
"usly created on the heap. Array takes over ownership of the element.&][s1; [*C@3 "
"i]-|Position.&][s1; [*C@3 newt]-|New element allocated on the heap.&][s1; [*/ Return "
"value]-|Reference to new element.&][s0;3 &][s4;:`:`:Array`:`:Insert`(int`,T`*`): "
"void [* Insert](int [*@3 i], T `*[*@3 newt])&][s6; Inserts an element previously "
"created on the heap at the specified position. Array takes over ownershi")
TOPIC_TEXT(
"p of element.&][s11; Invalidates iterators to the Array.&][s1; [*C@3 i]-|Insertion "
"position.&][s1; [*C@3 newt]-|New element allocated on the heap.&][s0;3 &][s4;:`:`:Array`:`:Drop`(int`): "
"void [* Drop](int [*@3 n] `= 1)&][s6; Drops specified number of last elements in "
"the Array (same as Trim(GetCount() `- n)).&][s1;* [*C@3 n]-|Number of elements.&][s0;3 "
"&][s4;:`:`:Array`:`:Top`(`): T`& [* Top]()&][")
TOPIC_TEXT(
"s6; Returns reference to the last element in the Array.&][s1; [*/ Return value]-|Reference "
"of last element in the Array.&][s0;3 &][s4;:`:`:Array`:`:Top`(`)const: const T`& "
"[* Top]() const&][s6; Returns constant reference to the last element in the Array.&][s1; "
"[*/ Return value]-|Reference of last element in the Array.&][s0;3 &][s4;:`:`:Array`:`:PopDetach`(`): "
"T `*[* PopDetach]()&][s6; Drops the la")
TOPIC_TEXT(
"st element in the Array, giving up ownership (same as Detach(GetCount() `- 1)). "
"Client is responsible for deletion of element.&][s11; Invalidates iterators to the "
"Array.&][s1; [*/ Return value]-|Pointer to element allocated on the heap.&][s0;3 "
"&][s4;:`:`:Array`:`:Swap`(`:`:Array`&`): void [* Swap](Array`& [*@3 b])&][s6; Swaps "
"content of Array with another array in constant time operation.&][s1; [*")
TOPIC_TEXT(
"C@3 b]-|Target array.&][s0;3 &][s4;:`:`:Array`:`:operator`<`<`(const T`&`): Array`& "
"[* operator<<](const T`& [*@3 x])&][s6; Operator replacement of [* void Add(const "
"T`&x)]. By returning a reference to the Array it allows adding more elements in a "
"single expression, thus e.g. allowing to construct a temporary Array as part of an "
"expression like Foo((Array<int>() << 1 << 2 << 4)).&][s11; Requires T")
TOPIC_TEXT(
" to have deep copy constructor.&][s11; Invalidates iterators to the Array.&][s1; "
"[*C@3 x]-|The value that is copied to newly created element.&][s1; [*/ Return value]-|Reference "
"to the Array (`*this).&][s0;3 &][s4;:`:`:Array`:`:operator`<`<`(T`*`): Array`& [* "
"operator<<](T `*[*@3 newt])&][s6; Operator replacement of [* void Add(T `*x)]. By "
"returning a reference to the Array it allows adding more el")
TOPIC_TEXT(
"ements in a single expression, thus e.g. allowing to construct a temporary Array "
"as part of an expression like Foo((Array<Bar>() << new Bar << new DerivedFromBar)).&][s11; "
"Invalidates iterators to the Array.&][s1; [*C@3 newt]-|Object to be added.&][s1; "
"[*/ Return value]-|Reference to the Array (`*this).&][s0; &][s4;:Array`& operator`|`(pick`_ "
"T`& x`): Array`& [* operator`|](pick`_ T`& [*@3 x])&][s")
TOPIC_TEXT(
"6; Operator replacement of [* void AddPick(pick`_ T`&x)]. By returning a reference "
"to the Array it allows adding more elements in a single expression, thus e.g. allowing "
"to construct a temporary Array as part of an expression.&][s11; Requires T to have "
"pick constructor.&][s11; Invalidates iterators to the Array.&][s1; [*C@3 x]-|Source "
"instance of T that is to be picked.&][s1; [*/ Return value]-|Re")
TOPIC_TEXT(
"ference to Array (`*this).&][s0;3 &][s4;:`:`:Array`:`:IsPicked`(`)const: bool [* "
"IsPicked]() const&][s6; Returns [* true] if the Array is in picked state.&][s1; [*/ "
"Return value]-|[* true] if the Array is in picked state, [* false] otherwise.&][s0;3 "
"&][s4;:`:`:Array`:`:Serialize`(`:`:Stream`&`): void [* Serialize](Stream`& [*@3 s])&][s6; "
"Serializes the content of the Array to/from the Stream. Work")
TOPIC_TEXT(
"s only if NTL is used as part of UPP. Does not work with polymorphic Arrays (those "
"storing objects derived from T).&][s11; Requires T to have serialization operator "
"defined.&][s1; [*C@3 s]-|Target/source stream.&][s0;3 &][s4;:`:`:Array`:`:Array`(`): "
"[* Array]()&][s6; Default constructor. Constructs an empty Array.&][s0;3 &][s4;:`:`:Array`:`:`~Array`(`): "
"`~[* Array]()&][s6; Destructor. Invokes the ")
TOPIC_TEXT(
"destructor of every element in the Array.&][s0;3 &][s4;:`:`:Array`:`:Array`(pick`_`:`:Array`&`): "
"[* Array](pick`_ Array`& [*@3 v])&][s6; Pick constructor. Transfers the source Array "
"in low constant time, but destroys it by picking. &][s1; [*C@3 v]-|Source Array.&][s0;3 "
"&][s4;:`:`:Array`:`:operator`=`(pick`_`:`:Array`&`):~~~.992; void [* operator`=](pick`_ "
"Array`& [*@3 v])&][s6; Pick operator. Tran")
TOPIC_TEXT(
"sfers the source Array in low constant time, but destroys it by picking.&][s1; [*C@3 "
"v]-|Source Array.&][s0;3 &][s4;:`:`:Array`:`:Array`(const`:`:Array`&`,int`): [* Array](const "
"Array`& [*@3 v], int)&][s6; Optional deep copy constructor.&][s11; Requires T to "
"have deep copy constructor or optional deep copy constructor if the Array stores "
"only objects of type T.&][s11; Requires polymorphic deep cop")
TOPIC_TEXT(
"y if the Array stores also objects of type derived from T.&][s1; [*C@3 v]-|Source "
"Array.&][s0;3 &][s4;:`:`:DeepCopyOption`:`:operator`<`<`=`(T`&`,const T`&`): void "
"[* operator<<`=](const Array`& [*@3 v])&][s6; Optional deep copy operator. Defined "
"using DeepCopyOption base class.&][s11; Requires T to have deep copy constructor "
"or optional deep copy if the Array stores only objects of type T.&][s11;")
TOPIC_TEXT(
" Requires polymorphic deep copy if the Array stores also objects of type derived "
"from T.&][s1; [*C@3 v]-|Source Array.&][s0;3 &][s4;:`:`:Array`:`:ValueType`:`:typedef:* "
"[* typedef T ]ValueType&][s6; Typedef of T for use in templated algorithms.&][s0;3 "
"&][s4;:`:`:Array`:`:Iterator`:`:class: typedef [/ type] [* Iterator];&][s6; Iterator "
"type.&][s0;3 &][s4;:`:`:Array`:`:ConstIterator`:`:class: typede")
TOPIC_TEXT(
"f [/ type] [* ConstIterator];&][s6; Constant iterator type.&][s0;3 &][s4;:`:`:Array`:`:Begin`(`): "
"Iterator [* Begin]()&][s6; Returns a non`-constant iterator to the first element "
"in the Array. &][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:Array`:`:End`(`): "
"Iterator [* End]()&][s6; Returns a non`-constant iterator to the position just beyond "
"the last element in Array.&][s1; [*/ Return value")
TOPIC_TEXT(
"]-|Iterator.&][s0;3 &][s4;:`:`:Array`:`:GetIter`(int`): Iterator [* GetIter](int "
"[*@3 pos])&][s6; Returns a non`-constant iterator to the element at the specified "
"position. Same as [* Begin() `+ pos]. The benefit of this method is that [* pos] "
"is range checked in debug mode.&][s1; [*C@3 pos]-|Required position.&][s1; [*/ Return "
"value]-|Iterator.&][s0;3 &][s4;:`:`:Array`:`:Begin`(`)const: ConstIter")
TOPIC_TEXT(
"ator [* Begin]() const&][s6; Returns a constant iterator to the first element in "
"Array. &][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:Array`:`:End`(`)const: "
"ConstIterator [* End]() const&][s6; Returns a constant iterator to the position just "
"beyond the last element in Array.&][s1; [*/ Return value]-|Iterator.-|&][s0;3 &][s4;:`:`:Array`:`:GetIter`(int`)const: "
"ConstIterator [* GetIter](int [")
TOPIC_TEXT(
"*@3 pos]) const&][s6; Returns a constant iterator to the element at the specified "
"position. Same as [* Begin() `+ pos]. The benefit of this method is that [* pos] "
"is range checked in debug mode..&][s1; [*C@3 pos]-|Required position.&][s1; [*/ Return "
"value]-|Iterator.&][s0;3 &][s4;:`:`:Array`:`:Swap`(`:`:Array`&`,`:`:Array`&`): friend "
"void [* Swap](Array`& [*@3 a], Array`& [*@3 b])&][s6; Specializa")
TOPIC_TEXT(
"tion of the generic [* Swap] for Array. Swaps the arrays in simple constant time "
"operation.&][s1; [*C@3 a]-|First Array to swap.&][s1; [*C@3 b]-|Second Array to swap.&][s0;3 "
"&][s4;:`:`:Array`:`:IterSwap`(`:`:Array`:`:Iterator`,`:`:Array`:`:Iterator`): friend "
"void [* IterSwap](Iterator [*@3 a], Iterator [*@3 b])&][s6; Specialization of the "
"generic [* IterSwap] for Array. Swaps the elements in an Ar")
TOPIC_TEXT(
"ray without any requirements for T.&][s1; [*C@3 a]-|Iterator to first element.&][s1; "
"[*C@3 b]-|Iterator to second element.&][s0;3 ]")
