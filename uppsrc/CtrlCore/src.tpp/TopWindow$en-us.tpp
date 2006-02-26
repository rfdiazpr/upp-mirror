TITLE("TopWindow")
REF("::TopWindow::class")
REF("::TopWindow::Close()")
REF("::TopWindow::WhenClose")
REF("::TopWindow::Backup()")
REF("::TopWindow::Restore()")
REF("::TopWindow::Break(int)")
REF("::TopWindow::AcceptBreak(int)")
REF("::TopWindow::RejectBreak(int)")
REF("::TopWindow::WorkAreaTrim()")
REF("::TopWindow::Breaker(int)")
REF("::TopWindow::Acceptor(int)")
REF("::TopWindow::Rejector(int)")
REF("::TopWindow::Breaker(::Ctrl&,int)")
REF("::TopWindow::Acceptor(::Ctrl&,int)")
REF("::TopWindow::Rejector(::Ctrl&,int)")
REF("::TopWindow::NoCenter()")
REF("::TopWindow::CenterOwner()")
REF("::TopWindow::CenterScreen()")
REF("::TopWindow::SetMinSize(::Size)")
REF("::TopWindow::Open(HWND)")
REF("::TopWindow::Style(::dword)")
REF("::TopWindow::GetStyle()const")
REF("::TopWindow::ExStyle(::dword)")
REF("::TopWindow::GetExStyle()const")
REF("::TopWindow::Open(::Ctrl*)")
REF("::TopWindow::Open()")
REF("::TopWindow::OpenMain()")
REF("::TopWindow::Run(bool)")
REF("::TopWindow::RunAppModal()")
REF("::TopWindow::Execute()")
REF("::TopWindow::Minimize(bool)")
REF("::TopWindow::Maximize(bool)")
REF("::TopWindow::Overlap(bool)")
REF("::TopWindow::IsMaximized()const")
REF("::TopWindow::IsMinimized()const")
REF("::TopWindow::IsOverlapped()const")
REF("::TopWindow::ActiveFocus(::Ctrl&)")
REF("::TopWindow::Title(const::WString&)")
REF("::TopWindow::Title(const char*)")
REF("::TopWindow::GetTitle()const")
REF("::TopWindow::Sizeable(bool)")
REF("::TopWindow::NoSizeable()")
REF("::TopWindow::IsSizeable()const")
REF("::TopWindow::Zoomable(bool)")
REF("::TopWindow::NoZoomable()")
REF("::TopWindow::IsZoomable()const")
REF("::TopWindow::Background(const::PaintRect&)")
REF("::TopWindow::GetBackground()const")
REF("::TopWindow::ToolWindow(bool)")
REF("::TopWindow::NoToolWindow()")
REF("::TopWindow::IsToolWindow()const")
REF("::TopWindow::NoAccessKeysDistribution()")
REF("::TopWindow::Icon(const::Image&)")
REF("::TopWindow::LargeIcon(const::Image&)")
REF("::TopWindow::Icon(const::Image&,const::Image&)")
REF("::TopWindow::SerializePlacement(::Stream&,bool)")
REF("::TopWindow::TopWindow()")
REF("::TopWindow::~TopWindow()")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2; TopWindow&][s5; &][s10;:`:`:TopWindow`:`:class:* "
"[%00-00* class_][%00-00 TopWindow]&][s6; &][s9;^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:class^ "
"[/^^ Derived from][^^  ][%00-00 Ctrl]&][s0; &][s0; TopWindow represents top`-level "
"independe")
TOPIC_TEXT(
"nt window of host OS, usually decorated with caption, close/zoom/restore buttons "
"etc.&][s0; &][s0; Important fact to note is that window`-rectangle (as defined by "
"Ctrl`::SetRect and Ctrl`::GetRect methods) does not include any host GUI specific "
"decorations (caption, thick border for resizing), just the area that is `\"controlled`\" "
"by U`+`+.&][s0; &][s0; When TopWindow is opened, it is by default ce")
TOPIC_TEXT(
"ntered w.r.t. to its owner (if there is any). This behaviour can be altered by NoCenter "
"or CenterScreen methods.&][s0; &][s0; &][s4;:`:`:TopWindow`:`:Close`(`): [%00-00 "
"virtual void_][%00-00* Close][%00-00 ()]&][s6; This virtual method is implemented "
"in TopWindow so that it breaks modal loop if TopWindow runs it. If there is Breaker "
"with IDCANCEL defined, performs RejectBreak(IDCANCEL). Otherwise,")
TOPIC_TEXT(
" if there is Breaker with IDYES (as assigned to exit Ctrl by CtrlLayoutExit), performs "
"AcceptBreak(IDYES). If none if that is true, AcceptBreak(IDOK) is performed.&][s6; "
"If ToWindow does not run modal loop, it is closed.&][s0; &][s4;:`:`:TopWindow`:`:WhenClose:* "
"[%00-00* Callback_][%00-00 WhenClose]&][s6; This callback is by default invoked when "
"Close button of host GUI window is pushed. Default a")
TOPIC_TEXT(
"ssigned action is to perform Close virtual method.&][s0; &][s4;:`:`:TopWindow`:`:Backup`(`): "
"[%00-00 void_][%00-00* Backup][%00-00 ()]&][s6; Serializes the content of all contained "
"Ctrls (using [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:Serialize`(`:`:Stream`&`)^ "
"Serialize] virtual method) into internal storage of TopWindow. Useful when implementing "
"Cancel buttons.&][s0; &][s4;:`:`:TopW")
TOPIC_TEXT(
"indow`:`:Restore`(`): [%00-00 void_][%00-00* Restore][%00-00 ()]&][s6; Restores "
"content of all contained Ctrls from internal storage. If internal storage is empty, "
"does nothing.&][s0; &][s4;:`:`:TopWindow`:`:Break`(int`): [%00-00 void_][%00-00* "
"Break][%00-00 (int_][%00-00*@3 ID][%00-00 _`=_IDEXIT)]&][s6; If TopWindow is running "
"its loop (see [^topic`:`/`/CtrlCore`/src`/TopWindow`$en`-us`#`:`:TopWi")
TOPIC_TEXT(
"ndow`:`:Run`(bool`)^ Run] or [^topic`:`/`/CtrlCore`/src`/TopWindow`$en`-us`#`:`:TopWindow`:`:RunAppModal`(`)^ "
"RunAppModal] methods), breaks the loop with specified exit code.&][s1; [%00-00*C@3 "
"ID]-|Exit code.&][s0; &][s4;:`:`:TopWindow`:`:AcceptBreak`(int`): [%00-00 bool_][%00-00* "
"AcceptBreak][%00-00 (int_][%00-00*@3 ID][%00-00 )]&][s6; Tries to accept all contained "
"Ctrls by calling Accept for all")
TOPIC_TEXT(
" child Ctrls, until some returns false `- in such case this method returns false. "
"If all child Ctrls return true, calls Break with specified ID and returns true.&][s1; "
"[%00-00*C@3 ID]-|Exit code.&][s1; [*/ Return value]-|True if all child Ctrls were "
"accepted and Break(ID) was invoked.&][s0; &][s4;:`:`:TopWindow`:`:RejectBreak`(int`): "
"[%00-00 void_][%00-00* RejectBreak][%00-00 (int_][%00-00*@3 ID][")
TOPIC_TEXT(
"%00-00 )]&][s6; Rejects all co&][s1; [%00-00*C@3 ID]-|&][s0; &][s4;:`:`:TopWindow`:`:WorkAreaTrim`(`): "
"[%00-00 void_][%00-00* WorkAreaTrim][%00-00 ()]&][s6; Adjusts window position so "
"that it does not exceeds desktop workspace. Exceeding parts are trimmed (decreasing "
"window size).&][s0; &][s4;:`:`:TopWindow`:`:Breaker`(int`): [%00-00 Callback_][%00-00* "
"Breaker][%00-00 (int_][%00-00*@3 ID][%00-00 _")
TOPIC_TEXT(
"`=_IDEXIT)]&][s6; Returns Callback that, when invoked, invokes [* Break] with the "
"specified exit code.&][s1; [%00-00*C@3 ID]-|Message loop exit code.&][s1; [*/ Return "
"value]-|Callback.&][s0; &][s4;:`:`:TopWindow`:`:Acceptor`(int`): [%00-00 Callback_][%00-00* "
"Acceptor][%00-00 (int_][%00-00*@3 ID][%00-00 )]&][s6; Returns Callback that, when "
"invoked, invokes [* AcceptBreak] with the specified exit co")
TOPIC_TEXT(
"de.&][s1; [%00-00*C@3 ID]-|Message loop exit code.&][s1; [*/ Return value]-|Callback.&][s0; "
"&][s4;:`:`:TopWindow`:`:Rejector`(int`): [%00-00 Callback_][%00-00* Rejector][%00-00 "
"(int_][%00-00*@3 ID][%00-00 )]&][s6; Returns Callback that, when invoked, invokes "
"[* RejectBreak] with the specified exit code.&][s1; [%00-00*C@3 ID]-|Message loop "
"exit code.&][s1; [*/ Return value]-|Callback.&][s0; &][s4;:")
TOPIC_TEXT(
"`:`:TopWindow`:`:Breaker`(`:`:Ctrl`&`,int`): [%00-00 TopWindow`&_][%00-00* Breaker][%00-00 "
"(Ctrl`&_][%00-00*@3 m][%00-00 , int_][%00-00*@3 ID][%00-00 )]&][s6; Same as m <<`= "
"[* Breaker](ID) `- assigns [* Breaker] callback to WhenAction method of specified "
"Ctrl.&][s1; [%00-00*C@3 m]-|Ctrl.&][s1; [%00-00*C@3 ID]-|Message loop exit code.&][s1; "
"[*/ Return value]-|`*this.&][s0; &][s4;:`:`:TopWindow`:`:")
TOPIC_TEXT(
"Acceptor`(`:`:Ctrl`&`,int`): [%00-00 TopWindow`&_][%00-00* Acceptor][%00-00 (Ctrl`&_][%00-00*@3 "
"m][%00-00 , int_][%00-00*@3 ID][%00-00 )]&][s6; Same as m <<`= [* Acceptor](ID) `- "
"assigns [* Acceptor] callback to WhenAction method of specified Ctrl.&][s1; [%00-00*C@3 "
"m]-|Ctrl.&][s1; [%00-00*C@3 ID]-|Message loop exit code.&][s1; [*/ Return value]-|`*this.&][s0; "
"&][s4;:`:`:TopWindow`:`:Rejector`(`:`")
TOPIC_TEXT(
":Ctrl`&`,int`): [%00-00 TopWindow`&_][%00-00* Rejector][%00-00 (Ctrl`&_][%00-00*@3 "
"m][%00-00 , int_][%00-00*@3 ID][%00-00 )]&][s6; Same as m <<`= [* Rejector](ID) `- "
"assigns [* Rejector] callback to WhenAction method of specified Ctrl.&][s1; [%00-00*C@3 "
"m]-|Ctrl.&][s1; [%00-00*C@3 ID]-|Message loop exit code.&][s1; [*/ Return value]-|`*this.&][s0; "
"&][s4;:`:`:TopWindow`:`:NoCenter`(`): [%00-00 TopW")
TOPIC_TEXT(
"indow`&_][%00-00* NoCenter][%00-00 ()]&][s6; Sets TopWindow not to be centered when "
"opening.&][s1; [*/ Return value]-|`*this.&][s0; &][s4;:`:`:TopWindow`:`:CenterOwner`(`): "
"[%00-00 TopWindow`&_][%00-00* CenterOwner][%00-00 ()]&][s6; Sets TopWindow to be "
"centered w.r.t. its owner. This is default.&][s1; [*/ Return value]-|`*this.&][s0; "
"&][s4;:`:`:TopWindow`:`:CenterScreen`(`): [%00-00 TopWindow`&_]")
TOPIC_TEXT(
"[%00-00* CenterScreen][%00-00 ()]&][s6; Sets TopWindow to be desktop centered.&][s1; "
"[*/ Return value]-|`*this.&][s0; &][s4;:`:`:TopWindow`:`:SetMinSize`(`:`:Size`): "
"[%00-00 void_][%00-00* SetMinSize][%00-00 (Size_][%00-00*@3 sz][%00-00 )]&][s6; Sets "
"the minimal size for window. This size is then returned by GetMinSize virtual method. "
"If window is resizable, it cannot be made smaller than this siz")
TOPIC_TEXT(
"e.&][s1; [%00-00*C@3 sz]-|Minimal size of window.&][s0; &][s4;:`:`:TopWindow`:`:Open`(HWND`): "
"[%00-00 void_][%00-00* Open][%00-00 (HWND_][%00-00*@3 ownerhwnd][%00-00 )]&][s6; "
"Opens window with owner specified as Win32 handle.&][s11; Win32 specific.&][s1; [%00-00*C@3 "
"ownerhwnd]-|Owner window handle.&][s0; &][s4;:`:`:TopWindow`:`:Style`(`:`:dword`): "
"[%00-00 TopWindow`&_][%00-00* Style][%00-00 (dword")
TOPIC_TEXT(
"_][%00-00*@3 `_style][%00-00 )]&][s6; Sets Win32 window style.&][s11; Win32 specific.&][s1; "
"[%00-00*C@3 `_style]-|Style.&][s1; [*/ Return value]-|`*this.&][s0; &][s4;:`:`:TopWindow`:`:GetStyle`(`)const: "
"[%00-00 dword_][%00-00* GetStyle][%00-00 ()_const]&][s6; Sets Win32 window extended "
"style.&][s11; Win32 specific.&][s1; [*/ Return value]-|`*this.&][s0; &][s4;:`:`:TopWindow`:`:ExStyle`(`:`:dword`)")
TOPIC_TEXT(
": [%00-00 TopWindow`&_][%00-00* ExStyle][%00-00 (dword_][%00-00*@3 `_exstyle][%00-00 "
")]&][s6; &][s11; Win32 specific.&][s1; [%00-00*C@3 `_exstyle]-|&][s1; [*/ Return "
"value]-|&][s0; &][s4;:`:`:TopWindow`:`:GetExStyle`(`)const: [%00-00 dword_][%00-00* "
"GetExStyle][%00-00 ()_const]&][s6; &][s1; [*/ Return value]-|&][s0; &][s4;:`:`:TopWindow`:`:Open`(`:`:Ctrl`*`): "
"[%00-00 void_][%00-00* Open][%00-00 (C")
TOPIC_TEXT(
"trl_`*][%00-00*@3 owner][%00-00 )]&][s6; Opens TopWindow as window in host GUI with "
"specified owner.&][s1; [%00-00*C@3 owner]-|Owner of window. If NULL, window has no "
"owner.&][s0; &][s4;:`:`:TopWindow`:`:Open`(`): [%00-00 void_][%00-00* Open][%00-00 "
"()]&][s6; Opens TopWindow with owner equal to current active window if it is owned "
"by the process.&][s0; &][s4;:`:`:TopWindow`:`:OpenMain`(`): [%00-00")
TOPIC_TEXT(
" void_][%00-00* OpenMain][%00-00 ()]&][s6; Opens window without owner.&][s0; &][s4;:`:`:TopWindow`:`:Run`(bool`): "
"[%00-00 int_][%00-00* Run][%00-00 (bool_][%00-00*@3 appmodal][%00-00 _`=_false)]&][s6; "
"Execute modal loop for TopWindow. If TopWindow is not open, it calls Open before "
"starting loop, but it does not close it.&][s1; [%00-00*C@3 appmodal]-|If true, all "
"application window are disabled whi")
TOPIC_TEXT(
"le performing loop. If false, only TopWindow`'s relative windows (with the same "
"`\"super`\"`-owner) are disabled.&][s1; [*/ Return value]-|Result code of modal loop.&][s0; "
"&][s4;:`:`:TopWindow`:`:RunAppModal`(`): [%00-00 int_][%00-00* RunAppModal][%00-00 "
"()]&][s6; Same as Run(true).&][s1; [*/ Return value]-|Result code of modal loop.&][s0; "
"&][s4;:`:`:TopWindow`:`:Execute`(`): [%00-00 int_][%00-00* E")
TOPIC_TEXT(
"xecute][%00-00 ()]&][s6; Similar to Run, but closes TopWindow when loop exits.&][s1; "
"[*/ Return value]-|Result code of modal loop.&][s0; &][s4;:`:`:TopWindow`:`:Minimize`(bool`): "
"[%00-00 void_][%00-00* Minimize][%00-00 (bool_][%00-00*@3 effect][%00-00 _`=_false)]&][s6; "
"Minimize window.&][s1; [%00-00*C@3 effect]-|Do platform specific GUI effects.&][s0; "
"&][s4;:`:`:TopWindow`:`:Maximize`(bool`): [%00")
TOPIC_TEXT(
"-00 void_][%00-00* Maximize][%00-00 (bool_][%00-00*@3 effect][%00-00 _`=_false)]&][s6; "
"Maximize window.&][s1; [%00-00*C@3 effect]-|Do platform specific GUI effects.&][s0; "
"&][s4;:`:`:TopWindow`:`:Overlap`(bool`): [%00-00 void_][%00-00* Overlap][%00-00 (bool_][%00-00*@3 "
"effect][%00-00 _`=_false)]&][s6; Put window to normal overlapped state.&][s1; [%00-00*C@3 "
"effect]-|Do platform specific GUI effects")
TOPIC_TEXT(
".&][s0; &][s4;:`:`:TopWindow`:`:IsMaximized`(`)const: [%00-00 bool_][%00-00* IsMaximized][%00-00 "
"()_const]&][s1; [*/ Return value]-|True if TopWindow is maximized.&][s0; &][s4;:`:`:TopWindow`:`:IsMinimized`(`)const: "
"[%00-00 bool_][%00-00* IsMinimized][%00-00 ()_const]&][s1; [*/ Return value]-|True "
"if TopWindow is minimized.&][s0; &][s4;:`:`:TopWindow`:`:IsOverlapped`(`)const: [%00-00 "
"bool_][%00-00")
TOPIC_TEXT(
"* IsOverlapped][%00-00 ()_const]&][s1; [*/ Return value]-|True if TopWindow is in "
"normal (overlapped) state.&][s0; &][s4;:`:`:TopWindow`:`:ActiveFocus`(`:`:Ctrl`&`): "
"[%00-00 TopWindow`&_][%00-00* ActiveFocus][%00-00 (Ctrl`&_][%00-00*@3 ctrl][%00-00 "
")]&][s6; Sets the ActiveFocus Ctrl. This Ctrl gets focus when TopWindow is activated. "
"If there is no Ctrl with active focus, first Ctrl is activated. W")
TOPIC_TEXT(
"hen TopWindow is deactivated, Ctrl that has focus is stored as new ActiveFocus Ctrl "
"and gets focus once TopWindow is activated again (unless this method changes ActiveFocus "
"meanwhile). When this, method is called while TopWindow is active, it is equivalent "
"to ctrl.SetFocus().&][s1; [%00-00*C@3 ctrl]-|Ctrl to have &][s1; [*/ Return value]-|`*this.&][s0; "
"&][s4;:`:`:TopWindow`:`:Title`(const`:`:WStri")
TOPIC_TEXT(
"ng`&`): [%00-00 TopWindow`&_][%00-00* Title][%00-00 (const_WString`&_][%00-00*@3 "
"`_title][%00-00 )]&][s6; Sets the title of the window.&][s1; [%00-00*C@3 `_title]-|Title.&][s1; "
"[*/ Return value]-|`*this.&][s0; &][s4;:`:`:TopWindow`:`:Title`(const char`*`): [%00-00 "
"TopWindow`&_][%00-00* Title][%00-00 (const_char_`*][%00-00*@3 s][%00-00 )]&][s6; "
"Sets the title of the window.&][s1; [%00-00*C@3 s]-|Ti")
TOPIC_TEXT(
"tle.&][s1; [*/ Return value]-|`*this.&][s0; &][s4;:`:`:TopWindow`:`:GetTitle`(`)const: "
"[%00-00 const_WString`&_][%00-00* GetTitle][%00-00 ()_const]&][s1; [*/ Return value]-|Current "
"title of window.&][s0; &][s4;:`:`:TopWindow`:`:Sizeable`(bool`): [%00-00 TopWindow`&_][%00-00* "
"Sizeable][%00-00 (bool_][%00-00*@3 b][%00-00 _`=_true)]&][s6; Indicates whether TopWindow "
"should be resizeable by user.&][s1")
TOPIC_TEXT(
"; [%00-00*C@3 b]-|True to set.&][s1; [*/ Return value]-|`*this.&][s0; &][s4;:`:`:TopWindow`:`:NoSizeable`(`): "
"[%00-00 TopWindow`&_][%00-00* NoSizeable][%00-00 ()]&][s6; Same as Sizeable(false).&][s1; "
"[*/ Return value]-|`*this.&][s0; &][s4;:`:`:TopWindow`:`:IsSizeable`(`)const: [%00-00 "
"bool_][%00-00* IsSizeable][%00-00 ()_const]&][s1; [*/ Return value]-|Value&][s0; "
"&][s4;:`:`:TopWindow`:`:Zoomable`")
TOPIC_TEXT(
"(bool`): [%00-00 TopWindow`&_][%00-00* Zoomable][%00-00 (bool_][%00-00*@3 b][%00-00 "
"_`=_true)]&][s6; Sets TopWindow to be Zoomable `- to have maximize/minimize flags, "
"if supported by host platform.&][s1; [%00-00*C@3 b]-|True to set.&][s1; [*/ Return "
"value]-|`*this.&][s0; &][s4;:`:`:TopWindow`:`:NoZoomable`(`): [%00-00 TopWindow`&_][%00-00* "
"NoZoomable][%00-00 ()]&][s6; Same as Zoomable(false).&][s1")
TOPIC_TEXT(
"; [*/ Return value]-|`*this.&][s0; &][s4;:`:`:TopWindow`:`:IsZoomable`(`)const: "
"[%00-00 bool_][%00-00* IsZoomable][%00-00 ()_const]&][s1; [*/ Return value]-|Value "
"of Zoomable flag.&][s0; &][s4;:`:`:TopWindow`:`:Background`(const`:`:PaintRect`&`): "
"[%00-00 TopWindow`&_][%00-00* Background][%00-00 (const_PaintRect`&_][%00-00*@3 prect][%00-00 "
")]&][s6; Sets PaintRect that will be used to paint TopWindo")
TOPIC_TEXT(
"w background. Default PaintRect paints background with SColorFace.&][s1; [%00-00*C@3 "
"prect]-|PaintRect.&][s1; [*/ Return value]-|`*this.&][s0; &][s4;:`:`:TopWindow`:`:GetBackground`(`)const: "
"[%00-00 const_PaintRect`&_][%00-00* GetBackground][%00-00 ()_const]&][s6; Returns "
"active PaintRect used to paint TopWindow background.&][s1; [*/ Return value]-|PaintRect.&][s0; "
"&][s4;:`:`:TopWindow`:`:ToolWind")
TOPIC_TEXT(
"ow`(bool`): [%00-00 TopWindow`&_][%00-00* ToolWindow][%00-00 (bool_][%00-00*@3 b][%00-00 "
"_`=_true)]&][s6; Makes TopWindow appear with thin caption, of supported by host platform. "
"Must be set before opening.&][s1; [%00-00*C@3 b]-|True to set the flag.&][s1; [*/ "
"Return value]-|`*this.&][s0; &][s4;:`:`:TopWindow`:`:NoToolWindow`(`): [%00-00 TopWindow`&_][%00-00* "
"NoToolWindow][%00-00 ()]&][s6; Same as")
TOPIC_TEXT(
" ToolWindow(false).&][s1; [*/ Return value]-|`*this.&][s0; &][s4;:`:`:TopWindow`:`:IsToolWindow`(`)const: "
"[%00-00 bool_][%00-00* IsToolWindow][%00-00 ()_const]&][s1; [*/ Return value]-|Value "
"of ToolWindow flag.&][s0; &][s4;:`:`:TopWindow`:`:NoAccessKeysDistribution`(`): [%00-00 "
"TopWindow`&_][%00-00* NoAccessKeysDistribution][%00-00 ()]&][s6; Disables distribution "
"of access keys. Normally, TopWindo")
TOPIC_TEXT(
"w tries to assign access`-keys (also known as `\"hot`-keys`\") to all of its child "
"windows before opening. This method disables this behaviour.&][s1; [*/ Return value]-|`*this.&][s0; "
"&][s4;:`:`:TopWindow`:`:Icon`(const`:`:Image`&`): [%00-00 TopWindow`&_][%00-00* Icon][%00-00 "
"(const_Image`&_][%00-00*@3 m][%00-00 )]&][s6; Defines icon for TopWindow that is "
"usually displayed in window caption (platform")
TOPIC_TEXT(
" specif).&][s1; [%00-00*C@3 m]-|Icon.&][s1; [*/ Return value]-|`*this.&][s0; &][s4;:`:`:TopWindow`:`:LargeIcon`(const`:`:Image`&`): "
"[%00-00 TopWindow`&_][%00-00* LargeIcon][%00-00 (const_Image`&_][%00-00*@3 m][%00-00 "
")]&][s6; Defines `\"large`\" icon for TopWindow. It can be displayed e.g. in task "
"switcher and other places (platform specific).&][s1; [%00-00*C@3 m]-|Icon.&][s1; "
"[*/ Return value]-|`*t")
TOPIC_TEXT(
"his.&][s0; &][s4;:`:`:TopWindow`:`:Icon`(const`:`:Image`&`,const`:`:Image`&`): [%00-00 "
"TopWindow`&_][%00-00* Icon][%00-00 (const_Image`&_][%00-00*@3 smallicon][%00-00 , "
"const_Image`&_][%00-00*@3 largeicon][%00-00 )]&][s6; Defines both `\"small`\" and "
"`\"large`\" icons for TopWindow.&][s1; [%00-00*C@3 smallicon]-|Small icon.&][s1; "
"[%00-00*C@3 largeicon]-|Large icon.&][s1; [*/ Return value]-|`*this.&][s")
TOPIC_TEXT(
"0; &][s4;:`:`:TopWindow`:`:SerializePlacement`(`:`:Stream`&`,bool`): [%00-00 void_][%00-00* "
"SerializePlacement][%00-00 (Stream`&_][%00-00*@3 s][%00-00 , bool_][%00-00*@3 reminimize][%00-00 "
"_`=_false)]&][s6; Serializes current placement of TopWindow. It is platform specific, "
"however information can include screen position in `\"normal`\" state and current "
"status (maximized, minimized, normal).&][s1; ")
TOPIC_TEXT(
"[%00-00*C@3 s]-|Stream.&][s1; [%00-00*C@3 reminimize]-|If false and loading the "
"information from the stream, minimized state is not restored.&][s0; &][s4;:`:`:TopWindow`:`:TopWindow`(`): "
"[%00-00* TopWindow][%00-00 ()]&][s6; Constructor.&][s0; &][s4;:`:`:TopWindow`:`:`~TopWindow`(`): "
"[%00-00 `~][%00-00* TopWindow][%00-00 ()]&][s6; Destructor.&][s0; &][s0;3 ]")
