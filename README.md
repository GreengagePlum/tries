# Tries

<!-- For the compatibility of GitHub and Doxygen at the same time I had to use this line of html to attach an image -->
<a href="https://github.com/GreengagePlum/tries/actions/workflows/test.yml"><img alt="test workflow status" src="https://github.com/GreengagePlum/tries/actions/workflows/test.yml/badge.svg"/></a>
<a href="https://github.com/GreengagePlum/tries/actions/workflows/deploy.yml"><img alt="deploy workflow status" src="https://github.com/GreengagePlum/tries/actions/workflows/deploy.yml/badge.svg"/></a>

Here is [a programming project](https://github.com/GreengagePlum/tries) in **C (ISO C17)** that aims to implement the Patricia-Trie and the Hybrid Trie. The trie is a data structure that is used to represent a dictionary of words.

This project uses the [Unity](https://github.com/ThrowTheSwitch/Unity.git) unit test framework for C as well as
[cJSON](https://github.com/DaveGamble/cJSON.git) for the handling of JSON. These dependencies are integrated to the
project via git submodules so don't forget to fetch them when cloning as explained further below.

The online documentation for this project is hosted by GitHub Pages and is found at this address <https://greengageplum.github.io/tries/>. It is automatically built during each release using GitHub Actions.

## A preview

_Under construction..._

## Authors

This programming project was carried out for the ALGAV (Advanced Algorithmics) class of the Computer Science Master's first year, first semester (M1S1 2024/25) at Sorbonne Université by the following two fellow gentlemen.

* Efe ERKEN 21400542
* Deniz Ali DURBIN 21111116

## gcc/clang versions

The C language is used (ISO C17). "GNU gcc" and "Apple clang" are used as compilers. Right below are the versions of these compilers that were used:

```text
Apple clang version 16.0.0 (clang-1600.0.26.3)

gcc-14 (Homebrew GCC 14.2.0_1) 14.2.0

Homebrew clang version 19.1.2
```

## Usage

### How to compile and execute?

First, install the dependencies with your package manager (apt, dnf, apk, brew, etc.): `gcc` (GNU C toolchain), `make`.

Then download the project on your machine via `git clone --recurse-submodules`.

Once that's done, go to the project directory and compile.

```sh
cd tries/

make
```

From now on you can run the program via its interface scripts so long as you're in its directory.

```sh
./inserer [0 | 1] [file] [isRebalanced]
./suppression [0 | 1] [file]
./fusionPat [file] [file]
./listeMots [0 | 1] [file]
./profondeurMoyenne [0 | 1] [file]
./prefixe [0 | 1] [file] [word]
```

### Generate the documentation

First, install the dependencies with your package manager (apt, dnf, apk, brew, etc.): `doxygen`, `graphviz`.

To generate the documentation for your copy of the program, use the following command and take a look at the file `doc/public/index.html` in your favorite web browser.

```sh
make doc
```

Or else, you could see the [online documentation](https://greengageplum.github.io/tries/) for the latest version.

### Run the unit tests

```sh
make test
```

You can also use `make all` to both compile the final executable and compile and run the unit tests like `make test`.
It's a handy shortcut to running both operations at once to check that the changes you have introduced don't break
neither the program compilation nor the unit tests.

### How to clean?

To clean the project directory to start from zero there are multiple useful commands.

Remove all compilation files (`*.o`, `*.d`, final executable). This is to be done especially if you're changing the options of `gcc` (like adding `-g` for debugging).

```sh
make clean
```

Remove the documentation and its directories

```sh
make cleandoc
```

This is a command that groups together all that came before. It removes all the compilation, documentation and archive files as well as all directories.

```sh
make cleanall
```

### How to switch between debug and release builds?

To build the debug version (`-g` compiler option for debugger and assertions enabled), all you have to do is add `DEBUG=1` after `make` in any of the above described build targets
just like `make DEBUG=1 all` for example. But be careful to run `make cleanall` before changing between debug and release
builds to remove and force rebuild of all files. If you omit the `DEBUG=...` flag after `make` or use `DEBUG=0`, the
release build will be built (compiler optimizations enabled, assertions disabled), which is geared to run fast for
production.

### Development environment

To have a developer environment for this projet to develop locally or to make contributions, follow the past instructions to get the source code and install the dependencies.

## License

This project is under the ["MIT" license](LICENSE).
