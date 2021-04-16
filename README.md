# libugds

libugds is a simple C library that provides a handful of common functionalities used in my C projects.  It is primarily meant for my side projects, but others are free to commit or use it.  

## Principles

* Dynamic.  Changing types at runtime is fine and is preferable to macro based generics.
* Clean. The APIs should be simple and easy to reason about.
* Robust. Handle memory allocation failure and other issues correctly.
* Independent.  The library should function on any POSIX system with a C99 compiler, with minimal dependencies. (The current only dependency is utf8proc).
* Iterative.  The library is meant to be expanded over time and optimized as needed.  Many of the algorithms in this library are naive and will be fixed when needed.

## Building

### Requirements

Building requires ``meson`` and ``ninja``.

It will also require you to have the development libraries for utf8proc.
On Debian based systems this is libutf8proc-dev.

``git clone https://github.com/garfr/libugds``

``cd libugds``

``mkdir build/``

``meson configure build``

``cd build``

``ninja``
