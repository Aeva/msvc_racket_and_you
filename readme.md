# How to embed Racket CS into C++ programs on Windows with Visual Studio

Windows users attempting to embed Racket CS into a C or C++ program will find that it is
currently impossible to do so by only following
[the documentation](https://docs.racket-lang.org/inside/cs-embedding.html), at least as of September 19th, 2021.

There are two pitfalls you will run into:

 1. Racket CS is not distributed with .lib files for the dlls, so symbol resolution at link time is
    not possible without generating your own.  Delay loading is suggested as a possibility, but
    there is no provided functionality to assist in this, so you're on your own to write all of your
    mountain of GetProcAddress boilerplate.

 2. The Windows distribution of Racket CS 8.2 ships with the Linux `chezscheme.h`, so it is impossible to
    end up with a working program anyway.

Racket CS is built with MinGW, and as one of the maintainers has assured me, it is simply just not worth their
time to support embedding Racket with the platform's native toolchain.

So the solutions to the above problems are:

 1. Generate your own .lib file, or write your own GetProcAddress boilerplate.  God help your painful deteriorating
    RSI-addled wrists, but take comfort that a maintainer didn't have to update the build system.

 2. This project has the correct version of `chezscheme.h`.  The build system has been at least been since
    corrected to distribute the correct headers, so if you wait a bit, the official distribution will probably be
    fine too.

## Generating your own .lib files

 1. Create a file called `libracketcs_d9hn5s.def` (or similar, if your .dll is not named `libracketcs_d9hn5s.dll`)
    with each symbol you need on a separate line after `EXPORTS`, like so:
    ```
    EXPORTS
    racket_boot
    declare_modules
    ```

 2. Run this command (adjust the file names to match your dll version) from a Visual Studio developer command prompt:
    ```lib /def:libracketcs_d9hn5s.def /out:libracketcs_d9hn5s.lib /machine:x64```

 3. Add your `libracketcs_d9hn5s.lib` to the project's "Additional Dependencies" field in the Linker -> Input settings.

 4. (Optional) use the "Custom Build Step" project setting to automate step 2.  Use the same command for the "Command Line" field,
    set Outputs to "libracketcs_d9hn5s.lib", and set "Execute Before" to Link.  You will have to rebuild the project every time you
    update defs however.
