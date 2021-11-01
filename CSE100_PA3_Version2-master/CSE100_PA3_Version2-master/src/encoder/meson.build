#inc = include_directories('.')

HC = library('HC', sources:['HCNode.hpp', 'HCTree.cpp', 'HCTree.hpp'], dependencies:[BitInputStream_dep, BitOutputStream_dep])
inc = include_directories('.')
HC_dep = declare_dependency(
    include_directories: inc, dependencies:[BitInputStream_dep, BitOutputStream_dep],link_with: HC)