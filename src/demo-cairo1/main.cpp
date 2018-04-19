#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <cairo/cairo-svg.h>
#include <iostream>

using std::cout;
using std::endl;

struct item;

typedef void (* show_func) (item & it, char const * extra);

struct item
{
    cairo_surface_t * surface;
    cairo_t *         context;
    show_func         show;
    char const *      extra;
};

typedef item * iterator_type;

static void show_image (item & it, char const * filename)
{
    cairo_surface_write_to_png(it.surface, filename);
}

static void show_pdf (item & it, char const *)
{
    cairo_show_page(it.context);
}

static void show_svg (item & it, char const *)
{
    cairo_show_page(it.context);
}

static void create_contexts (iterator_type begin, iterator_type end)
{
    for (iterator_type it = begin; it != end; ++it) {
        it->context = cairo_create(it->surface);
    }
}

static void draw (iterator_type begin, iterator_type end)
{
    for (iterator_type it = begin; it != end; ++it) {
        cairo_set_source_rgb(it->context, 0, 0, 0);
        cairo_select_font_face(it->context
                , "Sans"
                , CAIRO_FONT_SLANT_NORMAL
                , CAIRO_FONT_WEIGHT_NORMAL);

        cairo_set_font_size(it->context, 40.0);
        cairo_move_to(it->context, 10.0, 50.0);
        cairo_show_text(it->context, "Hello, World!");
        
        it->show(*it, it->extra);
    }
}

static void destroy (iterator_type begin, iterator_type end)
{
    for (iterator_type it = begin; it != end; ++it) {
        cairo_destroy(it->context);
        cairo_surface_destroy(it->surface);
    }
}

int main (void)
{
    item items[] = {
          { 0, 0, & show_image, "demo-cairo1-image.png" }
        , { 0, 0, & show_pdf, 0 }
        , { 0, 0, & show_svg, 0 }
    };
    
    int count = sizeof(items)/sizeof(items[0]);
            
    items[0].surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 390, 60);
    items[1].surface = cairo_pdf_surface_create("demo-cairo1-pdffile.pdf", 504, 648);
    items[2].surface = cairo_svg_surface_create("demo-cairo1-svgfile.svg", 390, 60);
    
    iterator_type begin = items;
    iterator_type end = & items[0] + count;
    
    create_contexts(begin, end);
    draw(begin, end);
    destroy(begin, end);

    return 0;
}
