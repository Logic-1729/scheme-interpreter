#ifndef RUNTIMEERROR
#define RUNTIMEERROR

#include <exception>
#include <string>

class RuntimeError : std::exception 
{
private:
    std :: string s;
public:
    RuntimeError(std :: string);
    std :: string message() const;
};

#endif