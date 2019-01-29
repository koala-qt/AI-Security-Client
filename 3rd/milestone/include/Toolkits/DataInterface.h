/**
 * @file
 *
 * @brief Header file containing definitions of toolkit data interfaces.
 */

#if !defined(Toolkits_DataInterface_h_INCLUDED)
#define Toolkits_DataInterface_h_INCLUDED

// Standard headers
#include <vector>

// Local headers
#include "Types.h"
#include "PropertyInterface.h"

/* === Namespace =========================================================== */

namespace NmToolkit
{

  /* === Toolkit data interface ============================================ */

  /* --- ImData class ------------------------------------------------------ */

  /**
   * @brief Base data interface of all toolkit data.
   *
   * All data handled by toolkits must implement this interface.
   *
   * Associated with an ImData instance, is a begin and end time
   * stamp. These can be equal, but the end time stamp cannot be
   * before the begin time stamp. See GetBeginTimeStamp() and
   * GetEndTimeStamp() methods for more details.
   *
   * The actual data can be accessed using the GetDataSize() and
   * GetDataPointer() methods.
   *
   * An instance of ImData must be deallocated using the delete
   * operator.
   * A "copy" of an instance of ImData of the proper type can be 
   * generated via the Clone() method.
   */
  class ImData
  {
    public:
      /** @name Constructors and destructors */ //@{
      /**
       * @brief Virtual data destructor to ensure proper destruction through interface.
       */
      virtual ~ImData() throw() {}
      //@}

      /** @name Time stamp methods */ //@{
      /**
       * @brief Get begin time stamp of data.
       *
       * @return Begin time stamp of data.
       *
       * @see GetEndTimeStamp()
       */
      virtual utc_time_t GetBeginTimeStamp() const=0;

      /**
       * @brief Get end time stamp of data.
       *
       * @return End time stamp of data.
       *
       * @see GetBeginTimeStamp()
       */
      virtual utc_time_t GetEndTimeStamp() const=0;
      //@}

      /** @name Data methods */ //@{
      /**
       * @brief Get number of overall bytes in data.
       *
       * @return Number of overall bytes in data.
       *
       * @see GetDataPointer()
       */
      virtual std::size_t GetDataSize() const=0;

      /**
       * @brief Get pointer to data.
       *
       * @return Pointer to data.
       *
       * @see GetDataSize()
       */
      virtual const byte_t *GetDataPointer() const=0;
      //@}

      /** @name Property methods */ //@{
      /**
       * @brief Get property of data.
       *
       * The properties available on data is dependent on how the data
       * was retrieved. For data that is retrieved through a source
       * toolkit, the available properties is requested when
       * connecting to the source. Calling this method for a property
       * that is not available will cause the method to return
       * null. Note that even though a property was marked as
       * available after calling Connect, null might still be returned
       * for some data instances.
       *
       * The ImData object manages the property objects internally.
       * Hence, the caller should not delete the returned object.
       *
       * @param propertyName Name identifying the property to get.
       * @return Pointer to property of specified name. Is null if property is not available.
       *
       * @see ImSourceToolkit::Connect(const std::set<utf8_string_t> &requestedPropertyNames, std::set<utf8_string_t> &availablePropertyNames)
       */
      virtual const ImProperty *GetProperty(const utf8_string_t &propertyName) const=0;
      //@}

      /** @name Cloning methods */ //@{
      /**
       * @brief Get a proper clone of the current data instance.
       *
       * There is no guarantee that the actual data, properties etc. contained 
       * and handled by an instance of ImData are really copied or if the contents 
       * is instead just referenced via standard reference-counting semantics. 
       * Rather, the latter is more likely to occur for performance reasons.
       * Nevertheless, it should not be necessary to rely on either, and as it 
       * will be completely transparent, it can be thought of as ordinary full
       * copies. Clones may be deallocated in any order and independently of the
       * originating data instance and other clones. There is no real distinction
       * between clones and their originating data or other clones (e.g. clones of
       * clones) as they are all equal in contents and relation.
       *
       * @return Pointer to ordinarily heap-allocated clone ("copy") of the
       *  present ImData instance of the right leaf type. Must be deallocated 
       *  by caller as any other retrieved ImData instance.
       */
      virtual ImData *Clone() const=0;
      //@}
  };


  /* --- ImMediaData class ------------------------------------------------- */

  /**
   * @brief Media data interface.
   *
   * All media data handled by toolkits must implement this interface.
   */
  class ImMediaData : public ImData
  {
    public:
      /** @name Media body methods */ //@{
      /**
       * @brief Get number of bytes in media body, i.e. excluding the header.
       *
       * @return Number of bytes in media body.
       *
       * @see GetMediaBodyPointer()
       */
      virtual std::size_t GetMediaBodySize() const=0;

