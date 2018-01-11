set(target axpy)

file(GLOB src "*.c")
file(GLOB cl "*.cl")

file(GLOB hrc "*.h")

add_executable(${target} ${hrc} ${src})
target_link_libraries(${target} ${OPENCL_LIBRARIES})
target_link_libraries(${target} ${AlteraCL_LIBRARY})
file(COPY ${cl} DESTINATION ".")