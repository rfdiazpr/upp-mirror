TITLE("One")
REF("::One::class")
REF("::One::One()")
REF("::One::One(T*)")
REF("::One::One(pick_::One<T>&)")
REF("::One::One(const::One<T>&,int)")
REF("::One::~One()")
REF("::One::Free()")
REF("::One::Detach()pick_")
REF("::One::operator-()pick_")
REF("::One::Clear()")
REF("::One::operator=(T*)")
REF("::One::operator=(pick_::One<T>&)")
REF("::One::operator->()const")
REF("::One::operator->()")
REF("::One::operator~()const")
REF("::One::operator~()")
REF("::One::operator*()const")
REF("::One::operator*()")
REF("::One::IsPicked()const")
REF("::One::IsEmpty()const")
REF("::One::operator bool()const")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2;:`:`:One`:`:class: [%00-00 "
"One]&][s10; [%00-00 template_<class_][%00-00*@4 T][%00-00 >]&][s0;3 &][s10;:`:`:One`:`:class:* "
"[%00-00* class_][%00-00 One]&][s6; &][s12; [%00-00*C@4 T]-|Type or base class of "
"element stored in One.&][s5; One is a containe")
TOPIC_TEXT(
"r capable of containing none or single element of type specified as template argument "
"or derived from it.&][s5; One is [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/ "
" ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick and optional deep copy] "
"transfer semantics. Calling methods of picked One is logic error with exception of&][s0;C+75 "
"void [* operator`=](pick`_ One`& [*@3 v])&")
TOPIC_TEXT(
"][s0;/+75 [/C void ][*/C operator<<`=][/C (const One`& ][*/C@3 v][/C )][/  ](defined "
"using DeepCopyOption template base class)&][s0;C+75 void [* Clear]()&][s0;C+75 bool "
"[* IsPicked]() const&][s3; Members&][s0;3 &][s4;:`:`:One`:`:One`(`): [%00-00* One][%00-00 "
"()]&][s6; Constructor. Constructs empty One.&][s0;3 &][s4;:`:`:One`:`:One`(T`*`): "
"[%00-00* One][%00-00 (T_`*][%00-00*@3 newt][%00-00 )]&][s6;")
TOPIC_TEXT(
" Constructs One with content. Content is specified by pointer to object created "
"using operator new. One takes over ownership of this this object. &][s1; [*C@3 newt]-|Object "
"to be added.&][s0;3 &][s4;:`:`:One`:`:One`(pick`_`:`:One`<T`>`&`): [%00-00* One][%00-00 "
"(pick`__][%00-00* One][%00-00 <T>`&_][%00-00*@3 p][%00-00 )]&][s6; Pick constructor. "
"Transfers source One in low constant time, but destroy")
TOPIC_TEXT(
"s it by picking. &][s1; [*C@3 p]-|Source One.&][s0;3 &][s4;:`:`:One`:`:One`(const`:`:One`<T`>`&`,int`): "
"[%00-00* One][%00-00 (const_][%00-00* One][%00-00 <T>`&_][%00-00*@3 p][%00-00 , int)]&][s6; "
"Optional deep copy constructor.&][s11; Requires T to have deep copy constructor or "
"optional deep copy constructor.&][s1; [*C@3 p]-|Source One.&][s0;3 &][s4;:`:`:One`:`:`~One`(`): "
"[%00-00 `~][%00-00* One][")
TOPIC_TEXT(
"%00-00 ()]&][s6; Destructor.&][s0;3 &][s4;:`:`:One`:`:Free`(`): [%00-00 void_][%00-00* "
"Attach][%00-00 (T_`*][%00-00*@3 data][%00-00 )]&][s6; Assigns element to One. (Note: "
"if there is content in One while calling this function, it is destroyed).&][s1; [%00-00*C@3 "
"data]-|Pointer to the new element created using operator new.&][s0;3 &][s4;:`:`:One`:`:Detach`(`)pick`_: "
"[%00-00 T_`*][%00-00* Detach][%")
TOPIC_TEXT(
"00-00 ()_pick`_]&][s6; Removes content giving up ownership. Client is responsible "
"for deletion of content.&][s1; [*/ Return value]-|Pointer to the content allocated "
"on the heap.&][s0;3 &][s4;:`:`:One`:`:operator`-`(`)pick`_: [%00-00 T_`*][%00-00* "
"operator`-][%00-00 ()_pick`_]&][s6; Same as Detach() (operator version).&][s1; [*/ "
"Return value]-|Pointer to the content allocated on the heap.&][s0;3 &]")
TOPIC_TEXT(
"[s4;:`:`:One`:`:Clear`(`): [%00-00 void_][%00-00* Clear][%00-00 ()]&][s6; Removes "
"and destroys content.&][s0;3 &][s4;:`:`:One`:`:operator`=`(T`*`): [%00-00 void_][%00-00* "
"operator`=][%00-00 (T_`*][%00-00*@3 data][%00-00 )]&][s6; Same as Attach(data) (operator "
"version).&][s1; [%00-00*C@3 data]-|Pointer to the new element created using operator "
"new.&][s0;3 &][s4;:`:`:One`:`:operator`=`(pick`_`:`:One")
TOPIC_TEXT(
"`<T`>`&`): [%00-00 void_][%00-00* operator`=][%00-00 (pick`__One<T>`&_][%00-00*@3 "
"d][%00-00 )]&][s6; Pick operator. Transfers source One in low constant time, but "
"destroys it by picking.&][s1; [%00-00*C@3 d]-|Source One.&][s0;3 &][s4;:`:`:One`:`:operator`-`>`(`)const: "
"[%00-00 const_T_`*][%00-00* operator`->][%00-00 ()_const]&][s6; Constant content "
"access operator. Illegal if there is no content.&]")
TOPIC_TEXT(
"[s1; [*/ Return value]-|Returns constant pointer to content.&][s0;3 &][s4;:`:`:One`:`:operator`-`>`(`): "
"[%00-00 T_`*][%00-00* operator`->][%00-00 ()]&][s6; Content access operator. Illegal "
"if there is no content.&][s1; [*/ Return value]-|Returns pointer to content.&][s0;3 "
"&][s4;:`:`:One`:`:operator`~`(`)const: [%00-00 const_T_`*][%00-00* operator`~][%00-00 "
"()_const]&][s6; Constant content pointer ")
TOPIC_TEXT(
"access.&][s1; [*/ Return value]-|Returns constant pointer to content or NULL when "
"there is no content.&][s0;3 &][s4;:`:`:One`:`:operator`~`(`): [%00-00 T_`*][%00-00* "
"operator`~][%00-00 ()]&][s6; Content pointer access.&][s1; [*/ Return value]-|Returns "
"pointer to content or NULL when there is no content.&][s0;3 &][s4;:`:`:One`:`:operator`*`(`)const: "
"[%00-00 const_T`&_][%00-00* operator`*][%00-00 ()")
TOPIC_TEXT(
"_const]&][s6; Content constant reference access. Illegal if there is no content.&][s1; "
"[*/ Return value]-|Constant reference to content.&][s0;3 &][s4;:`:`:One`:`:operator`*`(`): "
"[%00-00 T`&_][%00-00* operator`*][%00-00 ()]&][s6; Content reference access. Illegal "
"if there is no content.&][s1; [*/ Return value]-|Reference to content.&][s4; [%00-00* "
"template <class ][%00-00*@4 TT][%00-00* >][%00-00*@")
TOPIC_TEXT(
"4  TT][%00-00 `&_][%00-00* Create][%00-00 ()]&][s6; Creates content in One. (Note: "
"if there is content in One while calling this function, it is destroyed).&][s1; [*C@4 "
"TT]-|Type of content. Must be equal to T or derived from it.&][s1; [*/ Return value]-|Reference "
"to actual content (of type TT).&][s0;3 &][s4;:`:`:One`:`:IsPicked`(`)const: [%00-00 "
"bool_][%00-00* IsPicked][%00-00 ()_const]&][s1; [*/")
TOPIC_TEXT(
" Return value]-|true if One is picked.&][s0;3 &][s4;:`:`:One`:`:IsEmpty`(`)const: "
"[%00-00 bool_][%00-00* IsEmpty][%00-00 ()_const]&][s1; [*/ Return value]-|true if "
"there is no content.&][s0;3 &][s4;:`:`:One`:`:operator bool`(`)const: [%00-00* operator_bool][%00-00 "
"()_const]&][s1; [*/ Return value]-|true if there is content.&][s0;3 ]")
