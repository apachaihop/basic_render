#ifndef RENDER_TRIANGLE_RENDER_HXX
#define RENDER_TRIANGLE_RENDER_HXX

#include "basic_canvas.hxx"
#include "render_line.hxx"

struct triangle_render : render_line
{
    triangle_render(canvas& buffer, size_t width, size_t height);

    virtual std::vector<position> pixels_positions_triangle(position v0,
                                                            position v1,
                                                            position v2);
    void draw_triangles(std::vector<position>& vertexes,
                        size_t                 num_vertexes,
                        pixel                  p);
};
#endif // RENDER_TRIANGLE_RENDER_HXX
