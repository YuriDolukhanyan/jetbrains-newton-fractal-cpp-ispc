#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include "newton_ispc.h"

int main(int argc, char* argv[]) {
    int width = 1655, height = 1655;
    // take the second argument as \'n\'
    // if missing by default 5
    int n = (argc > 1) ? std::stoi(argv[1]) : 5;
    const int max_iter = 100;
    const float epsilon = 1e-3f;
    const float x_min = -2.0f, x_max = 2.0f;
    const float y_min = -2.0f, y_max = 2.0f;

    // prepare n-th roots
    std::vector<float> roots_real(n), roots_imag(n);
    for (int k = 0; k < n; ++k) {
        float angle = 2.0f * M_PI * k / n;
        roots_real[k] = std::cos(angle);
        roots_imag[k] = std::sin(angle);
    }

    std::vector<int> output(width * height, 0);

    // call ispc function
    ispc::compute_fractal(width, height, n,
                          x_min, x_max, y_min, y_max,
                          max_iter, epsilon,
                          roots_real.data(), roots_imag.data(), n,
                          output.data());

    // write the ppm image
    std::string filename = "newton_" + std::to_string(n) + ".ppm";
    std::ofstream img(filename);
    if (!img.is_open()) {
        std::cerr << "error: cannot open output file.\n";
        return 1;
    }

    img << "P3\n" << width << " " << height << "\n255\n";

    for (int idx = 0; idx < width * height; ++idx) {
        int root = (output[idx] >> 8) & 0xFF;  // assumed packing: root in bits [8..15]
        int iter = output[idx] & 0xFF;         // iterations in [0..7]
        float hue = static_cast<float>(root) / n;
        float brightness = 1.0f - static_cast<float>(iter) / max_iter;

        // hue-based rgb mapping with 120 degree phase shifts
        int r = static_cast<int>(255 * fabs(std::sin(hue * 2 * M_PI)));
        int g = static_cast<int>(255 * fabs(std::sin(hue * 2 * M_PI + 2 * M_PI / 3)));
        int b = static_cast<int>(255 * fabs(std::sin(hue * 2 * M_PI + 4 * M_PI / 3)));

        r = static_cast<int>(r * brightness);
        g = static_cast<int>(g * brightness);
        b = static_cast<int>(b * brightness);

        img << r << " " << g << " " << b << " ";
    }

    img.close();
    std::cout << "newton fractal was saved to " << filename << " \n";
    return 0;
}
