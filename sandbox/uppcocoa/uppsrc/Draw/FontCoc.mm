//#include <CtrlCore/CtrlCore.h>
#define PLATFORM_COCOA 1
#ifdef PLATFORM_COCOA
#include "Draw.h"
#include <UNS/UNS.h>


#ifdef PLATFORM_COCOA

NAMESPACE_UPP

#define LLOG(x)       LOG(x)
#define LTIMING(x)   RTIMING(x)

void GetStdFontSys(String& name, int& height)
{
	name = "Arial";
	height = 12;
}


class XftNSFont : public NSFont
{
public:
	XftNSFont();
};
//static FT_Library sFTlib;

//bool sInitFt(void)
//{
////	if(sFTlib)
////		return true;
////	return FT_Init_FreeType(&sFTlib) == 0;
//	return false;	
//}

#define STYLE_REF 100

NSFontDescriptor *CreateFcPattern(Font font)
{
	LTIMING("CreateFcPattern");
//	double sina, cosa;

	int hg = abs(font.GetHeight());
	if(hg == 0) hg = 10;
	String face = font.GetFaceName();
		NSString *family = [NSString stringWithUTF8String:font.GetFaceName()];
//	NSFontDescriptor *p = [[NSFontDescriptor alloc] initWithFontAttributes:nil];
	NSFontDescriptor *p = [NSFontDescriptor fontDescriptorWithName:[NSString stringWithUTF8String:font.GetFaceName()] size:(CGFloat)hg];
//	p = [p fontDescriptorWithSize:(CGFloat)hg];
//	p = [p fontDescriptorWithName:family];

////	FcPattern *p = FcPatternCreate();
////	FcPatternAddString(p, FC_FAMILY, (FcChar8*)~face);
////	FcPatternAddInteger(p, FC_SLANT, font.IsItalic() ? 110 : 0);
////	FcPatternAddInteger(p, FC_PIXEL_SIZE, hg);

////	FcPatternAddInteger(p, FC_WEIGHT, font.IsBold() ? 200 : 100);
////	FcPatternAddBool(p, FC_MINSPACE, 1);
////	FcConfigSubstitute(0, p, FcMatchPattern);
////	FcDefaultSubstitute(p);
////	FcResult result;
////	FcPattern *m = FcFontMatch(0, p, &result);
////	FcPatternDestroy(p);
//	return m;

	return p;	

//	rteturn NULL;

	NSFontDescriptor *fdesc;
	NSMutableDictionary *fdAttrs = [NSMutableDictionary new];
	NSMutableDictionary *tdict = [NSMutableDictionary new];
	
//	NSString *family = ns_get_family(font_spec);


	float n;

	/* add each attr in font_spec to fdAttrs.. */
	n = min(font.IsBold() ? 200 : 100, 200);

	if (n != -1 && n != STYLE_REF)
		[tdict setObject: [NSNumber numberWithFloat: (n - 100.0) / 100.0] forKey: NSFontWeightTrait];

	n = min(font.IsItalic() ? 110 : 0, 200);

	if (n != -1 && n != STYLE_REF)
		[tdict setObject: [NSNumber numberWithFloat: (n - 100.0) / 100.0] forKey: NSFontSlantTrait];

//	n = min(FONT_WIDTH_NUMERIC(font_spec), 200);
//
//	if (n > -1 && (n > STYLE_REF + 10 || n < STYLE_REF - 10))
//		[tdict setObject: [NSNumber numberWithFloat: (n - 100.0) / 100.0] forKey: NSFontWidthTrait];

	if ([tdict count] > 0)
		[fdAttrs setObject: tdict forKey: NSFontTraitsAttribute];

	fdesc = [NSFontDescriptor fontDescriptorWithFontAttributes: fdAttrs];

	if (family != nil)
		fdesc = [fdesc fontDescriptorWithFamily: family];

	return fdesc;

}

