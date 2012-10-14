
#ifndef ADM_COREIMAGELOADER6_EXPORT_H
#define ADM_COREIMAGELOADER6_EXPORT_H

#ifdef ADM_COREIMAGELOADER6_STATIC_DEFINE
#  define ADM_COREIMAGELOADER6_EXPORT
#  define ADM_COREIMAGELOADER6_NO_EXPORT
#else
#  ifndef ADM_COREIMAGELOADER6_EXPORT
#    ifdef ADM_coreImageLoader6_EXPORTS
       /* We are building this library */
#      ifdef _WIN32
#        define ADM_COREIMAGELOADER6_EXPORT __declspec(dllexport)
#      else
#        define ADM_COREIMAGELOADER6_EXPORT __attribute__((visibility("default")))
#      endif
#    else
       /* We are using this library */
#      ifdef _WIN32
#        define ADM_COREIMAGELOADER6_EXPORT __declspec(dllimport)
#      else
#        define ADM_COREIMAGELOADER6_EXPORT __attribute__((visibility("default")))
#      endif
#    endif
#  endif

#  ifndef ADM_COREIMAGELOADER6_NO_EXPORT
#    ifdef _WIN32
#      define ADM_COREIMAGELOADER6_NO_EXPORT 
#    else
#      define ADM_COREIMAGELOADER6_NO_EXPORT __attribute__((visibility("hidden")))
#    endif
#  endif
#endif

#ifndef ADM_COREIMAGELOADER6_DEPRECATED
#  ifdef __GNUC__
#    define ADM_COREIMAGELOADER6_DEPRECATED __attribute__ ((__deprecated__))
#    define ADM_COREIMAGELOADER6_DEPRECATED_EXPORT ADM_COREIMAGELOADER6_EXPORT __attribute__ ((__deprecated__))
#    define ADM_COREIMAGELOADER6_DEPRECATED_NO_EXPORT ADM_COREIMAGELOADER6_NO_EXPORT __attribute__ ((__deprecated__))
#  elif defined(_WIN32)
#    define ADM_COREIMAGELOADER6_DEPRECATED __declspec(deprecated)
#    define ADM_COREIMAGELOADER6_DEPRECATED_EXPORT ADM_COREIMAGELOADER6_EXPORT __declspec(deprecated)
#    define ADM_COREIMAGELOADER6_DEPRECATED_NO_EXPORT ADM_COREIMAGELOADER6_NO_EXPORT __declspec(deprecated)
#  endif
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define ADM_COREIMAGELOADER6_NO_DEPRECATED
#endif

#endif
