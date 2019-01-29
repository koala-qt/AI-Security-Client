/**
 * @file
 *
 * @brief Header file containing definitions of export toolkit interfaces.
 */

#if !defined(Toolkits_ExportToolkitInterface_h_INCLUDED)
#define Toolkits_ExportToolkitInterface_h_INCLUDED

// Local headers
#include "Types.h"
#include "DataInterface.h"
#include "BaseToolkitInterface.h"

// System headers
#include <list>
#include <utility>


/* === Namespace =========================================================== */

namespace NmToolkit
{
  
  /* --- ImExportToolkit class --------------------------------------------- */

  /**
   * @brief Export toolkit interface.
   *
   * This is the interface which all export toolkits must implement.
   */
  class ImExportToolkit : public ImToolkit
  {
    public:
      /**
       * @brief Enumeration of possible export results.
       */
      enum export_result_t
      {
        EXPORT_RESULT_DONE,      ///< The export completed successfully and produced a non-empty result.
        EXPORT_RESULT_EMPTY,     ///< The export completed successfully and produced an empty result.
        EXPORT_RESULT_CANCELLED  ///< The export was cancelled by the user.
      };
  
  public:
      /** @name Export methods */ //@{
      /**
       * @brief Start the export.
       *
       * The method will block until the export is done. You can call
       * CancelExport() from another thread to cancel the export. Also
       * you can call GetPercentageCompleted() to get progress
       * information.
       *
       * The object assigned to @p exportData after a successful call
       * to Export() must be deleted by the user. Otherwise it will
       * result in a memory leak.
       *
       * @param exportData Pointer to data instance containing relevant information about the course of the export.
       * @param sourceToolkitConfigurationXml Optional pointer to a source toolkit configuration XML to read the exported data. The default value is NULL.
       * @return Status of export. See @ref export_result_t for possible values.
       *
       * @exception ImToolkitError Is thrown if a failure occurs during the export process.
       *
       * @see GetPercentageCompleted(), CancelExport()
       */
      virtual export_result_t Export(ImData *&exportData, utf8_string_t *sourceToolkitConfigurationXml = NULL) = 0;

      /**
       * @brief Cancels an export operation in progress.
       *
       * @param keepExportedData If true the export completed prior to the operation being cancelled will be kept, otherwise it will be deleted. The default value is false.
       *
       * @exception ImInvalidOperationError Is thrown if the export was not yet started by Export().
       *
       * @see Export()
       */
      virtual void CancelExport(bool keepExportedData = false) = 0;
      //@}

      /** @name Estimation methods */ //@{
      /**
       * @brief Runs a blocking export size analysis. Call CancelEstimation() from another thread to cancel the estimation.
       *
       * @param estimatedSize Reference to a file size variable that will contain the estimated export size.
       * @return EXPORT_RESULT_DONE if all requested sources were estimated successfully, EXPORT_RESULT_CANCELLED if the estimation was cancelled before it completed.
       *
       * @exception ImToolkitError Is thrown if a failure occurs during the estimation process.
       *
       * @see GetPercentageCompleted(), CancelEstimation()
       */
      virtual export_result_t EstimateSizeOfExport(file_size_t &estimatedSize) = 0;

      /**
       * @brief Cancels an export size analysis operation in progress.
       *
       * @exception ImInvalidOperationError Is thrown if the estimation was not yet started by EstimateSizeOfExport().
       *
       * @see EstimateSizeOfExport()
       */
      virtual void CancelEstimation() = 0;
      //@}

      /** @name Query methods */ //@{
      /**
       * @brief This method returns the percentage completed of the Export() and the EstimateSizeOfExport() methods, respectively.
       *
       * This method should be called from another thread that the one that runs Export() or EstimateSizeOfExport().
       *
       * @param itemCurrentlyProcessed Optional pointer to a string that will contain the item currently processed. The default value is NULL.
       * @return A percentage (integer from 0 to 100) of how far the operation is from completion.
       *
       * @see Export(), EstimateSizeOfExport()
       */
      virtual std::size_t GetPercentageCompleted(utf8_string_t *itemCurrentlyProcessed = NULL) const = 0;
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
       * Consult the documentation of the specific export toolkit in
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
       * except it can update a list of parameters and apply these in
       * one atomic call.
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

#endif // Toolkits_ExportToolkitInterface_h_INCLUDED
