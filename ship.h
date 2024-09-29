#ifndef SHIP_H
#define SHIP_H

#include <iostream>
#include <vector>
#include <stdexcept>

enum class Orientation{ Horizontal, Vertical }; //ориентация корабля на поле
enum class SegmentState { Whole, Injured, Destroyed }; //состояние сегмента корабля
enum class CellFieldStatus{ Unknown, Empty, Ship}; //статус клетки поля

class Ship{
private:    
    int length;//сколько палуб
    Orientation orientation;
    std::vector<SegmentState> segments;
    int index_of_ship;

public:
    Ship(int length, Orientation orientation);

    int getLength() const;
    void setIndex(int index);
    int getIndex() const;

    SegmentState getSegmentState(int index_of_ship) const;
    SegmentState changeSegmentState(int index_of_segment);

};

#endif