#include "ship.h"

Ship::Ship(int length, Orientation orientation)
    : length(length), orientation(orientation), segments(length, SegmentState::Whole)
    {
        if (!( 1 <= length && length <= 4 )){
            throw std::invalid_argument("Invalid type of ship, length might be only in the range 1 to 4");
        }
    }

int Ship::getLength() const { return length; }
void Ship::setIndex(int index) {this->index_of_ship = index;}
int Ship::getIndex () const { return index_of_ship; }

SegmentState Ship::getSegmentState(int index_of_ship) const {
    if ( 0 > index_of_ship || index_of_ship >= length ){
        throw std::invalid_argument("Invalid segment index, it should be like amount of decks");
    }
    return segments[index_of_ship];
}

SegmentState Ship::changeSegmentState(int index_of_segment) {
    if ( segments[index_of_segment] == SegmentState::Whole  ){
        segments[index_of_segment] = SegmentState::Injured;
    }else if ( segments[index_of_segment] == SegmentState::Injured  ){
        segments[index_of_segment] = SegmentState::Destroyed;
    }

    return segments[index_of_segment];
}