#include "raylib.h"
#include "raymath.h"
#include <vector>

#define WIN_W 1920
#define WIN_H 1080

#define CELL_SIZE 15

#define COLS WIN_W/CELL_SIZE
#define ROWS WIN_H/CELL_SIZE

bool simulation_running = false;
bool grid_active = false;
float update_time = 0.15f;

class IntVector2{
    public:
        int x, y;

};

IntVector2 toIntVector(Vector2 vector){
    return {(int)vector.x, (int)vector.y};
}

void handleMouseInput(std::vector<std::vector<bool>>& grid){
    Vector2 mousePos;
    static bool holding_l = false;
    static bool holding_r = false;
    static IntVector2 startVec, endVec;

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        mousePos = Vector2Clamp(GetMousePosition(), {0, 0}, {WIN_W-1, WIN_H-1});
        startVec = toIntVector(Vector2Divide(mousePos, {CELL_SIZE, CELL_SIZE}));
        holding_l = true;
    }
    else if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && holding_l){
        holding_l = false;
        mousePos = Vector2Clamp(GetMousePosition(), {0, 0}, {WIN_W-1, WIN_H-1});
        endVec = toIntVector(Vector2Divide(mousePos, {CELL_SIZE, CELL_SIZE}));

        if(startVec.x > endVec.x){
            int temp = startVec.x;
            startVec.x = endVec.x;
            endVec.x = temp;
        }
        if(startVec.y > endVec.y){
            int temp = startVec.y;
            startVec.y = endVec.y;
            endVec.y = temp;
        }

        for(int y = startVec.y; y <= endVec.y; y++){
            for(int x = startVec.x; x <= endVec.x; x++){
                grid[y][x] = true;
            }
        }
    }
    else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
        mousePos = Vector2Clamp(GetMousePosition(), {0, 0}, {WIN_W-1, WIN_H-1});
        startVec = toIntVector(Vector2Divide(mousePos, {CELL_SIZE, CELL_SIZE}));
        holding_r = true;
    }
    else if(IsMouseButtonReleased(MOUSE_RIGHT_BUTTON) && holding_r){
        holding_r = false;
        mousePos = Vector2Clamp(GetMousePosition(), {0, 0}, {WIN_W-1, WIN_H-1});
        endVec = toIntVector(Vector2Divide(mousePos, {CELL_SIZE, CELL_SIZE}));

        if(startVec.x > endVec.x){
            int temp = startVec.x;
            startVec.x = endVec.x;
            endVec.x = temp;
        }
        if(startVec.y > endVec.y){
            int temp = startVec.y;
            startVec.y = endVec.y;
            endVec.y = temp;
        }

        for(int y = startVec.y; y <= endVec.y; y++){
            for(int x = startVec.x; x <= endVec.x; x++){
                grid[y][x] = false;
            }
        }
    }
}

void handleKeyboardInput(std::vector<std::vector<bool>>& bufGrid, std::vector<std::vector<bool>>& grid){
    if(IsKeyPressed(KEY_SPACE))
        simulation_running = !simulation_running;
    if(IsKeyPressed(KEY_C)){
        for(int y = 0; y < ROWS; y++){
            for(int x = 0; x < COLS; x++){
                grid[y][x] = false;
                bufGrid[y][x] = false;
                simulation_running = false;
            }
        }
    }
    if(IsKeyPressed(KEY_R)){
        for(int y = 0; y < ROWS; y++){
            for(int x = 0; x < COLS; x++){
                grid[y][x] = GetRandomValue(0, 1);
                simulation_running = false;
            }
        }
        bufGrid = grid;
    }
    if(IsKeyPressed(KEY_DOWN) && update_time <= 0.25f)
        update_time += 0.05f;
    if(IsKeyPressed(KEY_UP) && update_time >= 0.10f)
        update_time -= 0.05f;
    if(IsKeyPressed(KEY_G))
        grid_active = !grid_active;
}

int getAliveNeighbours(std::vector<std::vector<bool>>& grid, int r, int c){
    int alive = 0;
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(i == 0 && j == 0)
                continue;
            if(r+i >= 0 && r+i < ROWS && c+j >= 0 && c+j < COLS){
                alive += grid[r+i][c+j];
            }
        }
    }
    return alive;
}

void updateGrid(std::vector<std::vector<bool>>& grid, std::vector<std::vector<bool>>& bufGrid){

    int alive = 0;
    grid = bufGrid;
        for(int y = 0; y < ROWS; y++){
            for(int x = 0; x < COLS; x++){
                alive = getAliveNeighbours(grid, y, x);
                if(alive < 2 || alive > 3)
                    bufGrid[y][x] = false;
                else if(alive == 3)
                    bufGrid[y][x] = true;
            }
        }
}

int main(){

    std::vector<std::vector<bool>> grid(ROWS, std::vector<bool>(COLS));
    std::vector<std::vector<bool>> bufGrid(ROWS, std::vector<bool>(COLS));

    float time = 0.0f;

    InitWindow(WIN_W, WIN_H, "cellular automata");
    while(!WindowShouldClose()){

        handleMouseInput(bufGrid);
        handleKeyboardInput(bufGrid, grid);

        if(simulation_running){
            time += GetFrameTime();
            if(time >= update_time){
                time -= update_time;
                updateGrid(grid, bufGrid);
            }
        }

        BeginDrawing();
        ClearBackground(grid_active ? DARKGRAY : BLACK);
        for(int y = 0; y < ROWS; y++){
            for(int x = 0; x < COLS; x++){
                DrawRectangle(x*CELL_SIZE, y*CELL_SIZE, grid_active ? CELL_SIZE-1 : CELL_SIZE, grid_active ? CELL_SIZE-1 : CELL_SIZE, bufGrid[y][x] == true ? GREEN : BLACK);
            }
        }

        EndDrawing();
    }


    return 0;
}
