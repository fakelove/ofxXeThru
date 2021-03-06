#ifndef X4M200_INTERFACE_H
#define X4M200_INTERFACE_H

#include "NotSupportedData.hpp"
#include "Data.hpp"
#include "Bytes.hpp"
#include "LockedRadarForward.hpp"

#include <cinttypes>
#include <string>
#include <vector>
#include <memory>

struct RadarInterface;

namespace XeThru {

class X4M200Private;

/**
 * @class X4M200
 *
 * Interface to the Xethru X4M200 Application module.
 * This interface is used to control the XeThru X4M200 module.
 * See the data sheet of the X4M200 module for more information on the X4M200 module.
 *
 */
class X4M200
{
public:
    /**
     * Constructor
     *
     * @param[in] radar_interface internal object used to interface the radar
     */
    X4M200(LockedRadarInterfacePtr & radar_interface);

    /**
     * Destructor
     *
     */
    ~X4M200();


    /**
     * Sets debug level in the Xethru module.
     *
     * @param[in]  level New debug level. Valid range [0-9].
     * @return status : success in case of 0 / failure in any other case
     */
    int set_debug_level(unsigned char level);

    /**
     * Set baudrate for serial communication during ModuleConnector operation.
     *
     * @param baudrate: enum representing the baudrate, XTID_BAUDRATE_XXX in xtid.h.
     * @return status : success in case of 0 / failure in any other case
     */
    int set_baudrate(uint32_t baudrate);

    /**
     * Make sure there is a connection to firmware on the Xethru X4M300  module

     * @param[out] pong value: 0xaaeeaeea means system ready and 0xaeeaeeaa means system not ready.
     *
     * @return status : success in case of 0 / failure in any other case
     */
    int ping(uint32_t *pong_value);

    /**
     * Returns a string containing system information given by infocode:<br>
     * @param[in] info_code: what information to query. Possible values are?<br>
     *  XTID_SSIC_ITEMNUMBER -> Returns the product Item Number, including revision.\  This is programmed in Flash during manufacturing<br>
     *  XTID_SSIC_ORDERCODE -> Returns the product order code<br>
     *  XTID_SSIC_FIRMWAREID -> Returns the installed Firmware ID.\  As viewed from the "highest" level of the software, "X4M200"<br>
     *  XTID_SSIC_VERSION  -> Returns the installed Firmware Version.\  As viewed from the "highest" level of the software<br>
     *  XTID_SSIC_BUILD -> Returns information of the SW Build installed on the device<br>
     *  XTID_SSIC_SERIALNUMBER -> Returns the product serial number<br>
     *  XTID_SSIC_VERSIONLIST ->Returns ID and version of all components.\ Calls all components and compound a string E.g.\  "X4M300:1.0.0.3;XEP:2.3.4.5;X4C51:1.0.0.0;DSP:1.1.1.1"
     * @return status : success in case of 0 / failure in any other case
     */
    int get_system_info(const uint8_t info_code, std::string *system_info);

    /**
     * Resets and restart the module.
     * This is a convenience method that calls module_reset(), disconnects the communication port, and then reestablishes connection with the module.
     *
     * @return status : success in case of 0 / failure in any other case
     */
    int reset();

    /**
     * Resets the module.
     * The client must perform a close and then an open on the ModuleConnector to reeastablish connection.
     *
     * @return status : success in case of 0 / failure in any other case
     */
    int module_reset();

    /**
     * Resets all parameters in the module to factory presets.
     *
     * @return status : success in case of 0 / failure in any other case
     */
    int reset_to_factory_preset();

    /**
     * Enters the bootloader for FW upgrades.
     *
     * @return status : success in case of 0 / failure in any other case
     */
    int start_bootloader();

    /**
     * Runs the different manufacturing tests identified by testcode. Can return any number of results depending on test_mode. Host must know how to parse test results.
     *
     * @param[in] testcode:
     * @param[out] data: data buffer containing the result from a test run.
     * @return status : success in case of 0 / failure in any other case
     */
    int system_run_test(const uint8_t testcode, Bytes * data);

