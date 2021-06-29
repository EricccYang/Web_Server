


add_executable(ky_server server_ky.c epoll.c http.c http_request.c http_parse.c pq.c read.c threadpool.c timer.c util.c )

add_executable(pq_test pq_test.c)
set_target_properties(ky_server
        PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED ON)


target_link_libraries(
        ky_server
        pthread
)