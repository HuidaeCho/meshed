#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdint.h>

#ifdef _MSC_VER
#include <winsock2.h>
/* gettimeofday.c */
int gettimeofday(struct timeval *, struct timezone *);
#else
#include <sys/time.h>
#endif
#include "raster.h"

#define LOOP_THEN_TASK

#define REALLOC_INCREMENT 1024

#define NE 128
#define N 64
#define NW 32
#define W 16
#define SW 8
#define S 4
#define SE 2
#define E 1

#define SHED_NULL INT32_MIN
#define SHED_HIER_NULL -1

struct outlet_list
{
    int nalloc, n;
    int *row, *col;
    int *id;
    /* MESHED only */
    unsigned char *dir;
};

struct shed_hier
{
    int n;
    int *self, *up, *down;
};

#ifdef LOOP_THEN_TASK
#ifdef _MAIN_C_
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL int tracing_stack_size;
#endif

/* timeval_diff.c */
long long timeval_diff(struct timeval *, struct timeval *, struct timeval *);

/* recode.c */
double recode_encoding(double, void *);
double recode_degree(double, void *);

/* outlet_list.c */
void init_outlet_list(struct outlet_list *);
void reset_outlet_list(struct outlet_list *);
void free_outlet_list(struct outlet_list *);
void add_outlet(struct outlet_list *, int, int, int, unsigned char);

/* outlets.c */
struct outlet_list *read_outlets(const char *, const char *, const char *,
                                 const char *, struct raster_map *, int);
int write_outlets(const char *, struct outlet_list *);

/* delineate.c */
void delineate(struct raster_map *, struct outlet_list *, int);

/* delineate_lessmem.c */
void delineate_lessmem(struct raster_map *, struct outlet_list *);

/* delineate_moremem.c */
void delineate_moremem(struct raster_map *, struct outlet_list *);

/* shed_hier.c */
struct shed_hier *analyze_shed_hier(struct raster_map *,
                                    struct outlet_list *);
int write_shed_hier(const char *, struct shed_hier *, const char *);

#endif
