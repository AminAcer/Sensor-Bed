#include "dis/dis.h"

#include <Vector3Float.h>

DIS::EntityStatePdu create_entity(double pos_x, double pos_y, double pos_z) {
    DIS::Vector3Double pos;
    pos.setX(pos_x);
    pos.setY(pos_y);
    pos.setZ(pos_z);

    DIS::EntityStatePdu entity;
    entity.setEntityLocation(pos);

    return entity;
}
