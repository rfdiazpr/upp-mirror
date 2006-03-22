TITLE("Layout tools")
REF("::InitLayout(::Ctrl&,L&)")
REF("::CtrlLayout(T&)")
REF("::CtrlLayout(T&,const char*)")
REF("::CtrlLayoutOK(T&,const char*)")
REF("::CtrlLayoutCancel(T&,const char*)")
REF("::CtrlLayoutOKCancel(T&,const char*)")
REF("::CtrlLayoutExit(T&,const char*)")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2; [%00-00 Layout functions]&][s0; "
"&][s4; [%00-00 template_<class_][%00-00*@4 L][%00-00 >]&][s4;:`:`:InitLayout`(`:`:Ctrl`&`,L`&`): "
"[%00-00 void_][%00-00* InitLayout][%00-00 (Ctrl`&_][%00-00*@3 ctrl][%00-00 , ][%00-00*@4 "
"L][%00-00 `&_][%00-00*@3 layou")
TOPIC_TEXT(
"t][%00-00 )]&][s6; Assigns layout parameters to member Ctrl variables of [@3 layout], "
"and adds them to the specified [*@3 ctrl].&][s1; [*C@4 L]-|Class (or structure) with "
"Ctrl variables. Ctrl variables must have public access.&][s1; [%00-00*C@3 ctrl]-|Parent "
"Ctrl.&][s1; [%00-00*C@3 layout]-|[*C@4 L] instance.&][s0; &][s4;:`:`:CtrlLayout`(T`&`): "
"[%00-00 template_<class_][%00-00*@4 T][%00-00 >]&][s4")
TOPIC_TEXT(
";:`:`:CtrlLayout`(T`&`): [%00-00 void_][%00-00* CtrlLayout][%00-00 (][%00-00*@4 "
"T][%00-00 `&_][%00-00*@3 ctrl][%00-00 )]&][s6; Assigns layout parameters to member "
"variables. Required layout must be one of base classes of T.&][s1; [*C@4 T]-|Parent "
"Ctrl type.&][s1; [%00-00*C@3 ctrl]-|Parent Ctrl.&][s0; &][s4;:`:`:CtrlLayout`(T`&`,const "
"char`*`): [%00-00 template_<class_][%00-00*@4 T][%00-00 >]&][s4;")
TOPIC_TEXT(
":`:`:CtrlLayout`(T`&`,const char`*`): [%00-00 void_][%00-00* CtrlLayout][%00-00 "
"(][%00-00*@4 T][%00-00 `&_][%00-00*@3 ctrl][%00-00 , const_char_`*][%00-00*@3 title][%00-00 "
")]&][s6; Calls CtrlLayout and then calls Title method of [*@3 ctrl ](assigning a "
"caption text to the TopWindow).&][s1; [*C@4 T]-|Parent Ctrl type.&][s1; [%00-00*C@3 "
"ctrl]-|Parent Ctrl.&][s1; [%00-00*C@3 title]-|Window title.&][s")
TOPIC_TEXT(
"0; &][s4;:`:`:CtrlLayoutOK`(T`&`,const char`*`): [%00-00 template_<class_][%00-00*@4 "
"T][%00-00 >]&][s4;:`:`:CtrlLayoutOK`(T`&`,const char`*`): [%00-00 void_][%00-00* "
"CtrlLayoutOK][%00-00 (][%00-00*@4 T][%00-00 `&_][%00-00*@3 ctrl][%00-00 , const_char_`*][%00-00*@3 "
"title][%00-00 )]&][s6; Calls CtrlLayout and then assigns Acceptor(IDOK) to the [/ "
"ok] member Ctrl variable and makes it the default but")
TOPIC_TEXT(
"ton (activated by Enter).&][s1; [*C@4 T]-|Parent Ctrl type.&][s1; [%00-00*C@3 ctrl]-|Parent "
"Ctrl.&][s1; [%00-00*C@3 title]-|Window title.&][s0; &][s4;:`:`:CtrlLayoutCancel`(T`&`,const "
"char`*`): [%00-00 template_<class_][%00-00*@4 T][%00-00 >]&][s4;:`:`:CtrlLayoutCancel`(T`&`,const "
"char`*`): [%00-00 void_][%00-00* CtrlLayoutCancel][%00-00 (][%00-00*@4 T][%00-00 "
"`&_][%00-00*@3 ctrl][%00-00 , const_c")
TOPIC_TEXT(
"har_`*][%00-00*@3 title][%00-00 )]&][s6; Calls CtrlLayout and then assigns Rejector(IDCANCEL) "
"to the [/ cancel] member Ctrl variable and makes it the default cancel button (activated "
"by Esc).&][s1; [*C@4 T]-|Parent Ctrl type.&][s1; [%00-00*C@3 ctrl]-|Parent Ctrl.&][s1; "
"[%00-00*C@3 title]-|Window title.&][s0; &][s4;:`:`:CtrlLayoutOKCancel`(T`&`,const "
"char`*`): [%00-00 template_<class_][%00-00*@4 T]")
TOPIC_TEXT(
"[%00-00 >]&][s4;:`:`:CtrlLayoutOKCancel`(T`&`,const char`*`): [%00-00 void_][%00-00* "
"CtrlLayoutOKCancel][%00-00 (][%00-00*@4 T][%00-00 `&_][%00-00*@3 ctrl][%00-00 , const_char_`*][%00-00*@3 "
"title][%00-00 )]&][s6; Call CtrlLayoutOK and then assigns Rejector(IDCANCEL) to the "
"[/ cancel] member Ctrl variable and makes it the default cancel button (activated "
"by Esc).&][s1; [*C@4 T]-|Parent Ctrl type.&]")
TOPIC_TEXT(
"[s1; [%00-00*C@3 ctrl]-|Parent Ctrl.&][s1; [%00-00*C@3 title]-|Window title.&][s0; "
"&][s4;:`:`:CtrlLayoutExit`(T`&`,const char`*`): [%00-00 template_<class_][%00-00*@4 "
"T][%00-00 >]&][s4;:`:`:CtrlLayoutExit`(T`&`,const char`*`): [%00-00 void_][%00-00* "
"CtrlLayoutExit][%00-00 (][%00-00*@4 T][%00-00 `&_][%00-00*@3 ctrl][%00-00 , const_char_`*][%00-00*@3 "
"title][%00-00 )]&][s6; Call CtrlLayoutOK and then")
TOPIC_TEXT(
" assigns Acceptor(IDEXIT) to the [/ exit] member Ctrl variable and makes it the "
"default button (activated by Enter).&][s1; [*C@4 T]-|Parent Ctrl type.&][s1; [%00-00*C@3 "
"ctrl]-|Parent Ctrl.&][s1; [%00-00*C@3 title]-|Window title.&][s0;3 ]")
