/**
 * @file
 *
 * @brief Header file containing definitions of the server command service client interface.
 */

#if !defined(ServerCommandServiceClient_h_INCLUDED)
#define ServerCommandServiceClient_h_INCLUDED

// The stdint.h header file was not shipped with products prior to
// Borland C++ Builder 6 and Microsoft Visual Studio 2010.
#if defined(__BORLANDC__) && (__BORLANDC__ < 0x0560) || defined(_MSC_VER) && (_MSC_VER < 1600)
  #include <windows.h>          // Use Windows' type definition
  typedef INT64 int64_t;
#else
  #include <stdint.h>           // Use the C99 official header
#endif
#include <string>


/* === Namespace =========================================================== */

namespace NmServerCommandService
{

  /* ==== Macros =========================================================== */

  /**
    * @brief Macros specifying the calling convention.
    */
  #if defined(WIN32)
    #define DLLCALL __stdcall
    #define DLLFUNCTION(retval) extern "C" retval __stdcall
  #else
    #define DLLCALL
    #define DLLFUNCTION(retval) extern "C" retval
  #endif


  /* ==== DLL function prototypes ========================================== */

  extern "C"
  {
    typedef class ImServerCommandServiceClient* (DLLCALL *CreateInstanceFuncPtr_t)();
    typedef void (DLLCALL *DeleteInstanceFuncPtr_t)(class ImServerCommandServiceClient *client);
  }


  /* ==== Basic types ====================================================== */

  /**
   * @brief Type representing an UTF-8 encoded character.
   */
  typedef const char utf8_char_t;

  /**
   * @brief Type representing a time stamp as milliseconds since January 1, 1970, 00:00:00 UTC.
   */
  typedef int64_t utc_time_t;

  /**
   * @brief Type representing a time span in milliseconds.
   *
   * A time span can be both positive and negative.
   */
  typedef int64_t time_span_t;

  /**
    * @brief XProtect product family enumeration.
    */
  enum XProtectProductFamily_t
  {
    XProtectCorporateFamily,
    XProtectEnterpriseFamily
  };

  /**
    * @brief Enumeration of supported authentication methods.
    */
  enum AuthenticationMethod_t
  {
    WindowsAuthentication,
    BasicAuthentication
  };


  /* === Declaration of the client interface =============================== */

  /**
    * @brief ImServerCommandServiceClient interface class.
    */
  class ImServerCommandServiceClient
  {
  public:
    /** @name Constants */ //@{
    static const unsigned short DefaultPort = 0;
    //@}

  public:
    /** @name Set server methods */ //@{
    virtual void DLLCALL SetServerProduct(XProtectProductFamily_t product) = 0;                 ///< Set the product family of the server.
    virtual void DLLCALL SetServerHostName(utf8_char_t hostName[], size_t hostNameLength) = 0;  ///< Set the host name or ip address of the server.
    virtual void DLLCALL SetServerPort(unsigned short port) = 0;                                ///< Set the port number of the server.
    //@}

    /** @name Set authentication methods */ //@{
    virtual void DLLCALL SetUserName(utf8_char_t userName[], size_t userNameLength) = 0;        ///< Set the user name when connecting to the server.
    virtual void DLLCALL SetPassword(utf8_char_t password[], size_t passwordLength) = 0;        ///< Set the host name or ip address of the server.
    virtual void DLLCALL SetAuthenticationMethod(AuthenticationMethod_t method) = 0;            ///< Set the host name or ip address of the server.
    //@}

  public:
    /** @name Login/logout methods */ //@{
    virtual bool DLLCALL Login(utf8_char_t* token[],
                               size_t *tokenLength,
                               utc_time_t *registrationTime,
                               time_span_t *timeToLive,
                               bool *limited) = 0;          ///< Login on the server and obtain a token.
    virtual bool DLLCALL Logout() = 0;                      ///< Logout from the server and release token.
    //@}

    /** @name Get property methods */ //@{
    virtual bool DLLCALL GetServerId(utf8_char_t* serverId[], size_t *serverIdLength) = 0;  ///< Get the unique id of the management or enterprise server.
    //@}

    virtual ~ImServerCommandServiceClient() {};
  };


  /* === Exported DLL functions ============================================ */

  /* --- Create/delete instance  ------------------------------------------- */

  /**
  * @brief Create new client instance.
  * @return A new server command service client instance that implements the ImServerCommandServiceClient interface.
  */
  DLLFUNCTION(ImServerCommandServiceClient *) CreateInstance();

  /**
  * @brief Delete a client instance.
  * @param client The server command service client instance to delete.
  */
  DLLFUNCTION(void) DeleteInstance(ImServerCommandServiceClient *client);

}

#endif // ServerCommandServiceClient_h_INCLUDED
