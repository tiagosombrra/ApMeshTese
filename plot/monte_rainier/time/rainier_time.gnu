set size 0.75,.75
set encoding iso_8859_1
set ylabel "Tempo de execu\347\343o (segundos)"
set xlabel "N\372mero de threads com um processo"
#set title "Monte Rainier"
set key horizontal bottom center outside samplen 2 width -8 height 2
set style line 2 lc rgb '#2b8cbe' lt 1 lw 1 pt 7 ps 0.5   # --- red = dd181f
set style data linespoints
set xrange [0:16]
set yrange [0:1000]
set xtics("1" 1,"2" 2, "4" 3, "6" 4, "8" 5, "10" 6, "12" 7, "14" 8, "16" 9, "18" 10, "20" 11, "22" 12, "24" 13, "36" 14, "48" 15)
set ytics ("0" 0, "200" 200, "400" 400, "600" 600, "800" 800, "1.000" 1000)
set grid ytics
set terminal svg
set output 'rainier_time.svg'
plot "tempo.t" t "Tempo de execu\347\343o" with linespoints ls 2 
set output

# comado para salvar em pdf: inkscape rainier_time.svg --export-pdf=rainier_time.pdf
