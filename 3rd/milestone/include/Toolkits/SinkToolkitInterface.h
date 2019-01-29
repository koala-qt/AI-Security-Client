/**
 * @file
 *
 * @brief Header file containing definitions of sink toolkit interfaces.
 */

#if !defined(Toolkits_SinkToolkitInterface_h_INCLUDED)
#define Toolkits_SinkToolkitInterface_h_INCLUDED

// Local headers
#include "Types.h"
#include "BaseToolkitInterface.h"

/* === Namespace =========================================================== */

namespace NmToolkit
{

  /* === Sink toolkit interface ================================================= */


  /* === Sink toolkit interface ========================================== */

  /**
   * @brief Sink toolkit interface.
   *
   * This is the interface which all sink toolkits must implement.
   *
   * Before a sink toolkit can be used it must be connected to the
   * sink. This is done by calling the Connect() method.
   *
   */
  class ImSinkToolkit : public ImToolkit
  { 
    public:
      /** @name Connect and disconnect methods */ //@{
      /**
       * @brief Connect to Sink to put data.
       *
       * A connection is needed in order to put data to a
       * sink. The sink to connect to is determined at the time of
       * construction (by the toolkit factory) and cannot be changed
       * unless the specific sink toolkit allows this through on of
       * the UpdateConfiguration() methods.
       *
       * It is not an error to call Connect() on an already connected
       * sink toolkit instance. In this case nothing happens.
       *
       * A waiting connect call can be cancelled by the user from
       * another thread by calling Disconnect(). If this happens an
       * ImNotConnectedError is thrown.
       *
       * @param timeout The time-out value for the connect to complete, in milliseconds. Specifying -1 indicates an infinite time-out period. The default value is -1.
       * 
       * @exception ImBreakError Is thrown if the call was interrupted due to a call to Break().
       * @exception ImAuthenticationError Is thrown on failure to connect due to an authentication issue.
       * @exception ImNotConnectedError Is thrown if connect was cancelled by a user disconnect or the connect timed out.
       * @exception ImToolkitError Is thrown on failure to connect to Sink.
       *
       * @see Disconnect(), UpdateConfiguration()
       */
      virtual void Connect(time_span_t timeout = -1) = 0;

      /**
       * @brief Disconnect from sink.
       *
       * This method will disconnect from the sink and clean up all
       * resoruces taken. After calling Disconnect(), the connection
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
       * Consult the documentation of the specific sink toolkit in
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

      /** @name Data methods */ //@{
      /**
       * @brief Put data to sink
       *
       * Writes data to the data sink. If the data sink support time ordered data, the data is inserted at position data.GetBeginTimeStamp().
       * A data sink may allow insertion only after the most recent stored data timestamp. If data cannot be inserted at data.GetBeginTimeStamp()
       * PutData should (but is not required to) throw an appropiate exception.
       *
       * @param data Data to be written to sink
       *
       * @exception ImNotConnectedError Is thrown if the connection is lost or was never established.
       * @exception ImToolkitError Is thrown on failure to store the data.
       *
       */
      virtual void PutData(ImData &data)=0;
      //@}
  };

}

#endif // Toolkits_SinkToolkitInterface_h_INCLUDED
