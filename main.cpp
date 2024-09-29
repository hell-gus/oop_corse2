#include "field.h"

int main(){
    Field play1(10, 10, true);
    std::vector<int> sizes_my = {2, 3};
    Ship_Manager ships_my(sizes_my, 2);
    std::vector<std::pair<int, int>> start_coords_my = {{2, 2}, {5, 5}};

    Field play2 (10, 10, false);
    std::vector<int> sizes_enemy = {2, 3};
    Ship_Manager ships_enemy(sizes_enemy, 2);
    std::vector<std::pair<int, int>> start_coords_enemy = {{2, 2}, {5, 5}};

    play1.drawField(ships_my);
    play2.drawField(ships_enemy);

    int index_my = 0;

    for(auto& ship_my: ships_my.getArrayShips()){
        play1.PutShip(ship_my, Orientation::Vertical, start_coords_my[index_my].first, start_coords_my[index_my].second);
        index_my++;
    }

    int index_enemy = 0;

    for(auto& ship_enemy: ships_enemy.getArrayShips()){
        play2.PutShip(ship_enemy, Orientation::Vertical, start_coords_enemy[index_enemy].first, start_coords_enemy[index_enemy].second);
        index_enemy++;
    }


    play1.attackCell(9, 2, ships_my);

    play1.attackCell(2, 2, ships_my);
    play1.attackCell(2, 2, ships_my);

    play1.attackCell(3, 2, ships_my);


    play2.attackCell(2, 2, ships_enemy);

    play1.drawField(ships_my);
    play2.drawField(ships_enemy);
    return 0;

    //НО МЫ АТАКУЕМ И СТАВИМ КОРАБЛИ НЕ ПО КООРДИНАТАМ А ПО ИНДЕКСАМ
    //ДАЛЕЕ ЭТО БУДЕТ ИСПРАВЛЕННО ПРИ НАПИСАНИИ ВВОДА ОТ ПОЛЬЗОВАТЕЛЯ
}
