TITLE("Pick Behaviour Explained")
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
"[s2; Pick Behaviour Ex")
TOPIC_TEXT(
"plained&][s0; Pick behaviour is really the most `\"weird`\" part of U`+`+ and one "
"which is really good to understand.&][s0; &][s0; In STL, when you assign a vector "
"variable, the deep copy is performed, i.e. the whole (potentially very complex) object "
"is duplicated. Similarly, when you have a function returning a vector, when you (typically) "
"construct a local variable of the vector type within the fu")
TOPIC_TEXT(
"nction, fill it in and at the end return it as the function value, again a duplication "
"of the whole complex object has to take place under standard C`+`+ semantics. While "
"the former case (assignment) is questionable, it is clear that in many cases the "
"copy in the latter case is a complete waste of time as the local object is going "
"to be destroyed immediately afterwards. There are movements suggest")
TOPIC_TEXT(
"ing that a future update of the C`+`+ standard might take care of this by means "
"of a new `\"temporary reference`\" modifier, however current C`+`+ compilers don`'t "
"have any mechanism to cope with this problem.&][s0; &][s0; After careful consideration "
"we decided to go the hard way, making it perhaps a little more tricky for the programmer "
"but with the effect of vast time / space savings. The types wh")
TOPIC_TEXT(
"ich are called `\"pick`\" have different value transfer mechanism: the destructive "
"copy (also called the pick copy). Note that it doesn`'t have to do anything with "
"destructors as such: an object which is transferred using the pick copy formally "
"continues to exist (every object belongs somewhere, so the object will be actually "
"destroyed and its destructor called when it comes out of scope), but it is")
TOPIC_TEXT(
" sort of `\"castrated`\", i.e. its data can no longer be considered valid. In some "
"situations such objects define a set of methods which can be used even in this `\"castrated`\" "
"(i.e. `\"picked`\") state; these typically reinitialize the object to an initial "
"state similar to the one after (empty) construction. This is e.g. the case of Vector`::Clear "
"and a few more.&][s0; &][s0; Anyway, in some specific ")
TOPIC_TEXT(
"cases you might need to perform `\"deep copy`\" operations even with types that "
"have default `\"pick`\" semantics. That is why we have introduced `\"optional deep "
"copy types`\" `- you can get deep copies of such types (we call such types `\"pick "
"with deep copy option), but you have to use different operations: the `\"deep copy "
"constructor`\" T(const T`&, int) and the `\"deep copy operator`\" T`& <<`= (const ")
TOPIC_TEXT(
"T`&). All U`+`+ containers are `\"pick types with optional deep copy`\". Note that "
"that `\"int`\" parameter in copy constructor is actually not used and serves only "
"to distinguish that constructor.&][s0; &][s0; Important note about composition: If "
"you make a member variable of a pick type within another object, that another object "
"`\"inherits`\" the pick type characteristic; by means of standard C`+`+ i")
TOPIC_TEXT(
"nheritance mechanisms, its (standard) copy constructor and assignment operator do "
"the pick copy. If you want to implement optional deep copy semantics for your type "
"as well, you can do that by inheriting it like&][s0; &][s7; class MyObject : DeepCopyOption<MyObject>&][s0; "
"&][s0; (this template defines the <<`= operator) and implement for it the deep copy "
"constructor. Let`'s try it on a simple exam")
TOPIC_TEXT(
"ple:&][s0; &][s7; class StateInfo : DeepCopyOption<StateInfo> `{&][s7; public:&][s7; "
"    StateInfo() `{`}&][s7;     StateInfo(const StateInfo`& s, int deep)&][s7;    "
"     : state`_names(s.state`_names, deep)&][s7;         , populations(s.populations, "
"deep)&][s7;         , capitals(s.capitals, deep) `{`}&][s7; &][s7; public:&][s7; "
"    Vector<String> state`_names;&][s7;     Vector<double> population")
TOPIC_TEXT(
"s;&][s7;     Vector<String> capitals;&][s7; `};&][s0; &][s0; Then when you write "
"in another function, say,&][s0; &][s7; StateInfo FilterPopulation(const StateInfo`& "
"si, double max`_population)&][s7; `{&][s7;     StateInfo s(si, 0);&][s7;     for(int "
"i `= s.populations.GetCount(); `-`-i;)&][s7;         if(s.populations`[i`] > max`_population) "
"`{&][s7;             s.state`_names.Remove(i);&][s7;    ")
TOPIC_TEXT(
"         s.populations.Remove(i);&][s7;             s.capitals.Remove(i);&][s7; "
"        `}&][s7;     return s;&][s7; `}&][s0; &][s0; Note the two different types "
"of value transfer: in the beginning, a deep copy is performed (that is necessary "
"in order not to damage the constant argument). Afterwards the filtering is performed "
"in the for() loop and finally the filtered variable s is returned from t")
TOPIC_TEXT(
"he function using the pick copy construction (s will no longer be used, so its contents "
"can safely go to hell).&][s0; &][s0; Now what makes this so tricky: it is in partial "
"contrast with standards of `\"polite`\" behaviour in C`+`+, where you normally expect "
"the `= operator to just fill in a new variable, not to wipe out the one on the right "
"side. That is just what happens when you `= a pick object.")
TOPIC_TEXT(
" Therefore you must remember which of your objects are of pick type and every time "
"you assign them, copy`-construct them or pass them to functions, you must decide "
"whether to use the pick method or the deep method.&][s0; &][s0; In practice this "
"is not as hard as it may sound, because typically pick objects are large, complex "
"objects or objects specially optimized for performance, and when you opti")
TOPIC_TEXT(
"mize for performance, you usually care about what`'s going on during their manipulation "
"anyway. However, in certain cases you want to use a pick type object without having "
"to care about the picks: in such situations you can use the WithDeepCopy<> modifier, "
"which wraps a pick type object with deep copy option in such a way it starts to behave "
"as the ordinary objects with default deep copy semantics")
TOPIC_TEXT(
".]")
