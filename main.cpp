#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define TARGET_FPS 60
#define FRAME_TIME_USEC (1000000 / TARGET_FPS)
#define MAP_SIZE 32
#define TILE_SIZE 8

int toggle_map = 0;
int height, width, ch;
char *display_buffer = NULL;

struct p {
    float wx, wy, wz, r;
    int mx = 1, my = 1;
    float speed = 50.0;
};

struct p player;

const char *map[MAP_SIZE] = {
    "################################",
    "#              ##              #",
    "# #### ####### ## ####### #### #",
    "# #  # #     # ## #     # #  # #",
    "# #### ####### ## ####### #### #",
    "#                              #",
    "# #### ## ########## ## ####   #",
    "#      ##     ##     ##        #",
    "###### ### ######## ### ########",
    "#    # ##            ## #      #",
    "#    # ## ########## ## #      #",
    "###### ## #        # ## ########",
    "#         #        #           #",
    "###### ## #        # ## ########",
    "#    # ## ########## ## #      #",
    "#    # ##            ## #      #",
    "###### ## ########## ## ########",
    "#              ##              #",
    "# #### ####### ## ####### #### #",
    "#   ##         ##         ##   #",
    "### ## ## ########## ## ## #####",
    "#      ##     ##     ##        #",
    "# ########### ## ###########   #",
    "# #           ##           #   #",
    "# # ######### ## ######### #   #",
    "# #           ##           #   #",
    "# ########### ## ###########   #",
    "#                              #",
    "# ########## #  # ##########   #",
    "# #        # #  # #        #   #",
    "# #        # #  # #        #   #",
    "################################"
};









void init_buf() {
    getmaxyx(stdscr, height, width);
    if (display_buffer) free(display_buffer);
    display_buffer = (char*)malloc(height * width + 1);
}

void init() {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);
    player.wz = 10;
    player.wx = player.mx * TILE_SIZE;
    player.wy = player.my * TILE_SIZE;
    player.r = 0;
}

void insert(int y, int x, char v) {
    if (y >= 0 && y < height && x >= 0 && x < width) 
        display_buffer[width * y + x] = v;
}

void draw() {
    memset(display_buffer, ' ', height * width);
    //insert((int)player.wy, (int)player.wx, '@');
    if(toggle_map == 1){
        for(int i = 0; i<MAP_SIZE; i++)
            for(int j = 0; j<MAP_SIZE; j++) insert(i+1, j, map[i][j]);
        insert(player.my + 1, player.mx, '*');
    }
    mvaddnstr(0, 0, display_buffer, height * width);
}

void inputs(){

    ch = getch();
    if (ch == ERR) return;
    // Use a small constant for movement or calculate actual delta
    float dt = 1.0 / TARGET_FPS; 
    if (ch == 'd'){
        player.wx += player.speed * dt;
        player.mx = player.wx / TILE_SIZE;
    }
    else if (ch == 'a'){
        player.wx -= player.speed * dt;
        player.mx = player.wx / TILE_SIZE;
    }
    else if (ch == 's'){
        player.wy += player.speed * dt;
        player.my = player.wy / TILE_SIZE;
    }
    else if (ch == 'w'){
        player.wy -= player.speed * dt;
        player.my = player.wy / TILE_SIZE;
    }
    else if (ch == 'm') toggle_map = !(toggle_map);
    
    
}










int main() {
    init();
    init_buf();

    struct timespec start, end;
    float fps = 0.0; // Units per second

    while (1) {
        clock_gettime(CLOCK_MONOTONIC, &start);

        inputs();

        draw();
        attron(A_BOLD);
        mvprintw(0, 0, "FPS: %2.0f  |  Pos: [%2.1f, %2.1f]", fps, player.wx, player.wy);
        refresh();

        clock_gettime(CLOCK_MONOTONIC, &end);
        long elapsed = (end.tv_sec - start.tv_sec) * 1000000 + 
                       (end.tv_nsec - start.tv_nsec) / 1000;

        insert(0, 0, 'F');
        insert(1, 0, 'P');
        insert(2, 0, 'S');
        if (elapsed < FRAME_TIME_USEC) {
            usleep(FRAME_TIME_USEC - elapsed);
        }
        struct timespec total_end;
        clock_gettime(CLOCK_MONOTONIC, &total_end);
        double total_frame_time = (total_end.tv_sec - start.tv_sec) + 
                                  (total_end.tv_nsec - start.tv_nsec) / 1e9;
        fps = 1.0 / total_frame_time;
    }

    if (display_buffer) free(display_buffer);
    endwin();
    return 0;
}