#ifndef _GraphDraw_GraphUndo_h_
#define _GraphDraw_GraphUndo_h_


namespace GraphDraw_ns
{
	
	class GraphUndoData : Moveable<GraphUndoData> {
		public:
		Callback undoAction;
		Callback redoAction;
	
		void Merge(const GraphUndoData& mostRecentAction) {
			// x === A==> y === B==> z        x ==> C==> z    C =  A >> B
			// x <==~A=== y <==~B=== z        x <==~C<== z   ~C = ~B >> ~A
			Callback cb;
			(cb << mostRecentAction.undoAction) << undoAction;
			undoAction = cb;
			
			cb.Clear();
			(cb << redoAction) << mostRecentAction.redoAction;
			redoAction = cb;
		}
	};
	
	
	class GraphUndo {
		public:
			
		private:
			Vector<GraphUndoData> _undoStack;
			Vector<GraphUndoData> _redoStack;
			int64 _lastActionAddTime;
			int _undoStackSize;
			
			void trimStack() {
				if ( _undoStack.GetCount() > _undoStackSize) {
					_undoStack.Remove(0, _undoStack.GetCount() - _undoStackSize);
				}
			}
		public:
			GraphUndo() : _lastActionAddTime(0), _undoStackSize(100) {}
			
			void SetUndoStackSize(unsigned int p) {
				_undoStackSize = p;
				trimStack();
			}
			bool CanUndo() const { return !_undoStack.IsEmpty(); }
			bool CanRedo() const  { return !_redoStack.IsEmpty(); }
			
			void AddUndoAction(GraphUndoData& CB) {
				int64 currTime = GetTickCount();
				if ((currTime - _lastActionAddTime) < 500) _undoStack.Top().Merge(CB);
				else                                       _undoStack.Add(CB);
				_redoStack.Clear();
				_lastActionAddTime = currTime;
				trimStack();
			}
	
			void Undo() {
				if ( !_undoStack.IsEmpty() ) {
					GraphUndoData cb = _undoStack.Pop();
					_redoStack.Add(cb);
					cb.undoAction();
				}
			}
			
			void Redo() {
				if ( !_redoStack.IsEmpty() ) {
					GraphUndoData cb = _redoStack.Pop();
					_undoStack.Add(cb);
					cb.redoAction();
				}
			}
	};
}

#endif
