#pragma once
#include "glad/glad.h"
#include <assert.h>
#include <iostream>
enum GLenumWrapper{
    BYTE,
    UNSIGNED_BYTE,

    SHORT,
    UNSIGNED_SHORT,
    HALF_FLOAT,

    INT,
    UNSIGNED_INT,
    FLOAT,
    FIXED,
    INT_2_10_10_10_REV,
    UNSIGNED_INT_2_10_10_10_REV,
    UNSIGNED_INT_10F_11F_11F_REV,

    DOUBLE,
};

inline size_t glenum_size(GLenumWrapper glenum){
    switch(glenum){
	case BYTE:
	case UNSIGNED_BYTE:
	    return 1;

	case SHORT:
	case UNSIGNED_SHORT:
	case HALF_FLOAT:
	    return 2;

	case INT:
	case UNSIGNED_INT:
	case FLOAT:
	case FIXED:
	case INT_2_10_10_10_REV:
	case UNSIGNED_INT_2_10_10_10_REV:
	case UNSIGNED_INT_10F_11F_11F_REV:
	    return 4; 

	case DOUBLE:
	    return 8;
    }
    assert(false);
}
inline GLenum unwrap(GLenumWrapper glenum){
    switch(glenum){
	case BYTE:
	    return GL_BYTE;
	case UNSIGNED_BYTE:
	    return GL_UNSIGNED_BYTE;

	case SHORT:
	    return GL_SHORT;
	case UNSIGNED_SHORT:
	    return GL_UNSIGNED_SHORT;
	case HALF_FLOAT:
	    return GL_HALF_FLOAT;

	case INT:
	    return GL_INT;
	case UNSIGNED_INT:
	    return GL_UNSIGNED_INT;
	case FLOAT:
	    return GL_FLOAT;
	case FIXED:
	    return GL_FIXED;
	case INT_2_10_10_10_REV:
	    return GL_INT_2_10_10_10_REV;
	case UNSIGNED_INT_2_10_10_10_REV:
	    return GL_UNSIGNED_INT_2_10_10_10_REV;
	case UNSIGNED_INT_10F_11F_11F_REV:
	    return GL_UNSIGNED_INT_10F_11F_11F_REV;

	case DOUBLE:
	    return GL_DOUBLE;
    }
    assert(false);
}

template <GLenumWrapper T>
struct size;

template<> struct size<BYTE>{static const size_t get = 1;};
template<> struct size<UNSIGNED_BYTE>{static const size_t get = 1;};

template<> struct size<SHORT>{static const size_t get = 2;};
template<> struct size<UNSIGNED_SHORT>{static const size_t get = 2;};
template<> struct size<HALF_FLOAT>{static const size_t get = 2;};

template<> struct size<INT>{static const size_t get = 4;};
template<> struct size<UNSIGNED_INT>{static const size_t get = 4;};
template<> struct size<FLOAT>{static const size_t get = 4;};
template<> struct size<FIXED>{static const size_t get = 4;};
template<> struct size<INT_2_10_10_10_REV>{static const size_t get = 4;};
template<> struct size<UNSIGNED_INT_2_10_10_10_REV>{static const size_t get = 4;};
template<> struct size<UNSIGNED_INT_10F_11F_11F_REV>{static const size_t get = 4;};

template<> struct size<DOUBLE>{static const size_t get = 8;};

inline void check_gl(const char* where)
{
    GLenum err;

    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error at "
                  << where
                  << ": 0x"
                  << std::hex
                  << err
                  << std::dec
                  << '\n';
    }
}
