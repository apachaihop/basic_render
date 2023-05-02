//
// Created by apachai on 26.04.23.
//

#ifndef RENDER_RENDER_LINE_HXX
#define RENDER_RENDER_LINE_HXX
#include "basic_canvas.hxx"

class render_line : public base_render
{
public:
    render_line(canvas& buffer, size_t width, size_t height);
    void                  clear(pixel) override;
    void                  set_pixel(position, pixel) override;
    std::vector<position> pixels_positions(position start,
                                           position end) override;
    void                  draw_line(position start, position end, pixel);
    std::vector<position> pixels_positions_circle(position center, int r);
    void                  draw_circle(position center, int r, pixel p);

private:
    canvas&      buffer;
    const size_t w;
    const size_t h;
};

#endif // RENDER_RENDER_LINE_HXX
