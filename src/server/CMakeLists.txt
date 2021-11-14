target_sources(server
  PRIVATE
    server.c
  PUBLIC
    server.h
)

target_link_libraries(server LINK_PUBLIC pthread)
