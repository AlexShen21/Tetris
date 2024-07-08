#include <iostream>
#include "view.hxx"
#include "model.hxx"

static int const grid_size = 64;
View::View(Model const& model, ge211::Mixer& mixer)
        : model_(model),
        // Loading sprites
          pink_square({-2+initial_window_dimensions().width/10,
                       -2+initial_window_dimensions().height/20},{255,192,
                                                                  203}),
          red_square({-2+initial_window_dimensions().width/10,
                      -2+initial_window_dimensions().height/20},{255,0,0, 95}),
          orange_square({-2+initial_window_dimensions().width/10,
                         -2+initial_window_dimensions().height/20},{255,165,
                                                                    0, 85}),
          yellow_square({-2+initial_window_dimensions().width/10,
                         -2+initial_window_dimensions().height/20},
                        {255,255,100}),
          green_square({-2+initial_window_dimensions().width/10,
                        -2+initial_window_dimensions().height/20},{0,255,0,
                                                                   75}),
          blue_square({-2+initial_window_dimensions().width/10,
                       -2+initial_window_dimensions().height/20},{0,0,255, 75}),
          purple_square({-2+initial_window_dimensions().width/10,
                         -2+initial_window_dimensions().height/20},{160,32,
                                                                    240, 75}),
          background_piece({-2+initial_window_dimensions().width/10,
                            -2+initial_window_dimensions().height/20}, {255,255,
                                                                        255}),
        // Sound
        mixer_(mixer)
{
    // Vector holding all the color options
    colors.push_back(pink_square);
    colors.push_back(red_square);
    colors.push_back(orange_square);
    colors.push_back(yellow_square);
    colors.push_back(green_square);
    colors.push_back(blue_square);
    colors.push_back(purple_square);

    // Music
    if (mixer_.is_enabled()) {
        load_audio_();
    }
}

void
View::draw(ge211::Sprite_set& set)
{
    for(Position p: model_.currPosns){
        set.add_sprite(colors[model_.curr_color], board_to_screen(p), 2);
    }

    // Draws background
    for (Position p : model_.all_positions()) {
        set.add_sprite(background_piece, board_to_screen(p), 0);
    }

    // Draws all the placed blocks
    for (Model::live_block s : model_.bricks) {
        ge211::Posn<int> posn = {s.rect.x, s.rect.y};
        set.add_sprite(colors[s.color],board_to_screen(posn),1);
    }

    // Score / Text stuff
    ge211::Text_sprite::Builder current_score(sans);
    current_score.color({0,0,0}) << model_.score;
    score_sprite.reconfigure(current_score);
    set.add_sprite(score_sprite, board_to_screen({0,0}), 5);

    if(model_.gameover){
    ge211::Text_sprite::Builder over(sans);
    over.color({0,0,0}) << "Game Over Please Exit and Reboot";
    over_sprite.reconfigure(over);
    set.add_sprite(over_sprite, board_to_screen({0,10}), 5);
}
}

View::Dimensions
View::initial_window_dimensions() const
{
    // You can change this if you want:
    return grid_size * model_.all_positions().dimensions();
}


View::Position View::board_to_screen(Model::Position pos) const {
    pos.x *= grid_size;
    pos.y *= grid_size;
    return pos;
}

void
View::resume_music_if_ended() const
{
    // restart music if it stops. Checked each frame
    if (mixer_.get_music_state() == ge211::Mixer::State::paused) {
        mixer_.resume_music();
    }
}

static std::string const music_filename =
        "trendsetter-mood-maze-main-version-02-53-1004 (1).mp3";

void
View::load_audio_()
{
    // Load audio
    music_.try_load(music_filename, mixer_);

    // Start background music
    mixer_.play_music(music_);
}