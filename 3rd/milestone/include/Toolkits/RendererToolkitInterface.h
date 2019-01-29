/**
 * @file
 *
 * @brief Header file containing definitions of renderer toolkit interfaces.
 */

#if !defined(Toolkits_RendererToolkitInterface_h_INCLUDED)
#define Toolkits_RendererToolkitInterface_h_INCLUDED

#include <Toolkits/DataInterface.h>
#include <Toolkits/Types.h>
#include <Toolkits/BaseToolkitInterface.h>

#include <map>
#include <set>
#include <list>
#include <tuple>
#include <memory>

/* === Namespace =========================================================== */

namespace NmToolkit
{
  /* === ImDigitalZoomController interface ====================================== */

  class ImDigitalZoomController
  {
  public:
    enum class eMoveCommand
    {
      Up,
      UpRight,
      Right,
      DownRight,
      Down,
      DownLeft,
      Left,
      UpLeft,
      Home,
      ZoomIn,
      ZoomOut
    };

    virtual void SetAbsolutePosition(const std::string &position) = 0;
    virtual const std::string GetAbsolutePosition() = 0;

    virtual void MoveRelative(const double left, const double top, const double right, const double bottom) = 0;
    virtual void MoveRelative(const double x, const double y) = 0;

    virtual void Move(const eMoveCommand command) = 0;

    virtual double GetZoom() const = 0;
  };


  /**
   * @interface ImRendererToolkit
   * @brief Interface for rendering data from a number of source toolkits.
   */
  class ImRendererToolkit : public ImToolkit
  {
    public:
      /**
       * @brief Unique identifier of a source.
       *
       * Valid identifiers are positive numbers greater than 0. A 
       * negative or nul source id does not represent a source.
       */
      typedef int source_id_t;

      /**
       * @brief Playback speed as a signed floating point.
       *
       * A speed of 1.0 means forward playback in realtime as it was
       * recorded. Likewise a speed of 0.5 and 2.0 means respecitively
       * forward playback of half and double speed relatively to how
       * it was recorded. Backwards playback is achieved using
       * negative values. A speed of 0.0 will pause the playback.
       */
      typedef double playback_speed_t;

      /**
      * @brief Enumeration of possible source states.
      *
      * @see ImSourceStateHandler::HandleSourceStateChanged()
      */
      enum deinterlace_mode_t
      {
        NoFilter,           ///< Perform simple deinterlacing
        StretchTopField,    ///< Uneven lines will be doubled
        StretchBottomField, ///< Uneven lines will be doubled
        ContentAdaptive     ///< Decide how to deinterlace based on content (usually motion or static)
      };

      enum class decoding_hardware_selection_t
      {
        OnlyIPP,
        OnlyIntel,
        OnlyNvidia,
        AutoIntel,
        AutoNvidia
      };
      
      /**
       * @brief Enumeration of possible source states.
       *
       * @see ImSourceStateHandler::HandleSourceStateChanged()
       */
      enum source_state_t
      {
        Disconnected,         ///< Source is currently disconnected. Will not leave this state until actively connected using the ConnectSources() method.
        Connecting,           ///< Source is currently attempting to establish a connection. When established, the source will enter the @ref Connected state.
        Connected,            ///< Source is currently connected. Data is now ready to be recieved from the source.
        ConnectionFailed,     ///< Connection could not be established to source; will retry connecting very shortly and enter the @ref Connecting state.
        ConnectionLost,       ///< Source has lost its connection and will retry connecting very shortly and enter the @ref Reconnecting state.
        Reconnecting          ///< Source has lost its connection and is now attempting to restore the connection. Once restored, the state of the source will be set to @ref Connected.
      };

      /**
       * @brief Enumeration of defined source flags.
       *
       * Each flag is represented by a bit in the source flags integer
       * provided in @ref ImSourceStateHandler::HandleSourceStateChanged() "HandleSourceStateChanged()"
       *
       * @see ImSourceStateHandler::HandleSourceStateChanged()
       */
      enum source_flags_t
      {
        LiveNotSupported              = 1<<0, ///< Bit 0; If set, source does not implement the ImLiveSourceToolkit interface.
        PlaybackNotSupported          = 1<<1, ///< Bit 1; If set, source does not implement the ImPlaybackSourceToolkit interface.
        NotAuthorized                 = 1<<2, ///< Bit 2; If set, source has reported that it was not authorized to get data.
        LicenseInvalid                = 1<<3, ///< Bit 3; If set, source has reported that data cannot be retrieved because of an invalid license.
        MaxNumberOfConnectionsReached = 1<<4, ///< Bit 4; If set, source has reported that data cannot be retrieved because the maximum number of connections has been reached.
        DewarpingFailed               = 1<<5  ///< Bit 5; If set, source is delivering images that can't be dewarped.
      };

			enum hardware_acceleration_t
			{
				Nvidia,
				Intel,
				Off
			};

      typedef int syncronization_tag_t;

