#ifndef __GAMERUN_H
#define __GAMERUN_H
#include "Headers.hpp"
#include "Game_Task.hpp"
#include "Game_Thread.hpp"
#include "utils.hpp"
#include "Semaphore.hpp"
/*--------------------------------------------------------------------------------
								  Auxiliary Structures
--------------------------------------------------------------------------------*/
struct game_params {
	// All here are derived from ARGV, the program's input parameters. 
	uint n_gen;
	uint n_thread;
	string filename;
	bool interactive_on; 
	bool print_on; 
};


/*--------------------------------------------------------------------------------
									Class Declaration
--------------------------------------------------------------------------------*/
class Game {
public:

	Game(game_params);
	~Game();
	void run(); // Runs the game
	const vector<double> gen_hist() const; // Returns the generation timing histogram
	const vector<tile_record> tile_hist() const; // Returns the tile timing histogram
	uint thread_num() const; //Returns the effective number of running threads = min(thread_num, field_height)


protected: // All members here are protected, instead of private for testing purposes

	// See Game.cpp for details on these three functions
	void _init_game(); 
	void _step(uint curr_gen); 
	void _destroy_game(); 
	inline void print_board(const char* header);
	//implementation addition
		//added fields to game
	string filename;
	uint m_rows;
	uint m_cols;
    bool_mat* old_matrix;
    bool_mat* new_matrix;
	PCQueue<Game_Task> game_task_queue;
	vector<Game_Task> private_game_task_queue;
	Semaphore working_sem;
	//end of added fields
		//added methods
		//end of added methods
	//end of additional implementation
	uint m_gen_num; 			 		// The number of generations to run
	uint m_thread_num; 			 		// Effective number of threads = min(thread_num, field_height)
	vector<tile_record> m_tile_hist; 	// Shared Timing history for tiles: First m_thread_num cells are the calculation durations for tiles in generation 1 and so on. 
							   	 		// Note: In your implementation, all m_thread_num threads must write to this structure. 
	vector<double> m_gen_hist;  	 	// Timing history for generations: x=m_gen_hist[t] iff generation t was calculated in x microseconds
	vector<Thread*> m_threadpool; 		// A storage container for your threads. This acts as the threadpool.

	bool interactive_on; // Controls interactive mode - that means, prints the board as an animation instead of a simple dump to STDOUT
	bool print_on; // Allows the printing of the board. Turn this off when you are checking performance (Dry 3, last question)


};
#endif
