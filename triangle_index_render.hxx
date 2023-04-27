//
// Created by apachai on 27.04.23.
//

#ifndef RENDER_TRIANGLE_INDEX_RENDER_HXX
#define RENDER_TRIANGLE_INDEX_RENDER_HXX
#include "triangle_render.hxx"

struct triangle_indexed_render : triangle_render
{
    triangle_indexed_render(canvas& buffer, size_t width, size_t height);

    void draw_triangles(std::vector<position>& vertexes,
                        std::vector<uint8_t>&  indexes,
                        pixel                  p);
};
#endif // RENDER_TRIANGLE_INDEX_RENDER_HXX
