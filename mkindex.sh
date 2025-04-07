#!/bin/bash
cat <<HEADER
<html>
<head>
	<title> Index </title>
</head>
<body>
HEADER
sqlite3 -markdown 'index.db' "
select date,printf('[%s](%s)',title,replace(A.filename,'.md','.html')) as link,title,tags from (select distinct filename from metadata ) as A 
left join (select filename,value as date from metadata where key='date') as B on A.filename = B.filename 
left join (select filename,value as title from metadata where key='title') as C on A.filename = C.filename 
left join (select filename,value as tags from metadata where key = 'tags' ) as D on A.filename = D.filename
ORDER BY date DESC;
" | ../build/blgmd -N
cat <<FOOTER
</body>
</html>
FOOTER
