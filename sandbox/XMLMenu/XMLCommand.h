#ifndef _XMLCommand_h_
#define _XMLCommand_h_

#include <CtrlCore/CtrlCore.h>

NAMESPACE_UPP

// a single command, i.e. available entry for menu and bars
class XMLCommand
{
	private:
		// embedded control, if any
		Ptr<Ctrl> control;
		
		// associated callback, if any
		Callback callback;
		
		// enabled flag
		bool enabled;
		
		// custom command flag
		bool custom;

	public:
		Ctrl *GetCtrl(void);
		Callback &GetCallback(void);
		bool GetIsEnabled(void);
		bool GetIsCustom(void);
		
		bool operator==(XMLCommand &other) const;
};

// an array of available commands
class XMLCommands
{
	private:
		ArrayMap<String, XMLCommand> commands;

	public:
		// adds a custom command
		XMLCommands &Add(String const &id);
		
		// adds a built-in command with given callback
		XMLCommands &Add(String const &id, Callback cb);
		
		// adds a control
		XMLCommands &Add(String const &id, Ctrl &ctrl);
		
		// adds a custom command, allows enable/disable item
		XMLCommands &Add(bool enabled, String const &id);
		
		// adds a built-in command with given callback, allows enable/disable item
		XMLCommands &Add(bool enabled, String const &id, Callback cb);
		
		// adds a control, allows enable/disable item
		XMLCommands &Add(bool enabled, String const &id, Ctrl &ctrl);

		// get all available command IDs
		Array<String> const &GetIds(void);
		
		// get a command for a given id
		XMLCommand &GetCommand(String const &id);
		
		// sets the commands by a callback
		void Set(Callback1<XMLCommands &> commands);
};

END_UPP_NAMESPACE

#endif
