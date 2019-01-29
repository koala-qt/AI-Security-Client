/**
 * @file
 *
 * @brief Header file containing definitions pertaining to the SearchSourceToolkit interface.
 */

#if !defined(Toolkits_SearchSourceToolkitInterface_h_INCLUDED)
#define Toolkits_SearchSourceToolkitInterface_h_INCLUDED

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
   * @brief Interface for searching in data.
   *
   * The search toolkit interface contains methods to start and stop the
   * search and a method for getting the search results.
   *
   * Before the method @ref GetSearchResult() can be called, the search
   * must be started by calling the @ref StartSearch() method. When
   * the last search result is gotten, the @ref StopSearch() method must
   * be called to free the used resources.
   *
   * A blocking call to @ref GetSearchResult() call, can be cancelled by 
   * another thread, by calling @ref Break(). If this happens an
   * ImBreakError is thrown.
   */
  class ImSearchSourceToolkit : public virtual ImSourceToolkit
  {
    public:
      /** @name Search control methods */ //@{
      /**
       * @brief Starts the search.
       *
       * This method will start the search and in the background results will begin
       * to queue up. To get the results, the @ref GetSearchResult() method must be
       * called.
       *
       * The search will be limited to look for results between the given begin and
       * end time stamp.
       *
       * @param begin Begin time of search.
       * @param end End time of search.
       * @param maxResults The maximum number of search results that is needed. Use 0 to indicate no limit.
       * @exception ImToolkitError Is thrown on failure to start search.
       *
       * @see StopSearch(), GetSearchResult()
       */
      virtual void StartSearch(utc_time_t begin, utc_time_t end, unsigned int maxResults) = 0;

      /**
       * @brief Stops the search.
       * 
       * This method will stop the currently running search and releases any
       * underlying ressources which may have been allocated. The method can
       * be called before the search is done if no more search results are needed.
       *
       * @see StartSearch()
       */
      virtual void StopSearch() = 0;
      //@}

      /** @name Get search results methods */ //@{
      /**
       * @brief Get next search result.
       *
       * Before calling this method, the the search must have been started
       * by calling @ref StartSearch(). After calling @ref StopSearch(),
       * it is an error to call this method.
       *
       * The underlying search toolkit keeps a queue of search results. During
       * execution of GetSearchResult() the oldest one in this queue is removed
       * and returned in @p searchResult. Assuming the queue is not empty, the
       * method will return instantly with a new result. If the queue is empty,
       * the method will wait for a next search result to become available. You
       * can at any time call @ref Break() to make this method return with an
       * @ref ImBreakError exception. If there are no more search results available
       * the method will return false.
       *
       * After a successful call to GetSearchResult() the @p searchResult will
       * point to a new object. Hence, a possible previous object
       * assigned to @p searchResult will no longer be referenceable.
       *
       * The object assigned to @p searchResult after a successful call to
       * GetSearchResult() must be deleted by the user. Otherwise it will
       * result in a memory leak.
       *
       * In case of a timeout, the method will return true but set the
       * @p searchResult pointer to null.
       *
       * @param searchResult Reference to pointer to update with next search result (this is an out parameter only). Will be set to null in case of a timeout.
       * @param timeout The timeout value for the operation to complete, in milliseconds. Specifying -1 indicates an infinite timeout and is the default.
       * @return False if there are no more search results to get within the given query. Will be true in case of a timeout.
       *
       * @exception ImBreakError Is thrown if the call was interrupted due to a call to Break().
       * @exception ImInvalidOperationError Is thrown if the search has not been started.
       * @exception ImToolkitError Is thrown on failure to get next search result due to other reasons.
       */
      virtual bool GetSearchResult(ImData *&searchResult, time_span_t timeout = -1) = 0;
      //@}
  };
}

#ifdef _WIN32
#pragma vtordisp(pop)
#endif

#endif // Toolkits_SearchSourceToolkitInterface_h_INCLUDED
