%module stp
%include "stdint.i" 
%include "std_vector.i" 
%include "pyabc.i"     
%include <std_string.i>
%include <std_shared_ptr.i>

namespace std {
template<class Ty> class weak_ptr {
public:
    typedef Ty element_type;

    weak_ptr();
    weak_ptr(const weak_ptr&);
    template<class Other>
        weak_ptr(const weak_ptr<Other>&);
    template<class Other>
        weak_ptr(const shared_ptr<Other>&);

    weak_ptr(const shared_ptr<Ty>&);


    void swap(weak_ptr&);
    void reset();

    long use_count() const;
    bool expired() const;
    shared_ptr<Ty> lock() const;
};
}

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