//FT_Face CreateFTFace(const FcPattern *pattern, String *rpath) {
//	FT_Face	    face = NULL;
//
//	int		    id;
//	double	    dsize;
//	double	    aspect;
//	FcChar8    *filename;
//
//	if(!sInitFt())
//		return NULL;
//
////	if(FcPatternGetString(pattern, FC_FILE, 0, &filename) != FcResultMatch)
////		return NULL;
////	if(rpath)
////		*rpath = (const char *)filename;
////
////	if(FcPatternGetDouble(pattern, FC_PIXEL_SIZE, 0, &dsize) != FcResultMatch)
////		dsize = 16;
////
////	if (FcPatternGetDouble(pattern, FC_ASPECT, 0, &aspect) != FcResultMatch)
////		aspect = 1.0;
////
////	FT_F26Dot6 ysize = (FT_F26Dot6) (dsize * 64.0);
////	FT_F26Dot6 xsize = (FT_F26Dot6) (dsize * aspect * 64.0);
////
////	if(FT_New_Face(sFTlib, (const char *)filename, 0, &face))
////		return NULL;
////
////	FT_Set_Char_Size(face, xsize, ysize, 0, 0);
//	return face;
//}

#if 0
FT_Face CreateFTFace(const NSFontDescriptor *pattern, NSFont *nsfnt) {
	FT_Face	    face = NULL;
	
	//........
	return face;
}
#endif

#define FONTCACHE 37//96//37

struct CocFaceEntry {
	Font    font;
	NSFont *hfont;
//	CocFace face;
//	String  path;
};

static CocFaceEntry coc_font_cache[FONTCACHE];

void ClearCocFaceCache()
{
	LLOG("==================================================ClearCocFaceCache");
	for(int i = 0; i < FONTCACHE; i++)
	{
		coc_font_cache[i].font.Height(-30000);  //it's just randon int init clearance
	//	[coc_font_cache[i].hfont release];
	}
}

#if 0
//DrawText11.cpp
//XftFont *GetXftFont(Font fnt, int angle)
//{

FT_Face (*FTFaceXft)(Font fnt, NSFont *nsfnt);
//@font-face is a css rule which allows you to download a particular font from your server to render a webpage if the user hasn't got that font installed.
// This means that web designers will no longer have to adhere to a particular set of "web safe" fonts that the user has pre-installed on their computer.
FT_Face FTFace(Font fnt, NSFont *nsfnt = NULL)
{
	LTIMING("FTFace");
	LLOG("======================================================FTFace="<<fnt);
	if(FTFaceXft)
		return (*FTFaceXft)(fnt, nsfnt);
	ONCELOCK {
		ClearFtFaceCache();
	}
	FtFaceEntry be;
	be = ft_cache[0];
	for(int i = 0; i < FONTCACHE; i++) {
		FtFaceEntry e = ft_cache[i];
		if(i)
			ft_cache[i] = be;
		if(e.font == fnt) {
			if(rpath)
				*rpath = e.path;
			if(i)
				ft_cache[0] = e;
			return e.face;
		}
		be = e;
	}
	LTIMING("FTFace2");
	if(be.face) {
		LLOG("Removing " << be.font << " - " << (void *)be.face);
//		FT_Done_Face(be.face);
	}
	be.font = fnt;
	FcPattern *p = CreateFcPattern(fnt);
	be.face = CreateFTFace(p, &be.path);
//	FcPatternDestroy(p);
	ft_cache[0] = be;
	if(rpath)
		*rpath = be.path;
	return be.face;
}
#endif
//GetCocFont
//NSFont (*UNSFont)(Font fnt);

//#ifdef __APPLE__

