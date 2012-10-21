function set_cnt(sel){
	loc=window.location.href.replace(/[&]*cnt=[0-9]*/g,'')+
		(location.search?'&':'?')+'cnt='+sel.options[sel.selectedIndex].text;
	window.location.replace(loc)
}
