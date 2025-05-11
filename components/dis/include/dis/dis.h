#pragma once

#include <dis7/EntityStatePdu.h>

/// @brief Creates an Entity based on ECEF coords
///
/// @param pos_x: ECEF Positon X
/// @param pos_y: ECEF Positon Y
/// @param pos_z: ECEF Positon Z
///
/// @return Filled out EntityStatePDU
DIS::EntityStatePdu create_entity(double pos_x, double pos_y, double pos_z);
