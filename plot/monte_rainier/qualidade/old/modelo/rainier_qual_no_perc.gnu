#set size .75,.5
set boxwidth 0.5 absolute #seta o gnuplot para plotar grafico de barras e diz qual deve ser a largura de cada barra
set style fill solid 1.0 border -1 #diz como deve ser a borda de cada barra
set style data histogram #seta que o grafico a ser plotado sera um grafico de barras
set style histogram rowstacked gap 0 #diz o quanto um grupo de barras estara distante dos grupos adjacentes
set yrange [0:100]#seta a altura do eixo y
set xrange [-1:5]#seta a largura do eixo x
set xtics("1" 0,"2" 1, "4" 2, "6" 3, "8" 4)#coloca uma string numa posição do eixo x, pode ser qualquer numero real do real do eixo x
set ylabel "Número de nós (%)" #titulo para o eixo vertical
set xlabel "Número de processos" #titulo para o eixo horizontal
set key horizontal bottom center outside samplen 2 width -4 height 1
set terminal svg
set output 'rainier_qual_no_perc.svg'

set style line 1 lc rgb '#2b8cbe' lt 1 lw 1 pt 7 ps 0.5 
set style line 2 lc rgb '#ef8a62' lt 1 lw 1 pt 7 ps 0.5

plot 'dados.t' using 2 t "Erro local [0,0 - 0,1]" w boxes fillcolor rgb '#2b8cbe', 'dados.t' using 3 t "Erro local [0,1 - 1,0]" w boxes fillcolor rgb '#ef8a62' 
set output

# comado para salvar em pdf: inkscape rainier_qual_no_perc.svg --export-pdf=rainier_qual_no_perc.pdf
