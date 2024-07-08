#include "model.hxx"
#include <catch.hxx>



TEST_CASE( "test_block_movement")
{
    Model model;

    // Move the block to the left

    model.currPosns = {{1,0}, {1,1}, {1,2},{1,3}};
    model.move_block_left();
    std::vector<ge211::Posn<int>> Positions = {{0, 0}, {0, 1}, {0, 2},
                                               {0, 3}};
    CHECK(model.currPosns == Positions);

    std::vector<ge211::Posn<int>> Positions2 = {{1, 0}, {1, 1}, {1, 2},
                                               {1, 3}};
    model.move_block_right();


    CHECK(model.currPosns == Positions2);

    model.move_block_down_on_key();
    std::vector<ge211::Posn<int>> Positions3 = {{1, 1}, {1, 2}, {1, 3},
                                                {1, 4}};

    CHECK(model.currPosns == Positions3);
}


TEST_CASE( "test_tetris")
{
    Model model;

    // Move the block to the left

    model.currPosns = {{0,0}, {1,0}, {2,0},{3,0}};
    model.kill_block();
    model.currPosns = {{4,0}, {5,0}, {6,0},{7,0}};
    model.kill_block();
    model.currPosns = {{8,0}, {9,0}};
    model.kill_block();

    CHECK(model.check_tetris(19) == true);

    model.currPosns = {{0,0}, {1,0}, {2,0},{3,0}};
    model.kill_block();
    model.currPosns = {{4,0}, {5,0}, {6,0},{7,0}};
    model.kill_block();
    model.currPosns = {{8,0}, {9,0}};
    model.kill_block();

    CHECK(model.check_tetris(18) == true);

}


TEST_CASE( "Block Stacking")
{
    Model model;

    // Move the block to the left

    model.currPosns = {{0,0}, {0,1}, {0,2},{0,3}};
    model.kill_block();
    model.currPosns = {{0,0}, {0,1}, {0,2},{0,3}};
    model.kill_block();

    CHECK(model.taken_map[{0,19}] == true);
    CHECK(model.taken_map[{0,18}] == true);
    CHECK(model.taken_map[{0,17}] == true);
    CHECK(model.taken_map[{0,16}] == true);
    CHECK(model.taken_map[{0,15}] == true);
    CHECK(model.taken_map[{0,14}] == true);
    CHECK(model.taken_map[{0,13}] == true);
    CHECK(model.taken_map[{0,12}] == true);



    model.currPosns = {{5,0}, {5,1}, {5,2},{5,3}};
    model.kill_block();
    model.currPosns = {{5,0}, {5,1}, {5,2},{6,1}};
    model.kill_block();

    CHECK(model.taken_map[{5,19}] == true);
    CHECK(model.taken_map[{5,18}] == true);
    CHECK(model.taken_map[{5,17}] == true);
    CHECK(model.taken_map[{5,16}] == true);
    CHECK(model.taken_map[{5,15}] == true);
    CHECK(model.taken_map[{5,14}] == true);
    CHECK(model.taken_map[{5,13}] == true);
    CHECK(model.taken_map[{6,14}] == true);


}


TEST_CASE( "Time Divider test")
{
    Model model;

    model.score = 105;
    model.decrease_time();
    CHECK(model.time_divider == 0.2);
    model.score = 200;
    model.decrease_time();
    CHECK(model.time_divider == 0.2);
     model.score = 202;
    model.decrease_time();
    CHECK(model.time_divider == 0.4);
    model.score = 500;
    model.decrease_time();
    CHECK(model.time_divider == 0.4);
    model.score = 507;
    model.decrease_time();
    CHECK(model.time_divider == 0.6);
    model.score = 1000;
    model.decrease_time();
    CHECK(model.time_divider == 0.6);
    model.score = 1126;
    model.decrease_time();
    CHECK(model.time_divider == 0.8);

}

TEST_CASE( "Rotate off screen test")
{
    Model model;

    std::vector<ge211::Posn<int>> Positions1 = {{0,2}, {1,2}, {2,2}, {3,2}};

    model.currPosns = {{0,0}, {0,1}, {0,2}, {0,3}};
    model.rotate_block();
    CHECK(model.currPosns == Positions1);


    std::vector<ge211::Posn<int>> Positions2 = {{6,2}, {7,2}, {8,2}, {9,2}};

    const std::vector<std::vector<bool>> I_shape{
            { false,false,false, true, true, true, true, false,false, false}
    };
    model.currPiece = I_shape;
    model.currPosns = {{9,0}, {9,1}, {9,2}, {9,3}};
    model.rotate_block();

    CHECK(model.currPosns == Positions2);

}


