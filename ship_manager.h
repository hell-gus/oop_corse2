#ifndef SHIPMANAGER_H
#define SHIPMANAGER_H

#include "ship.h"

class Ship_Manager{
private:
    std::vector<Ship> ships;

public:
    Ship_Manager( std::vector<int> length_of_ship, int count_of_ships );

    Ship& getShip( int index );
    std::vector<Ship>& getArrayShips();

    void printStateSegment( const Ship& ship) const;

};

#endif