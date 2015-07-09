//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2009-2011
// European Synchrotron Radiation Facility
// BP 220, Grenoble 38043
// FRANCE
//
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//###########################################################################

#ifndef MERLINCAMERA_H_
#define MERLINCAMERA_H_

#include <ostream>
#include <map>
#include "lima/HwMaxImageSizeCallback.h"
#include "lima/HwBufferMgr.h"
#include "lima/HwInterface.h"
#include "lima/Debug.h"
#include "MerlinNet.h"

using namespace std;

namespace lima {
namespace Merlin {

const static string actions[] = {"GET","SET","CMD"};
const static string id = "MPX,";
const int xPixelSize = 55; // pixel size is 55 micron
const int yPixelSize = 55;

class BufferCtrlObj;

/*******************************************************************
 * \class Camera
 * \brief object controlling the Merlin camera
 *******************************************************************/
class Camera : public HwMaxImageSizeCallbackGen {
DEB_CLASS_NAMESPC(DebModCamera, "Camera", "Merlin");

public:

	Camera(std::string& hostname, int cmdPort = 6341, int dataPort = 6342,
	       int npixels = 512, int nrasters = 512, int nchips = 4, bool simulate=false);
	~Camera();
	
	enum DetectorStatus {
	  IDLE,     ///< Detector idle
	  BUSY,     ///< Detector Busy
	  STANDBY,  ///< Detecotr in standby
	};
	enum ColourMode {Monochrome, Colour};
	enum Switch {OFF, ON};
	enum GainSetting {SLGM, LGM, HGM, SHGM};
	enum Counter {COUNTER0, COUNTER1, BOTH};
	enum Depth {BPP1=1, BPP6=6, BPP12=12, BPP24=24};
	enum Trigger {INTERNAL, RISING_EDGE_TTL, FALLING_EDGE_TTL, RISING_EDGE_LVDS, FALLING_EDGE_LVDS, SOFT};
	enum TriggerOutput {TTL, LVDS, TTL_DELAYED, LVDS_DELAYED, FOLLOW_SHUTTER, ONE_PER_ACQ_BURST,
			   SHUTTER_AND_SENSOR_READ, OUTPUT_BUSY};
	enum TriggerLevel {NORMAL, INVERTED};
	enum Threshold {THRESHOLD0, THRESHOLD1, THRESHOLD2, THRESHOLD3, THRESHOLD4,
		THRESHOLD5, THRESHOLD6, THRESHOLD7};
	enum FillMode { NOFILL, INSERTZERO, DISTRIBUTE, INTERPOLATE };

	void init();
	void reset();
	void prepareAcq();
	void startAcq();
	void stopAcq();
	void getStatus(DetectorStatus& status);
	int getNbHwAcquiredFrames();

	// -- detector info object
	void getImageType(ImageType& type);
	void setImageType(ImageType type);

	void getDetectorType(std::string& type);
	void getDetectorModel(std::string& model);
	void getDetectorImageSize(Size& size);
	void getPixelSize(double& sizex, double& sizey);

	// -- Buffer control object
	HwBufferCtrlObj* getBufferCtrlObj();

	//-- Synch control object
	void setTrigMode(TrigMode mode);
	void getTrigMode(TrigMode& mode);

	void setExpTime(double exp_time);
	void getExpTime(double& exp_time);

	void setLatTime(double lat_time);
	void getLatTime(double& lat_time);

	void getExposureTimeRange(double& min_expo, double& max_expo) const;
	void getLatTimeRange(double& min_lat, double& max_lat) const;

	void setNbFrames(int nb_frames);
	void getNbFrames(int& nb_frames);

	bool isAcqRunning() const;

	///////////////////////////////
	// -- merlin specific functions
	///////////////////////////////

	void startAcquisition();
	void stopAcquisition();
	void softTrigger();
	void abort();
	void thscan();
	void resetHw();

