#include <stdio.h>
#include <stdlib.h>
#include "global.h"

void init_outlet_list(struct outlet_list *ol)
{
    ol->nalloc = ol->n = 0;
    ol->row = ol->col = NULL;
    ol->id = NULL;

    /* MESHED only */
    ol->dir = NULL;
}

void reset_outlet_list(struct outlet_list *ol)
{
    ol->n = 0;
}

void free_outlet_list(struct outlet_list *ol)
{
    if (ol->row)
        free(ol->row);
    if (ol->col)
        free(ol->col);
    if (ol->id)
        free(ol->id);

    /* MESHED only */
    if (ol->dir)
        free(ol->dir);

    init_outlet_list(ol);
}

void add_outlet(struct outlet_list *ol, int row, int col, int id,
                unsigned char dir)
{
    if (ol->n == ol->nalloc) {
        ol->nalloc += REALLOC_INCREMENT;
        ol->row = realloc(ol->row, sizeof *ol->row * ol->nalloc);
        ol->col = realloc(ol->col, sizeof *ol->col * ol->nalloc);
        ol->id = realloc(ol->id, sizeof *ol->id * ol->nalloc);

        if (dir)
            /* MESHED only */
            ol->dir = realloc(ol->dir, sizeof *ol->dir * ol->nalloc);

        if (!ol->row || !ol->col || !ol->id || (dir && !ol->dir)) {
            fprintf(stderr, "Unable to increase outlet list");
            exit(EXIT_FAILURE);
        }
    }
    ol->row[ol->n] = row;
    ol->col[ol->n] = col;
    ol->id[ol->n] = id;

    if (dir)
        /* MESHED only */
        ol->dir[ol->n] = dir;

    ol->n++;
}
