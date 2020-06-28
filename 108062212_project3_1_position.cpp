#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>

#define C1 99
#define C2 -8
#define C3 8
#define C4 6
#define C5 -24
#define C6 -4
#define C7 -3
#define C8 7
#define C9 4
#define C10 0

struct Point {
    int x, y;
};

int player;
const int SIZE = 8;
std::array<std::array<int, SIZE>, SIZE> board;
std::vector<Point> next_valid_spots;

int position_value[SIZE][SIZE] = {
    C1, C2, C3 ,C4 ,C4 ,C3 ,C2 ,C1,
    C2, C5, C6 ,C7 ,C7 ,C6 ,C5 ,C2,
    C3, C6, C8 ,C9 ,C9 ,C8 ,C6 ,C3,
    C4, C7, C9 ,C10 ,C10 ,C9 ,C7 ,C4,
    C4, C7, C9 ,C10 ,C10 ,C9 ,C7 ,C4,
    C3, C6, C8 ,C9 ,C9 ,C8 ,C6 ,C3,
    C2, C5, C6 ,C7 ,C7 ,C6 ,C5 ,C2,
    C1, C2, C3 ,C4 ,C4 ,C3 ,C2 ,C1,
};

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

void read_valid_spots(std::ifstream& fin) {
    int n_valid_spots;
    fin >> n_valid_spots;
    int x, y;
    for (int i = 0; i < n_valid_spots; i++) {
        fin >> x >> y;
        next_valid_spots.push_back({x, y});
    }
}

//the function of value of position


void write_valid_spot(std::ofstream& fout) {
    int n_valid_spots = next_valid_spots.size();
    //int index = (rand() % n_valid_spots);
    Point p = next_valid_spots[0];
    for(int i = 0; i < n_valid_spots ; i++){
        if(position_value[next_valid_spots[i].y][next_valid_spots[i].x]>=position_value[p.y][p.x]){
            // Remember to flush the output to ensure the last action is written to file.
            p = next_valid_spots[i];
            fout << p.x << " " << p.y << std::endl;
            fout.flush();
        }

    }
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    read_valid_spots(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
