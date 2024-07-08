#pragma once

#include <ge211.hxx>
#include <unordered_map>



class Model
{
public:
    explicit Model();

    //Taken Map + function that updates the map
    std::unordered_map<ge211::Posn<int>, bool> taken_map;
    std::unordered_map<ge211::Posn<int>, int> color_map;
    void fill_map();

    using Rectangle = ge211::Rect<int>;
    using Position = ge211::Posn<int>;

    Rectangle all_positions() const;

    //Pick a random block as the current and use that as the model_block
    //make a block move down
    //when you pick a piece store which positions are taken by the piece into
    // a current positions
    //move all those positions down / sideways on user input

    void pickCurrPiece();
    std::vector<std::vector<bool>> currPiece = {{false, false}};
    std::vector<ge211::Posn<int>> currPosns = {};
    int curr_color = rand() % 7;
    void findPosns();

    // Edit block
    void move_block_left();
    void move_block_right();
    void move_block_down_on_timer();
    void move_block_down_on_key();
    void rotate_block();

    // Called on spacebar- drops block to bottom
    void kill_block();

    // Returns true if a row is full
    bool check_tetris(int row);
    // Resets taken_map on tetris
    void reset_on_tetris();

    // Holds a rect and an integer - used in view
    struct live_block {
        ge211::Posn<int> rect = ge211::Posn<int>(0,0);
        int color = rand() % 7;
    };

    // Calls exit if a placed block is on the top row
    void game_over();

    // Vector used to load sprites in view
    std::vector<struct live_block> bricks;

    // Initialize score
    double time_divider = 0;
    int score = 0;
    void decrease_time();
    bool gameover = false;

    //void check_kill_block_timer();

private:
    // Tracks when the block should move down
    ge211::time::Timer timer;
    ge211::time::Timer kill_timer;
    static std::string const music_filename;
};