subdir('bitStream')
subdir('encoder')

#include_directories('../subprojects/cxxopts')

FileUtils_dep = declare_dependency(include_directories: include_directories('.'))

bitconverter_exe = executable('bitconverter.cpp.executable', 
                    sources:['bitconverter.cpp'], 
                    dependencies:[BitInputStream_dep, BitOutputStream_dep], 
                    install: true)

compress_exe = executable('compress.cpp.executable', 
                    sources:['compress.cpp'], 
                    dependencies:[HC_dep, cxxopts_dep, FileUtils_dep], 
                    install: true)

uncompress_exe = executable('uncompress.cpp.executable', 
                    sources:['uncompress.cpp'], 
                    dependencies:[HC_dep, cxxopts_dep, FileUtils_dep], 
                    install: true)

# FileUtils_exe = executable('FileUtils.hpp.executable', 
#                     sources:['FileUtils.hpp'], 
#                     install: true)