    /**
     * Loads the profile given by profileid. If another profile is already loaded, the other profile is unloaded before the new profile is loaded.
     * The profile does not start, the module remains idle. <br>
     *
     * @param[in] profileid: the id of the profile to load
     * profileid = XTS_ID_APP_RESPIRATION_2 (xtid.h) : sleep profile
     * @return status : success in case of 0 / failure in any other case
     */
    int load_profile(const uint32_t profileid);

    /**
     * Control the execution mode of the sensor.
     *
     * @param[in] mode: see xtid.h for profileid values.<br>
     * XTID_SM_RUN  : Start profile execution<br>
     * XTID_SM_IDLE : Halts profile execution. Can be resumed by setting mode to Run.<br>
     * XTID_SM_STOP : Stops profile execution. Must do load_profile to continue.<br>
     * XTID_SM_MANUAL : Routes X4 radar data directly to host rather than to profile execution. Can then interact directly with XEP / X4Driver. Will disrupt profile performance.<br>
     * @param[in] param: Not used, ignored, can be 0.
     * @return status : success in case of 0 / failure in any other case
     */
    int set_sensor_mode(const uint8_t mode, const uint8_t param);

    /**
     * Sets the overall sensitivity
     *
     * @param[in] sensitivity : 0 to 9, 0 = low, 9 = high
     * @return status : success in case of 0 / failure in any other case
     */
    int set_sensitivity(const uint32_t sensitivity);

    /**
     * Sets the current detection zone. Rules: See X4M200 datasheet.
     * The actual detection zone is determined by profile configuration. Use the get_detection_zone command to get the actual values
     *
     * @param[in] start: start of detection zone in meters
     * @param[in] end: end of detection zone in meters
     * @return status : success in case of 0 / failure in any other case
     */
    int set_detection_zone(const float start, const float end);
    /**
     * Returns the actual detection zone.
     *
     * @param[out] start: start of detection zone in meters
     * @param[out] end: end of detection zone in meters
     * @return status : success in case of 0 / failure in any other case
     */
    int get_detection_zone(float * start, float * end);

    /**
     * Returns the potential settings of detection zone from the module.
     *
     * @param[out] min: smallest value for detection zone start
     * @param[out] max: largest value for detection zone end
     * @param[out] step: detection zone start and end step size
     * @return status : success in case of 0 / failure in any other case
     */
    int get_detection_zone_limits(float * min, float * max, float * step);

    /**
     * This command configures the LED mode.<br>
     *
     * @param[in] mode: (modes are defined in xtid.h) <br>
     * mode = XTID_LED_MODE_OFF : OFF <br>
     * mode = XTID_LED_MODE_SIMPLE : simple <br>
     * mode = XTID_LED_MODE_FULL : full (default) <br>
     * @param[in] intensity: 0 to 100, 0=low, 100=high, not implemented yet
     * @return status : success in case of 0 / failure in any other case
     */
    int set_led_control(const uint8_t mode, uint8_t intensity);

