%module stp
%include "stdint.i" 
%include "std_vector.i" //podrska za vector
%include "pyabc.i"      //podrska za vector
%include <std_string.i>
%{
#include "stp.hpp"
%}
%include "stp.hpp"
%template (LinkVector) std::vector<Link>;