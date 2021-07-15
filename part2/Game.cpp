#include "Game.hpp"
/*--------------------------------------------------------------------------------

--------------------------------------------------------------------------------*/
Game::Game(game_params g) : filename(g.filename), m_rows(0), m_cols(0), old_matrix(), new_matrix(), game_task_queue(),private_game_task_queue(),working_sem(),m_gen_num(g.n_gen)\
, m_thread_num(g.n_thread), m_tile_hist(), m_threadpool(), interactive_on(g.interactive_on), print_on(g.print_on) {
}
Game::~Game() {
}

const vector<double> Game::gen_hist() const {
	return this->m_gen_hist;
}

const vector<tile_record> Game::tile_hist() const {
	return this->m_tile_hist;
}

uint Game::thread_num() const {
	return this->m_thread_num;
}

/*--------------------------------------------------------------------------------

--------------------------------------------------------------------------------*/
void Game::run() {
	//auto testing_time = std::chrono::system_clock::now();
	_init_game(); // Starts the threads and all other variables you need
	print_board("Initial Board");
	for (uint i = 0; i < m_gen_num; ++i) {
		auto gen_start = std::chrono::system_clock::now();
		_step(i); // Iterates a single generation
		auto gen_end = std::chrono::system_clock::now();
		m_gen_hist.push_back((double)std::chrono::duration_cast<std::chrono::microseconds>(gen_end - gen_start).count());
		print_board(nullptr);
	} // generation loop
	print_board("Final Board");
	_destroy_game();
	//auto testing_time2 = std::chrono::system_clock::now();
	//printf("total time: %lf \n",((double)std::chrono::duration_cast<std::chrono::seconds>(testing_time2 - testing_time).count()));

}

void Game::_init_game() {

	// Create game fields - Consider using utils:read_file, utils::split
	new_matrix = new bool_mat();
	old_matrix = new bool_mat();
	vector<string> strvec = utils::read_lines(this->filename);
	for(uint i = 0 ; i<strvec.size() ; i++){
        vector<bool> bool_row = utils::split2(strvec[i],' ');
		(*old_matrix).push_back(bool_row);
		(*new_matrix).push_back(bool_row);
	}
	(*old_matrix).shrink_to_fit();
	(*new_matrix).shrink_to_fit();
	m_rows = (uint)(*old_matrix).size();
	m_cols = (uint)(*old_matrix)[0].size();
	// Create & Start threads
	if(m_thread_num > m_rows)
		m_thread_num = m_rows;
    m_tile_hist.clear();
    m_threadpool.clear();
    m_gen_hist.clear();
	for(uint i=0; i<m_thread_num;i++){
		auto g_t = new Game_Thread(i,game_task_queue);
		m_threadpool.push_back(g_t);
	}
	for(uint i=0; i<m_thread_num;i++){
		m_threadpool[i]->start();
	}
	m_tile_hist.resize(m_thread_num*m_gen_num);
	 // Testing of your implementation will presume all threads are started here
	 private_game_task_queue.clear();
    uint rows_per_thread = m_rows / m_thread_num;
    for (int i = 0; i < m_thread_num; i++) {
        Game_Task g_t(m_rows, m_cols, i * rows_per_thread,
                      ((i == m_thread_num - 1) ? m_rows - 1 :
                       (i + 1) * rows_per_thread - 1), &old_matrix,
                      &new_matrix, false, working_sem,m_tile_hist,(0)*m_thread_num+i);
        private_game_task_queue.push_back(g_t);
        //game_task_queue.push(g_t);
    }

}

void Game::_step(uint curr_gen) {
	// Push jobs to queue
	   for(int i=0;i<m_thread_num;i++) {
           private_game_task_queue[i].tile_index = curr_gen * m_thread_num + i;
       }
         game_task_queue.push_all(private_game_task_queue,(int)private_game_task_queue.size());
		// Wait for the workers to finish calculating
		for (uint i = 0; i < m_thread_num; i++) {
			working_sem.down();
		}
		if (curr_gen == m_gen_num-1) {
			for (int i = 0; i < m_thread_num; i++) {
				private_game_task_queue[i].game_task_kill = true;
			}
			game_task_queue.push_all(private_game_task_queue,(int)private_game_task_queue.size());
		}

		// Swap pointers between current and next field
		std::swap(new_matrix, old_matrix);
		// NOTE: Threads must not be started here - doing so will lead to a heavy penalty in your grade
	}

void Game::_destroy_game(){
	// Destroys board and frees all threads and resources
	// Not implemented in the Game's destructor for testing purposes.
	// All threads must be joined here
	for (uint i = 0; i < m_thread_num; i++) {
        m_threadpool[i]->join();
    }
	for (uint i=0; i<m_thread_num; i++){
	    delete m_threadpool[i];
	}
    delete old_matrix;
    delete new_matrix;
}

/*--------------------------------------------------------------------------------

--------------------------------------------------------------------------------*/
inline void Game::print_board(const char* header) {

	if(print_on){

		// Clear the screen, to create a running animation
		if(interactive_on)
			system("clear");

		// Print small header if needed
		if (header != nullptr)
			cout << "<------------" << header << "------------>" << endl;

		cout << u8"╔" << string(u8"═") * m_cols << u8"╗" << endl;
		for (uint i = 0; i < m_rows ;++i) {
			cout << u8"║";
			for (uint j = 0; j < m_cols; ++j) {
				cout << (((*old_matrix)[i][j]) ? u8"█" : u8"░");
			}
			cout << u8"║" << endl;
		}
		cout << u8"╚" << string(u8"═") * m_cols << u8"╝" << endl;

		// Display for GEN_SLEEP_USEC micro-seconds on screen
		if(interactive_on)
			usleep(GEN_SLEEP_USEC);
	}

}






