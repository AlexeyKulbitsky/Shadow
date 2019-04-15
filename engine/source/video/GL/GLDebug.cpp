#include "video/GL/GLDebug.h"

namespace sh
{

namespace video
{

    void GLDebug::CheckGLError(const char* file, unsigned line)
    {
        GLenum error = glGetError();
        while (error != GL_NO_ERROR)
        {
            std::ostringstream ss;
            ss << "File: " << file << " Line: " << line << " ";
            switch (error)
            {
            case GL_INVALID_ENUM:
                ss << "ERROR: GL_INVALID_ENUM\n" << "Enumeration parameter(s) is(are) not a legal enumeration for this function";
                break;
            case GL_INVALID_VALUE:
                ss << "ERROR: GL_INVALID_VALUE\n" << "Value parameter(s) is(are) not a legal value(s) for this function";
                break;
            case GL_INVALID_OPERATION:
                ss << "ERROR: GL_INVALID_OPERATION\n" << "The set of state for a command is not legal for the parameters given to that command";
                break;
            case GL_STACK_OVERFLOW:
                ss << "ERROR: GL_STACK_OVERFLOW\n" << "Stack pushing operation cannot be done because it would overflow the limit of that stack's size";
                break;
            case GL_STACK_UNDERFLOW:
                ss << "ERROR: GL_STACK_UNDERFLOW\n" << "Stack popping operation cannot be done because the stack is already at its lowest point";
                break;
            case GL_OUT_OF_MEMORY:
                ss << "ERROR: GL_OUT_OF_MEMORY\n" << "Memory cannot be allocated";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                ss << "ERROR: GL_INVALID_FRAMEBUFFER_OPERATION\n" << "Doing anything that would attempt to read from or write/render to a framebuffer that is not complete";
                break;
            default:
                ss << "ERROR: unknown OpenGL error";
                break;
            }
            std::string message = ss.str();
            std::cout << "[OpenGL error]: " << message << std::endl;
            error = glGetError();
        }
    }

} // video

} // sh