      /**
       * @interface ImRenderingParameters
       * @brief Parameters specifying how to render data.
       *
       * @see AddSources(), UpdateSourceRenderingParameters()
       */
      class ImRenderingParameters
      {
        public:
          /** @name Constructors and destructors */ //@{
          /**
           * @brief Virtual toolkit destructor to ensure proper destruction through interface.
           */
          virtual ~ImRenderingParameters() throw() {}
          //@}
      };

      /**
       * @struct SmImageRenderingParameters
       * @brief Parameters specifying how to render images.
       */
      class SmImageRenderingParameters : public ImRenderingParameters
      {
        public:
          SmImageRenderingParameters()
            : m_DeinterlaceMode(deinterlace_mode_t::NoFilter),
              m_RemovableMasksLifted(false),
              m_HardwareAcceleration(decoding_hardware_selection_t::OnlyIPP),
              m_NumberOfDecodingThreads(false),
              m_KeepAspectRatio(false),
              m_WindowWidth(0),
              m_WindowHeight(0),
              m_BufferTimeSpan(0)
          {}

          SmImageRenderingParameters(
            deinterlace_mode_t DeinterlaceMode,
            bool RemovableMasksLifted,
            decoding_hardware_selection_t HardwareAcceleration,
            int NumberOfDecodingThreads,
            bool KeepAspectRatio,
            int WindowWidth,
            int WindowHeight,
            time_span_t BufferTimeSpan
          )
          : m_DeinterlaceMode(DeinterlaceMode),
          m_RemovableMasksLifted(RemovableMasksLifted),
          m_HardwareAcceleration(HardwareAcceleration),
          m_NumberOfDecodingThreads(NumberOfDecodingThreads),
          m_KeepAspectRatio(KeepAspectRatio),
          m_WindowWidth(WindowWidth),
          m_WindowHeight(WindowHeight),
          m_BufferTimeSpan(BufferTimeSpan)
          {}


          ~SmImageRenderingParameters() throw() {};

          deinterlace_mode_t m_DeinterlaceMode;                ///< Specifies which filter to use while deinterlacing (if any).
          bool m_RemovableMasksLifted;                         ///< If the removable masks are lifted or not.
          decoding_hardware_selection_t m_HardwareAcceleration;///< Priority of hardware acceleration options.
          int m_NumberOfDecodingThreads;                       ///< The number of decoder threads to use (-1 is auto).
          bool m_KeepAspectRatio;                              ///< True if aspect ratio should be kept.
          int m_WindowWidth;                                   ///< Width of window in number of pixels into which image will be rendered.
          int m_WindowHeight;                                  ///< Height of window in number of pixels into which image will be rendered.
          time_span_t m_BufferTimeSpan;                        ///< Buffer size in time of how many images to buffer ahead of time.

          bool operator==(const SmImageRenderingParameters &other) const
          {
            return m_DeinterlaceMode == other.m_DeinterlaceMode &&
              m_RemovableMasksLifted == other.m_RemovableMasksLifted &&
              m_HardwareAcceleration == other.m_HardwareAcceleration &&
              m_NumberOfDecodingThreads == other.m_NumberOfDecodingThreads &&
              m_KeepAspectRatio == other.m_KeepAspectRatio &&
              m_WindowWidth == other.m_WindowWidth &&
              m_WindowHeight == other.m_WindowHeight &&
              m_BufferTimeSpan == other.m_BufferTimeSpan;
          }

          bool IsWindowSizeChanged(const SmImageRenderingParameters &other) const
          {
            return m_WindowWidth != other.m_WindowWidth || m_WindowHeight == other.m_WindowHeight;
          }
      };

      /**
       * @interface ImRenderingInformation
       * @brief Information on how data was rendered.
       *
       * @see HandleRenderedData()
       */
      class ImRenderingInformation
      {
        public:
          /** @name Constructors and destructors */ //@{
          /**
           * @brief Virtual toolkit destructor to ensure proper destruction through interface.
           */
          virtual ~ImRenderingInformation() throw() {}
          //@}
      };

      /**
       * @interface ImImageRenderingInformation
       * @brief Information on how image was rendered.
       */
      class ImImageRenderingInformation : public ImRenderingInformation
      {
        public:
          /**
           * @brief Enummeration of different image models.
           */
          enum image_model_t
          {
            BGR32Image,  ///< The image is rendered into a BGR32 image. Downcast to the @ref ImBGR32ImageRenderingInformation interface for more information.
            D3D9Surface  ///< The image is rendered into a Direct3D (DirectX 9) Surface. Downcast to the @ref ImD3D9SurfaceRenderingInformation for more information.
          };

        public:
          /**
           * @brief Get tag that was attached to image.
           *
           * @return Tag that was attached to image.
           */
          virtual syncronization_tag_t GetImageSynchronizationTag() const = 0;

          /**
           * @brief Get image model used for rendered image.
           *
           * @return Image model used for rendered image.
           */
          virtual image_model_t GetImageModel() const = 0;

