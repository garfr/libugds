project('ugds', 'c')

src = [
  'ugds.c',
]

inc = include_directories('include')

ugds = library(
  'ugds',
  src,
  c_args : ['-Wall', '-Wextra', '-Werror','-g', '-std=c99', '-pedantic'],
  include_directories : [inc]
)
#main = executable(
  #'main', 'main.c', 
  #c_args : ['-Wall', '-Wextra', '-Werror','-g', '-std=c99', '-pedantic'],
  #include_directories : [inc], link_with: ugds)
