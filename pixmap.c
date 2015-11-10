#include "pixmap.h"

#include <stdlib.h>
#include <stdio.h>

Pixmap* px_create(int width, int height)
{
    Pixmap* px = malloc(sizeof(Pixmap));
    px->width = width;
    px->height = height;
    px->buf = calloc(width * height, sizeof(unsigned));

    return px;
}

void px_destroy(Pixmap* px)
{
    free(px->buf);
    free(px);
}

void px_normalize(Pixmap* px, unsigned ceil)
{
    unsigned* buf = px->buf;
    int len = px->width * px->height;
    unsigned max = buf[0];

    for (int i = 1; i < len; i++) {
        if (buf[i] > max) {
            max = buf[i];
        }
    }
        
    for (int i = 0; i < len; i++) {
        unsigned long long m = (unsigned long long) buf[i] * ceil;
        buf[i] = (int) (m / max);
    }
}

unsigned px_get(Pixmap* px, int x, int y)
{
    return px->buf[y * px->width + x];
}

void px_set(Pixmap* px, int x, int y, unsigned v)
{   
    px->buf[y * px->width + x] = v;
}

void px_inc(Pixmap* px, int x, int y)
{
    px->buf[y * px->width + x]++;
}

void px_dump_raw(Pixmap* px, const char* filename)
{
    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        perror("px_dump_raw");
        return;
    }

    for (int y = 0; y < px->height; y++) {
        for (int x = 0; x < px->width; x++) {
            unsigned c = px_get(px, x, y);
            char r = (c >> 16) & 0xff;
            char g = (c >> 8) & 0xff;
            char b = (c >> 0) & 0xff;
            fwrite(&r, 1, 1, f);
            fwrite(&g, 1, 1, f);
            fwrite(&b, 1, 1, f);
        }
    }

    fclose(f);
}

void px_dump_ppm(Pixmap* px, int max, const char* filename)
{
    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        perror("px_dump_ppm");
        return;
    }

    fputs("P3", f);
    fprintf(f, "%d %d\n%d\n", px->width, px->height, max);
    for (int y = 0; y < px->height; y++) {
        for (int x = 0; x < px->width; x++) {
            unsigned c = px_get(px, x, y);
            unsigned r = (c >> 16) & 0xff;
            unsigned g = (c >> 8) & 0xff;
            unsigned b = (c >> 0) & 0xff;
            fprintf(f, "%u %u %u ", r, g, b);
        }
        fputc('\n', f);
    }

    fclose(f);
}

Pixmap* px_combine_rgb(Pixmap* p1, Pixmap* p2, Pixmap* p3)
{
    int len = p1->width * p1->height;
    Pixmap* res = px_create(p1->width, p1->height);

    for (int i = 0; i < len; i++) {
        unsigned r = (p1->buf[i] << 16) ^ (p2->buf[i] << 8) ^ p3->buf[i];
        res->buf[i] = r;
    }

    return res;
}

