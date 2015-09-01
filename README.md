**Note**: If you're interested in Sailfish, you might also want to take a look
at our new software, [Salmon](https://COMBINE-lab.github.io/salmon). 

The documentation for Salmon and Sailfish is being migrated to [ReadTheDocs](http://readthedocs.org).
To see [the latest documentation there](http://sailfish.readthedocs.org).

Setting up Sailfish
===================

[![Join the chat at https://gitter.im/kingsfordgroup/sailfish](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/kingsfordgroup/sailfish?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Requirements:
-------------

* A C++-11 compliant version of GCC.  Any version of [g++](gcc.gnu.org) >= 4.7 
  should work.

* [CMake](www.cmake.org).  Sailfish uses the CMake build system to check,
  fetch and install dependencies, and to compile and install Sailfish.  CMake
  is available for all major platforms (though Sailfish is currently
  unsupported on Windows.)

Installation:
-------------

After downloading the Sailfish source distribution and unpacking it, change 
into the top-level directory:

~~~~
> cd Sailfish-0.6.3-Source
~~~~

Then, create an out-of-source build directory and change into it:

~~~~
> mkdir build
> cd build
~~~~

Sailfish makes extensive use of [Boost](www.boost.org).  We recommend
installing the most recent version (1.55) systemwide if possible. If Boost is 
not installed on your system, the build process will fetch, compile and install 
it locally.  However, if you already have a recent version of Boost available on
your system, it make sense to tell the build system to use that.

If you have Boost installed you can tell CMake where to look for it. Likewise, 
if you already have 
Intel's [Threading Building Blocks](http://threadingbuildingblocks.org/)
library installed, you can tell CMake where it is as well. The flags for CMake 
are as follows:

* -DFETCH_BOOST=TRUE --  If you don't have Boost installed (or have an
   older version of it), you can provide the FETCH_BOOST flag instead of the
   BOOST_ROOT variable, which will cause CMake to fetch and build Boost locally.

* -DBOOST_ROOT=<boostdir> -- Tells CMake where an existing installtion of Boost 
   resides, and looks for the appropritate version in <boostdir>.  This is the 
   top-level directory where Boost is installed (e.g. /opt/local).

* -DTBB_INSTALL_DIR=<tbbroot> -- Tells CMake where an existing installation of 
   Intel's TBB is installed (<tbbroot>), and looks for the apropriate headers 
   and libraries there. This is the top-level directory where TBB is installed 
   (e.g. /opt/local).

* -DCMAKE_INSTALL_PREFIX=<install_dir> -- <install_dir> is the directory to 
   which you wish Sailfish to be installed.  If you don't specify this option, 
   it will be installed locally in the top-level directory (i.e. the directory 
   directly above "build").

Setting the appropriate flags, you can then run the CMake configure step as 
follows:

~~~~
> cmake [FLAGS] ..
~~~~

The above command is the cmake configuration step, which *should* complain if
anything goes wrong.  Next, you have to run the build step. Depending on what
libraries need to be fetched and installed, this could take a while
(specifically if the installation needs to install Boost).  To start the
build, just run make.

~~~~
> make
~~~~

If the build is successful, the appropriate executables and libraries should be
created. There are two points to note about the build process.  First, if the
build system is downloading and compiling boost, you may see a large number of
warnings during compilation; these are normal.  Second, note that CMake has
colored output by default, and the steps which create or link libraries are
printed in red.  This is the color chosen by CMake for linking messages, and
does not denote an error in the build process. 

Finally, after everything is built, the libraries and executable can be
installed with:

~~~~
> make install
~~~~

To ensure that Sailfish has access to the appropriate libraries you should
ensure that the PATH variabile contains \<install_dir\>/bin, and that
LD_LIBRARY_PATH (or DYLD_FALLBACK_LIBRARY_PATH on OSX) contains
\<install_dir\>/lib.

After the paths are set, you can test the installation by running

~~~~
> make test
~~~~

This should run a simple test and tell you if it succeeded or not.

Running Sailfish
================

Sailfish is a tool for transcript quantification from RNA-seq data.  It
requires a set of target transcripts (either from a reference or _de-novo_
assembly) to quantify.  All you need to run Sailfish is a fasta file containing
your reference transcripts and a (set of) fasta/fastq file(s) containing your
reads.  Sailfish runs in two phases; indexing and quantification.  The indexing
step is independent of the reads, and only need to be run one for a particular
set of reference transcripts and choice of k (the k-mer size). The
quantification step, obviously, is specific to the set of RNA-seq reads and is
thus run more frequently. For a more complete description of all available
options in Sailfish, see the manual.


Indexing
--------

To generate the Sailfish index for your reference set of transcripts, you
should run the following command:

~~~~
> sailfish index -t <ref_transcripts> -o <out_dir> -k <kmer_len>
~~~~

This will build a Sailfish index for k-mers of length \<kmer_len\> for the
reference transcripts  provided in the file \<ref_transcripts\> and place the
index under the directory \<out_dir\>.  There  are additional options that can
be passed to the Sailfish indexer (e.g. the number of threads to use).  These
can be seen by executing the command "Sailfish index -h".

Quantification
--------------

Now that you have generated the Sailfish index (say that it's the directory
\<index_dir\> --- this corresponds to the \<out_dir\> argument provided in the
previous step), you can quantify the transcript expression for a given set of
reads.  To perform the quantification, you run a command like the following:

~~~~
> sailfish quant -i <index_dir> -l "<libtype>" {-r <unmated> | -1 <mates1> -2 <mates2>} -o <quant_dir>
~~~~

Where \<index_dir\> is, as described above, the location of the sailfish index,
\<libtype\> is a string describing the format of the read library (see the
[library string](#library-string) section below) \<unmated\> is a list of files
containing unmated reads, \<mates{1,2}\> are lists of files containing,
respectively, the first and second mates of paired-end reads.  Finally,
\<quant_dir\> is the directory where the output should be written. Just like
the indexing step, additional options are available, and can be viewed by
running "sailfish quant -h".

When the quantification step is finished, the directory \<quant_dir\> will
contain a file named "quant.sf" (and, if bias correction is enabled, an
additional file names "quant_bias_corrected.sf").  This file contains the
result of the Sailfish quantification step.  This file contains a number of
columns (which are listed in the last of the header lines beginning with '#').
Specifically, the columns are (1) Transcript ID, (2) Transcript Length, (3)
Transcripts per Million (TPM), (4) Estimated number of reads (an estimate of the 
number of reads drawn from this transcript given the transcript's relative abnundance and length).  The first
two columns are self-explanatory, the next four are measures of transcript
abundance and the final is a commonly used input for differential expression
tools.  The Transcripts per Million quantification number is computed as
described in [1], and is meant as an estimate of the number of transcripts, per
million observed transcripts, originating from each isoform.  Its benefit over
the (still common) F/RPKM measure is that it is independent of the mean expressed transcript
length (i.e. if the mean expressed transcript length varies between samples,
for example, this alone can affect differential analysis based on the K/RPKM.).

### Library Format String ### {#library-string}

For a description of the library format strings and meanings, check out the 
[ReadTheDocs documentation](http://sailfish.readthedocs.org).

License
=======

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see \<http://www.gnu.org/licenses/\>.

References
==========

[1] Li, Bo, et al. "RNA-Seq gene expression estimation with read mapping uncertainty." 
    Bioinformatics 26.4 (2010): 493-500.












