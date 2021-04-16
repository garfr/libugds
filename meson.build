project('ugds', 'c')

src = [
  'ugds.c',
]

inc = include_directories('include')

cc = meson.get_compiler('c')

utf8proc_dep = cc.find_library('utf8proc')

ugds = library(
  'ugds',
  src,
  c_args : ['-Wall', '-Wextra', '-Werror','-g', '-std=c99', '-pedantic'],
  dependencies: utf8proc_dep,
  include_directories : [inc]
)
#main = executable(
  #'main', 'main.c', 
  #c_args : ['-Wall', '-Wextra', '-Werror','-g', '-std=c99', '-pedantic'],
  #include_directories : [inc], link_with: ugds, dependencies: utf8proc_dep)
