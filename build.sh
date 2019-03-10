g++ -std=c++14 -pthread -Og -I $BOOST_INC -L $BOOST_LIB -lboost_program_options-mt -lboost_system-mt -ldl -o test main.cpp $JEMALLOC_LIB/libjemalloc.a   
