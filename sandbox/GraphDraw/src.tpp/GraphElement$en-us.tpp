topic "2 - GraphElement class";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Custom types]]}}&]
[s0;%- &]
[s0;%- &]
[s4;%- &]
[s5;:typedef:%- typedef_enum_[*3 FitToDataStrategy]&]
[s2; Used to indicate if refresh needs to be complete or partial 
(in order to be FAST).&]
[s2; This is usefull when accomplishing mouse move/resize and that 
the data is to big to get refreshed rapidly.&]
[s2; The values accepted are the following :&]
[s2;i150;O0; [*@(128.0.255) REFRESH`_FAST : ]only partial data is draw 
(to be faster)&]
[s2;i150;O0; [*@(128.0.255) REFRESH`_TOTAL : ]everything is redrawn&]
[s2;i150;O0; [*@(128.0.255) REFRESH`_KEEP`_DATA : ]data is not redrawn, 
it`'s buffered image is used instead&]
[s3;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 GraphElement]]}}&]
[s1;:GraphElement`:`:class:%- [@(0.0.255)3 class][3 _][*3 GraphElement]&]
[s2; Base class used to implement all Graph Elements.&]
[s2; It manages all the common details&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s0;%- &]
[s4;%- &]
[s5;:GraphElement`:`:AdjustToPlotRect`(const Rect`&`):%- [@(0.0.255) void]_[* AdjustToPlo
tRect]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 plotRect])&]
[s2;  [%-*@3 plotRect] : plot area Rect&]
[s2; Adjusts ElementFrame according to plotRect.&]
[s2; This method is called when all GraphElements have been added 
to the Graph, and finally, all elements need they`'re size to 
be adjusted.&]
[s2; NB: for internal use only&]
[s3; &]
[s4;%- &]
[s5;:GraphElement`:`:SetName`(const char`*`):%- [@(0.0.255) void]_[* SetName]([@(0.0.255) c
onst]_[@(0.0.255) char`*]_[*@3 name])&]
[s2;  [%-*@3 name] : name of element (the one that is used to identify 
the element in the elements list.&]
[s3; &]
[s4;%- &]
[s5;:GraphElement`:`:Contains`(Point`)const:%- [@(0.0.255) virtual] 
[@(0.0.255) bool]_[* Contains]([_^Point^ Point]_[*@3 p])_[@(0.0.255) const]&]
[s2;  [%-*@3 p] : position to test ( absolute position in whole GraphCtrl 
).&]
[s2; Returns TRUE if p is in the GraphElement. This is used to distribute 
events to the correct element according to mouse position.&]
[s3; &]
[s4;%- &]
[s5;:GraphElement`:`:SetFloatFrame`(Rect`):%- [@(0.0.255) void]_[* SetFloatFrame]([_^Rect^ R
ect]_[*@3 v])&]
[s2;  [%-*@3 v] .&]
[s3; &]
[s4;%- &]
[s5;:GraphElement`:`:PaintFloatElement`(Draw`&`,int`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* PaintFloatElement]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 dw], 
[@(0.0.255) int]_[*@3 scale])&]
[s2;  [%-*@3 dw ]:.&]
[s2;  [%-*@3 scale] :.&]
[s2; This method is called to paint the element [*@(128.0.255) WHEN 
it is FLOATING] (and only when it is floating). So if an element 
is not allowed to be flottable `=> no need to implement this 
method&]
[s2; Paints the graph element somewhere inside the ctrl area as a 
FLOATING element (legend, ...)&]
[s2; X/Y axis are not taken into account&]
[s2; [*@(128.0.255) Offset and clipping are set] with the `'`_floatFrame`' 
settings.&]
[s2; See [^topic`:`/`/GraphDraw`/srcdoc`/1`_GraphDraw`_basics`$en`-us`#GraphElement Painting Details^ G
raphElement Painting Details] for more detailed explanations.&]
[s3; &]
[s4;%- &]
[s5;:GraphElement`:`:PaintElement`(Draw`&`,int`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* PaintElement]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 dw], 
[@(0.0.255) int]_[*@3 scale])_`=_[@3 0]&]
[s2; [%-*@3 dw] : &]
[s2; [%-*@3 scale] : .&]
[s2; This method is called to paint the element [*@(128.0.255) WHEN 
it is NOT FLOATING]&]
[s2; Paints the element in his own area ( it is not dedicated to 
painting on plot area because only one axis is taken into account: 
either X or Y, another method exists for this purpose )&]
[s2; There is [*@(128.0.255) no clipping] `=`=> drawing can overlap 
plot or other elements&]
[s2; See [^topic`:`/`/GraphDraw`/srcdoc`/1`_GraphDraw`_basics`$en`-us`#GraphElement Painting Details^ G
raphElement Painting Details] for more detailed explanations.&]
[s3; &]
[s4;%- &]
[s5;:GraphElement`:`:PaintOnPlot`_underData`(Draw`&`,int`,int`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* PaintOnPlot`_underData]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 dw], 
[@(0.0.255) int]_[*@3 otherWidth], [@(0.0.255) int]_[*@3 scale])&]
[s5;:GraphElement`:`:PaintOnPlot`_overData`(Draw`&`,int`,int`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* PaintOnPlot`_overData]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 dw], 
[@(0.0.255) int]_[*@3 otherWidth], [@(0.0.255) int]_[*@3 scale])&]
[s2;  [%-*@3 dw] :&]
[s2; [%-*@3 otherWidth] :&]
[s2; [%-*@3 scale] :&]
[s2; Draws additionnal  GraphElement element stuff on PLOT AREA : 
grids, square zones, ...anything you want.&]
[s2; This method is dedicated to painting on plotting area because 
both X/Y zoom/scroll are applied to the drawn data&]
[s2; [*@(128.0.255) Offset and clipping are set with the plot Rect] 
so nothing can be drawn outside.&]
[s2;%- [* PaintOnPlot`_underData() : ]draws under the data plot&]
[s2;%- [* PaintOnPlot`_overData() :  ] draws on top of the data plot&]
[s2; See [^topic`:`/`/GraphDraw`/srcdoc`/1`_GraphDraw`_basics`$en`-us`#GraphElement Painting Details^ G
raphElement Painting Details] for more detailed explanations.&]
[s3; &]
[s4;%- &]
[s5;:GraphElement`:`:FitToData`(FitToDataStrategy`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* FitToData]([^topic`:`/`/GraphDraw`/src`/GraphElementFrame`$en`-us`#typedef^ F
itToDataStrategy]_[*@3 fitStrategy])&]
[s2;  [%-*@3 fitStrategy] .&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor detail]]}}&]
[s0; &]
[s0; ]]