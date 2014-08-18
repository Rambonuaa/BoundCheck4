This is the file README for the boundcheck distribution,version 1.0

what is boundcheck?
Without the boundary security checking for memory operations of 
C language, the efficiency of the program will be high. At the same time, 
it brings a variety of security issues, such as array bounds violation, 
pointer accesses violation and the illegal operated by C library 
functions and others. Runtime verification technology based on objects 
provides a capacity, which is powerful, can monitor program running 
and find bugs automatically. Based on the original basis of object 
technology, improve the structure variable, multidimensional array 
address range recording; use two query operations to detect a pointer
which points to from an effective memory block to another effective 
one; optimize the record store with balanced binary tree structure. 
Experiments show that this method can effectively detect boundary 
security, and significantly reduce the impact on the performance 
of program running.

how to use?
clang version is 3.4. Get libboundcheck_rt.a in dir 
boundcheck-lib and boundcheck in dir src first,then corvert testing 
files to new files by commond ./boundcheck filename.Finally,compile
the new files and link them by libboundcheck_rt.a 

contact
Please let me know if you find boundcheck useful.
Thanks for your time and attention--

Rambo <hunanlwm@sina.cn>





