
#ifndef WELKIN_EXPORT_H
#define WELKIN_EXPORT_H

#ifdef WELKIN_STATIC_DEFINE
#  define WELKIN_EXPORT
#  define WELKIN_NO_EXPORT
#else
#  ifndef WELKIN_EXPORT
#    ifdef welkin_EXPORTS
        /* We are building this library */
#      define WELKIN_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define WELKIN_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef WELKIN_NO_EXPORT
#    define WELKIN_NO_EXPORT 
#  endif
#endif

#ifndef WELKIN_DEPRECATED
#  define WELKIN_DEPRECATED __declspec(deprecated)
#endif

#ifndef WELKIN_DEPRECATED_EXPORT
#  define WELKIN_DEPRECATED_EXPORT WELKIN_EXPORT WELKIN_DEPRECATED
#endif

#ifndef WELKIN_DEPRECATED_NO_EXPORT
#  define WELKIN_DEPRECATED_NO_EXPORT WELKIN_NO_EXPORT WELKIN_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef WELKIN_NO_DEPRECATED
#    define WELKIN_NO_DEPRECATED
#  endif
#endif

#endif /* WELKIN_EXPORT_H */
