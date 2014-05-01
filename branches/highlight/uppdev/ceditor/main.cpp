#include <CodeEditor/CodeEditor.h>

using namespace Upp;

GUI_APP_MAIN {
	TopWindow win;
	CodeEditor editor;
	
	win.Add(editor.SizePos());
	win.Run();
}