	reset
	set xrange [150:4500]
	set yrange [0:0.7]
	g(x) = 0.00000003 *x*x
	plot "insertionsort.out", g(x)
