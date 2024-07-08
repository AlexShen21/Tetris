#include <iostream>
#include "model.hxx"
using namespace ge211;

Model::Model()
        : timer(),
        kill_timer()
{
    fill_map();
}

const std::vector<std::vector<bool>> I_shape{
        { false,false,false, true, true, true, true, false,false, false}
        };

std::vector<std::vector<bool>> J_shape{
        { false,false,false,false,false, true,false,false,false, false},
        { false,false,false,false,false, true,false,false,false, false},
        { false,false,false,false, true, true,false,false,false, false}

};

const std::vector<std::vector<bool>> L_shape{
        { false,false,false,false, true,false,false,false,false, false},
        { false,false,false,false, true,false,false,false,false, false},
        { false,false,false,false, true, true,false,false,false, false}
};

const std::vector<std::vector<bool>> square_shape{
        { false,false,false,false, true, true,false,false,false, false},
        { false,false,false,false, true, true,false,false,false, false}
};

const std::vector<std::vector<bool>> S_shape{
        { false,false,false,false, true, true,false,false,false, false},
        { false,false,false, true, true,false,false,false,false, false}
};

const std::vector<std::vector<bool>> T_shape{
        { false,false,false, true, true , true,false,false,false, false},
        { false,false,false,false, true,false,false,false,false, false}
};

const std::vector<std::vector<bool>> Z_shape{
        { false,false,false, true, true, false ,false,false,false, false},
        { false,false,false, false, true, true,false,false,false, false}
};

std::vector<std::vector<std::vector<bool>>> allPieces=
        {
        I_shape,
        J_shape,
        L_shape,
        square_shape,
        S_shape,
        T_shape,
        Z_shape
        };

void Model::pickCurrPiece(){
    int randomIndex = rand() % allPieces.size();
    currPiece = allPieces.at(randomIndex);
    curr_color = randomIndex;
}

void Model::findPosns(){
    std::vector<std::vector<bool>> shape = currPiece;
    for (unsigned int row = 0; row < shape.size(); ++row) {
        for (unsigned int col = 0; col < shape[row].size(); ++col) {
            if (shape.at(row).at(col)) {
                currPosns.push_back({int(col), int(row)});
            }
        }
    }
    for (Position p: currPosns){
        if (taken_map[{p}]){
            gameover = true;
        }
    }
}

void Model::fill_map()
{
    for (Position p : Model::all_positions()) {
        taken_map[p] = false;
        color_map[p] = rand() % 7;
    }
    pickCurrPiece();
    findPosns();
}

void
Model::move_block_left()
{
    bool cond = true;
    //check if all the blocks can be moved if any cant then make cond false
    for (Position& p : currPosns) {
        if ((p.x == 0) || (taken_map[{p.x - 1, p.y}])) {
            cond = false;
        }
    }
    if (cond){
        for (Position& p : currPosns) {
            p.x = p.x - 1;
        }
    }
}

void
Model::move_block_right()
{
    bool cond = true;
    //check if all the blocks can be moved if any cant then make cond false
    for (Position& p : currPosns) {
        if ((p.x == 9) || (taken_map[{p.x + 1, p.y}])) {
            cond = false;
        }
    }

    if (cond){
        for (Position& p : currPosns) {
            p.x = p.x + 1;
        }
    }
}

void
Model::move_block_down_on_key()
{
    bool cond = false;
    //check if all the blocks can be moved if any cant then make cond false
    for (Position& p : currPosns) {
        if ((p.y == 19) || (taken_map[{p.x, p.y+1}])) {
            cond = true;
        }
    }

    if (!cond){
        for (Position& p : currPosns) {
            p.y = p.y + 1;
        }
    }
}

void
Model::move_block_down_on_timer()
{
    //go through all pos
    bool cond = false;
    if (timer.elapsed_time() >= Duration(1-time_divider)) {
        for (Position& p: currPosns) {
            if (taken_map[{p.x, p.y + 1}] || p.y == 19) {
                cond = true;
                }
            }
        // do this only if each piece in the curr block can move down
        if(!cond) {
                for (Position& p: currPosns) {
                    p.y += 1;
                }
            }
        timer.reset();
        }
    for (Position p: currPosns) {
        if (p.y == 19 || taken_map[{p.x, p.y+1}]) {
            kill_block();
        }
    }
}