NSFont *UNSFont(Font fnt)
{
	LTIMING("UNSFont");
//	LLOG("----------------NSFont *UNSFont="<<fnt);
//	__BREAK__;
//	NSString *fontName;
//	WString fontName = WString(fnt.GetFaceName());
//	fontName = (wchar*)ws;
//	fnt.GetLineHeight()

//	if(UNSFont)
//		return (*UNSFont)(fnt);//, nsfnt);

	static CocFaceEntry cache[FONTCACHE];
	ONCELOCK {
		ClearCocFaceCache();
	}
//	LLOG("family01 ="<<fnt.GetFaceName());
//
	CocFaceEntry be;
	be = cache[0];
	for(int i = 0; i < FONTCACHE; i++) {
		CocFaceEntry e = cache[i];
	//	LLOG("family02 i="<<i<<" fnt="<<fnt);
		if(i)
		{
//			LLOG("family002 i======= "<<i<<" ===="<<e.font.GetFaceName()<<"====???===="<<fnt.GetFaceName());
			cache[i] = be;  //0 element from before the for loop!
		}
		if(e.font == fnt) // && e.angle == angle) {
		{
			if(i)
				cache[0] = e; //current i to 0 element
//			LLOG("family03 ==============good==========="<<fnt.GetFaceName());
			return e.hfont;
		}
	//	LLOG("family04 ="<<fnt.GetFaceName());
		be = e;
//		LLOG("family05 ="<<fnt.GetFaceName());
	}

////	LLOG("family06 ="<<fnt.GetFaceName());
//	LLOG("family07 ="<<fnt.GetFaceName());
//
	be.font = fnt;
//	UppCocAutoReleasePool mpool; 
//		[UNSApplication sharedApplication]; on gnustep font manager require shared app to be running..
	NSFontManager *fontManager = [NSFontManager sharedFontManager];
//	LLOG("family="<<fnt.GetFaceName());
	NSString *family = [NSString stringWithUTF8String:fnt.GetFaceName()];
//	NSFont *myFont;
	
//	CFStringRef fntName =(CFStringRef) (const char*)WString(fnt.GetFaceName());
//	[fontManager fontWithFamily:[NSString stringWithUTF8String:fnt.GetFaceName()] //UNSString("Arial")

	//correct?
//   NSFontDescriptor *font1 = [NSFontDescriptor fontDescriptorWithName:[NSString stringWithUTF8String:fnt.GetFaceName()] size: abs(fnt.GetHeight())];
    LLOG("family08 ="<<fnt.GetFaceName());
//	NSFontDescriptor *desc = CreateFcPattern(fnt);

//	NSArray *afonts = [fontManager availableFontNamesMatchingFontDescriptor:CreateFcPattern(fnt)];
//	NSArray *afonts = [fontManager availableMembersOfFontFamily:family];
//	if(afonts!=nil)
	
//	NSLog(@"afonts= %@", afonts);
//	NSFont *hfont = [NSFont fontWithDescriptor: CreateFcPattern(fnt)
//	                                      size:  ( abs(fnt.GetHeight()) )];                  
	int hg = abs(fnt.GetHeight());
	if(hg==0) hg = 10;
	NSFont *hfont = [fontManager fontWithFamily:family //UNSString("Arial")	 
	                                      traits:0// NSUnBoldFontMask //|NSItalicFontMask
	                                      weight:5
	                                       size:hg]; 
//	LLOG("family08 be.hfont="<<CocUString([be.hfont fontName]));
	if(hfont == nil)
	{
		LLOG("!!!!!!!!!==!!!!!!!!!!!!!NO FONT!!!!!!!!!!!=!!!!!!!!!!!!!");
		//check "Corsiva Hebrew"!!!
		//CocAlert("no font in *UNSFont(Font font)",fnt.GetFaceName());
		be.font = fnt;  //FIXME
	}
	else
	{
		[be.hfont release]; //CHECK!!!
		be.hfont = hfont;
		[be.hfont retain]; //CHECK!!!
	}
//	[fontManager release];
//	[pool release];
//	cache[0] = be;
	LLOG("//family ="<<fnt.GetFaceName());
	cache[0] = be;
	return  be.hfont; //   hfont;
}