      /**
       * @brief Get pointer to media body data, i.e. excluding the header.
       *
       * @return Pointer to media body data.
       *
       * @see GetMediaBodySize()
       */
      virtual const byte_t *GetMediaBodyPointer() const=0;
      //@}
  };


  /**
   * @brief Media data vector interface.
   *
   * All media data handled by toolkits must implement this interface.
   * 
   * This instance can return data as one record (e.g. a GOP in case
   * of video) using the ImMediaData interface or as a vector of sub
   * records (e.g. the individual frames in a GOP) using the
   * GetVector() method.
   */
  class ImMediaDataVector : public ImMediaData
  {
    public:
      // @name Data methods @{
      /**
       * @brief Get media data as a vector of ImMediaData sub records.
       *
       * @return ImMediaData vector instance.
       */
      virtual std::vector<const ImMediaData*> GetVector() const=0;
      //@}
  };

  
  /* --- ImImage class ----------------------------------------------------- */

  /**
   * @brief Image data interface.
   *
   * All images (in decoded form) handled by toolkits must implement
   * this interface.
   */
  class ImImage : public ImMediaData
  {
    public:
      /**
       * @brief Enummeration of the color format and layout of decoded images.
       */
      enum image_format_t
      {
        Gray,             ///< or luminance-only (Y' only)
        RGB24,            ///< R', G' and B' components: RGBRGBRGBRGB
        BGR24,            ///< B', G' and R' components: BGRBGRBGRBGR
        RGB32,            ///< R', G' and B' components: RGB?RGB?RGB?RGB?
        BGR32,            ///< B', G' and R' components: BGR?BGR?BGR?BGR?
        YCbCr420_Planar,  ///< YUV/Y'CbCr 4:2:0 (planar): YYYY  +  UU  +  VV
                          ///<                            YYYY
        YCbCr422_Planar,  ///< YUV/Y'CbCr 4:2:2 (planar): YYYY  +  UU  +  VV
        YCbCr444_Planar,  ///< YUV/Y'CbCr 4:4:4 (planar): YYYY  +  UUUU  +  VVVV
        RGBA,             ///<  R', G' and B' components, pre-multipled with alpha values: RGBARGBARGBARGBA
        BGRA,             ///<  B', G' and R' components, pre-multipled with alpha values: BGRABGRABGRABGRA
        NV12              ///< Planar Y', merged/composite Cr->Cb (4:2:0): YYYY  +  UVUV
      };

      /**
       * @brief Enummeration describing how image coefficients in memory are to be interpreted.
       *
       * Currently all color sub-formats describes various kinds of
       * YCbCr. For other color formats than YCbCr, the NotApplicable
       * value must be used.
       */
      enum color_space_detail_t
      {
        NotApplicable,      ///< No additional color space info is available, color space implied by image_format_t.
        YCbCr_JFIF,         ///< YCbCr coefficients are coded according to the JPEG File Interchange Format - "JFIF" convention.
        YCbCr_RecBT601,     ///< YCbCr coefficients are coded according to the ITU-R Recommendation BT.601.
        YCbCr_FullRange     ///< YCbCr coefficients are coded using the full 8-bit range. 
      };

    public:
      /** @name Get methods */ //@{
      /**
       * @brief Get image format.
       *
       * @return The image format.
       */
      virtual image_format_t GetImageFormat() const=0;
      
      /**
       * @brief Get image color space detail.
       *
       * @return The image color space detail.
       */
      virtual color_space_detail_t GetColorSpaceDetail() const=0;

      /**
       * @brief Get pixel aspect ratio (pixel_width / pixel_height).
       *
       * @return Pixel aspect ratio.
       */
      virtual double GetPixelAspectRatio() const=0;

      /**
       * @brief Get image width in pixels.
       *
       * @return Image width in pixels.
       */
      virtual std::size_t GetImageWidth() const=0;

      /**
       * @brief Get image height in pixels.
       *
       * @return Image height in pixels.
       */
      virtual std::size_t GetImageHeight() const=0;
      
      /**
       * @brief Get plane width in pixels.
       *
       * @param planeIndex Zero-based index for the requested plane.
       * @return Plane width in pixels.
       */
      virtual std::size_t GetPlaneWidth(std::size_t planeIndex) const=0;

      /**
       * @brief Get plane height in pixels.
       *
       * @param planeIndex Zero-based index for the requested plane.
       * @return Plane height in pixels.
       */
      virtual std::size_t GetPlaneHeight(std::size_t planeIndex) const=0;

