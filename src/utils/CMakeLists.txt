target_sources(utils
  PRIVATE
    utils.c
  PUBLIC
    utils.h
)

target_link_libraries(utils LINK_PUBLIC pthread)
