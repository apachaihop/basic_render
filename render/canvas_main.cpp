#include "basic_canvas.hxx"
#include <exception>
#include <iostream>

int main()
{
    size_t width  = 320;
    size_t height = 240;

    canvas image(width, height);

    std::fill(image.begin(), image.end(), green);

    const char* file_name = "green_image.ppm";
    try
    {
        if (!image.save_image(file_name))
        {
            throw std::runtime_error("Cannot save image");
        };
    }
    catch (std::exception& error)
    {
        std::cout << error.what() << std::endl;
    }
    canvas image_loaded(0, 0);
    try
    {
        image_loaded.load_image(file_name);
    }
    catch (std::runtime_error& err)
    {
        std::cout << err.what();
    }

    if (image != image_loaded)
    {
        std::cerr << "image != image_loaded\n";
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "image == image_loaded\n";
    }

    return 0;
}
