get_filename_component(proto "../../../runtime/src/main/proto/service.proto" ABSOLUTE)

get_filename_component(proto_path "${proto}" PATH)

set(proto_srcs "${CMAKE_CURRENT_SOURCE_DIR}/service.pb.cc")
set(proto_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/service.pb.h")
set(grpc_srcs "${CMAKE_CURRENT_SOURCE_DIR}/service.grpc.pb.cc")
set(grpc_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/service.grpc.pb.h")

add_custom_command(
    OUTPUT "${proto_srcs}" "${proto_hdrs}" "${grpc_srcs}" "${grpc_hdrs}"
    COMMAND protoc
    ARGS --grpc_out "${CMAKE_CURRENT_SOURCE_DIR}"
    --cpp_out "${CMAKE_CURRENT_SOURCE_DIR}"
    -I "${proto_path}"
    --plugin=protoc-gen-grpc="${grpc_BINARY_DIR}/grpc_cpp_plugin"
    "${proto}"
    DEPENDS "${proto}")

add_library(breakpoint_service_proto
    ${proto_srcs} ${proto_hdrs} ${grpc_srcs} ${grpc_hdrs})

target_include_directories(breakpoint_service_proto
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(breakpoint_service_proto
    grpc++_reflection
    grpc++
    libprotobuf)
