%module stp
%include "stdint.i" 
%include "std_vector.i" 
%include "pyabc.i"     
%include <std_string.i>
%include <std_shared_ptr.i>

%shared_ptr(Switch)
%{
#include "stp.hpp"
#include <memory>
%}
%inline %{
std::shared_ptr<Switch> makeSwitch(const std::string& str){return std::make_shared<Switch>(str);}
%}
%include "stp.hpp"
%template(SwitchVector)std::vector<std::weak_ptr<Switch>>;
%template(SwitchWeakPtr) std::weak_ptr<Switch>;

//%template (LinkVector) std::vector<Link>;
