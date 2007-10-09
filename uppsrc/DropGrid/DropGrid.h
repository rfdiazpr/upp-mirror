#ifndef _DropGrid_DropGrid_h_
#define _DropGrid_DropGrid_h_

#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>

NAMESPACE_UPP

class DropGrid : public DropBox, public Convert
{
	private:

		class PopUpGrid : public GridCtrl
		{
			private:
			
				void CloseNoData();
				void CloseData();
						
			public:
				Callback WhenPopDown;
				Callback WhenClose;
				Callback WhenCloseData;
				Callback WhenCloseNoData;
		
				typedef PopUpGrid CLASSNAME;
			
				virtual void Deactivate();
				void PopUp(Ctrl *owner, const Rect &r);

				PopUpGrid();
		};

	protected:
	
		int key_col;
		int value_col;
		Vector<int> value_cols;
		PopUpGrid list;
	
	private:
		int rowid;
		Value value;
		
		int list_width;
		int list_height;
		int drop_lines;
		
		bool display_all:1;
		bool header:1;
		bool valuekey:1;
		bool notnull:1;
		bool format_columns:1;
		bool drop_enter:1;
		bool data_action:1;
		bool searching:1;
		
		GridDisplay *display;
				
		void Change(int dir);
		void SearchCursor();
		void DoAction(int row, bool action = true);
		Value MakeLongValue(int r = -1, bool columns = true) const;
		void UpdateValue();
		
	public:
	
		typedef DropGrid CLASSNAME;
		DropGrid();
		
		void Close();
		void CloseData();
		void CloseNoData();		
		void Drop();
		
		DropGrid& Width(int w);
		DropGrid& Height(int h);
		DropGrid& SetKeyColumn(int n);
		DropGrid& SetValueColumn(int n);
		DropGrid& AddValueColumn(int n);
		DropGrid& AddValueColumns(int first = -1, int last = -1);
		DropGrid& DisplayAll(bool b = true);
		DropGrid& SetDropLines(int d);
		DropGrid& Header(bool b = true);
		DropGrid& NoHeader();
		DropGrid& Resizeable(bool b = true);
		DropGrid& ColorRows(bool b = true);
		DropGrid& NotNull(bool b = true);
		DropGrid& ValueAsKey(bool b = true);
		DropGrid& SetDisplay(GridDisplay &d);
		DropGrid& FormatColumns(bool b = true);
		DropGrid& DropEnter(bool b = true);
		DropGrid& DataAction(bool b = true);
		DropGrid& Searching(bool b = true);
		
		GridCtrl::ItemRect& AddColumn(const char *name, int width = GridCtrl::GD_COL_WIDTH, bool idx = false);
		GridCtrl::ItemRect& AddColumn(Id id, const char *name, int width = GridCtrl::GD_COL_WIDTH, bool idx = false);
		GridCtrl::ItemRect& AddIndex(const char *name = NULL);
		GridCtrl::ItemRect& AddIndex(Id id);
		int AddColumns(int cnt);
		
		int SetIndex(int n);
		int GetIndex() const;
		
		int GetCount() const;
		
		void Reset();
		void Clear();
		void ClearValue();
		
		virtual Value GetData() const;
		virtual void SetData(const Value& v);
		
		Value GetValue() const;
		Value GetKey() const;
		
		virtual bool Key(dword k, int);
		virtual void Paint(Draw& draw);
		virtual void LeftDown(Point p, dword keyflags);
		virtual void GotFocus();
		virtual void LostFocus();
		
		Value Get(int c) const;
		Value Get(Id id) const;		
		Value Get(int r, int c) const;
		Value Get(int r, Id id) const;		
		void  Set(int c, const Value& v);
		void  Set(Id id, const Value& v);
		void  Set(int r, int c, const Value& v);
		void  Set(int r, Id id, const Value& v);
		void  Set(int r, const Vector<Value> &v, int data_offset = 0, int column_offset = 0);
		void  Add(const Vector<Value> &v, int data_offset = 0, int column_offset = 0);
		
		GridCtrl::ItemRect& GetRow(int r);
		
		bool IsSelected();
		bool IsEmpty();
		
		int Find(const Value& v, int col = 0);
		int Find(const Value& v, Id id);
		
		virtual Value Format(const Value& q) const;
				
		//$-DropCtrl& Add(const Value& [, const Value& ]...);
		#define  E__Add(I)      DropGrid& Add(__List##I(E__Value));
			__Expand(E__Add)
		#undef   E__Add
		//$+
};

END_UPP_NAMESPACE

#endif

