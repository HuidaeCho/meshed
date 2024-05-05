# Memory-Efficient Watershed Delineation (MESHED)

Part of Memory-efficient Input Drainage paths Analysis System (MIDAS)

```bash
# build meshed
make

# download test data
mkdir inputs outputs
for i in \
  fdr.tif \
  outlets1000.dbf \
  outlets1000.prj \
  outlets1000.shp \
  outlets1000.shx \
  ; do
  curl https://data.isnew.info/meidas/$i -o inputs/$i
done

# run the more-memory version
meshed inputs/fdr.tif inputs/outlets1000.shp cat outputs/wsheds1000.tif

# run the less-memory version
meshed -l inputs/fdr.tif inputs/outlets1000.shp cat outputs/wsheds1000-lessmem.tif

# compare the two outputs
diff inputs/wsheds1000*
```
