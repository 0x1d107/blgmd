project('blgmd','c','cpp')
md4c_dep = dependency('md4c') 
srchl_dep = dependency('source-highlight')

deps=[md4c_dep,srchl_dep]

src=[
	'blgmd.cpp',
	'extrender.cpp',
]

src+=[
	'entity.c',
	'md4c-html.c',
]

subdir('itex-src')
deps+=itex_dep
inja_proj = subproject('inja')
inja_dep = inja_proj.get_variable('inja_dep')
deps+=inja_dep

executable('blgmd',src,dependencies:deps)
