# C++ Kit
This repo is a collection of C++ tools which I created myself to facilitate my C++ development process.
## Content
- [cpp-styleguide](styleguide.md), based on personal preferences
- [hppmerge](hppmerge/hppmerge.py), to resolve includes in a header or source file
- build-scripts, using premake5
    - for [console-app](app.lua) development
    - for [library](lib.lua) development

## C++ Styleguide
This styleguide is based on personal preferences and is inspired by [google's styleguide](https://google.github.io/styleguide/cppguide.html).

Currently featured contents are
- naming conventions
- comments
- formatting

## HppMerge
>Python script to resolve includes in a C++ header or source file

This project allows you to resolve include dependencies in your header or source files.
A usecase for this would be if you want to distribute your C++ project using a single-include header file.

>[!IMPORTANT]  
>The output file with the resolved includes will have all multi-line comments removed.

### Usage
Execute the following command in your console.
```console
python3 path/to/hppmerge.py ORIGIN DEST --default DEFAULT --ignore IGNORE
```
- `ORIGIN` = the path to your header / source file you want to resolve the includes in
- `DEST` = the desired path of the output file
- `DEFAULT` (_optional_) = all default include directories that your compiler is using (_seperated by whitespace_)
- `IGNORE` (_optional_) = all filepaths (_glob_) hppmerge should not resolve (_seperated by whitespace_)

## Build Scripts
Use the following scripts
- for [console-app](app.lua) development
- for [library](lib.lua) development

It is recommended to put the respective script in a subdirectory, e.g. `/build`.
### Dependencies
The script is processed by [premake5](https://premake.github.io).

Additionally, you need
- [make](https://www.gnu.org/software/make) for linux development
- [visual studio](https://visualstudio.microsoft.com) for windows development

### Usage
Execute the following command in your console
```console
premake5 path/to/script.lua gmake && make
```
if you're using linux or
```console
premake5 path/to/script.lua vs2022
```
and navigate visual studio
if you're using windows.