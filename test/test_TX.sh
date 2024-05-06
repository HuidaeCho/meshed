#!/bin/sh
export PATH="..:$PATH"
for o in inputs_TX/outlets*.shp; do
	w=$(basename $o | sed 's/outlets/wsheds/; s/shp/tif/')
	meshed -c inputs_TX/fdr.tif $o cat outputs_TX/$w
done
