topic "TickMark";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_}%EN-US 
[s0;*@7;4 &]
[ {{10000@(113.42.0) [s0; [*@7;4 TickMark]]}}&]
[s1;:TickMark`:`:class:%- [@(0.0.255)3 class][3 _][*3 TickMark]&]
[s2; Base class used by all the [*@(128.0.255) TickMark] derived classes.&]
[s2; The `"Tick`"s are the drawings used in [* axis ]or for [* Markers 
](Or anything you wan`'t)&]
[s2; &]
[s3;%- &]
[s0;i448;a25;kKO9;@(0.0.255)%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s4;%- &]
[s5;:TickMark`:`:UpdateTick`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* UpdateTick]()&]
[s2; This method is intended to contain all internal state calculation/update 
code when some internal data is changed.&]
[s2; [_ Ex:] if you change tick size, maybe some other internal parameters 
need to be updated to. This must be done inside UpdateTick() 
method.&]
[s3;%- &]
[s4;%- &]
[s5;:TickMark`:`:GetTickRect`(Point`,ElementPosition`,int`,int`,const int`)const:%- [@(0.0.255) v
irtual] [_^Rect^ Rect]_[* GetTickRect]([_^Point^ Point]_[*@3 xyOffset], 
ElementPosition_[*@3 side], [@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], 
[@(0.0.255) const]_[@(0.0.255) int]_[*@3 scale]) `= 0&]
[s2; [%-*@3 xyOffset :] offset applied to X and Y in order to put them 
in the correct coordinates&]
[s2; [%-*@3 side] : side of the Graph on which the tick is drawn&]
[s2; [%-*@3 x] : x coordinate&]
[s2; [%-*@3 y] : y coordinate&]
[s2; [%-*@3 scale] : scale applied to drawing and therefore applied 
to TickRect&]
[s2; Returns the Rect containing the Tick drawing.&]
[s2; This mehotd is useful when you need to determine if a point 
is inside the TickMarker.&]
[s2; It is also used to HIGHLIGHT the TickMark when ( point selection 
for example )&]
[s3; &]
[s4;%- &]
[s5;:TickMark`:`:SetBackGroundStyle`(const Value`):%- [@(0.0.255) void]_[* SetBackGroundS
tyle]([@(0.0.255) const]_[_^Value^ Value]_[*@3 s])&]
[s2; Sets the background style of the TickMark. [%-*@3 s ]can be anything 
that is accepted by [^topic`:`/`/CtrlCore`/srcdoc`/AboutChameleon`$en`-us^ Chameleo
n] ChPaint() method :&]
[s2;i150;O0; Color&]
[s2;i150;O0; Image (hot spots are used)&]
[s2;i150;O0; ...&]
[s3; &]
[s4;%- &]
[s5;:TickMark`:`:Contains`(Point`,Point`,ElementPosition`,int`,int`,const int`)const:%- [@(0.0.255) b
ool]_[* Contains]([_^Point^ Point]_[*@3 p], [_^Point^ Point]_[*@3 xyOffset], 
ElementPosition_[*@3 side], [@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], 
[@(0.0.255) const]_[@(0.0.255) int]_[*@3 scale]_`=_[@3 1])&]
[s2; [%-*@3 p] : position to test ( absolute position in whole GraphCtrl 
).&]
[s2; [%-*@3 offset] : offset to apply to x/y pos in order get absolute 
positions&]
[s2; [%-*@3 side]: side of the Graph on which the element is drawn&]
[s2; [%-*@3 x] : X position of TickMark in coordinates relative to GraphElement&]
[s2; [%-*@3 y] : Y position of TickMark in coordinates relative to GraphElement&]
[s2; [%-*@3 scale] : scale applied when drawing the TickMark. So it 
is taken in account to calculate result&]
[s2; &]
[s2; Returns TRUE if p is in the GraphElement.&]
[s2; This method is used to distribute events to the correct element 
according to mouse position.&]
[s2; &]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor detail]]}}&]
[s0; &]
[s0; &]
[s0; &]
[s0; &]
[s0;*@7;4 &]
[ {{10000@(113.42.0) [s0; [*@7;4 SmartTextTickMark]]}}&]
[s0;%- [@(0.0.255)3 class][3 _][*3 SmartTextTickMark][3 _:_][@(0.0.255)3 public][3 _][*@3;3 TickMark
]&]
[s2; &]
[s3;%- &]
[s0; ]]