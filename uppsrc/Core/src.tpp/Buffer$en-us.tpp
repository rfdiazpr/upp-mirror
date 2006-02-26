TITLE("Buffer")
REF("::Buffer::class")
REF("::Buffer::Buffer()")
REF("::Buffer::Buffer(int)")
REF("::Buffer::Buffer(int,const T&)")
REF("::Buffer::Buffer(pick_::Buffer&)")
REF("::Buffer::operator=(pick_::Buffer&)")
REF("::Buffer::operator T*()")
REF("::Buffer::operator const T*()const")
REF("::Buffer::operator~()")
REF("::Buffer::operator~()const")
REF("::Buffer::Alloc(int)")
REF("::Buffer::Alloc(int,const T&)")
REF("::Buffer::Clear()")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2; Buffer&][s10;:`:`:Buffer`:`:class: "
"template_<class_[*@4 T]>&][s10;* [* class_]Buffer&][s6; &][s12; [%00-00*C@4 T]-|Type "
"of elements stored in Buffer.&][s0; &][s5; Buffer is a simple class used to manage "
"a fixed size plain old C dynamically allocate")
TOPIC_TEXT(
"d vector of elements of a specified type. The size of the buffer is specified as "
"a constructor parameter and it can be also reallocated with new size, while loosing "
"all current data.&][s5; Buffer is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ "
"moveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick] (only) "
"transfer semantics. Calling methods of picked Buffer is logic error w")
TOPIC_TEXT(
"ith the exceptions of&][s0;C+75 void Alloc(int size);&][s0;C+75 void Alloc(int size, "
"const T`& in);&][s0;C+75 void Clear();&][s0;C+75 void operator`=(pick`_ Buffer`& "
"v);&][s12;*C@4 &][s9; [/ Derived from] [%00-00 Moveable< Buffer<T> > ]&][s0;3 &][s4;:`:`:Buffer`:`:Buffer`(`): "
"[%00-00* Buffer][%00-00 ()]&][s6; Constructs an empty buffer.&][s0;3 &][s4;:`:`:Buffer`:`:Buffer`(int`): "
"[%00-00* Buffer][%")
TOPIC_TEXT(
"00-00 (int_][%00-00*@3 size][%00-00 )]&][s6; Constructs the Buffer.&][s11; [@4 T] "
"must have default constructor.&][s1; [%00-00*C@3 size]-|Number of elements.&][s0;3 "
"&][s4;:`:`:Buffer`:`:Buffer`(int`,const T`&`): [%00-00* Buffer][%00-00 (int_][%00-00*@3 "
"size][%00-00 , const_T`&_][%00-00*@3 init][%00-00 )]&][s6; Constructs the Buffer "
"initializing the elements to the specified value.&][s11; [@4 T] mu")
TOPIC_TEXT(
"st have deep copy constructor.&][s1; [%00-00*C@3 size]-|Number of elements.&][s1; "
"[%00-00*C@3 init]-|Initialization value.&][s0;3 &][s4;:`:`:Buffer`:`:Buffer`(pick`_`:`:Buffer`&`): "
"[%00-00* Buffer][%00-00 (pick`__][%00-00* Buffer][%00-00 `&_][%00-00*@3 v][%00-00 "
")]&][s6; Pick constructor.&][s1; [%00-00*C@3 v]-|Source Buffer.&][s0;3 &][s4;:`:`:Buffer`:`:operator`=`(pick`_`:`:Buffer`&`): "
"[%00-00 voi")
TOPIC_TEXT(
"d_][%00-00* operator`=][%00-00 (pick`__Buffer`&_][%00-00*@3 v][%00-00 )]&][s6; Pick "
"operator.&][s1; [%00-00*C@3 v]-|Source buffer to be picked.&][s0;3 &][s4;:`:`:Buffer`:`:operator "
"T`*`(`): [%00-00* operator_T`*][%00-00 ()]&][s1; [*/ Return value]-|Returns a pointer "
"to the first element of the Buffer or NULL if the Buffer is empty.&][s0;3 &][s4;:`:`:Buffer`:`:operator "
"const T`*`(`)const: [%00-00* ")
TOPIC_TEXT(
"operator_const_T`*][%00-00 ()_const]&][s1; [*/ Return value]-|Returns a pointer "
"to the first element of the Buffer or NULL if the Buffer is empty.&][s0;3 &][s4;:`:`:Buffer`:`:operator`~`(`): "
"[%00-00 T_`*][%00-00* operator`~][%00-00 ()]&][s1; [*/ Return value]-|Returns a pointer "
"to the first element of the Buffer or NULL if the Buffer is empty.&][s0;3 &][s4;:`:`:Buffer`:`:operator`~`(`)const: "
"[%00-")
TOPIC_TEXT(
"00 const_T_`*][%00-00* operator`~][%00-00 ()_const]&][s1; [*/ Return value]-|Returns "
"a pointer to the first element of the Buffer or NULL if the Buffer is empty.&][s0;3 "
"&][s4;:`:`:Buffer`:`:Alloc`(int`): [%00-00 void_][%00-00* Alloc][%00-00 (int_][%00-00*@3 "
"size][%00-00 )]&][s6; Clears the buffer and allocates it with the new size. All current "
"elements are lost.&][s11; [@4 T] must have default con")
TOPIC_TEXT(
"structor.&][s1; [%00-00*C@3 size]-|Number of elements.&][s0;3 &][s4;:`:`:Buffer`:`:Alloc`(int`,const "
"T`&`): [%00-00 void_][%00-00* Alloc][%00-00 (int_][%00-00*@3 size][%00-00 , const_T`&_][%00-00*@3 "
"in][%00-00 )]&][s6; Clears the buffer and allocates it with the new size, using the "
"initialization value. All current elements are lost.&][s11; [@4 T] must have deep "
"copy constructor.&][s1; [%00-00*C@3")
TOPIC_TEXT(
" size]-|Number of elements.&][s1; [%00-00*C@3 in]-|Initialization value.&][s0;3 "
"&][s4;:`:`:Buffer`:`:Clear`(`): [%00-00 void_][%00-00* Clear][%00-00 ()]&][s6; Clears "
"the buffer to the same state as default constructor. All current elements are destroyed.]")
