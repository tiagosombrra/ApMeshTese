#resetset size .75,.5
set boxwidth 0.75 absolute
set style fill solid 1.0
set style data histogram
set style histogram rowstacked gap 0 
set yrange [0:100]
set xrange [-1:4]
set xtics("2" 0,"4" 1, "6" 2, "8" 3)
set ylabel "Tempo de execução (%)" 
set xlabel "Número de processos"
set key horizontal bottom center outside samplen 2 width -8 height 2
set terminal svg
set output 'rainier_time_det_perc.svg'
set grid ytics

plot 'tempo_det_perc.t' using 2 t "Inicialização", \
'tempo_det_perc.t' using 3 t "Estimativa e balanceamento de carga",\
'tempo_det_perc.t' using 4 t "Geração da malha de background",\
'tempo_det_perc.t' using 5 t "Adaptação das curvas", \
'tempo_det_perc.t' using 6 t "Adaptação do domínio", \
'tempo_det_perc.t' using 7 t "Comunicação", \
'tempo_det_perc.t' using 8 t "Overhead" 
set output
# comado para salvar em pdf: inkscape rainier_time_det_perc.svg --export-pdf=rainier_time_det_perc.pdf

