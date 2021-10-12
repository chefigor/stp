# stp

build.sh for generating SWIG library manually 
swig -python -c++ -extranative -outdir . src/stp_c.i 
g++ -std=c++17 -Isrc -fPIC -c src/stp_c.cpp src/stp_c_wrap.cxx  $(pkg-config --cflags --libs python3)  -pthread
g++ -std=c++17 -shared -fPIC -o _stp_c.so stp_c.o stp_c_wrap.o -pthread 
python -c "import stp_c" #test