          /**
           * @brief Which type of hardware decoding was used.
           *
           * @return Nvidia if Nvidia decoding was used, Intel if Intel Media SDK Hardware decoding was used and Off if any other decoding mode was used
           * (1) Intel Media UMC (using IPP),
           * (2) Intel Media SDK Software (uses the Intel Media SDK Software dll distributed with XProtect)
           * (3) fallback from Intel Media SDK hardware to Intel Media SDK software (uses the Intel Media SDK hardware dll installed with the graphics driver).
           */
          virtual hardware_acceleration_t GetHardwareAccelerationType() const = 0;

          /**
           * @brief Is rendered image coming from a live source.
           *
           * @return True if rendered image is coming from a live source.
           */
          virtual bool IsLive() const = 0;

          /**
           * @brief Is rendered image flipped horizontally.
           *
           * @return True if rendered image is flipped horizontally.
           */
          virtual bool IsFlippedHorizontally() const = 0;

          /**
           * @brief Is rendered image flipped vertically.
           *
           * @return True if rendered image is flipped vertically.
           */
          virtual bool IsFlippedVertically() const = 0;

          /**
           * @brief Get time stamp of rendered image.
           *
           * @return Time stamp of rendered image.
           */
          virtual utc_time_t GetImageTime() const = 0;

          /**
           * @brief Get time stamp of image just after the one rendered.
           *
           * @return Time stamp of next image.
           */
          virtual utc_time_t GetNextImageTime() const = 0;

          /**
           * @brief Get time stamp of image just before the one rendered.
           *
           * @return Time stamp of previous image.
           */
          virtual utc_time_t GetPreviousImageTime() const = 0;

          /**
           * @brief Get width and height of image before any scaling has been applied.
           *
           * @param[out] width Width of original image.
           * @param[out] height Height of original image.
           */
          virtual void GetOriginalImageSize(size_t &width, size_t &height) const = 0;

          enum class eDataAvailabilityAtRequestedTime
          {
            ImageAvailable,
            BeforeDatabaseStart,
            AfterDatabaseEnd,
            InGap,
            NoData,
            BlankFiller,
            DecodingFailed
          };

          virtual eDataAvailabilityAtRequestedTime GetDataAvailabilityAtRequestedTime() const = 0;

          /**
           * @brief Get video codec by which image was compressed.
           *
           * @return Video coedec identifier (e.g. H.264, JPEG, ...).
           */
          virtual std::string GetVideoCodec() const = 0;
      };

      /**
       * @interface ImBGR32ImageRenderingInformation
       * @brief Information on how image was rendered into a BGR32 buffer.
       */
      class ImBGR32ImageRenderingInformation : public ImImageRenderingInformation
      {
        public:
          /**
           * @brief Get image model used for rendered image.
           *
           * @return Always returns BGR32Image.
           */
          virtual image_model_t GetImageModel() const { return BGR32Image; }

          /**
           * @brief Get pointer to BGR32 buffer into which image has been rendered.
           *
           * @return Pointer to BGR32 buffer.
           */
          virtual byte_t *GetBuffer() const = 0;

          /**
           * @brief Get width, height and stride of image in BGR32 buffer.
           *
           * @param[out] width Width of image in pixels.
           * @param[out] height Height of image in pixels.
           * @param[out] stride Row stride in BGR32 buffer in number bytes.
           */
          virtual void GetImageSize(size_t &width, size_t &height, size_t &stride) const = 0;
      };

      /**
       * @interface ImD3D9SurfaceRenderingInformation
       * @brief Information on how image was rendered into a Direct3D Surface.
       */
      class ImD3D9SurfaceRenderingInformation : public ImImageRenderingInformation
      {
        public:
          /**
           * @brief Get image model used for rendered image.
           *
           * @return Always returns D3D9Surface.
           */
          virtual image_model_t GetImageModel() const { return D3D9Surface; }

          /**
           * @brief Get pointer to surface into which image has been rendered.
           *
           * @return Pointer to Direct3D (DirectX 9) Surface.
           */
          virtual void *GetSurface() const = 0;

          /**
           * @brief Get viewport of image in surface as rectangle defined by coordinates of the upper-left and lower-right corners.
           *
           * Left is smaller than right and top is smaller than bottom.
           * Also the right and bottom edges are considered exclusive.
           *
           * @param[out] left The x-coordinate of the upper-left corner of the rectangle.
           * @param[out] top The y-coordinate of the upper-left corner of the rectangle.
           * @param[out] right The x-coordinate of the lower-right corner of the rectangle.
           * @param[out] bottom The y-coordinate of the lower-right corner of the rectangle.
           */
          virtual void GetImageViewport(size_t &left, size_t &top, size_t &right, size_t &bottom) const = 0;

