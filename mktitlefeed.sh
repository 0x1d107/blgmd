#!/bin/bash
BASEURL="https://example.com"
cat <<HEADER
<rss version="2.0">
<channel>
<title> Test channel </title>
<link>${BASEURL}</link>
<description> Testing testing testing... </description>
HEADER
sqlite3 -readonly 'index.db' "
select printf('<item>
	<title>%s</title>
	<link>${BASEURL}/%s</link>
	<pubDate>%s %s %s</pubDate>
	<description> <![CDATA[ %s ]]> </description>
</item>',
title,replace(A.filename,'.md','.html'),strftime('%d',trim(pdate)),
substr('JanFebMarAprMayJunJulAugSepOctNovDec', 1 + 3*strftime('%m', trim(pdate)), -3),
strftime('%Y %H:%M:%S GMT',trim(pdate)),
readfile(replace(A.filename,'.md','.html'))
) 
as link  from (select distinct filename from metadata ) as A 
left join (select filename,value as pdate from metadata where key='date') as B on A.filename = B.filename 
left join (select filename,value as title from metadata where key='title') as C on A.filename = C.filename 
ORDER BY pdate DESC;
"
cat <<FOOTER
</channel>
</rss>
FOOTER
