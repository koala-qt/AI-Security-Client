/** 
 * @file 
 *
 * @brief Header file containing defintions pertaining to the PlaybackSourceToolkit interface.
 */

#if !defined(Toolkits_PlaybackSourceToolkitInterface_h_INCLUDED)
#define Toolkits_PlaybackSourceToolkitInterface_h_INCLUDED

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
   * @brief Interface for browsing data through a source toolkit.
   *
   * To navigate though the data, the playback source toolkit
   * interface offers a cursor interface. The cursor is represented by
   * a time stamp and maps to the data having this as its begin time
   * stamp. You can move to the first position (see MoveFirst()), the
   * last position (see MoveLast()), the next position (see
   * MoveNext()), the previous position (see MovePrevious()) and to a
   * position given by a specific time stamp (see MoveTo()).
   *
   * The navigation can also be done on sequences by calling the
   * MovePreviousSequence() and MoveNextSequence() methods. A sequence
   * is a group of one or more data adjacent to each other by
   * time. Excatly how data are grouped into sequences is up to the
   * source toolkit implementation. In general however sequences
   * cannot overlap (the same data cannot be in two different
   * sequences) but also sequences need not cover all data (not all
   * data belongs to a sequence). When navigating by sequences, the
   * cursor position is always placed at the first data of a sequence.
   *
   * At a given cursor position, it is possible to get the associated
   * data. See GetData() for more details. One thing that is important
   * to note is that the interface is not atomic with regard to moving
   * and getting data. It is thus possible to move the cursor to a
   * position being told that everything went well and then when
   * calling GetData() a failure is reported that there is no data at
   * the current cursor position. This can happen if the data at the
   * current cursor position is deleted between the two calls.
   * 
   * Objects returned by GetData() (ie. obeying the NmToolkit::ImData
   * interface) represent records stored in or supplied by some kind 
   * of data source in chronological order. Any such record can be 
   * composed from a sequence of sub-records in which case the record
   * object may obey the NmToolkit::ImMediaDataVector interface or the
   * sub-records are implicitly defined by the binary content of the
   * record.
   *
   * Every record rec has a time span defined by its begin and end
   * times given by rec.GetBeginTimeStamp() and rec.GetEndTimeStamp().
   * The time span of different records of a specific source does not
   * overlap (not even at the end-points).
   *
   * Sub-records of a record are associated a begin time each. In case
   * the record rec obeys the NmToolkit::ImMediaDataVector interface the
   * begin time of the N'th sub-record is given by
   * (*rec.GetVector())[N].GetBeginTimeStamp().
   *
   * For every record rec rec.GetBeginTimeStamp() will match the begin
   * time of the first sub-record and rec.GetEndTimeStamp() will match
   * the begin time of the last sub-record.
   *
   * When searching for records in a data source using the
   * NmToolkit::ImPlaybackSourceToolkit::MoveTo(t, sc) method the
   * timeStamp argument t is matched against the begin and end time of
   * the records according to the search criteria sc. The search rules
   * are as follows:
   *
   * MoveTo(t, NEAREST) will move to the record having begin or end time
   * nearest to t. In case two records are equally close the latest
   * record will be moved to. Only if the datasource does not contain
   * any records will the move fail.
   *
   * MoveTo(t, BEFORE) will move to the latest record rec having
   * rec.GetEndTimeStamp() < t. If no such record exists the move fails.
   *
   * MoveTo(t, AT_OR_BEFORE) will move to the latest record rec having
   * rec.GetBeginTimeStamp() <= t. If no such record exists the move
   * fails.
   *
   * MoveTo(t, AFTER) will move to the earliest record rec having
   * rec.GetBeginTimeStamp() > t. If no such record exists the move fails.
   *
   * MoveTo(t, AT_OR_AFTER) will move to the earliest record rec having
   * rec.GetEndTimeStamp() >= t. If no such record exists the move fails.
   */
  class ImPlaybackSourceToolkit : public virtual ImSourceToolkit
  {
    public:
      /**
       * @brief Enumeration of possible move results.
       *
       * This enumeration is used by all move methods of the source
       * toolkit interface in order to tell the caller the status of
       * the operation.
       *
       * @see MoveFirst(), MoveLast(), MoveNext(), MoveNextSequence(), MovePrevious(), MovePreviousSequence(), MoveTo()
       */
      enum move_status_t
      {
        MOVED_OK,        ///< The move completed successfully.
        NOT_MOVED        ///< The move was not possible because there was no data at the requested position.
      };

      /**
       * @brief Enumeration of possible search criteria.
       *
       * This enumeration is used by the MoveTo() method to specify
       * exactly how to move to cursor to the specified time stamp.
       */
      enum search_criteria_t
      {
        NEAREST,       ///< Search for data nearest given time stamp.
        BEFORE,        ///< Search for data completely before given time stamp.
        AT_OR_BEFORE,  ///< Search for data containing or completely before given time stamp.
        AFTER,         ///< Search for data completely after given time stamp.
        AT_OR_AFTER    ///< Search for data containing or completely after given time stamp.
      };

      /**
       * @brief Enumeration of possible get data results.
       *
       * This enumeration is used by the GetData() method to tell the
       * caller the status of the operation.
       */
      enum get_data_status_t
      {
        DATA_RETRIEVED,    ///< The requested data was retrieved successfully.
        NO_DATA_AVAILABLE  ///< There is no data available at the current cursor position.
      };

    public:
      /** @name Navigation methods */ //@{
      /**
       * @brief Move to first data of source.
       *
       * Calling this method will move the cursor to the first data of
       * the source. If there is no data available, the cursor is not
       * moved and @ref NOT_MOVED is returned.
       *
       * Note that if Disconnect() is called (from another thread)
       * while waiting for the move to complete, an
       * ImNotConnectedError will be thrown.
       *
       * @return On success @ref MOVED_OK "MOVED_OK" is returned. See @ref move_status_t for other possible return values.
       *
       * @exception ImNotConnectedError Is thrown if the connection is lost or was never established.
       * @exception ImToolkitError Is thrown on failure to move to first data in source.
       *
       * @see MoveLast(), MoveNext(), MoveNextSequence(), MovePrevious(), MovePreviousSequence(), GetData()
       */
      virtual move_status_t MoveFirst()=0;

      /**
       * @brief Move to last data of source.
       *
       * Calling this method will move the cursor to the last data of
       * the source. If there is no data available, the cursor is not
       * moved and @ref NOT_MOVED is returned.
       *
       * Note that if Disconnect() is called (from another thread)
       * while waiting for the move to complete, an
       * ImNotConnectedError will be thrown.
       *
       * @return On success @ref MOVED_OK is returned. See @ref move_status_t for other possible return values.
       *
       * @exception ImNotConnectedError Is thrown if the connection is lost or was never established.
       * @exception ImToolkitError Is thrown on failure to move to last data in source.
       *
       * @see MoveFirst(), MoveNext(), MoveNextSequence(), MovePrevious(), MovePreviousSequence(), MoveTo(), GetData()
       */
      virtual move_status_t MoveLast()=0;

      /**
       * @brief Move to next data of source.
       *
       * Calling this method will move the cursor to the next data of
       * the source. If there is no data available or the cursor is
       * placed at the last data, the cursor is not moved and @ref
       * NOT_MOVED is returned.
       *
       * Note that if Disconnect() is called (from another thread)
       * while waiting for the move to complete, an
       * ImNotConnectedError will be thrown.
       *
       * @return On success @ref MOVED_OK is returned. See @ref move_status_t for other possible return values.
       *
       * @exception ImNotConnectedError Is thrown if the connection is lost or was never established.
       * @exception ImToolkitError Is thrown on failure to move to next data in source.
       *
       * @see MoveFirst(), MoveLast(), MoveNextSequence(), MovePrevious(), MovePreviousSequence(), MoveTo(), GetData()
       */
      virtual move_status_t MoveNext()=0;

      /**
       * @brief Move to previous data of source.
       *
       * Calling this method will move the cursor to the previous data
       * of the source. If there is no data available or the cursor is
       * placed at the first data, the cursor is not moved and @ref
       * NOT_MOVED is returned.
       *
       * Note that if Disconnect() is called (from another thread)
       * while waiting for the move to complete, an
       * ImNotConnectedError will be thrown.
       *
       * @return On success @ref MOVED_OK is returned. See @ref move_status_t for other possible return values.
       *
       * @exception ImNotConnectedError Is thrown if the connection is lost or was never established.
       * @exception ImToolkitError Is thrown on failure to move to previous data in source.
       *
       * @see MoveFirst(), MoveLast(), MoveNext(), MoveNextSequence(), MovePreviousSequence(), MoveTo(), GetData()
       */
      virtual move_status_t MovePrevious()=0;

      /**
       * @brief Move to first data of next sequence of source.
       *
       * Calling this method will move the cursor to the first data of
       * the next sequence relative to the current cursor position. If
       * there is no data available or there is no next sequence, the
       * cursor is not moved and @ref NOT_MOVED is returned.
       *
       * Note that if Disconnect() is called (from another thread)
       * while waiting for the move to complete, an
       * ImNotConnectedError will be thrown.
       *
       * @return On success @ref MOVED_OK is returned. See @ref move_status_t for other possible return values.
       *
       * @exception ImNotConnectedError Is thrown if the connection is lost or was never established.
       * @exception ImToolkitError Is thrown on failure to move to next sequence in source.
       *
       * @see MoveFirst(), MoveLast(), MoveNextSequence(), MovePrevious(), MovePreviousSequence(), MoveTo(), GetData()
       */
      virtual move_status_t MoveNextSequence()=0;

      /**
       * @brief Move to first data of previous sequence of source.
       *
       * Calling this method will move the cursor to the first data of
       * the previous sequence relative to the current cursor
       * position. If there is no data available or there is no
       * previous sequence, the cursor is not moved and @ref NOT_MOVED
       * is returned.
       *
       * Note that if Disconnect() is called (from another thread)
       * while waiting for the move to complete, an
       * ImNotConnectedError will be thrown.
       *
       * @return On success @ref MOVED_OK is returned. See @ref move_status_t for other possible return values.
       *
       * @exception ImNotConnectedError Is thrown if the connection is lost or was never established.
       * @exception ImToolkitError Is thrown on failure to move to previous sequence in source.
       *
       * @see MoveFirst(), MoveLast(), MoveNext(), MoveNextSequence(), MovePrevious(), MoveTo(), GetData()
       */
      virtual move_status_t MovePreviousSequence()=0;

      /**
       * @brief Move to data of specific time.
       *
       * Calling this method will move the cursor to the data of the
       * specified time stamp using the specified search criteria. If
       * there is no data available or no data could be found matching
       * the search criteria, the cursor is not moved and @ref
       * NOT_MOVED is returned.
       *
       * Note that if Disconnect() is called (from another thread)
       * while waiting for the move to complete, an
       * ImNotConnectedError will be thrown.
       *
       * @param timeStamp Time stamp of data to move to.
       * @param searchCriteria Search criteria to use when moving the cursor.
       * @return On success @ref MOVED_OK is returned. See @ref move_status_t for other possible return values.
       *
       * @exception ImNotConnectedError Is thrown if the connection is lost or was never established.
       * @exception ImToolkitError Is thrown on failure to move to specified time stamp in source.
       *
       * @see MoveFirst(), MoveLast(), MoveNext(), MoveNextSequence(), MovePrevious(), MovePreviousSequence(), GetData()
       */
      virtual move_status_t MoveTo(utc_time_t timeStamp, search_criteria_t searchCriteria)=0;
      //@}

      /** @name Data methods */ //@{
      /**
       * @brief Get data at current cursor position.
       *
       * If calling GetData() at a cursor position where there is no
       * data available, @ref NO_DATA_AVAILABLE is returned.
       *
       * If there is data available, @ref DATA_RETRIEVED is returned
       * and the instance of @p data is updated. 
       *
       * Also note that if Disconnect() is called (from another
       * thread) while waiting for data to be retrieved, an
       * ImNotConnectedError will be thrown.
       *
       * After a successful call to GetData() the @p data will point
       * to a new object. Hence, a possible previous object assigned
       * to @p data will no longer be referenceable.
       *
       * The object assigned to @p data after a successful call to
       * GetData() must be deleted by the user. Otherwise it will
       * result in a memory leak.
       *
       * @param data Reference to pointer to update with retrieved data (this is an out parameter only and it is only updated if @ref DATA_RETRIEVED is returned).
       * @return On success @ref DATA_RETRIEVED is returned. See @ref get_data_status_t for other possible return values.
       *
       * @exception ImNotConnectedError Is thrown if the connection is lost or was never established.
       * @exception ImToolkitError Is thrown on failure to get the data.
       *
       * @see MoveFirst(), MoveLast(), MoveNext(), MoveNextSequence(), MovePrevious(), MovePreviousSequence(), MoveTo()
       *
       */
      virtual get_data_status_t GetData(ImData *&data)=0;
      //@}
  };

}

#ifdef _WIN32
#pragma vtordisp(pop)
#endif

#endif // Toolkits_PlaybackSourceToolkitInterface_h_INCLUDED
