# pysumo
Python module for running SUMO traffic simulations.

SUMO is great traffic simulator, but using the standard module `traci` is slow, especially when trying to use SUMO for reinforcement learning.

pysumo wraps the SUMO code so it can run as a python extension, significantly improving performance.

pysumo runs on top of libsumo, a custom build target added to the SUMO build
https://github.com/bstriner/sumo

##Getting the code
pysumo install will automatically download and build the SUMO submodule before building pysumo.

```
git clone https://github.com/bstriner/pysumo.git
cd pysumo
sudo python setup.py install
```
