#include "SystemDraw.h"


NAMESPACE_UPP

#define LLOG(x)

void OpenGLFont::Load(const String& fileName)
{
	String filePath = GetDataFile(fileName);
	String xml = LoadFile(filePath);
	XmlParser p(xml);

	while(!p.IsTag())
		p.Skip();

	p.PassTag("font");
	
	while(!p.End())
	{
		if(p.TagE("common"))
		{
			scaleW = p.Int("scaleW");
			scaleH = p.Int("scaleH");
			lineHeight = p.Int("lineHeight");
			base = p.Int("base");
		}
		else if(p.Tag("pages"))
		{
			while(!p.End())
			{
				if(p.TagE("page"))
				{
					String fileName = p["file"];
					files.Add(fileName);
					//Image img = StreamRaster::LoadFileAny(GetDataFile(fileName));
					//int serialId = Resources::Bind(img);
					pages.Add(-1);
				}
				else
					p.Skip();
			}
			
		}
		else if(p.Tag("chars"))
		{
			while(!p.End())
			{
				if(p.TagE("char"))
				{
					CharInfo& ci = chars.Add();
					
					int page = p.Int("page");
				
					ci.id = p.Int("id");
					ci.x = p.Int("x");
					ci.y = p.Int("y");
					ci.width = p.Int("width");
					ci.height = p.Int("height");
					ci.xoffset = p.Int("xoffset");
					ci.yoffset = p.Int("yoffset");
					ci.xadvance = p.Int("xadvance");
					ci.page = page;
				}
				else
					p.Skip();
			}
		}
		else if(p.Tag("kernings"))
		{
			while(!p.End())
			{
				if(p.TagE("kerning"))
				{
					int first = p.Int("first"); 
					int second = p.Int("second");
					int amount = p.Int("amount");
					
					VectorMap<int, int>& vm = kerns.GetAdd(first);
					vm.Add(second, amount);
				}
				else
					p.Skip();
			}
		}		
		else
			p.Skip();
	}
}

void OpenGLFont::UpdateTextures()
{
	if(texturesUpdated)
		return;
				
	for(int i = 0; i < files.GetCount(); i++)
	{
		Image img = StreamRaster::LoadFileAny(GetDataFile(files[i]));
		int64 serialId = Resources::Bind(img, false);
		pages[i] = serialId;
	}
	
	texturesUpdated = true;
}

void OpenGLDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink, int n, const int *dx)
{
	if(!text)
		return;
	
	const wchar* s = text;	
	OpenGLFont& fi = Resources::StdFont(font.IsBold());
	
	fi.UpdateTextures();	

	glColor4ub(ink.GetR(), ink.GetG(), ink.GetB(), (int) alpha);
	glEnable(GL_TEXTURE_2D);
	
	#if CLIP_MODE == 3
	float cl = (float) clip.left;
	float ct = (float) clip.top;
	float cr = (float) clip.right;
	float cb = (float) clip.bottom;
	#endif
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	while(*s && n > 0)
	{
		int ch = *s;
		int cn = ch - 32;
		
		if(cn >= 0 && cn < fi.chars.GetCount())
		{
			const OpenGLFont::CharInfo& ci = fi.chars[cn];
	
			cn <<= 3;
	
			Resources::Bind(fi.pages[ci.page], true);

			float sx = (float) ci.xoffset + x + drawing_offset.x;
			float sy = (float) ci.yoffset + y + drawing_offset.y;
			float dx = sx + ci.width;
			float dy = sy + ci.height;

			#if CLIP_MODE == 3
			if(sx <= clip.right && sy <= clip.bottom && dx >= clip.left && dy >= clip.top)
			#endif
			{
				float tl = (float) ci.x;
				float tt = (float) ci.y;
				float tr = (float) ci.x + ci.width;
				float tb = (float) ci.y + ci.height;
				
				float sw = (float) fi.scaleW;
				float sh = (float) fi.scaleH;
				
				#if CLIP_MODE == 3
				if(sx < cl)
				{
					tl += (cl - sx);
					sx = cl;
				}
				
				if(sy < ct)
				{
					tt += (ct - sy);
					sy = ct;
				}
				
				if(dx > cr)
				{
					tr -= dx - cr;
					dx = cr;
				}
				
				if(dy > cb)
				{
					tb -= dy - cb;
					dy = cb;
				}
				#endif
				
				float tw = 1.f / sw;
				float th = 1.f / sh;
				
				tl *= tw;
				tt *= th;
				tr *= tw;
				tb *= th;
		
				float vtx[] = {
					sx, dy,
					sx, sy,
					dx, dy,
					dx, sy
				};
			
				float crd[] = {
					tl, tb,
					tl, tt,
					tr, tb,
					tr, tt
				};
				//SetVec(vtx, sx, sy, dx, dy);
				//SetVec(crd, tl, tt, tr, tb);

				glTexCoordPointer(2, GL_FLOAT, 0, crd);
				glVertexPointer(2, GL_FLOAT, 0, vtx);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}
	
			x += ci.xadvance;
			
			int k = fi.kerns.Find(*s);
			if(k >= 0)
				x += fi.kerns[k].Get(*(s + 1), 0);
		}
		
		++s;
		--n;
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

Size GetTextSize(const wchar *text, const OpenGLFont& fi, int n)
{
	if(n < 0)
		n = wstrlen(text);
	Size sz;
	sz.cx = 0;
	const wchar *wtext = (const wchar *)text;
	while(n > 0) {
		int ch = *wtext++;
		int cn = ch - 32;
		
		if(cn >= 0 && cn < fi.chars.GetCount())
		{
			const OpenGLFont::CharInfo& ci = fi.chars[cn];
			sz.cx += ci.xadvance;
			int k = fi.kerns.Find(ch);
			if(k >= 0)
				sz.cx += fi.kerns[k].Get(*wtext, 0);
		}
		n--;
	}
	sz.cy = fi.lineHeight;
	return sz;
}

END_UPP_NAMESPACE