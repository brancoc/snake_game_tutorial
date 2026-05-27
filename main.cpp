#include <iostream>
#include <raylib.h>

using namespace std;

Color red = {223, 85, 80, 255};
Color green = {173, 204, 96, 255};
Color dark_green = {43, 51, 24, 255};

int cell_size = 30;
int cell_count = 25;

class Food 
{

public:
    Vector2 position; // numbers represent cells, not pixels

    Food() // constructor of the object
    {
        position = GenerateRandomPos();
    }

    void Draw() 
    {
        Rectangle myRect = {static_cast<float>(position.x * cell_size), 
                            static_cast<float>(position.y * cell_size), 
                            static_cast<float>(cell_size), 
                            static_cast<float>(cell_size)};
        DrawRectangleRounded(myRect, 1.0f, 0, red);
    }

    Vector2 GenerateRandomPos()
    {
        float x = GetRandomValue(0, cell_count - 1);
        float y = GetRandomValue(0, cell_count - 1);
        return Vector2{x, y}; // Numbers represent cells, not pixels
    }

};


int main () 
{

    cout << "Starting the game..." << endl;
    int size = cell_size * cell_count;
    InitWindow(size, size, "Retro Snake");
    SetTargetFPS(60);

    Food food = Food();

    while (WindowShouldClose() == false) 
    {
        BeginDrawing();

        // Drawing
        ClearBackground(green);
        food.Draw();

        EndDrawing();
    }


    CloseWindow();
    return 0;
}