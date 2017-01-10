# pysumo
Python module for running SUMO traffic simulations.

SUMO is great traffic simulator, but using the standard module `traci` is slow, especially when trying to use SUMO for reinforcement learning.

pysumo wraps the SUMO code so it can run as a python extension, significantly improving performance.

##Getting the code
Make sure to download submodules to get the latest sumo code

```
git clone https://github.com/bstriner/pysumo.git
cd pysumo
git submodule update --init --recursive
```

##Building SUMO
Build SUMO before building pysumo. Must be built with `enable_pic=yes` so the libraries can be linked into shared libraries.

```
cd sumo/sumo
autoreconf -i
aclocal
autoconfig
automake
./configure enable_pic=yes
make
```

##Building pysumo

```
sudo python setup.py install
```
