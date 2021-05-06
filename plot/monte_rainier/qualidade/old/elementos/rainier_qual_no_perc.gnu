#set size 0.75,.75
set encoding iso_8859_1
set ylabel "N\372mero de elementos (%)"
set xlabel "Qualidade dos elementos"
#set title "Mont Rainier"
set key horizontal bottom center outside samplen 2 width -8 height 2

set style line 1 lc rgb '#4575b4' lt 1 lw 1 pt 7 ps 0.5   
set style line 2 lc rgb '#abd9e9' lt 1 lw 1 pt 7 ps 0.5 
set style line 3 lc rgb '#fee090' lt 1 lw 1 pt 7 ps 0.5   
set style line 4 lc rgb '#fdae61' lt 1 lw 1 pt 7 ps 0.5 
set style line 5 lc rgb '#d7191c' lt 1 lw 1 pt 7 ps 0.5    

 
set style data histogram
set style histogram cluster gap 1

set style fill solid border rgb "black"
set auto x
set yrange [0:45]
set xtics("0,1" 0,"0,2" 1, "0,3" 2, "0,4" 3, "0,5" 4, "0,6" 5, "0,7" 6, "0,8" 7, "0,9" 8, "1,0" 9)
set ytics (0, 5, 10, 15, 20, 25, 30, 35, 40, 45)
set grid ytics
set terminal svg
set output 'rainier_qual_perc.svg'
set key horizontal bottom center outside samplen 2 width -1 height 1
plot "dados_perc.t" u 2:xtic(1) t "1 processo" fc rgb '#d7191c',\
'' u 3:xtic(1) t "2 processos" fc rgb '#fdae61',\
'' u 4:xtic(1) t "4 processos" fc rgb '#ffffbf',\
'' u 5:xtic(1) t "6 processos" fc rgb '#abd9e9',\
'' u 6:xtic(1) t "8 processos" fc rgb '#2c7bb6'
set output

# comado para salvar em pdf: inkscape rainier_qual_perc.svg --export-pdf=rainier_qual_perc.pdf
