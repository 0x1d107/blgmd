#!/bin/bash
cat <<HEADER
<html>
<head>
	<title> Index </title>
</head>
<body>
HEADER
sqlite3 -readonly -markdown 'index.db' "
select date,printf('[%s](%s)',title,replace(A.filename,'.md','.html')) as link,tags from (select distinct filename from metadata ) as A 
left join (select filename,value as date from metadata where key='date') as B on A.filename = B.filename 
left join (select filename,value as title from metadata where key='title') as C on A.filename = C.filename 
left join (
with RECURSIVE split_tags(filename,tag,tail) as (
	select filename,'',value||',' from metadata where key='tags'
	UNION ALL SELECT filename,trim(substr(tail,0,instr(tail,','))),substr(tail,instr(tail,',')+1) from split_tags where tail != ''
) select filename,group_concat(printf('[%s](tag-%s.html)',tag,tag),',') as tags from split_tags where tag != '' group by filename
) as D on A.filename = D.filename
ORDER BY date DESC;
" | ../build/blgmd -N
cat <<FOOTER
</body>
</html>
FOOTER
