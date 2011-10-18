#include "XMLCommand.h"

NAMESPACE_UPP

////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
// adds a custom command
XMLCommands &XMLCommands::Add(String const &id)
{
	XMLCommand *cmd = new XMLCommand;
	cmd->control = NULL;
	cmd->enabled = true;
	cmd->custom = true;
	cmd->callback.Clear();
	commands.Add(id, cmd);
	return *this;
}

// adds a built-in command with given callback
XMLCommands &XMLCommands::Add(String const &id, Callback cb)
{
	XMLCommand *cmd = new XMLCommand;
	cmd->control = NULL;
	cmd->enabled = true;
	cmd->custom = false;
	cmd->callback = cb;
	commands.Add(id, cmd);
	return *this;
}

// adds a control
XMLCommands &XMLCommands::Add(String const &id, Ctrl &ctrl)
{
	XMLCommand *cmd = new XMLCommand;
	cmd->control = &ctrl;
	cmd->enabled = true;
	cmd->custom = false;
	cmd->callback.Clear();
	commands.Add(id, cmd);
	return *this;
}

// adds a custom command, allows enable/disable item
XMLCommands &XMLCommands::Add(bool enabled, String const &id)
{
	XMLCommand *cmd = new XMLCommand;
	cmd->control = NULL;
	cmd->enabled = enabled;
	cmd->custom = true;
	cmd->callback.Clear();
	commands.Add(id, cmd);
	return *this;
}

// adds a built-in command with given callback, allows enable/disable item
XMLCommands &XMLCommands::Add(bool enabled, String const &id, Callback cb)
{
	XMLCommand *cmd = new XMLCommand;
	cmd->control = NULL;
	cmd->enabled = enabled;
	cmd->custom = false;
	cmd->callback = cb;
	commands.Add(id, cmd);
	return *this;
}

// adds a control, allows enable/disable item
XMLCommands &XMLCommands::Add(bool enabled, String const &id, Ctrl &ctrl)
{
	XMLCommand *cmd = new XMLCommand;
	cmd->control = &ctrl;
	cmd->enabled = true;
	cmd->custom = false;
	cmd->callback.Clear();
	commands.Add(id, cmd);
	return *this;
}

// get all available command IDs
Vector<String> const &XMLCommands::GetIds(void) const
{
	return commands.GetKeys();
}

struct XMLCmdLess
{
	bool operator()(String const &a, String const &b) const
		{ return ToUpper(a) < ToUpper(b); }
};
	
// sort items - alphabetically, but first built-in commands, then custom ones
XMLCommands &XMLCommands::Sort(void)
{
	Array<XMLCommand> builtIn, custom;
	Array<String> builtInIdx, customIdx;
	for(int i = 0; i < commands.GetCount(); i++)
	{
		if(commands[i].GetIsCustom())
		{
			custom.Add(commands[i]);
			customIdx.Add(commands.GetKey(i));
		}
		else
		{
			builtIn.Add(commands[i]);
			builtInIdx.Add(commands.GetKey(i));
		}
	}
	IndexSort(customIdx, custom, XMLCmdLess());
	IndexSort(builtInIdx, builtIn, XMLCmdLess());
	commands.Clear();
	for(int i = 0; i < builtIn.GetCount(); i++)
		commands.Add(builtInIdx[i], builtIn[i]);
	for(int i = 0; i < custom.GetCount(); i++)
		commands.Add(customIdx[i], custom[i]);
	
	return *this;
}
		
END_UPP_NAMESPACE