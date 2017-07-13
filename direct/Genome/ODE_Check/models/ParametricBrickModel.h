/*
 * @(#) ParametricBrickModel.h   1.0   Feb 14, 2013
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
#ifndef ROBOGEN_PARAMETRIC_BRICK_MODEL_H_
#define ROBOGEN_PARAMETRIC_BRICK_MODEL_H_

#include "../Model.h"

namespace robogen {

/**
 * A parametric brick is modelled with 3 boxes
 */
class ParametricBrickModel: public Model {

public:

	static const float MASS_SLOT;
	static const float MASS_CONNECTION_PER_CM;
	static const float SLOT_WIDTH;
	static const float SLOT_THICKNESS;
	static const float CAPSULE_HEIGHT;
	static const float CAPSULE_LENGTH;
	static const float CONNECTION_PART_THICKNESS;
	static const float CONNECTION_PART_WIDTH;

	static const unsigned int SLOT_A = 0;
	static const unsigned int SLOT_B = 1;

	static const unsigned int B_SLOT_A_ID = 0;
	static const unsigned int B_SLOT_B_ID = 1;
	static const unsigned int B_CONNECTION_ID = 2;
	static const unsigned int B_CAPSULE_ID = 3;

	/**
	 * Initialize a parametric brick model
	 *
	 * @param odeWorld
	 * @param odeSpace
	 * @param connectionPartLength in m
	 * @param angleA in radians
	 * @param angleB in radians
	 */
	ParametricBrickModel(dWorldID odeWorld, dSpaceID odeSpace, std::string id,
			float connectionPartLength, float angleA, float angleB);

	virtual ~ParametricBrickModel();

	virtual bool initModel();

	virtual dBodyID getRoot();

	virtual dBodyID getSlot(unsigned int i);

	virtual osg::Vec3 getSlotPosition(unsigned int i);

	virtual osg::Vec3 getSlotOrientation(unsigned int i);

	virtual osg::Vec3 getSlotAxis(unsigned int i);

	float getConnectionLength();

private:

	float connectionPartLength_;
	float angleA_;
	float angleB_;

	dBodyID brickRoot_;
	dBodyID brickTail_;

};

}

#endif /* ROBOGEN_PARAMETRIC_BRICK_MODEL_H_ */
