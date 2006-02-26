TITLE("Callbacks")
REF("::Callback1::Callback1()")
REF("::Callback1::~Callback1()")
REF("::Callback1::Clear()")
REF("::Gate::ClearTrue()")
REF("::Gate::ClearFalse()")
REF("::Callback1::operator bool()const")
REF("::Callback1::Execute(P1)const")
REF("::Callback1::operator()(P1)const")
REF("::Callback1::Empty()")
REF("::Callback4Action::Callback4Action()")
REF("::Callback4Action::~Callback4Action()")
REF("::Callback4Action::Execute(P1,P2,P3,P4)")
REF("::callback(OBJECT*,void(METHOD::*)(P1 p1))")
REF("::pteback(OBJECT*,void(METHOD::*)(P1 p1))")
REF("::callback(void(*)(P1 p1))")
REF("::callback1(Object*,void(M::*)(P),T)")
REF("::callback1(const Object*,void(M::*)(P)const,T)")
REF("::pteback1(Object*,void(M::*)(P),T)")
REF("::callback1(void(*)(P),T)")
REF("::callback1(Object*,void(M::*)(P1,P),T)")
REF("::callback1(const Object*,void(M::*)(P1,P)const,T)")
REF("::pteback1(Object*,void(M::*)(P1,P),T)")
REF("::callback1(void(*)(P1,P),T)")
REF("::callback2(Object*,R(O::*)(A,B),T1,T2)")
REF("::callback2(const Object*,R(O::*)(A,B)const,T1,T2)")
REF("::pteback2(Object*,R(O::*)(A,B),T1,T2)")
REF("::callback2(R(*)(A,B),T1,T2)")
REF("::THISBACK(x)")
REF("::THISBACK1(x, arg)")
REF("::THISBACK2(m, a, b)")
REF("::PTEBACK(x)")
REF("::PTEBACK1(x, arg)")
REF("::PTEBACK2(m, a, b)")
REF("::CallbackArgTarget::CallbackArgTarget()")
REF("::CallbackArgTarget::operator const T&()")
REF("::CallbackArgTarget::IsNullInstance()const")
REF("::CallbackArgTarget::operator[](const T&)")
TOPIC_TEXT(
"[2 $$0,0#00000000000000000000000000000000:Default][l288;i1120;a17;O9;~~~.1408;2 "
"$$1,0#10431211400427159095818037425705:param][a83;*R6 $$2,5#31310162474203024125188417583966:caption][b83;*4 "
"$$3,5#07864147445237544204411237157677:title][i288;O9;C2 $$4,6#40027414424643823182269349404212:item][b42;a42;2 "
"$$5,5#45413000475342174754091244180557:text][l288;b42;a17;2 $$6,6#27521748481378242620020725143825:")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2; Callbacks&][s5; Callbacks "
"can be described as a very generalized form of function pointers. Each Callback represents "
"some kind of action (usually calling a certain function or a certain object method) "
"that can be invoked at any time.&][s5; There ar")
TOPIC_TEXT(
"e several basic callback types, depending on number of parameters passed and return "
"value. In order to keep description of callbacks short, all these types are described "
"in a single `\"parametrized`\" description, with parameters [*/@(128.0.255) highlighted].&][s3; "
"Callback types&][s6; &][ {{1514:5185:1933:1368h1;t/17b/17@(204) [s12;i0;b0;a0;~~*/@(128.0.255)1 "
"[%00-00 CallbackType]]:: [s12;b0;a0;*/@(")
TOPIC_TEXT(
"128.0.255)1 [%00-00 parameters]]:: [s12;b0;a0;*/@(128.0.255)1 [%00-00 arguments]]:: "
"[s12;b0;a0;*/@(128.0.255)1 [%00-00 return`_type]]::@2 [s12;i0;b0;a0;*C+75 [%00-00 "
"Callback]]:: [s12;b0;a0;*/C@(0.0.255)+75 [%00-00 none]]:: [s12;b0;a0;*/C@(0.0.255)+75 "
"[%00-00 none]]:: [s12;b0;a0;*/C+75 [%00-00 void]]:: [s12;b0;a0;*C+75 [%00-00 Callback1]]:: "
"[s12;b0;a0;*/C+75 [%00-00 class P1]]:: [s12;b0;a0;*/C+75 ")
TOPIC_TEXT(
"[%00-00 P1]]:: [s12;b0;a0;*/C+75 [%00-00 void]]:: [s12;b0;a0;*C+75 Callback2]:: "
"[s12;b0;a0;*/C+75 class P1, class P2]:: [s12;b0;a0;*/C+75 P1, P2]:: [s12;b0;a0;*/C+75 "
"void]:: [s12;b0;a0;*C+75 Callback3]:: [s12;i0;b0;a0;~~*/C+75 class P1, class P2, "
"class P3]:: [s12;i0;b0;a0;~~*/C+75 P1, P2, P3]:: [s12;i0;b0;a0;~~*/C+75 void]:: [s12;b0;a0;*C+75 "
"Callback4]:: [s12;i0;b0;a0;~~*/C+75 class P1, class P2, ")
TOPIC_TEXT(
"class P3, class P4]:: [s12;i0;b0;a0;~~*/C+75 P1, P2, P3, P4]:: [s12;i0;b0;a0;~~*/C+75 "
"void]:: [s12;b0;a0;*C+75 Gate]:: [s12;b0;a0;*/C@(0.0.255)+75 none]:: [s12;b0;a0;*/C@(0.0.255)+75 "
"none]:: [s12;b0;a0;*/C+75 bool]:: [s12;b0;a0;*C+75 Gate1]:: [s12;b0;a0;*/C+75 class "
"P1]:: [s12;b0;a0;*/C+75 P1]:: [s12;b0;a0;*/C+75 bool]:: [s12;b0;a0;*C+75 Gate2]:: "
"[s12;b0;a0;*/C+75 class P1, class P2]:: [s12;b0;a0;")
TOPIC_TEXT(
"*/C+75 P1, P2]:: [s12;b0;a0;*/C+75 bool]:: [s12;b0;a0;*C+75 Gate3]:: [s12;i0;b0;a0;~~*/C+75 "
"class P1, class P2, class P3]:: [s12;i0;b0;a0;~~*/C+75 P1, P2, P3]:: [s12;i0;b0;a0;~~*/C+75 "
"bool]}}&][s0; &][s0; &][s10; `[ template_<[*/@(128.0.255) parameters]> `]&][s10;*/@(128.0.255) "
"[%00-00*/@0 class_]CallbackType&][s6; &][s6; Callback type. Callbacks are moveable "
"types with fast deep copy (using refer")
TOPIC_TEXT(
"ence counting).&][s6; &][s0; &][s4; [%00-00 explicit_][*/@(128.0.255) CallbackType][%00-00 "
"(][*/@(128.0.255) CallbackType][%00-00 Action<][*/@(128.0.255) arguments>][%00-00 "
"_`*][%00-00*@3 newaction][%00-00 )]&][s6; Constructs callback based from new action.&][s1; "
"[%00-00*C@3 newaction]-|Action. Must be created using [* new] operator. Callback "
"takes over its ownership.&][s0; &][s4;:`:`:Callback1`:`")
TOPIC_TEXT(
":Callback1`(`): [*/@(128.0.255) CallbackType][%00-00 ()]&][s6; Creates an empty "
"callback.&][s0; &][s4; [*/@(128.0.255) CallbackType][%00-00 (const_][*/@(128.0.255) "
"CallbackType][%00-00 `&_][%00-00*@3 c][%00-00 )]&][s6; Copy constructor. &][s1; [%00-00*C@3 "
"c]-|Source callback.&][s0; &][s4;:`:`:Callback1`:`:`~Callback1`(`): [%00-00 `~][*/@(128.0.255) "
"CallbackType][%00-00 ()]&][s6; Destructor.&][s0; ")
TOPIC_TEXT(
"&][s4;:`:`:Callback1`:`:Clear`(`): [%00-00 void_][%00-00* Clear][%00-00 ()]&][s6; "
"Empties the callback.&][s0; &][s4;:`:`:Gate`:`:ClearTrue`(`): [%00-00 void_][%00-00* "
"ClearTrue][%00-00 ()]&][s11; Only in Gate callbacks.&][s6; Clears Gate. After this "
"method is invoked, the Gate performs no actions but returns true.&][s0; &][s4;:`:`:Gate`:`:ClearFalse`(`): "
"[%00-00 void_][%00-00* ClearFalse][%00-00 (")
TOPIC_TEXT(
")]&][s11; Only in Gate callbacks.&][s6; Clears Gate. After this method is invoked, "
"the Gate performs no actions and returns false. This is the same as Clear.&][s0; "
"&][s4;:`:`:Callback1`:`:operator bool`(`)const: [%00-00* operator_bool][%00-00 ()_const]&][s1; "
"[*/ Return value]-|True if the callback is valid. Invoking a valid callback invokes "
"an action.&][s0; &][s4;:`:`:Callback1`:`:Execute`(P1`)con")
TOPIC_TEXT(
"st: [%00-00*/@(128.0.255) return`_type][%00-00 _][%00-00* Execute][%00-00 (][*/@(128.0.255) "
"arguments][%00-00 )_const]&][s6; Executes the callback with given set of arguments.&][s0; "
"&][s4;:`:`:Callback1`:`:operator`(`)`(P1`)const: [*/@(128.0.255) return`_type][%00-00 "
"_][%00-00* operator()][%00-00 (][*/@(128.0.255) arguments][%00-00 )_const]&][s6; "
"Same as the Execute method.&][s0; &][s4;:`:`:Callba")
TOPIC_TEXT(
"ck1`:`:Empty`(`):*/@(128.0.255) &][s10; [%00-00 `[ template_<][%00-00*/@(128.0.255) "
"parameters][%00-00 > `]]&][s10;* [%00-00* struct_][%00-00/@(128.0.255) CallbackType][%00-00 "
"Action]&][s6; This is the abstract base class for callback action implementations. "
"Callback holds reference counted pointer to instance of class derived from this class.&][s0; "
"&][s4;:`:`:Callback4Action`:`:Callback4Action`(`")
TOPIC_TEXT(
"): [%00-00*/@(128.0.255) CallbackType][%00-00* Action][%00-00 ()]&][s6; Constructor. "
"Assigns 1 to the reference count.&][s0; &][s4;:`:`:Callback4Action`:`:`~Callback4Action`(`): "
"[%00-00 `~][%00-00*/@(128.0.255) CallbackType][%00-00* Action][%00-00 ()]&][s6; Virtual "
"destructor.&][s0; &][s4;:`:`:Callback4Action`:`:Execute`(P1`,P2`,P3`,P4`):@3 [%00-00@0 "
"virtual ][*/@(128.0.255) return`_type][%00-00@0")
TOPIC_TEXT(
" _][%00-00*@0 Execute][%00-00@0 (][%00-00*/@(128.0.255) arguments][%00-00@0 )_`=_][%00-00 "
"0]&][s6; Performs the action with specified set of arguments.&][s0; &][s4; [%00-00 "
"virtual bool_][%00-00* IsValid][%00-00 ()_const]&][s1; [*/ Return value]-|true if "
"the Action is valid. Default implementation returns true.&][s0; &][s4;* [%00-00* "
"Atomic_][%00-00 count]&][s6; Member variable used as reference c")
TOPIC_TEXT(
"ount.&][s0; &][s3; Common functions creating callbacks&][s13;:`:`:callback`(OBJECT`*`,void`(METHOD`:`:`*`)`(P1 "
"p1`)`): template_<class [*@4 OBJECT], class [*@4 METHOD][*@(128.0.255) ,][*@4  ][*/@(128.0.255) "
"parameters]>&][s14; [%00-00*/@(128.0.255) CallbackType][%00-00 <][%00-00*/@(128.0.255) "
"arguments][%00-00 > ][%00-00* callback][%00-00 (][%00-00*@4 OBJECT][%00-00 _`*][%00-00*@3 "
"object][%00-00 ,")
TOPIC_TEXT(
" void_(][%00-00*@4 METHOD][%00-00 `::`*][%00-00*@3 method][%00-00 )(][*/@(128.0.255) "
"arguments][%00-00 ))]&][s6; Creates a callback that invokes the specified method "
"of the specified object.&][s1; [%00-00*C@3 object]-|Pointer to the object.&][s1; "
"[%00-00*C@3 method]-|Pointer to the method.&][s1; [*/ Return value]-|Created callback.&][s0; "
"&][s13;:`:`:callback`(OBJECT`*`,void`(METHOD`:`:`*`)`(P1 p1`")
TOPIC_TEXT(
")`): template_<class [*@4 OBJECT], class [*@4 METHOD][*@(128.0.255) ,][*@4  ][*/@(128.0.255) "
"parameters]>&][s14; [%00-00*/@(128.0.255) CallbackType][%00-00 <][%00-00*/@(128.0.255) "
"arguments][%00-00 > ][%00-00* callback][%00-00 (const ][%00-00*@4 OBJECT][%00-00 "
"_`*][%00-00*@3 object][%00-00 , void_(][%00-00*@4 METHOD][%00-00 `::`*][%00-00*@3 "
"method][%00-00 )(][*/@(128.0.255) arguments][%00-00 ) con")
TOPIC_TEXT(
"st)]&][s6; Constant variant of callback. The created Callback object invokes a constant "
"method of the object.&][s1; [%00-00*C@3 object]-|Pointer to the object.&][s1; [%00-00*C@3 "
"method]-|Pointer to the constant method to invoke.&][s1; [*/ Return value]-|Created "
"callback.&][s0; &][s13;:`:`:pteback`(OBJECT`*`,void`(METHOD`:`:`*`)`(P1 p1`)`): `[ "
"template_<[*/@(128.0.255) parameters]> `]&][s14; [%00-0")
TOPIC_TEXT(
"0*/@(128.0.255) CallbackType]<[*/@(128.0.255) arguments]> [%00-00* pteback][%00-00 "
"(][%00-00*@4 OBJECT][%00-00 _`*][%00-00*@3 object][%00-00 , void_(][%00-00*@4 METHOD][%00-00 "
"`::`*][%00-00*@3 method][%00-00 )(][*/@(128.0.255) arguments][%00-00 ))]&][s6; Creates "
"a Callback object which is similar to the one created by callback, but which can "
"be safely invoked even after destruction of [@3 object] ")
TOPIC_TEXT(
"(results in no operation).&][s1; [%00-00*C@3 object]-|Pointer to the object. Must "
"be an ancestor of the [^topic`:`/`/Core`/src`/PtePtr`$en`-us`#`:`:Pte`:`:template "
"`<class T`> class Pte^ Pte] class.&][s1; [%00-00*C@3 method]-|Pointer to the method.&][s1; "
"[*/ Return value]-|Created callback.&][s0; &][s13;:`:`:callback`(void`(`*`)`(P1 p1`)`): "
"`[ template_<[*/@(128.0.255) parameters]> `]&][s14; [%00-")
TOPIC_TEXT(
"00*/@(128.0.255) CallbackType]<[*/@(128.0.255) arguments]> [%00-00* callback][%00-00 "
"(void_(`*][%00-00*@3 fn][%00-00 )(][*/@(128.0.255) arguments][%00-00 ))]&][s6; Creates "
"a Callback object that invokes a non`-member function.&][s1; [%00-00*C@3 fn]-|Pointer "
"to the function.&][s1; [*/ Return value]-|Created callback.&][s0; &][s13; `[ template_<[*/@(128.0.255) "
"parameters]> `]&][s14; [%00-00*/@(128.0")
TOPIC_TEXT(
".255) CallbackType]<[*/@(128.0.255) arguments]>[%00-00  ][%00-00* Proxy][%00-00 "
"(][%00-00*/@(128.0.255) CallbackType]<[*/@(128.0.255) arguments]>[%00-00 `&_][%00-00*@3 "
"cb][%00-00 )]&][s6; Creates a Callback object that invokes another Callback object. "
"Useful for callback routing (e.g. when some parent Ctrl wants to reroute some of "
"its child callbacks via its interface). Note that this function sto")
TOPIC_TEXT(
"res a reference to the target callback in the created callback `- storing callbacks "
"that are used as Proxy targets in Vector flavor of containers is not a good idea.&][s1; "
"[%00-00*C@3 cb]-|Target callback.&][s1; [*/ Return value]-|Created callback.&][s0; "
"&][s13; `[ template_<[*/@(128.0.255) parameters]> `]&][s14; [*/@(128.0.255) CallbackType]<[*/@(128.0.255) "
"arguments]> [%00-00* callback][%00-00 (")
TOPIC_TEXT(
"][*/@(128.0.255) CallbackType]<[*/@(128.0.255) arguments]> [%00-00 _][%00-00*@3 "
"cb1][%00-00 , ][*/@(128.0.255) CallbackType]<[*/@(128.0.255) arguments]> [%00-00 "
"_][%00-00*@3 cb2][%00-00 )]&][s6; Creates a Callback object that invokes two other "
"callbacks. Note that this function stores a reference to the target callbacks in "
"the created callback `- storing callbacks that are used as targets here in ")
TOPIC_TEXT(
"Vector flavor of containers is not a good idea.&][s1; [%00-00*C@3 cb1]-|First target "
"callback.&][s1; [%00-00*C@3 cb2]-|Second target callback.&][s1; [*/ Return value]-|Created "
"callback.&][s0; &][s13; `[ template_<[*/@(128.0.255) parameters]> `]&][s14; [*/@(128.0.255) "
"CallbackType]<[*/@(128.0.255) arguments]>[%00-00 `&_][%00-00* operator<<][%00-00 "
"(][*/@(128.0.255) CallbackType]<[*/@(128.0.255) arg")
TOPIC_TEXT(
"uments]> [%00-00 `&_][%00-00*@3 a][%00-00 , ][*/@(128.0.255) CallbackType]<[*/@(128.0.255) "
"arguments]>[%00-00 _][%00-00*@3 b][%00-00 )]&][s6; Operator variant of the previous "
"function. Allows chaining thus imitating callback insertion.&][s1; [%00-00*C@3 a]-|First "
"callback. Its value is replaced by a callback that calls both the first and the second "
"callback.&][s1; [%00-00*C@3 b]-|Second callback.&")
TOPIC_TEXT(
"][s1; [*/ Return value]-|Reference to the first callback.&][s0;3 &][s3; Special "
"functions creating callbacks&][s0; &][s4;:`:`:callback1`(Object`*`,void`(M`:`:`*`)`(P`)`,T`): "
"[%00-00 template_<class_][%00-00*@4 Object][%00-00 , class_][%00-00*@4 M][%00-00 "
", class_][%00-00*@4 P][%00-00 , class_][%00-00*@4 T][%00-00 >]&][s4;:`:`:callback1`(Object`*`,void`(M`:`:`*`)`(P`)`,T`): "
"[%00-00 Callback_][%00-0")
TOPIC_TEXT(
"0* callback1][%00-00 (][%00-00*@4 Object][%00-00 _`*][%00-00*@3 object][%00-00 , "
"void_(][%00-00*@4 M][%00-00 `::`*][%00-00*@3 method][%00-00 )(][%00-00*@4 P][%00-00 "
"), ][%00-00*@4 T][%00-00 _][%00-00*@3 arg][%00-00 )]&][s6; Returns a no`-parameter "
"callback for a method expecting a single parameter. The parameter is supplied as "
"[@3 arg] and stored in the Callback.&][s1; [%00-00*C@3 object]-|Object.")
TOPIC_TEXT(
"&][s1; [%00-00*C@3 method]-|Method pointer.&][s1; [%00-00*C@3 arg]-|Argument&][s1; "
"[*/ Return value]-|Created callback.&][s0; &][s4;:`:`:callback1`(const Object`*`,void`(M`:`:`*`)`(P`)const`,T`): "
"[%00-00 template_<class_][%00-00*@4 Object][%00-00 , class_][%00-00*@4 M][%00-00 "
", class_][%00-00*@4 P][%00-00 , class_][%00-00*@4 T][%00-00 >]&][s4;:`:`:callback1`(const "
"Object`*`,void`(M`:`:`*`)`(P`)con")
TOPIC_TEXT(
"st`,T`): [%00-00 Callback_][%00-00* callback1][%00-00 (const_][%00-00*@4 Object][%00-00 "
"_`*][%00-00*@3 object][%00-00 , void_(][%00-00*@4 M][%00-00 `::`*][%00-00*@3 method][%00-00 "
")(][%00-00*@4 P][%00-00 )_const, ][%00-00*@4 T][%00-00 _][%00-00*@3 arg][%00-00 )]&][s6; "
"Constant variant of the previous callback1 routine.&][s1; [%00-00*C@3 object]-|Object.&][s1; "
"[%00-00*C@3 method]-|Method pointer.&]")
TOPIC_TEXT(
"[s1; [%00-00*C@3 arg]-|Argument&][s1; [*/ Return value]-|Created callback.&][s0;*C@4 "
"&][s4;:`:`:pteback1`(Object`*`,void`(M`:`:`*`)`(P`)`,T`): [%00-00 template_<class_][%00-00*@4 "
"Object][%00-00 , class_][%00-00*@4 M][%00-00 , class_][%00-00*@4 P][%00-00 , class_][%00-00*@4 "
"T][%00-00 >]&][s4;:`:`:pteback1`(Object`*`,void`(M`:`:`*`)`(P`)`,T`): [%00-00 Callback_][%00-00* "
"pteback1][%00-00 (][%00-00*@4")
TOPIC_TEXT(
" Object][%00-00 _`*][%00-00*@3 object][%00-00 , void_(][%00-00*@4 M][%00-00 `::`*][%00-00*@3 "
"method][%00-00 )(][%00-00*@4 P][%00-00 ), ][%00-00*@4 T][%00-00 _][%00-00*@3 arg][%00-00 "
")]&][s6; Returns a no`-parameter callback for a method expecting a single parameter. "
"Parameter is supplied as [@3 arg] and stored in the Callback. The created Callback "
"object can be safely invoked even after the object")
TOPIC_TEXT(
" instance is destructed.&][s1; [%00-00*C@3 object]-|Object. Must be an ancestor "
"of [^topic`:`/`/Core`/src`/PtePtr`$en`-us`#`:`:Pte`:`:template `<class T`> class "
"Pte^ Pte].&][s1; [%00-00*C@3 method]-|Method pointer.&][s1; [%00-00*C@3 arg]-|Argument&][s1; "
"[*/ Return value]-|Created callback.&][s0; &][s4;:`:`:callback1`(void`(`*`)`(P`)`,T`): "
"[%00-00 template_<class_][%00-00*@4 T][%00-00 , class_][%00")
TOPIC_TEXT(
"-00*@4 P][%00-00 >]&][s4;:`:`:callback1`(void`(`*`)`(P`)`,T`): [%00-00 Callback_][%00-00* "
"callback1][%00-00 (void_(`*][%00-00*@3 fn][%00-00 )(][%00-00*@4 P][%00-00 ), ][%00-00*@4 "
"T][%00-00 _][%00-00*@3 arg][%00-00 )]&][s6; Returns a no`-parameter callback for "
"a non`-member function expecting a single parameter. Parameter is supplied as [@3 "
"arg] and stored in the Callback.&][s1; [%00-00*C@3 fn]-|Fu")
TOPIC_TEXT(
"nction.&][s1; [%00-00*C@3 arg]-|Argument.&][s1; [*/ Return value]-|Created callback.&][s0; "
"&][s4;:`:`:callback1`(Object`*`,void`(M`:`:`*`)`(P1`,P`)`,T`): [%00-00 template_<class_][%00-00*@4 "
"Object][%00-00 , class_][%00-00*@4 M][%00-00 , class_][%00-00*@4 P1][%00-00 , class_][%00-00*@4 "
"P][%00-00 , class_][%00-00*@4 T][%00-00 >]&][s4;:`:`:callback1`(Object`*`,void`(M`:`:`*`)`(P1`,P`)`,T`): "
"[%00-00 C")
TOPIC_TEXT(
"allback1<][%00-00*@4 P1][%00-00 >_][%00-00* callback1][%00-00 (][%00-00*@4 Object][%00-00 "
"_`*][%00-00*@3 object][%00-00 , void_(][%00-00*@4 M][%00-00 `::`*][%00-00*@3 method][%00-00 "
")(][%00-00*@4 P1][%00-00 , ][%00-00*@4 P][%00-00 ), ][%00-00*@4 T][%00-00 _][%00-00*@3 "
"arg][%00-00 )]&][s6; Returns a single`-parameter callback for a method expecting "
"two parameters. Additional parameter is supplied a")
TOPIC_TEXT(
"s [@3 arg] and stored in the Callback.&][s1; [%00-00*C@3 object]-|Object.&][s1; "
"[%00-00*C@3 method]-|Method pointer.&][s1; [%00-00*C@3 arg]-|Second argument to the "
"method.&][s1; [*/ Return value]-|Created callback.&][s0; &][s4;:`:`:callback1`(const "
"Object`*`,void`(M`:`:`*`)`(P1`,P`)const`,T`): [%00-00 template_<class_][%00-00*@4 "
"Object][%00-00 , class_][%00-00*@4 M][%00-00 , class_][%00-00*@4 P1][")
TOPIC_TEXT(
"%00-00 , class_][%00-00*@4 P][%00-00 , class_][%00-00*@4 T][%00-00 >]&][s4;:`:`:callback1`(const "
"Object`*`,void`(M`:`:`*`)`(P1`,P`)const`,T`): [%00-00 Callback1<][%00-00*@4 P1][%00-00 "
">_][%00-00* callback1][%00-00 (const_][%00-00*@4 Object][%00-00 _`*][%00-00*@3 object][%00-00 "
", void_(][%00-00*@4 M][%00-00 `::`*][%00-00*@3 method][%00-00 )(][%00-00*@4 P1][%00-00 "
", ][%00-00*@4 P][%00-00 )_const, ][")
TOPIC_TEXT(
"%00-00*@4 T][%00-00 _][%00-00*@3 arg][%00-00 )]&][s6; Constant variant of the previous "
"callback.&][s1; [%00-00*C@3 object]-|Object.&][s1; [%00-00*C@3 method]-|Method pointer.&][s1; "
"[%00-00*C@3 arg]-|Second argument to the method.&][s1; [*/ Return value]-|Created "
"callback.&][s0; &][s4;:`:`:pteback1`(Object`*`,void`(M`:`:`*`)`(P1`,P`)`,T`): [%00-00 "
"template_<class_][%00-00*@4 Object][%00-00 , class_")
TOPIC_TEXT(
"][%00-00*@4 M][%00-00 , class_][%00-00*@4 P1][%00-00 , class_][%00-00*@4 P][%00-00 "
", class_][%00-00*@4 T][%00-00 >]&][s4;:`:`:pteback1`(Object`*`,void`(M`:`:`*`)`(P1`,P`)`,T`): "
"[%00-00 Callback1<][%00-00*@4 P1][%00-00 >_][%00-00* pteback1][%00-00 (][%00-00*@4 "
"Object][%00-00 _`*][%00-00*@3 object][%00-00 , void_(][%00-00*@4 M][%00-00 `::`*][%00-00*@3 "
"method][%00-00 )(][%00-00*@4 P1][%00-00 , ][%00-")
TOPIC_TEXT(
"00*@4 P][%00-00 ), ][%00-00*@4 T][%00-00 _][%00-00*@3 arg][%00-00 )]&][s6; Returns "
"a single`-parameter callback for a method expecting two parameters. Additional parameter "
"is supplied as [@3 arg] and stored in the Callback. The created Callback can be safely "
"invoked even after the object instance is destructed.&][s1; [%00-00*C@3 object]-|Object. "
"Must be an ancestor of [^topic`:`/`/Core`/src`/PtePt")
TOPIC_TEXT(
"r`$en`-us`#`:`:Pte`:`:template `<class T`> class Pte^ Pte].&][s1; [%00-00*C@3 method]-|Method "
"pointer.&][s1; [%00-00*C@3 arg]-|Second argument to the method.&][s0; &][s4;:`:`:callback1`(void`(`*`)`(P1`,P`)`,T`): "
"[%00-00 template_<class_][%00-00*@4 T][%00-00 , class_][%00-00*@4 P1][%00-00 , class_][%00-00*@4 "
"P][%00-00 >]&][s4;:`:`:callback1`(void`(`*`)`(P1`,P`)`,T`): [%00-00 Callback1<][%00-00*@4 "
"P")
TOPIC_TEXT(
"1][%00-00 >_][%00-00* callback1][%00-00 (void_(`*][%00-00*@3 fn][%00-00 )(][%00-00*@4 "
"P1][%00-00 , ][%00-00*@4 P][%00-00 ), ][%00-00*@4 T][%00-00 _][%00-00*@3 arg][%00-00 "
")]&][s6; Returns a single`-parameter callback for a non`-member function expecting "
"two parameters. Additional parameter is supplied as [@3 arg] and stored in the Callback.&][s1; "
"[%00-00*C@3 fn]-|Function.&][s1; [%00-00*C@3 arg]-|")
TOPIC_TEXT(
"Method pointer.&][s1; [*/ Return value]-|Created callback.&][s0; &][s4; [%00-00 "
"template_<class_][%00-00*@4 T][%00-00 , class_][%00-00*@4 P][%00-00 >]&][s4; [%00-00 "
"Callback_][%00-00* callback1][%00-00 (Callback1<][%00-00*@4 P][%00-00 >_][%00-00*@3 "
"cb][%00-00 , ][%00-00*@4 T][%00-00 _][%00-00*@3 arg][%00-00 )]&][s6; Combines a single`-parameter "
"callback and an argument into a no`-parameter callbac")
TOPIC_TEXT(
"k.&][s1; [%00-00*C@3 cb]-|Single`-parameter callback.&][s1; [%00-00*C@3 arg]-|Argument.&][s1; "
"[*/ Return value]-|Created callback.&][s0; &][s4; [%00-00 template_<class_][%00-00*@4 "
"T][%00-00 , class_][%00-00*@4 P1][%00-00 , class_][%00-00*@4 P][%00-00 >]&][s4; [%00-00 "
"Callback1<][%00-00*@4 P1][%00-00 >_][%00-00* callback1][%00-00 (Callback2<][%00-00*@4 "
"P1][%00-00 , ][%00-00*@4 P][%00-00 >_][%00-00*")
TOPIC_TEXT(
"@3 cb][%00-00 , ][%00-00*@4 T][%00-00 _][%00-00*@3 arg][%00-00 )]&][s6; Combines "
"a double`-parameter callback and an argument into a single parameter callback.&][s1; "
"[%00-00*C@3 cb]-|Double`-parameter callback.&][s1; [%00-00*C@3 arg]-|Argument.&][s1; "
"[*/ Return value]-|Created callback.&][s0; &][s4;:`:`:callback2`(Object`*`,R`(O`:`:`*`)`(A`,B`)`,T1`,T2`): "
"[%00-00 template_<class_][%00-00*@4 Object")
TOPIC_TEXT(
"][%00-00 , class_][%00-00*@4 R][%00-00 , class_][%00-00*@4 O][%00-00 , class_][%00-00*@4 "
"A][%00-00 , class_][%00-00*@4 B][%00-00 , class_][%00-00*@4 T1][%00-00 , class_][%00-00*@4 "
"T2][%00-00 >]&][s4;:`:`:callback2`(Object`*`,R`(O`:`:`*`)`(A`,B`)`,T1`,T2`): [%00-00 "
"Callback_][%00-00* callback2][%00-00 (][%00-00*@4 Object][%00-00 _`*][%00-00*@3 object][%00-00 "
", ][%00-00*@4 R][%00-00 _(][%00-00*@4 O]")
TOPIC_TEXT(
"[%00-00 `::`*][%00-00*@3 method][%00-00 )(][%00-00*@4 A][%00-00 , ][%00-00*@4 B][%00-00 "
"), ][%00-00*@4 T1][%00-00 _][%00-00*@3 arg1][%00-00 , ][%00-00*@4 T2][%00-00 _][%00-00*@3 "
"arg2][%00-00 )]&][s6; Returns a no`-parameter callback for a method taking two parameters. "
"Parameters are supplied as [@3 arg1] and [@3 arg2. ]They are stored in the created "
"callback object.&][s1; [%00-00*C@3 object]-|Obje")
TOPIC_TEXT(
"ct.&][s1; [%00-00*C@3 method]-|Method pointer.&][s1; [%00-00*C@3 arg1]-|First argument.&][s1; "
"[%00-00*C@3 arg2]-|Second argument.&][s1; [*/ Return value]-|Created callback.&][s0; "
"&][s4;:`:`:callback2`(const Object`*`,R`(O`:`:`*`)`(A`,B`)const`,T1`,T2`): [%00-00 "
"template_<class_][%00-00*@4 Object][%00-00 , class_][%00-00*@4 R][%00-00 , class_][%00-00*@4 "
"O][%00-00 , class_][%00-00*@4 A][%00-00 , cla")
TOPIC_TEXT(
"ss_][%00-00*@4 B][%00-00 , class_][%00-00*@4 T1][%00-00 , class_][%00-00*@4 T2][%00-00 "
">]&][s4;:`:`:callback2`(const Object`*`,R`(O`:`:`*`)`(A`,B`)const`,T1`,T2`): [%00-00 "
"Callback_][%00-00* callback2][%00-00 (const_][%00-00*@4 Object][%00-00 _`*][%00-00*@3 "
"object][%00-00 , ][%00-00*@4 R][%00-00 _(][%00-00*@4 O][%00-00 `::`*][%00-00*@3 method][%00-00 "
")(][%00-00*@4 A][%00-00 , ][%00-00*@4 B][%00-00")
TOPIC_TEXT(
" )_const, ][%00-00*@4 T1][%00-00 _][%00-00*@3 arg1][%00-00 , ][%00-00*@4 T2][%00-00 "
"_][%00-00*@3 arg2][%00-00 )]&][s6; Constant version of the previous callback.&][s1; "
"[%00-00*C@3 object]-|Object.&][s1; [%00-00*C@3 method]-|Method.&][s1; [%00-00*C@3 "
"arg1]-|First argument.&][s1; [%00-00*C@3 arg2]-|Second argument.&][s1; [*/ Return "
"value]-|Created callback.&][s1;*C@3 &][s4;:`:`:pteback2`(Object`*`,R")
TOPIC_TEXT(
"`(O`:`:`*`)`(A`,B`)`,T1`,T2`): [%00-00 template_<class_][%00-00*@4 Object][%00-00 "
", class_][%00-00*@4 R][%00-00 , class_][%00-00*@4 O][%00-00 , class_][%00-00*@4 A][%00-00 "
", class_][%00-00*@4 B][%00-00 , class_][%00-00*@4 T1][%00-00 , class_][%00-00*@4 "
"T2][%00-00 >]&][s4;:`:`:pteback2`(Object`*`,R`(O`:`:`*`)`(A`,B`)`,T1`,T2`): [%00-00 "
"Callback_][%00-00* pteback2][%00-00 (][%00-00*@4 Object][%00-00")
TOPIC_TEXT(
" _`*][%00-00*@3 object][%00-00 , ][%00-00*@4 R][%00-00 _(][%00-00*@4 O][%00-00 `::`*][%00-00*@3 "
"method][%00-00 )(][%00-00*@4 A][%00-00 , ][%00-00*@4 B][%00-00 ), ][%00-00*@4 T1][%00-00 "
"_][%00-00*@3 arg1][%00-00 , ][%00-00*@4 T2][%00-00 _][%00-00*@3 arg2][%00-00 )]&][s6; "
"Returns a no`-parameter callback for a method taking two parameters. Parameter are "
"supplied as [@3 arg1] and [@3 arg2. ]They are ")
TOPIC_TEXT(
"stored in the created callback. The created Callback can be safely invoked even "
"after the object instance is destructed.&][s1; [%00-00*C@3 object]-|Object.&][s1; "
"[%00-00*C@3 method]-|Method pointer.&][s1; [%00-00*C@3 arg1]-|First argument.&][s1; "
"[%00-00*C@3 arg2]-|Second argument.&][s1; [*/ Return value]-|Created callback.&][s0; "
"&][s0; &][s4;:`:`:callback2`(R`(`*`)`(A`,B`)`,T1`,T2`): [%00-00 templ")
TOPIC_TEXT(
"ate_<class_][%00-00*@4 R][%00-00 , class_][%00-00*@4 A][%00-00 , class_][%00-00*@4 "
"B][%00-00 , class_][%00-00*@4 T1][%00-00 , class_][%00-00*@4 T2][%00-00 >]&][s4;:`:`:callback2`(R`(`*`)`(A`,B`)`,T1`,T2`): "
"[%00-00 Callback_][%00-00* callback2][%00-00 (][%00-00*@4 R][%00-00 _(`*][%00-00*@3 "
"fn][%00-00 )(][%00-00*@4 A][%00-00 , ][%00-00*@4 B][%00-00 ), ][%00-00*@4 T1][%00-00 "
"_][%00-00*@3 arg1][%00-00")
TOPIC_TEXT(
" , ][%00-00*@4 T2][%00-00 _][%00-00*@3 arg2][%00-00 )]&][s6; Returns a no`-parameter "
"callback for a non`-member function taking two parameters. Parameter are supplied "
"as [@3 arg1] and [@3 arg2. ]They are stored in the created callback.&][s1; [%00-00*C@3 "
"fn]-|Function pointer.&][s1; [%00-00*C@3 arg1]-|First argument.&][s1; [%00-00*C@3 "
"arg2]-|Second argument.&][s1; [*/ Return value]-|Created callbac")
TOPIC_TEXT(
"k.&][s0; &][s4; [%00-00 template_<class_][%00-00*@4 A][%00-00 , class_][%00-00*@4 "
"B][%00-00 , class_][%00-00*@4 T1][%00-00 , class_][%00-00*@4 T2][%00-00 >]&][s4; "
"[%00-00 Callback_][%00-00* callback2][%00-00 (Callback2<][%00-00*@4 A][%00-00 , ][%00-00*@4 "
"B][%00-00 >_][%00-00*@3 cb][%00-00 , ][%00-00*@4 T1][%00-00 _][%00-00*@3 arg1][%00-00 "
", ][%00-00*@4 T2][%00-00 _][%00-00*@3 arg2][%00-00 )]&][s6;")
TOPIC_TEXT(
" Combines a two`-parameter callback and two arguments into a no`-parameter callback.&][s1; "
"[%00-00*C@3 cb]-|Two`-parameter callback.&][s1; [%00-00*C@3 arg1]-|First argument.&][s1; "
"[%00-00*C@3 arg2]-|Second argument.&][s1; [*/ Return value]-|Created callback.&][s0;*C@3 "
"&][s3; Callback macros&][s0; &][s0; Callback macros reduce verbosity of instance "
"method callbacks. They expect CLASSNAME to be type")
TOPIC_TEXT(
"def`-ed as the type of current class and return method callbacks bound to the [*@(0.0.255) "
"this] pointer.&][s0; &][s4;:`:`:THISBACK`(x`): [%00-00* THISBACK][%00-00 (][%00-00*@3 "
"x][%00-00 )]&][s6; Expands to callback(this, `&CLASSNAME`::[@3 x]).&][s1; [%00-00*C@3 "
"x]-|Name of method.&][s0; &][s4;:`:`:THISBACK1`(x`, arg`): [%00-00* THISBACK1][%00-00 "
"(][%00-00*@3 x][%00-00 , ][%00-00*@3 arg][%00-00 )]")
TOPIC_TEXT(
"&][s6; Expands to callback1(this, `&CLASSNAME`::[@3 x], [@3 arg]).&][s1; [%00-00*C@3 "
"x]-|Name of method.&][s1; [%00-00*C@3 arg]-|Additional parameter.&][s0; &][s4;:`:`:THISBACK2`(m`, "
"a`, b`): [%00-00* THISBACK2][%00-00 (][%00-00*@3 m][%00-00 , ][%00-00*@3 a][%00-00 "
", ][%00-00*@3 b][%00-00 )]&][s6; Expands to callback2(this, `&CLASSNAME`::[@3 x], "
"[@3 a],[@3  b]).&][s1; [%00-00*C@3 m]-|Name of metho")
TOPIC_TEXT(
"d.&][s1; [%00-00*C@3 a]-|First additional parameter.&][s1; [%00-00*C@3 b]-|Second "
"additional parameter.&][s0; &][s4;:`:`:PTEBACK`(x`): [%00-00* PTEBACK][%00-00 (][%00-00*@3 "
"x][%00-00 )]&][s6; Expands to pteback(this, `&CLASSNAME`::[@3 x]).&][s1; [%00-00*C@3 "
"x]-|Name of method.&][s0; &][s4;:`:`:PTEBACK1`(x`, arg`): [%00-00* PTEBACK1][%00-00 "
"(][%00-00*@3 x][%00-00 , ][%00-00*@3 arg][%00-00 )]&][s6; ")
TOPIC_TEXT(
"Expands to pteback1(this, `&CLASSNAME`::[@3 x], [@3 arg]).&][s1; [%00-00*C@3 x]-|Name "
"of method.&][s1; [%00-00*C@3 arg]-|Additional parameter.&][s0; &][s4;:`:`:PTEBACK2`(m`, "
"a`, b`): [%00-00* PTEBACK2][%00-00 (][%00-00*@3 m][%00-00 , ][%00-00*@3 a][%00-00 "
", ][%00-00*@3 b][%00-00 )]&][s6; Expands to pteback2(this, `&CLASSNAME`::[@3 x], "
"[@3 a],[@3  b]).&][s1; [%00-00*C@3 m]-|Name of method.&][s1; [%")
TOPIC_TEXT(
"00-00*C@3 a]-|First additional parameter.&][s1; [%00-00*C@3 b]-|Second additional "
"parameter.&][s6; &][s0; &][s3; [%00-00 CallbackArgTarget]&][s0; &][s10; [%00-00 template_<class_][%00-00*@4 "
"T][%00-00 >]&][s10;* [%00-00* class_][%00-00 CallbackArgTarget]&][s6; This helper "
"class is useful in a situation when a set of callbacks define some output value. "
"Example is pop`-up menu that provides selection")
TOPIC_TEXT(
" of one character `- in that case, CallbackArgTarget can be used instead of dedicated "
"method to obtain result of user selection.&][s12; [%00-00*C@4 T]-|Type of output "
"value. Must be able to be assigned Null.&][s6; &][s4;:`:`:CallbackArgTarget`:`:CallbackArgTarget`(`): "
"[%00-00* CallbackArgTarget][%00-00 ()]&][s6; Constructor. Assigns Null to the output "
"value.&][s6; &][s4;:`:`:CallbackArgTarget`:`:o")
TOPIC_TEXT(
"perator const T`&`(`): [%00-00* operator_const_T`&][%00-00 ()]&][s1; [*/ Return "
"value]-|Result.&][s0; &][s4;:`:`:CallbackArgTarget`:`:IsNullInstance`(`)const: [%00-00 "
"bool_][%00-00* IsNullInstance][%00-00 ()_const]&][s1; [*/ Return value]-|True if "
"there is not Null in output value.&][s0; &][s4;:`:`:CallbackArgTarget`:`:operator`[`]`(const "
"T`&`): [%00-00 Callback_][%00-00* operator`[`]][%00-00 (con")
TOPIC_TEXT(
"st_T`&_][%00-00*@3 value][%00-00 )]&][s6; Returns a Callback that, when invoked, "
"assigns [@3 value] to the output value.&][s1; [%00-00*C@3 value]-|Value.&][s1; [*/ "
"Return value]-|Callback.&][s0;3 ]")
