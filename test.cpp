#include "raylib.h"
#include <math.h>

#define MAP_SIZE 32
#define TILE_SIZE 24 // Increased from 8 so the window isn't tiny

struct p {
    float wx, wy, wz, r;
    int mx, my;
    float speed;
};

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

int main() {
    // Initialize Raylib window
    const int screenWidth = MAP_SIZE * TILE_SIZE;
    const int screenHeight = MAP_SIZE * TILE_SIZE;
    InitWindow(screenWidth, screenHeight, "Raylib Top-Down Map");
    SetTargetFPS(60);

    // Initialize Player
    struct p player = {0};
    player.mx = 1;
    player.my = 1;
    // Center player inside the starting tile
    player.wx = player.mx * TILE_SIZE + (TILE_SIZE / 2.0f);
    player.wy = player.my * TILE_SIZE + (TILE_SIZE / 2.0f);
    player.r = PI; // Facing left (180 degrees)
    player.speed = 150.0f; // Scaled up to match the new TILE_SIZE

    bool toggle_map = true;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        
        // --- Inputs & Logic ---
        if (IsKeyPressed(KEY_M)) toggle_map = !toggle_map;

        // Rotation
        if (IsKeyDown(KEY_A)) player.r -= 3.0f * dt;
        if (IsKeyDown(KEY_D)) player.r += 3.0f * dt;

        // Calculate movement deltas based on rotation
        float mov_dist = player.speed * dt;
        float dx = cos(player.r) * mov_dist;
        float dy = sin(player.r) * mov_dist;

        float next_wx = player.wx;
        float next_wy = player.wy;

        if (IsKeyDown(KEY_W)) {
            next_wx += dx;
            next_wy += dy;
        }
        if (IsKeyDown(KEY_S)) {
            next_wx -= dx;
            next_wy -= dy;
        }

        // --- Collision ---
        // Check map grid coordinates for the next position
        int next_mx = (int)(next_wx / TILE_SIZE);
        int next_my = (int)(next_wy / TILE_SIZE);

        // Keep inside bounds and check for walls
        if (next_mx >= 0 && next_mx < MAP_SIZE && next_my >= 0 && next_my < MAP_SIZE) {
            if (map[next_my][next_mx] != '#') {
                player.wx = next_wx;
                player.wy = next_wy;
                player.mx = next_mx;
                player.my = next_my;
            }
        }

        // --- Rendering ---
        BeginDrawing();
        ClearBackground(BLACK);

        if (toggle_map) {
            // Draw Map
            for (int y = 0; y < MAP_SIZE; y++) {
                for (int x = 0; x < MAP_SIZE; x++) {
                    if (map[y][x] == '#') {
                        DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKGRAY);
                    } else {
                        DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, RAYWHITE);
                    }
                    // Optional: Draw grid lines for readability
                    DrawRectangleLines(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, LIGHTGRAY);
                }
            }

            // Draw Player
            DrawCircle((int)player.wx, (int)player.wy, TILE_SIZE / 3.0f, BLUE);

            // Draw Direction Line (Visible Vector)
            Vector2 lineEnd = { 
                player.wx + cos(player.r) * (TILE_SIZE * 1.2f), 
                player.wy + sin(player.r) * (TILE_SIZE * 1.2f) 
            };
            DrawLineEx((Vector2){player.wx, player.wy}, lineEnd, 3.0f, RED);
        }

        // Draw HUD
        DrawRectangle(0, 0, 420, 30, Fade(BLACK, 0.7f));
        DrawText(TextFormat("FPS: %d  |  Pos: [%.1f, %.1f]  |  Rot: %.2f", 
                 GetFPS(), player.wx, player.wy, player.r), 10, 5, 20, GREEN);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}