Size CocGetTextSize(const wchar *text,  Font fnt, int n)
{
	LTIMING("CocGetTextSize");
	WString s;
	s.Cat(text, n);
	LLOG("CocGetTextSize fnt="<<fnt<<" text="<<WString(s).ToString());
//	if (fnt.GetFaceName() =="STDFONT")
//		__BREAK__;
	NSFont *myFont = UNSFont(fnt);

	NSString *myString = [NSString stringWithUTF8String:WString(s).ToString()];

//	NSDictionary *attributes = [NSDictionary dictionaryWithObjectsAndKeys:
//	               myFont, NSFontAttributeName, UNSColor(color), NSForegroundColorAttributeName, nil];
	NSDictionary *attributes = [NSDictionary dictionaryWithObject:myFont forKey: NSFontAttributeName];
	NSSize nsz = [myString sizeWithAttributes:attributes];
	return Size(nsz.width, nsz.height);
#if 0
	NSTextStorage *textStorage = [[[NSTextStorage alloc] initWithString:myString] autorelease];
	NSTextContainer *textContainer = [[[NSTextContainer alloc] initWithContainerSize:NSMakeSize(myWidth, FLT_MAX)] autorelease];
	NSLayoutManager *layoutManager = [[[NSLayoutManager alloc] init] autorelease];
	[layoutManager addTextContainer:textContainer];
	[textStorage addLayoutManager:layoutManager];
	[textStorage addAttribute:NSFontAttributeName value:myFont range:NSMakeRange(0, [textStorage length])];
	[textContainer setLineFragmentPadding:0.0];

	(void) [layoutManager glyphRangeForTextContainer:textContainer];
	NSSize nsz = [layoutManager usedRectForTextContainer:textContainer].size;
	return Size(nsz.width, nsz.height);
#endif
//#if __APPLE__

//[@"Label" sizeWithAttributes: [NSDictionary dictionaryWithObject: [NSFont fontWithName: @"Lucida Grande" size: 13] forKey: NSFontAttributeName]].width
//	NSString *cstr = [NSString stringWithFormat: @"%c", c];
//	return max(2.0, [cstr sizeWithAttributes:[NSDictionary dictionaryWithObject:sfont forKey:NSFontAttributeName]].width);
//#else
	/* deprecated in OS X 10.4 */
//	return = max(2.0, [sfont widthOfString: cstr]);
//#endif
}


Size GetTextSize(const wchar *text, Font font, int n)
{
	LLOG("GetTextSize font"<<font);
//	WString s;
//	s.Cat(text, n);
//	LLOG("CocGetTextSize fnt="<<font<<" text="<<WString(s).ToString());
////	if (fnt.GetFaceName() =="STDFONT")
////		__BREAK__;
//	NSFont *myFont = UNSFont(font);
//
//	NSString *myString = [NSString stringWithUTF8String:WString(s).ToString()];
//
////	NSDictionary *attributes = [NSDictionary dictionaryWithObjectsAndKeys:
////	               myFont, NSFontAttributeName, UNSColor(color), NSForegroundColorAttributeName, nil];
//	NSDictionary *attributes = [NSDictionary dictionaryWithObject:myFont forKey: NSFontAttributeName];
//	NSSize nsz = [myString sizeWithAttributes:attributes];
//	return Size(nsz.width, nsz.height);

//////	
	FontInfo fi = font.Info();
	if(n < 0)
		n = wstrlen(text);
	Size sz;
	sz.cx = 0;
	const wchar *wtext = (const wchar *)text;
	while(n > 0) {
		sz.cx += fi[*wtext++];
		LLOG("GetTextSize +fi="<<fi[*wtext++]);
		n--;
	}
	sz.cy = fi.GetHeight();
	LLOG("GetTextSize sz="<<sz.cx<<" :: "<<sz.cy);
	return sz;
//		LLOG("fontinfo ="<<fi);
//	return Size(22, 18);
//	return GetTextSize(text, Resources::GetFont(font), n); 
//	return CocGetTextSize(text, font, n); 
}
//#else
//Size GetTextSize(const wchar *text, const CocFont& fi, int n)
//{
//	return Size(12,12);
//}
//#endif

CommonFontInfo (*GetFontInfoSysCoc)(Font font);

