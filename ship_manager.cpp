#include "ship_manager.h"

Ship_Manager::Ship_Manager( std::vector<int> length_of_ship, int count_of_ships )
    {
        if ( count_of_ships != length_of_ship.size() ) {
            throw std::invalid_argument("Number of ships and size of the length vector must be equal");
        }

        for ( int i = 0; i < count_of_ships; i++) {
            ships.resize(count_of_ships, Ship{length_of_ship[i], Orientation::Horizontal});
            ships[i].setIndex(i);
        }
    }
    
Ship& Ship_Manager::getShip( int index ){return ships[index];}
std::vector<Ship>& Ship_Manager::getArrayShips(){return ships;}

void Ship_Manager::printStateSegment( const Ship& ship) const 
{
    for ( int i = 0; i < ship.getLength(); i++){
        SegmentState state = ship.getSegmentState(i);
        std::cout << "Segment " << i << ": " << static_cast<int>(state) << std::endl;
    }
};