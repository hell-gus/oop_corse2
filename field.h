#ifndef FIELD_H
#define FIELD_H

#include "ship.h"
#include "ship_manager.h"

class Field{
private:
    int width;
    int height;
    std::vector<std::vector<CellFieldStatus>> field;
    std::vector<std::vector<int>> cells;
    bool my;
public:
    Field( int width, int height, bool My );
    Field(const Field &other);
    Field(Field &&other) noexcept;

    Field &operator=(const Field &other);
    Field &operator=(Field &&other) noexcept;

    bool PutShip( const Ship& ship, Orientation orientation, int x, int y );
    bool isInField(const Ship& ship,int x, int y, Orientation orientation) const;
    bool isCellEmpty(int x, int y) const;
    bool isPlaceValid(const Ship& ship, int x, int y, Orientation orientation);
    
    int getIndexShip(int index, int x, int y);
    
    void attackCell(int y, int x, Ship_Manager& manager);
    
    void drawField(Ship_Manager& manager);

};

#endif