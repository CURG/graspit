//######################################################################
//
// GraspIt!
// Copyright (C) 2002-2015  Columbia University in the City of New York.
// All rights reserved.
//
// GraspIt! is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GraspIt! is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GraspIt!.  If not, see <http://www.gnu.org/licenses/>.
//
// Author(s): Matei T. Ciocarlie
//
// $Id:$
//
//######################################################################

/*! \file 
  \brief Defines the interface for dynamics engines used by GraspIt!
 */
#ifndef DYNAMICSENGINE_HXX
#define DYNAMICSENGINE_HXX

#include "world.h"
#include <ctime>
#include <iostream>
#include <time.h>
#include "debug.h"

class DynamicsEngine {
 public:
  virtual ~DynamicsEngine() {}
  virtual void addBody(Body *newBody) = 0;
  virtual void addRobot(Robot *robot) = 0;

  virtual void turnOnDynamics() = 0;
  virtual void turnOffDynamics() = 0;
  virtual int stepDynamics() = 0;

  virtual double moveDynamicBodies(double timeStep) = 0;
  virtual int computeNewVelocities(double timeStep) = 0;

    int frame_count;
    clock_t Start;

    void printFPS()
    {

        //if count is not initalized, set it to 0
        if (frame_count < 0  || frame_count > 60)
        {
            frame_count = 0;
        }

        if(frame_count ==0)
        {
            Start = clock();
        }
        if(frame_count ==60)
        {
            float time_passed_in_sec = ((float)clock() - (float)Start)/(1.0*CLOCKS_PER_SEC);

            DBGA("60 Frames, Time Passed(s): " << time_passed_in_sec);
            DBGA("Dynamics FPS: " << 1.0 / time_passed_in_sec);

            frame_count = -1;
        }

        frame_count +=1;
    };
};


#endif
