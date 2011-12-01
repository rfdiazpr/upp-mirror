#include <base.tpl>

#def CONTENT

#if count(view) == 0
	<div>There are no data in the view.</div>
#else
	<TD>
	#for view
		<TR>
			<a href="/edit-name/$view[].ID">
				<TD>$(view[].FIRST_NAME)</a></TD>
				<TD>$(view[].SECOND_NAME)</TD>
			</a>
		</TR>
	#endfor
	</TD>
#endif
