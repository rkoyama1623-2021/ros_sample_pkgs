# python_pkg_tutorial
## how to make python package in ros
1. `CMakeLists.txt`に`catkin_python_setup()`を追加
1. packageを格納するディレクトリ`src`を追加する。
1. `CMakeLists.txt`と同じディレクトリに`setup.py`を追加する。
1. `setup.py`に、パッケージ名とパッケージのディレクトリを追加する。

```python
## ! DO NOT MANUALLY INVOKE THIS setup.py, USE CATKIN INSTEAD

from distutils.core import setup
from catkin_pkg.python_setup import generate_distutils_setup

# fetch values from package.xml
setup_args = generate_distutils_setup(
    packages=['tutorial_package'],
        package_dir={'': 'src'},
        )

setup(**setup_args)

```
