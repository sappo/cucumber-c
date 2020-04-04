# Cucumber-C

Cucumber-C allows Cucumber to support step definitions written in C.

* [Cucumber Website](https://cucumber.io)

## Overview

### Scope and Goals

Cucumber-C has these goals:

* To enbale C developers with a library to write Gherkin style BDD tests.
* To provide a clean API that let's the user(s) focus on writing step definitions.
* To integrate with [zproject](https://github.com/zeromq/zproject).

### Ownership and License

The contributors are listed in AUTHORS. This project uses the MPL v2 license, see LICENSE.

Cucumber-C uses the [C4.1 (Collective Code Construction Contract)](https://rfc.zeromq.org/spec:22) process for contributions.

Cucumber-C uses the [CLASS (C Language Style for Scalabilty)](https://rfc.zeromq.org/spec:21) guide for code style.

To report an issue, use the [Cucumber-C issue tracker](https://github.com/sappo/cucumber-c/issues) at github.com.

## Getting started

TBD...

## Under the Hood

### Hints to Contributors

Cucumber-C is a nice, neat library, and you may not immediately appreciate why. Read the CLASS style guide please, and write your code to make it indistinguishable from the rest of the code in the library. That is the only real criteria for good style: it's invisible.

Don't include system headers in source files. The right place for these is cucumber_prelude.h. If you need to check against configured libraries and/or headers, include platform.h in the source before including cuc.h.

Do read your code after you write it and ask, "Can I make this simpler?" We do use a nice minimalist and yet readable style. Learn it, adopt it, use it.

Before opening a pull request read our [contribution guidelines](https://github.com/sappo/cucumber-c/blob/master/CONTRIBUTING.md). Thanks!

### Code Generation

We generate scripts for build systems like autotools, cmake and others as well as class skeletons, class headers, the selftest runner and more using zproject. Generated files will have a header and footer telling you that this file was generated. To re-generate those files it is recommended to use the latest `zeromqorg/zproject` docker image. 

#### Docker

* Clone [libzmq](https://github.com/zeromq/libzmq) into the same directory as cucumber-c. 
* Clone [czmq](https://github.com/zeromq/czqm) into the same directory as cucumber-c. 

Next always download the latest image: 

```sh
# Make sure 
docker pull zeromqorg/zproject:latest
```

Then run the following command:

```sh
# Shell and Powershell
docker run -v ${PWD}/..:/workspace -e BUILD_DIR=/workspace/cucumber-c zeromqorg/zproject

# Windows CMD
docker run -v %cd%/..:/workspace -e BUILD_DIR=/workspace/cucumber-c zeromqorg/zproject
```

#### Linux and MacOS

* Install [GSL](https://github.com/zeromq/gsl) and [zproject](https://github.com/zeromq/zproject)
* Clone [libzmq](https://github.com/zeromq/libzmq) into the same directory as cucumber-c
* Clone [czmq](https://github.com/zeromq/czmq) into the same directory as cucumber-c

Then run the following command:

	gsl project.xml