void
Model::rotate_block()
{

    Position center{0, 0};
    std::vector<ge211::Posn<int>> currPosns2 = {};
    for (const Position& pos : currPosns) {
            center.x += pos.x;
            center.y += pos.y;
        }
    center.x /= currPosns.size();
    center.y /= currPosns.size();

    for (Position pos : currPosns) {
            // Translate the position relative to the pivot
            int translatedX = pos.x - center.x;
            int translatedY = pos.y - center.y;

            // Apply the rotation
            int rotatedX = translatedY;
            int rotatedY = -translatedX;

            // Translate the rotated position back to the original
            // coordinate system
            pos.x = rotatedX + center.x;
            pos.y = rotatedY + center.y;

            currPosns2.push_back({int(pos.x), int(pos.y) + 1});
        }
    for (Position& pos : currPosns2) {
        if (taken_map[{pos}] || pos.y > 19) {
            return;
        }
    }


    //rotate along right wall
    bool cond1 = false;
    for (Position& pos : currPosns2) {
        if (pos.x > 9) {
            cond1 = true;
        }
    }
    if (cond1){
        if(currPiece == I_shape){
        for (Position& pos : currPosns2) {
            pos.x -=2;
        }
        }
        else{
            for (Position& pos : currPosns2) {
                pos.x -=1;
            }
        }

        currPosns.clear();
        currPosns = currPosns2;
        currPosns2.clear();
        return;
    }
    // rotate along left wall
    bool cond2 = false;
    for (Position& pos : currPosns2) {
        if (pos.x < 0 ) {
            cond2 = true;
        }
    }
    if (cond2){
        for (Position& pos : currPosns2) {
             pos.x +=1;
        }

        currPosns.clear();
        currPosns = currPosns2;
        currPosns2.clear();
        return;
    }

    currPosns.clear();
    currPosns = currPosns2;
    currPosns2.clear();


}

void
Model::kill_block()
{
    //go through all pos
    bool cond = false;
    while (true){
        for (Position& p: currPosns) {
            if (taken_map[{p.x, p.y + 1}] || p.y == 19) {
                cond = true;
            }
        }
        // do this only if each piece in the curr block can move down
        if(!cond) {
            for (Position& p: currPosns) {
                p.y += 1;
            }
        }
        else{
            for (Position& p: currPosns) {
                // update map
                taken_map[p] = true;
                color_map[p] = curr_color;
                // add each posn to bricks
                struct live_block q;
                q.rect = p;
                q.color = curr_color;
                bricks.push_back(q);
            }
            break;
        }
    }
    score += 10;
    pickCurrPiece();
    currPosns.clear();
    findPosns();
    decrease_time();
}

Model::Rectangle
Model::all_positions() const
{
    return Rectangle::from_top_left(the_origin, {10,20});
}

void Model::game_over() {
    for (Position p: all_positions()) {
        if (taken_map[p]) {
            if (p.y == 0) {
                gameover = true;
            }
        }
    }
}

bool Model::check_tetris(int row)
{
    for (int i = 0; i<=9; i++) {
        if (!taken_map[{i,row}]) {
            return false;
        }
    }
    return true;
}

void Model::reset_on_tetris() {
    //Check if each row has a tetris
    for (int i = 0; i<=19; i++) {
        if (check_tetris(i)) {
            // Reset bricks
            bricks.clear();

            // Update taken map
            for (int j = 0; j<=9; j++) {
                taken_map[{j,i}] = false;
                for (int k = i-1; k >= 0; k = k - 1) {
                    taken_map[{j,k+1}] = taken_map[{j,k}];
                    taken_map[{j,k}] = false;
                    color_map[{j,k+1}] = color_map[{j,k}];
                }
            }
            // Re-add all taken spots
            for (Position p : all_positions()) {
                if (taken_map[p]) {
                    struct live_block q;
                    q.rect = p;
                    q.color = color_map[{p.x, p.y}];
                    bricks.push_back(q);
                }
            }
            // score
            score += 50 + 50*time_divider;
        }
    }
}

void Model::decrease_time() {
    if (score > 1000) {
        time_divider = .8;
    }
    else if (score > 500) {
        time_divider = .6;
    }
    else if (score > 200) {
        time_divider = .4;
    }
    else if (score > 100) {
        time_divider = .2;
    }
}