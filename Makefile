ifeq ($(OS),Windows_NT)
	CFLAGS=-Wall -Werror -O3 -fopenmp -I/c/OSGeo4W/include
	GDALLIBS=/c/OSGeo4W/lib/gdal_i.lib
	EXT=.exe
else
	CFLAGS=-Wall -Werror -O3 -fopenmp
	GDALLIBS=`gdal-config --libs`
	EXT=
endif
LDFLAGS=-O3 -fopenmp

all: meshed$(EXT)

clean:
	$(RM) *.o

meshed$(EXT): \
	main.o \
	timeval_diff.o \
	raster.o \
	outlet_list.o \
	outlets.o \
	delineate.o \
	delineate_lessmem.o \
	delineate_moremem.o \
	hierarchy.o
	$(CC) $(LDFLAGS) -o $@ $^ $(GDALLIBS)

*.o: global.h raster.h
delineate_*.o: delineate_funcs.h
