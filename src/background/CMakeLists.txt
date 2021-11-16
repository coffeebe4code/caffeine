target_sources(background
  PRIVATE
    background.c
  PUBLIC
    background.h
)

target_link_libraries(background LINK_PUBLIC pthread)
