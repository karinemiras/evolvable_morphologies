/*
 * @(#) CoreComponentModel.h   1.0   Feb 8, 2013
 *
 * Andrea Maesani (andrea.maesani@epfl.ch)
 * Joshua Auerbach (joshua.auerbach@epfl.ch)
 *
 * The ROBOGEN Framework
 * Copyright © 2012-2014 Andrea Maesani, Joshua Auerbach
 *
 * Laboratory of Intelligent Systems, EPFL
 *
 * This file is part of the ROBOGEN Framework.
 *
 * The ROBOGEN Framework is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (GPL)
 * as published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @(#) $Id$
 */
#ifndef ROBOGEN_CORE_COMPONENTMODEL_H_
#define ROBOGEN_CORE_COMPONENTMODEL_H_

#include "PerceptiveComponent.h"
#include "ImuSensor.h"

namespace robogen {

class CoreComponentModel: public PerceptiveComponent {

public:

	static const float BRICK_MASS;
	static const float CORE_MASS;
	static const float WIDTH;

	static const unsigned int B_CORE_COMPONENT_ID = 0;

	static const unsigned int LEFT_FACE_SLOT = 0;
	static const unsigned int RIGHT_FACE_SLOT = 1;
	static const unsigned int TOP_FACE_SLOT = 2;
	static const unsigned int BOTTOM_FACE_SLOT = 3;
	static const unsigned int FRONT_FACE_SLOT = 4;
	static const unsigned int BACK_FACE_SLOT = 5;

	/**
	 * Initializes a CoreComponentModel
	 * @param odeWorld
	 * @param odeSpace
	 * @param id
	 * @param hasSensors if true the core component will contain gyro and accelerometer sensors, if false it won't provide any sensor
	 */
	CoreComponentModel(dWorldID odeWorld, dSpaceID odeSpace, std::string id,
			bool hasSensors);

	virtual ~CoreComponentModel();

	virtual bool initModel();

	virtual dBodyID getRoot();

	virtual dBodyID getSlot(unsigned int i);

	virtual osg::Vec3 getSlotPosition(unsigned int i);

	virtual osg::Vec3 getSlotOrientation(unsigned int i);

	virtual osg::Vec3 getSlotAxis(unsigned int i);

	virtual void getSensors(std::vector<boost::shared_ptr<Sensor> >& sensors);

	virtual void updateSensors(boost::shared_ptr<Environment>& env);

	inline bool hasSensors() {
		return hasSensors_;
	}

private:

	boost::shared_ptr<ImuSensor> sensor_;

	dBodyID coreComponent_;

	bool hasSensors_;

};

}

#endif /* ROBOGEN_CORE_COMPONENTMODEL_H_ */
