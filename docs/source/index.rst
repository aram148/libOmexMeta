##########################
libOmexMeta documentation!
##########################



.. toctree::
   :maxdepth: 1
   :hidden:

   background.rst
   reading_rdf/reading_rdf.rst
   writing_rdf/writing_rdf.rst
   singular_annotations/index.rst
   composite_annotations/index.rst
   querying_rdf/querying_rdf.rst
   diagrams/diagrams_index.rst
   annotating_models/annotating_models_index.rst
   APIReference/api_reference_index.rst
   Developers/developers_index.rst

LibOMEXmeta is a library aimed at providing developer-level support for
reading, writing, editing and managing semantic annotations for biosimulation
models.  The `COMBINE modeling community <http://co.mbine.org/>`_  has developed
consensus around how best to annotate models and how to package these models
into archives (OMEX files) that include the modeling source code, the annotations,
files that describe parameters and settings needed for simulations (in a SEDML file),
and potentially the data used for these modeling efforts. This consensus was initially
described in the  publication `"Harmonizing semantic annotations for computational
models in biology" (Briefings in Bioinformatics, 2018) <https://academic.oup.com/bib/article/20/2/540/5164345)>`_.

The goal of semantic annotations are to make explicit the biology that underlies the
semantics of biosimulation models. By using standard knowledge resources about biology
and biological processes (such as CheBI, Uniprot, and ontologies of anatomy), we can
make the models more understandable, reusable and reproducible. More information can
be found at the `OMEX Metadata Specification web page <http://co.mbine.org/standards/omex-metadata>`_.

LibOmexMeta is a C++ library with a C interface that is used to build a Python front end (pyomexmeta). libOmexMeta uses `RDF <https://www.w3.org/RDF/>`_
as a framework for representing these annotations. At the core of libOmexMeta are the `Redland libraries <http://librdf.org/>`_:

    * `raptor2 <http://librdf.org/raptor/>`_ for parsing RDF syntax into RDF graphs and serializing the output
    * `rasqal <http://librdf.org/rasqal/>`_ for querying RDF graphs
    * `librdf <http://librdf.org/>`_ as a front end to raptor2 and rasqal and for triple stores.

Features
========

`Parsers <http://librdf.org/raptor/api-1.4/raptor-parsers.html>`_
------------------------------------------------------------------

  * rdfxml, ntriples, turtle, guess, rdfa, nquads

`Serializers <http://librdf.org/raptor/api-1.4/raptor-serializers.html>`_
-------------------------------------------------------------------------

   * ntriples, turtle, rdfxml-xmp, rdfxml-abbrev, rdfxml, dot, json-triples, json, nquads, html

`Querying <http://librdf.org/rasqal/docs/api/>`_
------------------------------------------------

  * Languages
    * `SPARQL <https://www.w3.org/TR/sparql11-query/>`_
  * Query result formats:
    * xml, json, table, csv, mkr, tsv, html, turtle, rdfxml,

`Storages modules <http://librdf.org/docs/api/redland-storage-modules.html>`_
--------------------------------------------------------------------------------

  * hashes, memory, file, sqlite, uri, tstore (may be supported on request), postgresql (may be uspported on request), virtuoso (may be supported on request)


Platform
========

  * Windows
  * Linux Ubuntu 18.04 / 20.04, untested on other flavours.
  * MacOS


Installation
============

pyomexmeta - the python front end
-----------------------------------

.. code-block:: bash

    pip install pyomexmeta



Build from Source
==================

The procedure is the same on all platforms.

.. note::

    You may look at the libOmexMeta `azure-pipelines <https://github.com/sys-bio/libOmexMeta/blob/master/azure-pipelines.yml>`_
    which is actively used to build libOmexMeta in the cloud on all platforms.


.. note::

    libOmexMeta was built with gcc-10 on linux. It is highly likely to work with
    other versions but untested.
    Use the following commands to use g++-10:

    .. code-block:: bash

        sudo add-apt-repository ppa:ubuntu-toolchain-r/test
        sudo apt-get update
        sudo apt-get install -y gcc-10 g++-10
        update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10


#. Get vcpkg, which provides some of the dependencies


.. code-block:: bash

    VCPKG_INSTALL_PREFIX=~/full/path/to/where/you/want/vcpkg
    git clone https://github.com/microsoft/vcpkg.git $(VCPKG_INSTALL_PREFIX)
    cd $(VCPKG_INSTALL_PREFIX)
    ./bootstrap-vcpkg.sh
    vcpkg install curl pcre openssl yajl sqlite3 liblzma


#. Clone libOmexMeta

.. code-block:: bash

    LIBOMEXMETA_DIRECTORY=/full/path/to/where/you/want/libomexmeta
    git clone https://github.com/sys-bio/libOmexMeta.git --recurse-submodules $(LIBOMEXMETA_DIRECTORY)

