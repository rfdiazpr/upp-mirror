topic "3 - GraphDraw class";
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
[{_} 
[s0;*@7;4%% &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 GraphDraw class]]}}&]
[s0; &]
[s0; [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 TYPES][3 , ][@(0.0.255)3 class][3 _][*@4;3 D
ERIVED][3 >]&]
[s1;:CRTP`_EmptyGraphDraw`:`:class: [@(0.0.255) class]_[* CRTP`_EmptyGraphDraw]_:_[@(0.0.255) p
ublic]_[*@3 SeriesGroup]<_[*@4 TYPES], [*@4 DERIVED]_>_, [@(0.0.255) public]_[*@3 GraphElem
entParent]&]
[s2;%% This class is the base class of GraphDraw, it contains all 
the common methods.&]
[s2;%% See [^topic`:`/`/GraphDraw`/src`/ClassHierarchy`$en`-us^ ClassHierarchy 
]for details on hinheritance tree.&]
[s2;%% &]
[s2;%% &]
[s3; &]
[s0;i448;a25;kKO9;@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:SetDrawMode`(DrawMode`): [*@4 DERIVED][@(0.0.255) `&]_[* SetDr
awMode](DrawMode_[*@3 m])&]
[s2;%% Set the drawing mode to be used&]
[s3;%% &]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:GetDrawMode`(`): [@(0.0.255) virtual] 
DrawMode_[* GetDrawMode]()&]
[s2;%% Returns the drawing mode used&]
[s2;%% [*_ NB:] Part of [^topic`:`/`/GraphDraw`/src`/GraphElementParent`$en`-us`#GraphElementParent`:`:class^ G
raphElementParent ]interface&]
[s3; &]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:AddXConverter`(CRTP`_EmptyGraphDraw`:`:TypeCoordConverter`&`): [_^CRTP`_EmptyGraphDraw`:`:TypeCoordConverter^ T
ypeCoordConverter][@(0.0.255) `&]_[* AddXConverter]([_^CRTP`_EmptyGraphDraw`:`:TypeCoordConverter^ T
ypeCoordConverter][@(0.0.255) `&]_[*@3 conv])&]
[s5;:CRTP`_EmptyGraphDraw`:`:AddYConverter`(CRTP`_EmptyGraphDraw`:`:TypeCoordConverter`&`): [_^CRTP`_EmptyGraphDraw`:`:TypeCoordConverter^ T
ypeCoordConverter][@(0.0.255) `&]_[* AddYConverter]([_^CRTP`_EmptyGraphDraw`:`:TypeCoordConverter^ T
ypeCoordConverter][@(0.0.255) `&]_[*@3 conv])&]
[s2;%% [%-*@3 conv] : CorrdinateConverter instance to add&]
[s2;%% Adds a CoordinateConverter instance to the list of the ones 
managed by the GraphDraw.&]
[s2;%% Plotting can then use this CoordinateConverter for plotting.&]
[s2;%% GraphDraw does [*_ not take the ownership] of the CoordinateConverter 
instance so it has to ba managed elsewhere.&]
[s2;%% It`'s main usage is for Child classes who derive from GraphDraw 
and add more axis&]
[s3;%% &]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:SetCurrentXConverter`(int`): [@(0.0.255) void]_[* SetCurrent
XConverter]([@(0.0.255) int]_[*@3 n])&]
[s5;:CRTP`_EmptyGraphDraw`:`:SetCurrentYConverter`(int`): [@(0.0.255) void]_[* SetCurrent
YConverter]([@(0.0.255) int]_[*@3 n])&]
[s2;%%  [%-*@3 n] : index of CoordinateConverter to use&]
[s2;%% Set the CoordinateConverter that will be used when adding 
a series.&]
[s3;%% &]
[s3; &]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:GetXCoordConverter`(`): [_^CRTP`_EmptyGraphDraw`:`:TypeCoordConverter^ T
ypeCoordConverter][@(0.0.255) `&]_[* GetXCoordConverter]()&]
[s2;%% &]
[s3; &]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:GetYCoordConverter`(`): [_^CRTP`_EmptyGraphDraw`:`:TypeCoordConverter^ T
ypeCoordConverter][@(0.0.255) `&]_[* GetYCoordConverter]()&]
[s2;%% &]
[s3; &]
[s3;%% &]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:FitToData`(FitToDataStrategy`): [@(0.0.255) void]_[* FitToDa
ta](FitToDataStrategy_[*@3 fitStrategy][@(0.0.255) `=]ALL`_SERIES)&]
[s2;%%  [%-*@3 fitStrategy] .&]
[s3;%% &]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* GraphElements management]]}}&]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:updateSizes`(const int`): [@(0.0.255) void]_[* updateSizes](
_[@(0.0.255) const]_[@(0.0.255) int]_[*@3 scale]_`=_[@3 1]_)&]
[s2;%%  [%-*@3 scale] .&]
[s3;%% &]
[s4;%% &]
[s5;:CRTP`_EmptyGraphDraw`:`:CreateElement`(int`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T], [@(0.0.255) int]_[*@4 POS`_OF`_GRAPH]>_[*@4 T][@(0.0.255) `&]_[* CreateElement
]([@(0.0.255) int]_[*@3 elementWidth], [@(0.0.255) int]_[*@3 stackPrio])&]
[s2;%% [%-*@3 elementWidth ]:&]
[s2;l576;i64;O0;%% [%-*@3 stackPrio ]:Used to manage the stacking order 
of GraphElements (can be interpreted as the distance from the 
graph). See[^topic`:`/`/GraphDraw`/srcdoc`/1`_GraphDraw`_basics`$en`-us`#GraphElement Stacks^  
stacking details].&]
[s2;l576;%% &]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:CreateElement1`(int`,int`,P1`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T], [@(0.0.255) int]_[*@4 POS`_OF`_GRAPH], [@(0.0.255) class]_[*@4 P1]>_[*@4 T][@(0.0.255) `&
]_[* CreateElement1]([@(0.0.255) int]_[*@3 elementWidth], [@(0.0.255) int]_[*@3 stackPrio],
 [*@4 P1][@(0.0.255) `&]_[*@3 p1]_)&]
