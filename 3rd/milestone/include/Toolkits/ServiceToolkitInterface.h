/**
 * @file
 *
 * @brief Header file containing definitions of sink toolkit interfaces.
 */

#if !defined(Toolkits_ServiceToolkitInterface_h_INCLUDED)
#define Toolkits_ServiceToolkitInterface_h_INCLUDED

// Local headers
#include "Types.h"
#include "BaseToolkitInterface.h"

/* === Namespace =========================================================== */

namespace NmToolkit
{

  /* === Service toolkit interface ========================================= */

  /* --- ImServiceToolkit class -------------------------------------------- */

  /**
   * @brief Service toolkit interface.
   *
   * This is the interface which all toolkits must implement.
   */
  class ImServiceToolkit : public ImToolkit
  {
    public:
      /** @name Constructors and destructors */ //@{
      /**
       * @brief Virtual toolkit destructor to ensure proper destruction through interface.
       */
      virtual ~ImServiceToolkit() throw() {}
      //@}

      /** @name Process request methods */ //@{
      /** 
       * @brief Process request and get response
       *
       * @param request XML document encoding the toolkit specific request
       * @param response The toolkit specific response to the request encoded as XML document
       *
       * @exception ImInvalidArgumentError Is thrown if @p request could not be parsed.
       */
      virtual void ProcessRequest(const utf8_string_t &request, utf8_string_t &response) = 0;
      //@}
  };

}

#endif // Toolkits_ServiceToolkitInterface_h_INCLUDED
