# README #

Install using Tinderbox.


To compile mupdf on OSX:

- recursively clone the [repo](git://git.ghostscript.com/mupdf.git)
- compile without X11 `make HAVE_X11=no`
- copy over include folder and the compiled libs

To compile mupdf on Windows:

- recursively clone the [repo](git://git.ghostscript.com/mupdf.git)
- open Visual Studio solution with Visual Studio Community 2013 and let project update
- build the `libmupdf` project in `Release mode` for `x64`
- copy over include folder and the compiled libs