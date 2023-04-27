#include "basic_canvas.hxx"
position operator-(const position& left, const position& right)
{
    return { left.x - right.x, left.y - right.y };
}

bool operator==(const position& left, const position& right)
{
    return left.x == right.x && left.y == right.y;
}

position position::generate_random(int width, int height)
{
    std::random_device                     rd;
    std::mt19937                           mt(rd());
    std::random_device                     rd1;
    std::mt19937                           mt1(rd1());
    std::uniform_real_distribution<double> dist_width(1.0, width - 1);
    std::uniform_real_distribution<double> dist_height(1.0, height - 1);
    return { (int32_t)dist_width(mt1), (int32_t)dist_height(mt) };
}

bool operator==(pixel p1, pixel p2)
{
    return p1.red == p2.red && p1.green == p2.green && p1.blue == p2.blue;
}
