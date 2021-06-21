


add_executable(ky_server server_ky.c epoll.c http.c http_parse.c pq.c read.c threadpool.c timer.c util.c)
set_target_properties(ky_server
        PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED ON)