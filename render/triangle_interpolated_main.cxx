#include "triangle_interpolated.hxx"

int main(int, char**)
{
    const pixel black = { 0, 0, 0 };

    constexpr size_t width  = 320;
    constexpr size_t height = 240;

    canvas image(width, height);

    triangle_interpolated interpolated_render(image, width, height);

    interpolated_render.clear(black);

    std::vector<vertex> triangle_v{ { 100, 0, 1, 255, 0, 0 },
                                    { 0, 239, 0, 0, 255, 0 },
                                    { 319, 239, 0, 0, 0, 255 } };

    std::vector<uint16_t> indexes_v{ 0, 1, 2 };

    interpolated_render.draw_triangles(triangle_v, indexes_v);

    image.save_image("interpolated.ppm");

    return 0;
}