CommonFontInfo GetFontInfoSys(Font font)
{
//	sInitFt();
	LLOG("CommonFontInfo GetFontInfoSys 01="<<font);
//	if(GetFontInfoSysCoc)
//		return (*GetFontInfoSysCoc)(font);

	CommonFontInfo fi;
		memset(&fi, 0, sizeof(fi));
//	String path;
//	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
//	FT_Face face = FTFace(font, &path);
	LLOG("CommonFontInfo GetFontInfoSys 02="<<font);
	NSFont *cocfont = UNSFont(font); //was working in variant 1!!!
	
	LLOG("CommonFontInfo GetFontInfoSys 03="<<font);
	if(cocfont) {
		fi.ascent =(int)[cocfont ascender];
			LLOG("CommonFontInfo GetFontInfoSys fi.ascent="<<fi.ascent);
		fi.descent = -(int)[cocfont descender];
			LLOG("CommonFontInfo GetFontInfoSys fi.descent="<<fi.descent);

		fi.height = fi.ascent + fi.descent;
		fi.lineheight = fi.height+3;  //FIXME it should be  + line gap(leading) or get it from NSLayoutManager?
		fi.external = 0; //check
		fi.internal = 0; //check
		fi.overhang = 0; //check
		NSSize maxAdvancement = [cocfont maximumAdvancement];	
		fi.maxwidth = maxAdvancement.width;
		fi.avewidth = (int)(fi.maxwidth/2);
		fi.default_char = '?'; //check
		fi.fixedpitch = font.GetFaceInfo() & Font::FIXEDPITCH;
////		if(path.GetCount() < 250)
////			strcpy(fi.path, ~path);
////		else
////			*fi.path = 0;
	}
//	[pool release];
	return fi;
}

#define FLOOR(x)    ((x) & -64)
#define CEIL(x)	    (((x)+63) & -64)
#define TRUNC(x)    ((x) >> 6)
#define ROUND(x)    (((x)+32) & -64)

//GlyphInfo (*GetGlyphInfoSysXft)(Font font, int chr);

GlyphInfo  GetGlyphInfoSys(Font font, int chr)
{
//	LTIMING("GetGlyphInfoSys");
	LLOG("===============0 ===========GetGlyphInfoSys font="<<font<< "chr="<<(char)chr);
	GlyphInfo gi;
	
	NSFont *face = UNSFont(font);
	LLOG("============== 1 =========GetGlyphInfoSys font="<<font<< "chr="<<(char)chr);	
	gi.lspc = gi.rspc = 0;
	gi.width = 0x8000;
	
//	- (unsigned)getGlyphsInRange:(NSRange)glyphsRange glyphs:(NSGlyph
//	*)glyphBuffer characterIndexes:(unsigned *)charIndexBuffer
//	glyphInscriptions:(NSGlyphInscription *)inscribeBuffer elasticBits:(BOOL
//	*)elasticBuffer

	if(face != nil)
	{
//		LTIMING("GetGlyphInfoSys 2");
//		int glyph_index = FT_Get_Char_Index(face, chr);
		String s;
		//s<<  (char)chr;
		s="s";
//		NSString * glyphName = [NSString stringWithUTF8String:s.Begin()];
//		NSGlyph glyph = [face glyphWithName:@"g"];
//		NSSize maxAdvancement = [face maximumAdvancement];	
//		unsigned int numGlyphs = [face numberOfGlyphs];
//		if(glyph)
//			NSLog(@"glyph OK" );
//		else
//			NSLog(@"glyph SHIT" );
			
//		if(glyph_index) {
//			if(GetGlyphInfoSysXft)
//				return (*GetGlyphInfoSysXft)(font, chr);
//			if(FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT|FT_LOAD_NO_BITMAP) == 0 ||
//			   FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT) == 0) {
//				FT_Glyph_Metrics& m = face->glyph->metrics;
//				int left  = FLOOR(m.horiBearingX);
//				int width = TRUNC(CEIL(m.horiBearingX + m.width) - left);				
				//gi.width = TRUNC(ROUND((int)maxAdvancement.width));
			//	if(chr==0)
//				if(chr != '\000' && chr!= '\020')
				{	
    				
				LLOG("===============21==========GetGlyphInfoSys chr="<<(char)chr);

					//NSString *cstr = [NSString stringWithFormat: @"%C", str[0]];
					NSString *cstr = [NSString stringWithUTF8String:String((char)chr,1)];
				LLOG("===============212==========GetGlyphInfoSys chr="<<(char)chr);
			//	NSLog(@"===============212========== %@", cstr);	
					
	gi.width = max(2.0, (double)[cstr sizeWithAttributes:[NSDictionary dictionaryWithObject:face forKey:NSFontAttributeName]].width);
			
//				gi.width = ns_char_width([face screenFont], chr); //((int)maxAdvancement.width/2);  //FIXME!!!
			//	gi.width = ns_char_width(face, chr); //((int)maxAdvancement.width/2);  //FIXME!!!
				LLOG("===============22==========GetGlyphInfoSys gi.width="<<gi.width<< "chr="<<(char)chr);				
				}
//				else
//					gi.width = 2;
					
				gi.lspc = 0;//[face boundingRectForFont].origin.x;  //guess!
				LLOG("===============23==========GetGlyphInfoSys gi.width="<<gi.lspc<< "chr="<<(char)chr);

//				gi.lspc = TRUNC(left);
//				gi.rspc = gi.width - width - gi.lspc;
				gi.rspc = 2;

//			}
//		}
	}
	else
	{
		LLOG("shit font="<<font.GetFaceName());
	//	CocAlert("shit font=",font.GetFaceName());
	}
	return gi;
}

