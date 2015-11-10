#ifndef PIXMAP_H
#define PIXMAP_H

struct Pixmap {
    int width;
    int height;

    // private data
    unsigned* buf;
};
typedef struct Pixmap Pixmap;

unsigned px_get(Pixmap* px, int x, int y);
void px_set(Pixmap* px, int x, int y, unsigned v);

Pixmap* px_create(int width, int height);
void px_destroy(Pixmap* px);
void px_normalize(Pixmap* px, unsigned ceil);
void px_inc(Pixmap* px, int x, int y);

Pixmap* px_combine_rgb(Pixmap* p1, Pixmap* p2, Pixmap* p3);

void px_dump_raw(Pixmap* px, const char* file);
void px_dump_ppm(Pixmap* px, int max, const char* file);

#endif
