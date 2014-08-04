#ifndef YINEXCEPT
#define YINEXCEPT
#include<exception>

// exception classes
class NoMem: std::exception
{
public:
    virtual const char* what() const
    {
        return "No Memory";
    }

};
class OutOfBounds: std::exception
{
public:
    virtual const char* what() const
    {
        return "OutOfBounds";
    }

};

class SizeNotMatch: std::exception
{
public:
    virtual const char* what() const
    {
        return "SizeNotMatch";
    }

};

class BadInput: std::exception
{
public:
    virtual const char* what() const
    {
        return "BadInput";
    }

};
#endif