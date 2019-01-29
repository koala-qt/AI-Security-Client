/**
 * @file
 *
 * @brief Header file containing definitions pertaining to the LiveSourceToolkit interface.
 */

#if !defined(Toolkits_LiveSourceToolkitInterface_h_INCLUDED)
#define Toolkits_LiveSourceToolkitInterface_h_INCLUDED

// Local headers
#include "Types.h"
#include "SourceToolkitInterface.h"
#include "DataInterface.h"


/* === Namespace =========================================================== */

#ifdef _WIN32
#pragma vtordisp(push, 2)
#endif

namespace NmToolkit
{
  /**
   * @brief Interface for streaming live data through a source toolkit.
   *
   * The live source toolkit interface contains methods to start, stop
   * and poll a live stream of data.
   *
   * Before the method @ref GetLiveData() can be called, the live
   * stream must be started. This means that the method @ref
   * StartLiveStream() has been called and returned successfully, and
   * no subsequent calls have been made to the method @ref
   * StopLiveStream().
   *
   * A waiting @ref GetLiveData() call can be cancelled by 
   * another thread calling @ref Break(). If this happens an
   * ImBreakError is thrown.
   */
  class ImLiveSourceToolkit : public virtual ImSourceToolkit
  {
    public:
      /**
       * @brief Live data retrieval status type.
       *
       * This type is returned by @ref GetLiveData().
       */
      enum get_live_status_t
      {
        LIVE_DATA_RETRIEVED,       ///< Live data was successfully retrieved.
        LIVE_REQUEST_TIMED_OUT     ///< Live data request timed out.
      };

      /** @name Stream control methods */ //@{
      /**
       * @brief Starts the live stream.
       *
       * This method starts the underlying live stream feeding the <em>live source
       * toolkit</em> with data. This method must be called successfully before the
       * first call to @ref GetLiveData().
       *
       * @exception ImToolkitError Is thrown on failure to start live stream.
       *
       * @see StopLiveStream(), GetLiveData()
       */
      virtual void StartLiveStream() = 0;

      /**
       * @brief Stops the live stream.
       * 
       * This method stops the live stream and releases any underlying
       * ressources which may have been allocated.
       *
       * @see StartLiveStream()
       */
      virtual void StopLiveStream() = 0;
      //@}

      /** @name Get data methods */ //@{
      /**
       * @brief Get next data from live stream.
       *
       * Before calling this method the underlying live stream must be
       * ready. That is, @ref StartLiveStream() has been successfully
       * called and no intermediate call to @ref StopLiveStream() has
       * been performed.
       *
       * The underlying live source toolkit keeps a queue of live data records. 
       * During execution of GetData() The oldest one in this queue is removed 
       * and returned in @p data. Assuming the queue
       * is not empty this method should return instantaneously. If the
       * queue is empty the method will wait for at most @p timeout milliseconds
       * for a new data record to arrive. The size of the live queue can be fixed
       * or alternatively a parameter specified in the configuration xml.
       *
       * After a successful call to GetLiveData() the @p data will
       * point to a new object. Hence, a possible previous object
       * assigned to @p data will no longer be referenceable.
       *
       * The object assigned to @p data after a successful call to
       * GetLiveData() must be deleted by the user. Otherwise it will
       * result in a memory leak.
       *
       * @param data Reference to pointer to update with retrieved data (this is an out parameter only).
       * @param timeout The time-out value for the operation to complete, in milliseconds. Specifying -1 indicates an infinite time-out period, specifying 0 will poll and return immediately. The default value is -1.
       * @return @ref LIVE_DATA_RETRIEVED if data was successfully retrieved. If the specified time-out is reached @ref LIVE_REQUEST_TIMED_OUT is returned.
       *
       * @exception ImBreakError Is thrown if the call was interrupted due to a call to Break().
       * @exception ImNotConnectedError Is thrown if Disconnect() is called while waiting for live data to get in.
       * @exception ImInvalidOperationError Is thrown if the live stream is not started.
       * @exception ImToolkitError Is thrown on failure to retrieve data due to an internal error.
       */
      virtual get_live_status_t GetLiveData(ImData *&data, time_span_t timeout = -1) = 0;
      //@}
  };
}

#ifdef _WIN32
#pragma vtordisp(pop)
#endif

#endif // Toolkits_LiveSourceToolkitInterface_h_INCLUDED
