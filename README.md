# jemalloc_segfault_test

To reproduce the segfault, build the executable with build.sh. You must have the following variables in the path (either exported or on the command line); BOOST_INC, BOOST_LIB, and JEMALLOC_LIB which should be, respectively the paths to the boost include directory, the boost libraries directory and the directory containing libjemalloc.a.

The segfault is triggered under valgrind and with the option MALLOC_CONF=tcache:false, but the run.sh script will take care of executing the binary in a manner that triggers the segfault. So far, this is the minimum reproducible example that I have been able to make.
