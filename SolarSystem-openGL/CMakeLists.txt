cmake_minimum_required (VERSION 2.8)

#项目名称
project(NFF)
set(SERVER_NAME "NFFServer")

#编译器参数
set(CMAKE_CXX_COMPILER g++ CACHE STRING "C++ compiler" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_compile_options(-fPIE -pie -fstack-protector-strong -Wl,-z,relro,-z,now,-z,noexecstack)

# 包匹配
find_package(Threads)
find_package(PkgConfig)

pkg_check_modules(GLM glm)
pkg_check_modules(GLFW glfw3)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(OPENGL_DEPEND_LIBS
	${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
	${GLM_LIBRARIES}
    ${GLFW_LIBRARIES}
)
set(OPENGL_DEPEND_DIR
	${OPENGL_INCLUDE_DIR}
	${GLUT_INCLUDE_DIR}
	${GLM_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
)


# 指定库文件连接目录
link_directories(
    ${CMAKE_BINARY_DIR}
)
LINK_LIBRARIES(m dl) 

# 头文件
include_directories(
	"${PROJECT_SOURCE_DIR}/"
	${PROJECT_SOURCE_DIR}/glad/include
	${OPENGL_DEPEND_DIR}
)
# 源文件
set(OPENGL_MODULES
    solar_systerm
)

# 指定生成库文件

foreach (_module ${OPENGL_MODULES})
    aux_source_directory(
        ${_module} MODULE_${_module}_SOURCES
    )
	message(STATUS "${MODULE_${_module}_SOURCES}")
    add_executable(${_module} ${MODULE_${_module}_SOURCES} ${GLAD_SRCS})
    #set_target_properties(${_module} PROPERTIES PREFIX "exe")
    target_link_libraries(${_module} 
        ${CMAKE_THREAD_LIBS_INIT} 
        ${OPENGL_DEPEND_LIBS}
    )
endforeach()
