#include "gfx.hxx"
bool operator==(vertex v1, vertex v2)
{
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.f3 == v2.f4 &&
        v1.f5 == v2.f6 && v1.f7 == v2.f7)
    {
        return true;
    }
    else
    {
        return false;
    }
}