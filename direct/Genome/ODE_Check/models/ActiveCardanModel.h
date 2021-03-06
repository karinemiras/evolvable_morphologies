/*
 * @(#) ActiveCardanModel.h   1.0   Feb 14, 2013
 *
 * Andrea Maesani (andrea.maesani@epfl.ch)
 *
 * The ROBOGEN Framework
 * Copyright © 2012-2013 Andrea Maesani
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
#ifndef ROBOGEN_ACTIVE_CARDAN_MODEL_H_
#define ROBOGEN_ACTIVE_CARDAN_MODEL_H_

#include "ActuatedComponent.h"

namespace robogen {

/**
 * A Cardan is modelled with 4 boxes, a central cross formed by two boxes and two hinges.
 * Furthermore each cross part has two more boxes to avoid penetrations in the
 * central frame.
 */
class ActiveCardanModel: public ActuatedComponent {

public:

	// TODO Titus: why is this here?
	static const float MASS_SERVO;
	static const float MASS_SLOT;
	static const float MASS_CROSS;
	static const float SLOT_WIDTH;
	static const float SLOT_THICKNESS;
	static const float CONNNECTION_PART_WIDTH;
	static const float CONNNECTION_PART_LENGTH;
	static const float CONNECTION_PART_HEIGHT;
	static const float CONNECTION_PART_OFFSET;
	static const float CONNNECTION_PART_ROTATION_OFFSET;
	static const float CROSS_CENTER_OFFSET;
	static const float CROSS_WIDTH;
	static const float CROSS_HEIGHT;
	static const float CROSS_THICKNESS;

	static const unsigned int SLOT_A = 0;
	static const unsigned int SLOT_B = 1;

	static const unsigned int B_SLOT_A_ID = 0;
	static const unsigned int B_SLOT_B_ID = 1;
	static const unsigned int B_CONNECTION_A_ID = 2;
	static const unsigned int B_CONNECTION_B_ID = 3;
	static const unsigned int B_CROSS_PART_A_ID = 4;
	static const unsigned int B_CROSS_PART_B_ID = 5;
	static const unsigned int B_CROSS_PART_A_EDGE_1_ID = 6;
	static const unsigned int B_CROSS_PART_A_EDGE_2_ID = 7;
	static const unsigned int B_CROSS_PART_B_EDGE_1_ID = 8;
	static const unsigned int B_CROSS_PART_B_EDGE_2_ID = 9;

	ActiveCardanModel(dWorldID odeWorld, dSpaceID odeSpace, std::string id);

	virtual ~ActiveCardanModel();

	virtual bool initModel();

	virtual dBodyID getRoot();

	virtual dBodyID getSlot(unsigned int i);

	virtual osg::Vec3 getSlotPosition(unsigned int i);

	virtual osg::Vec3 getSlotOrientation(unsigned int i);

	virtual osg::Vec3 getSlotAxis(unsigned int i);

	virtual void getMotors(std::vector<boost::shared_ptr<Motor> >& motors);

private:

	dBodyID cardanRoot_;
	dBodyID cardanTail_;

	boost::shared_ptr<Motor> motor1_;
	boost::shared_ptr<Motor> motor2_;

};

}

#endif /* ROBOGEN_ACTIVE_CARDAN_MODEL_H_ */
