/**
 * @file
 *
 * @brief Header file containing definition of CmToolkitFactoryProvider class.
 */

#ifndef Toolkits_ToolkitFactoryProvider_h_INCLUDED
#define Toolkits_ToolkitFactoryProvider_h_INCLUDED

#include <list>
#include <iostream>
#include <algorithm>
#include <Toolkits/ToolkitInterface.h>


/* === Function declarations =============================================== */

extern "C"
{
  NmToolkit::ImToolkitFactory *CreateInstance();
  void DeleteInstance(NmToolkit::ImToolkitFactory *toolkitFactory);
}


/* === Namespace =========================================================== */

namespace NmToolkit
{

  /* --- CmToolkitFactoryProvider interface -------------------------------- */

  /**
   * @brief Toolkit factory provider.
   *
   * This toolkit factory provider can be used for creating instances
   * of CmToolkitFactory through a dynamic link library or a shared
   * object file. This allows one to distribute only the library and
   * this header file (and the toolkits interface header file) to
   * third party in order for them to use our toolkits. Even further,
   * the class is carefully designed to allow mixing different
   * versions of the run-time including debug and release
   * configurations.
   */
  class CmToolkitFactoryProvider
  {
    private:
      std::list<ImToolkitFactory *> m_CreatedInstances;  ///< List of created instances.

    public:
      /** @name Constructors and destructors */ //@{
      CmToolkitFactoryProvider();           ///< Initialize toolkit factory provider.
      virtual ~CmToolkitFactoryProvider();  ///< Deinitialize toolkit factory provider.
      //@}

      /** @name Provider methods */ //@{
      virtual ImToolkitFactory *CreateInstance();                     ///< Create new instance of toolkit factory.
      virtual void DeleteInstance(ImToolkitFactory *toolkitFactory);  ///< Delete toolkit factory instance.
      //@}
  };


  /* --- CmToolkitFactoryProvider implementation --------------------------- */

  /* --- Constructors and destructors -------------------------------------- */

  /**
   */
  inline CmToolkitFactoryProvider::CmToolkitFactoryProvider()
  {
    // Initialize toolkit factory provider
  }

  /**
   * This destructor will make sure that all toolkit factories not
   * deleted using the DeleteInstance() method will be deleted before
   * the provider is destroyed.
   */
  inline CmToolkitFactoryProvider::~CmToolkitFactoryProvider()
  {
    // Deinitialize tookit factory provider
    while (!m_CreatedInstances.empty())
      DeleteInstance(m_CreatedInstances.front());
  }


  /* ---Provider methods --------------------------------------------------- */

  /**
   * Use this method to create a toolkit factory. When done, you can
   * delete it again by calling DeleteInstance(). If not all factories
   * are deleted when the provider is destroyed, these will
   * automatically be deleted first.
   *
   * @return Created toolkit factory or null in case of an error.
   *
   * @see DeleteInstance()
   */
  inline ImToolkitFactory *CmToolkitFactoryProvider::CreateInstance()
  {
    // Create toolkit factory instance and return it
    m_CreatedInstances.push_back(::CreateInstance());
    return m_CreatedInstances.back();
  }

  /**
   * Use this method for deleting toolkit factories created with the
   * CreateInstance() method. If not all factories are deleted when
   * the provider is destroyed, these will automatically be deleted
   * first.
   *
   * @param toolkitFactory Toolkit factory instance to delete.
   *
   * @see CreateInstance()
   */
  inline void CmToolkitFactoryProvider::DeleteInstance(ImToolkitFactory *toolkitFactory)
  {
    std::list<ImToolkitFactory *>::iterator itr;

    // Delete toolkit factory
    itr = find(m_CreatedInstances.begin(), m_CreatedInstances.end(), toolkitFactory);
    if (itr != m_CreatedInstances.end())
    {
      ::DeleteInstance(toolkitFactory);
      m_CreatedInstances.erase(itr);
    }
  }

}

#endif