    /**
     * Control module profile output. Enable and disable data messages. Several
     * calls can be made, one for each available output message the profile
     * provides.
     *
     * Only one of XTS_ID_BASEBAND_IQ and XTS_ID_BASEBAND_AMPLITUDE_PHASE can
     * be enabled at a time. Enabling one disables the other. Disabling one,
     * even if it is already disabled, disables the other.
     *
     * Only one of XTS_ID_PULSEDOPPLER_FLOAT and XTS_ID_PULSEDOPPLER_BYTE can
     * be enabled for each of XTID_OUTPUT_CONTROL_PD_SLOW_ENABLE and
     * XTID_OUTPUT_CONTROL_PD_FAST_ENABLE. Same with XTS_ID_NOISEMAP_FLOAT and
     * XTS_ID_NOISEMAP_BYTE. Turning on a float output automatically disables
     * the byte output, and vice versa.
     *
     * @param[in] output_feature: see values in xtid.h.<br>
     * Possible features are:<br>
     * XTS_ID_RESPIRATION_MOVINGLIST<br>
     * XTS_ID_RESPIRATION_DETECTIONLIST<br>
     * XTS_ID_RESP_STATUS<br>
     * XTS_ID_RESP_STATUS_EXT<br>
     * XTS_ID_BASEBAND_IQ<br>
     * XTS_ID_BASEBAND_AMPLITUDE_PHASE<br>
     * XTS_ID_PULSEDOPPLER_FLOAT<br>
     * XTS_ID_PULSEDOPPLER_BYTE<br>
     * XTS_ID_NOISEMAP_FLOAT<br>
     * XTS_ID_NOISEMAP_BYTE<br>
     * @param[in] output_control: see values in xtid.h<br>
     * Typical values:<br>
     * XTID_OUTPUT_CONTROL_DISABLE = disable<br>
     * XTID_OUTPUT_CONTROL_ENABLE = enable<br>
     * <br>
     * For pulse-Doppler and noisemap byte/float:<br>
     * XTID_OUTPUT_CONTROL_PD_SLOW_ENABLE<br>
     * XTID_OUTPUT_CONTROL_PD_FAST_ENABLE<br>
     *
     * @return status : success in case of 0 / failure in any other case
     */
    int set_output_control(const uint32_t output_feature, const uint32_t output_control);


    /**
     * Return number of BasebandApData messages available in queue.
     *
     * @return size: number of messages in buffer
     */
    int peek_message_baseband_ap();

    /**
     * Read a single BasebandApData item from the queue. Blocks if queue is empty.
     *
     * @param[out] baseband_ap: pointer to returned BasebandApData item
     * @return status : success in case of 0 / failure in any other case
     */
    int read_message_baseband_ap(BasebandApData * baseband_ap);

    /**
     * Return number of BasebandIqData messages available in queue.
     *
     * @return size: number of messages in buffer
     */
    int peek_message_baseband_iq();

    /**
     * Read a single BasebandIqData item from the queue. Blocks if queue is empty.
     *
     * @param[out] baseband_iq: pointer to returned BasebandIqData item
     * @return status : success in case of 0 / failure in any other case
     */
    int read_message_baseband_iq(BasebandIqData * baseband_iq);

    /**
     * Return number of legacy respiration packets available in the queue.
     *
     * @return size: number of messages in buffer
     */
    int peek_message_respiration_legacy();

    /**
     * Read a single RespirationData item from the queue. Blocks if queue is empty.
     *
     * @param[out] respiration_data: pointer to returned RespirationData item
     * @return status : success in case of 0 / failure in any other case
     */
    int read_message_respiration_legacy(RespirationData * respiration_data);

    /**
     * Return number of respiration sleep packets available in the queue.
     *
     * @return size: number of messages in queue
     */
    int peek_message_respiration_sleep();

    /**
     * Get one sleep data message from subscription queue.
     *
     * @param[out]  sleep_data A data object holding the resulting
     *                         respiration data.
     *
     * @return status : success in case of 0 / failure in any other case
     */
    int read_message_respiration_sleep(SleepData * sleep_data);

    /**
     * Return number of respiration moving list packets available in the queue.
     *
     * @return size: number of messages in queue
     */
    int peek_message_respiration_movinglist();

    /**
     * Get one respiration moving list data message from subscription queue.
     *
     * @param[out] data A data object holding the resulting respiration moving listdata.
     *
     * @return status : success in case of 0 / failure in any other case
     */
    int read_message_respiration_movinglist(RespirationMovingListData *data);

    /**
     * Return number of respiration detection list packets available in the queue.
     *
     * @return size: number of messages in queue
     */
    int peek_message_respiration_detectionlist();

    /**
     * Get one respiration detection list data message from subscription queue.
     *
     * @param[out] data A data object holding the resulting respiration detection listdata.
     *
     * @return status : success in case of 0 / failure in any other case
     */
    int read_message_respiration_detectionlist(RespirationDetectionListData *data);

