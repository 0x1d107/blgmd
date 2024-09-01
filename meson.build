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



executable('blgmd',src,dependencies:deps)
