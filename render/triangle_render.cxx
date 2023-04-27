#include "triangle_render.hxx"
#include "basic_canvas.hxx"

triangle_render::triangle_render(canvas& buffer, size_t width, size_t height)
    : render_line(buffer, width, height)
{
}
std::vector<position> triangle_render::pixels_positions_triangle(position v0,
                                                                 position v1,
                                                                 position v2)
{
    using namespace std;
    std::vector<position> pixels_pos;

    for (auto [start, end] : { pair{ v0, v1 }, pair{ v1, v2 }, pair{ v2, v0 } })
    {
        std::vector<position> line = render_line::pixels_positions(start, end);
        pixels_pos.insert(std::end(pixels_pos), line.begin(), line.end());
    }

    return pixels_pos;
}

void triangle_render::draw_triangles(std::vector<position>& vertexes,
                                     size_t                 num_vertexes,
                                     pixel                  p)
{
    std::vector<position> triangles_edge_pixels;

    for (size_t i = 0; i < num_vertexes / 3; ++i)
    {
        position v0 = vertexes.at(i * 3 + 0);
        position v1 = vertexes.at(i * 3 + 1);
        position v2 = vertexes.at(i * 3 + 2);

        for (auto pixel_pos : pixels_positions_triangle(v0, v1, v2))
        {
            triangles_edge_pixels.push_back(pixel_pos);
        }
    }

    for (auto pos : triangles_edge_pixels)
    {
        set_pixel(pos, p);
    }
}
