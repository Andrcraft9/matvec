set terminal png enhanced large size 512, 512
set output "mvm.png"
set title "Speedup of parallel MVM"
set key left
set xlabel "p"
set ylabel "S"
y(x) = x ### linear speedup
### Here: S(p)=p/(1+tau*L), where tau=tc/ta and L=Lc/La
tau=30. ; La(p,N)=N*N/p ; Lc(p,N)=N/p*(p-1) ; L(p,N)=Lc(p,N)/La(p,N) ### estimates for MVM
S(p,N) = p/(1.+tau*L(p,N)) ### speedup estimation
plot y(x)      with lines lw 1 lc 0 title "linear speedup" ,\
     S(x,1152) with lines lw 1 lc 1 title "theory N=1152 mpi" ,\
     S(x,9216) with lines lw 2 lc 1 title "theory N=9216 mpi" ,\
     "mvm-1152-mpi.dat" using 1:4 with linespoints lw 2 lc 3 pt 2 title "mpi N=1152" ,\
     "mvm-9216-mpi.dat" using 1:4 with linespoints lw 3 lc 3 pt 6 title "mpi N=9216" ,\
     "mvm-1152-omp.dat" using 2:4 with linespoints lw 2 lc 2 pt 2 title "omp N=1152" ,\
     "mvm-9216-omp.dat" using 2:4 with linespoints lw 3 lc 2 pt 6 title "omp N=9216" ,\
     "mvm-1152-omp-opt.dat" using 2:4 with linespoints lw 2 lc 4 pt 2 title "omp opt N=1152" ,\
     "mvm-9216-omp-opt.dat" using 2:4 with linespoints lw 3 lc 4 pt 6 title "omp opt N=9216"

