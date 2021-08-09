# Client
add_executable(Client Client.cpp)
target_compile_features(Client PRIVATE cxx_std_20)
target_compile_definitions(Client PRIVATE)
target_link_libraries(Client PRIVATE Sennet::Sennet)

# Sandbox
add_executable(Sandbox Sandbox.cpp)
target_compile_features(Sandbox PRIVATE cxx_std_20)
target_compile_definitions(Sandbox PRIVATE)
target_link_libraries(Sandbox PRIVATE Sennet::Sennet)

# Server
add_executable(Server Server.cpp)
target_compile_features(Server PRIVATE cxx_std_20)
target_compile_definitions(Server PRIVATE)
target_link_libraries(Server PRIVATE Sennet::Sennet)