Vector<FaceInfo> GetAllFacesSys()
{
	LLOG("Vector<FaceInfo> GetAllFacesSys");
//	CocAlert("Vector<FaceInfo> GetAllFacesSys");
//	static const char *basic_fonts[] = {
//		"sans-serif",
//		"serif",
//		"sans-serif",
//		"monospace",
//	};
//	
	Vector<FaceInfo> list;
//	for(int i = 0; i < __countof(basic_fonts); i++) {
//		FaceInfo& fi = list.Add();
//		fi.name = basic_fonts[i];
//		fi.info = (i == 3) ? Font::SCALEABLE|Font::FIXEDPITCH : Font::SCALEABLE;
//	}
	
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	NSMutableArray	*fFontArray;
	NSString *family = NULL;
	
	[NSApplication sharedApplication];
		
	NSFontManager *fontManager = [NSFontManager sharedFontManager];
	NSEnumerator *families = [[fontManager availableFontFamilies] objectEnumerator];
//	[family UTF8String]
	while (family = [families nextObject])
	{
		FaceInfo& fi = list.Add();
		fi.name = [family UTF8String];
		fi.info = 0;
		NSFont *font  = [fontManager fontWithFamily:family traits:0 weight:0 size:0];
		if([font isFixedPitch])
			fi.info |= Font::FIXEDPITCH;
		fi.info |= Font::SCALEABLE; //maybe I'm wrong but... all apple fonts scalable?
	}

//	FcPattern *p = FcPatternCreate();
//	FcObjectSet *os = FcObjectSetBuild(FC_FAMILY, FC_SPACING, FC_SCALABLE, (void *)0);
//	FcFontSet *fs = FcFontList(NULL, p, os);
//	FcPatternDestroy(p);
//	FcObjectSetDestroy(os);
//	for(int i = 0; i < fs->nfont; i++) {
//		FcChar8 *family = NULL;
//		if(FcPatternGetString(fs->fonts[i], FC_FAMILY, 0, &family) == 0 && family) {
//			FaceInfo& fi = list.Add();
//			fi.name = (const char *)family;
//			fi.info = 0;
//			int iv;
//			if(FcPatternGetInteger(fs->fonts[i], FC_SPACING, 0, &iv) == 0 && iv == FC_MONO)
//				fi.info |= Font::FIXEDPITCH;
//			FcBool bv;
//			if(FcPatternGetBool(fs->fonts[i], FC_SCALABLE, 0, &bv) == 0 && bv)
//				fi.info |= Font::SCALEABLE;
//		}
//	}
//	FcFontSetDestroy(fs);
	[pool release];
	return list;
}




END_UPP_NAMESPACE

#endif
#endif
