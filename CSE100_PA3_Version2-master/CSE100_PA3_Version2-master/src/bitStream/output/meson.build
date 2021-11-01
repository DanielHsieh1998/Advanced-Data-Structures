# Define BitOutputStream using function library()

inc = include_directories('.')

BitOutputStream = library('BitOutputStream', sources:['BitOutputStream.cpp','BitOutputStream.hpp'])

BitOutputStream_dep = declare_dependency(include_directories: inc, link_with: BitOutputStream)