# ICNP 2017

This is the code used to generate simulations presented in Figure 8.

Prerequisites:

 * [LEMON](http://lemon.cs.elte.hu/trac/lemon)
 * [click](http://click.pocoo.org/)
 * [CMake](https://cmake.org/)

Graphs generated with _Topobench_ library are contained in `graphs` directory:

 * _Jelly Fish_
 * _Small World Data Center_
 * _Hybercube_

Run instruction:

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
$ cd ../utils
$ ./sim_ng.py [path to graph]
```

Output structure: `[graph].[alg].[var].[params].csv'`, where

 * `[graph]` is one of Hypercube, JellyFish and SWDCRing
 * `[alg]` is an algorithm name
 * `[var]` is an varying parameter: `numc` for _number of chunks_, `maxe` for _maximal edge weight_, and `avgc` for average chunks size.
 * `[params]` other fixed parameters.
