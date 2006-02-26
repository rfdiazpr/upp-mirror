#ifndef _IdeVectorDes_IdeVectorDes_h_
#define _IdeVectorDes_IdeVectorDes_h_

#include <VectorDes/VectorDes.h>
#include <ide/Common/Common.h>

class VectorDesigner : public IdeDesigner, public VectorDesHost
{
	One<VectorDes> designer;

public:
	VectorDesigner()                               { designer = new VectorDes(*this); Add(designer->SizePos()); }
	~VectorDesigner()                              { Shutdown(); }

	virtual void   Close()                         { designer->CloseDesigner(); }
	virtual void   ChildGotFocus()                 { if(!IsShutdown()) designer->FrameFocus(); }
	virtual bool   Key(dword key, int repcnt)      { return designer->Key(key, repcnt); }
	virtual String GetFileName() const             { return filename; }
	virtual void   Save();
	virtual void   SyncUsc()                       {}
	virtual void   ActivateDesigner()              { SetForeground(); }
	virtual void   SaveEditPos();
	virtual void   EditMenu(Bar& menu)             { designer->EditMenu(menu); }

	void           Serialize(Stream& s)            { designer->Serialize(s); }
	bool           Load(const char *fn);
	void           SetConfigName(const char *name) { cfgname = name; }

	static void    SerializeEditPos(Stream& stream);

	struct EditPos {
		Time filetime;

		void Serialize(Stream& stream);
	};

private:
	String         cfgname;
	String         filename;
};

VectorDesigner *CreateVectorDesigner(const char *filename, const char *cfgname, bool topwindow);
void            RegisterIdeVectorDes();

#endif
