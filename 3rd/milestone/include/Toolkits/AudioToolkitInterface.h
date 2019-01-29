/**
 * @file
 *
 * @brief Header file containing definitions of audio toolkit interfaces.
 */

#if !defined(Toolkits_AudioToolkitInterface_h_INCLUDED)
#define Toolkits_AudioToolkitInterface_h_INCLUDED

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
  /**
  * @interface ImAudioToolkit
  * @brief Interface for retrieving audio data from a source toolkits.
  */
  class ImAudioToolkit : public ImToolkit
  {
  public:
    /**
    * @brief Enumeration of possible states.
    *
    * @see ImStateHandler::HandleStateChanged()
    */
    enum state_t
    {
      Disconnected,         ///< Source is currently disconnected. Will not leave this state until actively connected using the Connect() method.
      Connecting,           ///< Source is currently attempting to establish a connection. When established, the source will enter the @ref Connected state.
      Connected,            ///< Source is currently connected. Data is now ready to be recieved from the source.
      ConnectionFailed,     ///< Connection could not be established to source; will retry connecting very shortly and enter the @ref Connecting state.
      ConnectionLost,       ///< Source has lost its connection and will retry connecting very shortly and enter the @ref Reconnecting state.
      Reconnecting          ///< Source has lost its connection and is now attempting to restore the connection. Once restored, the state of the source will be set to @ref Connected.
    };

    /**
    * @brief Enumeration of defined flags.
    *
    * Each flag is represented by a bit in the flags integer
    * provided in @ref ImStateHandler::HandleStateChanged() "HandleStateChanged()"
    *
    * @see ImStateHandler::HandleStateChanged()
    */
    enum flags_t
    {
      LiveNotSupported = 1 << 0, ///< Bit 0; If set, source does not implement the ImLiveSourceToolkit interface.
      PlaybackNotSupported = 1 << 1, ///< Bit 1; If set, source does not implement the ImPlaybackSourceToolkit interface.
      NotAuthorized = 1 << 2, ///< Bit 2; If set, source has reported that it was not authorized to get data.
      LicenseInvalid = 1 << 3, ///< Bit 3; If set, source has reported that data cannot be retrieved because of an invalid license.
      MaxNumberOfConnectionsReached = 1 << 4 ///< Bit 4; If set, source has reported that data cannot be retrieved because the maximum number of connections has been reached.
    };

    /**
    * @interface ImAudioInformation
    * @brief Raw aduio data and information about the data.
    *
    * @see HandleAudioData()
    */
    class ImAudioInformation
    {
    public:
      /** @name Constructors and destructors */ //@{
                                                /**
                                                * @brief Virtual toolkit destructor to ensure proper destruction through interface.
                                                */
      virtual ~ImAudioInformation() throw() {}
      //@}

      virtual const byte_t * RawData() const = 0;

      virtual size_t NumberOfChannels() const = 0;
      virtual size_t SamplingRate() const = 0;
      virtual size_t BitDepth() const = 0;
      virtual size_t DataSize() const = 0;
      virtual std::string Codec() const = 0;
    };


    /**
    * @interface ImAudioHandler
    * @brief Interface with call-back method for handling audio data.
    *
    * @see SetAudioHandler()
    */
    class ImAudioHandler
    {
    public:
      /** @name Constructors and destructors */ //@{
                                                /**
                                                * @brief Virtual toolkit destructor to ensure proper destruction through interface.
                                                */
      virtual ~ImAudioHandler() throw() {}
      //@}

      /** @name Callback methods */ //@{
                                    /**
                                    * @brief Method that is called every time data has been retrieved for a source.
                                    *
                                    * No lenghty operations should be performed in this
                                    * call-back method. Also no exceptions must leave the
                                    * call-back method.
                                    *
                                    * @param audioInformation Audio information for the source.
                                    */
      virtual void HandleAudioData(const std::shared_ptr<const NmToolkit::ImAudioToolkit::ImAudioInformation> &audioInformation) = 0;
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
                                    * @brief Method that is called every time out-of-band data is received from the source.
                                    *
                                    * No lenghty operations should be performed in this
                                    * call-back method. Also no exceptions must leave the
                                    * call-back method.
                                    *
                                    * @param outOfBandData Out-of-band data received.
                                    */
      virtual void HandleOutOfBandData(NmToolkit::ImOutOfBandData *outOfBandData) = 0;
      //@}
    };

    /**
    * @interface ImStateHandler
    * @brief Interface with method handling state changes.
    *
    * @see SetStateHandler()
    */
    class ImStateHandler
    {
    public:
      /** @name Constructors and destructors */ //@{
                                                /**
                                                * @brief Virtual toolkit destructor to ensure proper destruction through interface.
                                                */
      virtual ~ImStateHandler() throw() {}
      //@}

      /** @name Callback methods */ //@{
                                    /**
                                    * @brief Method that is called every time the state has changed.
                                    *
                                    * No lenghty operations should be performed in this
                                    * call-back method. Also no exceptions must leave the
                                    * call-back method.
                                    *
                                    * Note that this method is likely to be called from source
                                    * specific threads and that there is no synchronization
                                    * between these.
                                    *
                                    * @param state New state of source.
                                    * @param flags Flags represented as individual bits (see @ref flags_t)
                                    */
      virtual void HandleStateChanged(NmToolkit::ImAudioToolkit::state_t state, uint32_t flags) = 0;
      //@}
    };

  public:
    /**
    * @brief Update single parameter in configuration.
    *
    * This method updates
    * the parameter with name @p configurationParameterName to the
    * value of @p configurationParameterValue. If the given
    * parameter name is not applicable for the audio toolkit, or the used
    * source toolkit, or if the parameter was not configured to be
    * updateable, the method does nothing.
    *
    * If the configuration parameter value specified cannot be parsed to
    * the type matching what is expected by the parameter to
    * update, then an exception is also thrown.
    *
    * Consult the documentation of the specific source toolkit
    * used by audio toolkit to learn which parameters can be used, when they can be
    * used and what their legal values are.
    *
    * @param configurationParameterName Name of configuration parameter to update.
    * @param configurationParameterValue Value of configuration parameter to apply as a string.
    *
    * @exception ImInvalidArgumentError If @p configurationParameterValue could not be parsed to the correct type.
    */
    virtual void UpdateConfiguration(const utf8_string_t &configurationParameterName, const utf8_string_t &configurationParameterValue) = 0;

    /**
    * @brief Update list of parameters in configuration.
    *
    * This method works like the other update configuration method
    * except it can update a list of parameters in one atomic call.
    *
    * @param configurationParameterList List of parameter name and value pairs representing the parameters to be updated.
    *
    * @exception ImInvalidArgumentError If one of the specified configuration parameter values could not be parsed to the correct type.
    */
    virtual void UpdateConfiguration(const std::list<std::pair<utf8_string_t, utf8_string_t> > &configurationParameterList) = 0;


    /**
    * @brief Connect the source toolkit.
    *
    * This method will connect the source toolkit to
    * the endpoint and will enter the @ref
    * Connecting state.
    *
    * Connecting when it is already connected is not an error
    * and does nothing.
    *
    * @see Disconnect(), SetStateHandler()
    */
    virtual void Connect() = 0;

    /**
    * @brief Disconnect the sourcde toolkit.
    *
    * This method will disconnect the source toolkit from the
    * endpoints and the will enter the @ref
    * Disconnected state.
    *
    * Disconnecting when it is already disconnected is not an error
    * and does nothing.
    *
    * @see Connect(), SetStateHandler()
    */
    virtual void Disconnect() = 0;
    //@}


    /**
    * @brief Enter playback mode / synchronize playback.
    *
    * This method will make the audio toolkit enter @a playback
    * mode if not already in that mode. The @a playback mode can be
    * entered directly from @a live mode. In @a
    * playback mode, audio data from the source will be retrieved
    * through its ImPlaybackSourceToolkit interface from the
    * specified @p timeStamp. If the source does not implement the
    * ImPlaybackSourceToolkit interface, it will enter the @ref
    * PlaybackNotSupported state.
    *
    * When @a playback mode is entered, the @p timeStamp specifies
    * where to start the playback from. If already in @a playback
    * mode, the @p timeStamp is interpreted as a synchronization
    * hint to the currently ongoing playback. This can be used to
    * make sure tha audio is synchronized to the associated video.
    *
    * @param timeStamp Time stamp of where to start playback / synchronize playback.
    *
    * @see DoLive()
    */
    virtual void DoPlayback(utc_time_t timeStamp) = 0;

    /**
    * @brief Enter live mode.
    *
    * This method will make the audio toolkit enter the live
    * mode. The @a live mode can be entered directly from both @a
    * @a playback mode. In @a live mode, audio data from the
    * source will be retrieved through its ImLiveSourceToolkit
    * interface in realtime. If the source does not
    * implement the ImLiveSourceToolkit interface, it will enter
    * the @ref LiveNotSupported state.
    *
    * @see DoPlayback()
    */
    virtual void DoLive() = 0;
    //@}

    /** @name Handler methods */ //@{
                                 /**
                                 * @brief Set instance of audio handler.
                                 *
                                 * The purpose of the audio handler is to provide a call-back
                                 * interface through which audio data and information can be given about
                                 * the audio data.
                                 *
                                 * By default there is no audio handler set and thus no
                                 * call-backs are made before this has been set.
                                 *
                                 * @param audioHandler Audio handler instance to use.
                                 *
                                 * @see ImAudioHandler, ImAudioHandler::HandleAudioData()
                                 */
    virtual void SetAudioHandler(std::shared_ptr<ImAudioHandler> audioHandler) = 0;

    /**
    * @brief Set instance of out-of-band handler.
    *
    * The purpose of the out-of-band handler is to provide a
    * call-back interface through which out-of-band data received
    * can be provided.
    *
    * By default there is no out-of-band handler set and thus no
    * call-backs are made before this has been set.
    *
    * @param outOfBandHandler Out-of-band handler instance to use. Use null, if handler is not needed.
    *
    * @see ImOutOfBandHandler, ImOutOfBandHandler::HandleOutOfBandData()
    */
    virtual void SetOutOfBandHandler(std::shared_ptr<ImOutOfBandHandler> outOfBandHandler) = 0;

    /**
    * @brief Set instance of state handler.
    *
    * The purpose of the state handler is to provide a
    * call-back interface through which changes in state can
    * be communicated.
    *
    * By default there is no state handler set and thus no
    * call-backs are made before this has been set.
    *
    * @param stateHandler state handler instance to use. Use null, if handler is not needed.
    *
    * @see ImStateHandler, ImStateHandler::HandleStateChanged()
    */
    virtual void SetStateHandler(std::shared_ptr<ImStateHandler> stateHandler) = 0;

    //@}
  };

}
#endif // Toolkits_AudioToolkitInterface_h_INCLUDED
