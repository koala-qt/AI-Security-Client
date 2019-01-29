/**
 * @file
 *
 * @brief Header file containing definitions of source toolkit interfaces.
 */

#if !defined(Toolkits_SourceToolkitInterface_h_INCLUDED)
#define Toolkits_SourceToolkitInterface_h_INCLUDED

// Local headers
#include "Types.h"
#include "BaseToolkitInterface.h"

// System headers
#include <list>
#include <set>
#include <utility>


/* === Namespace =========================================================== */

namespace NmToolkit
{

  /* === Source Toolkit interface ========================================== */

  /**
   * @brief Source toolkit interface.
   *
   * This is the interface which all source toolkits must implement.
   *
   * Before a source toolkit can be used it must be connected to the
   * source. This is done by calling one of the Connect() methods. To
   * actually get data from the souce toolkit, you must downcast to
   * either the ImPlaybackSourceToolkit or the ImLiveSourceToolkit
   * interface. Some toolkits support both whereas others only support
   * one of these.
   *
   */
  class ImSourceToolkit : public ImToolkit
  {
    public:
      /** @name Connect and disconnect methods */ //@{
      /**
       * @brief Connect to source from which to get data.
       *
       * A connection is needed in order to get data from a
       * source. The source to connect to is determined at the time of
       * construction (by the toolkit factory) and cannot be changed
       * unless the specific source toolkit allows this through on of
       * the UpdateConfiguration() methods.
       *
       * It is not an error to call Connect() on an already connected
       * source toolkit instance. In this case nothing happens.
       *
       * A waiting @ref Connect() call can be cancelled by the user from
       * another thread by calling Break(). If this happens an
       * ImBreakError is thrown.
       *
       * @param timeout The time-out value for the connect to complete, in milliseconds. Specifying -1 indicates an infinite time-out period. The default value is -1.
       *
       * @exception ImBreakError Is thrown if the call was interrupted due to a call to Break().
       * @exception ImAuthorizationError Is thrown on failure to connect due to an authentication issue.
       * @exception ImNotConnectedError Is thrown if connect was cancelled by a user disconnect or the connect timed out.
       * @exception ImToolkitError Is thrown on failure to connect to source.
       *
       * @see Disconnect(), UpdateConfiguration()
       */
      virtual void Connect(time_span_t timeout = -1) = 0;

      /**
       * @brief Connect to source from which to get data and specify a list of properties which should be available on the data.
       *
       * This method is identical to the other connect method (see
       * Connect()) except that it allows for requesting a set of
       * properties to be available on the data that is retrieved
       * through the source toolkit (see ImData::GetProperty()).
       * Different source toolkits support different properties and
       * the documentation of the specific source toolkit must be
       * consulted to learn which can be used.
       *
       * When the method returns, it will have populated the @p
       * availablePropertyNames set with a subset of the @p
       * requestedPropertyNames set. Those properties not in the
       * subset will not be available on the data retrieved through
       * the source toolkit. Note that for some properties, a
       * performance degradation can occur if they are requested. In
       * general it is thus advised to only request those properties
       * that are going to be used.
       *
       * A waiting connect call can be cancelled from
       * another thread by calling Break(). If this happens an
       * ImBreakError is thrown.
       *
       * @param requestedPropertyNames Set of property names to request.
       * @param availablePropertyNames Subset of requested properties names that are actually available.
       * @param timeout The time-out value for the connect to complete, in milliseconds. Specifying -1 indicates an infinite time-out period. The default value is -1.
       *
       * @exception ImBreakError Is thrown if the call was interrupted due to a call to Break().
       * @exception ImAuthenticationError Is thrown on failure to connect due to an authentication issue.
       * @exception ImNotConnectedError Is thrown if connect was cancelled by a user disconnect or the connect timed out.
       * @exception ImToolkitError Is thrown on failure to connect to source.
       *
       * @see Connect(), Disconnect(), ImData::GetProperty()
       */
      virtual void Connect(const std::set<utf8_string_t> &requestedPropertyNames, std::set<utf8_string_t> &availablePropertyNames, time_span_t timeout = -1) = 0;

      /**
       * @brief Disconnect from source.
       *
       * This method will disconnect from the source and clean up all
       * resources taken. After calling Disconnect(), the connection
       * can be re-established by calling Connect().
       *
       * @see Connect()
       */
      virtual void Disconnect() = 0;
      //@}

      /** @name Configuration methods */ //@{
      /** 
       * @brief Update a single configuration parameter.
       *
       * Updates the parameter with name @p configurationParameterName
       * to the value of @p configurationParameterValue. If the given
       * parameter name is not applicable for this specific toolkit or
       * if the parameter was not configured to be updateable, the
       * method does nothing.
       *
       * If the configuration parameter value specified cannot be
       * parsed to the type matching what is expceted by the parameter
       * to update, then an exception is thrown.
       *
       * Consult the documentation of the specific source toolkit in
       * use to learn which parameters can be used, when they can be
       * used and what their legal values are.
       *
       * @param configurationParameterName Name of configuration parameter to update.
       * @param configurationParameterValue Value of configuration parameter to apply as a string.
       *
       * @exception ImInvalidArgumentError Is thrown if @p configurationParameterValue could not be parsed to the correct type.
       *
       * @see UpdateConfiguration(const std::list<std::pair<utf8_string_t, utf8_string_t> > &configurationParameterList)
       */
      virtual void UpdateConfiguration(const utf8_string_t &configurationParameterName, const utf8_string_t &configurationParameterValue) = 0;
      
      /**
       * @brief Update a list of configuration parameters.
       *
       * This method works like the other update configuration method
       * except it can update a list of parameters in one atomic call.
       *
       * @param configurationParameterList List of parameter name and value pairs representing the parameters to be updated.
       *
       * @exception ImInvalidArgumentError Is thrown if one of the specified configuration parameter values could not be parsed to the correct type.
       *
       * @see UpdateConfiguration(const utf8_string_t &configurationParameterName, const utf8_string_t &configurationParameterValue)
       */
      virtual void UpdateConfiguration(const std::list<std::pair<utf8_string_t, utf8_string_t> > &configurationParameterList) = 0;
      //@}
  };

}

#endif // Toolkits_SourceToolkitInterface_h_INCLUDED
