add_executable(tcp_server socket.cpp)

target_compile_features(tcp_server PRIVATE cxx_std_17)