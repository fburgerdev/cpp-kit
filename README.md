# Description
>Python script for resolving includes in a C++ header file

This project allows you to resolve include dependancies in your C++ header files.
This is useful if you want to distribute your C++ project using a single header file.

Note: The output file with the resolved includes won't have any multi-line comments.

# Usage
Execute the following command in your console.
```console
python3 PATH_TO_REPO/hppmerge.py ORIGIN_PATH DEST_PATH --default DEFAULT1 DEFAULT2 ... --ignore IGNORE1 IGNORE2 ...
```

# Installation
At your desired location, execute the following command in the console.
``` console
git clone git@github.com:fburgerdev/hppmerge.git
```
or simply copy the content of the _hppmerge.py_ wherever you want.