	void setAcquisitionPeriod(float millisec);
	void getAcquisitionPeriod(float &millisec);
	void setAcquisitionTime(float millisec);
	void getAcquisitionTime(float &millisec);
	void getSoftwareVersion(float &version);
	void setChargeSumming(Switch mode);
	void getChargeSumming(Switch &mode);
	void setColourMode(ColourMode mode);
	void getColourMode(ColourMode &mode);
	void setContinuousRW(Switch mode);
	void getContinuousRW(Switch &mode);
	void setCounterDepth(Depth depth);
	void getCounterDepth(Depth &depth);
	void setEnableCounters(Counter counter);
	void getEnableCounters(Counter &counter);
	void setFramesPerTrigger(int frames);
	void getFramesPerTrigger(int &frames);
	void setGain(GainSetting gain);
	void getGain(GainSetting &gain);
	void setNumFramesToAcquire(int frames);
	void getNumFramesToAcquire(int &frames);
	void setOperatingEnergy(float kev);
	void getOperatingEnergy(float &kev);
	void getTemperature(float &temp);
	void setThreshold(Threshold threshold, float kev);
	void getThreshold(Threshold threshold, float &kev);
	void setTriggerStartType(Trigger type);
	void getTriggerStartType(Trigger &type);
	void setTriggerStopType(Trigger type);
	void getTriggerStopType(Trigger &type);
	void getTriggerOutTTL(TriggerOutput &trigOut);
	void setTriggerOutTTL(TriggerOutput trigOut);
	void getTriggerOutLVDS(TriggerOutput &trigOut);
	void setTriggerOutLVDS(TriggerOutput trigOut);
	void getTriggerOutTTLInvert(TriggerLevel &trigLevel);
	void setTriggerOutTTLInvert(TriggerLevel trigLevel);
	void getTriggerOutLVDSInvert(TriggerLevel &trigLevel);
	void setTriggerOutLVDSInvert(TriggerLevel trigLevel);
	void getTriggerInTTLDelay(long long &delay);
	void setTriggerInTTLDelay(long long delay);
	void getTriggerInLVDSDelay(long long &delay);
	void setTriggerInLVDSDelay(long long delay);
	void getTriggerUseDelay(Switch &mode);
	void setTriggerUseDelay(Switch mode);
	void setTHScanNum(int num);
	void getTHScanNum(int &num);
	void setTHStart(float kev);
	void getTHStart(float &kev);
	void setTHStop(float kev);
	void getTHStop(float &kev);
	void setTHStep(float kev);
	void getTHStep(float &kev);
	void setFileDirectory(string directory);
	void getFileDirectory(string &directory);
	void setFileName(string filename);
	void getFileName(string &filename);
	void setFileEnable(Switch mode);
	void getFileEnable(Switch &mode);
	void getDetectorStatus(DetectorStatus &status);
	void getImageX(int &imagex);
	void getImageY(int &imagey);
	void getFillMode(FillMode &mode);
	void setFillMode(FillMode mode);

private:

	//////////////////////////////
	// merlin specific definitions
	//////////////////////////////

	enum Action {GET, SET, CMD};
	enum ActionCmd {
	  SOFTWAREVERSION,
		ACQUISITIONPERIOD,
		STARTACQUISITION,
		STOPACQUISITION,
		SOFTTRIGGER,
		ABORT,
		THSCAN,
		RESET,
		ACQUISITIONTIME,
		CHARGESUMMING,
		COLOURMODE,
		CONTINUOUSRW,
		COUNTERDEPTH,
		ENABLECOUNTER1,
		NUMFRAMESPERTRIGGER,
		GAIN,
		NUMFRAMESTOACQUIRE,
		OPERATINGENERGY,
		TEMPERATURE,
		THRESHOLD_0,
		THRESHOLD_1,
		THRESHOLD_2,
		THRESHOLD_3,
		THRESHOLD_4,
		THRESHOLD_5,
		THRESHOLD_6,
		THRESHOLD_7,
		THSCANNUM,
		THSTART,
		THSTEP,
		THSTOP,
		TRIGGERSTART,
		TRIGGERSTOP,
		TRIGGEROUTTTL,
		TRIGGEROUTLVDS,
		TRIGGEROUTTTLINVERT,
		TRIGGEROUTLVDSINVERT,
		TRIGGERINTTLDELAY,
		TRIGGERINLVDSDELAY,
		TRIGGERUSEDELAY,
		FILEDIRECTORY,
		FILENAME,
		FILEENABLE,
		DETECTORSTATUS,
		IMAGEX,
		IMAGEY,
		FILLMODE,
	};

	enum errorCode {
		CmdOk,           ///< Command understood
		SystemBusy,   ///< The system is busy
		WrongCmd,     ///< The command has not been recognised
		ParamRange,   ///< Parameter is out of range
	};

	MerlinNet *m_merlin;
	string m_hostname;
	int m_cmdPort;
	int m_dataPort;
	int m_npixels;
	int m_nrasters;
	int m_nchips;
	bool m_simulated;

	class AcqThread;

	AcqThread *m_acq_thread;
	TrigMode m_trigger_mode;
	double m_exp_time;
	double m_lat_time;
	ImageType m_image_type;
	int m_nb_frames; // nos of frames to acquire
	bool m_thread_running;
	bool m_wait_flag;
	bool m_quit;
	int m_acq_frame_nb; // nos of frames acquired
	int m_pipes[2];
	mutable Cond m_cond;
	Counter m_counter;
	FillMode m_fillMode;
	ColourMode m_colourMode;
	Switch m_continuous;


