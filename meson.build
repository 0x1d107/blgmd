project('blgmd','c','cpp',default_options:['cpp_std=c++17'])
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
boost_regex_dep = dependency('boost',modules:['regex'])
deps+= boost_regex_dep
sqcpp_proj = subproject('sqlitecpp')
sqcpp_dep = sqcpp_proj .get_variable('sqlitecpp_dep')
deps+=sqcpp_dep

executable('blgmd',src,dependencies:deps)
