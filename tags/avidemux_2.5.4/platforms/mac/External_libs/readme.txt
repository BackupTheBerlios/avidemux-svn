External Libraries
(This is for 10.4 or above)

**************************************************************************
* 22 July 2008, Harry van der wolf
* This is very much a "work in progress" thing and far from stable.
* It can be considered in pre-alpha stage.
* Scripts up to and including 0215 "should" work
* Some 300 scripts should work too (xvid), others "are in the make"
**************************************************************************



CREDITS
This way of building libraries for OSX has been developed (AFAIK) by Ippei Ukai for the
Open Source project Hugin. I simply copied the way of working and developed and created
the scripts necessary for Avidemux (unfortunately only a few libraries are the same) 

INTRODUCTION
The scripts in the scripts directory can be used to build the necessary libraries for Avidemux.
This part describes where we want to place the development tree for our "External Libraries" that 
Avidemux depends on. Although the "External Libraries" directory "External_libs" structure is 
placed inside the Avidemux SVN tree by default, this does not necessarily need to be the place 
where you really want to build your libraries. Even more, it's better to place your build tree
somewhere else. 

So the first question then is: where do you want to have your development tree? As you (might) know,
the "normal" location is /usr/local. MacPorts uses /opt/local by default and Fink uses /sw. 
We do not want to use these locations!

Apart from the fact that it is a bad idea to mix up development trees, another drawback is that 
these directories are not in "user space", therefore always requiring a root authorization, 
e.g. "sudo make install" as a last step. When keeping the development tree in user space 
(e.g. /Users/<your_username>/development/ or /Users/Shared/development/), you don't need to "sudo". 
Note that the latter option also creates a development directory in user space but keeps it away from 
your "normal" user data.

So, from this moment "we" have decided to build our development tree in user space.

Note: As mentioned before: If you position your development tree outside user space, you need to 
run everything as root user. The scripts are not tailored towards that "sudo" kind of use 
and need modification to work that way. 

PREPARATIONS
It is best to create your development tree next to your avidemux tree. So if you have your
Avidemux SVN tree in /Users/<username>/development/avidemux_2.4_branch, you make your
"External_libs" tree in /Users/<username>/development/External_libs.
This has a couple of advantages:
- If you screw up your avidemux tree, you can simply throw it away without worrying about
your built libraries.
- If the name of the avidemux branch might change due to new developments or versions, you can still
continue to use your own libraries tree and leave it where it is.
- If you also built on other projects and want to use this same approach you can use this libraries
tree next to your avidemux_2.4_branch, <project 2>, <project 3>, etc .. source trees (like /usr/local,
/opt/local (MacPorts) or /sw (Fink) also contain all "non system" libraries on your system). 

We first create our development tree next to our avidemux tree (assuming we are in 
/Users/<username>/development where also our avidemux tree resides) and inside it a repository directory
four our binaries/libraries.
$ mkdir External_libs External_libs/repository

Now you can create a scripts directory inside your External_libs directory or create a symbolic
link (recommended) to the scripts directory in your avidemux tree, thereby automatically synchronizing
scripts in that tree via SVN. I create a symbolic link.
$ ln -s /Users/<username>/development/avidemux_2.4_branch/platforms/mac/External_libs/scripts /Users/<username>/development/External_libs/scripts


WARNING
The configure scripts of the libraries you want to compile, also check for libraries and headers
inside /usr/local, /opt/local (Maccports) and /sw (Fink).
This means that we need to get them "out of the way" as we do not want the libraries to link to 
these headers and libraries, but only to the headers and libraries inside out External_libs build tree.
So, before starting to build, you need to disable them by renaming them (not entirely without risk).
$ sudo mv /usr/local /usr/local.org   (Linux way of building)
$ sudo mv /opt/local /opt/local.org   (MacPorts)
$ sudo mc /sw /sw.org                 (Fink)

Note: You can off course give them any name you like.
Note 2: Do not a "sudo mv /usr /usr.org" as your system won't function anymore 


HOWTO:
0. Modify your External_libs/scripts/SetEnv-universal.txt. The path in the myREPOSITORYDIR variable 
needs to match exactly the path you use. So, if you are Spiderman and you build inside your HOME 
directory you need to specify:
myREPOSITORYDIR="/Users/Spiderman/development/External_libs/repository";

1. Download the sources. The top of the script will show you a "download location" where you can find the source.
You typically want to place it in the "External_libs" folder.
2. Check your External_libs/scripts/SetEnv-universal.txt file, especially the myREPOSITORYDIR variable.
3. Open a Terminal window (bash is preferred).
5. 'cd' into the directory of source you want to compile. (eg. 'cd External_libs/jpeg-6b')
4. Set the variables for the compilation. (eg. 'source ../scripts/SetEnv-universal.txt')
6. Using the appropriate shell script, build the source. (eg. 'sh ../scripts/0120-libjpeg.sh')


RESULT:
The programs and libraries will be installed into $myREPOSITORYDIR, which you can manage independently 
from the systems you are currently using (e.g /usr, /usr/local, /opt, /sw).


TIPS:
When compiling programs from source you need to specify the above directory as prefix. You probably have to 
specify the correct SDK (-isysroot) and MacOS target version (-mmacosx-version-min) as well.
You can make multiple 'repositories' as well. For example, you can make one for statically and 
one for dynamically linked product, or ones with different target architecture/OS versions. 


LIBRARIES:
The library scripts are all numbered and should be executed in the order they are numbered for the
scripts below 300.
The scripts above 300 build the plugins for Avidemux. They are not neccessary but will expand the
possible codecs you can use. Note that most scripts have gaps between numbers. There are also a couple
of patch scripts. They should be run (once) before building the libraries.


LICENSE:
The scripts for compiling universal builds are originally copyrighted by Ippei Ukai (2007-2008), and distributed under the modified BSD license.


