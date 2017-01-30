from distutils.core import setup, Extension
from distutils.cmd import Command
from distutils.command.install import install
from distutils.command.build import build
from distutils.core import Distribution
import subprocess
import os


class PysumoDistribution(Distribution):
    global_options = Distribution.global_options + [
        ('skip-libsumo', None, 'Skip building and installing libsumo library; use if libsumo already installed.')
    ]

    def __init__(self, *args, **kwargs):
        self.skip_libsumo = False
        Distribution.__init__(self, *args, **kwargs)


class BuildLibsumoCommand(Command):
    """Build libsumo module."""

    description = 'Build libsumo shared library'
    user_options = []

    def initialize_options(self):
        # """Set default values for options."""
        # Each user option must be listed here with their default value.
        pass

    def finalize_options(self):
        #  """Post-process options."""
        if not self.distribution.skip_libsumo:
            libsumodir = 'sumo/sumo/src/libsumo'
            if not os.path.exists(libsumodir):
                assert (subprocess.Popen(['git', 'submodule', 'init']).wait() == 0)
                assert (subprocess.Popen(['git', 'submodule', 'update']).wait() == 0)
            assert os.path.exists(libsumodir), \
                '{} does not exist. Did you git submodule init and git submodule update?'.format(libsumodir)

    def run(self):
        """Run command."""
        if self.distribution.skip_libsumo:
            self.announce("Skipping libsumo shared library build")
        else:
            self.announce("Building libsumo shared library")
            srcdir = './sumo/sumo'
            assert (subprocess.Popen(['aclocal'], cwd=srcdir).wait() == 0)
            assert (subprocess.Popen(['libtoolize'], cwd=srcdir).wait() == 0)
            assert (subprocess.Popen(['autoconf'], cwd=srcdir).wait() == 0)
            assert (subprocess.Popen(['autoreconf', '-i'], cwd=srcdir).wait() == 0)
            assert (subprocess.Popen(['automake', '--add-missing'], cwd=srcdir).wait() == 0)
            assert (subprocess.Popen(['./configure', '--enable-libsumo=yes'], cwd=srcdir).wait() == 0)
            assert (subprocess.Popen(['make'], cwd=srcdir).wait() == 0)
            libsumodir = './sumo/sumo/src/libsumo'
            assert (subprocess.Popen(['make', 'install'], cwd=libsumodir).wait() == 0)
            assert (subprocess.Popen(['ldconfig']).wait() == 0)
            self.announce('Successfully built libsumo')


class PysumoBuildCommand(build):
    def run(self):
        self.run_command("build_libsumo")
        build.run(self)


cmdclass = {
    "build": PysumoBuildCommand,
    "build_libsumo": BuildLibsumoCommand
}

ext_args = {
    'include_dirs': ['src', 'sumo/sumo/src', '/usr/include/fox-1.6'],
    'libraries': ['sumo']
}

src = [
    'src/inductionloop.cpp',
    'src/meme.cpp',
    'src/simulation.cpp',
    'src/tls.cpp',
    'src/vehicle.cpp',
    'src/pysumo.cpp',
    'src/lane.cpp'
]

ext_modules = [
    Extension('pysumo', src, **ext_args)
]

setup(name='pysumo',
      version='0.0.1',
      description='Pysumo',
      author='Ben Striner',
      author_email='bstriner@gmail.com',
      url='https://github.com/bstriner/pysumo',
      cmdclass=cmdclass,
      distclass=PysumoDistribution,
      ext_modules=ext_modules)
