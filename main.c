#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

#include "pixmap.h"
#include "generator.h"

static void print_usage(const char* prog)
{
    fprintf(stderr, "Usage: %s r_iter r_point g_iter g_point b_iter b_point\n", prog);
}

long long convert_to_ll(const char* str, int* state)
{
    long long ret = strtoll(str, NULL, 10);
    if (errno == ERANGE || errno == EINVAL) {
        *state = -1;
    }
    return ret;
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    double cx = -0.45;
    double cy = 0.0;

    double rw = 2 * 1.6;
    double rh = 2;

    if (argc < 7) {
        print_usage(argv[0]);
        return 1;
    }

    int state = 0;
    int r_iter = (int) convert_to_ll(argv[1], &state);
    long long r_point = convert_to_ll(argv[2], &state);

    int g_iter = (int) convert_to_ll(argv[3], &state);
    long long g_point = convert_to_ll(argv[4], &state);

    int b_iter = (int) convert_to_ll(argv[5], &state);
    long long b_point = convert_to_ll(argv[6], &state);
    
    if (state == -1) {
        print_usage(argv[0]);
        return 1;
    }

    int iters[] = { r_iter, g_iter, b_iter };
    long long points[] = { r_point, g_point, b_point };
    Pixmap* px[3];

    int i;

    for (i = 0; i < 3; i++) {
        px[i] = px_create(1280, 800);
        generate_fractal(cx, cy, rw, rh, px[i], iters[i], points[i]);
        px_normalize(px[i], 255);
    }

    Pixmap* pxc = px_combine_rgb(px[0], px[1], px[2]);

    px_dump_ppm(pxc, 255, "./buddhabrot.ppm");

    px_destroy(px[0]);
    px_destroy(px[1]);
    px_destroy(px[2]);

    px_destroy(pxc);

    return 0;
}

