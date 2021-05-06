set boxwidth 0.75 absolute #seta o gnuplot para plotar grafico de barras e diz qual deve ser a largura de cada barra
set style fill solid 1.0 border -1 #diz como deve ser a borda de cada barra
set style data histogram #seta que o grafico a ser plotado sera um grafico de barras
set style histogram cluster gap 1#diz o quanto um grupo de barras estara distante dos grupos adjacentes
set bars front
set yrange [0:900000]#seta a altura do eixo y
set xrange [-1:4]#seta a largura do eixo x
#set xtics("2" 0,"2" 1, "4" 2, "4" 3, "4" 4, "4" 5, "6" 6,"6" 7, "6" 8,"6" 9, "6" 10,"6" 11, "8" 12,"8" 12, "8" 13,"8" 14, "8" 15,"8" 16, "8" 17, "8" 18)#coloca uma string numa posição do eixo x, pode ser qualquer numero real do real do eixo x
set ylabel "Número de elementos" #titulo para o eixo vertical
set xlabel "Processo" #titulo para o eixo horizontal
set terminal svg
set output 'rainier_est_total.svg'
set key horizontal bottom center outside samplen 2 width -8 height 2
set grid ytics
set ytics ("0" 0, "100.000" 100000, "200.000" 200000, "300.000" 300000, "400.000" 400000, "500.000" 500000, "600.000" 600000, "700.000" 700000, "800.000" 800000, "900.000" 900000)
plot 'rainier_est_total.t' using 1 ti "Número de elementos gerados com 2 processos" , '' u 2 ti col

#plot 'rainier_est_total.t' using 1 t "Número de elementos gerados com 2 processos" w boxes lc rgb "#2b83ba", 'rainier_est_total.t' using 6 t "Número de elementos gerados com 4 processos" w boxes lc rgb "#abdda4", 'rainier_est_total.t' using 4 t "Número de elementos gerados com 6 processos" w boxes lc rgb "#ffffbf", 'rainier_est_total.t' using 2 t "Número de elementos gerados com 8 processos" w boxes lc rgb "#fdae61", 'rainier_est_total.t' using 1 t "Número de elementos estimados" w lines lc rgb "#d7191c" lw 2
set output
# comado para salvar em pdf: inkscape rainier_est_total.svg --export-pdf=rainier_est_total.pdf