          /**
           * @brief Get viewport of window in surface as rectangle defined by coordinates of the upper-left and lower-right corners.
           *
           * Left is smaller than right and top is smaller than bottom.
           * Also the right and bottom edges are considered exclusive.
           *
           * @param[out] left The x-coordinate of the upper-left corner of the rectangle.
           * @param[out] top The y-coordinate of the upper-left corner of the rectangle.
           * @param[out] right The x-coordinate of the lower-right corner of the rectangle.
           * @param[out] bottom The y-coordinate of the lower-right corner of the rectangle.
           */
          virtual void GetWindowViewport(size_t &left, size_t &top, size_t &right, size_t &bottom) const = 0;
      };

      /**
       * @interface ImRenderingHandler
       * @brief Interface with call-back method for handling rendered data.
       *
       * @see SetRenderingHandler()
       */
      class ImRenderingHandler
      {
        public:
          /** @name Constructors and destructors */ //@{
          /**
           * @brief Virtual toolkit destructor to ensure proper destruction through interface.
           */
          virtual ~ImRenderingHandler() throw() {}
          //@}

          /** @name Callback methods */ //@{
          /**
           * @brief Method that is called every time data has been rendered for one or more sources.
           *
           * No lenghty operations should be performed in this
           * call-back method. Also no exceptions must leave the
           * call-back method.
           *
           * @param renderingInformation Rendering information for each source that had data rendered.
           */
          virtual void HandleRenderedData(const std::map<NmToolkit::ImRendererToolkit::source_id_t, std::shared_ptr<const NmToolkit::ImRendererToolkit::ImRenderingInformation> > &renderingInformation) = 0;
          //@}
      };

      /**
       * @interface ImOutOfBandHandler
       * @brief Interface with method for handling out-of-band data.
       *
       * @see SetOutOfBandHandler()
       */
      class ImOutOfBandHandler
      {
        public:
          /** @name Constructors and destructors */ //@{
          /**
           * @brief Virtual toolkit destructor to ensure proper destruction through interface.
           */
          virtual ~ImOutOfBandHandler() throw() {}
          //@}

          /** @name Callback methods */ //@{
          /**
           * @brief Method that is called every time out-of-band data is received from a source.
           *
           * No lenghty operations should be performed in this
           * call-back method. Also no exceptions must leave the
           * call-back method.
           *
           * Note that this method is likely to be called from source
           * specific threads and that there is no synchronization
           * between these.
           *
           * @param sourceId Id of source for which the out-of-band data is received.
           * @param outOfBandData Out-of-band data received.
           */
          virtual void HandleOutOfBandData(NmToolkit::ImRendererToolkit::source_id_t sourceId, NmToolkit::ImOutOfBandData *outOfBandData) = 0;
          //@}
      };

      /**
       * @interface ImSourceStateHandler
       * @brief Interface with method handling source state changes.
       *
       * @see SetSourceStateHandler()
       */
      class ImSourceStateHandler
      {
        public:
          /** @name Constructors and destructors */ //@{
          /**
           * @brief Virtual toolkit destructor to ensure proper destruction through interface.
           */
          virtual ~ImSourceStateHandler() throw() {}
          //@}

          /** @name Callback methods */ //@{
          /**
           * @brief Method that is called every time the state of a source has changed.
           *
           * No lenghty operations should be performed in this
           * call-back method. Also no exceptions must leave the
           * call-back method.
           *
           * Note that this method is likely to be called from source
           * specific threads and that there is no synchronization
           * between these.
           *
           * @param sourceId Id of source for which the new state applies.
           * @param sourceState New state of source.
           * @param sourceFlags Flags of source represented as individual bits (see @ref source_flags_t)
           */
          virtual void HandleSourceStateChanged(NmToolkit::ImRendererToolkit::source_id_t sourceId, NmToolkit::ImRendererToolkit::source_state_t sourceState, uint32_t sourceFlags) = 0;
          //@}
      };

      /**
       * @interface ImDiagnosticsHandler
       * @brief Interface with method for handling the reception of diagnostics data.
       *
       * @see SetDiagnosticsHandler()
       */
      class ImDiagnosticsHandler
      {
      public:
        /** @name Constructors and destructors */ //@{
        /**
         * @brief Virtual toolkit destructor to ensure proper destruction through interface.
         */
        virtual ~ImDiagnosticsHandler() throw() {}
        //@}

      /** @name Callback methods */ //@{
      /**
       * @brief Method that is called with regular intervals to
       * provide information about the performance of a data
       * source.
       *
       * No lenghty operations should be performed in this
       * call-back method. Also no exceptions must leave the
       * call-back method.
       *
       * Note that this method is likely to be called from source
       * specific threads and that there is no synchronization
       * between these.
       *
       * @param sourceId Id of source of which we handle diagnostics data.
       * @param diagnosticsData a key/value list of diagnostics parameters.
       */
        virtual void HandleDiagnosticsData(NmToolkit::ImRendererToolkit::source_id_t sourceId, const std::map<std::string, std::string> &diagnosticsData) = 0;

      /**
       * @brief Method that is called with regular intervals to
       * provide general information about the performance of the
       * renderer toolkit.
       *
       * No lenghty operations should be performed in this
       * call-back method. Also no exceptions must leave the
       * call-back method.
       *
       * @param diagnosticsData a key/value list of diagnostics parameters.
       */
        virtual void HandleDiagnosticsData(const std::map<std::string, std::string> &diagnosticsData) = 0;
      //@}
    };

