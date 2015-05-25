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

class BuilderSetupHelperInterface {
public:
	virtual ~BuilderSetupHelperInterface() {}

	virtual void New(const String& builder) = 0;
	virtual void OnLoad() = 0;
};

class AndroidBuilderSetup :
	public WithBuildMethodsAndroidBuilderSetupLayout<ParentCtrl> {
public:
	AndroidBuilderSetup();
	
	VectorMap<Id, Ctrl*> GetSetupCtrlsMap();

public:
	FrameRight<Button> ndkBrowse;
	FrameRight<Button> jdkBrowse;
};

class AndroidBuilderSetupHelper : public BuilderSetupHelperInterface {
public:
	void SetSetup(AndroidBuilderSetup* setup) { this->setup = setup; }
	
	virtual void New(const String& builder);
	virtual void OnLoad();
	
private:
	AndroidBuilderSetup* setup;
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
	
	VectorMap<Id, Ctrl*> GetSetupCtrlsMap();
};

class DefaultBuilderSetupHelper : public BuilderSetupHelperInterface {
public:
	DefaultBuilderSetupHelper();
	
	void SetSetup(DefaultBuilderSetup* setup) { this->setup = setup; }
	
	virtual void New(const String& builder);
	virtual void OnLoad() {}
	
private:
	DefaultBuilderSetup* setup;
};

class BuilderSetup : public Moveable<BuilderSetup> {
public:
	BuilderSetup(ParentCtrl* setupCtrl,
	             BuilderSetupHelperInterface* setupCtrlHelper,
	             VectorMap<Id, Ctrl*> setupCtrlsMap);
	
	ParentCtrl* setupCtrl;
	BuilderSetupHelperInterface* setupCtrlHelper;
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
	
	AndroidBuilderSetup       androidSetup;
	AndroidBuilderSetupHelper androidSetupHelper;
	DefaultBuilderSetup       defaultSetup;
	DefaultBuilderSetupHelper defaultSetupHelper;
	
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
	
private:
	void          SieveBuilders(Index<String>& sievedBuilders, const Index<String>& builders) const;
	String        BuildersToString(const Index<String>& builders) const;
	Index<String> StringToBuilders(const String& str) const;
	Index<String> GetBuilders() const;
};

END_UPP_NAMESPACE

#endif
