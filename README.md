# 焔式(Homura-Shiki)
Homura-Shiki is a new method of differential sort algorithm that applies merge sort.  
For a sorted array, focus on the place where the value change is made and sort fast.  

It has the following features.(Let D be the changed range)  
* Comparison sort
* Stable sort
* External area: N
* Average time: O ((D log D) + N)
* Worst time: O ((D log D) + N)
* Best time: O (D)
* Recursion: None

<br>

# Basic algorithm
* Change the value at any range in the sorted array.
* Perform comparison stable sorting on the change range.
* Merge three parcels in "Forward Range, Change Range, Rearward Range".

## Examples
~~~
Change the value at any range in the sorted array.
0 2 3 4 5 6 7 9|Sorted array
    ↓ ↓ ↓
0 2|8 1 3|6 7 9|
~~~
~~~
Perform comparison stable sorting on the change range.
0 2|8 1 3|6 7 9|
    ↓ ↓ ↓
0 2|1 3 8|6 7 9|
~~~
~~~
Merge three parcels in "Forward Range, Change Range, Rearward Range".
0 2|1 3 8|6 7 9|

02   Forward Range
138  Change Range
679  Rearward Range

012    Merge with change range until there is no forward range.(Forward part)
36789  Merge the remainder of the change range with the rearward range.(Rearward part)

01236789  Join forward part and rearward part.(sort complete)
~~~

<br>

# Build & Test
The following environment has been verified.  
* Windows 10 Pro 64bit
* Core i7-8700 3.20GHz

Prepare [Hayate-Shikli](https://github.com/EmuraDaisuke/SortingAlgorithm.HayateShiki) and [Setsuna-Shiki](https://github.com/EmuraDaisuke/SortingAlgorithm.SetsunaShiki) in the sibling directory and build.  

## **Msvc**
Microsoft(R) C/C++ Optimizing Compiler Version 19.16.27027.1 for x64  
~~~
cl Main.cpp -Ox -EHsc -Fe:TestMsvc.exe
TestMsvc.exe
~~~

## **clang++**
clang version 8.0.0 (tags/RELEASE_800/final)  
Target: x86_64-w64-windows-gnu  
~~~
clang++ Main.cpp -O3 -o TestClang++.exe
TestClang++.exe
~~~

## **g++**
gcc version 8.3.0 (Rev2, Built by MSYS2 project)  
Target: x86_64-w64-mingw32  
~~~
g++ Main.cpp -O3 -o TestG++.exe
TestG++.exe
~~~

<br>

# Average benchmark
The following is the case of changing the random range of the sorted array to a random value.  
The unit is seconds, the lower the number, the faster.  

## **Msvc**
|Array|std::sort|std::stable_sort|Homura-Shiki|
|---:|---:|---:|---:|
|10,000|0.00031643|0.00016890|**0.00014270**|
|1,000,000|0.05004999|0.02833382|**0.02414271**|
|100,000,000|3.43509980|3.48550165|**2.55574413**|

## **clang++**
|Array|std::sort|std::stable_sort|Homura-Shiki|
|---:|---:|---:|---:|
|10,000|0.00027717|0.00020017|**0.00015642**|
|1,000,000|0.03866672|0.02881882|**0.02255046**|
|100,000,000|3.38201222|3.64178056|**2.60998679**|

## **g++**
|Array|std::sort|std::stable_sort|Homura-Shiki|
|---:|---:|---:|---:|
|10,000|0.00029155|0.00020019|**0.00014887**|
|1,000,000|0.04052708|0.02760306|**0.02054782**|
|100,000,000|3.58270940|3.45231729|**2.36870185**|

<br>

# Limited condition benchmark
The following is sorted by the array [100,000,000].  
The unit is seconds, the lower the number, the faster.  

## Best case
When the range is specified randomly without changing the value for the sorted array.  

||std::sort|std::stable_sort|Homura-Shiki|
|---:|---:|---:|---:|
|Msvc|0.23993706|1.30073792|**0.01059685**|
|clang++|1.04052412|1.52173567|**0.01611595**|
|g++|1.34400042|1.31273356|**0.01510180**|

<br>

# Finally
How was it?  

We came up with the idea of ​​differential sorting, in terms of the fact that sorting does not always have to be done in its entirety.  
Although the comparison between std::sort and std::stable_sort, which is an overall sort, and Homura-Shiki, which is an diff sort, is not fair, it should be used to determine the usefulness of diff sort.  

I think that it could be shown that sorting can be done faster by devising it.  
As a guideline for operation, if (D <(N / 2)), differential sorting, otherwise it is a feeling that the whole sort is good.  

The sort algorithm is still romantic.  

---
It may be more enjoyable if you read the following together.  
* [Hayate-Shiki](https://github.com/EmuraDaisuke/SortingAlgorithm.HayateShiki)
* [Setsuna-Shiki](https://github.com/EmuraDaisuke/SortingAlgorithm.SetsunaShiki)
