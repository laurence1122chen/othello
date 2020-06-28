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

#define MAX_DEPTH 12
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
const std::array<Point, 8> directions{{
        Point(-1, -1), Point(-1, 0), Point(-1, 1),
        Point(0, -1), /*{0, 0}, */Point(0, 1),
        Point(1, -1), Point(1, 0), Point(1, 1)
}};

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

//the function of calculating heuristic value
int heuristic_value(std::array<std::array<int, SIZE>, SIZE> bd){
    int value = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if(bd[i][j]==0)continue;
            else if(bd[i][j] == player)value += position_value[i][j];
            else value -= position_value[i][j];
        }
    }
    return value;
}

//the function of alpha beta search
int alpha_beta_search(std::array<std::array<int, SIZE>, SIZE> bd, int depth, int a, int b, bool Player)
{
    if( depth >= MAX_DEPTH){
        return heuristic_value(board);
    }
    if(Player){
        int value = -1000000000;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                Point p = Point(i, j);
                if (board[i][j] != EMPTY)
                    continue;
                if (is_spot_valid(p))
                    valid_spots.push_back(p);
            }
        }
        return
    }else{
        return
    }
}
    if depth = 0 or node is a terminal node then
        return the heuristic value of node
    if maximizingPlayer then
        value := −∞
        for each child of node do
            value := max(value, alphabeta(child, depth − 1, α, β, FALSE))
            α := max(α, value)
            if α ≥ β then
                break (* β cut-off *)
        return value
    else
        value := +∞
        for each child of node do
            value := min(value, alphabeta(child, depth − 1, α, β, TRUE))
            β := min(β, value)
            if β ≤ α then
                break (* α cut-off *)
        return value

void write_valid_spot(std::ofstream& fout) {
    int n_valid_spots = next_valid_spots.size();

    Point p = next_valid_spots[0];

    fout << p.x << " " << p.y << std::endl;
    fout.flush();



    int value = alpha_beta_search(board,0, -1000000000, 1000000000, true);

    for(int i = 1; i < n_valid_spots ; i++){
        if(alpha_beta_search(board,0, -1000000000, 1000000000, true) >= value){
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
