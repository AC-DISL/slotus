include(${PROJECT_SOURCE_DIR}/src/utils/protobuf/common.cmake)

include_directories(
    ${PROJECT_SOURCE_DIR}/third_party/xlog/include
    ${PROJECT_SOURCE_DIR}/third_party/nlohmann/include
    ${PROJECT_SOURCE_DIR}/src/utils/protobuf
)


set(GRPC_SERVER_SRCS
  ${PROJECT_SOURCE_DIR}/src/utils/protobuf/job.pb.cc
  ${PROJECT_SOURCE_DIR}/src/utils/protobuf/job.grpc.pb.cc
  grpc-server/submit_server_impl.cpp
)

add_library(server_grpc_proto SHARED
  ${GRPC_SERVER_SRCS}
)
target_link_libraries(server_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)



set(SERVER_SRCS
  main.cc
)
add_executable(slotus_server ${SERVER_SRCS})

target_link_libraries(slotus_server 
  server_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
  xlog

)