#include "render_line.hxx"
#include "basic_canvas.hxx"

#include <algorithm>

render_line::render_line(canvas& buff, size_t width, size_t height)
    : buffer(buff)
    , w(width)
    , h(height)
{
}

void render_line::clear(pixel p)
{
    std::fill(buffer.begin(), buffer.end(), p);
}

void render_line::set_pixel(position pos, pixel p)
{
    buffer.set_pixel(pos.x, pos.y, p);
}

std::vector<position> render_line::pixels_positions(position start,
                                                    position end)
{
    std::vector<position> result;
    int                   x0            = start.x;
    int                   x1            = end.x;
    int                   y0            = start.y;
    int                   y1            = end.y;
    auto                  plot_line_low = [&](int x0, int y0, int x1, int y1)
    {
        int dx = x1 - x0;
        int dy = y1 - y0;
        int yi = 1;
        if (dy < 0)
        {
            yi = -1;
            dy = -dy;
        }
        int D = 2 * dy - dx;
        int y = y0;

        for (int x = x0; x <= x1; ++x)
        {
            result.push_back(position{ x, y });
            if (D > 0)
            {
                y += yi;
                D -= 2 * dx;
            }
            D += 2 * dy;
        }
    };

    auto plot_line_high = [&](int x0, int y0, int x1, int y1)
    {
        int dx = x1 - x0;
        int dy = y1 - y0;
        int xi = 1;
        if (dx < 0)
        {
            xi = -1;
            dx = -dx;
        }
        int D = 2 * dx - dy;
        int x = x0;

        for (int y = y0; y <= y1; ++y)
        {
            result.push_back(position{ x, y });
            if (D > 0)
            {
                x += xi;
                D -= 2 * dy;
            }
            D += 2 * dx;
        }
    };

    if (std::abs(y1 - y0) <= std::abs(x1 - x0))
    {
        if (x0 > x1)
        {
            plot_line_low(x1, y1, x0, y0);
        }
        else
        {
            plot_line_low(x0, y0, x1, y1);
        }
    }
    else
    {
        if (y0 > y1)
        {
            plot_line_high(x1, y1, x0, y0);
        }
        else
        {
            plot_line_high(x0, y0, x1, y1);
        }
    }
    return result;
}
void render_line::draw_line(position start, position end, pixel p)
{
    std::vector<position> line = pixels_positions(start, end);
    std::for_each(
        line.begin(), line.end(), [&](auto& pos) { set_pixel(pos, p); });
}
std::vector<position> render_line::pixels_positions_circle(position center,
                                                           int      r)
{
    std::vector<position> result;
    int                   X1    = center.x;
    int                   Y1    = center.y;
    int                   x     = 0;
    int                   y     = r;
    int                   delta = 1 - 2 * r;
    int                   error = 0;
    while (y >= x)
    {
        result.push_back(position{ X1 + x, Y1 + y });
        result.push_back(position{ X1 + x, Y1 - y });
        result.push_back(position{ X1 - x, Y1 + y });
        result.push_back(position{ X1 - x, Y1 - y });
        result.push_back(position{ X1 + y, Y1 + x });
        result.push_back(position{ X1 + y, Y1 - x });
        result.push_back(position{ X1 - y, Y1 + x });
        result.push_back(position{ X1 - y, Y1 - x });
        error = 2 * (delta + y) - 1;
        if ((delta < 0) && (error <= 0))
        {
            delta += 2 * ++x + 1;
            continue;
        }
        if ((delta > 0) && (error > 0))
        {
            delta -= 2 * y-- + 1;
            continue;
        }
        delta += 2 * (++x - --y);
    }
    return result;
}

void render_line::draw_circle(position center, int r, pixel p)
{
    auto circle = pixels_positions_circle(center, r);
    std::for_each(
        circle.begin(), circle.end(), [&](auto& pos) { set_pixel(pos, p); });
}
