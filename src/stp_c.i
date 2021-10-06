%module stp_c
%include "stdint.i" 
%include <std_string.i>
%{
bool TERMINATE;
#include "stp_c.h"
%}
%include "stp_c.h"
bool TERMINATE;