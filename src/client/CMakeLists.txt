include(${PROJECT_SOURCE_DIR}/src/utils/protobuf/common.cmake)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/utils
    ${PROJECT_SOURCE_DIR}/third_party/xlog/include
    ${PROJECT_SOURCE_DIR}/third_party/nlohmann/include
    ${PROJECT_SOURCE_DIR}/src/utils/protobuf
)


set(GRPC_CLIENT_SRCS
  ${PROJECT_SOURCE_DIR}/src/utils/protobuf/job.pb.cc
  ${PROJECT_SOURCE_DIR}/src/utils/protobuf/job.grpc.pb.cc
  grpc-client/submit_client.cpp
)

add_library(client_grpc_proto SHARED
  ${GRPC_CLIENT_SRCS}
)
target_link_libraries(client_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)



set(CLIENT_SRCS
  main.cc
  ${PROJECT_SOURCE_DIR}/src/utils/common.cc
)
add_executable(slotus_client ${CLIENT_SRCS})

target_link_libraries(slotus_client 
  client_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
  xlog

)