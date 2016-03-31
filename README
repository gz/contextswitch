Quantifying the cost of context switch
======================================

Note that this is not my own code but rather imported from the archived sources here:
 * http://cs.rochester.edu/u/cli/research/switch.htm
The original authors and the corresponding paper can be found here:
 * http://dl.acm.org/citation.cfm?id=1281702

Measuring the indirect cost of context switch is a challenging problem. In this paper, we show our results of
experimentally quantifying the indirect cost of context switch using a synthetic workload. Specifically, we measure the
impact of program data size and access stride on context switch cost. We also demonstrate the potential impact of OS
background interrupt handling on the measurement accuracy. Such impact can be  alleviated by using a multi-processor
system on which one processor is employed for context switch measurement while the other runs OS background tasks.

0. What are in the directory:

    measureSwitch.c -- two processes communication via a pipe
    measureSingle.c -- single process simulating two processes communications
    util.c -- utility program for context switch cost measurement
    util.h -- utility header file

1. Compile:

    make 	-- 	compile the programs on a Unix platform

2. Usage:

    ./measureSingle [-n ArraySize] [-s StrideSize]
    ./measureSwitch [-n ArraySize] [-s StrideSize]

    2.1 By default, ArraySize and StrideSize are both zeros.
        Both ArraySize and StrideSize need to be given in byte.

    2.2 Remember to give the same parameters to measureSingle and
        measureSwitch.

3. Output:

    measureSingle outputs time1
        time1 = Overhead of traversing through array + pipe overhead

    measureSwitch outputs time2
        time2 = time1 + context switch overhead

    total cost of context switch    = time2 - time1 (microseconds)

    indirect cost of context switch = total cost -  direct cost

4. Note:

    4.1 When both parameters are zeros, the result (time2 - time1) is the
       direct cost of context switch.

    4.2 We separate measureSingle and measureSwitch into two programs in
	order to avoid interference they may cause to each other.
