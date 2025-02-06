# Memory-Efficient Watershed Delineation (MESHED)

Part of the [Memory-Efficient I/O-Improved Drainage Analysis System (MIDAS)](https://github.com/HuidaeCho/midas)

Please cite [Cho (2025)](https://doi.org/10.1016/j.envsoft.2024.106244) when you use this algorithm or the [r.hydrobasin](https://grass.osgeo.org/grass-stable/manuals/addons/r.hydrobasin.html) [GRASS](https://grass.osgeo.org/) addon.

**Citation**: Huidae Cho, January 2025. Avoid Backtracking and Burn Your Inputs: CONUS-Scale Watershed Delineation Using OpenMP. Environmental Modelling & Software 183, 106244. [doi:10.1016/j.envsoft.2024.106244](https://doi.org/10.1016/j.envsoft.2024.106244). [Author's Version](https://idea.isnew.info/publications/Avoid%20backtracking%20and%20burn%20your%20inputs:%20CONUS-scale%20watershed%20delineation%20using%20OpenMP.pdf).

![image](https://clawrim.isnew.info/wp-content/uploads/2024/07/meshed-flyer.png)

Predefined flow direction encodings in GeoTIFF: power2 (default, r.terraflow, ArcGIS), taudem (d8flowdir), 45degree (r.watershed)<br>
![image](https://github.com/user-attachments/assets/990f0530-fded-4ee5-bfbb-85056a50ca1c)
![image](https://github.com/user-attachments/assets/a02dfc15-a825-4210-82c4-4c9296dafadc)
![image](https://github.com/user-attachments/assets/64f5c65a-c7cc-4e06-a69f-6fccd6435426)

Custom flow direction encoding is also possible by passing `-e E,SE,S,SW,W,NW,N,NE` (e.g., 1,8,7,6,5,4,3,2 for taudem).

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
