ifeq ($(OS),Windows_NT)
	GDAL_CFLAGS=-I/c/OSGeo4W/include
	GDAL_LIBS=/c/OSGeo4W/lib/gdal_i.lib
	EXT=.exe
else
	GDAL_LIBS=`gdal-config --libs`
endif
CFLAGS=-Wall -Werror -O3 -fopenmp $(GDAL_CFLAGS)
LDFLAGS=-O3 -fopenmp -lm

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
	$(CC) $(LDFLAGS) -o $@ $^ $(GDAL_LIBS)

*.o: global.h raster.h
delineate_*.o: delineate_funcs.h
