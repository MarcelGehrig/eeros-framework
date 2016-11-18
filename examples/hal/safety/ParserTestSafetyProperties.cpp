#include "ParserTestSafetyProperties.hpp"
#include "../control/ParserTestControlSystem.hpp"
#include <eeros/hal/HAL.hpp>
#include <eeros/safety/InputAction.hpp>
#include <eeros/safety/inputActions.hpp>
#include <eeros/safety/OutputAction.hpp>
#include <eeros/safety/ControlInput.hpp>
#include <eeros/math/Matrix.hpp>

#include <unistd.h>
#include <iostream>
#include <vector>
#include <initializer_list>
#include <cmath>

using namespace eeros;
using namespace eeros::hal;
using namespace eeros::safety;

ParserTestSafetyProperties::ParserTestSafetyProperties(ParserTestControlSystem* cs) : slOff("System off"), controlSys(cs) {

// 	  HAL& hal = HAL::instance();
  
	// ############ Define critical outputs ############
	
	// ############ Define critical inputs ############
	
	// ############ Define Levels ############
	
	addLevel(slOff);
	
	// ############ Add events to the levels ############
	
	// ############ Define input states and events for all levels ############
		
	// Define output states and events for all levels 

	// *** Define and add level functions *** //

	// Define entry level
	setEntryLevel(slOff);
}

ParserTestSafetyProperties::~ParserTestSafetyProperties() {
	// nothing to do
}