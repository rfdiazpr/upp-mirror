#include <WinVer.h>
#include <DbgHelp.h>
#include <psapi.h>
#include "cvconst.h"

#include <plugin/ndisasm/ndisasm.h>

#define KEYGROUPNAME "Pdb debugger"
#define KEYNAMESPACE PdbKeys
#define KEYFILE      <ide/Debuggers/Pdb.key>
#include             <CtrlLib/key_header.h>

using namespace PdbKeys;

struct Pdb : Debugger, Ctrl {
	virtual void Stop();
	virtual bool IsFinished();
	virtual bool Key(dword key, int count);
	virtual void DebugBar(Bar& bar);
	virtual bool SetBreakpoint(const String& filename, int line, const String& bp);
	virtual bool RunTo();
	virtual void Run();
	virtual void Serialize(Stream& s);

	struct ModuleInfo : Moveable<ModuleInfo> {
		dword  base;
		dword  size;
		String path;
		bool   symbols;

		ModuleInfo() { base = size = 0; symbols = false; }
	};

	struct FilePos {
		String path;
		int    line;
		dword  address;

		operator bool() const                   { return !IsNull(path); }

		FilePos() {}
		FilePos(const String& p, int l) : path(p), line(l) {}
	};

	struct MemPg : Moveable<MemPg> {
		char data[1024];
	};

	struct CdbHexView : HexView {
		Pdb *cdb;

		virtual int Byte(int64 addr)            { return cdb->Byte((dword)addr); }

		CdbHexView()                            { SetTotal(0x80000000); }
	}
	memory;

	struct FnInfo {
		String name;
		dword  address;
		dword  size;
		dword  pdbtype;

		FnInfo() { address = size = pdbtype = 0; }
	};

	struct Val : Moveable<Val> {
		int    type;
		int    ref;
		bool   array:1;
		bool   rvalue:1;
		byte   bitpos;
		byte   bitcnt;
		union {
			dword  address;
			int64  ival;
			double fval;
		};

		Val() { rvalue = false; ref = 0; array = false; bitcnt = 0; address = 0; }
	};

	struct NamedVal : Moveable<NamedVal> {
		String name;
		Val    val;
	};

	enum { UNKNOWN = -99, BOOL1, SINT1, UINT1, SINT2, UINT2, SINT4, UINT4, SINT8, UINT8, FLT, DBL, PFUNC };

	struct Type : Moveable<Type> {
		Type() : size(-1), vtbl_typeindex(-1) {}

		dword  modbase;

		String name;
		int    size;

		int    vtbl_typeindex;
		int    vtbl_offset;

		Vector<Val>            base;
		VectorMap<String, Val> member;
		VectorMap<String, Val> static_member;
	};

	struct Frame : Moveable<Frame> {
		dword                  eip, ebp;
		FnInfo                 fn;
		VectorMap<String, Val> param;
		VectorMap<String, Val> local;
	};

	struct VisualPart : Moveable<VisualPart> {
		String  text;
		Color   ink;
		bool    mark;
	};

	struct Visual {
		int                length;
		Vector<VisualPart> part;

		void   Cat(const String& text, Color ink = SColorText);
		void   Cat(const char *text, Color ink = SColorText);
		String GetString() const;
		void   Clear() { part.Clear(); length = 0; }

		Visual()       { length = 0; }
	};

	struct VisualDisplay : Display {
		virtual void Paint(Draw& w, const Rect& r, const Value& q,
	                       Color ink, Color paper, dword style) const;
	};

	int                     lock;
	bool                    running;
	bool                    stop;
	HANDLE                  hProcess;
	DWORD                   processid;
	VectorMap<dword, dword> threadsp;
	bool                    terminated;
	bool                    refreshmodules;
	Vector<ModuleInfo>      module;
	DEBUG_EVENT             event;
	HWND                    hWnd;
	VectorMap<dword, byte>  bp_set;
	CONTEXT                 context;
	ArrayMap<dword, CONTEXT> ctx;

	Index<dword>            invalidpage;
	VectorMap<dword, MemPg> mempage;

	Index<dword>            breakpoint;

	ArrayMap<int, Type>     type;

	String                  disas_name;

	Array<Frame>            frame;
	String                  autotext;


	FrameBottom<WithRegistersLayout<StaticRect> > regs;

	Vector<String>     regname;
	Vector<Label *>    reglbl;

	DbgDisas           disas;

	EditString         watchedit;

	TabCtrl            tab;
	DropList           threadlist;
	DropList           framelist;
	Label              dlock;
	ArrayCtrl          locals;
	ArrayCtrl          watches;
	ArrayCtrl          autos;
	ArrayCtrl          explorer;
	EditString         expexp;
	Button             exback, exfw;
	StaticRect         explorer_pane;
	StaticRect         pane;
	Splitter           split;
	TreeCtrl           tree;

	VectorMap<String, String> treetype;

	Vector<String>      exprev, exnext;

	Index<String>          noglobal;
	VectorMap<String, Val> global;

