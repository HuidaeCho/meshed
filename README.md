# Memory-Efficient Watershed Delineation (MESHED)

Part of the [Memory-Efficient I/O-Improved Drainage Analysis System (MIDAS)](https://github.com/HuidaeCho/midas)

Flow direction encoding in GeoTIFF:<br>
![image](https://github.com/HuidaeCho/mefa/assets/7456117/6268b904-24a4-482e-8f6d-9ec9c4edf143)

A journal manuscript has been submitted to [Environmental Modelling & Software](https://www.sciencedirect.com/journal/environmental-modelling-and-software).

## Requirements

* C compiler with [OpenMP](https://www.openmp.org/) support
* [GDAL](https://gdal.org/)

For Windows, use [MSYS2](https://www.msys2.org/) and [OSGeo4W](https://trac.osgeo.org/osgeo4w/) to install [GCC](https://gcc.gnu.org/) and [GDAL](https://gdal.org/), respectively.

## How to compile MESHED

```bash
make
```

## Test data

Test data is available at https://data.isnew.info/meshed.html.

## Testing on Windows

```cmd
cd test
pretest_TX.bat
test_TX.bat
```

## Testing on Linux

```bash
cd test
./pretest_TX.sh
./test_TX.sh
```
