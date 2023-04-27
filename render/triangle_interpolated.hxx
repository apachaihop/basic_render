#ifndef RENDER_TRIANGLE_INTERPOLATED_HXX
#define RENDER_TRIANGLE_INTERPOLATED_HXX
#include "triangle_index_render.hxx"

struct vertex
{
    double x  = 0; /// x
    double y  = 0; /// y
    double z  = 0; /// z
    double f3 = 0; /// r
    double f4 = 0; /// g
    double f5 = 0; /// b
    double f6 = 0; /// u (texture coordinate)
    double f7 = 0; /// v (texture coordinate)
};

double interpolate(const double f0, const double f1, const double t);

vertex interpolate(const vertex& v0, const vertex& v1, const double t);

struct triangle_interpolated : triangle_indexed_render
{
    triangle_interpolated(canvas& buffer, size_t width, size_t height);
    void draw_triangles(std::vector<vertex>&   vertexes,
                        std::vector<uint16_t>& indexes);

private:
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
