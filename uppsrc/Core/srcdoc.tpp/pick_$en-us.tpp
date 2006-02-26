TITLE("Transfer semantics")
TOPIC_TEXT(
"[2 $$0,0#00000000000000000000000000000000:Default][l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param][a83;*R6 "
"$$2,5#31310162474203024125188417583966:caption][b83;* $$3,5#07864147445237544204411237157677:title][b167;a42;C2 "
"$$4,6#40027414424643823182269349404212:item][b42;a42;2 $$5,5#45413000475342174754091244180557:text][l288;a17;2 "
"$$6,6#27521748481378242620020725143825:desc][")
TOPIC_TEXT(
"l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code][b2503; $$8,0#65142375456100023862071332075487:separator][*@(0.0.255)2 "
"$$9,0#83433469410354161042741608181528:base][t4167;C $$10,0#37138531426314131251341829483380:class][l288;a17;*1 "
"$$11,11#70004532496200323422659154056402:requirement][i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam][b167;C2 "
"$$13,13#924304594434")
TOPIC_TEXT(
"60461911108080531343:item1][i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2][*@2$(0.128.128) "
"$$15,15#34511555403152284025741354420178:NewsDate][l321;*C$7;2 $$16,16#03451589433145915344929335295360:result][l321;b83;a83;*C$7;2 "
"$$17,17#07531550463529505371228428965313:result`-line][l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title][{_}%EN-US "
"[s2; Transfer semantic")
TOPIC_TEXT(
"s&][s3; Deep copy semantics&][s5; Talking about transfer semantics, we are interested "
"in passing values from one place to another, e.g&][s7;l320; int a, b;&][s7;l320; "
"a `= b;&][s5; Here we transfer the value of variable [* b] to variable [* a]. We "
"do this by using the implicit assignment operator of type [* int.] This assignment "
"operator passes value of source to target while preserving value of s")
TOPIC_TEXT(
"ource. This kind of transfer semantics we call [*/ deep copy]. For large number "
"of types, deep copy is good choice as default transfer semantics. However there are "
"types where preserving source is not that important and dropping this feature has "
"advantages. Consider this simple `'container`'&][s7; class IntArray `{&][s7; -|int "
"count;&][s7; -|int `*array;&][s7; -|void Copy(const IntArray`& src) `{&")
TOPIC_TEXT(
"][s7; -|-|array `= new int`[count `= src.count`];&][s7; -|-|memcpy(array, src.array, "
"count `* sizeof(int));&][s7; -|`}&][s7; public:&][s7; -|int`& operator`[`](int i) "
"       `{ return array`[i`]; `}&][s7; -|int  GetCount() const         `{ return count; "
"`}&][s7; -|IntArray(int n)               `{ count `= n; array `= new int`[n`]; `}&][s7; "
"-|IntArray(const IntArray`& src) `{ Copy(src); `}&][s7; -|")
TOPIC_TEXT(
"IntArray`& operator`=(const IntArray`& src)&][s7; -|                            "
"  `{ delete`[`] array; Copy(src) `}&][s7; -|`~IntArray()                   `{ delete`[`] "
"array; `}&][s7; `};&][s5;* [* This shows typical implementation of container class "
"with deep copy transfer semantics. Now consider we want to have a function which "
"returns an ]IntArray&][s7; IntArray MakeArray(int n) `{&][s7; -|Int")
TOPIC_TEXT(
"Array a(n);&][s7; -|for(int i `= 0; i < n; i`+`+)&][s7; -|-|a`[i`] `= i;&][s7; -|return "
"a;&][s7; `}&][s5; and see what happens if we use this function&][s7; IntArray y `= "
"MakeArray(1000);&][s5; Problem here is that [* IntArray] copy constructor will be "
"invoked to copy created [* IntArray] to temporary object and to copy this temporary "
"object to target [* y]. (Some really good C`+`+ compilers might")
TOPIC_TEXT(
" elide these copy constructors in this simple case, but there are other cases where "
"this is not possible). And all these deep copies are made to preserve the value of "
"the source object that is destroyed just one step later anyway. &][s3; Pick semantics&][s5; "
"If we don`'t need to preserve the source value we could simply copy the [* array] "
"member of the source object to the target and somehow disal")
TOPIC_TEXT(
"low delete in [* `~IntArray] of the source object. And in fact, we rarely need deep "
"copies of containers. This is why we have introduced [*/ pick][/  ]transfer semantics. "
"Let us rewrite [* IntArray] with it&][s7; class IntArray `{&][s7; -|int count;&][s7; "
"-|[* mutable] int `*array;&][s7;* -|void Pick(pick`_ IntArray`& src) `{&][s7;* -|-|count "
"`= src.count;&][s7;* -|-|array `= src.array;&][s7;* -|-")
TOPIC_TEXT(
"|src.array `= NULL;&][s7;* -|`}&][s7; public:&][s7; -|int`& operator`[`](int i) "
"       `{ return array`[i`]; `}&][s7; -|int  GetCount() const         `{ return count; "
"`}&][s7; -|IntArray(int n)               `{ count `= n; array `= new int`[n`]; `}&][s7; "
"-|IntArray([* pick`_] IntArray`& src) `{ [* Pick](src); `}&][s7; -|IntArray`& operator`=([* "
"pick`_] IntArray`& src)&][s7; -|                     ")
TOPIC_TEXT(
"         `{ [* if(array) ]delete`[`] array;&][s7; -|                            "
"    Pick(src) `}&][s7; -|`~IntArray()                   `{ [* if(array)] delete`[`] "
"array; `}&][s7; `};&][s5; With pick semantics, no unnecessary copying is made and "
"we are free to use such container as return value. What has to be explained here "
"is `'[* pick`_]`'. The problem is that C`+`+ disallows binding temporarie")
TOPIC_TEXT(
"s to non`-const references `- and that is unfortunately just the thing we need to "
"do here, as we need to change the source temporary returned from a function. C`+`+ "
"allows only one possibility for temporary to be passed `- via const reference. But "
"using const reference in interface would be pretty misguiding here, as what we do "
"is quite opposite `- we change parameter. That is why we decided to&][")
TOPIC_TEXT(
"s7; #define pick`_ const&][s5; Yes, this [* is] [* very ugly], but unfortunately "
"this is best option that the current C`+`+ provides. There are efforts to introduce "
"pick`-related changes in a future version of C`+`+, but for now we have to live with "
"this.&][s3; Pick caveats&][s5; Now let us look in pick semantics little bit deeper. "
"One thing is apparent `- if a type has default pick transfer seman")
TOPIC_TEXT(
"tics, one has to be cautious when using copy constructor or operator`=&][s7; IntArray "
"a `= MakeArray(100);&][s7; IntArray b `= a; // ATTENTION: a is now picked !&][s7; "
"a`[3`] `= 10; //illegal !&][s5; This example shows one important aspect of pick semantics "
"that we use throughout our code: after being picked, number of operations that can "
"be performed on source is limited. In most cases, only allo")
TOPIC_TEXT(
"wed operations here are operator`= and destructor. Real containers add some more "
"(we will provide common list of allowed operations later), but operations accessing "
"or changing data of picked object are always disallowed.&][s5; In fact, we had a "
"choice here `- alternative approach could be to simply reset the source to some default "
"state `- such as in std`::auto`_ptr. However several years of prac")
TOPIC_TEXT(
"tice in using pick semantics taught us that accessing or changing data in picked "
"object is almost always error and usually hard to spot one. So we made them illegal "
"and we are also checking for such logic errors in debug mode.&][s3; Optional deep "
"copy&][s5; Now we have efficient way of transferring values without copying the internal "
"data at the price of invalidating the source object. But what if")
TOPIC_TEXT(
" we would like to preserve source in some cases? No problem, we can simply add alternative "
"methods for the [*/ optional deep copy]. Unlike normal deep copy and pick semantics, "
"where signatures of methods are dictated by C`+`+ (that is, operator`= and copy constructor), "
"here we can choose names of methods ourselves. Anyway, it is a good idea to standardize "
"them. We selected (for type T)&][s7; T(con")
TOPIC_TEXT(
"st T`&, int)&][s5; to be the signature of the [*/ optional deep copy constructor]. "
"We have to distinguish it from pick constructor [*@5 T(pick`_ T`&)] and we can do "
"it only by adding a parameter. It could be anything, but int somewhat resembles solution "
"for postfix operator`+`+. We also need optional deep copy assignment, so we overload&][s7; "
"T`& operator<<`=(const T`&)&][s5; as the [*/ optional d")
TOPIC_TEXT(
"eep copy operator]. Now we can improve our simple container class to include optional "
"deep copy semantics&][s7; class IntArray `{&][s7; -|int count;&][s7; -|mutable int "
"`*array;&][s7; -|void Pick(pick`_ IntArray`& src) `{&][s7; -|-|count `= src.count;&][s7; "
"-|-|array `= src.array;&][s7; -|-|src.array `= NULL;&][s7; -|`}&][s7;* [* -|]void "
"Copy(const IntArray`& src) `{&][s7;* -|-|array `= new int`[c")
TOPIC_TEXT(
"ount `= src.count`];&][s7;* -|-|memcpy(array, src.array, count `* sizeof(int));&][s7;* "
"-|`}&][s7; public:&][s7; -|int`& operator`[`](int i)        `{ return array`[i`]; "
"`}&][s7; -|int  GetCount() const         `{ return count; `}&][s7; -|IntArray(int "
"n)               `{ count `= n; array `= new int`[n`]; `}&][s7; -|IntArray(pick`_ "
"IntArray`& src) `{ Pick(src); `}&][s7; -|IntArray`& operator`=(pick")
TOPIC_TEXT(
"`_ IntArray`& src)&][s7; -|                              `{ if(array) delete`[`] "
"array;&][s7; -|                                Pick(src) `}&][s7;* -|IntArray(const "
"IntArray`& src, int) `{ Copy(src); `}&][s7;* -|IntArray`& operator<<`=(const IntArray`& "
"src)&][s7;* -|                              `{ if(array) delete`[`] array;&][s7;* "
"-|                                Copy(src) `}-|&][s7; -|`~IntArr")
TOPIC_TEXT(
"ay()                   `{ if(array) delete`[`] array; `}&][s7; `};&][s5; With optional "
"deep copy we have full control of transfer&][s7; IntArray a `= MakeArray(100);&][s7; "
"IntArray b(a, 1); // deep copy&][s7; a`[3`] `= 10; //legal&][s7; b <<`= a; // deep "
"copy&][s7; IntArray c(a); // pick&][s7; a `= b; // pick `- now b is picked, a and "
"c contain MakeArray(100)&][s3; Uniform access to deep copy&][s5")
TOPIC_TEXT(
"; But we need even more. Consider this example with real NTL container :&][s7; Vector< "
"IntArray > x, y;&][s7; ....&][s7; // put something to y&][s7; ....&][s7; x <<`= y;&][s5; "
"Here we are deep`-copy assigning container of pick with optional deep`-copy types "
"to another container. During this operation we need deep copies of each individual "
"element of [* y]. The problem is that we need the [* Vector")
TOPIC_TEXT(
"] container to work with both normal deep copy types and with optional deep`-copy "
"types. We need an uniform way to invoke the deep`-copy constructor whether it has "
"[* T(const T`&)] or [* T(const T`&, int)] signature. The solution is to provide two "
"additional functions&][s7; void DeepCopyConstruct(void `*p, const T`& x);&][s7; T "
"  `*DeepCopyNew(const T`& x);&][s5; [* DeepCopyConstruct] constructs a")
TOPIC_TEXT(
" new deep copy of [* x] at the given memory address (much like placement operator "
"new does), [* DeepCopyNew] constructs a new deep copy of [* x] on the heap (like "
"normal new). Now we can provide default versions of these functions that will be "
"used for types with default deep copy &][s7; template <class T>&][s7; inline void "
"DeepCopyConstruct(void `*p, const T`& x) `{&][s7; -|`::new(p) T(x);&][s7; ")
TOPIC_TEXT(
"`}&][s7; &][s7; template <class T>&][s7; inline T `*DeepCopyNew(const T`& x) `{&][s7; "
"-|return new T(x);&][s7; `}&][s5; For types with optional deep copy we simply provide "
"specialized version&][s7; void DeepCopyConstruct(void `*p, const IntArray`& x) `{&][s7; "
"-|`::new(p) IntArray(x, 1);&][s7; `}&][s7; &][s7; IntArray `*DeepCopyNew(const IntArray`& "
"x) `{&][s7; -|return new IntArray(x, 1);&][s7; `}&")
TOPIC_TEXT(
"][s3; Making things easy&][s5; Now there exists a way how to deep copy objects of "
"both default and optional deep copy semantics. Anyway, this might seem pretty annoying "
"`- you have to implement two methods and two global functions to get optional deep "
"copy semantics working. Fortunately, it is possible to automatize this work by using "
"[* DeepCopyOption ]base&][s7; class IntArray : DeepCopyOption<I")
TOPIC_TEXT(
"ntArray> `{&][s7; -|int count;&][s7; -|mutable int `*array;&][s7; -|void Pick(pick`_ "
"IntArray`& src) `{&][s7; -|-|count `= src.count;&][s7; -|-|array `= src.array;&][s7; "
"-|-|src.array `= NULL;&][s7; -|`}&][s7; -|void Copy(const IntArray`& src) `{&][s7; "
"-|-|array `= new int`[count `= src.count`];&][s7; -|-|memcpy(array, src.array, count "
"`* sizeof(int));&][s7; -|`}&][s7; public:&][s7; -|int`& operat")
TOPIC_TEXT(
"or`[`](int i)        `{ return array`[i`]; `}&][s7; -|int  GetCount() const     "
"    `{ return const; `}&][s7; -|IntArray(int n)               `{ count `= n; array "
"`= new int`[n`]; `}&][s7; -|IntArray(pick`_ IntArray`& src) `{ Pick(src); `}&][s7; "
"-|IntArray`& operator`=(pick`_ IntArray`& src)&][s7; -|                          "
"    `{ if(array) delete`[`] array;&][s7; -|                              ")
TOPIC_TEXT(
"  Pick(src) `}&][s7; -|IntArray(const IntArray`& src, int) `{ Copy(src); `}&][s7; "
"-|`~IntArray()                   `{ if(array) delete`[`] array; `}&][s7; `};&][s7; "
"&][s5; Using [* DeepCopyOption] template all you must provide is optional deep copy "
"constructor `- and template generates rest of methods and functions automatically.&][s3; "
"Changing default semantics&][s5; If for some reason you need v")
TOPIC_TEXT(
"ersion of [/ optional] deep copy type with [/ default] deep copy, you can easily "
"create it with [* WithDeepCopy ]template&][s7; IntArray a `= MakeArray(100);&][s7; "
"WithDeepCopy<IntArray> b(a); // b now has deep copy semantics&][s7; a`[3`] `= 10; "
"//legal&][s7; b `= a; // deep copy&][s7; a `= b; // pick&][s7; b`[4`] `= 1; // illegal&][s3; "
"Polymorphic deep copy&][s5;* [* Now there is one last type of")
TOPIC_TEXT(
" transfer semantics. Some of containers allow storing polymorphic elements. E.g. "
"Array allows storing object of type derived from basic element type. This causes "
"new problems in situation when we want to create deep copy of such container `- what "
"we need is polymorphic deep copy of elements. Problem is solved by already introduced "
"]DeepCopyNew&][s7; class Base : PolyDeepCopyNew<Base> `{&][s7; -|..")
TOPIC_TEXT(
"..&][s7; public:&][s7; -|....&][s7; -|virtual Base  `*Copy() const    `{ return "
"new Base(`*this); `}&][s7; `};&][s7; &][s7; class Derived : Base `{&][s7; -|....&][s7; "
"public:&][s7; -|....&][s7; -|virtual Base    `*Copy() const  `{ return new Derived(`*this); "
"`}&][s7; `};&][s7; &][s7; &][s7; ....&][s7; Array<Base> a;&][s7; a.Add(new Base);&][s7; "
"a.Add(new Derived);&][s7; Array<Base> b(a, 1); // dee")
TOPIC_TEXT(
"p copy using Copy members&][s5; [* PolyDeepCopyNew][%EN-GB*  ][%EN-GB base class "
"creates appropriate ][* DeepCopyNew ]specialization based on [* Copy] virtual method. "
"We call this transfer method [*/ polymorphic deep copy].&][s0;3 ]")
