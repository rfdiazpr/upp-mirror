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

class AndroidBuilderSetup : public WithBuildMethodsAndroidBuilderSetupLayout<TopWindow> {
public:
	AndroidBuilderSetup();
	
	void New();
};

class DefaultBuilderSetup : public WithBuildMethodsDefaultBuilderSetupLayout<TopWindow> {
public:
	TextOption debug_blitz;
	TextSwitch debug_linkmode;
	TextOption release_blitz;
	TextSwitch release_linkmode;
	TextOption allow_pch;
	DirTable   path;
	DirTable   include;
	DirTable   lib;
	
	DefaultBuilderSetup();
	void New(const String& builder);
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
	
	typedef BuildMethods CLASSNAME;

	BuildMethods();
};

END_UPP_NAMESPACE

#endif
