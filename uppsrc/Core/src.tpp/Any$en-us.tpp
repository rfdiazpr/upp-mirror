TITLE("Any")
REF("::Any::class")
REF("::Any::Any(pick_::Any&)")
REF("::Any::Any()")
REF("::Any::~Any()")
REF("::Any::Create()")
REF("::Any::Is()const")
REF("::Any::Get()")
REF("::Any::Get()const")
REF("::Any::Clear()")
REF("::Any::IsEmpty()const")
REF("::Any::IsPicked()const")
REF("::Any::operator=(pick_::Any&)")
TOPIC_TEXT(
"[2 $$0,0#00000000000000000000000000000000:Default][l288;i1120;a17;O9;~~~.1408;2 "
"$$1,0#10431211400427159095818037425705:param][a83;*R6 $$2,5#31310162474203024125188417583966:caption][b83;*4 "
"$$3,5#07864147445237544204411237157677:title][i288;O9;C2 $$4,6#40027414424643823182269349404212:item][b42;a42;2 "
"$$5,5#45413000475342174754091244180557:text][l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc][l321;t246;C@5;1 "
"$$7,7#20902679421464641399138805415013:code][b2503; $$8,0#65142375456100023862071332075487:separator][*@(0.0.255)2 "
"$$9,0#83433469410354161042741608181528:base][t4167;C $$10,0#37138531426314131251341829483380:class][l288;a17;*1 "
"$$11,11#70004532496200323422659154056402:requirement][i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam][b167;C2 "
"$$13,13#92430459443460461911108080531343:item1][i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2][*@2$(0.128.128) "
"$$15,15#34511555403152284025741354420178:NewsDate][l321;*C$7;2 $$16,16#03451589433145915344929335295360:result][l321;b83;a83;*C$7;2 "
"$$17,17#07531550463529505371228428965313:result`-line][l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title][ "
"$$19,0#53580023442335529039900623488521:gap][t4167;C2 $$20,20#70211524482531209251820423858195:class`-nested][b50;2 "
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2; Any&][s0;3 &][s10;:`:`:Any`:`:class:* "
"[%00-00* class_][%00-00 Any]&][s6; &][s5; Any is a special type of container capable "
"of containing none or single element of [/ any] type. It also provides methods for "
"querying the type stored and retrieving the content of specific type.&][s5; Any has "
"pick semantics.&][s3; Members&][s0;3 &][s4;:`:`:Any`:`:Any`(pick`_`:`:Any`&`): [%00-00* "
"Any][%00-00 (pick`__][%00-00* Any][%00-00 `&_][%00-00*@3 s][%00-00 )]&][s6; Pick "
"constructor. Transfers content of source Any while destroying its content by picking.&][s1; "
"[%00-00*C@3 s]-|Source Any.&][s0;3 &][s4;:`:`:Any`:`:Any`(`): [%00-00* Any][%00-00 "
"()]&][s6; Cons")
TOPIC_TEXT(
"tructs an empty Any.&][s0;3 &][s4;:`:`:Any`:`:`~Any`(`): [%00-00 `~][%00-00* Any][%00-00 "
"()]&][s6; Destructor.&][s0;3 &][s4;:`:`:Any`:`:Create`(`): [%00-00 template <class "
"][%00-00*@4 T][%00-00 >][%00-00*  ][%00-00*@4 T][%00-00 `&_][%00-00* Create][%00-00 "
"()]&][s6; Creates content of type T inside Any.&][s1; [*C@4 T]-|Type of content.&][s1; "
"[*/ Return value]-|Reference to the newly created content.&][s0;3 &][s4;:`:`:Any`:`:Is`(`)const: "
"template <class [*@4 T]>[*  ][%00-00 bool_][%00-00* Is][%00-00 ()_const]&][s6; Tests "
"whether Any contains content of type T.&][s1; [*C@4 T]-|Required type.&][s1; [*/ "
"Return value]-|true if there is content with type T in Any.&][s0;3 &][s4;:`:`:Any`:`:Get`(`): "
"template <class [*@4 T]>[*  ][%00-00*@4 T][%00-00 `&_][%00-00* Get][%00-00 ()]&][s6; "
"Returns reference to content. Is<T> must be true, otherwise this operation is illegal.&][s1; "
"[*C@4 T]-|Required type.&][s1; [*/ Return value]-|Reference to content.&][s0;3 &][s4;:`:`:Any`:`:Get`(`)const: "
"template <class [*@4 T]>[*  ][%00-00 const_][%00-00*@4 T][%00-00 `&_][%00-00* Get][%00-00 "
"()_const]&][s6; Returns constant reference to content. Is<T> must be true, otherwise "
"this operation is illegal.&][s1; [*C@4 T]-|Required type.&][s1; [*/ Return value]-|Reference "
"to content.&][s0;3 &][s4;:`:`:Any`:`:Clear`(`): [%00-00 void_][%00-00* Clear][%00-00 "
"()]&][s6; Removes (and destroys) content.&][s0;3 &][s4;:`:`:Any`:`:IsEmpty`(`)const: "
"[%00-00 bool_][%00-00* IsEmpty][%00-00 ()_const]&][s1; [*/ Return value]-|true if "
"there is no content.&][s0;3 &][s4;:`:`:Any`:`:IsPicked`(`)const: [%00-00 bool_][%00-00* "
"IsPicked][%00-00 ()_const]&][s1; [*/ Return value]-|true if Any is picked.&][s0;3 "
"&][s4;:`:`:Any`:`:operator`=`(pick`_`:`:Any`&`): [%00-00 void_][%00-00* operator`=][%00-00 "
"(pick`__Any`&_][%00-00*@3 s][%00-00 )]&][s6; Pick operator. Transfers content while "
"destroying source.&][s1; [%00-00*C@3 s]-|Source Any.&][s0;3 ]")
