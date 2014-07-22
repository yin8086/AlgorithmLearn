#include<iostream>
#include <functional>


std::function<void(void)> func1()
{
    int a=1;
    return [a]() mutable{std::cout<<a++<<std::endl;};
}

int main()
{
    std::function<void(void)> testFunc = func1();
    testFunc();
    testFunc();
    testFunc();
    std::cin.get();
}


/* JS
function func1()
{
    var a = 1;
    return function() { console.log(a++);}
}

newF = func1();
newF();
newF();
*/

/* Python 2, 不支持对单纯变量直接catch为左值，而容器作为引用则可以改编饮用对象
Python  3 中通过nonlocal 关键词 支持catch为左值修改,C++为Mutable
def func1():
    a = [1]
    def funcIn():
        print a[0]
        a[0] += 1
    return funcIn

newF = func1()
newF()
newF()
newF()
*/