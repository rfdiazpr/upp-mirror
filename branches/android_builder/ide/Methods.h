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

class BuilderSetup {
public:
	BuilderSetup(const String& prefix_);
	virtual ~BuilderSetup() {}
	
	virtual String GetPrefix() const { return this->prefix; }
	
	virtual VectorMap<Id, Ctrl*> GetSetupCtrlsMap() = 0;
	virtual Index<String> GetCoreIds();
	
protected:
	const String prefix;
};

class AndroidBuilderSetup :
	public WithBuildMethodsAndroidBuilderSetupLayout<TopWindow>,
	public BuilderSetup  {
public:
	AndroidBuilderSetup();
	
	void New();
	virtual VectorMap<Id, Ctrl*> GetSetupCtrlsMap();
};

class DefaultBuilderSetup :
	public WithBuildMethodsDefaultBuilderSetupLayout<TopWindow>,
	public BuilderSetup {
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
	virtual VectorMap<Id, Ctrl*> GetSetupCtrlsMap();
};

class BuildMethods : public WithBuildMethodsLayout<TopWindow> {
public:
	AndroidBuilderSetup androidSetup;
	DefaultBuilderSetup defaultSetup;
	
	OpenFileButton open_script;
	TextOption linkmode_lock;
	
	EditStringNotNull name;
	Index<String>     origfile;
	String            default_method;

	void Load();
	bool Save();

	void NewBuilder();
	void ShowDefault();
	void SetDefault();
	void ChangeMethod();
	void Import();

	void MethodMenu(Bar& bar);
	
	void SwitchSetupView();
	void AddBuilderSetupCtrls(BuilderSetup& builderSetup);
	VectorMap<String, String> SieveBuilderVars(const VectorMap<String, String>& map);
	VectorMap<String, String> MapBuilderVars(const VectorMap<String, String>& map);
	
	typedef BuildMethods CLASSNAME;

	BuildMethods();
};

END_UPP_NAMESPACE

#endif
