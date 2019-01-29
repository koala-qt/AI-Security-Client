/**
 * @file
 *
 * @brief Header file containing definitions of toolkit factory interfaces.
 */

#if !defined(Toolkits_ToolkitFactoryInterface_h_INCLUDED)
#define Toolkits_ToolkitFactoryInterface_h_INCLUDED

// Local headers
#include "Types.h"
#include "BaseToolkitInterface.h"


/* === Namespace =========================================================== */

/**
 * @brief Namespace of all toolkit constructs.
 */
namespace NmToolkit
{

  /* === Toolkit factory interface ========================================= */

  /**
   * @brief Toolkit factory interface.
   *
   * This is the interface which all toolkit factories must
   * implement. With a toolkit factory you will be able to create
   * instances of toolkits.
   */
  class ImToolkitFactory
  {
    public:
      /** @name Constructors and destructors */ //@{
      /**
       * @brief Virtual toolkit factory destructor to ensure proper destruction through interface.
       *
       * This destructor will make sure that all toolkits which have
       * been created using the CreateInstance() method will be
       * deleted if not already done so.
       *
       * @see CreateInstance(), DeleteInstance()
       */
      virtual ~ImToolkitFactory() throw() {}
      //@}

      /** @name Factory methods */ //@{
      /**
       * @brief Create toolkit instance.
       *
       * The returned toolkit instance must be deleted using the
       * DeleteInstance() method. If you don't call DeleteInstance(),
       * the destructor of the toolkit factory will do this for you.
       *
       * The @p configurationXml parameter specifies values necessary
       * for properly configuring the toolkit. The mandatory tags are:
       *
       * @code
       *      <?xml version='1.0' encoding="utf-8"?>
       *      <toolkit type="toolkit_type">
       *        <provider>toolkit_provider</provider>
       *          .
       *          .
       *          <!-- toolkit specific tags -->
       *          .
       *          .
       *      </toolkit>
       * @endcode
       *
       * Consult the documentation for the @ref
       * available_source_toolkits "available source toolkits" and the
       * @ref available_export_toolkits "available export toolkits"
       * for details on the configuration XML structure.
       *
       * @param configurationXml Configuration XML of toolkit.
       * @return New toolkit instance.
       *
       * @exception ImCreatingToolkitError Is thrown on failure to create toolkit instance.
       *
       * @see DeleteInstance()
       */
      virtual ImToolkit *CreateInstance(const utf8_string_t &configurationXml)=0;

      /**
       * @brief Delete toolkit instance.
       *
       * The specified toolkit instance must have been created using
       * the CreateInstance() method. If this method is not called,
       * the toolkit factory destructor will make sure to do this for
       * you.
       *
       * @param toolkit Pointer to toolkit instance to delete.
       *
       * @see CreateInstance()
       */
      virtual void DeleteInstance(ImToolkit *toolkit)=0;
      //@}
  };

}

#endif // Toolkits_ToolkitFactoryInterface_h_INCLUDED
