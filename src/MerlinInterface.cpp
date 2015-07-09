//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2009-2014
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

#include "MerlinInterface.h"

using namespace lima;
using namespace lima::Merlin;

Interface::Interface(Camera& cam) :
		m_cam(cam), m_det_info(cam), m_sync(cam)
{
	DEB_CONSTRUCTOR();
	HwDetInfoCtrlObj *det_info = &m_det_info;
	m_cap_list.push_back(det_info);

	m_bufferCtrlObj = m_cam.getBufferCtrlObj();
	HwBufferCtrlObj *buffer = m_cam.getBufferCtrlObj();
	m_cap_list.push_back(buffer);

	HwSyncCtrlObj *sync = &m_sync;
	m_cap_list.push_back(sync);
}

Interface::~Interface() {
	DEB_DESTRUCTOR();
	delete &m_det_info;
	delete &m_sync;
}

void Interface::getCapList(CapList &cap_list) const {
	DEB_MEMBER_FUNCT();
	cap_list = m_cap_list;
}

void Interface::reset(ResetLevel reset_level) {
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(reset_level);

	stopAcq();
	m_cam.reset();
}

void Interface::prepareAcq() {
	DEB_MEMBER_FUNCT();
	m_cam.prepareAcq();
}

void Interface::startAcq() {
	DEB_MEMBER_FUNCT();
	m_cam.startAcq();
}

void Interface::stopAcq() {
	DEB_MEMBER_FUNCT();
	m_cam.stopAcq();
}

void Interface::getStatus(StatusType& status) {
	DEB_MEMBER_FUNCT();
	status.acq = m_cam.isAcqRunning() ? AcqRunning : AcqReady;
	DEB_TRACE() << "AcqStatus " << status.acq;

	Camera::DetectorStatus detstat;
	m_cam.getStatus(detstat);
	if (detstat == Camera::DetectorStatus::BUSY) {
	  DEB_TRACE() << "DetExposure";
		status.det = DetExposure;
	} else {
	  DEB_TRACE() << "DetIdle";
		status.det = DetIdle;
	}

	DEB_RETURN() << DEB_VAR1(status);
}

int Interface::getNbHwAcquiredFrames() {
	DEB_MEMBER_FUNCT();
	return m_cam.getNbHwAcquiredFrames();
}