	void       Error();

// debug
	void       LoadModuleInfo();
	int        FindModuleIndex(dword base);
	void       UnloadModuleSymbols();
	void       CleanupOnExit();
	void       AddThread(dword dwThreadId, HANDLE hThread);
	void       Lock();
	void       Unlock();
	bool       RunToException();
	bool       AddBp(dword address);
	bool       RemoveBp(dword address);
	bool       RemoveBp();
	bool       IsBpSet(dword address) const { return bp_set.Find(address) >= 0; }
	bool       Continue();
	bool       SingleStep();
	void       BreakRunning();
	void       SetBreakpoints();
	void       SaveForeground();
	void       RestoreForeground();

	const CONTEXT& CurrentContext();
	void           WriteContext(dword cf = CONTEXT_CONTROL);

// mem
	int        Byte(dword addr);
	bool       Copy(dword addr, void *ptr, int count);
	String     ReadString(dword addr, int maxlen = INT_MAX);
	WString    ReadWString(dword addr, int maxlen = INT_MAX);

// sym
	struct LocalsCtx;
	static BOOL CALLBACK  EnumLocals(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext);
	static BOOL CALLBACK  EnumGlobals(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext);
	void                  TypeVal(Pdb::Val& v, int typeId, dword modbase);
	String                GetSymName(dword modbase, dword typeindex);
	dword                 GetSymInfo(dword modbase, dword typeindex, IMAGEHLP_SYMBOL_TYPE_INFO info);
	const Type&           GetType(int ti);
	int                   GetTypeIndex(dword modbase, dword typeindex);
	const Type&           GetTypeId(dword modbase, dword typeindex) { return GetType(GetTypeIndex(modbase, typeindex)); }
	Val                   GetGlobal(const char *fn, const String& name);

	dword                 GetAddress(FilePos p);
	FilePos               GetFilePos(dword address);
	FnInfo                GetFnInfo(dword address);
//	FnInfo                GetFnInfo(String name);
	void                  GetLocals(dword eip, dword ebp, VectorMap<String, Pdb::Val>& param,
	                                VectorMap<String, Pdb::Val>& local);
	String                TypeAsString(int ti, bool deep = true);

// exp
	void       ThrowError(const char *s);
	int        SizeOfType(int ti);
	Val        GetRVal(Val v);
	Val        DeRef(Val v);
	Val        Ref(Val v);
	int64      GetInt(Val v);
	double     GetFlt(Val v);
	Val        Compute(Val v1, Val v2, int oper);
	Val        RValue(int v);
	Val        Field0(Pdb::Val v, const String& field);
	Val        Field(Pdb::Val v, const String& field);
	Val        Term(CParser& p);
	Val        Post(CParser& p);
	Val        Unary(CParser& p);
	Val        Additive(CParser& p);
	Val        Multiplicative(CParser& p);
	Val        Exp0(CParser& p);
	Val        Exp(CParser& p);
	void       Visualise(Visual& result, Pdb::Val val, int expandptr, int slen, int maxlen = 250);
	Visual     Visualise(Val v, int maxlen = 250);
	Visual     Visualise(const String& exp, int maxlen = 250);

// code
	int        Disassemble(dword ip);
	void       Reg(Label& reg, dword val);
	bool       IsValidFrame(dword eip);
	void       Sync0();
	void       Sync();
	void       SetThread();
	void       SetFrame();
	bool       Step(bool over);
	void       Trace(bool over);
	void       StepOut();
	void       DoRunTo() { RunTo(); }
	dword      CursorAdr();
	void       SetIp();

	void       Break();

// data
	static VectorMap<String, Value> DataMap(const ArrayCtrl& a);
	static Value Vis(const String& key, const VectorMap<String, Value>& prev,
	                 pick_ Visual& vis, bool& ch);
	static void  Vis(ArrayCtrl& a, const String& key,
	                 const VectorMap<String, Value>& prev, pick_ Visual& vis);

	void      DisasCursor() {}
	void      DisasFocus() {}
	void      Locals();
	void      Watches();
	void      TryAuto(const String& exp, const VectorMap<String, Value>& prev);
	void      Autos();
	void      Explore(const Val& val, const VectorMap<String, Value>& prev);
	void      Explore(const String& exp);
	void      ExploreKey(ArrayCtrl *a);
	void      Explorer();
	void      ExpExp();
	void      ExBack();
	void      ExFw();
	void      DoExplorer();
	String    GetExpExp();
	void      ExplorerTree();
	void      Data();
	void      AddWatch();
	void      SetTab(int i);

	void      SetTree(const String& exp);
	void      SetTreeA(ArrayCtrl *data);
	void      TreeNode(int parent, const String& name, Val val);
	void      TreeExpand(int node);
	String    StoreTree(int parent);
	void      SaveTree();
	void      ExpandTreeType(int parent, CParser& p);

	void      CopyStack();
	void      CopyDisas();

	void      MemoryGoto(const String& exp);

	void      MemMenu(ArrayCtrl& array, Bar& bar, const String& exp);
	void      DataMenu(ArrayCtrl& array, Bar& bar, const String& exp);
	void      AutosMenu(Bar& bar);
	void      LocalsMenu(Bar& bar);
	void      WatchesMenu(Bar& bar);
	void      ExplorerMenu(Bar& bar);

	void      Tab();

	void      AddReg(const char *reg, Label *lbl) { regname.Add(reg); reglbl.Add(lbl); }

	bool      Create(One<Host> host, const String& exefile, const String& cmdline);

	typedef Pdb CLASSNAME;

	Pdb();
	virtual ~Pdb();
};
