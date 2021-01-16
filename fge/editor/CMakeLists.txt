add_executable(fgedit)

target_sources(fgedit
  PRIVATE
  main.cpp
  )

target_link_libraries(fgedit
  PRIVATE
  fge
  )
