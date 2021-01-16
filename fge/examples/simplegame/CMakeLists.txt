add_executable(simplegame)

target_sources(simplegame
  PRIVATE
  main.cpp
  )

target_link_libraries(simplegame
  PRIVATE
  fge
  )
