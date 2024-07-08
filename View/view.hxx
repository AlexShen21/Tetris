#pragma once

#include "model.hxx"

class View
{
public:

    explicit View(Model const& model, ge211::Mixer& mixer);

    using Position = ge211::Posn<int>;
    using Dimensions = ge211::Dims<int>;

    void draw(ge211::Sprite_set& set) ;
    View::Position board_to_screen(Model::Position) const;
    Dimensions initial_window_dimensions() const;

    void resume_music_if_ended() const;

private:
    Model const& model_;

    //Sprites
    ge211::Rectangle_sprite pink_square;
    ge211::Rectangle_sprite red_square;
    ge211::Rectangle_sprite orange_square;
    ge211::Rectangle_sprite yellow_square;
    ge211::Rectangle_sprite green_square;
    ge211::Rectangle_sprite blue_square;
    ge211::Rectangle_sprite purple_square;
    ge211::Rectangle_sprite const background_piece;

    // Vector of sprites
    std::vector<ge211::Rectangle_sprite> colors;

    //Score
    ge211::Text_sprite score_sprite;
    ge211::Text_sprite over_sprite;
    ge211::Font sans{"sans.ttf", 40};

    // Sound
    void load_audio_();
    ge211::Mixer& mixer_;
    ge211::Music_track music_;
};
