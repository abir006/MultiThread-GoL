//
// Created by Abir on 25/05/2019.
//

#ifndef THREAD_HPP_GAME_TASK_HPP
#define THREAD_HPP_GAME_TASK_HPP

#include "Headers.hpp"
#include "Semaphore.hpp"
struct tile_record {
    double tile_compute_time; // Compute time for the tile
    uint thread_id; // The thread responsible for the compute
};
class Game;
struct Game_Task {
    Game_Task(const uint game_num_of_rows,const uint game_num_of_cols, const int task_start_row,
    const int task_end_row, bool_mat** old_game_board,bool_mat** new_game_board,bool
    game_task_kill,Semaphore &sem,vector<tile_record>& m_tile_hist,int tile_index) : game_num_of_rows(game_num_of_rows) , game_num_of_cols(game_num_of_cols), task_start_row(task_start_row),
    task_end_row(task_end_row), old_game_board(old_game_board), new_game_board(new_game_board),game_task_kill(game_task_kill),sem(sem),m_tile_hist(m_tile_hist),tile_index(tile_index){}

    Game_Task(const Game_Task &p1) = default;
    const uint game_num_of_rows;
    const uint game_num_of_cols;
    const int task_start_row;
    const int task_end_row;
    bool_mat** old_game_board;
    bool_mat** new_game_board;
    bool game_task_kill;
    Semaphore &sem;
    vector<tile_record>& m_tile_hist;
    int tile_index;
};



#endif //THREAD_HPP_GAME_TASK_HPP
