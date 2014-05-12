#include "CodeEditor.h"

NAMESPACE_UPP

ArrayMap<String, SyntaxDef>& EditorSyntax::defs()
{
	static ArrayMap<String, SyntaxDef> d;
	return d;
}

void EditorSyntax::Register(const char *id, Gate1<One<EditorSyntax>&> factory, const char *exts, const char *description)
{
	SyntaxDef& f = defs().GetAdd(id);
	f.factory = factory;
	f.exts = exts;
	f.description = description;
}

One<EditorSyntax> EditorSyntax::Create(const char *id)
{
	One<EditorSyntax> s;
	SyntaxDef *f = defs().FindPtr(id);
	if(f)
		f->factory(s);
	if(!s)
		s.Create();
	return s;
}

String EditorSyntax::GetSyntaxForExtension(const char *ext)
{
	return Null;
}

END_UPP_NAMESPACE
