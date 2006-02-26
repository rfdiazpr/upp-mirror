TITLE("TheIDE macros")
TOPIC_TEXT(
"[2 $$0,0#00000000000000000000000000000000:Default][l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param][a83;*R6 "
"$$2,5#31310162474203024125188417583966:caption][b83;* $$3,5#07864147445237544204411237157677:title][i288;b167;a42;O9;C2 "
"$$4,6#40027414424643823182269349404212:item][b42;a42;2 $$5,5#45413000475342174754091244180557:text][l288;a17;2 "
"$$6,6#2752174848137824262002072514382")
TOPIC_TEXT(
"5:desc][l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code][b2503; $$8,0#65142375456100023862071332075487:separator][*@(0.0.255)2 "
"$$9,0#83433469410354161042741608181528:base][t4167;C $$10,0#37138531426314131251341829483380:class][l288;a17;*1 "
"$$11,11#70004532496200323422659154056402:requirement][i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam][b167;C2 "
"$$13,13#9243")
TOPIC_TEXT(
"0459443460461911108080531343:item1][i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2][*@2$(0.128.128) "
"$$15,15#34511555403152284025741354420178:NewsDate][l321;*C$7;2 $$16,16#03451589433145915344929335295360:result][l321;b83;a83;*C$7;2 "
"$$17,17#07531550463529505371228428965313:result`-line][l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title][{_}%EN-US "
"[s2; TheIDE ma")
TOPIC_TEXT(
"cros&][s0; TheIDE macros are written in [^topic`:`/`/Esc`/srcdoc`/Esc`$en`-us^ Esc "
"scripting language] and placed in .usc files. .usc files can be either part of regular "
"packages or are placed in Common or Local directory.&][s0; &][s0; Macro definition "
"in .usc file starts with header in form&][s0; &][s7;*/@4 [/@5 macro][*/@5  ]Macro`_menu[/@5 "
":]Macro`_item[*/@5  ]Key&][s0; &][s0; where [*/@4 Macro")
TOPIC_TEXT(
"`_menu] and [*/@4 Macro`_item] are text literals (with C syntax) and [*/@4 Key] "
"is key description similar to those shown in menus. [*/@4 Macro`_menu] with [@(0.0.255) "
":] and [*/@4 Key] can be omitted. Macro header is followed by [^topic`:`/`/Esc`/srcdoc`/Esc`$en`-us^ "
"Esc] body.&][s0; &][s0; Examples:&][s0; &][s7; macro `\"Navigation`\":`\"Find next "
"`\\`\"TopWindow`\\`\"`\" Ctrl`+T `{&][s7; -|.Find(`\"TopW")
TOPIC_TEXT(
"indow`\");&][s7; `}&][s7; &][s7; macro `\"Insert current date`\" `{&][s7; -|s `= "
"`[`];&][s7; -|tm `= GetSysTime();&][s7; -|s << to`_string(tm.year) << `\"`-`\" << "
"to`_string(tm.month) << `\"`-`\" << to`_string(tm.day);&][s7; -|.Insert(s);&][s7; "
"`}&][s0; &][s0; Formally, from Esc perspective, macro is a `\"method`\" of editor "
"`- all editor commands are written in `\"method`\" notation (starting with dot).&][s")
TOPIC_TEXT(
"0; &][s0; Most of editor methods are provided in two (`\"overloaded`\") forms `- "
"implicit that works with cursor and selection or explicit with given positions.&][s0; "
"&][s0; Set of editor methods:&][s0; &][ {{3667:6333h1;@(204) [s0; Method]:: [s0; "
"Description]::^@2 [s0;*C+75 GetLength()]:: [s0; Length of text.]:: [s0;*C+75 GetLineCount()]:: "
"[s0; Number of lines in text.]:: [s0;*C+75 GetLinePos([*/ l")
TOPIC_TEXT(
"ine])]:: [s0; Position (offset from the beginning of the file) of the first character "
"of [/ line].]:: [s0;*C+75 GetCursor()]:: [s0; Cursor offset.]:: [s0;*C+75 GetLine([*/ "
"position])]:: [s0; Line for given [/ position].]:: [s0;*C+75 GetColumn([*/ position])]:: "
"[s0; Position in line for given [/ position].]:: [s0;*C+75 GetSelBegin()]:: [s0; "
"Start of selection.]:: [s0;*C+75 GetSelCount()]:: [s0; Num")
TOPIC_TEXT(
"ber of characters in selection. If zero, GetSelBegin() is equal to GetCursor().]:: "
"[s0;*C+75 SetCursor([*/ position])]:: [s0; Sets cursor to given [/ position].]:: "
"[s0;*C+75 SetSelection([*/ position], [*/ count])]:: [s0; Sets selection starting "
"with [/ position] with [/ count] characters.]:: [s0;*C+75 ClearSelection()]:: [s0; "
"Cancels selection.]:: [s0;*C+75 Get([*/ position], [*/ count])]:: [s0;/")
TOPIC_TEXT(
" [/ Returns array of ]count[/  characters, starting at ]position.]:: [s0;*C+75 Get([*/ "
"position])]:: [s0; Same as Get(position, 1).]:: [s0;*C+75 Remove([*/ position], [*/ "
"count])]:: [s0;/ [/ Removes ]count[/  characters at ]position.]:: [s0;*C+75 Remove([*/ "
"count])]:: [s0; Same as Remove(GetCursor(), [/ count]) `- removes [/ count] characters "
"at cursor position.]:: [s0;*C+75 Remove()]:: [s0; Same ")
TOPIC_TEXT(
"as Remove(GetSelBegin(), GetSelCount()) `- removes selection.]:: [s0;*C+75 Insert([*/ "
"position], [*/ text])]:: [s0;/ [/ Inserts array of characters ]text[/  at the ]position.]:: "
"[s0;*C+75 Insert([*/ text])]:: [s0; Same as Insert(GetCursor(), [/ text]) `- inserts "
"[/ text] at cursor position.]:: [s0;*C+75 Find([*/ text], [*/ down], [*/ whole`_word], "
"[*/ ignore`_case], [*/ wildcards])]:: [s0; Finds [")
TOPIC_TEXT(
"/ text], using give options. Options can be omitted `- in that case [/ down] is "
"considered true and rest of options false. If text is found, function returns 1, "
"otherwise 0.]:: [s0;*C+75 Replace([*/ text], [*/ replace`_with], [*/ whole`_word], "
"[*/ ignore`_case], [*/ widcards])]:: [s0; Block replace, using given options. Options "
"ca be omitted `- in that case they are considered false. Returns numbe")
TOPIC_TEXT(
"r of strings replaced.]:: [s0;*C+75 MoveLeft(...)&][s0;*C+75 MoveRight(...)&][s0;*C+75 "
"MoveWordLeft(...)&][s0;*C+75 MoveWordRight(...)&][s0;*C+75 MoveUp(...)&][s0;*C+75 "
"MoveDown(...)&][s0;*C+75 MoveHome(...)&][s0;*C+75 MoveEnd(...)&][s0;*C+75 MovePageUp(...)&][s0;*C+75 "
"MovePageDown(...)&][s0;*C+75 MoveTextBegin(...)&][s0;*C+75 MoveTextEnd(...)&][s0;*C+75 "
"MoveLeft([*/ sel])&][s0;*C+75 MoveRight([*/")
TOPIC_TEXT(
" sel])&][s0;*C+75 MoveWordLeft([*/ sel])&][s0;*C+75 MoveWordRight([*/ sel])&][s0;*C+75 "
"MoveUp([*/ sel])&][s0;*C+75 MoveDown([*/ sel])&][s0;*C+75 MoveHome([*/ sel])&][s0;*C+75 "
"MoveEnd([*/ sel])&][s0;*C+75 MovePageUp([*/ sel])&][s0;*C+75 MovePageDown([*/ sel])&][s0;*C+75 "
"MoveTextBegin([*/ sel])&][s0;*C+75 MoveTextEnd(sel)]:: [s0; Moves cursor in given "
"direction. Variant with [/ sel] makes selection ")
TOPIC_TEXT(
"(like when corresponding movement while pressing Shift key) if [/ sel] is 1.]:: "
"[s0;*C+75 Input([*/ label], ....)]:: [s0; Simple input dialog. Provides as many input "
"text fields as is number of parameters specifying labels for these fields. If user "
"chooses Cancel, returns void, otherwise returns single string if there is just one "
"edit field or array of strings for multiple edit fields.]:: [s0;*C+7")
TOPIC_TEXT(
"5 Execute([*/ cmdline])]:: [s0; Executes cmdline, capturing the output to the console.]:: "
"[s0;*C+75 Launch([*/ cmdline])]:: [s0; Launches the application.]:: [s0;*C+75 ClearConsole()]:: "
"[s0; Clear the output console.]}}]")
