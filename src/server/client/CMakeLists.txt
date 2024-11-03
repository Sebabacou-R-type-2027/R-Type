target_sources(r-type_server
    PRIVATE client.cpp
        ClientSaver.cpp
    PUBLIC FILE_SET HEADERS
        FILES client.hpp
            ClientSaver.hpp
)
