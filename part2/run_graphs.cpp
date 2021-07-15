
void run_graphs() {
    for (uint n = 1; n <= 20; ++n) {
        game_params params = {100, n,
                              "small.txt",
                              false, false};

        Game g(params);
        g.run();
        calc_and_append_statistics(g.thread_num(), g.gen_hist(), g.tile_hist());
    }
    for (uint n = 1; n <= 64; ++n) {
        game_params params = {100, n,
                              "mid.txt",
                              false, false};

        Game g(params);
        g.run();
        calc_and_append_statistics(g.thread_num(), g.gen_hist(), g.tile_hist());
    }
    for (uint n = 1; n <= 100; ++n) {
        cout << " " << n;
        game_params params = {100, n,
                              "big.txt",
                              false, false};

        Game g(params);
        g.run();
        calc_and_append_statistics(g.thread_num(), g.gen_hist(), g.tile_hist());
    }
}