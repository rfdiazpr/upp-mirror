#ifndef _ide_Methods_h_
#define _ide_Methods_h_

#include "ide.h"

NAMESPACE_UPP

class TextOption : public Option {
public:
	virtual void   SetData(const Value& data);
	virtual Value  GetData() const;
};

class TextSwitch : public Switch {
public:
	virtual void   SetData(const Value& data);
	virtual Value  GetData() const;
};

class DirTable : public ArrayCtrl {
public:
	virtual void   SetData(const Value& data);
	virtual Value  GetData() const;

protected:
	void Modify()  { Update(); }

	EditString      edit;
	SelectDirButton edit_dir;

	void Init(const char *name = NULL);

public:
	DirTable();
	DirTable(const char *name);
};

class DirMap : public ArrayCtrl {
public:
	virtual void   SetData(const Value& data);
	virtual Value  GetData() const;

protected:
	void Modify()  { Update(); }

	EditString      localpath, remotepath;
	SelectDirButton edit_dir;

public:
	DirMap();
};

class AndroidBuilderSetup :
	public WithBuildMethodsAndroidBuilderSetupLayout<ParentCtrl> {
public:
	AndroidBuilderSetup();
	
	VectorMap<Id, Ctrl*> GetSetupCtrlsMap();

private:
	FrameRight<Button> ndkBrowse;
	FrameRight<Button> jdkBrowse;
};

class DefaultBuilderSetup :
	public WithBuildMethodsDefaultBuilderSetupLayout<ParentCtrl> {
public:
	TextOption debug_blitz;
	TextSwitch debug_linkmode;
	TextOption release_blitz;
	TextSwitch release_linkmode;
	TextOption allow_pch;
	DirTable   path;
	DirTable   include;
	DirTable   lib;
	
public:
	DefaultBuilderSetup();
	
	void New(const String& builder);
	VectorMap<Id, Ctrl*> GetSetupCtrlsMap();
};

class BuilderSetup : public Moveable<BuilderSetup> {
public:
	BuilderSetup(ParentCtrl* setupCtrl, VectorMap<Id, Ctrl*> setupCtrlsMap);
	
	ParentCtrl* setupCtrl;
	VectorMap<Id, Ctrl*> setupCtrlsMap;
};

class BuildMethods : public WithBuildMethodsLayout<TopWindow> {
public:
	OpenFileButton open_script;
	TextOption linkmode_lock;
	
	EditStringNotNull name;
	Index<String>     origfile;
	String            default_method;
	
	VectorMap<String, BuilderSetup> setups;
	
	AndroidBuilderSetup androidSetup;
	DefaultBuilderSetup defaultSetup;
	
	void Load();
	bool Save();

	void NewBuilder();
	void ShowDefault();
	void SetDefault();
	void ChangeMethod();
	void Import();

	void MethodMenu(Bar& bar);
	
	String GetSetupPrefix(const String& setupKey) const;
	void   InitSetups();
	void   SwitchSetupView();
	VectorMap<String, String> SieveBuilderVars(const VectorMap<String, String>& map);
	VectorMap<String, String> MapBuilderVars(const VectorMap<String, String>& map);
	
	typedef BuildMethods CLASSNAME;

	BuildMethods();
};

END_UPP_NAMESPACE

#endif
