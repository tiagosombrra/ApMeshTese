set size .75,.75
set encoding iso_8859_1
set ylabel "Speed-up"
set xlabel "N\372mero de threads"
#set title "Mont Rainier 1 processo"
set key horizontal bottom center outside samplen 2 width -3 height 2
set style line 1 lc rgb '#525252' lt 1 lw 2 pt -1 ps 1.0   # --- blue = 0060ad
set style line 2 lc rgb '#2b8cbe' lt 1 lw 2 pt 4 ps 1.0   # --- red = dd181f
set style data linespoints
set xrange [0:15]
set yrange [0:15]
set xtics("1" 1,"2" 2, "4" 3, "6" 4, "8" 5, "10" 6, "12" 7, "14" 8, "16" 9, "18" 10, "20" 11, "22" 12, "24" 13, "36" 14, "48" 15)
set grid ytics
set terminal svg
set output 'rainier_speed-up.svg'
plot "speed-up.t" t "Speed-up" with linespoints ls 2, "linear.t" t "Linear" with linespoints ls 1 
set output

# comado para salvar em pdf: inkscape rainier_speed-up.svg --export-pdf=rainier_speed-up1.pdf
