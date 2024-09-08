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

lfiles = lgen.process('lex.l')
pfiles = pgen.process('y')

libitex = static_library('itex',lfiles,pfiles)
itex_dep = declare_dependency(link_with:libitex,include_directories:['.'])
