#set size 0.75,.75
set encoding iso_8859_1
set ylabel "Erro global (%)"
set xlabel "N\372mero de passos"
#set title "Mont Rainier"
set key horizontal bottom center outside samplen 2 width -8 height 2

set style line 1 lc rgb '#4575b4' lt 1 lw 1 pt 7 ps 0.5   
set style line 2 lc rgb '#abd9e9' lt 1 lw 1 pt 7 ps 0.5 
set style line 3 lc rgb '#fee090' lt 1 lw 1 pt 7 ps 0.5   
set style line 4 lc rgb '#fdae61' lt 1 lw 1 pt 7 ps 0.5 
set style line 5 lc rgb '#d7191c' lt 1 lw 1 pt 7 ps 0.5    

 
set style data linespoints
set xrange [-1:5]
set yrange [-5:70]
set xtics("0" 0,"1" 1, "2" 2, "3" 3, "4" 4)
set ytics ("0" 0, "10" 10, "20" 20, "30" 30, "40" 40, "50" 50, "60" 60, "70" 70)
set grid ytics
set terminal svg
set output 'rainier_qual.svg'
set key horizontal bottom center outside samplen 2 width -1 height 1
plot "tempo.t" u 2 t "Sequencial" with linespoints ls 1,\
'' u 3 t "2 processos" with linespoints ls 2,\
'' u 4 t "4 processos" with linespoints ls 3,\
'' u 5 t "6 processos" with linespoints ls 4,\
'' u 6 t "8 processos" with linespoints ls 5
set output

# comado para salvar em pdf: inkscape rainier_qual.svg --export-pdf=rainier_qual.pdf
