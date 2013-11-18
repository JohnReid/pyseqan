
.. index:: installation
.. _installation:

Installation
============
These installation instructions are specifically for an Ubuntu Linux system but should work with minor 
changes on other Linux systems and with some changes on other operating systems (Windows, MacOS, etc..). 
For other Linux systems you will need to replace any invocations of ``apt-get`` with your system's package
manager. For other operating systems you will need to work out the corresponding method of install.
The instructions install packages using ``apt-get`` where possible, otherwise they are installed in ``$HOME/local/``.

I have written these instructions to install pyseqan into a virtual Python environment using the virtualenv_ tool.
This has the benefits of isolating the pyseqan install from the rest of your system and can be done without
administrator privileges. 

.. _virtualenv: http://www.virtualenv.org/en/latest/index.html


.. index:: prerequisites

Prerequisites
-------------
The following software should be downloaded and unpacked/installed somewhere on your system:

- GCC_: I have tested pyseqan with g++ versions 4.7.3 but other recent versions should work.
  Every release of
  g++ seems to give it a stricter interpretation of the C++ standard so
  perhaps it is not a good idea to try a version too much newer. Other modern 
  compilers such as Microsoft C++ should work but I have not tried them. Please let me know
  if you try other compilers and run into any issues. 
  
  Make sure you have gcc (with g++) installed::
    
    sudo apt-get install gcc build-essential
  
  If gcc is not already installed and you do not have administrator privileges you could install it locally from source.

  
- `Python 2.7`_: Other versions of python 2 will probably work such as 2.5 or 2.6. Install Python 
  with virtualenv_::

    sudo apt-get install python-dev python-virtualenv

  Now create a virtual environment to install pyseqan into and activate it::
  
    mkdir -p $HOME/local
    virtualenv --system-site-packages $HOME/local/pyseqan-virtualenv
    . $HOME/local/pyseqan-virtualenv/bin/activate


- `Boost C++ libraries`_: I have tested pyseqan using version 1.54 of boost, 
  although any recent version should work. You can install the boost libraries with::
  
    sudo apt-get install libboost-dev
 
  If you need a different version or want to install from source, first download them::
  
    mkdir -p $HOME/local/src
    cd $HOME/local/src
    wget \
      http://sourceforge.net/projects/boost/files/boost/1.54.0/boost_1_54_0.tar.bz2/download \
      -O boost_1_54_0.tar.bz2
    tar jxf boost_1_54_0.tar.bz2


  then install boost following the commands_ given at the Boost website and update your 
  ``LD_LIBRARY_PATH`` environment variable so that the shared objects can be found
  at runtime::
  
    cd $HOME/local/src/boost_1_54_0
    ./bootstrap.sh --help
    ./bootstrap.sh --prefix=$HOME/local --with-libraries=python
    ./b2 --ignore-config install release
    export LD_LIBRARY_PATH=$HOME/local/lib:$LD_LIBRARY_PATH
  



- `SeqAn sequence analysis library`_: The SeqAn C++ library provides the suffix array implementation that
  pyseqan uses. I have used version 1.4.1 of SeqAn, although any recent version should work. You can
  install a version using apt-get::

    sudo apt-get install seqan-dev

  or install from source::

    cd $HOME/local/src
    wget http://packages.seqan.de/seqan-library/seqan-library-1.4.1.tar.bz2
    tar jxf seqan-library-1.4.1.tar.bz2


.. _GCC: http://gcc.gnu.org/
.. _Python 2.7: http://www.python.org/
.. _Boost C++ libraries: http://www.boost.org/
.. _commands: http://www.boost.org/doc/libs/1_49_0/more/getting_started/unix-variants.html#easy-build-and-install
.. _SeqAn sequence analysis library: http://www.seqan.de/






.. index:: download

Download pyseqan
----------------

If you have not already done so, download pyseqan and unpack it locally. You will need to replace
``*.*.*`` with the version of pyseqan you want to install (check at PyPI_)::

    cd $HOME/local/src
    wget http://pypi.python.org/packages/source/S/pyseqan/pyseqan-*.*.*.tar.gz
    tar zxf pyseqan-*.*.*.tar.gz
    cd $HOME/local/src/pyseqan-*.*.*

.. _PyPI: http://pypi.python.org/pypi/pyseqan/





.. index:: build environment

Configure, build, install
-------------------------

We are ready to configure pyseqan. pyseqan uses aksetup for installation, which means that
installation should be easy and quick. Try::
  
    python configure.py --help

to examine the possible options. By the way, if a configuration option says ``several ok``,
then you may specify several values, separated by commas. We need to tell pyseqan
where the boost and seqan C++ libraries are::

    python configure.py \
      --seqan-dir=$HOME/local/src/seqan-library-1.4.1/ \
      --boost-inc-dir=$HOME/local/include \
      --boost-lib-dir=$HOME/local/lib

Configuration is obtained from files in this order::

    /etc/aksetup-defaults.py
    $HOME/.aksetup-defaults.py
    $PACKAGEDIR/siteconf.py

Once you've run configure, you can copy options from your ``siteconf.py`` file to
one of these files, and you won't ever have to configure them again manually.
In fact, you may pass the options ``--update-user`` and ``--update-global`` to
configure, and it will automatically update these files for you. This is particularly 
handy if you want to perform an unattended or automatic installation via pip_ or easy_install_.

Now install pyseqan::
    
    python setup.py install

This can take some time to compile. When it has finished, check that pyseqan has been successfully installed::

    python -c "import seqan"

This should run without problems. 

.. _easy_install: http://packages.python.org/distribute/easy_install.html
.. _pip: http://pypi.python.org/pypi/pip

