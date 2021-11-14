target_sources(responder
  PRIVATE
    responder.c
  PUBLIC
    responder.h
)

target_link_libraries(responder LINK_PUBLIC pthread)