[s2;%%  [%-*@3 elementWidth] [%-*@3 stackPrio] [%-*@3 p1] .&]
[s5;:CRTP`_EmptyGraphDraw`:`:CreateElement2`(int`,int`,P1`&`,P2`&`): [@(0.0.255) templa
te]_<[@(0.0.255) class]_[*@4 T], [@(0.0.255) int]_[*@4 POS`_OF`_GRAPH], 
[@(0.0.255) class]_[*@4 P1], [@(0.0.255) class]_[*@4 P2]>_[*@4 T][@(0.0.255) `&]_[* CreateEleme
nt2]([@(0.0.255) int]_[*@3 elementWidth], [@(0.0.255) int]_[*@3 stackPrio], 
[*@4 P1][@(0.0.255) `&]_[*@3 p1], [*@4 P2][@(0.0.255) `&]_[*@3 p2]_)&]
[s2;%% [%-*@3 p1, p2] : parameters needed by constructor of <T> class.&]
[s2;%% Same as [%-*^topic`:`/`/GraphDraw`/src`/CRTP`_EmptyGraphDraw`$en`-us`#CRTP`_EmptyGraphDraw`:`:CreateElement`(int`,int`)^ C
reateElement()] but with additionnal parameters needed by GraphElement 
contructor&]
[s3;%% &]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:AddElement`(T`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T], [@(0.0.255) int]_[*@4 POS`_OF`_GRAPH]>_[*@4 T][@(0.0.255) `&]_[* AddElement]([*@4 T
][@(0.0.255) `&]_[*@3 v], [@(0.0.255) int]_[*@3 stackPrio])&]
[s5;:CRTP`_EmptyGraphDraw`:`:AddElement`(int`,T`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T], [@(0.0.255) int]_[*@4 POS`_OF`_GRAPH]>_[*@4 T][@(0.0.255) `&]_[* AddElement]([@(0.0.255) i
nt]_[*@3 elementWidth], [*@4 T][@(0.0.255) `&]_[*@3 v], [@(0.0.255) int]_[*@3 stackPrio])&]
[s2;%% [%-*@3 elementWidth] :&]
[s2;%% [%-*@3 v] : [^topic`:`/`/GraphDraw`/src`/2`_GraphElementFrame`$en`-us`#GraphElementFrame`:`:class^ G
raphElement ]instance to add&]
[s2;%% [%-*@3 stackPrio] :&]
[s2;%% Add a [^topic`:`/`/GraphDraw`/src`/2`_GraphElementFrame`$en`-us`#GraphElementFrame`:`:class^ G
raphElement ]to one of the [^topic`:`/`/GraphDraw`/src`/1`_GraphDraw`_basics`$en`-us^ f
ive stacks available].&]
[s2;%% When using this method the wonership of the GraphElement is 
not managed by GraphDraw class.&]
[s3;%% &]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:AddLeftElement`(T`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[*@4 T][@(0.0.255) `&]_[* AddLeftElement]([*@4 T][@(0.0.255) `&]_[*@3 v], 
[@(0.0.255) int]_[*@3 stackPrio])&]
[s5;:CRTP`_EmptyGraphDraw`:`:AddRightElement`(T`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[*@4 T][@(0.0.255) `&]_[* AddRightElement]([*@4 T][@(0.0.255) `&]_[*@3 v], 
[@(0.0.255) int]_[*@3 stackPrio])&]
[s5;:CRTP`_EmptyGraphDraw`:`:AddTopElement`(T`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[*@4 T][@(0.0.255) `&]_[* AddTopElement]([*@4 T][@(0.0.255) `&]_[*@3 v], 
[@(0.0.255) int]_[*@3 stackPrio])&]
[s5;:CRTP`_EmptyGraphDraw`:`:AddBottomElement`(T`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[*@4 T][@(0.0.255) `&]_[* AddBottomElement]([*@4 T][@(0.0.255) `&]_[*@3 v], 
[@(0.0.255) int]_[*@3 stackPrio])&]
[s5;:CRTP`_EmptyGraphDraw`:`:AddFloatElement`(T`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[*@4 T][@(0.0.255) `&]_[* AddFloatElement]([*@4 T][@(0.0.255) `&]_[*@3 v], 
[@(0.0.255) int]_[*@3 stackPrio])&]
[s2;%% Same as [%-*^topic`:`/`/GraphDraw`/src`/3`_GraphDraw`$en`-us`#CRTP`_EmptyGraphDraw`:`:AddElement`(T`&`,int`)^ A
ddElement()].&]
[s3;%% &]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:AddLeftElement`(int`,T`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[*@4 T][@(0.0.255) `&]_[* AddLeftElement]([@(0.0.255) int]_[*@3 elementWidth],
 [*@4 T][@(0.0.255) `&]_[*@3 v], [@(0.0.255) int]_[*@3 stackPrio])&]
[s5;:CRTP`_EmptyGraphDraw`:`:AddRightElement`(int`,T`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[*@4 T][@(0.0.255) `&]_[* AddRightElement]([@(0.0.255) int]_[*@3 elementWidth],
 [*@4 T][@(0.0.255) `&]_[*@3 v], [@(0.0.255) int]_[*@3 stackPrio])&]
[s5;:CRTP`_EmptyGraphDraw`:`:AddTopElement`(int`,T`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[*@4 T][@(0.0.255) `&]_[* AddTopElement]([@(0.0.255) int]_[*@3 elementWidth], 
[*@4 T][@(0.0.255) `&]_[*@3 v], [@(0.0.255) int]_[*@3 stackPrio])&]
[s5;:CRTP`_EmptyGraphDraw`:`:AddBottomElement`(int`,T`&`,int`): [@(0.0.255) template]_<
[@(0.0.255) class]_[*@4 T]>_[*@4 T][@(0.0.255) `&]_[* AddBottomElement]([@(0.0.255) int]_[*@3 e
lementWidth], [*@4 T][@(0.0.255) `&]_[*@3 v], [@(0.0.255) int]_[*@3 stackPrio])&]
[s5;:CRTP`_EmptyGraphDraw`:`:AddFloatElement`(int`,T`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[*@4 T][@(0.0.255) `&]_[* AddFloatElement]([@(0.0.255) int]_[*@3 elementWidth],
 [*@4 T][@(0.0.255) `&]_[*@3 v], [@(0.0.255) int]_[*@3 stackPrio])&]
[s2;%% Same as [%-*^topic`:`/`/GraphDraw`/src`/3`_GraphDraw`$en`-us`#CRTP`_EmptyGraphDraw`:`:AddElement`(T`&`,int`)^ A
ddElement()].&]
[s3;%% &]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:`_ctrlRect: [_^Rect^ Rect]_[* `_ctrlRect]&]
[s2;%% Rect of the whole Graph (Plot area `+ all GraphElements surrounding)&]
[s3; &]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:`_plotRect: [_^Rect^ Rect]_[* `_plotRect]&]
[s2;%% Rect of the graph plot area ( inside the `_ctrlRect )&]
[s3; &]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:`_PlotDrawImage: [_^Image^ Image]_[* `_PlotDrawImage]&]
[s2;%% Image of the drawn Plot data (and only that).&]
[s2;%% This Image is used to store data plotting result and therefor 
speed`-up drawing of Graph when plot data hasn`'t changed. In 
thsi case the plot image is just drawn on the Graph result without 
really processing the plot data&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Undo/Redo management methods]]}}&]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:AddUndoAction`(GraphUndoData`&`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* AddUndoAction]([_^GraphUndoData^ GraphUndoData][@(0.0.255) `&]_[*@3 CB
])&]
[s2;%%  [%-*@3 CB] : GraphUndoData object containg the Undo/Redo actions.&]
[s2;%% Undo/Redo is accomplished by using a stack of Undo/Redo objects.&]
[s2;%% See [^topic`:`/`/GraphDraw`/src`/GraphUndo`$en`-us`#GraphUndo`:`:class^ GraphUnd
o] for more details.&]
[s3;%% &]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:MakeRestoreGraphSizeCB`(`): [@(0.0.255) virtual] 
[_^Callback^ Callback]_[* MakeRestoreGraphSizeCB]()&]
[s2;%% Returns a Callback that, when executed, will set the Graph 
size settings identical to the current settings.&]
[s2;%% This is useful for UNDO / REDO management&]
[s2;%% NB: This method is part of the [^topic`:`/`/GraphDraw`/src`/GraphElementParent`$en`-us`#GraphElementParent`:`:MakeRestoreGraphSizeCB`(`)^ G
raphElementParent ]interface&]
[s3; &]
[s0;%% &]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:Undo`(`): [@(0.0.255) void]_[* Undo]()&]
[s2;%% Execute the most recent Undo action&]
[s3; &]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:Redo`(`): [@(0.0.255) void]_[* Redo]()&]
[s2;%% Execute the most recent Redo action&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Painting methods]]}}&]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:RefreshFromChild`(RefreshStrategy`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* RefreshFromChild](_RefreshStrategy_[*@3 doFastPaint]_)&]
[s2;%%  [%-*@3 doFastPaint] :&]
[s2;%% Method called from child GraphElements of GraphDraw when a 
Refresh is requested by the child&]
[s2;%% [*_ NB:] Part of [^topic`:`/`/GraphDraw`/src`/GraphElementParent`$en`-us`#GraphElementParent`:`:class^ G
raphElementParent ]interface&]
[s3;%% &]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:PaintPlotData`(T`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) void]_[* PaintPlotData]([*@4 T][@(0.0.255) `&]_[*@3 dw], 
[@(0.0.255) int]_[*@3 scale])&]
[s2;%% [%-*@3 dw] :&]
[s2;%% [%-*@3 scale] :&]
[s2;%% This method paints the data points on the plotting area.&]
[s2;%% It manages the [* FASTPAINT ]feature that only draws partial 
data in order to draw very quickly when [_ scrolling ]or [_ moving 
]the plot area.&]
[s2;%% Called by [^topic`:`/`/GraphDraw`/src`/3`_GraphDraw`$en`-us`#CRTP`_EmptyGraphDraw`:`:Paint`(T`&`,int`)^ P
aint()].&]
[s3;%% &]
[s4; &]
[s5;:CRTP`_EmptyGraphDraw`:`:Paint`(T`&`,int`): [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 T]>_[@(0.0.255) void]_[* Paint]([*@4 T][@(0.0.255) `&]_[*@3 dw], [@(0.0.255) int]_[*@3 scale
])&]
[s2;%% [%-*@3 dw] :&]
[s2;%% [%-*@3 scale] :&]
[s2;%% This method paints the complete GraphDraw ( it calls [%-^topic`:`/`/GraphDraw`/src`/3`_GraphDraw`$en`-us`#CRTP`_EmptyGraphDraw`:`:PaintPlotData`(T`&`,int`)^ P
aintPlotData()] to plot the data points ).&]
[s2;%% This method also manages the following cache related points:&]
[s2;i150;O0;%% GraphDraw background image caching&]
[s2;i150;O0;%% Plot area background image caching&]
[s2;i150;O0;%% Plotted data image caching&]
[s2;%% These points allow to speed up the drawing of GraphDraw when 
necessary&]
[s3;%% &]
[s0;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s0;%% &]
[s0;%% ]]