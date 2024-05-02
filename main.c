#define _MAIN_C_

#include <stdlib.h>
#include <string.h>
#include <gdal.h>
#include <cpl_conv.h>
#ifdef _MSC_VER
#include <winsock2.h>
#else
#include <sys/time.h>
#endif
#include "global.h"

int main(int argc, char *argv[])
{
    int i;
    int print_usage = 1, use_lessmem = 0, compress_output = 0, save_outlets =
        0;
    char *dir_path = NULL, *outlets_path = NULL, *id_col =
        NULL, *output_path = NULL, *hier_path = NULL;
    struct raster_map *dir_map;
    struct outlet_list *outlet_l;
    struct timeval start_time, end_time;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            int j, n = strlen(argv[i]);
            int unknown = 0;

            for (j = 1; j < n && !unknown; j++) {
                switch (argv[i][j]) {
                case 'l':
                    use_lessmem = 1;
                    break;
                case 'c':
                    compress_output = 1;
                    break;
                case 'o':
                    save_outlets = 1;
                    break;
                default:
                    unknown = 1;
                    break;
                }
            }
            if (unknown) {
                fprintf(stderr, "%c: Unknown flag\n", argv[i][j]);
                print_usage = 2;
                break;
            }
        }
        else if (!dir_path)
            dir_path = argv[i];
        else if (!outlets_path)
            outlets_path = argv[i];
        else if (!id_col)
            id_col = argv[i];
        else if (!output_path) {
            output_path = argv[i];
            print_usage = 0;
        }
        else if (!hier_path)
            hier_path = argv[i];
        else {
            fprintf(stderr, "%s: Unable to process extra arguments\n",
                    argv[i]);
            print_usage = 2;
            break;
        }
    }

    if (save_outlets && (compress_output || hier_path)) {
        if (compress_output && hier_path)
            fprintf(stderr,
                    "Unable to process -o, -c, and hierarchy.txt at once\n");
        else if (compress_output)
            fprintf(stderr, "Unable to process both -o and -c\n");
        else
            fprintf(stderr, "Unable to process both -o and hierarchy.txt\n");
        print_usage = 2;
    }

    if (print_usage) {
        if (print_usage == 2)
            printf("\n");
        printf
            ("Usage: meshed [-lco] fdr.tif outlets.shp id_col output.ext [hierarchy.txt]\n");
        printf("\n");
        printf("  -l\t\tUse less memory\n");
        printf("  -c\t\tCompress output GeoTIFF file\n");
        printf("  -o\t\tWrite outlet rows and columns, and exit\n");
        printf("  fdr.tif\tInput GeoTIFF file of flow direction raster\n");
        printf("  outlets.shp\tInput Shapefile of outlets\n");
        printf("  id_col\tID column\n");
        printf
            ("  output.ext\tOutput GeoTIFF file for subwatersheds raster\n");
        printf
            ("  \t\tOutput text file for outlet rows and columns with -o\n");
        printf
            ("  hierarchy.txt\tOutput text file for subwatershed hierarchy\n");
        exit(print_usage == 1 ? EXIT_SUCCESS : EXIT_FAILURE);
    }

    GDALAllRegister();

    printf("Reading flow direction raster <%s>...\n", dir_path);
    gettimeofday(&start_time, NULL);
    if (!(dir_map = read_raster(dir_path, RASTER_MAP_TYPE_INT32, 0))) {
        fprintf(stderr, "%s: Failed to read flow direction raster\n",
                dir_path);
        exit(EXIT_FAILURE);
    }
    gettimeofday(&end_time, NULL);
    printf("Input time for flow direction: %lld microsec\n",
           timeval_diff(NULL, &end_time, &start_time));

    printf("Reading outlets <%s>...\n", outlets_path);
    gettimeofday(&start_time, NULL);
    if (!(outlet_l = read_outlets(outlets_path, id_col, dir_map))) {
        fprintf(stderr, "%s: Failed to read outlets\n", outlets_path);
        exit(EXIT_FAILURE);
    }
    gettimeofday(&end_time, NULL);
    printf("Input time for outlets: %lld microsec\n",
           timeval_diff(NULL, &end_time, &start_time));

    if (save_outlets) {
        printf("Writing outlets...\n");
        gettimeofday(&start_time, NULL);
        if (write_outlets(output_path, outlet_l) > 0) {
            fprintf(stderr, "%s: Failed to write outlets file\n",
                    output_path);
            free_raster(dir_map);
            free_outlet_list(outlet_l);
            exit(EXIT_FAILURE);
        }
        gettimeofday(&end_time, NULL);
        printf("Output time for outlets: %lld microsec\n",
               timeval_diff(NULL, &end_time, &start_time));
    }
    else {
        printf("Delineating subwatersheds...\n");
        gettimeofday(&start_time, NULL);
        delineate(dir_map, outlet_l, use_lessmem);
        gettimeofday(&end_time, NULL);
        printf
            ("Computation time for subwatershed delineation: %lld microsec\n",
             timeval_diff(NULL, &end_time, &start_time));

        dir_map->compress = compress_output;
        printf("Writing subwatersheds raster <%s>...\n", output_path);
        gettimeofday(&start_time, NULL);
        if (write_raster(output_path, dir_map, RASTER_MAP_TYPE_AUTO) > 0) {
            fprintf(stderr, "%s: Failed to write subwatersheds raster\n",
                    output_path);
            free_raster(dir_map);
            free_outlet_list(outlet_l);
            exit(EXIT_FAILURE);
        }
        gettimeofday(&end_time, NULL);
        printf("Output time for subwatersheds: %lld microsec\n",
               timeval_diff(NULL, &end_time, &start_time));

        if (hier_path) {
            struct hierarchy *hier;

            printf("Analyzing subwatershed hierarchy...\n");
            gettimeofday(&start_time, NULL);
            hier = analyze_hierarchy(dir_map, outlet_l);
            gettimeofday(&end_time, NULL);
            printf
                ("Analysis time for subwatershed hierarchy: %lld microsec\n",
                 timeval_diff(NULL, &end_time, &start_time));

            if (write_hierarchy(hier_path, hier) > 0) {
                fprintf(stderr,
                        "%s: Failed to write subwatershed hierarchy file\n",
                        hier_path);
                free_raster(dir_map);
                free_outlet_list(outlet_l);
                exit(EXIT_FAILURE);
            }
            gettimeofday(&end_time, NULL);
            printf("Output time for subwatershed hierarchy: %lld microsec\n",
                   timeval_diff(NULL, &end_time, &start_time));
        }
    }

    free_raster(dir_map);
    free_outlet_list(outlet_l);

    exit(EXIT_SUCCESS);
}
