/**
 * @file
 *
 * @brief Header file containing definitions of dictionary toolkit interfaces.
 */

#if !defined(Toolkits_DictionaryToolkitInterface_h_INCLUDED)
#define Toolkits_DictionaryToolkitInterface_h_INCLUDED

// Local headers
#include "Types.h"
#include "BaseToolkitInterface.h"

/* === Namespace =========================================================== */

namespace NmToolkit
{

  /* === Dictionary Toolkit interface ====================================== */

  /* --- ImDictionaryToolkit class ----------------------------------------- */

  /**
   * @brief Dictionary toolkit interface.
   *
   * This is the interface which all toolkits must implement.
   */
  class ImDictionaryToolkit : public ImToolkit
  {
    public:
      /** @name Constructors and destructors */ //@{
      /**
       * @brief Virtual toolkit destructor to ensure proper destruction through interface.
       */
      virtual ~ImDictionaryToolkit() throw() {}
      //@}

      /** @name Add and remove toolkit methods */ //@{
      /** 
       * @brief Create toolkit instance and add it to the dictionary
       *
       * @param name The dictionary name of the toolkit instance
       * @param configXml The configuration xml for the toolkit instance
       *
       * @exception ImInvalidArgumentError Is thrown if name is already present in dictionary
       */
      virtual void Add(const utf8_string_t &name, const utf8_string_t &configXml) = 0;

      /** 
       * @brief Remove and destroy a toolkit instance by name
       *
       * @param name The dictionary name of the toolkit instance to remove from dictionary and destroy
       *
       * @exception ImInvalidArgumentError Is thrown if name was not found in dictionary
       */
      virtual void Remove(const utf8_string_t &name) = 0;
      //@}
  };

}

#endif // Toolkits_DictionaryToolkitInterface_h_INCLUDED
