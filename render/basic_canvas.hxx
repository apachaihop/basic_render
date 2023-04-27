#ifndef MY_CANVAS_BASIC_CANVAS_HXX
#define MY_CANVAS_BASIC_CANVAS_HXX

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
#include <vector>

;
#pragma pack(push, 1)

struct pixel
{
    std::uint8_t red   = 0;
    std::uint8_t green = 0;
    std::uint8_t blue  = 0;

    friend bool operator==(pixel p1, pixel p2);
};
#pragma pack(pop)

constexpr pixel red{ 255, 0, 0 };
constexpr pixel green{ 0, 255, 0 };
constexpr pixel blue{ 0, 0, 255 };

class canvas
{
    size_t             width;
    size_t             height;
    std::vector<pixel> pixels;

public:
    canvas(size_t w, size_t h)
        : width(w)
        , height(h)
    {
        pixels.resize(width * height);
    }
    /// Saves your canvas in ppm format
    /// \param file_name-name of the saved file
    /// \return true if file saved sucsesfull
    bool save_image(const std::string& file_name)
    {
        std::ofstream out_file;
        out_file.exceptions(std::ios_base::failbit);
        out_file.open(file_name, std::ios_base::binary);
        out_file << "P6\n" << width << ' ' << height << ' ' << 255 << '\n';
        decltype(auto) buf_size =
            static_cast<std::streamsize>(sizeof(pixel) * pixels.size());
        out_file.write((const char*)pixels.data(), buf_size);
        if (out_file.fail())
        {
            return false;
        }
        return true;
    }
    /// Load in your canvas ppm image
    /// \param file_name -name of the loaded file
    /// \return true if load was sucsesfull
    bool load_image(const std::string& file_name)
    {
        std::ifstream inp_file;
        inp_file.exceptions(std::ios_base::failbit);
        inp_file.open(file_name, std::ios_base::binary);
        std::string header;
        std::string color_format;
        char        last_next_line = 0;
        inp_file >> header >> width >> height >> color_format >>
            std::noskipws >> last_next_line;
        if (!iswspace(last_next_line))
        {
            throw std::runtime_error("exepexted whitespace");
        }
        pixels.resize(width * height);
        decltype(auto) buf_size =
            static_cast<std::streamsize>(sizeof(pixel) * pixels.size());
        inp_file.read((char*)pixels.data(), buf_size);
        if (inp_file.fail())
        {
            return false;
        }
        return true;
    }
    /// Get value of pixel of your canvas
    /// \param x- x coordinate
    /// \param y - y coordinate
    /// \return  pixel value
    pixel get_pixel(size_t x, size_t y)
    {
        if (x > width || y > height)
        {
            throw std::runtime_error("overflow");
        }
        const size_t index = width * y + x;
        return pixels.at(index);
    }
    /// Set pixel on your canvas
    /// \param x- x coordinate
    /// \param y - y coordinate
    /// \param pix - seted pixel
    void set_pixel(size_t x, size_t y, pixel pix)
    {
        if (x > width || y > height)
        {
            throw std::runtime_error("overflow");
        }
        if (x == width)
        {
            x -= 1;
        }
        if (y == height)
        {
            y--;
        }
        const size_t index   = width * y + x;
        pixel&       tar_pix = pixels.at(index);
        tar_pix              = pix;
    }
    bool operator==(const canvas& var) const { return pixels == var.pixels; }
    bool operator!=(const canvas& var) const { return !(*this == var); }
    ///
    /// \return begin-iterator to vector of pixels
    auto begin() { return pixels.begin(); }
    ///
    /// \return end-iterator to vector of pixels
    auto end() { return pixels.end(); }

    std::vector<pixel>& get_pixels() { return pixels; }

    size_t get_width() const { return width; }
    size_t get_height() const { return height; }
    size_t get_size() const { return pixels.size(); }
};
struct position
{
    int32_t         x = 0;
    int32_t         y = 0;
    double          length() const { return std::sqrt(x * x + y * y); }
    friend position operator-(const position& left, const position& right);
    friend bool     operator==(const position& left, const position& right);
    static position generate_random(int width, int height);
};
struct base_render
{
    virtual void                  clear(pixel)                   = 0;
    virtual void                  set_pixel(position, pixel)     = 0;
    virtual std::vector<position> pixels_positions(position start,
                                                   position end) = 0;
    virtual ~base_render() {}
};
#endif // MY_CANVAS_BASIC_CANVAS_HXX
