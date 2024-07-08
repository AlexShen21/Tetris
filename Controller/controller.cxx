#include <iostream>
#include "controller.hxx"

Controller::Controller()
        : model_(),
          view_(model_, mixer())

{ }

void
Controller::draw(ge211::Sprite_set& set)
{
    view_.draw(set);
}

void
Controller::on_frame(double dt)
{
    if (model_.gameover){
        return;
    }
    view_.resume_music_if_ended();
    model_.move_block_down_on_timer();
    model_.reset_on_tetris();
    model_.game_over();
    //model_.check_kill_block_timer();


}

void
Controller::on_key(ge211::Key key)
{
    if(!model_.gameover){
    if (key == ge211::Key::left()) {
        model_.move_block_left();
    } else if (key == ge211::Key::right()) {
        model_.move_block_right();
    } else if (key == ge211::Key::code(' ')) {
        model_.kill_block();
    } else if (key == ge211::Key::code('z')) {
        model_.rotate_block();
    } else if (key == ge211::Key::down()) {
        model_.move_block_down_on_key();
    }}
}

std::string
Controller::initial_window_title() const
{
    return "Tetris";
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}