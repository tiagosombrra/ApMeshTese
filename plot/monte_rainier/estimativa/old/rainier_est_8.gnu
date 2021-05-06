set size 0.75,.5
set boxwidth 0.5 absolute #seta o gnuplot para plotar grafico de barras e diz qual deve ser a largura de cada barra
set style fill solid 1.0 border -1 #diz como deve ser a borda de cada barra
set style data histogram #seta que o grafico a ser plotado sera um grafico de barras
set style histogram cluster gap 1#diz o quanto um grupo de barras estara distante dos grupos adjacentes
set bars front
set yrange [0:250000]#seta a altura do eixo y
set xrange [-1:8]#seta a largura do eixo x
set xtics("1" 0,"2" 1, "3" 2, "4" 3, "5" 4, "6" 5, "7" 6, "8" 7)#coloca uma string numa posição do eixo x, pode ser qualquer numero real do real do eixo x
set ylabel "Número de elementos" #titulo para o eixo vertical
set xlabel "Processo" #titulo para o eixo horizontal
set terminal svg
set output 'rainier_est_8.svg'
set key horizontal bottom center outside samplen 2 width -8 height 2
set grid ytics
set ytics ("0" 0, "50.000" 50000, "100.000" 100000, "150.000" 150000, "200.000" 200000, "250.000" 250000)
plot 'rainier_est.t' using 2 t "Número de elementos gerados" w boxes lc rgb "#2b8cbe", 'rainier_est.t' using 1 t "Número de elementos estimados" w lines lc rgb "#ef8a62" lw 2
set output
# comado para salvar em pdf: inkscape rainier_est_8.svg --export-pdf=rainier_est_8.pdf