    public:
      /**
       * @brief Post construction initializer.
       *
       * Since the constructor is called in a dll loader, any exceptions from it can not be passed
       * to the client. This function carries out the part of the initializaiton that could throw
       * an exception that the client could be interested in. It gives the toolkit factory an
       * opportunity to throw a meaningful exception as if it had been thrown directly by the
       * constructor method.
       */
      virtual void Initialize() = 0;

      /** @name Source methods */ //@{
      /**
       * @brief Add new sources.
       *
       * For every source added, two parameters are needed. The first
       * is the source toolkit configuration XML, which defines where
       * to get the data from. The second parameter is the render
       * parameters, which specifies how to render the data retrieved
       * from the source.
       *
       * The source toolkit specified does not need to implement both
       * the ImPlaybackSourceToolkit and the ImLiveSourceToolkit. If
       * not implemented however, respectively playback and live mode
       * will not work and the source will have respectively its @ref
       * PlaybackNotSupported and @ref LiveNotSupported source flags
       * set.
       *
       * Note that it is more efficient to add many sources in one
       * call instead of adding them individually.
       *
       * Also note that when a source is first added, it is in the
       * @ref Disconnected state. To connect a source, the method
       * ConnectSources() needs to be called.
       *
       * @param sourcesToAdd Vector containing pairs of source toolkit
       *     configuration XML and associated rendering parameters.
       * @return Vector of source identifiers. Each identifier refers
       *     to the source that was created for the pair corresponding
       *     in the input vector @p sourcesToAdd. Ids are allocated in
       *     ascending order and are not reused, unless the count wraps
       *     around on max_int, so it is allowed to assume that a newly
       *     allocated id will not match that of a recently removed
       *     source.
       *
       * @exception ImInvalidArgumentError Is thrown if the specified rendering parameters are not valid.
       * @exception ImCreatingToolkitError Is thrown on failure to create one of the source toolkit instances.
       *
       * @see RemoveSources(), ConnectSources(), SetSourceStateHandler()
       */
      virtual std::vector<source_id_t> AddSources(const std::vector<std::pair<utf8_string_t, std::shared_ptr<const ImRenderingParameters> > > &sourcesToAdd) = 0;

      /**
       * @brief Remove subset of sources.
       *
       * When removing a source, it is destroyed and all resources
       * associated with it are freed. A source can be removed at all
       * times no matter what state or mode it is in.
       *
       * @param sourcesToRemove Set of sources to remove.
       *
       * @exception ImInvalidArgumentError Is thrown if one of the specified source ids is unknown.
       *
       * @see AddSources()
       */
      virtual void RemoveSources(const std::set<source_id_t> &sourcesToRemove)=0;

      /**
       * @brief Update single parameter in configuration of source.
       *
       * For the source identified by @p sourceId, this method updates
       * the parameter with name @p configurationParameterName to the
       * value of @p configurationParameterValue. If the given
       * parameter name is not applicable for the specified source
       * toolkit or if the parameter was not configured to be
       * updateable, the method does nothing.
       *
       * If the specified source identifier does not identify an added
       * source, an exception will be thrown. Likewise if the
       * configuration parameter value specified cannot be parsed to
       * the type matching what is expected by the parameter to
       * update, then an exception is also thrown.
       *
       * Consult the documentation of the specific source toolkit in
       * use to learn which parameters can be used, when they can be
       * used and what their legal values are.
       *
       * @param sourceId Id of source to update.
       * @param configurationParameterName Name of configuration parameter to update.
       * @param configurationParameterValue Value of configuration parameter to apply as a string.
       *
       * @exception ImInvalidArgumentError Is thrown if @p sourceId is unknown or if @p configurationParameterValue could not be parsed to the correct type.
       */
      virtual void UpdateSourceConfiguration(source_id_t sourceId, const utf8_string_t &configurationParameterName, const utf8_string_t &configurationParameterValue) = 0;

      /**
       * @brief Update list of parameters in configuration of source.
       *
       * This method works like the other update configuration method
       * except it can update a list of parameters in one atomic call.
       *
       * @param sourceId Id of source to update.
       * @param configurationParameterList List of parameter name and value pairs representing the parameters to be updated.
       *
       * @exception ImInvalidArgumentError Is thrown if @p sourceId is unknown or if one of the specified configuration parameter values could not be parsed to the correct type.
       */
      virtual void UpdateSourceConfiguration(source_id_t sourceId, const std::list<std::pair<utf8_string_t, utf8_string_t> > &configurationParameterList) = 0;

