target_sources(barista
  PRIVATE
    barista.c
  PUBLIC
    barista.h
)

target_link_libraries(barista LINK_PUBLIC pthread)
