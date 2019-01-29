/**
 * @file
 *
 * @brief Header file containing definitions of base toolkit interfaces.
 */

#if !defined(Toolkits_BaseToolkitInterface_h_INCLUDED)
#define Toolkits_BaseToolkitInterface_h_INCLUDED


/* === Namespace =========================================================== */

namespace NmToolkit
{

  /* === Toolkit interface ================================================= */

  /* --- ImToolkit class --------------------------------------------------- */

  /**
   * @brief Toolkit interface.
   *
   * This is the interface which all toolkits must implement.
   *
   * Notice that toolkit interfaces are in general not thread safe. For a specific
   * toolkit instance at most one invocation of a method is allowed at any given time (@ref Break() is
   * an exception to the rule - it may be invoked from any thread at any time).
   *
   */
  class ImToolkit
  {
    public:
      /** @name Constructors and destructors */ //@{
      /**
       * @brief Virtual toolkit destructor to ensure proper destruction through interface.
       */
      virtual ~ImToolkit() throw() {}
      //@}

      /** @name Interruption */ //@{
      /**
       * @brief Cause a breakable method to terminate and throw an ImBreakError exception. May be called at any time from any thread.
       * If no breakable method is currently envoked the next call to a breakable method will fail and throw an ImBreakError exception.
       * When a breakable method throws an ImBreakError exception the break condition is cleared and subsequent calls to breakable methods
       * will not throw an ImBreakError exception unless of course Break is called again.
       * Breakable methods are those methods for which the documentation explicitly states that an ImBreakError may be thrown.
       */
      virtual void Break() throw() = 0;
      //@}
  };

}

#endif // Toolkits_BaseToolkitInterface_h_INCLUDED
