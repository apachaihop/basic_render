#include "basic_canvas.hxx"
#include "render_line.hxx"

int main()
{
    const pixel black = { 0, 0, 0 };
    const pixel white = { 255, 255, 255 };

    int32_t width  = 320;
    int32_t height = 240;

    canvas image(width, height);
    auto   size = image.get_size();

    render_line render(image, width, height);

    render.clear(black);

    render.draw_line(
        position{ 0, 0 }, position{ height - 1, height - 1 }, white);
    render.draw_line(position{ 50, 0 }, position{ 100, 100 }, red);

    render.draw_line(
        position{ 0, height - 1 }, position{ height - 1, 0 }, green);
    for (size_t i = 0; i < 5; ++i)
    {
        position start{ position::generate_random(width, height) };
        position end{ position::generate_random(width, height) };
        pixel    color{ static_cast<uint8_t>(rand() % 256),
                     static_cast<uint8_t>(rand() % 256),
                     static_cast<uint8_t>(rand() % 256) };
        render.draw_line(start, end, color);
    }
    render.draw_circle({ 50, 50 }, 10, red);
    image.save_image("lines.ppm");
    return 0;
}