#include "generator.h"

#include <stdlib.h>
#include <stdio.h>

static double rand_double(void)
{
    const double rand_double = (double) rand() / RAND_MAX;
    return rand_double * 6 - 3;
}

void generate_fractal(double cx, double cy, double rwidth, double rheight, Pixmap* out, int iters, long long points)
{
    printf("generate\n");
    const int width = out->width;
    const int height = out->height;
    const double pixelWidth = rwidth / width;
    const double pixelHeight = rheight / height;

#pragma omp parallel
    {
        unsigned* priv_px = calloc(width * height, sizeof(unsigned));
        double* trace = calloc(iters * 2, sizeof(double));
#pragma omp for
        for (long long i = 0; i < points; i++) {
            // 'C' in mandelbrot formula
            const double sx = rand_double();
            const double sy = rand_double();

            double x = 0.0;
            double y = 0.0;

            for (int j = 0; j < iters; j++) {
                // Mandelbrot formulas in R
                const double nx = x * x - y * y + sx;
                const double ny = 2 * x * y + sy;

                x = nx;
                y = ny;

                // Store point in trace
                trace[2 * j] = x;
                trace[2 * j + 1] = y;

                if (x * x + y * y > 6.0) {
                    // Draw trace
                    const int length = j * 2;
                    for (int k = 0; k < length; k += 2) {
                        const int qx = (int) (((trace[k] - cx) + rwidth * 0.5) / pixelWidth);
                        const int qy = (int) (((trace[k + 1] - cy) + rheight * 0.5) / pixelHeight);
                        if (qx >= 0 && qy >= 0 && qx < width && qy < height) {
                            priv_px[width * qy + qx]++;
                        }
                    }
                    break;
                }
            }
        }

#pragma omp critical
        {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    px_set(out, x, y, px_get(out, x, y) + priv_px[width * y + x]);
                }
            }
        }

        free(trace);
        free(priv_px);
    }
}