      /**
       * @brief Update rendering parameters associated with source.
       *
       * There is a significant performance hit associated with
       * updating rendering parameters. If updating multiple sources, it
       * is better to do this in one call instead of many.
       *
       * The rendering parameters of a source can be updated at all times
       * no matter what the current state or mode of the source is.
       *
       * @param renderingParameters rendering parameters to update for each source.
       *
       * @exception ImInvalidArgumentError Is thrown if one of the specified source ids is unknown or if some of the rendering parameters are invalid.
       */
      virtual void UpdateSourceRenderingParameters(const std::map<source_id_t, std::shared_ptr<const ImRenderingParameters> > &renderingParameters) = 0;

      /**
       * @brief Set a unique number that is attached to all images processed after this method is called.
       *
       * @param sourceId Id of source for which to set synchronization tag.
       * @param tag Unique value that can be compared with synchronization tags of images that are subsequently delivered.
       */
      virtual void SetImageSynchronizationTag(const source_id_t sourceId, syncronization_tag_t tag) = 0;

      /**
       * @brief Connect subset of sources.
       *
       * This method will connect the source toolkits to their
       * respective endpoints and the sources will enter the @ref
       * Connecting state.
       *
       * Connecting a source that is already connected is not an error
       * and does nothing.
       *
       * @param sourcesToConnect Set of sources to connect.
       *
       * @exception ImInvalidArgumentError Is thrown if a specified source id is unknown.
       *
       * @see DisconnectSources(), SetSourceStateHandler()
       */
      virtual void ConnectSources(const std::set<source_id_t> &sourcesToConnect)=0;

      /**
       * @brief Disconnect subset of sources.
       *
       * This method will disconnect the source toolkits from their
       * respecitive endpoints and the sources will enter the @ref
       * Disconnected state.
       *
       * Disconnecting a source that is already disconnected is not an
       * error and does nothing.
       *
       * @param sourcesToDisconnect Set of sources to disconnect.
       *
       * @exception ImInvalidArgumentError Is thrown if a specified source id is unknown.
       *
       * @see ConnectSources(), SetSourceStateHandler()
       */
      virtual void DisconnectSources(const std::set<source_id_t> &sourcesToDisconnect)=0;
      //@}

      virtual std::shared_ptr<ImDigitalZoomController> GetDigitalZoomController(source_id_t sourceId, const utf8_string_t &configurationXML)=0;

      virtual void GetCurrentImage(source_id_t sourceId, int &width, int &height, int &stride, unsigned char* &data) = 0;

      virtual void ClearCache(source_id_t sourceId, utc_time_t beginTimestamp, utc_time_t endTimestamp) = 0;

      /** @name Navigation methods */ //@{
      /**
       * @brief Move source to its first data.
       *
       * This method will enter the @a pause mode for the source
       * specified by @p sourceToMove and then move it to its first
       * data.
       *
       * It is not an error to move a source that is not in the @ref
       * Connected state. In this case the move will be ignored.
       *
       * Note that any pending navigation operations will be discarded
       * and the source will be moved directly to its first data.
       *
       * @param sourceToMove Id of source to move to its first data.
       *
       * @exception ImInvalidArgumentError Is thrown if specified source id @p sourceToMove is unknown.
       *
       * @see MoveLast(), MoveNext(), MovePrevious(), MoveNextSequence(), MovePreviousSequence(), MoveTo(), DoPlayback(), DoLive()
       */
      virtual void MoveFirst(source_id_t sourceToMove)=0;

      /**
       * @brief Move source to its last data.
       *
       * This method will enter the @a pause mode for the source
       * specified by @p sourceToMove and then move it to its last
       * data.
       *
       * It is not an error to move a source that is not in the @ref
       * Connected state. In this case the move will be ignored.
       *
       * Note that any pending navigation operations will be discarded
       * and the source will be moved directly to its last data.
       *
       * @param sourceToMove Id of source to move to its last data.
       *
       * @exception ImInvalidArgumentError Is thrown if specified source id @p sourceToMove is unknown.
       *
       * @see MoveFirst(), MoveNext(), MovePrevious(), MoveNextSequence(), MovePreviousSequence(), MoveTo(), DoPlayback(), DoLive()
       */
      virtual void MoveLast(source_id_t sourceToMove)=0;

      /**
       * @brief Move source to its next data.
       *
       * This method will enter the @a pause mode for the source
       * specified by @p sourceToMove and then move it to its next
       * data.
       *
       * It is not an error to move a source that is not in the @ref
       * Connected state. In this case the move will be ignored.
       *
       * Note that any pending navigation operations will be executed
       * before the source is moved to its next data.
       *
       * @param sourceToMove Id of source to move to its next data.
       *
       * @exception ImInvalidArgumentError Is thrown if specified source id @p sourceToMove is unknown.
       *
       * @see MoveFirst(), MoveLast(), MovePrevious(), MoveNextSequence(), MovePreviousSequence(), MoveTo(), DoPlayback(), DoLive()
       */
      virtual void MoveNext(source_id_t sourceToMove)=0;

