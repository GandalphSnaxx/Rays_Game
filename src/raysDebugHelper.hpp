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

// Error message macros
#define THROW_ERR(msg)      throw runtime_error(msg);
#define CERR(msg, var)      std::cerr << msg << var << std::endl;

// Check for vulkan errors
#define VK_CHECK(result)                \
    if (result != VK_SUCCESS) {         \
        CERR("Vulkan Error: ", result); \
        __debugbreak();                 \
    }

// A macro for error checking in functions that need to perform multiple functions that could return an error
#define ERROR_RETURN(var)                                                   \
result = var;                                                               \
if (result != VK_SUCCESS) {                                                 \
    DEBUG_MSG("Vulkan Error: " << result << " | In function: " << #var);    \
    return result;                                                          \
}                                                                           \

// Macro for getting the size of an array
#define ARRAY_SIZE(arr) sizeof(arr[0]) * arr.size()

#endif