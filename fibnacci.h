#ifndef FIBNACCI_H
#define FIBNACCI_H

using Rank = int;

class Fib{
    private:
        Rank f, g;
    public:
    Fib( Rank n) //获取不小于 n 的最小的一项的指标
    {f = 1; g = 0; while(g < n) next();}
    Rank get() {return g;} //获取当前指标
    Rank next() {g += f; f = g - f; return g;}
    Rank prev() {f = g - f; g = g - f; return g;}
};



#endif
