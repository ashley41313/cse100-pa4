actor_graph = library('actor_graph', sources:['ActorGraph.cpp', 'ActorGraph.hpp', 'UnionFind.cpp', 'UnionFind.hpp'])

inc = include_directories('.')

actor_graph_dep = declare_dependency(include_directories: inc,
  link_with: actor_graph)
