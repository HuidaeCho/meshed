CFLAGS=-Wall -Werror -O3 -fopenmp
LDFLAGS=-O3 -fopenmp

all: meshed

clean:
	$(RM) *.o

meshed: main.o timeval_diff.o raster.o outlet_list.o outlets.o delineate.o hierarchy.o
	$(CC) $(LDFLAGS) -o $@ $^ `gdal-config --libs`

%.o: %.c
	$(CC) $(CFLAGS) -c $<
