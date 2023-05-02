#ifndef RENDER_TRIANGLE_INTERPOLATED_HXX
#define RENDER_TRIANGLE_INTERPOLATED_HXX
#include "gfx.hxx"
#include "triangle_index_render.hxx"

double interpolate(const double f0, const double f1, const double t);

vertex interpolate(const vertex& v0, const vertex& v1, const double t);

struct triangle_interpolated : triangle_indexed_render
{
    triangle_interpolated(canvas& buffer, size_t width, size_t height);
    void draw_triangles(std::vector<vertex>&   vertexes,
                        std::vector<uint16_t>& indexes);
    void set_gfx(gfx_program& prog) { gfx = &prog; }

private:
    gfx_program*        gfx = nullptr;
    std::vector<vertex> rasterize_triangle(const vertex& v0,
                                           const vertex& v1,
                                           const vertex& v2);
    std::vector<vertex> raster_horizontal_triangle(const vertex& single,
                                                   const vertex& left,
                                                   const vertex& right);

    void raster_one_horizontal_line(const vertex&        left_vertex,
                                    const vertex&        right_vertex,
                                    std::vector<vertex>& out);
};
#endif // RENDER_TRIANGLE_INTERPOLATED_HXX
