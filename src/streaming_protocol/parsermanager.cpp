/*! \file
	\section FileCopyright Copyright Notice
	This is free and unencumbered software released into the public domain.

	Anyone is free to copy, modify, publish, use, compile, sell, or
	distribute this software, either in source code form or as a compiled
	binary, for any purpose, commercial or non-commercial, and by any
	means.

	In jurisdictions that recognize copyright laws, the author or authors
	of this software dedicate any and all copyright interest in the
	software to the public domain. We make this dedication for the benefit
	of the public at large and to the detriment of our heirs and
	successors. We intend this dedication to be an overt act of
	relinquishment in perpetuity of all present and future rights to this
	software under copyright law.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
	OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

#include "parsermanager.h"

#include "eulerdatagram.h"
#include "scaledatagram.h"
#include "metadatagram.h"
#include "quaterniondatagram.h"
#include "angularsegmentkinematicsdatagram.h"
#include "centerofmassdatagram.h"
#include "jointanglesdatagram.h"
#include "linearsegmentkinematicsdatagram.h"
#include "positiondatagram.h"
#include "timecodedatagram.h"
#include "trackerkinematicsdatagram.h"

ParserManager::ParserManager()
{ 
}

/*! Destructor */
ParserManager::~ParserManager()
{
}

Datagram* ParserManager::createDgram(StreamingProtocol proto)
{
	switch (proto)
	{
	case SPPoseEuler:		return new EulerDatagram;
	case SPPoseQuaternion:	return new QuaternionDatagram;
	case SPPosePositions:	return new PositionDatagram;
	case SPMetaScaling:		return new ScaleDatagram;
	case SPMetaMoreMeta:	return new MetaDatagram;
	case SPJointAngles:					return new JointAnglesDatagram;
	case SPLinearSegmentKinematics:		return new LinearSegmentKinematicsDatagram;
	case SPAngularSegmentKinematics:	return new AngularSegmentKinematicsDatagram;
	case SPTrackerKinematics:			return new TrackerKinematicsDatagram;
	case SPCenterOfMass:				return new CenterOfMassDatagram;
	case SPTimeCode:					return new TimeCodeDatagram;

	default:
		return nullptr;
	}	
}

/*! Read single datagram from the incoming stream */
void* ParserManager::readDatagram(const XsByteArray &data)
{
	StreamingProtocol type = static_cast<StreamingProtocol>(Datagram::messageType(data));
	Datagram *datagram = createDgram(type);

	if (datagram != nullptr) 
	{
		datagram->deserialize(data);

		//datagram->printHeader();
		//datagram->printData();
		
		if (Datagram::messageType(data) == StreamingProtocol::SPPoseEuler)
		{
			void *dataEuler = datagram->getData();
			if (dataEuler != nullptr)
			{
				dataEuler_.clear();
				std::vector<Kinematics> *dataEulerLocal;
				dataEulerLocal = static_cast<std::vector<Kinematics>*>(dataEuler);

				// local copy because std vector copy method just copy the adress which are delete at the end of this method so not good for our use case
				for (std::vector<Kinematics>::const_iterator it = dataEulerLocal->begin(); it != dataEulerLocal->end(); it++)
				{
					dataEuler_.push_back(*it);
				}
			}
			delete datagram;
			return &dataEuler_;
		}

		if (Datagram::messageType(data) == StreamingProtocol::SPJointAngles)
		{
			void *dataJoint = datagram->getData();
			if (dataJoint != nullptr)
			{
				dataAngle_.clear();
				std::vector<Joint> *dataAngleLocal;
				dataAngleLocal = static_cast<std::vector<Joint>*>(dataJoint);

				// local copy because std vector copy method just copy the adress which are delete at the end of this method so not good for our use case
				for (std::vector<Joint>::const_iterator it = dataAngleLocal->begin(); it != dataAngleLocal->end(); it++)
				{
					dataAngle_.push_back(*it);
				}
			}
			delete datagram;
			return &dataAngle_;
		}
	}
	return nullptr;
}
