//////////////////////////////////////////////////////////////////////
// CalcGuiType: gui-oriented calc types.

void UseCalcGuiType();

template <> struct CalcType<Pointf>        : public CalcRawNullType<Pointf> {};
template <> struct CalcType<const Pointf&> : public CalcRawNullType<Pointf> {};
template <> struct CalcType<Rectf>         : public CalcRawNullType<Rectf>  {};
template <> struct CalcType<const Rectf&>  : public CalcRawNullType<Rectf>  {};
template <> struct CalcType<Color>         : public CalcRawNullType<Color>  {};
template <> struct CalcType<const Color&>  : public CalcRawNullType<Color>  {};
