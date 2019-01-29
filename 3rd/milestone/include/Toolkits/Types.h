/**
 * @file
 *
 * @brief Header file containing definitions of toolkit types.
 */

#if !defined(Toolkits_Types_h_INCLUDED)
#define Toolkits_Types_h_INCLUDED

// The stdint.h header file is not shipped with Visual Studio C++ products prior to Visual Studio 2010
#if !defined(_MSC_VER) || (_MSC_VER >= 1600)
  #include <stdint.h>           // Use the C99 official header
#else
  #include <windows.h>          // Use MSVC's type definition
  typedef INT64      int64_t;
  typedef ULONGLONG  uint64_t;
  typedef UINT32     uint32_t;
#endif
#include <string>


/* === Namespace =========================================================== */

namespace NmToolkit
{

  /* ==== Basic types ====================================================== */

  /**
   * @brief Type representing a byte.
   */
  typedef unsigned char byte_t;

  /**
   * @brief Type representing a file size.
   */
  typedef uint64_t file_size_t;

  /**
   * @brief Type representing an UTF-8 encoded string.
   *
   * All strings that go in and out of the toolkits are encoded using
   * UTF-8. Note that UTF-8 is backwards compatible with ASCII strings
   * and these can thus be used without modification.
   */
  typedef std::string utf8_string_t;

  /**
   * @brief Type representing a time stamp as milliseconds since January 1, 1970, 00:00:00 UTC.
   *
   * All time stamps that go in and out of the toolkits are
   * represented by an 64-bit integer that holds the number of
   * milliseconds since January 1, 1970, 00:00:00 UTC. Note that the
   * integer is signed and can thus represent time before this date.
   */
  typedef int64_t utc_time_t;

  /**
   * @brief Type representing a time span in milliseconds.
   *
   * A time span can be both positive and negative.
   */
  typedef int64_t time_span_t;

}

#endif // Toolkits_Types_h_INCLUDED
