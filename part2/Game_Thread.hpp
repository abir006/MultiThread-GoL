//
// Created by Abir on 25/05/2019.
//

#ifndef THREAD_HPP_GAME_THREAD_HPP
#define THREAD_HPP_GAME_THREAD_HPP
#include "Headers.hpp"
#include "Thread.hpp"
#include "PCQueue.cpp"

class Game_Thread : public Thread {

public:
    Game_Thread(uint thread_id,PCQueue<Game_Task>& pcq_ptr) : Thread(thread_id), game_task_queue(pcq_ptr){}
protected:
    void thread_workload() override {
        while (true) {
            Game_Task game_task = game_task_queue.pop();
            if (game_task.game_task_kill){
                (game_task.sem).up();
                return;
            }
            auto tile_start = std::chrono::system_clock::now();
            for (int current_row = game_task.task_start_row; current_row <= game_task.task_end_row; current_row++) {  //iterate over the needed rows
                for (int current_col = 0; current_col < game_task.game_num_of_cols; current_col++) {  //iterate over all the cols for the row
                    Cell_Update(current_row, current_col, *game_task.new_game_board, *game_task.old_game_board,game_task.game_num_of_rows,game_task.game_num_of_cols);
                }
            }
            auto tile_end = std::chrono::system_clock::now();
            tile_record curr_tile;
            curr_tile.thread_id = this->thread_id();
            curr_tile.tile_compute_time = ((double)std::chrono::duration_cast<std::chrono::microseconds>(tile_end - tile_start).count());
            game_task.m_tile_hist[game_task.tile_index] = curr_tile;
            (game_task.sem).up();
        }
    };
    void Cell_Update(int &i,int &j,bool_mat *new_game_board, bool_mat *old_game_board,uint game_num_of_rows,uint game_num_of_cols){
        int neighbours=0;
        neighbours = is_Cell_Alive(i-1,j-1,old_game_board,game_num_of_rows,game_num_of_cols) +
                is_Cell_Alive(i-1,j,old_game_board,game_num_of_rows,game_num_of_cols) +
        is_Cell_Alive(i-1,j+1,old_game_board,game_num_of_rows,game_num_of_cols) +
        is_Cell_Alive(i,j-1,old_game_board,game_num_of_rows,game_num_of_cols) +
        is_Cell_Alive(i,j+1,old_game_board,game_num_of_rows,game_num_of_cols) +
        is_Cell_Alive(i+1,j-1,old_game_board,game_num_of_rows,game_num_of_cols) +
        is_Cell_Alive(i+1,j,old_game_board,game_num_of_rows,game_num_of_cols) +
        is_Cell_Alive(i+1,j+1,old_game_board,game_num_of_rows,game_num_of_cols);
        if((((*old_game_board)[i][j]) && (neighbours==2 || neighbours==3)) || ((false==((*old_game_board)[i][j]))&&(neighbours==3))){
            (*new_game_board)[i][j] = true;
        }
        else{
            (*new_game_board)[i][j] = false;
        }
    };

    int is_Cell_Alive(int i,int j, bool_mat *old_game_board,uint game_num_of_rows,uint game_num_of_cols){
        if (i<0 || j<0 || i>=game_num_of_rows || j >= game_num_of_cols){
            return 0;
        }
        return (int)(*old_game_board)[i][j];
    }

        private:
    PCQueue<Game_Task> &game_task_queue;
};
#endif //THREAD_HPP_GAME_THREAD_HPP
