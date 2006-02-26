TITLE("Algorithms")
REF("::Swap(T&,T&)")
REF("::IterSwap(I,I)")
REF("::FindLowerBound(const C&,int,int,const T&,const L&)")
REF("::FindLowerBound(const C&,const T&,const L&)")
REF("::FindLowerBound(const C&,const T&)")
REF("::FindUpperBound(const C&,int,int,const T&,const L&)")
REF("::FindUpperBound(const C&,const T&,const L&)")
REF("template<class C, class T> int FindUpperBound(const C& v, const T& val)")
REF("::FindUpperBound(const C&,const T&)")
REF("::FindBinary(const C&,const T&,int,int,const L&)")
REF("::FindBinary(const C&,const T&,const L&)")
REF("::FindBinary(const C&,const T&)")
REF("::AppendSorted(C&,const C&,const L&)")
REF("::AppendSorted(C&,const C&)")
REF("::UnionSorted(C&,const C&,const L&)")
REF("::UnionSorted(C&,const C&)")
REF("::RemoveSorted(C&,const C&,const L&)")
REF("::RemoveSorted(C&,const C&)")
REF("::IntersectSorted(D&,const S&,const L&)")
REF("::IntersectSorted(D&,const S&)")
REF("::Sort(T&,const Less&)")
REF("::Sort(T&)")
REF("::IndexSort(KC&,VC&,const Less&)")
REF("::IndexSort(KC&,VC&)")
REF("::GetSortOrder(const C&,const Less&)")
REF("::GetSortOrder(const C&)")
REF("::FieldRelation(O(T::*),const R&)")
REF("::MethodRelation(O(T::*)(),const R&)")
REF("::MethodRelation(O(T::*)()const,const R&)")
TOPIC_TEXT(
"[2 $$0,0#00000000000000000000000000000000:Default][l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param][a83;*R6 "
"$$2,5#31310162474203024125188417583966:caption][b83;* $$3,5#07864147445237544204411237157677:title][b167;a42;C2 "
"$$4,6#40027414424643823182269349404212:item][b42;a42;2 $$5,5#45413000475342174754091244180557:text][l288;a17;2 "
"$$6,6#27521748481378242620020725143825:desc][l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code][b2503; "
"$$8,0#65142375456100023862071332075487:separator][*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base][t4167;C "
"$$10,0#37138531426314131251341829483380:class][l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement][i417;b42;a42;O9;~~~.416;2 "
"$$12,12#10566046415157235020018451313112:tparam][b167;C2 $$13,13#92430459443460461911108080531343:item1][i288;a42;O9;C2 "
"$$14,14#77422149456609303542238260500223:item2][*@2$(0.128.128) $$15,15#34511555403152284025741354420178:NewsDate][l321;*C$7;2 "
"$$16,16#03451589433145915344929335295360:result][l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line][l160;t4167;*C+117 "
"$$18,5#88603949442205825958800053222425:package`-title][{_}%EN-US [s2; Algorithms&][s13; "
"template<class [*@4 T]>&][s14;:`:`:Swap`(T`&`,T`&`): void [* Swap]([*@4 T]`& [*@3 "
"a], [*@4 T]`& [*@3 b])&][s6; Swaps values. Specific types might specialize [* Swap] "
"with more effective variants.&][s11; T must have either deep copy operator or pick "
"operator.&][s1; [*C@4 T]-|Type of values.&][s1; [*C@3 a]-|First value to swap.&][s1; "
"[*C@3 b]-|Second value to swap.&][s13; template<class [*@4 I]>&][s14;:`:`:IterSwap`(I`,I`): "
"void [* IterSwap]([*@4 I][*  ][*@3 a], [*@4 I][*  ][*@3 b])&][s6; Swaps values pointed "
"to by iterators. Specific types might specialize [* IterSwap] with more effective "
"variants.&][s11; Swap must be defined for type pointed to by I.&][s1; [*C@4 I]-|Iterator "
"type.&][s1; [*C@3 a]-|Iterator pointing to first value.&][s1; [*C@3 b]-|Iterator "
"pointing to second value.&][s13; t")
TOPIC_TEXT(
"emplate<class [*@4 C], class [*@4 T], class [*@4 L]>&][s14;:`:`:FindLowerBound`(const "
"C`&`,int`,int`,const T`&`,const L`&`): int [* FindLowerBound](const [*@4 C]`& [*@3 "
"v], int [*@3 pos], int [*@3 count], const [*@4 T]`& [*@3 val], const [*@4 L]`& [*@3 "
"less])&][s6; Finds first position in range of container sorted by [* less] predicate "
"where [* val] can be inserted without breaking the ordering.&][s1; [*C@4 C]-|Type "
"of container.&][s1; [*C@4 T]-|Type of value.&][s1; [*C@3 v]-|Container.&][s1; [*C@3 "
"pos]-|Beginning of range.&][s1; [*C@3 count]-|Number of elements in range.&][s1; "
"[*C@3 val]-|Value to find.&][s1; [*C@3 less]-|Ordering predicate.&][s1; [*/ Return "
"value]-|Position in container.&][s13; template<class [*@4 C], class [*@4 T], class "
"[*@4 L]>&][s14;:`:`:FindLowerBound`(const C`&`,const T`&`,const L`&`): int [* FindLowerBound](const "
"[*@4 C]`& [*@3 v], const [*@4 T]`& [*@3 val], const [*@4 L]`& [*@3 less])&][s6; Finds "
"first position in sorted by [* less] predicate where [* val] can be inserted without "
"breaking the ordering.&][s1; [*C@4 C]-|Type of container.&][s1; [*C@4 T]-|Type of "
"value.&][s1; [*C@3 v]-|Container.&][s1; [*C@3 val]-|Value to find.&][s1; [*C@3 less]-|Ordering "
"predicate.&][s1; [*/ Return value]-|Position in container.&][s13; template<class "
"[*@4 C], class [*@4 T]>&][s14;:`:`:FindLowerBound`(const C`&`,const T`&`): int [* "
"FindLowerBound](const [*@4 C]`& [*@3 v], const [*@4 T]`& [*@3 val])&][s6; Finds first "
"position in sorted by [*/ operator<] predicate where [* val] can be inserted without "
"breaking the ordering.&][s1; [*C@4 C]-|Type of container.&][s1; [*C@4 T]-|Type of "
"value.&][s1; [*C@3 v]-|Container.&][s1; [*C@3 val]-|Value to find.&][s1; [*/ Return "
"value]-|Position in container.&][s13; template<class [*@4 C], class [*@4 T], class "
"[*@4 L]>&][s14;:`:`:FindUpperBound`(const C`&`,int`,int`,const T`&`,const L`&`): "
"int [* FindUpperBound](const [*@4 C]`& [*@3 v], int [*@3 pos], int [*@3 count], const "
"[*@4 T]`& [*@3 val], const [*@4 L]`& [*@3 less])&][")
TOPIC_TEXT(
"s6; Finds last position in range of container sorted by [* less] predicate where "
"[* val] can be inserted without breaking the ordering.&][s1; [*C@4 C]-|Type of container.&][s1; "
"[*C@4 T]-|Type of value.&][s1; [*C@3 v]-|Container.&][s1; [*C@3 pos]-|Beginning of "
"range.&][s1; [*C@3 count]-|Number of elements in range.&][s1; [*C@3 val]-|Value to "
"find.&][s1; [*C@3 less]-|Ordering predicate.&][s1; [*/ Return value]-|Position in "
"container.&][s13; template<class [*@4 C], class [*@4 T], class [*@4 L]>&][s14;:`:`:FindUpperBound`(const "
"C`&`,const T`&`,const L`&`): int [* FindUpperBound](const [*@4 C]`& [*@3 v], const "
"[*@4 T]`& [*@3 val], const [*@4 L]`& [*@3 less])&][s6; Finds last position in sorted "
"by [* less] predicate where [* val] can be inserted without breaking the ordering.&][s1; "
"[*C@4 C]-|Type of container.&][s1; [*C@4 T]-|Type of value.&][s1; [*C@3 v]-|Container.&][s1; "
"[*C@3 val]-|Value to find.&][s1; [*C@3 less]-|Ordering predicate.&][s1; [*/ Return "
"value]-|Position in container.&][s13;:template`<class C`, class T`> int FindUpperBound`(const "
"C`& v`, const T`& val`): template<class [*@4 C], class [*@4 T]>&][s14;:`:`:FindUpperBound`(const "
"C`&`,const T`&`): int [* FindUpperBound](const [*@4 C]`& [*@3 v], const [*@4 T]`& "
"[*@3 val])&][s6; Finds last position in sorted by [*/ operator<] predicate where "
"[* val] can be inserted without breaking the ordering.&][s1; [*C@4 C]-|Type of container.&][s1; "
"[*C@4 T]-|Type of value.&][s1; [*C@3 v]-|Container.&][s1; [*C@3 val]-|Value to find.&][s1; "
"[*/ Return value]-|Position in container.&][s13; template<class [*@4 C], class [*@4 "
"T], class [*@4 L]>&][s14;:`:`:FindBinary`(const C`&`,const T`&`,int`,int`,const L`&`): "
"int [* FindBinary](const [*@4 C]`& [*@3 v], const [*@4 T]`& [*@3 val], int [*@3 pos], "
"int [*@3 count], const [*@4 L]`& [*@3 less])&][s6; Finds position of element with "
"specified value in a range of container sorted by [* less] predicate. If no such "
"element exists, a negative value is returned.&][s1; [*C@4 C]-|Type of contai")
TOPIC_TEXT(
"ner.&][s1; [*C@4 T]-|Type of value.&][s1; [*C@3 v]-|Container.&][s1; [*C@3 pos]-|Beginning "
"of range.&][s1; [*C@3 count]-|Number of elements in range.&][s1; [*C@3 val]-|Value "
"to find.&][s1; [*C@3 less]-|Ordering predicate.&][s1; [*/ Return value]-|Position "
"in container.&][s13; template<class [*@4 C], class [*@4 T], class [*@4 L]>&][s14;:`:`:FindBinary`(const "
"C`&`,const T`&`,const L`&`): int [* FindBinary](const [*@4 C]`& [*@3 v], const [*@4 "
"T]`& [*@3 val], const [*@4 L]`& [*@3 less])&][s6; Finds position of element with "
"specified value in the container sorted by [* less] predicate. If no such element "
"exists, a negative value is returned.&][s1; [*C@4 C]-|Type of container.&][s1; [*C@4 "
"T]-|Type of value.&][s1; [*C@3 v]-|Container.&][s1; [*C@3 val]-|Value to find.&][s1; "
"[*C@3 less]-|Ordering predicate.&][s1; [*/ Return value]-|Position in container.&][s13; "
"template<class [*@4 C], class [*@4 T]>&][s14;:`:`:FindBinary`(const C`&`,const T`&`): "
"int [* FindBinary](const [*@4 C]`& [*@3 v], const [*@4 T]`& [*@3 val])&][s6; Finds "
"position of element with specified value in the container sorted by [*/ operator<] "
"predicate. If no such element exists, a negative value is returned.&][s1; [*C@4 C]-|Type "
"of container.&][s1; [*C@4 T]-|Type of value.&][s1; [*C@3 v]-|Container.&][s1; [*C@3 "
"val]-|Value to find.&][s1; [*/ Return value]-|Position in container.&][s13;~~~.992; "
"template<class [*@4 C]>&][s14;:`:`:AppendSorted`(C`&`,const C`&`,const L`&`):~~~.992; "
"[*@4 C]`& [* AppendSorted]([*@4 C]`& [*@3 dest], const [*@4 C]`& [*@3 src], const "
"[*@4 L]`& [*@3 less])&][s6; Merges source NTL container to destination NTL container. "
"Both containers must be sorted in ascending order. After the operation, destination "
"container is sorted in ascending order and contains values from both containers. "
"Duplicate values are preserved. Ordering predicate is determined by [* less].&][s1; "
"[*C@4 C]-|Type of container.&][s1; [*C@3 dest]-|Destination container.&][s1; [*C@3 "
"src]-|Source container.&][s1; [*C@3 less")
TOPIC_TEXT(
"]-|Ordering predicate.&][s1;C [*/A Return value][A -|Destination container.]template<class "
"[*@4 C]>&][s13;~~~.992; template<class [*@4 C]>&][s14;:`:`:AppendSorted`(C`&`,const "
"C`&`):~~~.992; [*@4 C]`& [* AppendSorted]([*@4 C]`& [*@3 dest], const [*@4 C]`& [*@3 "
"src])&][s6;~~~.992; Merges source NTL container to destination NTL container. Both "
"containers must be sorted in ascending order. After the operation, destination container "
"is sorted in ascending order and contains values from both containers. Duplicate "
"values are preserved. Ordering is determined by [*/ operator<].&][s1; [*C@4 C]-|Type "
"of container.&][s1; [*C@3 dest]-|Destination container.&][s1; [*C@3 src]-|Source "
"container.&][s1;C [*/A Return value][A -|Destination container.]template<class [*@4 "
"C]>&][s13; template<class [*@4 C], class [*@4 L]>&][s14;:`:`:UnionSorted`(C`&`,const "
"C`&`,const L`&`):~~~736; [*@4 C]`& [* UnionSorted]([*@4 C]`& [*@3 dest], const [*@4 "
"C]`& [*@3 src], const [*@4 L]`& [*@3 less])&][s6; Merges source NTL container to "
"destination NTL container. Both containers must be sorted in ascending order and "
"values must be unique. After the operation, destination container is sorted in ascending "
"order and contains unique values from both containers. Ordering is determined by "
"[* less].&][s1; [*C@4 C]-|Type of container.&][s1; [*C@3 dest]-|Destination container.&][s1; "
"[*C@3 src]-|Source container.&][s1; [*C@3 less]-|Ordering predicate.&][s1; [*/ Return "
"value]-|Destination container.&][s13; template<class [*@4 C]>&][s14;:`:`:UnionSorted`(C`&`,const "
"C`&`):~~~.992; [*@4 C]`& [* UnionSorted]([*@4 C]`& [*@3 dest], const [*@4 C]`& [*@3 "
"src])&][s6; Merges the source NTL container to the destination NTL container. Both "
"containers must be sorted in ascending order and values must be unique. After the "
"operation, destination container is sorted in ascending order and contains unique "
"values from both containers. Ordering is determined by [*/ operator<].&][s1; [*C@4 "
"C]-|Type of container.&][s1; [*C@3 dest]-|Dest")
TOPIC_TEXT(
"ination container.&][s1; [*C@3 src]-|Source container.&][s1; [*/ Return value]-|Destination "
"container.&][s13; template<class [*@4 C], class [*@4 L]>&][s14;:`:`:RemoveSorted`(C`&`,const "
"C`&`,const L`&`): [*@4 C]`& [* RemoveSorted]([*@4 C]`& [*@3 from], const [*@4 C]`& "
"[*@3 what], const [*@4 L]`& [*@3 less])&][s6; Removes elements of source container "
"from destination container. Both containers must be sorted in ascending order and "
"values must be unique. Ordering is determined by [* less].&][s1; [*C@4 C]-|Type of "
"container.&][s1; [*C@3 from]-|Destination container.&][s1; [*C@3 what]-|Source container.&][s1; "
"[*C@3 less]-|Ordering predicate.&][s1; [*/ Return value]-|Destination container.&][s13; "
"template<class [*@4 C]>&][s14;:`:`:RemoveSorted`(C`&`,const C`&`): [*@4 C]`& [* RemoveSorted]([*@4 "
"C]`& [*@3 from], const [*@4 C]`& [*@3 what])&][s6; Removes elements of source container "
"from destination container. Both containers must be sorted in ascending order and "
"values must be unique. Ordering is determined by [*/ operator<].&][s1; [*C@4 C]-|Type "
"of container.&][s1; [*C@3 from]-|Destination container.&][s1; [*C@3 what]-|Source "
"container.&][s1; [*/ Return value]-|Destination container.&][s13; template<class "
"[*@4 D], class [*@4 S], class [*@4 L]>&][s14;:`:`:IntersectSorted`(D`&`,const S`&`,const "
"L`&`): [*@4 D]`& [* IntersectSorted]([*@4 D]`& [*@3 dest], const [*@4 S]`& [*@3 src], "
"const [*@4 L]`& [*@3 less])&][s6; Removes elements from destination container that "
"are not contained in source container. Both containers must be sorted in ascending "
"order and values must be unique. Ordering is determined by [* less].&][s1; [*C@4 "
"D]-|Type of destination container.&][s1; [*C@4 S]-|Type of source container.&][s1; "
"[*C@3 dest]-|Destination container.&][s1; [*C@3 src]-|Source container.&][s1; [*C@3 "
"less]-|Destination container.&][s13; template<class [*@4 D], class [*@4 S]>&][s14;:`:`:IntersectSorted`(D`&`,const "
"S`&`): [*@4 D]`& [* IntersectSorted]([*@4 D]`& [*@3 dest], const [*@4 S]`& [*@")
TOPIC_TEXT(
"3 src])&][s6; Removes elements from destination container that are not contained "
"in source container. Both containers must be sorted in ascending order and values "
"must be unique. Ordering is determined by [*/ operator<].&][s1; [*C@4 D]-|Type of "
"destination container.&][s1; [*C@4 S]-|Type of source container.&][s1; [*C@3 dest]-|Destination "
"container.&][s1; [*C@3 src]-|Source container.&][s1; [*/ Return value]-|Destination "
"container.&][s13; template<class [*@4 T], class [*@4 Less]>&][s14;:`:`:Sort`(T`&`,const "
"Less`&`): void [* Sort]([*@4 T]`& [*@3 c], const [*@4 Less]`& [*@3 less])&][s6; Sorts "
"container. Ordering is determined by [* less].&][s11; IterSwap must be defined for "
"T`::Iterator.&][s1; [*C@4 T]-|Type of container.&][s1; [*C@3 c]-|Container.&][s1; "
"[*C@3 less]-|Ordering predicate.&][s13; template<class [*@4 T]>&][s14;:`:`:Sort`(T`&`): "
"void [* Sort]([*@4 T]`& [*@3 c])&][s6; Sorts container. Ordering is determined by "
"[*/ operator<].&][s6;*1 IterSwap must be defined for T`::Iterator.&][s1; [*C@4 T]-|Type "
"of container.&][s1; [*C@3 c]-|Container.&][s13; template<class [*@4 KC], class [*@4 "
"VC], class [*@4 Less]>&][s14;:`:`:IndexSort`(KC`&`,VC`&`,const Less`&`): void [* "
"IndexSort]([*@4 KC]`& [*@3 keys], [*@4 VC]`& [*@3 values], const [*@4 Less]`& [*@3 "
"less])&][s6; Sorts pair of containers. Both containers must have same number of items. "
"Resulting order is determined by the [* keys] container. Ordering is determined by "
"[* less].&][s1; [*C@4 KC]-|Type of keys container.&][s1; [*C@4 VC]-|Type of values.&][s1; "
"[*C@3 keys]-|Container of keys.&][s1; [*C@3 values]-|Container of values.&][s1; [*C@3 "
"less]-|Ordering predicate.&][s13; template<class [*@4 KC], class [*@4 VC]>&][s14;:`:`:IndexSort`(KC`&`,VC`&`): "
"void [* IndexSort]([*@4 KC]`& [*@3 keys], [*@4 VC]`& [*@3 values])&][s6; Sorts pair "
"of containers. Both containers must have same number of items. Resulting order is "
"determined by the [* keys] container. Ordering is determined by [*/ operator<].&][s1; "
"[*C@4 KC]-|Type of ke")
TOPIC_TEXT(
"ys container.&][s1; [*C@4 VC]-|Type of values container.&][s1; [*C@3 keys]-|Container "
"of keys.&][s1; [*C@3 values]-|Container of values.&][s13; template<class [*@4 C], "
"class [*@4 Less]>&][s14;:`:`:GetSortOrder`(const C`&`,const Less`&`): Vector<int> "
"[* GetSortOrder](const [*@4 C]`& [*@3 container], const [*@4 Less]`& [*@3 less])&][s6; "
"Creates ascending order of values in container. Ordering is determined by [* less].&][s1; "
"[*C@4 C]-|Type of container.&][s1; [*C@3 container]-|Source container.&][s1; [*C@3 "
"less]-|Ordering predicate.&][s1; [*/ Return value]-|Vector of positions of source "
"container in sorted order.&][s13; template<class [*@4 C]>&][s14;:`:`:GetSortOrder`(const "
"C`&`): Vector<int> [* GetSortOrder](const [*@4 C]`& [*@3 container])&][s6; Creates "
"ascending order of values in container. Ordering is determined by [*/ operator<].&][s1; "
"[*C@4 C]-|Type of container.&][s1; [*C@3 container]-|Source container.&][s1; [*/ "
"Return value]-|Vector of positions of source container in sorted order.&][s13; template<class "
"[*@4 O], class [*@4 T], class [*@4 R]>&][s14;:`:`:FieldRelation`(O`(T`:`:`*`)`,const "
"R`&`): [/ type] [* FieldRelation]([*@4 O] ([*@4 T]`::`*[*@3 member]), const [*@4 "
"R]`& [*@3 relation])&][s6; Creates ordering predicate for [* T] based on the value "
"of member variable of[*  T].&][s1; [*C@4 T]-|Type of element.&][s1; [*C@3 member]-|Member "
"variable of T.&][s1; [*C@3 relation]-|Ordering relation for [* member].&][s1; [*/ "
"Return value]-|Ordering predicate.&][s13; template<class [*@4 O], class [*@4 T], "
"class [*@4 R]>&][s14;:`:`:MethodRelation`(O`(T`:`:`*`)`(`)`,const R`&`): [/ type] "
"[* MethodRelation]([*@4 O] ([*@4 T]`::`*[*@3 method])(), const [*@4 R]`& [*@3 relation])&][s6; "
"Creates ordering predicate for [* T] based on the value returned by non`-const method "
"of [* T].&][s1; [*C@4 T]-|Type of element.&][s1; [*C@3 method]-|Method of T.&][s1; "
"[*C@3 relation]-|Ordering relation for value returned by method.&][s1; [*/ Return "
"value]-|Ordering predicate.&][s13; template")
TOPIC_TEXT(
"<class [*@4 O], class [*@4 T], class [*@4 R]>&][s14;:`:`:MethodRelation`(O`(T`:`:`*`)`(`)const`,const "
"R`&`): [/ type] [* MethodRelation]([*@4 O] ([*@4 T]`::`*[*@3 method])() const, const "
"[*@4 R]`& [*@3 relation])&][s6; Creates ordering predicate for [* T] based on the "
"value returned by const method of [* T].&][s1; [*C@4 T]-|Type of element.&][s1; [*C@3 "
"method]-|Method of T.&][s1; [*C@3 relation]-|Ordering relation for value returned "
"by method.&][s1; [*/ Return value]-|Ordering predicate.]")
