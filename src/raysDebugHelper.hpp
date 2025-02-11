#ifndef RAYS_DEBUG_HELPER_HPP__
#define RAYS_DEBUG_HELPER_HPP__
#include <iostream>

using namespace std;

// Uncomment the following line to enable debugging
// #define DEBUG

// Define the DEBUG_MSG, DEBUG_VAR, DEBUG_NO_ENDL, and DEBUG_ENDL macros
// If DEBUG is not defined, these macros will be empty and anywhere they are used will be removed by the preprocessor
#ifdef DEBUG
#define DEBUG_MSG(msg)      cout << msg << endl;
#define DEBUG_VAR(var)      cout << #var << " = " << var << endl;
#define DEBUG_NO_ENDL(msg)  cout << msg;
#define DEBUG_ENDL          cout << endl;
#else
#define DEBUG_MSG(msg)
#define DEBUG_VAR(var)
#define DEBUG_NO_ENDL(msg)
#define DEBUG_ENDL
#endif

#endif