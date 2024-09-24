
#include <iostream>
#include <vector>

using namespace std;

enum class Orientation{ Horizontal, Vertical }; //ориентация корабля на поле
enum class SegmentState { Whole, Injured, Destroyed }; //состояние сегмента корабля
enum class CellFieldStatus{ Unknown, Empty, Ship }; //статус клетки поля

class Ship{
private:    
    int length;//сколько палуб
    Orientation orientation;
    vector<SegmentState> segments;
    
public:
    Ship(int length, Orientation orientation)
    : length(length), orientation(orientation), segments(length, SegmentState::Whole)
    {
        if (!( 1 <= length && length <= 4 )){
            throw invalid_argument("Invalid type of ship, length might be only in the range 1 to 4");
        }
    }

    int getLength() const { return length; }

    Orientation getOrientation() const { return orientation; }

    SegmentState getSegmentState(int index_of_segment) const {
        if (!( 0 < index_of_segment && index_of_segment <= length )){
            throw invalid_index("Invalid segment index, it should be like amount of decks");
        }
        return segment[index_of_segment];
    }

    SegmentState changeSegmentState(int index_of_segment) {
        if ( segments[index_of_segment] == SegmentState::Whole  ){
            segments[index_of_segment] = SegmentState::Injured;
        }else if ( segments[index_of_segment] == SegmentState::Injured  ){
            segments[index_of_segment] = SegmentState::Destroyed;
        }

        return segments[index_of_segment];
    }
};

class Ship_Manager{
private:
    vector<Ship> ships;

public:
    Ship_Manager( vector<int> length_of_ship, int count_of_ships )
    {
        if ( count_of_ships != length_of_ship.size() ) {
            throw invalid_argument("Number of ships and size of the length vector must be equal");
        }

        for ( int i = 0; i < count_of_ships; i++) {
            Ship new_ship(length_of_ship[i], Orientation::Horizontal);
            ships.push_back(new_ship);
        }
    }

    void printStateSegment( const Ship& ship) const //  ХУЙНЯ
    {
        for ( int i = 0; i < ship.getLength(); i++){
            SegmentState state = ship.getSegmentState(i);
            cout << "Segment " << i << ": " << static_cast<int>(state) << endl;
        }
    }

    //место для функции получения корабля по индексу, если она появится

};

class Field{
private:
    int width;
    int height;
    vector<vector<CellFieldStatus>> field;
 public:
    Field( width, vector<CellFieldStatus>(height, CellFieldStatus::Unknown) )
    {
        if ( width <= 0 || height <= 0){
            throw invalid_argument("width or height cannot be be less than 0")
        }
    }

    bool PutShip( const Ship& ship, Coords coords, Orientation orientation, int x, int y )//  ХУЙНЯ
    {
        if (!( isEmptyCell(ship, x, y, orientation) )){
            return false;
        }

        if ( orientation == Orientation::Horizontal ){
            for ( int i = 0; i <= ship.getLength(); i++ ){
                field[x+i][y] = CellFieldStatus::Ship; 
            }
        } else if ( orientation == Orientation::Vertical ){
            for ( int i = 0; i <= ship.getLength(); i++){
                field[x][y+i] = CellFieldStatus::Ship;
            }
        }
        return true;
    }

    bool isEmptyCell( const Ship& ship, int x, int y, Orientation orientation){
        int length = ship.getLength()

        if ( orientation == Orientation::Horizontal ){
            if (!( x + length <= width)){
                return false;
            }
        } else {
            if (!( y + length <= height)){
                return false;
        }
        }

        if ( orientation == Orientation::Horizontal ){
            for ( x - 1; x <= x + length + 1){
                if (!( field[x][y-1] == CellFieldStatus::Empty)) return false;
                if (!( field[x][y] == CellFieldStatus::Empty)) return false;
                if (!( field[x][y+1] == CellFieldStatus::Empty)) return false;
            }
        } else{
            for ( y - 1; y <= y + length + 1){
                if (!( field[x-1][y] == CellFieldStatus::Empty)) return false;
                if (!( field[x][y] == CellFieldStatus::Empty)) return false;
                if (!( field[x+1][y] == CellFieldStatus::Empty)) return false;
            }
        }
    }

    void drawField() {//норм только для 10*10
            char rowLabels[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O','P', 'R', 'S', 'T'};
            std::cout << "  ";  // Пробел для выравнивания с буквами строк
            for (int i = 0; i < width; ++i) {
                std::cout << i << " "; 
            }
            std::cout << '\n';

            for (int y = 0; y < height; ++y) {
                std::cout << rowLabels[y] << " ";  
                for (int x = 0; x < width; ++x) {
                    switch (field[y][x]) {  
                        case CellState::EMPTY:
                            std::cout << "~ ";  // Пустое поле (вода)
                            break;
                        case CellState::SHIP:
                            std::cout << "# ";  // Корабль
                            break;
                        case CellState::HIT:
                            std::cout << "+ ";  // Попадание
                            break;
                        case CellState::DESTROYED:
                            std::cout << "X ";  // Попадание
                            break;
                        case CellState::UNKNOWN:
                            std::cout << "O ";  // Промах
                            break;
                    }
                }
                std::cout << '\n';  
            }
        }
        
};

int main(){
    GameField play (10, 10);
    std::vector<int> sizes = {4, 3, 3, 2, 1};
    std::vector<std::pair<int, int>> meow = {{1, 2}, {1, 4}, {3, 4}, {8, 4}, {8, 1}};
    std::vector<bool> woof = {true, false, false, false, false};
    ShipManager manager(sizes);
    int index = 0;
    for (auto& ship: manager.getShips()){
        play.placeShip(ship, woof[index], meow[index].first, meow[index].second);
        index++;
    }
    play.DrawField();
    play.attack(1, 2);
    play.DrawField();
    play.attack(1, 2);
    play.attack(3, 2);
    play.DrawField();
}




