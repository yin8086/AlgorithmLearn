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

/* Python 2, ��֧�ֶԵ�������ֱ��catchΪ��ֵ����������Ϊ��������Ըı����ö���
Python  3 ��ͨ��nonlocal �ؼ��� ֧��catchΪ��ֵ�޸�,C++ΪMutable
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