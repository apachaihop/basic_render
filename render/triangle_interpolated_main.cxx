#include "triangle_interpolated.hxx"

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <bitset>
#include <cstdint>

int main()
{
    const pixel black = { 0, 0, 0 };

    constexpr size_t width  = 320;
    constexpr size_t height = 240;

    SDL_Window* window =
        SDL_CreateWindow("triangle", width, height, SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        std::cerr << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, "opengl", SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        std::cerr << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    canvas                image(width, height);
    triangle_interpolated interpolated_render(image, width, height);

    interpolated_render.clear(black);

    //                                x  y              r  g  b  tx ty
    std::vector<vertex> triangle_v{ { 100, 0, 1, 255, 0, 0, 0 },
                                    { 0, 239, 0, 0, 255, 0, 1 },
                                    { 319, 239, 0, 0, 0, 255, 2 } };
    // clang-format on
    std::vector<uint16_t> indexes_v{ { 0, 1, 2 } };
    struct program : public gfx_program
    {

        uniforms uniform;
        void     set_uniforms(const uniforms& u) override { uniform = u; }
        vertex   vertex_shader(const vertex& in)
        {
            // Здесь нормируй
            vertex out = in;
            out.x -= (320 / 2);
            out.y -= (240 / 2);

            out.x *= 0.5;
            out.y *= 0.5;

            double alpha = 3.14159 * uniform.f7 * -1;
            double x     = std::cos(alpha) * out.x - std::sin(alpha) * out.y;
            double y     = std::sin(alpha) * out.x + std::cos(alpha) * out.y;
            double z     = out.z;
            out.x        = x;
            out.y        = y;
            out.z        = z;

            x = out.x;
            y = out.y * std::cos(uniform.f0) + out.z * std::sin(uniform.f0);
            z = -out.y * std::sin(uniform.f0) + out.z * std::cos(uniform.f0);
            out.x = x;
            out.y = y;
            out.z = z;

            x = out.x * std::cos(uniform.f1) + out.z * std::sin(uniform.f1);
            y = out.y;
            z = -out.x * std::sin(uniform.f1) + out.z * std::cos(uniform.f1);
            out.x = x;
            out.y = y;
            out.z = z;

            out.x += (320 / 2);
            out.y += (240 / 2);
            return out;
        }
        pixel fragment_shader(const vertex& in)
        {
            pixel res;
            res.red += static_cast<std::uint8_t>(abs(in.f3 / sin(uniform.f7)));
            res.green +=
                static_cast<std::uint8_t>(abs(in.f4 / sin(uniform.f7)));
            res.blue += static_cast<std::uint8_t>(abs(in.f5 / sin(uniform.f7)));
            return res;
        }
    } program1;
    void*     pixels = image.get_pixels().data();
    const int depth  = sizeof(pixel) * 8;
    const int pitch  = width * sizeof(pixel);
    const int rmask  = 0x000000ff;
    const int gmask  = 0x0000ff00;
    const int bmask  = 0x00ff0000;
    const int amask  = 0;

    interpolated_render.set_gfx(reinterpret_cast<gfx_program&>(program1));

    bool      continue_loop = true;
    double    beta          = 0;
    double    gamma         = 0;
    SDL_Event e;
    while (continue_loop)
    {

        interpolated_render.clear(black);
        double time_from_start = SDL_GetTicks() / 1000.0;

        while (SDL_PollEvent(&e))
        {

            if (e.type == SDL_EVENT_QUIT)
            {
                continue_loop = false;
                break;
            }
            if (e.type == SDL_EVENT_KEY_DOWN)
            {
                if (e.key.keysym.sym == SDLK_w)
                {
                    beta += 3.14159 / 10;
                }
                if (e.key.keysym.sym == SDLK_s)
                {
                    beta -= 3.14159 / 10;
                }
                if (e.key.keysym.sym == SDLK_d)
                {
                    gamma += 3.14159 / 10;
                }
                if (e.key.keysym.sym == SDLK_a)
                {
                    gamma -= 3.14159 / 10;
                }
            }
        }

        interpolated_render.clear(black);
        program1.set_uniforms(uniforms{ beta,
                                        gamma,
                                        triangle_v[0],
                                        triangle_v[1],
                                        triangle_v[2],
                                        static_cast<double>(triangle_v.size()),
                                        0,
                                        time_from_start });

        interpolated_render.draw_triangles(triangle_v, indexes_v);

        SDL_Surface* bitmapSurface = SDL_CreateSurfaceFrom(
            pixels, width, height, pitch, SDL_PIXELFORMAT_RGB24);
        if (bitmapSurface == nullptr)
        {
            std::cerr << "Failed to SDL_CreateSurfaceFrom(" << pixels << ", "
                      << width << ", " << height << ", " << pitch << ", "
                      << SDL_GetPixelFormatName(SDL_PIXELFORMAT_RGB888) << ") "
                      << SDL_GetError() << std::endl;
            SDL_RendererInfo renderer_info{};
            if (0 == SDL_GetRendererInfo(renderer, &renderer_info))
            {
                std::cerr << "name: " << renderer_info.name << '\n'
                          << "flags: " << std::bitset<32>(renderer_info.flags)
                          << '\n';
                for (int i = 0; i < renderer_info.num_texture_formats; i++)
                {
                    std::cerr << "supported texture format: "
                              << SDL_GetPixelFormatName(
                                     renderer_info.texture_formats[i])
                              << '\n';
                }
                std::cerr << "max texture width: "
                          << renderer_info.max_texture_width << '\n'
                          << "max texture height: "
                          << renderer_info.max_texture_height << '\n';
            }
            return EXIT_FAILURE;
        }
        SDL_Texture* bitmapTex =
            SDL_CreateTextureFromSurface(renderer, bitmapSurface);
        if (bitmapTex == nullptr)
        {
            std::cerr << SDL_GetError() << std::endl;
            return EXIT_FAILURE;
        }
        SDL_DestroySurface(bitmapSurface);

        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, bitmapTex, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(bitmapTex);
    }
    return 0;
}
