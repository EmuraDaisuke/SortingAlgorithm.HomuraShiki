


#include <cassert>
#include <vector>
#include <random>
#include <algorithm>
#include <memory.h>

#include "./HomuraShiki.h"
#include "./Lapse.h"



enum eSrc {
    Rand,
    Nop,
    
    Num,
};

using sort_t = float;



#define ORDER   0
#define FAT     0

struct Test
{
    sort_t m;
    #if ORDER//[
    std::size_t o;
    #endif//]
    #if FAT//[
    std::size_t f[FAT];
    #endif
};

bool operator <(const Test& s, const Test& t)
{
    return s.m < t.m;
}

bool operator ==(const Test& s, const Test& t)
{
    return s.m == t.m;
}



void init(eSrc Src, std::vector<Test>& a, std::mt19937& Rand, std::uniform_int_distribution<>& Range, std::size_t& roDirty, std::size_t& rnDirty)
{
    {   // 
        sort_t m = 0;
        std::size_t o = 0;
        for (auto& v : a){
            v.m = m++;
            #if ORDER//[
            v.o = o++;
            #endif//]
            #if FAT//[
            for (auto& f : v.f) f = 0;
            #endif//]
        }
    }
    
    {   // 
        std::size_t oHead = Range(Rand);
        std::size_t oTail = Range(Rand);
        if (oHead > oTail) std::swap(oHead, oTail);
        
        switch (Src){
            case eSrc::Rand:{
                for (auto o = oHead; o < oTail; ++o){
                    a[o].m = Range(Rand);
                }
                break;
            }
            case eSrc::Nop:{
                break;
            }
            default:{
                break;
            }
        }
        roDirty = oHead;
        rnDirty = oTail - oHead;
    }
}



void test(eSrc Src, int nTest, int nLoop)
{
    std::random_device Seed;
    std::mt19937 Rand(Seed());
    std::uniform_int_distribution<> Range(0, nTest-1);
    auto a = std::vector<Test>(nTest);
    
    static const char* apSrc[eSrc::Num]={
        "Rand",
        "Nop",
    };
    printf("\n\n--- %s %d\n", apSrc[Src], nTest);
    
    #if 1//[
    {   // 
        double t0 = 0;
        double t1 = 0;
        double t2 = 0;
        
        for (auto n = nLoop; n; --n){
            std::size_t oDirty = 0;
            std::size_t nDirty = 0;
            init(Src, a, Rand, Range, oDirty, nDirty);
            
            #if 1//[
            {   // 
                auto s = a;
                auto l = Lapse::Now();
                std::sort(s.begin(), s.end());
                t0 += Lapse::Now() - l;
            }
            #endif//]
            
            #if 1//[
            {   // 
                auto s = a;
                auto l = Lapse::Now();
                std::stable_sort(s.begin(), s.end());
                t1 += Lapse::Now() - l;
            }
            #endif//]
            
            #if 1//[
            {   // 
                auto s = a;
                auto l = Lapse::Now();
                HomuraShiki::Sort(s.data(), s.size(), oDirty, nDirty);
                t2 += Lapse::Now() - l;
            }
            #endif//]
        }
        
        printf("\n== std::sort\n");         Lapse::Out(t0 / nLoop);
        printf("\n== std::stable_sort\n");  Lapse::Out(t1 / nLoop);
        printf("\n== HomuraShiki::Sort\n"); Lapse::Out(t2 / nLoop);
    }
    #else//][
    for (auto n = nLoop; n; --n){
        std::size_t oDirty = 0;
        std::size_t nDirty = 0;
        init(Src, a, Rand, Range, oDirty, nDirty);
        
        auto s0 = a;
        auto s1 = a;
        auto s2 = a;
        
        std::sort(s0.begin(), s0.end());
        std::stable_sort(s1.begin(), s1.end());
        HomuraShiki::Sort(s2.data(), s2.size(), oDirty, nDirty);
        
        auto bEqual01 = (s0 == s1);
        auto bEqual12 = (s1 == s2);
        auto bEqual20 = (s2 == s0);
        
        printf("\n");
        printf("%d %d\n", bEqual01, (a == s0));
        printf("%d %d\n", bEqual12, (a == s1));
        printf("%d %d\n", bEqual20, (a == s2));
        assert(bEqual12);
    }
    #endif//]
}



int main(int argc, char* argv[])
{
    test(eSrc::Rand,     10000, 1000);
    test(eSrc::Rand,   1000000, 100);
    test(eSrc::Rand, 100000000, 50);
    
    test(eSrc::Nop,  100000000, 10);
    return 0;
}
