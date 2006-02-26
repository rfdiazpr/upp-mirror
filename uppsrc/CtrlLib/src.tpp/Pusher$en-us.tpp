TITLE("Pusher")
REF("::Pusher::class")
REF("::Pusher::Pusher()")
REF("::Pusher::KeyPush()")
REF("::Pusher::IsPush()")
REF("::Pusher::IsKeyPush()")
REF("::Pusher::FinishPush()")
REF("::Pusher::EndPush()")
REF("::Pusher::RefreshPush()")
REF("::Pusher::RefreshFocus()")
REF("::Pusher::PerformAction()")
REF("::Pusher::SetFont(::Font)")
REF("::Pusher::SetLabel(const char*)")
REF("::Pusher::PseudoPush()")
REF("::Pusher::accesskey")
REF("::Pusher::label")
REF("::Pusher::font")
REF("::Pusher::WhenPush")
REF("::Pusher::WhenRepeat")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2; Pusher&][s5; Pusher "
"is a base class for classes that perform simple action when clicked by mouse or by "
"some keyboard interface (usually access key). Pusher handles all mouse input and "
"checks whether keyboard events match Pusher`'s access key, howev")
TOPIC_TEXT(
"er keyboard input can be also processed by derived classes and projected into Pusher`'s "
"state by its methods (KeyPush, FinishPush). Accelerator key for Pusher is assigned "
"either through the label (using `'`&`' or `'`\\b`', see [^topic`:`/`/CtrlLib`/src`/LabelBase`$en`-us^ "
"LabelBase]).&][s5; When Pusher is pushed and mouse is then released while hovering "
"over pusher, PerformAction virtual method is ")
TOPIC_TEXT(
"invoked and its standard implementation invokes in turn WhenAction callback. Other "
"than that, WhenPush and WhenRepeat callbacks are available.&][s5; &][s0;3 &][s10;:`:`:Pusher`:`:class:* "
"[* class_]Pusher&][s6; &][s9;^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:class "
"Ctrl^ [/^^ Derived from][^^  ]Ctrl&][s0;3 &][s4;:`:`:Pusher`:`:Pusher`(`): [* Pusher]()&][s6; "
"Initializes Pusher. Font is sen")
TOPIC_TEXT(
"t to standard GUI font, accelerator key is cleared.&][s0;3 &][s4;:`:`:Pusher`:`:KeyPush`(`): "
"void_[* KeyPush]()&][s11; Protected.&][s6; This method puts Pusher into `\"pushed "
"by keyboard`\" state. It should be used as reaction to accelerator key by derived "
"class.&][s0;3 &][s4;:`:`:Pusher`:`:IsPush`(`): bool_[* IsPush]()&][s11; Protected.&][s1; "
"[*/ Return value]-|true when in pushed state.&][s0;3 &][")
TOPIC_TEXT(
"s4;:`:`:Pusher`:`:IsKeyPush`(`): bool_[* IsKeyPush]()&][s11; Protected.&][s1; [*/ "
"Return value]-|true when in pushed state initiated by keyboard.&][s0;3 &][s4;:`:`:Pusher`:`:FinishPush`(`): "
"bool_[* FinishPush]()&][s11; Protected.&][s1; [*/ Return value]-|Forces end of pushed "
"state and invokes PerformAction method (default implementation invokes standard WhenAction "
"callback).&][s0;3 &][s4;:`:`:Push")
TOPIC_TEXT(
"er`:`:EndPush`(`): void_[* EndPush]()&][s11; Protected.&][s6; Forces end of pushed "
"state.&][s0;3 &][s4;:`:`:Pusher`:`:RefreshPush`(`): virtual void_[* RefreshPush]()&][s6; "
"This method should refresh part of concrete derived Pusher class that is being graphically "
"changed as reaction to push state. Default implementation calls Refresh, however "
"some classes (like Option) overload this to minimize rep")
TOPIC_TEXT(
"aint area size.&][s0;3 &][s4;:`:`:Pusher`:`:RefreshFocus`(`): virtual void_[* RefreshFocus]()&][s6; "
"This method should refresh part of concrete derived Pusher class that is being graphically "
"changed as reaction to change of input focus. Default implementation calls Refresh, "
"however some classes (like Option) overload this to minimize repaint area size.&][s0;3 "
"&][s4;:`:`:Pusher`:`:PerformAction`(`)")
TOPIC_TEXT(
": virtual void_[* PerformAction]()&][s6; This method is called when `\"push`\" is "
"finished. Default implementation invokes WhenAction.&][s0;3 &][s4;:`:`:Pusher`:`:SetFont`(`:`:Font`): "
"Pusher`&_[* SetFont](Font_[*@3 fnt])&][s6; Sets the font that should be used in painting "
"the concrete derived Pusher class. Pusher itself does not use this font anywhere, "
"but most derived classes do.&][s1; [*C@3 fnt]-|")
TOPIC_TEXT(
"Font.&][s1; [*/ Return value]-|`*this for method chaining.&][s0;3 &][s4;:`:`:Pusher`:`:SetLabel`(const "
"char`*`): Pusher`&_[* SetLabel](const_char_`*[*@3 text])&][s6; Sets the text of pusher. "
"Pusher itself does not use this font anywhere, but most derived classes do. Text "
"can contain `'`&`' or `'`\\b`' characters to define access keys./&][s1; [*C@3 text]-|&][s1; "
"[*/ Return value]-|&][s0;3 &][s4;:`:`")
TOPIC_TEXT(
":Pusher`:`:PseudoPush`(`): void_[* PseudoPush]()&][s6; Invokes `\"push emulation`\". "
"Pusher will animate through states as if it would be pushed by the user, including "
"calls to PerformAction (WhenAction), WhenPush and WhenRepeat.&][s0;3 &][s4;:`:`:Pusher`:`:accesskey:* "
"[* byte_]accesskey&][s11; Protected.&][s6; Access key assigned to the Pusher.&][s0;3 "
"&][s4;:`:`:Pusher`:`:label:* [* String_]label&]")
TOPIC_TEXT(
"[s11; Protected.&][s6; Text assigned to the Pusher.&][s0;3 &][s4;:`:`:Pusher`:`:font:* "
"[* Font_]font&][s11; Protected.&][s6; Font assigned to the Pusher.&][s0;3 &][s4;:`:`:Pusher`:`:WhenPush:* "
"[* Callback_]WhenPush&][s6; This callback is invoked when pusher goes from `\"unpushed`\" "
"to `\"pushed`\" state.&][s0;3 &][s4;:`:`:Pusher`:`:WhenRepeat:* [* Callback_]WhenRepeat&][s6; "
"`\"Pusher autorepeat`\" `- th")
TOPIC_TEXT(
"is callback is periodically invoked when user holds mouse button over Pusher `- "
"it emulates keyboard autorepeat (e.g. arrow buttons in scrollbar).&][s0;3 ]")
