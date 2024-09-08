flex = find_program('flex')
bison = find_program('bison')
yyprefix = 'itex2MML_yy'

lgen = generator(flex,
output : '@PLAINNAME@.yy.c',
arguments : ['-P',yyprefix,'-o', '@OUTPUT@', '@INPUT@']
)

pgen = generator(bison,
output : ['@BASENAME@.tab.c', '@BASENAME@.tab.h'],
arguments : ['-y','-v','-p',yyprefix,'@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@'])

lex = custom_target('lex.yy.c',input:['itex2MML.l'],
	output: ['lex.yy.c'],
	command:[flex,'-P',yyprefix,'-o','@OUTPUT@', '@INPUT@']
)
tabs = custom_target('y.tab.c',input: ['itex2MML.y'],
	output : ['y.tab.c', 'y.tab.h'],
	command:[bison,'-y','-v','-p',yyprefix,'-d','@INPUT@']
)
lfiles = lgen.process('lex.l')
pfiles = pgen.process('y')

libitex = static_library('itex',lfiles,pfiles)
itex_dep = declare_dependency(link_with:libitex,include_directories:['.'])
