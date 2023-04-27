#include "triangle_interpolated.hxx"
#include <algorithm>
#include <array>
/// base func for interpolating
/// \param f0- first var
/// \param f1 - second var
/// \param t -coefficient from 0 to 1
/// \return new value that lie between f0 and f1
double interpolate(const double f0, const double f1, const double t)
{
    assert(t >= 0.0);
    assert(t <= 1.0);
    return f0 + (f1 - f0) * t;
}
///  interpolate each parametr of vertexes
/// \param v0  first vertex
/// \param v1 second vertex
/// \param t coefficient from 0 to 1
/// \return new vertex that is the result of interpolation
vertex interpolate(const vertex& v0, const vertex& v1, const double t)
{
    return { interpolate(v0.x, v1.x, t),   interpolate(v0.y, v1.y, t),
             interpolate(v0.z, v1.z, t),   interpolate(v0.f3, v1.f3, t),
             interpolate(v0.f4, v1.f4, t), interpolate(v0.f5, v1.f5, t),
             interpolate(v0.f6, v1.f6, t), interpolate(v0.f7, v1.f7, t) };
}

triangle_interpolated::triangle_interpolated(canvas& buffer,
                                             size_t  width,
                                             size_t  height)
    : triangle_indexed_render(buffer, width, height)
{
}
/// get interpolated vertexes of some HORIZONTAL line
/// \param left_vertex
/// \param right_vertex
/// \param out- vector of vertexes for this line
void triangle_interpolated::raster_one_horizontal_line(
    const vertex&        left_vertex,
    const vertex&        right_vertex,
    std::vector<vertex>& out)
{
    size_t num_of_pixels_in_line = static_cast<size_t>(
        std::round(std::abs(left_vertex.x - right_vertex.x)));
    if (num_of_pixels_in_line > 0)
    {
        for (size_t p = 0; p <= num_of_pixels_in_line + 1; ++p)
        {
            double t_pixel =
                static_cast<double>(p) / (num_of_pixels_in_line + 1);
            vertex pixel = interpolate(left_vertex, right_vertex, t_pixel);

            out.push_back(pixel);
        }
    }
    else
    {
        out.push_back(left_vertex);
    }
}
/// Raster triangel that has horizantal line
/// \param single - vertex that dont lie on horizontal line
/// \param left -left vertex of horizontal line
/// \param right - right vertex of horizantal line
/// \return  vextor of all vertexes included in this triangle
std::vector<vertex> triangle_interpolated::raster_horizontal_triangle(
    const vertex& single, const vertex& left, const vertex& right)
{
    std::vector<vertex> out;
    size_t              num_of_hlines =
        static_cast<size_t>(std::round(std::abs(single.y - left.y)));

    if (num_of_hlines > 0)
    {
        for (size_t i = 0; i <= num_of_hlines; ++i)
        {
            double t_vertical   = static_cast<double>(i) / num_of_hlines;
            vertex left_vertex  = interpolate(left, single, t_vertical);
            vertex right_vertex = interpolate(right, single, t_vertical);

            raster_one_horizontal_line(left_vertex, right_vertex, out);
        }
    }
    else
    {
        raster_one_horizontal_line(left, right, out);
    }

    return out;
}
/// Raster tringel by dividing it into two horizontal triangles
/// \param v0 vertex0
/// \param v1 vertex1
/// \param v2 vertex2
/// \return vextor of all vertexes included in this triangle
std::vector<vertex> triangle_interpolated::rasterize_triangle(const vertex& v0,
                                                              const vertex& v1,
                                                              const vertex& v2)
{
    std::vector<vertex> out;

    std::array<const vertex*, 3> in_vertexes{ &v0, &v1, &v2 };
    std::sort(begin(in_vertexes),
              end(in_vertexes),
              [](const vertex* left, const vertex* right)
              { return left->y < right->y; });

    const vertex& top    = *in_vertexes.at(0);
    const vertex& middle = *in_vertexes.at(1);
    const vertex& bottom = *in_vertexes.at(2);

    position start{ static_cast<int32_t>(std::round(top.x)),
                    static_cast<int32_t>(std::round(top.y)) };
    position end{ static_cast<int32_t>(std::round(bottom.x)),
                  static_cast<int32_t>(std::round(bottom.y)) };
    position middle_pos{ static_cast<int32_t>(std::round(middle.x)),
                         static_cast<int32_t>(std::round(middle.y)) };

    if (start == end)
    {
        position delta        = start - middle_pos;
        size_t   count_pixels = std::abs(delta.x) + std::abs(delta.y);
        for (size_t i = 0; i < count_pixels; ++i)
        {
            double t      = static_cast<double>(i) / count_pixels;
            vertex vertex = interpolate(top, middle, t);
            out.push_back(vertex);
        }

        return out;
    }

    if (start == middle_pos)
    {
        position delta        = start - end;
        size_t   count_pixels = std::abs(delta.x) + std::abs(delta.y) + 1;
        for (size_t i = 0; i < count_pixels; ++i)
        {
            double t      = static_cast<double>(i) / count_pixels;
            vertex vertex = interpolate(top, bottom, t);
            out.push_back(vertex);
        }

        return out;
    }

    if (end == middle_pos)
    {
        position delta        = start - middle_pos;
        size_t   count_pixels = std::abs(delta.x) + std::abs(delta.y);
        for (size_t i = 0; i < count_pixels; ++i)
        {
            double t      = static_cast<double>(i) / count_pixels;
            vertex vertex = interpolate(top, middle, t);
            out.push_back(vertex);
        }

        return out;
    }

    std::vector<position> longest_side_line = pixels_positions(start, end);

    auto it_middle = std::find_if(
        begin(longest_side_line),
        std::end(longest_side_line),
        [&](const position& pos)
        { return pos.y == static_cast<int32_t>(std::round(middle.y)); });
    assert(it_middle != std::end(longest_side_line));
    position second_middle = *it_middle;

    double t{ 0 };
    double end_start = (end - start).length();
    if (end_start > 0)
    {
        double middle_start = (second_middle - start).length();
        t                   = middle_start / end_start;
    }
    
    vertex second_middle_vertex = interpolate(top, bottom, t);

    std::vector<vertex> top_triangle =
        raster_horizontal_triangle(top, middle, second_middle_vertex);

    std::vector<vertex> bottom_triangle =
        raster_horizontal_triangle(bottom, middle, second_middle_vertex);

    out.insert(std::end(out), begin(top_triangle), std::end(top_triangle));

    out.insert(
        std::end(out), begin(bottom_triangle), std::end(bottom_triangle));

    return out;
}
/// Draw on canvas rastrizied triangle
/// \param vertexes vector of vertexes
/// \param indexes  vector of indexes of vertexes
void triangle_interpolated::draw_triangles(std::vector<vertex>&   vertexes,
                                           std::vector<uint16_t>& indexes)
{
    for (size_t index = 0; index < indexes.size(); index += 3)
    {
        const uint16_t index0 = indexes.at(index + 0);
        const uint16_t index1 = indexes.at(index + 1);
        const uint16_t index2 = indexes.at(index + 2);

        const vertex& v0 = vertexes.at(index0);
        const vertex& v1 = vertexes.at(index1);
        const vertex& v2 = vertexes.at(index2);

        const std::vector<vertex> interpoleted{ rasterize_triangle(
            v0, v1, v2) };
        for (const vertex& interpolated_vertex : interpoleted)
        {
            const pixel p =
                pixel{ static_cast<uint8_t>(interpolated_vertex.f3),
                       static_cast<uint8_t>(interpolated_vertex.f4),
                       static_cast<uint8_t>(interpolated_vertex.f5) };
            const position pos{
                static_cast<int32_t>(std::round(interpolated_vertex.x)),
                static_cast<int32_t>(std::round(interpolated_vertex.y))
            };
            set_pixel(pos, p);
        }
    }
}
