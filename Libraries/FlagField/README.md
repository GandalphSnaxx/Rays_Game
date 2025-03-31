# FlagField Library

## Overview
The FlagField Library is a header only C++ library designed to manage a bytefield of flags. It provides a simple and efficient way to set, clear, toggle, and check flags within a byte or a number of bytes, making it useful for applications that require bit manipulation. Operator overloads are also provided for faster and easier implementation.

## Features
- Manage a bytefield of flags with a single byte or multiple bytes.
- Constructors:
  - `FlagField<x>`: `x` is the number of flags to manage. Default `x` is 8.
  - `FlagField<x>()`: Default constructor initalizes every flag to 0.
  - `FlagField<x>(1, 2, 3...)`: Constructs a `FlagField<x>` with pre set flags.
  - `FlagField(FlagField<x>)`: Constructs a `FlagField<x>` as a copy of another `FlagField`.
- Public methods to manipulate flags:
  - `setFlag`: Set a specific flag.
  - `clearFlag`: Clear a specific flag.
  - `toggleFlag`: Toggle the state of a specific flag.
  - `isFlagSet`: Check if a specific flag is set.
  - `size`: Gets the number of flags managed.
- Operator overloads for faster implementation in your project.
  - Binary integer operators `a @ b` where `a` is a FlagField object, `b` is an integer type representing a flag number, and `@` is an operator below:
    - `=`: Clears `a` and sets flag `b`.
    - `==`: Returns `true` if only flag `b` is set.
    - `&`: Returns `true` if flag number `b` is set.
    - `&=`: Clears flag number `b`.
    - `|=`: Sets flag number `b`.
    - `^=`: Flips flag number `b`.
    - `+=`: Sets flag number `b` (same as `|=`).
    - `-=`: Clears flag number `b`.
  - Binary boolean operators `a @ b` where `a` is a FlagField object, `b` is a boolean type, and `@` is an operator below:
    - `*`: Makes a new FlagField object with the same flags as `a` if `b` is `true` or no flags if `b` is `false`.
    - `*=`: Clears every flag in `a` if `b` is `false`.
  - Binary FlagField operators `a @ b` where `a` and `b` are FlagField objects and `@` is an operator below:
    - `=`: Makes `a` match `b`.
    - `&`: Makes a new FlagField object with `a` and `b`'s matching flags.
    - `&=`: Clear assignment operator. Clears every matching `a` and `b` flag in `a`.
    - `|`: Makes a new FlagField object with both `a` and `b`'s flags.
    - `|=`: Set assignment operator. Sets every `b` flag in `a`.
    - `^`: Makes a new FlagField object with `a` XOR `b` flags.
    - `^=`: Set assignment operator. Toggles every `b` flag in `a`.
    - `==`: Returns `true` if `a` and `b` have the same flags set.
    - `!=`: Returns `false` if `a` and `b` have the same flags set.
    - `>`: Returns `true` if `a` has more flags set than `b`.
    - `<`: Returns `true` if `a` has less flags set than `b`.
    - `>=`: Returns `true` if `a` has more or the same number of flags set than `b`.
    - `<=`: Returns `true` if `a` has less or the same number of flags set than `b`.
    - `+`: Makes a new FlagField object with both `a` and `b`'s flags. Same as `|`.
    - `-`: Makes a new FlagField object with `b`'s flags removed from `a`.
    - `+=`: Set assignment operator. Sets every `b` flag in `a`. Same as `|=`.
    - `-=`: Clear assignment operator. Clears every `b` flag in `a`. Same as `&=`.
  - Integer member access operators `a(b)` or `a[b]` where `a` is a FlagField object and `b` is an integer type representing a flag number.
    - `()`: Returns `true` if flag number `b` is set.
    - `[]`: Returns `true` if flag number `b` is set.
  - Unary prefix operators `@a` where `@` is an operator below:
    - `!`: Returns true if no flags are set.
    - `+`: TODO
    - `-`: Casts `a` to an unsigned int.
    - `~`: Flips every flag in `a`.
    - `&`: Gets a pointer to `a`.
    - `*`: TODO
    - `(bool)`: Explicit cast `a` to `false` if no flags are set.
    - `(uint32_t)`: Explicit casts `a` to `uint32_t`.
  - Unary postfix operators `a@` where `@` is an operator below:
    - `++`: Sets every flag in `a`.
    - `--`: Clears every flag in `a`.
- Easy integration with existing C++ projects.

## Installation
To use the ByteField Library, you can clone the repository and build it using CMake. Follow these steps:

1. Clone the repository:
   ```
   git clone https://github.com/yourusername/flagfield-library.git
   ```

2. Navigate to the project directory:
   ```
   cd flagfield-library
   ```

3. Create a build directory and navigate into it:
   ```
   mkdir build
   cd build
   ```

4. Run CMake to configure the project:
   ```
   cmake ..
   ```

5. Build the project:
   ```
   cmake --build .
   ```

## Usage
To use the FlagField class in your project, include the header file and create an instance of the class. Here is a simple example:

```cpp
#include "FlagField.hpp"

constexpr enum flags {
   FLAG_A,  // flag 0
   FLAG_B,  // flag 1
   FLAG_C,  // flag 2
   FLAG_D}; // flag 3


int main() {
   FlagField ff1, ff2; // Makes two empty FlagField objects
   FlagField(FLAG_C, FLAG_D) ff3; // Makes a new FlagField object with FLAG_C and FLAG_D set
   FlagField<uint64_t> bigField; // Makes a FlagField object with 64 flags. Default is 32.

   ff1.setFlag(FLAG_A); // Set the first flag
   if (ff1.isFlagSet(FLAG_A)) { /* Flag is set */ }

   ff1 = FLAG_B; // Set only flag b
   if (ff1 & FLAG_B) { /* Flag is set */ }

   return 0;
}
```

## More Usage
Using enums, you can name flags for your project. Using operator overloads, you can manipulate a FlagField easily. Here is an example:

```cpp
#include <ostream>
#include "FlagField.hpp"

class ExampleClass {
   public:
   ExampleClass() { config = INIT_FLAG; } // Only INIT_FLAG is set

   FlagField config;
   std::vector<uint32_t> list{};

   enum ConfigFlags { // Config flag enum for this class
      INIT_FLAG,
      BLACKLIST_FLAG,
      WHITELIST_FLAG}

   void addWhitelist(uint32_t data) {
      if (config & BLACKLIST_FLAG) { return; } // Checks if BLACKLIST_FLAG is set
      config += WHITELIST_FLAG; // Sets WHITELIST_FLAG
      list.push_back(data); }
   void addBlacklist(uint32_t data) {
      if (config & WHITELIST_FLAG) { return; } // Checks if WHITELIST_FLAG is set
      config += BLACKLIST_FLAG; // Sets BLACKLIST_FLAG
      list.push_back(data);
   }
}

int main() {
   ExampleClass ex1, ex2;
   if (!ex1.config || !ex2.config) { std::cerr << "Somehow not initalized!"; }
   ex1.addWhitelist(10);
   ex2.addBlacklist(20);
   ex2.addWhitelist(40);
   std::cout << (ex1.config != ex2.config) << std::endl; // Output: 1
   FlagField moreFlags = 1 | 2 | 3; // Sets flags 1, 2, and 3 in moreFlags
}
```

## Testing
Unit tests for the ByteField class are provided in the `tests` directory. 

## License
This project is licensed under the MIT License. See the LICENSE file for more details.