#ifndef _XMLMenuEditor_h_
#define _XMLMenuEditor_h_

#include <CtrlLib/CtrlLib.h>

#include "XMLMenuInterface.h"

NAMESPACE_UPP

#define LAYOUTFILE <XMLMenu/XMLMenuEditor.lay>
#include <CtrlCore/lay.h>

class XMLMenuEditor : public WithXMLMenuEditorLayout<TopWindow>
{
	private :
		// interface to access menu system data
		XMLMenuInterface *iFace;
	
	protected :
	
		void cancelCb(void);
		void okCb(void);
		
		// gets commands from iFace and fills the command list
		void GetCommands(void);
	
	public :
	
		typedef XMLMenuEditor CLASSNAME;
		
		XMLMenuEditor(XMLMenuInterface *iFace);
		
		~XMLMenuEditor();
		
};

END_UPP_NAMESPACE

#endif
