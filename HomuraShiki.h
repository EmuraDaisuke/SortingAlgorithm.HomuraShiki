#pragma once
// Copyright(c) 2019 Emura Daisuke
// MIT License



#include <cstddef>
#include <utility>
#include "../SortingAlgorithm.HayateShiki/HayateShiki.h"
#include "../SortingAlgorithm.SetsunaShiki/SetsunaShiki.h"

#if _MSC_VER//[
// msvc
#elif __clang__//][
// clang
#else//][
// gcc
#endif//]

#define Auto    decltype(auto)



namespace HomuraShiki {
namespace Private {



// 

template <class T>
struct Unit
{
    T*              a;
    std::size_t     n;
};



// 

template <class T> std::size_t Num(T* a, T* e);

template <class T> T* Copy(T* pDst, T* pSrc, std::size_t nSrc);
template <class T> T* Copy(T* pDst, T* pSrc, std::size_t nSrc, T& v);

template <class T> T* Join(T* aDst, Unit<T>* pUnit, Unit<T>* pUnit0, Unit<T>* pUnit1);
template <class T> T* Join(T* aDst, Unit<T>* pUnit, Unit<T>* pUnit0, Unit<T>* pUnit1, bool bCopy);

template <class T> void InitUnit(Unit<T>* pUnit, T* pSrc, std::size_t nSrc);



// 

template <class T>
std::size_t Num(T* a, T* e)
{
    return (e - a);
}



template <class T>
T* Copy(T* pDst, T* pSrc, std::size_t nSrc)
{
    while (nSrc--) *pDst++ = std::move(*pSrc++);
    return pDst;
}



template <class T>
T* Copy(T* pDst, T* pSrc, std::size_t nSrc, T& v)
{
    *pDst = std::move(v);
    return Copy(++pDst, ++pSrc, --nSrc);
}



template <class T>
T* Join(T* aDst, Unit<T>* pUnit, Unit<T>* pUnit0, Unit<T>* pUnit1)
{
    auto pDst = aDst;
    auto p0 = pUnit0->a;
    auto p1 = pUnit1->a;
    auto n0 = pUnit0->n;
    auto n1 = pUnit1->n;
    auto v0 = std::move(*p0);
    auto v1 = std::move(*p1);
    
    while (true){
        if (v1 < v0){
            *pDst++ = std::move(v1);
            if (--n1){
                v1 = std::move(*++p1);
            } else {
                pDst = Copy(pDst, p0, n0, v0);
                break;
            }
        } else {
            *pDst++ = std::move(v0);
            if (--n0){
                v0 = std::move(*++p0);
            } else {
                *p1 = std::move(v1);
                break;
            }
        }
    }
    
    InitUnit(pUnit, p1, n1);
    return pDst;
}



template <class T>
T* Join(T* aDst, Unit<T>* pUnit, Unit<T>* pUnit0, Unit<T>* pUnit1, bool bCopy)
{
    auto pDst = aDst;
    auto n0 = pUnit0->n;
    auto n1 = pUnit1->n;
    
    if (n0 && n1 && pUnit1->a[0] < pUnit0->a[n0-1]){
        pDst = Join(pDst, pUnit1, pUnit0, pUnit1);
        if (bCopy) pDst = Copy(pDst, pUnit1->a, pUnit1->n);
    } else {
        pDst = Copy(pDst, pUnit0->a, n0);
        pDst = Copy(pDst, pUnit1->a, n1);
        pUnit1->n = 0;
    }
    
    InitUnit(pUnit, aDst, Num(aDst, pDst));
    return pDst;
}



template <class T>
void InitUnit(Unit<T>* pUnit, T* pSrc, std::size_t nSrc)
{
    pUnit->a = pSrc;
    pUnit->n = nSrc;
}
}



// 

template <class T> void Sort(T* const aSrc, std::size_t nSrc, std::size_t oDirty, std::size_t nDirty);



// 

template <class T>
void Sort(T* const aSrc, std::size_t nSrc, std::size_t oDirty, std::size_t nDirty)
{
    using namespace Private;
    
    if (aSrc && nSrc > 1 && oDirty < nSrc && nDirty > 0){
        nDirty = ((oDirty + nDirty) <= nSrc)? nDirty: (nSrc - oDirty);
        if (nDirty == 1){
            SetsunaShiki::Sort(aSrc, nSrc, oDirty);
        } else {
            auto aTmp = new T[nSrc];
            HayateShiki::Sort(&aSrc[oDirty], nDirty, aTmp);
            
            if (nDirty < nSrc){
                auto oPrev = 0;
                auto oNext = oDirty + nDirty;
                auto nPrev = oDirty;
                auto nNext = nSrc - oNext;
                auto bPrev = (nPrev && aSrc[nPrev] < aSrc[nPrev-1]);
                auto bNext = (nNext && aSrc[oNext] < aSrc[oNext-1]);
                
                if (bPrev | bNext){
                    Unit<T> vUnit0, vUnit1, vUnit2;
                    InitUnit(&vUnit0, &aSrc[oPrev], nPrev);
                    InitUnit(&vUnit1, &aSrc[oDirty], nDirty);
                    InitUnit(&vUnit2, &aSrc[oNext], nNext);
                    
                    Unit<T> vJoin0, vJoin1;
                    {   // 
                        auto pTmp = aTmp;
                        pTmp = Join(pTmp, &vJoin0, &vUnit0, &vUnit1, false);
                        pTmp = Join(pTmp, &vJoin1, &vUnit1, &vUnit2, true);
                    }
                    {   // 
                        Join(aSrc, &vUnit0, &vJoin0, &vJoin1, true);
                    }
                }
            }
            
            delete[] aTmp;
        }
    }
}
}



#undef Auto
