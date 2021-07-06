	reset
	set xrange [150:4500]
	set yrange [0:0.7]
	g(x) = 0.0000003 *x*log(x)
	plot "merge_sort.out", g(x)