	// Buffer control object
	SoftBufferCtrlObj m_bufferCtrlObj;

	string buildCmd(Action type, const string cmd, string value="");
	string decodeReply(const string cmd, const string reply);
	void sendCmd(Action type, string cmd, string& reply);
	void simulate(Action type, string cmd, string& reply);

	void requestCmd(ActionCmd cmd);
	template <typename T> void requestSet(ActionCmd cmd, T value);
	template <typename T> void requestGet(ActionCmd cmd, T& value);
	void requestGet(ActionCmd actionCmd, string& value);

	static std::map<ActionCmd, std::string> actionCmdMap;

	bool readFrame(void *bptr, int frame_nb);
};


inline ostream& operator <<(ostream& os, Camera::ColourMode &colourMode) {
	const char* name = "Unknown";
	switch (colourMode) {
	case Camera::Monochrome: name = "Monochrome";	break;
	case Camera::Colour: name = "Colour"; break;
	}
	return os << name;
}

inline ostream& operator <<(ostream& os, Camera::Switch &mode) {
	const char* name = "Unknown";
	switch (mode) {
	case Camera::OFF: name = "Disabled";	break;
	case Camera::ON: name = "Enabled"; break;
	}
	return os << name;
}

inline ostream& operator <<(ostream& os, Camera::GainSetting &gain) {
	const char* name = "Unknown";
	switch (gain) {
	case Camera::SLGM: name = "SuperLowGainMode";	break;
	case Camera::LGM: name = "LowGainMode"; break;
	case Camera::HGM: name = "HighGainMode";	break;
	case Camera::SHGM: name = "SuoerHighGainMode";	break;
	}
	return os << name;
}

inline ostream& operator <<(ostream& os, Camera::Counter &counter) {
	const char* name = "Unknown";
	switch (counter) {
	case Camera::COUNTER0: name = "Counter0";	break;
	case Camera::COUNTER1: name = "Counter1"; break;
	case Camera::BOTH: name = "Both";	break;
	}
	return os << name;
}

inline std::ostream& operator <<(std::ostream& os, Camera::Trigger const &trigger) {
	const char* name = "Unknown";
	switch (trigger) {
	case Camera::INTERNAL: name = "Internal";	break;
	case Camera::RISING_EDGE_TTL: name = "RisingEdgeTTL";	break;
	case Camera::FALLING_EDGE_TTL: name = "FallingEdgeTTL";	break;
	case Camera::RISING_EDGE_LVDS: name = "RisingEdgeLVDS";	break;
	case Camera::FALLING_EDGE_LVDS: name = "FallingEdgeLVDS";	break;
	case Camera::SOFT: name = "Soft";	break;
	}
	return os << name;
}

inline std::ostream& operator <<(std::ostream& os, Camera::TriggerOutput const &trigOut) {
	const char* name = "Unknown";
	switch (trigOut) {
	case Camera::TTL: name = "TTL";	break;
	case Camera::LVDS: name = "LVDS";	break;
	case Camera::TTL_DELAYED: name = "TTLDelayed";	break;
	case Camera::LVDS_DELAYED: name = "LVDSDelayed";	break;
	case Camera::FOLLOW_SHUTTER: name = "FollowShutter";	break;
	case Camera::ONE_PER_ACQ_BURST: name = "OneAcqPerBurst";	break;
	case Camera::SHUTTER_AND_SENSOR_READ: name = "ShutterAndSensorBusy";	break;
	case Camera::OUTPUT_BUSY: name = "OutputBusy";	break;
	 name = "";	break;
	}
	return os << name;
}

inline std::ostream& operator <<(std::ostream& os, Camera::TriggerLevel const &trigLevel) {
	const char* name = "Unknown";
	switch (trigLevel) {
	case Camera::NORMAL: name = "Normal";	break;
	case Camera::INVERTED: name = "Inverted";	break;
	}
	return os << name;
}

inline std::ostream& operator <<(std::ostream& os, Camera::FillMode const &mode) {
	const char* name = "Unknown";
	switch (mode) {
	case Camera::NOFILL: name = "NoFill";	break;
	case Camera::INSERTZERO: name = "InsertZero";	break;
	case Camera::DISTRIBUTE: name = "Distribute";	break;
	case Camera::INTERPOLATE: name = "Interpolate";	break;
	}
	return os << name;
}

} // namespace Merlin
} // namespace lima

#endif /* MERLINCAMERA_H_ */
