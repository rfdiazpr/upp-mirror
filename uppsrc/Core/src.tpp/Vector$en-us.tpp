TITLE("Vector")
REF("::Vector::class")
REF("::Vector::Add()")
REF("::Vector::Add(const T&)")
REF("::Vector::AddPick(pick_ T&)")
REF("::Vector::AddN(int)")
REF("::Vector::operator[](int)const")
REF("::Vector::operator[](int)")
REF("::Vector::GetCount()const")
REF("::Vector::IsEmpty()const")
REF("::Vector::Trim(int)")
REF("::Vector::SetCount(int)")
REF("::Vector::SetCount(int,const T&)")
REF("::Vector::SetCountR(int)")
REF("::Vector::SetCountR(int,const T&)")
REF("::Vector::Clear()")
REF("::Vector::At(int)")
REF("::Vector::At(int,const T&)")
REF("::Vector::Shrink()")
REF("::Vector::Reserve(int)")
REF("::Vector::GetAlloc()const")
REF("::Vector::Set(int,const T&,int)")
REF("::Vector::Remove(int,int)")
REF("::Vector::Remove(const int*,int)")
REF("::Vector::Remove(const::Vector<int>&)")
REF("::Vector::InsertN(int,int)")
REF("::Vector::Insert(int)")
REF("::Vector::Insert(int,const T&,int)")
REF("::Vector::Insert(int,const::Vector&)")
REF("::Vector::Insert(int,const::Vector&,int,int)")
REF("::Vector::InsertPick(int,pick_::Vector&)")
REF("::Vector::Append(const::Vector&)")
REF("::Vector::Append(const::Vector&,int,int)")
REF("::Vector::AppendPick(pick_::Vector&)")
REF("::Vector::Drop(int)")
REF("::Vector::Top()")
REF("::Vector::Top()const")
REF("::Vector::Pop()")
REF("::Vector::operator T*()")
REF("::Vector::operator const T*()const")
REF("::Vector::operator<<(const T&)")
REF("::Vector::operator|(pick_ T&)")
REF("::Vector::Serialize(::Stream&)")
REF("::Vector::Vector()")
REF("::Vector::~Vector()")
REF("::Vector::Vector(pick_::Vector&)")
REF("::Vector::operator=(pick_::Vector&)")
REF("::Vector::IsPicked()const")
REF("::Vector::Vector(const::Vector&,int)")
REF("::DeepCopyOption::operator<<=(T&,const T&)")
REF("::Vector::ValueType::typedef")
REF("::Vector::Iterator::typedef")
REF("::Vector::ConstIterator::typedef")
REF("::Vector::Begin()const")
REF("::Vector::End()const")
REF("::Vector::GetIter(int)const")
REF("::Vector::Begin()")
REF("::Vector::End()")
REF("::Vector::GetIter(int)")
REF("::Vector::Swap(::Vector&,::Vector&)")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2;~~~64; Vector&][s10;~~~64; "
"template <class [*@4 T]>&][s0;3 &][s10;:`:`:Vector`:`:class:~~~64;* [* class ]Vector&][s0; "
"&][s12; [*C@4 T]-|Type of elements stored in Vector. T is required to be [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ "
"moveable][*")
TOPIC_TEXT(
"/  ]and must have either [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ deep copy "
"constructor], [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick constructor] or [*/ "
"default constructor].&][s5; The most effective flavor of basic random access container. "
"Its features are derived from fact that it is implemented as simple C`-like vector "
"of elements.&][s5; Disadvantage of Vector is common requirement f")
TOPIC_TEXT(
"or elements to be stored in it `- they must be [*/ moveable]. Another disadvantage "
"is fact that many operations invalidate references (that means C`+`+ references and "
"pointers) to elements.&][s5; Like any other NTL container, Vector is [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ "
"moveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick and optional "
"deep copy] transfer semantics")
TOPIC_TEXT(
". Calling methods of picked Vector is logic error with exception of&][s0;C+75 void "
"[* operator`=](pick`_ Vector`& [*@3 v])&][s0;/+75 [/C void ][*/C operator<<`=][/C "
"(const Vector`& ][*/C@3 v][/C )][/  ](defined using DeepCopyOptionTemplate)&][s0;C+75 "
"void [* Clear]()&][s0;C+75 bool [* IsPicked]() const&][s5; Optional deep copy is "
"implemented through DeepCopyOptionTemplate macro.&][s3; Members&][s0")
TOPIC_TEXT(
";3 &][s4;:`:`:Vector`:`:Add`(`): T`& [* Add]()&][s6; Adds new default constructed "
"element to Vector.&][s11; Requires T to have default constructor.&][s11; Invalidates "
"iterators and references to Vector.&][s1; [*/ Return value]-|Reference to newly added "
"default constructed element.&][s0;3 &][s4;:`:`:Vector`:`:Add`(const T`&`):~~~.992; "
"void [* Add](const T`& [*@3 x])&][s6; Adds new element with spec")
TOPIC_TEXT(
"ified value to Vector.&][s11; Requires T to have deep copy constructor.&][s11; Invalidates "
"iterators and references to Vector.&][s1; [*C@3 x]-|The value that is copied to newly "
"created element.&][s0;3 &][s4;:`:`:Vector`:`:AddPick`(pick`_ T`&`): void [* AddPick](pick`_ "
"T`& [*@3 x])&][s6; Adds new element to Vector and picks value of parameter to it. "
"&][s11; Requires T to have pick constructor.&][s1")
TOPIC_TEXT(
"1; Invalidates iterators and references to Vector.&][s1; [*C@3 x]-|Source instance "
"of T that is to be picked.&][s0;3 &][s4;:`:`:Vector`:`:AddN`(int`): void [* AddN](int "
"[*@3 n])&][s6; Adds specified number of default constructed elements to Vector.&][s11; "
"Requires T to have default constructor.&][s11; Invalidates iterators and references "
"to Vector.&][s1; [*C@3 n]-|Number of elements to add&][s0;3 ")
TOPIC_TEXT(
"&][s4;:`:`:Vector`:`:operator`[`]`(int`)const: const T`& [* operator`[`]](int [*@3 "
"i]) const&][s6; Returns a reference to the element at specified position.&][s1; [*C@3 "
"i]-|Position of element.&][s1; [*/ Return value]-|Constant reference to the element.&][s0;3 "
"&][s4;:`:`:Vector`:`:operator`[`]`(int`): T`& [* operator`[`]](int [*@3 i])&][s6; "
"Returns a reference to the element at specified position.")
TOPIC_TEXT(
"&][s1; [*C@3 i]-|Position of element.&][s1; [*/ Return value]-|Reference to the "
"element.&][s0;3 &][s4;:`:`:Vector`:`:GetCount`(`)const: int [* GetCount]() const&][s6; "
"Return the number of elements in Vector.&][s1; [*/ Return value]-|Actual number of "
"elements.&][s0;3 &][s4;:`:`:Vector`:`:IsEmpty`(`)const: bool [* IsEmpty]() const&][s6; "
"Tests whether Vector  is empty. Same as GetCount() `=`= 0.&][s1")
TOPIC_TEXT(
"; [*/ Return value]-|true if Vector is empty, false otherwise.&][s0;3 &][s4;:`:`:Vector`:`:Trim`(int`): "
"void [* Trim](int [*@3 n])&][s6; Reduces number of elements in Vector to specified "
"number. Required number must be less than or equal to actual number of elements in "
"Vector.&][s11; Invalidates iterators to Vector.&][s1; [*C@3 n]-|Required number of "
"elements.&][s0;3 &][s4;:`:`:Vector`:`:SetCount`")
TOPIC_TEXT(
"(int`): void [* SetCount](int [*@3 n])&][s6; Changes count of elements in Vector "
"to specified value. If required number of elements is greater than actual number, "
"newly added elements are default constructed. If Vector has to increase capacity, "
"the new capacity will exactly match required number of elements (unlike [* SetCountR]).&][s11; "
"Requires T to have default constructor.&][s11; Invalidates i")
TOPIC_TEXT(
"terators and references to Vector.&][s1; [*C@3 n]-|Required number of elements.&][s0;3 "
"&][s4;:`:`:Vector`:`:SetCount`(int`,const T`&`): void [* SetCount](int [*@3 n], const "
"T`& [*@3 init])&][s6; Changes count of elements in Vector to specified value. If "
"required number of elements is greater than actual number, newly added elements are "
"initialized to specified value using copy constructor. If Vect")
TOPIC_TEXT(
"or has to increase capacity, the new capacity will exactly match required number "
"of elements (unlike [* SetCountR]).&][s11; Requires T to have deep copy constructor.&][s11; "
"Invalidates iterators and references to Vector.&][s1; [*C@3 n]-|Required number of "
"elements.&][s1; [*C@3 init]-|Initialization value of newly added elements.&][s0;3 "
"&][s4;:`:`:Vector`:`:SetCountR`(int`): void [* SetCountR](int ")
TOPIC_TEXT(
"[*@3 n])&][s6; Changes count of elements in Vector to specified value. If required "
"number of elements is greater than actual number, newly added elements are default "
"constructed. If Vector has to increase capacity, the new capacity will be greater "
"than required number of elements (unlike [* SetCount]) to allow adding other elements "
"without further increasing capacity.&][s11; Requires T to have def")
TOPIC_TEXT(
"ault constructor.&][s11; Invalidates iterators and references to Vector.&][s1; [*C@3 "
"n]-|Required number of elements.&][s0;3 &][s4;:`:`:Vector`:`:SetCountR`(int`,const "
"T`&`):~~~.992; void [* SetCountR](int [*@3 n], const T`& [*@3 init])&][s6; Changes "
"count of elements in Vector to specified value. If required number of elements is "
"greater than actual number, newly added elements are initialized to")
TOPIC_TEXT(
" specified value using copy constructor. If Vector has to increase capacity, the "
"new capacity will be greater than required number of elements (unlike [* SetCount]) "
"to allow adding other elements without further increasing capacity.&][s11; Requires "
"T to have deep copy constructor.&][s11; Invalidates iterators and references to Vector.&][s1; "
"[*C@3 n]-|Required number of elements.&][s1; [*C@3 init]-")
TOPIC_TEXT(
"|Initialization value of newly added elements.&][s0;3 &][s4;:`:`:Vector`:`:Clear`(`):~~~.992; "
"void [* Clear]()&][s6; Removes all elements from Vector. Capacity is also cleared "
"to zero.&][s11; Invalidates iterators and references to Vector.&][s0;3 &][s4;:`:`:Vector`:`:At`(int`):~~~.992; "
"T`& [* At](int [*@3 i])&][s6; If specified position is lower than number of elements "
"in Vector ([* i < GetCount()")
TOPIC_TEXT(
"]), returns reference to element at specified position. Otherwise increases number "
"of elements in Vector to [* i `+ 1]. Newly added elements are default constructed. "
"If Vector has to increase capacity, the new capacity will be greater than required "
"number of elements to allow adding other elements without further increasing capacity.&][s11; "
"Requires T to have default constructor.&][s11; Invalidate")
TOPIC_TEXT(
"s iterators and references to Vector.&][s1; [*C@3 i]-|Position of required element.&][s1; "
"[*/ Return value]-|Reference to required element.&][s0;3 &][s4;:`:`:Vector`:`:At`(int`,const "
"T`&`): T`& [* At](int [*@3 i], const T`& [*@3 x])&][s6; If specified position is "
"lower than number of elements in Vector ([* i < GetCount()]), returns reference to "
"element at specified position. Otherwise increases nu")
TOPIC_TEXT(
"mber of elements in Vector to [* i `+ 1]. Newly added elements are copy constructed "
"from [* x]. If Vector has to increase capacity, the new capacity will be greater "
"than required number of elements to allow adding other elements without further increasing "
"capacity.&][s11; Requires T to have deep copy constructor.&][s11; Invalidates iterators "
"and references to Vector.&][s1; [*C@3 i]-|Position of re")
TOPIC_TEXT(
"quired element.&][s1; [*C@3 x]-|Initialization value of newly added elements.&][s1; "
"[*/ Return value]-|Reference to required element.&][s0;3 &][s4;:`:`:Vector`:`:Shrink`(`): "
"void [* Shrink]()&][s6; Minimizes memory consumption of Vector by decreasing capacity "
"to number of elements.&][s0;3 &][s4;:`:`:Vector`:`:Reserve`(int`):~~~.992; void [* "
"Reserve](int [*@3 xtra])&][s6; Reserves capacity. If requ")
TOPIC_TEXT(
"ired capacity is greater than current capacity, capacity is increased to the required "
"value.&][s1; [*C@3 xtra]-|Required capacity.&][s0;3 &][s4;:`:`:Vector`:`:GetAlloc`(`)const:~~~.992; "
"int [* GetAlloc]() const&][s6; Returns current capacity of Vector.&][s1; [*/ Return "
"value]-|Capacity of Vector.&][s0;3 &][s4;:`:`:Vector`:`:Set`(int`,const T`&`,int`):~~~.992; "
"void [* Set](int [*@3 i], const T`& [*")
TOPIC_TEXT(
"@3 x], int [*@3 count] `= 1)&][s6; Sets requested number of elements starting at "
"the position [* i] to the specified value. If required number of elements exceeds "
"existing elements of Vector then elements are added to Vector.&][s11; Requires T "
"to have deep copy constructor.&][s11; Invalidates iterators and references to Vector.&][s1; "
"[*C@3 i]-|Starting position.&][s1; [*C@3 x]-|Value.&][s1; [*C@3 ")
TOPIC_TEXT(
"count]-|Number of elements.&][s0;3 &][s4;:`:`:Vector`:`:Remove`(int`,int`):~~~.992; "
"void [* Remove](int [*@3 i], int [*@3 count] `= 1)&][s6; Removes requested number "
"of elements.&][s11; Invalidates iterators and references to Vector.&][s1; [*C@3 i]-|Position.&][s1; "
"[*C@3 count]-|Number of elements to remove.&][s0;3 &][s4;:`:`:Vector`:`:Remove`(const "
"int`*`,int`): void [* Remove](const int `*[*@3 s")
TOPIC_TEXT(
"orted`_list], int [*@3 n])&][s6; Removes number of elements from Vector. Time of "
"operation almost does not depend on number of elements.&][s11; Invalidates iterators "
"and references to Vector.&][s1; [*C@3 sorted`_list]-|Pointer to array of positions "
"to remove. It must be sorted from lowest to greatest value.&][s1; [*C@3 n]-|Number "
"of elements to remove.&][s0;3 &][s4;:`:`:Vector`:`:Remove`(const`:`:")
TOPIC_TEXT(
"Vector`<int`>`&`): void [* Remove](const Vector<int>`& [*@3 sorted`_list])&][s6; "
"Removes number of elements form Vector. Same as [* Remove(sorted`_list, sorted`_list.GetCount())].&][s11; "
"Invalidates iterators and references to Vector.&][s1; [*C@3 sorted`_list]-|Sorted "
"Vector of positions to remove.&][s0;3 &][s4;:`:`:Vector`:`:InsertN`(int`,int`):~~~.992; "
"void [* InsertN](int [*@3 i], int [*@3 coun")
TOPIC_TEXT(
"t] `= 1)&][s6; Inserts a specified number of default constructed elements at a specified "
"position.&][s11; Requires T to have default constructor.&][s11; Invalidates iterators "
"and references to Vector.&][s1; [*C@3 i]-|Position.&][s1; [*C@3 count]-|Number of "
"elements to insert.&][s0;3 &][s4;:`:`:Vector`:`:Insert`(int`):~~~.992; T`& [* Insert](int "
"[*@3 i])&][s6; Inserts one default constructed elemen")
TOPIC_TEXT(
"t at the specified position.&][s1;*1 Requires T to have default constructor.&][s11; "
"Invalidates iterators and references to Vector.&][s1; [*C@3 i]-|Position.&][s0;3 "
"&][s4;:`:`:Vector`:`:Insert`(int`,const T`&`,int`): void [* Insert](int [*@3 i], "
"const T`& [*@3 x], int [*@3 count] `= 1)&][s6; Inserts a specified number of elements, "
"setting them to a specified value.&][s11;~~~.992; Requires T to hav")
TOPIC_TEXT(
"e deep copy constructor.&][s11; Invalidates iterators and references to Vector.&][s1; "
"[*C@3 i]-|Position.&][s1; [*C@3 x]-|Value of inserted elements.&][s1; [*C@3 count]-|Number "
"of elements to insert.&][s0;3 &][s4;:`:`:Vector`:`:Insert`(int`,const`:`:Vector`&`):~~~.992; "
"void [* Insert](int [*@3 i], const Vector`& [*@3 x])&][s6; Inserts all elements from "
"another Vector.&][s11;~~~.992; Requires T to ")
TOPIC_TEXT(
"have deep copy constructor.&][s11; Invalidates iterators and references to Vector.&][s1; "
"[*C@3 i]-|Position.&][s1; [*C@3 x]-|Source Vector.&][s0;3 &][s4;:`:`:Vector`:`:Insert`(int`,const`:`:Vector`&`,int`,int`):~~~.992; "
"void [* Insert](int [*@3 i], const Vector`& [*@3 x], int [*@3 offset], int [*@3 count])&][s6; "
"Inserts a range of elements from another Vector.&][s11;~~~.992; Requires T to have "
"dee")
TOPIC_TEXT(
"p copy constructor.&][s11; Invalidates iterators and references to Vector.&][s1; "
"[*C@3 i]-|Insertion position.&][s1; [*C@3 x]-|Source Vector.&][s1; [*C@3 offset]-|Position "
"of first element in source Vector to be inserted.&][s1; [*C@3 count]-|Number of elements "
"to insert.&][s0;3 &][s4;:`:`:Vector`:`:InsertPick`(int`,pick`_`:`:Vector`&`):~~~.992; "
"void [* InsertPick](int [*@3 i], pick`_ Vector`& [*@3")
TOPIC_TEXT(
" x])&][s6; Inserts source Vector at specified position using pick transfer semantics. "
"It is faster than deep copy insert, does not use deep copy constructor for T, but "
"destroys source Vector.&][s11; Invalidates iterators and references to Vector.&][s1; "
"[*C@3 i]-|Insertion position.&][s1; [*C@3 x]-|Source Vector.&][s0;3 &][s4;:`:`:Vector`:`:Append`(const`:`:Vector`&`): "
"void [* Append](const Vector`")
TOPIC_TEXT(
"& [*@3 x])&][s6; Appends all elements of source Vector.&][s11;~~~.992; Requires "
"T to have deep copy constructor.&][s11; Invalidates iterators and references to Vector.&][s1; "
"[*C@3 x]-|Source Vector.&][s0;3 &][s4;:`:`:Vector`:`:Append`(const`:`:Vector`&`,int`,int`): "
"void [* Append](const Vector`& [*@3 x], int [*@3 o], int [*@3 c])&][s6; Appends a "
"range of elements from source Vector.&][s11;~~~.992;")
TOPIC_TEXT(
" Requires T to have deep copy constructor.&][s11; Invalidates iterators and references "
"to Vector.&][s1; [*C@3 x]-|Source Vector.&][s1; [*C@3 o]-|Position of first element "
"in source Vector to be inserted.&][s1; [*C@3 c]-|Number of elements to insert.&][s0;3 "
"&][s4;:`:`:Vector`:`:AppendPick`(pick`_`:`:Vector`&`):~~~.992; void [* AppendPick](pick`_ "
"Vector`& [*@3 x])&][s6; Appends source Vector using p")
TOPIC_TEXT(
"ick transfer semantics. It is faster than deep copy insert, does not use deep copy "
"constructor for T, but destroys source Vector by picking.&][s11; Invalidates iterators "
"and references to Vector.&][s1; [*C@3 x]-|Source Vector.&][s0;3 &][s4;:`:`:Vector`:`:Drop`(int`): "
"void [* Drop](int [*@3 n] `= 1)&][s6; Drops specified number of last elements in "
"the Vector (same as Trim(GetCount() `- n)).&][s1; [")
TOPIC_TEXT(
"*C@3 n]-|Number of elements.&][s0;3 &][s4;:`:`:Vector`:`:Top`(`): T`& [* Top]()&][s6; "
"Returns reference to the last element in the Vector.&][s1; [*/ Return value]-|Reference "
"of last element in the Vector.&][s0;3 &][s4;:`:`:Vector`:`:Top`(`)const: const T`& "
"[* Top]() const&][s6; Returns constant reference to the last element in the Vector.&][s1; "
"[*/ Return value]-|Reference of last element in the V")
TOPIC_TEXT(
"ector.&][s0;3 &][s4;:`:`:Vector`:`:Pop`(`):~~~.992; T [* Pop]()&][s6; Drops last "
"element of Vector and returns its value.&][s11; Requires T to have deep copy constructor.&][s1; "
"[*/ Return value]-|Value of dropped element.&][s0;3 &][s4;:`:`:Vector`:`:operator "
"T`*`(`): [* operator T`*]()&][s6; Returns non`-constant pointer to elements.&][s1; "
"[*/ Return value]-|Pointer to elements.&][s0;3 &][s4;:`:`:")
TOPIC_TEXT(
"Vector`:`:operator const T`*`(`)const: [* operator const T`*]() const&][s6; Returns "
"constant pointer to elements.&][s1; [*/ Return value]-|Pointer to elements.&][s0;3 "
"&][s4;:`:`:Vector`:`:operator`<`<`(const T`&`): Vector`& [* operator<<](const T`& "
"[*@3 x])&][s6; Operator replacement of [* void Add(const T`&x)]. By returning reference "
"to to Vector allows adding more elements in single expression, ")
TOPIC_TEXT(
"thus e.g. allowing to construct temporary Vector as part of expression like Foo((Vector<int>() "
"<< 1 << 2 << 4)).&][s11; Requires T to have deep copy constructor.&][s11; Invalidates "
"iterators and references to Vector.&][s1; [*C@3 x]-|The value that is copied to newly "
"created element.&][s1; [*/ Return value]-|Reference to Vector (`*this).&][s0;3 &][s4;:`:`:Vector`:`:operator`|`(pick`_ "
"T`&`):~~~.992;")
TOPIC_TEXT(
" Vector`& [* operator`|](pick`_ T`& [*@3 x])&][s6; Operator replacement of [* void "
"AddPick(pick`_ T`&x)]. By returning reference to to Vector allows adding more elements "
"in single expression, thus e.g. allowing to construct temporary Vector as part of "
"expression.&][s11; Requires T to have pick constructor.&][s11; Invalidates iterators "
"and references to Vector.&][s1; [*C@3 x]-|Source instance of T ")
TOPIC_TEXT(
"that is to be picked.&][s1; [*/ Return value]-|Reference to Vector (`*this).&][s0;3 "
"&][s4;:`:`:Vector`:`:Serialize`(`:`:Stream`&`): void [* Serialize](Stream`& [*@3 "
"s])&][s6; Serializes content of Vector to/from Stream. Works only if NTL is used "
"as part of UPP.&][s11; Requires T to have serialization operator defined.&][s1; [*C@3 "
"s]-|Target/source stream.&][s0;3 &][s4;:`:`:Vector`:`:Vector`(`): [*")
TOPIC_TEXT(
" Vector]()&][s6; Default constructor. Constructs empty Vector.&][s0;3 &][s4;:`:`:Vector`:`:`~Vector`(`): "
"`~[* Vector]()&][s6; Destructor. Calls destructors of all elements in Vector.&][s0;3 "
"&][s4;:`:`:Vector`:`:Vector`(pick`_`:`:Vector`&`): [* Vector](pick`_ Vector`& [*@3 "
"v])&][s6; Pick constructor. Transfers source Vector in low constant time, but destroys "
"it by picking.&][s1; [*C@3 v]-|Source Ve")
TOPIC_TEXT(
"ctor.&][s0;3 &][s4;:`:`:Vector`:`:operator`=`(pick`_`:`:Vector`&`): void [* operator]`=(pick`_ "
"Vector`& [*@3 v])&][s6; Pick operator. Transfers source Vector in low constant time, "
"but destroys it by picking.&][s1; [*C@3 v]-|Source Vector.&][s0;3 &][s4;:`:`:Vector`:`:IsPicked`(`)const: "
"bool [* IsPicked]() const&][s6; Returns [* true] if Vector is in picked state.&][s1; "
"[*/ Return value]-|[* true] i")
TOPIC_TEXT(
"f Vector is in picked state, [* false] otherwise.&][s0;3 &][s4;:`:`:Vector`:`:Vector`(const`:`:Vector`&`,int`): "
"[* Vector](const [* Vector]`& [*@3 v], int)&][s6; Optional deep copy constructor.&][s11; "
"Requires T to have deep copy constructor or optional deep copy constructor.&][s1; "
"[*C@3 v]-|Source Vector.&][s0;3 &][s4;:`:`:DeepCopyOption`:`:operator`<`<`=`(T`&`,const "
"T`&`): void [* operator<<`=](")
TOPIC_TEXT(
"const Vector`& [*@3 v])&][s6; Optional deep copy operator. Defined using DeepCopyOptionTemplate.&][s11; "
"Requires T to have deep copy operator or optional deep copy operator.&][s1; [*C@3 "
"v]-|Source Vector.&][s0;3 &][s4;:`:`:Vector`:`:ValueType`:`:typedef:~~~.992;* [* "
"typedef T ]ValueType&][s6; Typedef of T for use in templated algorithms.&][s0;3 &][s4;:`:`:Vector`:`:Iterator`:`:typedef: "
"typedef T `")
TOPIC_TEXT(
"*[* Iterator];&][s6; Iterator type. Iterator is guaranteed to be of T`* type.&][s0;3 "
"&][s4;:`:`:Vector`:`:ConstIterator`:`:typedef: typedef T `*[* ConstIterator];&][s6; "
"Constant iterator type. Iterator is guaranteed to be of const T`* type.&][s0;3 &][s4;:`:`:Vector`:`:Begin`(`)const: "
"ConstIterator [* Begin]() const&][s6; Returns constant iterator to the first element "
"in Vector.&][s1; [*/ Return va")
TOPIC_TEXT(
"lue]-|Iterator.&][s0;3 &][s4;:`:`:Vector`:`:End`(`)const: ConstIterator [* End]() "
"const&][s6; Returns constant iterator to the position just beyond the last element "
"in Vector.&][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:Vector`:`:GetIter`(int`)const: "
"ConstIterator [* GetIter](int [*@3 i]) const&][s6; Returns constant iterator to the "
"element at specified position. Same as [* Begin() `+ i].")
TOPIC_TEXT(
" Benefit of this methods is that in debug mode [* pos] is range checked.&][s1; [*C@3 "
"i]-|Required position.&][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:Vector`:`:Begin`(`): "
"Iterator [* Begin]()&][s6; Returns non`-constant iterator to the first element in "
"Vector.&][s1; [*/ Return value]-|Iterator.&][s0;3 &][s4;:`:`:Vector`:`:End`(`): Iterator "
"[* End]()&][s6; Returns non`-constant iterator ")
TOPIC_TEXT(
"to the position just beyond the last element in Vector.&][s1; [*/ Return value]-|Iterator.&][s0;3 "
"&][s4;:`:`:Vector`:`:GetIter`(int`): Iterator [* GetIter](int [*@3 i])&][s6; Returns "
"non`-constant iterator to the element at specified position. Same as [* Begin() `+ "
"pos]. Benefit of this methods is that in debug mode [* pos] is range checked.&][s1; "
"[*C@3 i]-|Required position.&][s1; [*/ Return valu")
TOPIC_TEXT(
"e]-|Iterator.&][s0;3 &][s4;:`:`:Vector`:`:Swap`(`:`:Vector`&`,`:`:Vector`&`): friend "
"void [* Swap](Vector`& [*@3 a], Vector`& [*@3 b])&][s6; Specialization of generic "
"[* Swap] for Vector. Swaps Vector in simple constant time operation.&][s1; [*C@3 "
"a]-|First Vector to swap.&][s1; [*C@3 b]-|Second Vector to swap.]")
