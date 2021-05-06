#set size .75,.5
set boxwidth 0.75 absolute 
set style fill solid 1.0 #diz como deve ser a borda de cada barra
set style data histogram #seta que o grafico a ser plotado sera um grafico de barras
set style histogram rowstacked gap 0 #diz o quanto um grupo de barras estara distante dos grupos adjacentes
set yrange [0:900]#seta a altura do eixo y
set xrange [-1:4]#seta a largura do eixo x
set xtics("2" 0,"4" 1, "6" 2, "8" 3)#coloca uma string numa posição do eixo x, pode ser qualquer numero real do real do eixo x
set ylabel "Tempo de execução (segundos)" #titulo para o eixo vertical
set xlabel "Número de processos" #titulo para o eixo horizontal
set key horizontal bottom center outside samplen 2 width -8 height 2
set terminal svg
set output 'rainier_time_det.svg'
set style line 1 lc rgb "#e0f3f8"
set style line 2 lc rgb "#ffffbf"
set style line 3 lc rgb "#fee090"
set style line 4 lc rgb "#91bfdb"
set style line 5 lc rgb "#4575b4"
set style line 6 lc rgb "#fc8d59"
set style line 7 lc rgb "#d73027"
set grid ytics

plot 'tempo_det.t' using 2 t "Inicialização", \
'tempo_det.t' using 3 t "Estimativa e balanceamento de carga",\
'tempo_det.t' using 4 t "Geração da malha de background",\
'tempo_det.t' using 5 t "Adaptação das curvas", \
'tempo_det.t' using 6 t "Adaptação do domínio", \
'tempo_det.t' using 7 t "Comunicação", \
'tempo_det.t' using 8 t "Overhead" 
 
# comado para salvar em pdf: inkscape rainier_time_det.svg --export-pdf=rainier_time_det.pdf
