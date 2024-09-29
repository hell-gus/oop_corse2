//одна ищ финальных версий, ьез дублирования 29ю09 14-36 еще не проверила поле врага
#include <iostream>
#include <vector>
#include <stdexcept>


using namespace std;

enum class Orientation{ Horizontal, Vertical }; //ориентация корабля на поле
enum class SegmentState { Whole, Injured, Destroyed }; //состояние сегмента корабля
enum class CellFieldStatus{ Unknown, Empty, Ship}; //статус клетки поля
enum class FieldType { My, Enemy };
 
class Ship{
private:    
    int length;//сколько палуб
    Orientation orientation;
    vector<SegmentState> segments;
    int index_of_ship;
    
public:
    Ship(int length, Orientation orientation)
    : length(length), orientation(orientation), segments(length, SegmentState::Whole)
    {
        if (!( 1 <= length && length <= 4 )){
            throw invalid_argument("Invalid type of ship, length might be only in the range 1 to 4");
        }
    }

    int getLength() const { return length; }
    void setIndex(int index) {this->index_of_ship = index;}
    int getIndex () const { return index_of_ship; }

    Orientation getOrientation() const { return orientation; }

    SegmentState getSegmentState(int index_of_ship) const {
        if ( 0 > index_of_ship || index_of_ship >= length ){
            throw invalid_argument("Invalid segment index, it should be like amount of decks");
        }
        return segments[index_of_ship];
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
            ships.resize(count_of_ships, Ship{length_of_ship[i], Orientation::Horizontal});
            ships[i].setIndex(i);
        }
    }
    
    Ship& getShip( int index ){return ships[index];}
    vector<Ship>& getArrayShips(){return ships;}

    void printStateSegment( const Ship& ship) const //  ХУЙНЯ
    {
        for ( int i = 0; i < ship.getLength(); i++){
            SegmentState state = ship.getSegmentState(i);
            cout << "Segment " << i << ": " << static_cast<int>(state) << endl;
        }
    }


        

};

class Field{
private:
    int width;
    int height;
    vector<vector<CellFieldStatus>> field;
    vector<vector<int>> cells;
    FieldType type_of_field;
    bool My;

 public:
    Field( int width, int height, bool My )
    {
        if ( width <= 0 || height <= 0){
            throw invalid_argument("width or height cannot be be less than 0");
        }
        this->width = width;
        this->height = height;
        this->My = My;
        this->field = vector<vector<CellFieldStatus>>(height, vector<CellFieldStatus>(width, CellFieldStatus::Empty));
        this->cells = vector<vector<int>>(height, vector<int>(width, -1));
    }

    bool PutShip( const Ship& ship, Orientation orientation, int x, int y )//  ХУЙНЯ
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

    bool isInField(const Ship& ship,int x, int y, Orientation orientation) const {
        if ( orientation == Orientation::Horizontal){
            return x >= 0 && x + ship.getLength() <= width && y>= 0 && y <height;
        } else {
            return y>= 0 && y + ship.getLength() <= height && x >= 0 && x < width;
        }
    }

    bool isCellEmpty(int x, int y) const{
        return field[y][x] == CellFieldStatus::Empty;
    }

    bool isPlaceValid(const Ship& ship, int x, int y, Orientation orientation){
        //выход за границы поля
        if ( !isInField(ship, x, y, orientation)){
            return false;
        }
        
        //пересечение кораблей
        //цикл перебора по сегментам

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

    int getIndexShip(int index, int x, int y){
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

    void attackCell(int y, int x, Ship_Manager& manager){
        int index = cells[y][x];
        if( index != -1){
            manager.getShip(index).changeSegmentState(index);
            field[y][x] = CellFieldStatus::Ship;
            return;
         }
        
        field[y - 1][x - 1] = CellFieldStatus::Empty;//empty
    }

    void drawField(Ship_Manager& manager) {//норм только для 10*10
            char rowLabels[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O','P', 'R', 'S', 'T'};
            cout << "  ";  // Пробел для выравнивания с буквами строк
            for (int i = 1; i < width+1; ++i) {
                cout << i << " "; 
            }
            cout << '\n';
            for (int y = 0; y < height; ++y) {
                cout << rowLabels[y] << " ";  
                for (int x = 0; x < width; ++x) {
                    int index = cells[y][x];
                    if ( index != -1){
                        if ( manager.getShip(index).getSegmentState(getIndexShip(index, x, y)) == SegmentState::Whole){
                            if (My)
                                std::cout << "# ";
                            else
                                std::cout << "~ ";
                        }else if (manager.getShip(index).getSegmentState(getIndexShip(index, x, y)) == SegmentState::Injured){
                            std::cout << "+ ";
                        }else if (manager.getShip(index).getSegmentState(getIndexShip(index, x, y)) == SegmentState::Destroyed) {
                            std::cout << "X ";
                        }
                    }else if (field[y][x] == CellFieldStatus::Empty){
                        std:: cout << "0 ";
                    }else {
                        std:: cout << "~ ";
                    }
                }
                std::cout << "\n";
            }
        }
};
int main(){
    Field play1(10, 10, true);
    //Field play2 (10, 10, false);
    vector<int> sizes = {2, 3};
    Ship_Manager ships(sizes, 2);
    vector<pair<int, int>> start_coords = {{2, 2}, {5, 5}};
    //Ship_Manager ships2(sizes, 2);
    int index = 0;

    for(auto& ship: ships.getArrayShips()){
        play1.PutShip(ship, Orientation::Vertical, start_coords[index].first, start_coords[index].second);
        index++;
    }
    //play1.PutShip(ship, Orientation::Horizontal, 2, 2);

    play1.attackCell(9, 2, ships);
    play1.drawField(ships);
    return 0;
}

