GeneHunter
==========

[![Build Status](https://travis-ci.org/BrainTwister/GeneHunter.png)](https://travis-ci.org/BrainTwister/GeneHunter)
[![Coverage Status](https://coveralls.io/repos/github/BrainTwister/GeneHunter/badge.svg?branch=master)](https://coveralls.io/github/BrainTwister/GeneHunter?branch=master)
[![Code Quality](https://landscape.io/github/BrainTwister/GeneHunter/master/landscape.svg)](https://landscape.io/github/BrainTwister/GeneHunter/master)
<a href="https://scan.coverity.com/projects/braintwister-genehunter">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/6386/badge.svg"/>
</a>
[![MIT](http://img.shields.io/badge/license-MIT-green.svg?style=flat)](http://opensource.org/licenses/MIT)

Identification of organisms from a stream of DNA sequences.

Copyright &copy; 2013-2016 BrainTwister - All rights reserved.

GeneHunter is free software made available under the [MIT License](http://opensource.org/licenses/MIT). For details see [LICENSE](LICENSE.md).

Requirements
------------

- cmake (Version >= 3.0, http://www.cmake.org)
- Boost (Version >= 1.52, http://www.boost.org, modules: filesystem, iostreams, serialization, and system)
- MySQL
- gtest (optional)
- doxygen (optional)

For Ubuntu 14.04 please install following packages:

    sudo apt-get install cmake libboost-all-dev libgtest-dev doxygen mysql-client libmysqlclient-dev

Installation
------------

1) Get the source code using git:

    git clone https://github.com/BrainTwister/GeneHunter
  
2) Make build directory and change into it:
  
    mkdir GeneHunter/build
    cd GeneHunter/build

3) Generate Makefile using cmake:

    cmake ..

4) Compile:

    make -j <number of cores>

5) Execute the Tests

    make test

6) Download Nucleotide database

    wget ftp://ftp.ncbi.nih.gov/blast/db/FASTA/nt.gz

7) Download and generate gene database

    wget ftp://ftp.ncbi.nlm.nih.gov/genbank/*.seq.gz
 
    CDSDatabaseBuilder [input-files]

8) Download and generate taxonomy database

    wget ftp://ftp.ncbi.nih.gov/pub/taxonomy/taxdump.tar.gz
    wget ftp://ftp.ncbi.nih.gov/pub/taxonomy/gi_taxid_nucl.dmp.gz

    TaxonomyDatabaseBuilder [gi_taxid_nucl] [names] [nodes]

9) Edit and set environmental variables

    source ../settings/bashrc

Usage
-----

    GeneHunter [read-file] [description-file] [result-file] [nt-file]