      /**
       * @brief Get plane stride (distance between two lines/rows) in bytes.
       *
       * @param planeIndex Zero-based index for the requested plane.
       * @return Plane stride in bytes.
       */
      virtual std::size_t GetPlaneStride(std::size_t planeIndex) const=0; 

      /**
       * @brief Get pointer to plane.
       *
       * @param planeIndex Zero-based index for the requested plane.
       * @return Pointer to first byte in plane.
       */
      virtual const byte_t *GetPlanePointer(std::size_t planeIndex) const=0;
      //@}
  };


  /* --- ImEventData class -------------------------------------------------- */

  /**
   * @brief Event data interface.
   *
   * All meta data handled by toolkits must implement this interface.
   */
  class ImEventData : public ImData
  {
    public:
      /**
       * @brief Get event as xml string
       *
       * @return The xml encoded event
       */
      virtual utf8_string_t GetEventXml() const=0;
      //@}
  };


  /* --- ImSearchData class ------------------------------------------------ */

  /**
   * @brief Search data interface.
   *
   * All search data handled by toolkits must implement this interface.
   */
  class ImSearchData : public ImData
  {
    public:
      /** @name Object methods */ //@{
      /**
       * @brief Get object id associated with search data.
       *
       * @return Object id assoicated with search data (empty string means all objects)
       */
      virtual utf8_string_t GetObjectId() const = 0;
      //@}   
  };


  /* --- ImMetaData class -------------------------------------------------- */

  /**
   * @brief Meta data interface.
   *
   * All meta data handled by toolkits must implement this interface.
   */
  class ImMetaData : public ImData
  {
    public:
  };


  /* --- ImOutOfBandData class --------------------------------------------- */

  /**
   * @brief Out of band data interface.
   *
   * All out-of-band messages passed by toolkits must implement this
   * interface.
   */
   class ImOutOfBandData : public ImData
   {
     public:
   };


   /* --- ImOutOfBandData class --------------------------------------------- */

   /**
   * @brief Out of band data interface.
   *
   * All out-of-band messages passed by toolkits must implement this
   * interface.
   */
   class ImLiveStatusData : public ImOutOfBandData
   {
   public:
     virtual utc_time_t GetStatusTime() const = 0;

     virtual bool GetCameraLiveFeedStarted(bool &result) const = 0;
     virtual bool GetCameraLiveMotion(bool &result) const = 0;
     virtual bool GetCameraRecording(bool &result) const = 0;
     virtual bool GetCameraEventNotification(bool &result) const = 0;
     virtual bool GetCameraConnectionLost(bool &result) const = 0;
     virtual bool GetCameraDatabaseFail(bool &result) const = 0;
     virtual bool GetServerRunningOutOfDiskSpace(bool &result) const = 0;
     virtual bool GetClientLiveFeedStopped(bool &result) const = 0;
   };


   /* --- ImExportData class ----------------------------------------------- */
   
   /**
    * @brief Export data interface.
    *
    * All data output from an export must implement this interface.
    */
    class ImExportData : public ImData
    {
      public:
    };
  /**
  * @brief Sequence data interface.
  *
  * All sequence information must implement this interface
  */
  class ImSequenceInfoData : public ImData
  {
  public:
    /** @name Time slice methods */ //@{
    /**
    * @brief Get begin time slice of sequence.
    *
    * @return Begin time slice of sequence.
    *
    * @see GetEndTimeSlice()
    */
    virtual utc_time_t GetBeginTimeSlice() const = 0;

    /**
      * @brief Get end time slice of sequence.
      *
      * @return End time slice of sequence.
      *
      * @see GetBeginTimeSlice()
      */
      virtual utc_time_t GetEndTimeSlice() const = 0;
      //@}
  };

  /**
  * @brief Motion Level grid data interface.
  *
  * All Motion Level grid must implement this interface
  */

  class ImMotionLevelGridData : public ImData
  {
  public:
    /** @name Query */ //@{
    virtual const std::size_t GridWidth() = 0;                        ///< The width of the motion level grid.
    virtual const std::size_t GridHeight() = 0;                       ///< The height of the motion level grid.
    virtual const std::size_t ImageWidth() = 0;                       ///< The width of the image from which the motion level grid was generated.
    virtual const std::size_t ImageHeight() = 0;                      ///< The height of the image from which the motion levle grid was generated.
    virtual double Sensitivity() = 0;                                      ///< The motion sensitivity used to generate the motion level grid.
    virtual const std::vector<std::size_t> &AccumulatedCounts() = 0;  ///< The accumulated counts stored in the motion level grid.
  };

}

#endif // Toolkits_DataInterface_h_INCLUDED
