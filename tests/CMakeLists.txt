add_executable(test_utils utils.c)
add_executable(test_server server.c)
add_executable(test_barista barista.c)
add_executable(test_requester requester.c)
add_executable(test_responder responder.c)

target_link_libraries(test_responder responder)
target_link_libraries(test_requester requester)
target_link_libraries(test_barista barista requester responder)

add_test(NAME test_utils COMMAND test_utils)
add_test(NAME test_server COMMAND test_server)
add_test(NAME test_barista COMMAND test_barista)
add_test(NAME test_requester COMMAND test_requester)
add_test(NAME test_responder COMMAND test_responder)

