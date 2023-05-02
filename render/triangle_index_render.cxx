//
// Created by apachai on 27.04.23.
//

#include "triangle_index_render.hxx"
#include "basic_canvas.hxx"

triangle_indexed_render::triangle_indexed_render(canvas& buffer,
                                                 size_t  width,
                                                 size_t  height)
    : triangle_render(buffer, width, height)
{
}

void triangle_indexed_render::draw_triangles(std::vector<position>& vertexes,
                                             std::vector<uint8_t>&  indexes,
                                             pixel                  p)
{
    std::vector<position> triangles_edge_pixels;

    for (size_t i = 0; i < indexes.size() / 3; ++i)
    {
        uint8_t index0 = indexes[i * 3 + 0];
        uint8_t index1 = indexes[i * 3 + 1];
        uint8_t index2 = indexes[i * 3 + 2];

        position v0 = vertexes.at(index0);
        position v1 = vertexes.at(index1);
        position v2 = vertexes.at(index2);

        std::vector<position> lines = pixels_positions_triangle(v0, v1, v2);

        triangles_edge_pixels.insert(
            std::end(triangles_edge_pixels), lines.begin(), lines.end());
    }

    // apply color to every pixel position
    for (auto pos : triangles_edge_pixels)
    {
        set_pixel(pos, p);
    }
}