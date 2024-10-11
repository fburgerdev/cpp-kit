# C++ Styleguide
My personal projects follow these guidelines.
They are based on personal preferences.

# Table of Contents
1. [Naming Convetions](#1-naming-conventions)
2. [Comments](#2-comments)
3. [Formatting](#3-formatting)

# 1. Naming Conventions
## Types
>Types are named in `PascalCase`.

Note that this applies to
_classes_, _structs_, _enums_, _aliases_, _typedefs_ and _concepts_!

## Functions
>Functions are named in `camelCase`.

Function names should be structured in one of the following ways:
- _verb_, e.g. _sort( )_
- _verb_ + _Object_, e.g. _constructEntity( )_
- _verb_ + _Adjective_ + _Object_, e.g. _calcLegalMoves( )_

Functions of "constant character" are named in `PascalCase`.
They don't start with a verb.

For example:
```cpp
int TypeSize(DType type) {
    switch (type) {
    case DType::INT:
        return 4;
    case DType::FLOAT:
        return 4;
    case DType::DOUBLE:
        return 8;
    default:
        return -1;
    }
}
```

## Variables
>Variables are named in `camelCase`.

Depending on the storage duration, the following prefixes are added:
- `m_` for _member_ variables, e.g. _m_elementCount_
- `s_` for _static_ variables, e.g. _s_rootPath_
- `g_` for _global_ variables, e.g. _g_projectConfig_

## Constants
>Constants are named in `UPPER_CASE`.

A variable is considered a constant if declared _const_ or _constexpr_
and whose value is fixed for the duration of the program.

Note that the following is not considered a constant:

```cpp
Vec3 normalize(Vec3 vector) {
    const double length = vector.length();
    return vector / length;
}
```

## Namespaces
>Namespaces are named in `PascalCase`.

## Enumerators
>Enumerators are named in `UPPER_CASE`.

For example:
```cpp
enum Color {
    RED, GREEN, BLUE
};
```
## Macros
>Macros are named in `UPPER_CASE`.

# 2. Comments

>Use `// ...` insted of `/* ... */`.

## Structural Comments
Structural comments are used to emphasize the structure of a file.
They carry no additional information about the code semantics.

### Types
> Type declarations are emphasized by adding comments with their name in front.

For example:
```cpp
// Point
struct Point {
    int x, y;
}
```

### Functions
>Funtion definitions are emphasized by adding comments with their name in front.

For example:
```cpp
// isEven
bool isEven(int num) {
    return !bool(num % 2);
}
```

>Overloaded function declarations are emphasized _once_.

For example:

```cpp
// isPrime
bool isPrime(int num);
bool isPrime(unsigned int num);
```

>Functions can be grouped together
>by adding a comment with the group logic in front (in _lowercase_).

For example:
```cpp
// arithmetic
Vec3 operator+(Vec3 x, Vec3 y);
Vec3 operator-(Vec3 x, Vec3 y);
Vec3 operator*(Vec3 x, double scalar);
Vec3 operator/(Vec3 x, double scalar);
```

>Constructors and destructors are emphasized with `// constructor` and `// destructor` instead of their type names.

For example:
```cpp
// Point
class Point {
    // constructor
    Point(int x, int y)
        : x(x), y(y) {}
    // member
    int x, y;
};
```

>Nested emphases are prefixed with a `::`.

For example:
```cpp
// arithmetic
// :: operator+
Vec2 operator+(Vec2 x, Vec2 y);
Vec3 operator+(Vec3 x, Vec3 y);
// :: operator-
Vec2 operator-(Vec2 x, Vec3 2);
Vec3 operator-(Vec3 x, Vec3 y);
```

Note that this doesn't apply when the emphases occur in different scopes.


>Code blocks in function definitions can be grouped together into logical parts (in _lowercase_).

For example:
```cpp
// castle privileges
// :: white king moves
if (origin == toSquare("e1")) {
    castle[Castle::WHITE_KINGSIDE] = false;
    castle[Castle::WHITE_QUEENSIDE] = false;
}
// :: black king moves
else if (origin == toSquare("e8")) {
    castle[Castle::BLACK_KINGSIDE] = false;
    castle[Castle::BLACK_QUEENSIDE] = false;
}
// :: white kingside rook moves
else if (origin == toSquare("h1")) {
    castle[Castle::WHITE_KINGSIDE] = false;
}
// :: white queenside rook moves
else if (origin == toSquare("a1")) {
    castle[Castle::WHITE_QUEENSIDE] = false;
}
// :: black kingside rook moves
else if (origin == toSquare("h8")) {
    castle[Castle::BLACK_KINGSIDE] = false;
}
// :: black queenside rook moves
else if (origin == toSquare("a8")) {
    castle[Castle::BLACK_QUEENSIDE] = false;
}
```

# 3. Formatting
## Indentation
>Indent with spaces. Each indentation level contains 4 spaces.

>Braces are positioned using the `K&R` style.

For example:
```cpp
int main() {
    return EXIT_SUCCESS;
}
```