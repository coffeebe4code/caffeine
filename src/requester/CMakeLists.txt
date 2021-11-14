target_sources(requester
  PRIVATE
    requester.c
  PUBLIC
    requester.h
)

target_link_libraries(requester LINK_PUBLIC pthread)
