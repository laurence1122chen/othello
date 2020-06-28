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

#define MAX_DEPTH 20
struct Point {
    int x, y;
	Point() : Point(0, 0) {}
	Point(float x, float y) : x(x), y(y) {}
	bool operator==(const Point& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Point& rhs) const {
		return !operator==(rhs);
	}
	Point operator+(const Point& rhs) const {
		return Point(x + rhs.x, y + rhs.y);
	}
	Point operator-(const Point& rhs) const {
		return Point(x - rhs.x, y - rhs.y);
	}
};

int player;
const int SIZE = 8;
std::array<std::array<int, SIZE>, SIZE> board;
std::vector<Point> next_valid_spot;

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
class OthelloBoard {
public:
    enum SPOT_STATE {
        EMPTY = 0,
        BLACK = 1,
        WHITE = 2
    };
    static const int SIZE = 8;
    const std::array<Point, 8> directions{{
        Point(-1, -1), Point(-1, 0), Point(-1, 1),
        Point(0, -1), /*{0, 0}, */Point(0, 1),
        Point(1, -1), Point(1, 0), Point(1, 1)
    }};
    std::array<std::array<int, SIZE>, SIZE> board;
    std::vector<Point> next_valid_spots;
    std::array<int, 3> disc_count;
    int cur_player;
    bool done;
    int winner;
    int get_next_player(int player) const {
        return 3 - player;
    }
    bool is_spot_on_board(Point p) const {
        return 0 <= p.x && p.x < SIZE && 0 <= p.y && p.y < SIZE;
    }
    int get_disc(Point p) const {
        return board[p.x][p.y];
    }
    void set_disc(Point p, int disc) {
        board[p.x][p.y] = disc;
    }
    bool is_disc_at(Point p, int disc) const {
        if (!is_spot_on_board(p))
            return false;
        if (get_disc(p) != disc)
            return false;
        return true;
    }
    bool is_spot_valid(Point center) const {
        if (get_disc(center) != EMPTY)
            return false;
        for (Point dir: directions) {
            // Move along the direction while testing.
            Point p = center + dir;
            if (!is_disc_at(p, get_next_player(cur_player)))
                continue;
            p = p + dir;
            while (is_spot_on_board(p) && get_disc(p) != EMPTY) {
                if (is_disc_at(p, cur_player))
                    return true;
                p = p + dir;
            }
        }
        return false;
    }
    void flip_discs(Point center) {
        for (Point dir: directions) {
            // Move along the direction while testing.
            Point p = center + dir;
            if (!is_disc_at(p, get_next_player(cur_player)))
                continue;
            std::vector<Point> discs({p});
            p = p + dir;
            while (is_spot_on_board(p) && get_disc(p) != EMPTY) {
                if (is_disc_at(p, cur_player)) {
                    for (Point s: discs) {
                        set_disc(s, cur_player);
                    }
                    disc_count[cur_player] += discs.size();
                    disc_count[get_next_player(cur_player)] -= discs.size();
                    break;
                }
                discs.push_back(p);
                p = p + dir;
            }
        }
    }
    OthelloBoard() {
        reset();
    }
    void reset() {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = EMPTY;
            }
        }
        board[3][4] = board[4][3] = BLACK;
        board[3][3] = board[4][4] = WHITE;
        cur_player = BLACK;
        disc_count[EMPTY] = 8*8-4;
        disc_count[BLACK] = 2;
        disc_count[WHITE] = 2;

        done = false;
        winner = -1;
    }
    std::vector<Point> get_valid_spots() const {
        std::vector<Point> valid_spots;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                Point p = Point(i, j);
                if (board[i][j] != EMPTY)
                    continue;
                if (is_spot_valid(p))
                    valid_spots.push_back(p);
            }
        }
        return valid_spots;
    }
    bool put_disc(Point p) {
        if(!is_spot_valid(p)) {
            winner = get_next_player(cur_player);
            done = true;
            return false;
        }
        set_disc(p, cur_player);
        disc_count[cur_player]++;
        disc_count[EMPTY]--;
        flip_discs(p);
        // Give control to the other player.
        cur_player = get_next_player(cur_player);
        next_valid_spots = get_valid_spots();
        // Check Win
        if (next_valid_spots.size() == 0) {
            cur_player = get_next_player(cur_player);
            next_valid_spots = get_valid_spots();
            if (next_valid_spots.size() == 0) {
                // Game ends
                done = true;
                int white_discs = disc_count[WHITE];
                int black_discs = disc_count[BLACK];
                if (white_discs == black_discs) winner = EMPTY;
                else if (black_discs > white_discs) winner = BLACK;
                else winner = WHITE;
            }
        }
        return true;
    }
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
    int n_valid_spot;
    fin >> n_valid_spot;
    int x, y;
    for (int i = 0; i < n_valid_spot; i++) {
        fin >> x >> y;
        next_valid_spot.push_back({x, y});
    }
}

//the function of calculating heuristic value
int heuristic_value(std::array<std::array<int, SIZE>, SIZE> bd){
    int value = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if(board[i][j]==0)continue;
            else if(board[i][j]==player)value += position_value[i][j];
            else value -= position_value[i][j];
        }
    }
    return value;
}

//the function of alpha beta search
int alpha_beta_search(OthelloBoard bd, int depth, int a, int b, bool Player)
{
    if( depth >= MAX_DEPTH){
        return heuristic_value(bd.board);
    }
    if(Player){
        int value = -1000000000;
        for (int i = 0; i < bd.next_valid_spots.size(); i++) {
            OthelloBoard bd1;
            bd1.board = bd.board;
            bd1.cur_player = 3-player;
            bd1.put_disc(bd.next_valid_spots[i]);
            int t = alpha_beta_search(bd1, depth+1,a, b, false);
            value = (value > t)?value : t;
            a = (a > value)?a : value;
            if(a>=b){
                return value;
            }
        }
        return value;
    }else{
        int value = 1000000000;
        for (int i = 0; i < bd.next_valid_spots.size(); i++) {
            OthelloBoard bd1;
            bd1.board = bd.board;
            bd1.cur_player = player;
            bd1.put_disc(bd.next_valid_spots[i]);
            int t = alpha_beta_search(bd1, depth+1,a, b, true);
            value = (value < t)?value : t;
            b = (b < value)?b : value;
            if(a>=b){
                return value;
            }
        }
        return value;
    }
};

void write_valid_spot(std::ofstream& fout) {
    int n_valid_spot = next_valid_spot.size();

    Point p = next_valid_spot[0];

    fout << p.x << " " << p.y << std::endl;
    fout.flush();

    OthelloBoard bd;
    bd.board = board;
    bd.cur_player = player;
    bd.put_disc(p);

    int value = alpha_beta_search( bd ,0, -1000000000, 1000000000, true);

    for(int i = 1; i < n_valid_spot ; i++){

        OthelloBoard bd1;
        bd1.board = board;
        bd1.cur_player = player;
        bd1.put_disc(p);

        if(alpha_beta_search(bd1,0, -1000000000, 1000000000, true) >= value){
            // Remember to flush the output to ensure the last action is written to file.
            p = next_valid_spot[i];
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
