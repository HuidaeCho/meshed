#include <stdio.h>
#include <stdlib.h>
#include <gdal.h>
#include "global.h"

#define DIR(row, col) dir_map->cells.int32[(size_t)(row) * dir_map->ncols + (col)]

static void transform(struct raster_map *, double, double, int *, int *);

struct outlet_list *read_outlets(char *outlets_path, char *id_col,
                                 struct raster_map *dir_map)
{
    struct outlet_list *outlet_l = malloc(sizeof *outlet_l);
    GDALDatasetH dataset;
    OGRLayerH layer;
    OGRFeatureDefnH feature_def;
    int id_field_idx;
    OGRFieldDefnH id_field_def;
    int id_field_type;
    OGRFeatureH feature;

    if (!
        (dataset =
         GDALOpenEx(outlets_path, GDAL_OF_VECTOR, NULL, NULL, NULL)))
        return NULL;

    if (!GDALDatasetGetLayerCount(dataset))
        return NULL;

    layer = GDALDatasetGetLayer(dataset, 0);

    init_outlet_list(outlet_l);

    feature_def = OGR_L_GetLayerDefn(layer);
    if ((id_field_idx = OGR_FD_GetFieldIndex(feature_def, id_col)) < 0)
        return NULL;
    id_field_def = OGR_FD_GetFieldDefn(feature_def, id_field_idx);
    if ((id_field_type = OGR_Fld_GetType(id_field_def)) != OFTInteger)
        return NULL;

    while ((feature = OGR_L_GetNextFeature(layer))) {
        OGRGeometryH geometry;
        int id;
        double x, y;

        geometry = OGR_F_GetGeometryRef(feature);
        if (geometry &&
            wkbFlatten(OGR_G_GetGeometryType(geometry)) == wkbPoint) {
            int row, col;

            x = OGR_G_GetX(geometry, 0);
            y = OGR_G_GetY(geometry, 0);
            id = OGR_F_GetFieldAsInteger(feature, id_field_idx);

            transform(dir_map, x, y, &row, &col);

            /* if the outlet is outside the computational region, skip */
            if (row >= 0 && row < dir_map->nrows && col >= 0 &&
                col < dir_map->ncols)
                add_outlet(outlet_l, row, col, id, DIR(row, col));
            else
                printf
                    ("Skip outlet %d at (%d, %d) outside the current region\n",
                     id, row, col);
        }

        OGR_F_Destroy(feature);
    }

    GDALClose(dataset);

    return outlet_l;
}

int write_outlets(const char *outlets_path, struct outlet_list *outlet_l)
{
    FILE *fp;
    int i;

    if (!(fp = fopen(outlets_path, "w")))
        return 1;

    for (i = 0; i < outlet_l->n; i++)
        fprintf(fp, "%d %d %d\n", outlet_l->row[i], outlet_l->col[i],
                outlet_l->id[i]);

    fclose(fp);

    return 0;
}

static void transform(struct raster_map *dir_map, double x, double y,
                      int *row, int *col)
{
    *row = (dir_map->geotransform[4] * x - dir_map->geotransform[1] * y
            - dir_map->geotransform[4] * dir_map->geotransform[0]
            + dir_map->geotransform[1] * dir_map->geotransform[3]) /
        (dir_map->geotransform[4] * dir_map->geotransform[2]
         - dir_map->geotransform[1] * dir_map->geotransform[5]);
    *col = (dir_map->geotransform[5] * x - dir_map->geotransform[2] * y
            - dir_map->geotransform[5] * dir_map->geotransform[0]
            + dir_map->geotransform[2] * dir_map->geotransform[3]) /
        (dir_map->geotransform[5] * dir_map->geotransform[1]
         - dir_map->geotransform[2] * dir_map->geotransform[4]);
}
