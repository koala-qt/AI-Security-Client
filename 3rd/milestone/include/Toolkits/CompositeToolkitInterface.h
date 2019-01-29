/**
 * @file
 *
 * @brief Header file containing definitions of sink toolkit interfaces.
 */

#if !defined(Toolkits_CompositeToolkitInterface_h_INCLUDED)
#define Toolkits_CompositeToolkitInterface_h_INCLUDED

// Local headers
#include "Types.h"
#include "BaseToolkitInterface.h"

/* === Namespace =========================================================== */

namespace NmToolkit
{

  /* === Composite Toolkit interface ================================================= */

  /* --- ImCompositeToolkit class --------------------------------------------------- */

  /**
   * @brief Composite toolkit interface.
   *
   * This is the interface which all composit toolkits must implement.
   */
  class ImCompositeToolkit : public ImToolkit
  {
    public:
      /** @name Constructors and destructors */ //@{
      /**
       * @brief Virtual toolkit destructor to ensure proper destruction through interface.
       */
      virtual ~ImCompositeToolkit() throw() {}
      //@}

      /** @name Get Implementation method */ //@{
      /** 
       * @brief Get pointer to composite object implementation (may be this object itself or another toolkit referenced)
       */
      virtual ImToolkit* GetImplementation() throw() = 0;
      //@}
  };

}

#endif // Toolkits_CompositeToolkitInterface_h_INCLUDED
