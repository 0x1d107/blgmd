-- Create files for each tag with a list of posts that tagged with the tag
with RECURSIVE split_tags(filename,tag,tail) as (
	select filename,'',value||',' from metadata where key='tags'
	UNION ALL SELECT filename,trim(substr(tail,0,instr(tail,','))),substr(tail,instr(tail,',')+1) from split_tags where tail != ''
) 
select 
writefile(
'tag-'||tag||'.html',
printf(
'<html>
<head>
<title>Posts tagged %s</title>
</head>
<body>
<h1> Posts tagged %s </h1>
<ul>
',tag,tag)
||
group_concat(
printf('<li><a href="%s">%s</a></li>',replace(split_tags.filename,'.md','.html'),metadata.value),char(10))
||
'</ul> </body></html>'
)
from split_tags 
left join metadata on split_tags.filename = metadata.filename and metadata.key = 'title'
left join metadata as Mdate on split_tags.filename = Mdate.filename and Mdate.key = 'date'
where tag != '' group by tag order by Mdate.value
;
