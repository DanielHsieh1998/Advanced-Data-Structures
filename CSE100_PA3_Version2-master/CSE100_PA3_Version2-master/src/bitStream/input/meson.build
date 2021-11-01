# Define BitInputStream using function library()

inc = include_directories('.')

BitInputStream = library('BitInputStream', sources:['BitInputStream.cpp', 'BitInputStream.hpp'])

BitInputStream_dep = declare_dependency(include_directories: inc, link_with: BitInputStream)