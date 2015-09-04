ArgumentParser
==============

Copyright 2015 BrainTwister - All rights reserved.

ArgumentParser is free software made available under the MIT License. For details see the LICENSE file.

Requirements
------------

- cmake (Version >= 3.0, http://www.cmake.org)
- gtest (Optional, but recommended, https://code.google.com/p/googletest)

Description
-----------

ArgumentParser will help you to handle program options very easily with C++.

- All definitions are clearly arranged in the constructor.
- Automatic check of the correct argument type.

```
#include "BrainTwister/ArgumentParser.h"
#include <fstream>
#include <iostream>

namespace bt = BrainTwister;

int main(int argc, char **argv)
{
    bt::ArgumentParser arg(argc, argv, "1.0"
        {{"file", bt::Value<std::string>(), "Input file."}},
        {{"number_of_lines", "n", bt::Value<int>(5), "Number of lines to print."}}
    );

    ifstream ifs(arg.get<std::string>("file"));
    std::string line;
    for (int i(0); getline(ifs, line) and i < arg.get<int>("number_of_lines"); ++i)
        std::cout << line << std::endl;
}
```

- Automatic creation of the help page and the version string.

```
Usage: program <file> [--number_of_lines, -n] [--help, -h] [--version, -v]

Required arguments: 
    MatrixA                   Input file.

Optional arguments: 
    --number_of_lines, -n     Number of lines to print.
    --help, -h                Print help page.
    --version, -v             Print version string.
```

