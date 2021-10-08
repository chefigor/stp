%module stp_c
%include "stdint.i" 
%include "std_vector.i" //podrska za vector
%include "pyabc.i"      //podrska za vector
%include <std_string.i>
%{
#include "stp_c.h"
%}
%include "stp_c.h"
%template (LinkVector) std::vector<Link>;