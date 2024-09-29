#include "field.h"

Field::Field( int width, int height, bool my )
{
    if ( width <= 0 || height <= 0){
        throw std::invalid_argument("width or height cannot be be less than 0");
    }
    this->width = width;
    this->height = height;
    this->my = my;
    this->field = std::vector<std::vector<CellFieldStatus>>(height, std::vector<CellFieldStatus>(width, CellFieldStatus::Unknown));
    this->cells = std::vector<std::vector<int>>(height, std::vector<int>(width, -1));
}
Field::Field(const Field &other) : width(other.width), height(other.height), field(other.field), cells(other.cells), my(other.my){
}

Field::Field(Field &&other) noexcept : width(other.width), height(other.height), field(std::move(other.field)), cells(other.cells), my(other.my){
}

Field &Field::operator=(const Field &other)
{
    if(this == &other) return *this;

    width = other.width;
    height = other.height;
    field = other.field;
    cells = other.cells;
    my = other.my;
    return *this;
}

Field &Field::operator=(Field &&other) noexcept
{
    if (this == &other) return *this;
    
    width = other.width;
    height = other.height;
    field = std::move(other.field);
    cells = other.cells;
    my = other.my;

    return *this;
    
}
bool Field::PutShip( const Ship& ship, Orientation orientation, int x, int y )
{
    int length = ship.getLength();

    if (!isPlaceValid(ship, x, y, orientation)){
        return false;
    }

    if ( orientation == Orientation::Horizontal){
        for( int i = 0; i < ship.getLength(); i++){
            cells[y][x + i] = ship.getIndex();
            field[y][x + i] = CellFieldStatus::Ship;
        }
    } else {
        for( int i = 0; i < ship.getLength(); i++){
            cells[y + i][x] = ship.getIndex();
            field[y + i][x] = CellFieldStatus::Ship;
        }
    }
    return true;
    
}

bool Field::isInField(const Ship& ship,int x, int y, Orientation orientation) const {
    if ( orientation == Orientation::Horizontal){
        return x >= 0 && x + ship.getLength() <= width && y>= 0 && y <height;
    } else {
        return y>= 0 && y + ship.getLength() <= height && x >= 0 && x < width;
    }
}

bool Field::isCellEmpty(int x, int y) const{
    return field[y][x] != CellFieldStatus::Ship;
}

bool Field::isPlaceValid(const Ship& ship, int x, int y, Orientation orientation){
    //выход за границы поля
    if ( !isInField(ship, x, y, orientation)){
        return false;
    }

    for ( int i = 0; i < ship.getLength(); ++i){
        int checkX = (orientation == Orientation::Horizontal) ? x + i : x;
        int checkY = (orientation == Orientation::Horizontal) ? y : y + i;

        if (!isCellEmpty(checkX, checkY)){
            return false;
        }
    }

    for ( int dx = -1; dx <= 1; ++dx){
        for ( int dy = -1; dy <= 1; ++dy){
            for ( int i = 0; i < ship.getLength(); ++i){
                int checkX = (orientation == Orientation::Horizontal) ? x + i : x;
                int checkY = (orientation == Orientation::Horizontal) ? y : y + i;

                if (isInField(ship, checkX + dx, checkY + dy, orientation) && !isCellEmpty(checkX + dx, checkY + dy)){
                    return false;
                }
            }
        }
    }
    return true;
}

int Field::getIndexShip(int index, int x, int y){
    int index_segment = 0;
    while ( x > 0 && cells[y][x - 1] == index){
        x--;
        index_segment++;
    }
    while ( y > 0 &&  cells[y - 1][x] == index){
        y--;
        index_segment++;
    }
    return index_segment;

}

void Field::attackCell(int y, int x, Ship_Manager& manager){
    int index = cells[y][x];
    if( index != -1){
        manager.getShip(index).changeSegmentState(getIndexShip(index, x, y));
        field[y][x] = CellFieldStatus::Ship;
        return;
        }
    
    field[y][x] = CellFieldStatus::Empty;
}

void Field::drawField(Ship_Manager& manager) {//норм только для 10*10
        char rowLabels[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O','P', 'R', 'S', 'T'};
        std::cout << "  ";  // Пробел для выравнивания с буквами строк
        for (int i = 1; i < width + 1; ++i) {
            std::cout << i << " "; 
        }
        std::cout << '\n';
        for (int y = 0; y < height; ++y) {
            std::cout << rowLabels[y] << " ";  
            for (int x = 0; x < width; ++x) {
                int index = cells[y][x];
                if ( index != -1){
                    if ( manager.getShip(index).getSegmentState(getIndexShip(index, x, y)) == SegmentState::Whole){
                        if (my)
                            std::cout << "# ";
                        else
                            std::cout << "~ ";
                    }else if (manager.getShip(index).getSegmentState(getIndexShip(index, x, y)) == SegmentState::Injured){
                        std::cout << "+ ";
                    }else if (manager.getShip(index).getSegmentState(getIndexShip(index, x, y)) == SegmentState::Destroyed) {
                        std::cout << "X ";
                    }
                }else if (field[y][x] == CellFieldStatus::Empty){
                        std::cout << "0 ";//в меня не стреляли
                } else {
                        std:: cout << "~ ";//в врага стреляли
                }
            }
            std::cout << "\n";
        }
    }