.. note::

    It is easy to forget the `--recurse-submodules` flag. If you do the
    `third_party/zlib` (etc.) folders containing the submodules will be empty.
    You can get them by running `git submodule update --init --recursive`
    from the libOmexMeta root.

#. Build libOmexMeta

.. code-block:: bash


    BUILD_DIRECTORY=$(LIBOMEXMETA_DIRECTORY)/build
    INSTALL_DIRECTORY=/full/path/to/where/you/want/to/install/libomexmeta
    mkdir $(BUILD_DIRECTORY)
    cd $(BUILD_DIRECTORY)

    # cmake configure command
    cmake -DVCPKG_ROOT=$(VCPKG_INSTALL_PREFIX) -DCMAKE_INSTALL_PREFIX="$(INSTALL_DIRECTORY)" -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release ..

    # cmake build command
    cmake --build . --target install --config Release -j 12

.. note::

    Like all cmake options passed at the command line, they should be prepended
    with `-D` and followed by `=value`. i.e. `-DBUILD_OPTION=VALUE`

The following build options may be passed to the cmake configure command:

    * `BUILD_TESTS=ON`: Build the C++, C tests and include the Python tests when BUILD_PYTHON=ON

    * `BUILD_PYTHON=ON`: Installs pyomexmeta under the site-packages directory in the install tree and includes the Python tests in ctest when BUILD_TESTS=ON

    * `BUILD_DOCS=ON`: Build and install the documentation

        - `Python_ROOT_DIR=/path/to/python/distribution/to/use`: Manually specify the Python distribution to use
        - `SPHINX_EXECUTABLE/path/to/sphinx/executable`: Manually specify the sphinx executable to use


#. [Optional] Build the pip wheel

.. code-block:: bash

    pip install breathe sphinx-tabs requests sphinxcontrib-programoutput tellurium python-libcombine pytest graphviz
    # get root of currently active Python
    python_dir=$(dirname $(which python))
    cd $(BUILD_DIRECTORY)
    cmake -DVCPKG_ROOT=$(VCPKG_INSTALL_PREFIX) -DCMAKE_INSTALL_PREFIX="$(INSTALL_DIRECTORY)" -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release -DBUILD_PYTHON=ON -DPython_ROOT_DIR=$(python_dir) ..

    # [optional] run the tests
    cmake --build $(BUILD_DIRECTORY) --target run-ctest --config Release

    # install
    cmake --build . --target install --config Release -j 12

This time there will be a `site-packages/pyomexmeta` directory and a `site-packages/setup.py` file. To
build a pip wheel run:

.. code-block:: bash

    python setup.py bdist_wheel


#. [Optional] Run the tests

From the build tree (`$(BUILD_DIRECTORY)`) you can build the `run-ctest` target. When

.. code-block:: bash

    cmake --build $(BUILD_DIRECTORY) --target run-ctest --config Release

Behind the scenes, this will use te following `ctest` command, which you may also use manually:

.. code-block:: bash

    cd $(BUILD_DIRECTORY)
    # without any arguments
    ctest .
    # or with verbose output
    ctest --verbose --extra-verbose --progress .


#. [Optional] Running Python tests

When `-DBUILD_PYTHON=ON` the Python tests will also be executed by ctest. You may also
use `pytest` or `nosetests` (for example) in the usual way

.. code-block:: bash

    # ensure you have pytest/nose
    pip install pytest nose
    cd $(INSTALL_DIRECTORY)/site-packages/pyomexmeta

    # pytest will discover and run the tests
    pytest .

    # nosetests will discover and run the tests
    nosetests .


#. [For Developers] Running Python tests from the source tree

When you run the cmake configure command, a file in `src/pyomexmeta` will be created called 
`ExtraSearchDirectories.txt`. Inside this file is an (extendable) list of additional search 
directories that Python can use to locate the OmexMetaCAPI. By default these point to the current 
install tree. Therefore, to work on the pyomexmeta code the best course of action is to first build 
and install libomexmeta without the -DBUILD_PYTHON=ON argument (as per above) and then reconfigure 
with the `-DBUILD_PYTHON=ON` option. Now you can develop pyomexmeta from the source tree and you will 
be using binaries in the install tree. 

.. warning::

    Any changes you make in the C/C++ code base will not propagate to Python until you have 
    reinstalled libOmexMeta `cmake --build $(BUILD_DIRECTORY) --target install --config Release -j 12`



Docker
-------

You can get a docker image using

.. code-block:: bash

    $ docker pull ciaranwelsh/libomexmeta:v1.1.0

This is an Ubuntu 18.04 based container that has libOmexMeta prebuilt and installed
under `/libOmexMeta/install-docker`. See `dockerfile <https://github.com/sys-bio/libOmexMeta/blob/master/Dockerfile>`_ for full set of commands to build libOmexMeta on ubuntu.
Conda is preconfigured and pyomexmeta is installed.


Downloading Binaries
====================

You can download binaries from the `releases tab <https://github.com/sys-bio/libOmexMeta/releases>`_


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
