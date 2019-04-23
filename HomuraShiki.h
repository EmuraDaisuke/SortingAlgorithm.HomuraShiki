#pragma once
// Copyright(c) 2019 Emura Daisuke
// MIT License



#include <cstddef>
#include <utility>
#include "../SortingAlgorithm.HayateShiki/HayateShiki.h"
#include "../SortingAlgorithm.SetsunaShiki/SetsunaShiki.h"

#define Auto    decltype(auto)



namespace HomuraShiki {



// 

template <class RandomAccessIterator>
inline void sort(RandomAccessIterator const first, RandomAccessIterator const last, RandomAccessIterator const dirty_first, RandomAccessIterator const dirty_last);

template <class RandomAccessIterator, class Compare>
inline void sort(RandomAccessIterator const first, RandomAccessIterator const last, RandomAccessIterator const dirty_first, RandomAccessIterator const dirty_last, Compare comp);



// 

template <class RandomAccessIterator, class Compare> class Private
{
    private:
        template <class ForwardIterator, class ValueType> class Array
        {
            public:
                using val_t = ValueType;
                using ref_t = val_t&;
                using ptr_t = val_t*;
                using itr_t = ptr_t;
                using dif_t = ptrdiff_t;
            
            
            
            private:
                bool mbTemporary;
                dif_t mSize;
                ptr_t mData;
            
            
            
            public:
                ~Array() noexcept
                {
                    if (mbTemporary) ::operator delete(mData, std::nothrow);
                }
                
                
                
                Array(ForwardIterator first, ForwardIterator last)
                :mbTemporary(false)
                ,mSize(std::distance(first, last))
                ,mData(&*first)
                {}
                
                
                
                Array(dif_t Size)
                :mbTemporary(true)
                ,mSize(Size)
                ,mData(static_cast<ptr_t>(::operator new(sizeof(val_t) * Size)))
                {}
                
                
                
                itr_t begin() const noexcept
                {
                    return mData;
                }
                
                
                
                itr_t end() const noexcept
                {
                    return mData + mSize;
                }
            
            
            
            private:
                Array(const Array&) = delete;
                Array& operator =(const Array&) = delete;
        };
    
    
    
    public:
        class Technique {
            private:
                using rai_t = RandomAccessIterator;
                using rai_v = typename std::iterator_traits<rai_t>::value_type;
                
                using itr_t = typename Array<rai_t, rai_v>::itr_t;
                using val_t = typename Array<rai_t, rai_v>::val_t;
                using ref_t = typename Array<rai_t, rai_v>::ref_t;
                using dif_t = typename Array<rai_t, rai_v>::dif_t;
                
                using cmp_t = Compare;
            
            
            
            private:
                const cmp_t mComp;
            
            
            
            private:
                struct Unit
                {
                    itr_t a;
                    dif_t n;
                };
            
            
            
            private:
                void InitUnit(Unit& rUnit, itr_t iSrc, dif_t nSrc)
                {
                    rUnit.a = iSrc;
                    rUnit.n = nSrc;
                }
                
                
                
                itr_t Copy(itr_t iDst, itr_t iSrc, dif_t nSrc)
                {
                    while (nSrc--) *iDst++ = std::move(*iSrc++);
                    return iDst;
                }
                
                
                
                itr_t Copy(itr_t iDst, itr_t iSrc, dif_t nSrc, ref_t v)
                {
                    *iDst = std::move(v);
                    return Copy(++iDst, ++iSrc, --nSrc);
                }
                
                
                
                itr_t Join(itr_t aJoin, Unit& rUnit, const Unit& rUnit0, const Unit& rUnit1)
                {
                    auto iJoin = aJoin;
                    auto i0 = rUnit0.a;
                    auto i1 = rUnit1.a;
                    auto n0 = rUnit0.n;
                    auto n1 = rUnit1.n;
                    auto v0 = std::move(*i0);
                    auto v1 = std::move(*i1);
                    
                    auto Comp = mComp;
                    while (true){
                        if (Comp(v1, v0)){
                            *iJoin++ = std::move(v1);
                            if (--n1){
                                v1 = std::move(*++i1);
                            } else {
                                iJoin = Copy(iJoin, i0, n0, v0);
                                break;
                            }
                        } else {
                            *iJoin++ = std::move(v0);
                            if (--n0){
                                v0 = std::move(*++i0);
                            } else {
                                *i1 = std::move(v1);
                                break;
                            }
                        }
                    }
                    
                    InitUnit(rUnit, i1, n1);
                    return iJoin;
                }
                
                
                
                itr_t Join(itr_t aJoin, Unit& rUnit, const Unit& rUnit0, Unit& rUnit1, bool bCopy)
                {
                    auto iJoin = aJoin;
                    auto n0 = rUnit0.n;
                    auto n1 = rUnit1.n;
                    
                    if (n0 && n1 && rUnit1.a[0] < rUnit0.a[n0-1]){
                        iJoin = Join(iJoin, rUnit1, rUnit0, rUnit1);
                        if (bCopy) iJoin = Copy(iJoin, rUnit1.a, rUnit1.n);
                    } else {
                        iJoin = Copy(iJoin, rUnit0.a, n0);
                        iJoin = Copy(iJoin, rUnit1.a, n1);
                        rUnit1.n = 0;
                    }
                    
                    InitUnit(rUnit, aJoin, std::distance(aJoin, iJoin));
                    return iJoin;
                }
            
            
            
            public:
                ~Technique() noexcept
                {}
                
                
                
                Technique(cmp_t comp)
                :mComp(comp)
                {}
                
                
                
                void Sort(rai_t const first, rai_t const last, rai_t const dirty_first, rai_t const dirty_last)
                {
                    Auto nOriginal = std::distance(first, last);
                    if (nOriginal > 1){
                        Auto nDirty = std::distance(dirty_first, dirty_last);
                        if (nDirty > 0){
                            if (nDirty == 1){
                                SetsunaShiki::sort(first, last, dirty_first, mComp);
                            } else {
                                HayateShiki::sort(dirty_first, dirty_last, mComp);
                                
                                Auto oDirty = std::distance(first, dirty_first);
                                if ((oDirty + nDirty) <= nOriginal){
                                    auto oLower = 0;
                                    auto oUpper = oDirty + nDirty;
                                    auto nLower = oDirty;
                                    auto nUpper = nOriginal - oUpper;
                                    auto bLower = (nLower && mComp(first[nLower], first[nLower-1]));
                                    auto bUpper = (oUpper && mComp(first[oUpper], first[oUpper-1]));
                                    
                                    if (bLower | bUpper){
                                        Array<rai_t, rai_v> aOriginal(first, last);
                                        Array<rai_t, rai_v> aExternal(nOriginal);
                                        Auto aSrc = aOriginal.begin();
                                        Auto aTmp = aExternal.begin();
                                        
                                        Unit vUnit0, vUnit1, vUnit2;
                                        InitUnit(vUnit0, (aSrc + oLower), nLower);
                                        InitUnit(vUnit1, (aSrc + oDirty), nDirty);
                                        InitUnit(vUnit2, (aSrc + oUpper), nUpper);
                                        
                                        Unit vJoin0, vJoin1;
                                        {   // 
                                            auto iJoin = aTmp;
                                            iJoin = Join(iJoin, vJoin0, vUnit0, vUnit1, false);
                                            iJoin = Join(iJoin, vJoin1, vUnit1, vUnit2, true);
                                        }
                                        {   // 
                                            Join(aSrc, vUnit0, vJoin0, vJoin1, true);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
        };
    
    
    
    public:
        static void Sort(RandomAccessIterator const first, RandomAccessIterator const last, RandomAccessIterator const dirty_first, RandomAccessIterator const dirty_last, Compare comp)
        {
            Technique(comp).Sort(first, last, dirty_first, dirty_last);
        }
};



template <class RandomAccessIterator>
inline void sort(RandomAccessIterator const first, RandomAccessIterator const last, RandomAccessIterator const dirty_first, RandomAccessIterator const dirty_last)
{
    HomuraShiki::sort(first, last, dirty_first, dirty_last, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>());
}



template <class RandomAccessIterator, class Compare>
inline void sort(RandomAccessIterator const first, RandomAccessIterator const last, RandomAccessIterator const dirty_first, RandomAccessIterator const dirty_last, Compare comp)
{
    HomuraShiki::Private<RandomAccessIterator, Compare>::Sort(first, last, dirty_first, dirty_last, comp);
}
}



#undef Auto