      /**
       * @brief Move source to its previous data.
       *
       * This method will enter the @a pause mode for the source
       * specified by @p sourceToMove and then move it to its previous
       * data.
       *
       * It is not an error to move a source that is not in the @ref
       * Connected state. In this case the move will be ignored.
       *
       * Note that any pending navigation operations will be executed
       * before the source is moved to its previous data.
       *
       * @param sourceToMove Id of source to move to its previous data.
       *
       * @exception ImInvalidArgumentError Is thrown if specified source id @p sourceToMove is unknown.
       *
       * @see MoveFirst(), MoveLast(), MoveNext(), MoveNextSequence(), MovePreviousSequence(), MoveTo(), DoPlayback(), DoLive()
       */
      virtual void MovePrevious(source_id_t sourceToMove)=0;

      /**
       * @brief Move source to first data of next sequence.
       *
       * This method will enter the @a pause mode for the source
       * specified by @p sourceToMove and then move it to the first
       * data of the next sequence.
       *
       * It is not an error to move a source that is not in the @ref
       * Connected state. In this case the move will be ignored.
       *
       * Note that any pending navigation operations will be executed
       * before the source is moved to the first data of the next
       * sequence.
       *
       * @param sourceToMove Id of source to move to first data of next sequence.
       *
       * @exception ImInvalidArgumentError Is thrown if specified source id @p sourceToMove is unknown.
       *
       * @see MoveFirst(), MoveLast(), MoveNext(), MovePrevious(), MovePreviousSequence(), MoveTo(), DoPlayback(), DoLive()
       */
      virtual void MoveNextSequence(source_id_t sourceToMove)=0;

      /**
       * @brief Move source to first data of previous sequence.
       *
       * This method will enter the @a pause mode for the source
       * specified by @p sourceToMove and then move it to the first
       * data of the previous sequence.
       *
       * It is not an error to move a source that is not in the @ref
       * Connected state. In this case the move will be ignored.
       *
       * Note that any pending navigation operations will be executed
       * before the source is moved to the first data of the previous
       * sequence.
       *
       * @param sourceToMove Id of source to move to first data of previous sequence.
       *
       * @exception ImInvalidArgumentError Is thrown if specified source id @p sourceToMove is unknown.
       *
       * @see MoveFirst(), MoveLast(), MoveNext(), MovePrevious(), MoveNextSequence(), MoveTo(), DoPlayback(), DoLive()
       */
      virtual void MovePreviousSequence(source_id_t sourceToMove)=0;

      /**
       * @brief Move source to data of specified time stamp.
       *
       * This method will enter the @a pause mode for the source
       * specified by @p sourceToMove and then move it to the data of
       * the specified time stamp.
       *
       * If there is no data matching the specified time stamp
       * exactly, then the first data before the time stamp is
       * selected.
       *
       * It is not an error to move a source that is not in the @ref
       * Connected state. In this case the move will be ignored.
       *
       * Note that any pending navigation operations will be discarded
       * and the source will be moved directly to the data of the
       * specified time stamp.
       *
       * @param sourceToMove Id of source to move to data of specified time stamp.
       * @param timeStamp Time stamp of data to move source to.
       *
       * @exception ImInvalidArgumentError Is thrown if specified source id @p sourceToMove is unknown.
       *
       * @see MoveFirst(), MoveLast(), MoveNext(), MovePrevious(), MoveNextSequence(), MovePreviousSequence(), DoPlayback(), DoLive()
       */
      virtual void MoveTo(source_id_t sourceToMove, utc_time_t timeStamp)=0;

      /**
       * @brief Enter playback mode / synchronize playback of source.
       *
       * This method will make the specified source enter @a playback
       * mode if not already in that mode. The @a playback mode can be
       * entered directly from both @a pause and @a live modes. In @a
       * playback mode, data from the source will be retrieved
       * through its ImPlaybackSourceToolkit interface from the
       * specified @p timeStamp and rendered using the specified @p
       * speed. If the source does not implement the
       * ImPlaybackSourceToolkit interface, it will enter the @ref
       * PlaybackNotSupported state.
       *
       * A speed of 1.0 means forward playback in realtime as it was
       * recorded. Likewise a speed of 0.5 and 2.0 means respecitively
       * forward playback of half and double speed relatively to how
       * it was recorded. Backwards playback is achieved using
       * negative values. A speed of 0.0 will pause the playback but
       * the mode of the source is still @a playback. The difference
       * between this and the @a pause mode (triggered when using one
       * of the navigation methods like MoveTo()) is that all render
       * bufferes are still kept open. If the speed is later changed,
       * these can be reused and playback can start instantly without
       * first building up a buffer.
       *
       * When @a playback mode is entered, the @p timeStamp specifies
       * where to start the playback from. If already in @a playback
       * mode, the @p timeStamp is interpreted as a synchronization
       * hint to the currently ongoing playback. This can be used to
       * make sure several sources are synchronized to one master
       * clock. Also if already in @a playback mode, the @p speed
       * parameter can be used to change the speed of the currently
       * ongoing playback without first stopping the playback.
       *
       * @param sourceToDoPlayback Id of source for which to enter playback mode / synchronize playback.
       * @param timeStamp Time stamp of where to start playback / synchronize playback.
       * @param speed Speed of playback. A value of 1.0 means forward playback in the same speed as it was recorded.
       *
       * @exception ImInvalidArgumentError Is thrown if specified source id @p sourceToDoPlayback is unknown.
       *
       * @see MoveFirst(), MoveLast(), MoveNext(), MovePrevious(), MoveNextSequence(), MovePreviousSequence(), MoveTo(), DoLive()
       */
      virtual void DoPlayback(source_id_t sourceToDoPlayback, utc_time_t timeStamp, playback_speed_t speed)=0;

