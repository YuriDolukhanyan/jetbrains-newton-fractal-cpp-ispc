export void compute_fractal(uniform int width,
                            uniform int height,
                            uniform int n,
                            uniform float x_min,
                            uniform float x_max,
                            uniform float y_min,
                            uniform float y_max,
                            uniform int max_iter,
                            uniform float epsilon,
                            uniform float roots_real[],
                            uniform float roots_imag[],
                            uniform int num_roots,
                            uniform int output[])
{
    foreach (j = 0 ... height) {
        for (int i = 0; i < width; ++i) {
            // map pixel coordinates (i, j) to complex plane coordinates (zr, zi)
            float zr = x_min + (x_max - x_min) * ((float)i / (width - 1));
            float zi = y_min + (y_max - y_min) * ((float)j / (height - 1));

            int iter = 0;
            int root_index = -1;

            while (iter < max_iter) {
                // f(z) = z^n - 1
                float zr_pow = zr, zi_pow = zi;
                for (int k = 1; k < n; ++k) {
                    float tmp_real = zr_pow * zr - zi_pow * zi;
                    float tmp_imag = zr_pow * zi + zi_pow * zr;
                    zr_pow = tmp_real;
                    zi_pow = tmp_imag;
                }
                float fzr = zr_pow - 1.0f;
                float fzi = zi_pow;

                // f'(z) = n * z^(n-1)
                float zr_der = zr, zi_der = zi;
                for (int k = 1; k < n - 1; ++k) {
                    float tmp_real = zr_der * zr - zi_der * zi;
                    float tmp_imag = zr_der * zi + zi_der * zr;
                    zr_der = tmp_real;
                    zi_der = tmp_imag;
                }
                zr_der *= n;
                zi_der *= n;

                // newton iteration: z = z - f / f'
                float denom = zr_der * zr_der + zi_der * zi_der;
                float real_num = fzr * zr_der + fzi * zi_der;
                float imag_num = fzi * zr_der - fzr * zi_der;
                zr -= real_num / denom;
                zi -= imag_num / denom;

                // check convergence
                for (int r = 0; r < num_roots; ++r) {
                    float dr = zr - roots_real[r];
                    float di = zi - roots_imag[r];
                    if (dr * dr + di * di < epsilon * epsilon) {
                        root_index = r;
                        break;
                    }
                }
                if (root_index != -1) break;
                iter++;
            }

            // final output at each step
            output[j * width + i] = (root_index << 8) | (iter & 0xFF);
        }
    }
}
