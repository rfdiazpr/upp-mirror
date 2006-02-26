TITLE("Data")
REF("::Data::class")
REF("::Data::~Data()")
REF("::Data::SetData(const::Value&)")
REF("::Data::GetData()const")
REF("::Data::Accept()")
REF("::Data::Reject()")
REF("::Data::Serialize(::Stream&)")
REF("::Data::SetModify()")
REF("::Data::ClearModify()")
REF("::Data::IsModified()const")
REF("::Data::operator~()const")
REF("::Data::operator<<=(const::Value&)")
REF("::Data::IsNullInstance()const")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2; Data&][s10;:`:`:Data`:`:class:* "
"[* class_]Data&][s5; This class represents object that holds single Value and provides "
"interface for setting and querying this value and modification status.&][s3; Members&][s0;3 "
"&][s4;:`:`:Data`:`:`~Data`(`): virtua")
TOPIC_TEXT(
"l `~[* Data]()&][s6; Virtual destructor.&][s0;3 &][s4;:`:`:Data`:`:SetData`(const`:`:Value`&`): "
"virtual void_[* SetData](const_Value`&_[*@3 data])&][s6; Sets the new value to the "
"object. Default implementation is void.&][s1; [*C@3 data]-|New value.&][s0;3 &][s4;:`:`:Data`:`:GetData`(`)const: "
"virtual Value_[* GetData]()_const&][s6; Gets current value of the object. Default "
"implementation returns Va")
TOPIC_TEXT(
"lue() `- void value.&][s1; [*/ Return value]-|Value of object.&][s0;3 &][s4;:`:`:Data`:`:Accept`(`): "
"virtual bool_[* Accept]()&][s6; If object performs more complicated editing of stored "
"value, this method should check whether current edit state of object is regular. "
"If it is, it should finish editing and return [* true]. Otherwise it should return "
"[* false] to indicate that current value is not v")
TOPIC_TEXT(
"alid. Default implementation simply returns true.&][s1; [*/ Return value]-|[* true] "
"if edited value is accepted.&][s0;3 &][s4;:`:`:Data`:`:Reject`(`): virtual void_[* "
"Reject]()&][s6; If object supports rejecting of edit changes, this method should "
"revert all changes since last SetData or Accept.&][s0;3 &][s4;:`:`:Data`:`:Serialize`(`:`:Stream`&`): "
"virtual void_[* Serialize](Stream`&_[*@3 s])&][s6;")
TOPIC_TEXT(
" Serializes value. Default implementation calls GetData/SetData and serializes value "
"using Value serialization.&][s1; [*C@3 s]-|Target/source Stream.&][s0;3 &][s4;:`:`:Data`:`:SetModify`(`): "
"virtual void_[* SetModify]()&][s6; Sets modification flag. Default implementation "
"is empty.&][s0;3 &][s4;:`:`:Data`:`:ClearModify`(`): virtual void_[* ClearModify]()&][s6; "
"Clears modification flag. Default imp")
TOPIC_TEXT(
"lementation is empty.&][s0;3 &][s4;:`:`:Data`:`:IsModified`(`)const: virtual bool_[* "
"IsModified]()_const&][s6; Queries modification flag. Default implementation returns "
"false.&][s1; [*/ Return value]-|Modification flag.&][s0;3 &][s4;:`:`:Data`:`:operator`~`(`)const: "
"Value_[* operator`~]()_const&][s1; [*/ Return value]-|Returns GetData().&][s0;3 &][s4;:`:`:Data`:`:operator`<`<`=`(const`:`:Value`&`)")
TOPIC_TEXT(
": const_Value`&_[* operator<<`=](const_Value`&_[*@3 v])&][s6; Performs SetData(v).&][s1; "
"[*C@3 v]-|Value to set.&][s1; [*/ Return value]-|Reference to input value.&][s4; "
"bool_[* IsNullInstance]()_const&][s0;3 &][s6;:`:`:Data`:`:IsNullInstance`(`)const: "
"This method allows common IsNull to work with any Data derived object.&][s1; [*/ "
"Return value]-|IsNull(GetData()). &][s0;3 ]")
