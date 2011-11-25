
#def main
#if count(view) == 0 {
	<div>There are no data in the view.</div>
#}
	<TD>
	#for view {
		<TR>
			<TD><a href="">$(view[].FIRST_NAME)</a></TD>
			<TD>$(view[].SECOND_NAME)</TD>
		</TR>
	#}
	</TD>
# }

	<TD>
	#for(int i = 0; i < count(view); i++)
		<TR>
			<TD>$(view[i].FIRST_NAME)</TD>
			<TD>$(view[i].SECOND_NAME)</TD>
		</TR>
	#endfor
	</TD>
#endddef

#def left


{{
	<html>
}}
if(count(view) == 0) {{
	<div>
}}