    /**
     * Return number of pulse-Doppler float packets available in the queue.
     *
     * @return size: number of messages in queue
     */
    int peek_message_pulsedoppler_float();

    /**
     * Get one pulse-Doppler float data message from subscription queue.
     *
     * @return : A data object holding the resulting pulse-Doppler float data
     */
    int read_message_pulsedoppler_float(PulseDopplerFloatData *data);

    /**
     * Return number of pulse-Doppler byte packets available in the queue.
     *
     * @return size: number of messages in queue
     */
    int peek_message_pulsedoppler_byte();

    /**
     * Get one pulse-Doppler byte data message from subscription queue.
     *
     * @return : A data object holding the resulting pulse-Doppler byte data
     */
    int read_message_pulsedoppler_byte(PulseDopplerByteData *data);

    /**
     * Return number of noisemap float packets available in the queue.
     *
     * @return size: number of messages in queue
     */
    int peek_message_noisemap_float();

    /**
     * Get one noisemap float data message from subscription queue.
     *
     * @return : A data object holding the resulting noisemap float data
     */
    int read_message_noisemap_float(PulseDopplerFloatData *data);

    /**
     * Return number of noisemap byte packets available in the queue.
     *
     * @return size: number of messages in queue
     */
    int peek_message_noisemap_byte();

    /**
     * Get one noisemap byte data message from subscription queue.
     *
     * @return : A data object holding the resulting noisemap byte data
     */
    int read_message_noisemap_byte(PulseDopplerByteData *data);

    /**
     * Send command to module to load a previously stored noisemap
     *
     * Not yet functional as of FW 1.3
     *
     * @return status : success in case of 0 / failure in any other case
     */
    int load_noisemap();

    /**
     * Send command to module to store the current noisemap to module flash.
     *
     * Fails if a store already is active, for example during the first
     * initialize with XTID_NOISEMAP_CONTROL_INIT_ON_RESET disabled.
     *
     * @return status : success in case of 0 / failure in any other case
     */
    int store_noisemap();

    /**
     * Configure the use of noisemap.
     *
     * @param[in] noisemap_control A bitfield of the various features.
     * <br>
     * XTID_NOISEMAP_CONTROL_ENABLE -> Bit 0, turn on use of noisemap.<br>
     * XTID_NOISEMAP_CONTROL_ADAPTIVE -> Bit 1, adapt noisemap over time.<br>
     * XTID_NOISEMAP_CONTROL_INIT_ON_RESET -> Bit 2, don't store but initialize
     * the noisemap every time. Bit 2 unset means load the stored noisemap if
     * present, and initialize and store it if not present.<br>
     * @return status : success in case of 0 / failure in any other case
     */
    int set_noisemap_control(uint32_t noisemap_control);

    /**
     * Get current noisemap configuration.
     *
     * @param[out] noisemap_control A bitfield of the various features.
     * @return status : success in case of 0 / failure in any other case
     */
    int get_noisemap_control(uint32_t *noisemap_control);

    /**
     * Read the complete parameter file from the module
     * @param[out] data: pointer to a string containing the complete paramter file data.
     *
     * @return status : success in case of 0 / failure in any other case
     */
    int get_parameter_file(const std::string & filename, std::string * const data);

    /**
     * Get the id of the currently loaded profile.
     * @param[out] result: pointer to the resulting profile_id. 0 in case of no profile loaded.
     *
     * @return status : success in case of 0 / failure in any other case
     */
    int get_profileid(uint32_t * result);

    /**
     * Set a named parameter file on target.
     *
     * @param[in] filename: The name to call the parameter file.
     * @param[in] data: The content of the parameter-file.
     *
     * @return status : success in case of 0 / failure in any other case
     */
    int set_parameter_file(
        const std::string * filename,
        const std::string * data);


private:
    std::unique_ptr<X4M200Private> d_ptr;
};

} // namespace Novelda

#endif // X4M200_INTERFACE_H
