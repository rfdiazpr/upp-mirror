TITLE("Ptr and Pte")
REF("::Pte::class")
REF("::Ptr::class")
REF("::Ptr::Ptr()")
REF("::Ptr::Ptr(T*)")
REF("::Ptr::Ptr(const::Ptr&)")
REF("::Ptr::operator->()const")
REF("::Ptr::operator~()const")
REF("::Ptr::operator T*()const")
REF("::Ptr::operator=(T*)")
REF("::Ptr::operator=(const::Ptr&)")
REF("::Ptr::operator==(const::Ptr&,const T*)")
REF("::Ptr::operator==(const T*,const::Ptr&)")
REF("::Ptr::operator==(const::Ptr&,const::Ptr&)")
REF("::Ptr::operator==(const::Ptr&,T*)")
REF("::Ptr::operator==(T*,const::Ptr&)")
REF("::Ptr::operator!=(const::Ptr&,const T*)")
REF("::Ptr::operator!=(const T*,const::Ptr&)")
REF("::Ptr::operator!=(const::Ptr&,const::Ptr&)")
REF("::Ptr::ToString()const")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2; Ptr and Pte&][s5; Ptr "
"and Pte class templates provide a smart pointer system that cleared (assigned NULL) "
"when pointed object is destructed. That makes it useful in situations where life`-time "
"of object cannot be precisely determined.&][s5; While P")
TOPIC_TEXT(
"tr class template provides smart pointers, Pte is the class which adds necessary "
"functionality to pointed objects. Ptr could only point to objects of Pte`-derived "
"type. So to make a class `\"Ptr`-able`\" you should add Pte as one of class bases, "
"with the derived class as its parameter, e.g.:&][s5; struct Foo : Pte<Foo> `{....&][s5; "
"&][s10;:`:`:Pte`:`:class: template_<class_[*@4 T]>&][s10;* [* class_")
TOPIC_TEXT(
"]Pte&][s0;3 &][s0;* &][s6; This template class implements the functionality needed "
"in the pointed object.  &][s12; [*C@4 T]-|Type of pointed object.&][s9;^topic`:`/`/Core`/src`/PtePtr`$en`-us`#`:`:PteBase`:`:class "
"PteBase^ [/^^ Derived from][^^  ]PteBase&][s5; &][s5; &][s10;:`:`:Ptr`:`:class: template_<class_[*@4 "
"T]>&][s10;* [* class_]Ptr&][s0;3 &][s0;* &][s6; Type specific pointer.&][s12; [*C@4 "
"T")
TOPIC_TEXT(
"]-|Type of pointed object. T class must be derived from Pte<T>.&][s9;^topic`:`/`/Core`/src`/PtePtr`$en`-us`#`:`:PtrBase`:`:PtrBase`(`)^ "
"[/^^ Derived from][^^  ]PtrBase&][s0;3 &][s4;:`:`:Ptr`:`:Ptr`(`): [* Ptr]()&][s6; "
"Default constructor.&][s0;3 &][s4;:`:`:Ptr`:`:Ptr`(T`*`): [* Ptr](T_`*[*@3 ptr])&][s6; "
"Constructs Ptr pointing to specified object.&][s1; [*C@3 ptr]-|C`+`+ pointer to pointed "
"object.")
TOPIC_TEXT(
"&][s0;3 &][s4;:`:`:Ptr`:`:Ptr`(const`:`:Ptr`&`): [* Ptr](const_[* Ptr]`&_[*@3 ptr])&][s6; "
"Constructs Ptr pointing to the same object as other Ptr.&][s1; [*C@3 ptr]-|Other "
"Ptr.&][s0;3 &][s4;:`:`:Ptr`:`:operator`-`>`(`)const: T_`*[* operator`->]()_const&][s1; "
"[*/ Return value]-|C`+`+ pointer to pointed object or NULL if Ptr does not point "
"to any object.&][s0;3 &][s4;:`:`:Ptr`:`:operator`~`(`)const: ")
TOPIC_TEXT(
"T_`*[* operator`~]()_const&][s1; [*/ Return value]-|C`+`+ pointer to pointed object "
"or NULL if Ptr does not point to any object.&][s0;3 &][s4;:`:`:Ptr`:`:operator T`*`(`)const: "
"[* operator_T`*]()_const&][s1; [*/ Return value]-|C`+`+ pointer to pointed object "
"or NULL if Ptr does not point to any object.&][s0;3 &][s4;:`:`:Ptr`:`:operator`=`(T`*`): "
"Ptr`&_[* operator`=](T_`*[*@3 ptr])&][s6; Assigns ne")
TOPIC_TEXT(
"w pointer.&][s1; [*C@3 ptr]-|Pointer.&][s1; [*/ Return value]-|`*this.&][s0;3 &][s4;:`:`:Ptr`:`:operator`=`(const`:`:Ptr`&`): "
"Ptr`&_[* operator`=](const_Ptr`&_[*@3 ptr])&][s6; Assigns other Ptr.&][s1; [*C@3 "
"ptr]-|&][s1; [*/ Return value]-|&][s0;3 &][s4;:`:`:Ptr`:`:operator`=`=`(const`:`:Ptr`&`,const "
"T`*`): friend_bool_[* operator`=`=](const_Ptr`&_[*@3 a], const_T_`*[*@3 b])&][s4;:`:`:Ptr`:`:operat")
TOPIC_TEXT(
"or`=`=`(const T`*`,const`:`:Ptr`&`): friend_bool_[* operator`=`=](const_T_`*[*@3 "
"a], const_Ptr`&_[*@3 b])&][s4;:`:`:Ptr`:`:operator`=`=`(const`:`:Ptr`&`,const`:`:Ptr`&`): "
"friend_bool_[* operator`=`=](const_Ptr`&_[*@3 a], const_Ptr`&_[*@3 b])&][s4;:`:`:Ptr`:`:operator`=`=`(const`:`:Ptr`&`,T`*`): "
"friend_bool_[* operator`=`=](const_Ptr`&_[*@3 a], T_`*[*@3 b])&][s4;:`:`:Ptr`:`:operator`=`=`(T`*`,const")
TOPIC_TEXT(
"`:`:Ptr`&`): friend_bool_[* operator`=`=](T_`*[*@3 a], const_Ptr`&_[*@3 b])&][s4;:`:`:Ptr`:`:operator`!`=`(const`:`:Ptr`&`,const "
"T`*`): friend_bool_[* operator!`=](const_Ptr`&_[*@3 a], const_T_`*[*@3 b])&][s4;:`:`:Ptr`:`:operator`!`=`(const "
"T`*`,const`:`:Ptr`&`): friend_bool_[* operator!`=](const_T_`*[*@3 a], const_Ptr`&_[*@3 "
"b])&][s4;:`:`:Ptr`:`:operator`!`=`(const`:`:Ptr`&`,const`:`:Ptr`&`): fri")
TOPIC_TEXT(
"end_bool_[* operator!`=](const_Ptr`&_[*@3 a], const_Ptr`&_[*@3 b])&][s6; Comparison "
"operators.&][s0;3 &][s4;:`:`:Ptr`:`:ToString`(`)const: String_[* ToString]()_const&][s6; "
"Converts all information to string for diagnostic purposes&][s1; [*/ Return value]-|Text.&][s0;3 "
"]")