      /**
       * @brief Enter live mode of source.
       *
       * This method will make the specified source enter the live
       * mode. The @a live mode can be entered directly from both @a
       * pause and @a playback modes. In @a live mode, data from the
       * source will be retrieved through its ImLiveSourceToolkit
       * interface and rendered in realtime. If the source does not
       * implement the ImLiveSourceToolkit interface, it will enter
       * the @ref LiveNotSupported state.
       *
       * @param sourceToDoLive Id of source for which to enter live mode.
       * @param maxFrameRate Maximum number of frames per second to be delivered.
       *                     If set to zero playback is paused.
       *                     If set to -1 Playback is capped at the current monitor refresh rate.
       *
       * @exception ImInvalidArgumentError Is thrown if specified source id @p sourceToDoLive is unknown.
       *
       * @see MoveFirst(), MoveLast(), MoveNext(), MovePrevious(), MoveNextSequence(), MovePreviousSequence(), MoveTo(), DoPlayback()
       */
      virtual void DoLive(source_id_t sourceToDoLive, int maxFrameRate) = 0;
      //@}

      /** @name Handler methods */ //@{
      /**
       * @brief Set instance of rendering handler.
       *
       * The purpose of the rendering handler is to provide a call-back
       * interface through which information can be given about
       * rendered data.
       *
       * By default there is no rendering handler set and thus no
       * call-backs are made before this has been set.
       *
       * @param renderingHandler Rendering handler instance to use. Use null, if a handler is not needed.
       *
       * @see ImRenderingHandler, ImRenderingHandler::HandleRenderedData()
       */
      virtual void SetRenderingHandler(ImRenderingHandler *renderingHandler)=0;

      /**
       * @brief Set instance of out-of-band handler.
       *
       * The purpose of the out-of-band handler is to provide a
       * call-back interface through which out-of-band data received
       * from the sources can be provided.
       *
       * By default there is no out-of-band handler set and thus no
       * call-backs are made before this has been set.
       *
       * @param outOfBandHandler Out-of-band handler instance to use. Use null, if handler is not needed.
       *
       * @see ImOutOfBandHandler, ImOutOfBandHandler::HandleOutOfBandData()
       */
      virtual void SetOutOfBandHandler(ImOutOfBandHandler *outOfBandHandler)=0;

      /**
       * @brief Set instance of source state handler.
       *
       * The purpose of the source state handler is to provide a
       * call-back interface through which changes in source state can
       * be communicated.
       *
       * By default there is no source state handler set and thus no
       * call-backs are made before this has been set.
       *
       * @param sourceStateHandler Source state handler instance to use. Use null, if handler is not needed.
       *
       * @see ImSourceStateHandler, ImSourceStateHandler::HandleSourceStateChanged()
       */
      virtual void SetSourceStateHandler(ImSourceStateHandler *sourceStateHandler)=0;

      /**
       * @brief Set instance of diagnostics handler.
       *
       * The purpose of the diagnostics handler is to provide a
       * call-back interface through which information about system perfoamance and operation can
       * be communicated.
       *
       * By default there is no diagnostics handler set and thus no
       * call-backs are made before this has been set.
       *
       * @param diagnosticsHandler Diagnostics handler instance to use. Use null, if handler is not needed.
       *
       * @see ImDiagnosticsHandler, ImDiagnosticsHandler::HandleDiagnosticsData()
       */
      virtual void SetDiagnosticsHandler(ImDiagnosticsHandler *diagnosticsHandler) = 0;

      class ImDiagnosticsKeyInfo
      {
      public:
        enum class ePerformanceCounterType
        {
          NotPerformanceCounted,
          RateOfCountsPerSecond,
          NumberOfItems
        };

        virtual ePerformanceCounterType GetPerformanceCounterType() const = 0;
        virtual std::string GetPerformanceCounterDescription() const = 0;

        /**
         * Return the display level. 
         * @return Display level (0 means not displayed.)
         */
        virtual int GetDisplayLevel() const = 0;
      };

      /**
       * @brief Get a list of diagnostics values associated with descriptive information.
       */
      virtual std::map<const std::string, const ImRendererToolkit::ImDiagnosticsKeyInfo *> &GetDiagnosticsKeyInfo() const = 0;

      /**
      * @brief Call Shutdown to stop all threads.
      *
      *
      */
      virtual void Shutdown() = 0;

      //@}
  };

}

#endif // Toolkits_RendererToolkitInterface_h_INCLUDED
