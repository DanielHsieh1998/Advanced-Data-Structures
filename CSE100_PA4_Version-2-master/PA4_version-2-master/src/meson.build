subdir('ActorGraph')
subdir('Map')

pathfinder_exe = executable('pathfinder.exe', 
    sources: ['pathfinder.cpp'],
    dependencies: [actorgraph_dep],
    install : true)

map_exe = executable('map.exe', 
    sources: ['map.cpp'],
    dependencies: [map_dep, cxxopts_dep],
    install : true)

linkpredictor_exe = executable('linkpredictor.exe', 
    sources: ['linkpredictor.cpp'],
    dependencies: [actorgraph_dep],
    install : true)
