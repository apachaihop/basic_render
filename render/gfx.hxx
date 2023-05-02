#ifndef RENDER_GFX_HXX
#define RENDER_GFX_HXX
#include "triangle_index_render.hxx"

struct vertex
{
    double      x  = 0; /// x
    double      y  = 0; /// y
    double      z  = 0; /// z
    double      f3 = 0; /// r
    double      f4 = 0; /// g
    double      f5 = 0; /// b
    double      f6 = 0; /// u (texture coordinate)
    double      f7 = 0; /// v (texture coordinate)
    friend bool operator==(vertex v1, vertex v2);
};

struct uniforms
{
    double  f0 = 0;
    double  f1 = 0;
    vertex  f2;
    vertex  f3;
    vertex  f4;
    double  f5       = 0;
    double  f6       = 0;
    double  f7       = 0;
    canvas* texture0 = nullptr;
};

struct gfx_program
{
    virtual void   set_uniforms(const uniforms&)  = 0;
    virtual vertex vertex_shader(const vertex&)   = 0;
    virtual pixel  fragment_shader(const vertex&) = 0;
};

#endif // RENDER_GFX_HXX
