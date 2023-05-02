cmake_minimum_required(VERSION 3.25)
project(render)

find_package(SDL3 REQUIRED)

set(CMAKE_CXX_STANDARD 17)
add_executable(render canvas_main.cpp basic_canvas.cxx basic_canvas.hxx)

add_executable(line basic_canvas.cxx render_line_main.cxx render_line.cxx render_line.hxx)

add_executable(triangle triangle_render_main.cxx basic_canvas.cxx triangle_render.hxx triangle_render.cxx render_line.cxx)

add_executable(triangle_indexed triangle_index_main.cxx triangle_index_render.cxx triangle_index_render.hxx basic_canvas.cxx triangle_render.cxx render_line.cxx)

add_executable(triangle_interpolated triangle_interpolated_main.cxx triangle_interpolated.cxx triangle_interpolated.hxx basic_canvas.cxx triangle_index_render.cxx triangle_render.cxx render_line.cxx gfx.cxx)

target_link_libraries(triangle_interpolated PUBLIC SDL3::SDL3-shared)