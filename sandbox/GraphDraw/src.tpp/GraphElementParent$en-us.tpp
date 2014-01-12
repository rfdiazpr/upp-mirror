topic "GrapElementParent";
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
[ {{10000@(113.42.0) [s0; [*@7;4 GraphElementParent]]}}&]
[s0;%- &]
[s1;:GraphElementParent`:`:class:%- [@(0.0.255)3 class][3 _][*3 GraphElementParent]&]
[s2; Publishes the interface towards the Parent(container) GraphDraw 
instance.&]
[s2; This class is needed because these methods need to be present 
in GraphDraw package while used mainly for GraphCtrl.&]
[s2; This class allows to kick out `'Ctrl`' from GraphDraw (and therefor 
keep GraphDraw independent from CtrlLib).&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s4;%- &]
[s5;:GraphElementParent`:`:ScrollX`(TypeScreenCoord`,bool`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* ScrollX](_[_^TypeScreenCoord^ TypeScreenCoord]_[*@3 xOffset], 
[@(0.0.255) bool]_[*@3 doRefresh]_`=_[@(0.0.255) true])_`=_[@3 0]&]
[s5;:GraphElementParent`:`:ScrollY`(TypeScreenCoord`,bool`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* ScrollY](_[_^TypeScreenCoord^ TypeScreenCoord]_[*@3 yOffset], 
[@(0.0.255) bool]_[*@3 doRefresh]_`=_[@(0.0.255) true])_`=_[@3 0]&]
[s2; [%-*@3 yOffset] : scrolling offset&]
[s2; [%-*@3 doRefresh] : put tu FALSE if you don`'t need to do a refresh 
immediatly&]
[s2; Called when a GraphElement needs to[*  Scroll the graph], this 
results in modifying all the needed CoordinateConverters so no 
need to do it locally.&]
[s3;@3%- &]
[s4;@3%- &]
[s5;:GraphElementParent`:`:ZoomX`(TypeScreenCoord`,TypeScreenCoord`):%- [@(0.0.255) vir
tual] [@(0.0.255) void]_[* ZoomX]([_^TypeScreenCoord^ TypeScreenCoord]_[*@3 left], 
[_^TypeScreenCoord^ TypeScreenCoord]_[*@3 right])_`=_[@3 0]&]
[s5;:GraphElementParent`:`:ZoomY`(TypeScreenCoord`,TypeScreenCoord`):%- [@(0.0.255) vir
tual] [@(0.0.255) void]_[* ZoomY]([_^TypeScreenCoord^ TypeScreenCoord]_[*@3 top], 
[_^TypeScreenCoord^ TypeScreenCoord]_[*@3 bottom])_`=_[@3 0]&]
[s2; [%-*@3 input ]: coordinates [*@(128.0.255) relative to the graph 
plot frame] `=`=> Top Left corner `= (0, 0)&]
[s2; Called when a GraphElement needs to [* Zoom the graph], this results 
in modifying all the needed CoordinateConverters  so no need 
to do it locally.&]
[s2; &]
[s3; &]
[s4;%- &]
[s5;:GraphElementParent`:`:RefreshFromChild`(RefreshStrategy`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* RefreshFromChild](_RefreshStrategy_[*@3 doFastPaint]_)_`=_[@3 0]&]
[s2;  [%-*@3 doFastPaint]  : Refresh strategy to use : complete resfresh, 
or fast refresh (partial).&]
[s2; Lauches a refresh of the whole Ctrl.&]
[s3; &]
[s4;%- &]
[s5;:GraphElementParent`:`:GetSeries`(`):%- [@(0.0.255) virtual] TypeVectorSeries`&_[* Ge
tSeries]()_`=_[@3 0]&]
[s2; Returns the reference to the SERIES of PLOT DATA&]
[s3;%- &]
[s4;%- &]
[s5;:GraphElementParent`:`:GetParentCtrl`(`):%- [@(0.0.255) virtual] 
[_^Value^ Value]_[* GetParentCtrl]()_`=_[@3 0]&]
[s2; To be [*@(128.0.255) used ONLY with GraphCtrl][*  ]since it returns 
a Value object containing a `'Ctrl`*`'.&]
[s2; It can be used the following way:&]
[s2; [C1 Ctrl`* parentCtrl `= ValueTo<Ctrl`*>(`_B`::`_parent`->GetParentCtrl());]&]
[s3;%- &]
[s4;%- &]
[s5;:GraphElementParent`:`:AddUndoAction`(GraphUndoData`&`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* AddUndoAction]([_^topic`:`/`/GraphDraw`/src`/GraphUndo`$en`-us`#GraphUndoData`:`:class^@(0.0.255) G
raphUndoData][@(0.0.255) `&]_[*@3 CB])_`=_[@3 0]&]
[s2;  [%-*@3 CB] .&]
[s2; Adds an UNDO action to the [^topic`:`/`/GraphDraw`/src`/GraphUndo`$en`-us`#GraphUndo`:`:AddUndoAction`(GraphUndoData`&`)^ u
ndo stack].&]
[s3; &]
[s4;%- &]
[s5;:GraphElementParent`:`:DoLocalLoop`(MouseLocalLoopCB`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* DoLocalLoop]([_^MouseLocalLoopCB^ MouseLocalLoopCB]_[*@3 CB])&]
[s2;  [%-*@3 CB] : callback executed by LocalLoop.&]
[s2; Used to execute a local loop from a GraphElement ( [@(128.0.255) intended 
to manage move, resize, zoom actions ]).&]
[s2; This method is needed because the LocalLoop class constructor 
requests a Ctrl instance (to get the mouse/key events from) and 
then the LocalLoop cannot be run directly from a GraphElement.&]
[s2; By default, while executing the CB, the [@(128.0.255) MouseCursor 
image is kept fixed] to it`'s value when the local loop started.&]
[s3; &]
[s4;%- &]
[s5;:GraphElementParent`:`:MakeRestoreGraphSizeCB`(`):%- [@(0.0.255) virtual] 
[_^Callback^ Callback]_[* MakeRestoreGraphSizeCB]()_`=_[@3 0]&]
[s2; Returns a Callback that, when executed, will set the whole Graph 
size settings identical to the current settings.&]
[s2; This is useful for UNDO / REDO when the whole graph is zoomed/scrolled 
by a graphElement&]
[s3;%- &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor detail]]}}&]
[s4;%- &]
[s5;:GraphElementParent`:`:GetDrawMode`(`):%- [@(0.0.255) virtual] 
DrawMode_[* GetDrawMode]()_`=_[@3 0]&]
[s2; Returns the drawing mode used.&]
[s2; This method is used by [^topic`:`/`/GraphDraw`/src`/GraphElement`$en`-us`#GraphElement`:`:class^ G
raphElements ]that need to draw using Painter instead of the 
native Draw passed to GraphElement`::PaintXxx() methods.&]
[s0; ]]