#include base.tpl

#def MAIN

#if view.count == 0
	<div>There are no data in the view.</div>
#else
	<TD>
	#for view
		<TR>
		#if view.first
			
		#endif
			<a href="/edit-name/$view[].ID">
				<TD>$view[].FIRST_NAME</a></TD>
				<TD>$view[].SECOND_NAME</TD>
			</a>
		</TR>
	#endfor
	</TD>
#endif
