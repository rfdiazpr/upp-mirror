topic "class PipeStream : public Stream";
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
[ {{10000@(113.42.0) [s0; [*@7;4 PipeStream]]}}&]
[s1;:PipeStream`:`:class:%- [@(0.0.255)3 class][3 _][*3 PipeStream][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Stream]&]
[s2; PipeStream is a [%-*^topic`:`/`/Core`/src`/Stream`$en`-us`#Stream`:`:class^@3 Stre
am] based class providing a simple read and write interface around 
a circular buffer. The size of the internal buffer is by default 
managed automatically, growing to accommodate as many data as 
necessary. Optionally the size of the buffer can be altered manually 
or set to hold fixed size (producing an error when overfilled). 
Unlike most U`+`+ streams, PipeStream allows reading and writing 
on the same instance simultaneously. The typical usage of this 
class is an intermediate storage between two interfaces which 
emit and consume data at different rates.&]
[s3;%- &]
[s0;i448;a25;kKO9;@(0.0.255)%- &]
[ {{10000F(128)G(128)@1 [s0; [* Function List]]}}&]
[s3;%- &]
[s5;:PipeStream`:`:`_Put`(int`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* `_Put]([@(0.0.255) i
nt]_[*@3 w])&]
[s2; Called by [* Put] method in case that output byte cannot be stored 
into buffer ([* ptr >`= wrlim]).&]
[s7; [%-*C@3 w]-|Byte to store.&]
[s3; &]
[s4;%- &]
[s5;:PipeStream`:`:`_Term`(`):%- [@(0.0.255) virtual] [@(0.0.255) int]_[* `_Term]()&]
[s2; Called by [* Term] method in case that input byte is not in buffer 
([* ptr >`= rdlim]).&]
[s7; [*/ Return value]-|Value at current position in the stream. Current 
position is not advanced.&]
[s3; &]
[s4;%- &]
[s5;:PipeStream`:`:`_Get`(`):%- [@(0.0.255) virtual] [@(0.0.255) int]_[* `_Get]()&]
[s2; Called by [* Get] method in case that input byte is not in buffer 
([* ptr >`= rdlim]).&]
[s7; [*/ Return value]-|Value read from the stream.&]
[s3; &]
[s4;%- &]
[s5;:PipeStream`:`:`_Put`(const void`*`,dword`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* `_Put]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^dword^ dword]_[*@3 size])&]
[s2; Directly called by [* Put] method. Writes a block of binary data.&]
[s7; [%-*C@3 data]-|Pointer to data.&]
[s7; [%-*C@3 size]-|Size.&]
[s3; &]
[s4;%- &]
[s5;:PipeStream`:`:`_Get`(void`*`,dword`):%- [@(0.0.255) virtual] [_^dword^ dword]_[* `_Get
]([@(0.0.255) void]_`*[*@3 data], [_^dword^ dword]_[*@3 size])&]
[s2; Directly called by [* Get] method in case that there is not enough 
data available ([* ptr `+ size > rdlim]).&]
[s7; [%-*C@3 data]-|Buffer to store returned data.&]
[s7; [%-*C@3 size]-|Requested amount of data in bytes.&]
[s7; [*/ Return value]-|Real number of returned bytes.&]
[s2; &]
[s0;i448;a25;kKO9;:noref:@(0.0.255)%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:PipeStream`:`:IsOpen`(`)const:%- [@(0.0.255) virtual] [@(0.0.255) bool]_[* IsOpen]()_[@(0.0.255) c
onst]&]
[s7; [* Return value]-|Always [*@3 true].&]
[s3;%- &]
[s4;%- &]
[s5;:PipeStream`:`:Reserve`(int`):%- [@(0.0.255) void]_[* Reserve]([@(0.0.255) int]_[*@3 n])&]
[s2; Sets size of the internal buffer to [%-*@3 n][%-  bytes]. The contents 
of the stream is not affected by this operation. Value of [%-*@3 n] 
must be equal or greater then GetAvailable() and non`-zero.&]
[s3; &]
[s4;%- &]
[s5;:PipeStream`:`:GetReserved`(`)const:%- [@(0.0.255) int]_[* GetReserved]()_[@(0.0.255) c
onst]&]
[s2; Returns current size of the internal buffer.&]
[s3;%- &]
[s4;%- &]
[s5;:PipeStream`:`:AutoResize`(bool`):%- [@(0.0.255) void]_[* AutoResize]([@(0.0.255) bool]_
[*@3 resize]_`=_[@(0.0.255) true])&]
[s2; Whether to call [* Reserve] method to grow the buffer size automatically 
when [* Put] is called with more data then is the capacity left 
in the internal buffer. When automatic resizing is disabled, 
[* Put ]might set ERROR`_NOT`_ENOUGH`_SPACE when there is not enough 
space left in the buffer.&]
[s3; &]
[s4;%- &]
[s5;:PipeStream`:`:NoAutoResize`(`):%- [@(0.0.255) void]_[* NoAutoResize]()&]
[s2; Same as Autoresize(false).&]
[s3;%- &]
[s4;%- &]
[s5;:PipeStream`:`:IsAutoResize`(`)const:%- [@(0.0.255) bool]_[* IsAutoResize]()_[@(0.0.255) c
onst]&]
[s2; Returns true if automatic resizing is turned on, false otherwise.&]
[s3;%- &]
[s4;%- &]
[s5;:PipeStream`:`:GetAvailable`(`)const:%- [@(0.0.255) int]_[* GetAvailable]()_[@(0.0.255) c
onst]&]
[s2; Returns number of bytes currently available to be read from 
the stream.&]
[s3;%- &]
[s4;%- &]
[s5;:PipeStream`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Deletes contents and resets stream to initial state. The internal 
buffer is NOT resized to the default size.&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor detail]]}}&]
[s3;%- &]
[s5;:PipeStream`:`:PipeStream`(int`,bool`):%- [* PipeStream]([@(0.0.255) int]_[*@3 buffersi
ze]_`=_[@3 4096], [@(0.0.255) bool]_[*@3 resize]_`=_[@(0.0.255) true])&]
[s2; Constructor.&]
[s7;%- [*@3 buffersize]-|Initial size of the internal buffer&]
[s7;%- [*@3 resize]-|Whether to use automatic resizing&]
[s3; &]
[s4;%- &]
[s5;:PipeStream`:`:`~PipeStream`(`):%- [@(0.0.255) `~][* PipeStream]()&]
[s2; Desctructor.&]
[s0; ]]