#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color red = {223, 85, 80, 255};
Color green = {173, 204, 96, 255};
Color dark_green = {43, 51, 24, 255};

int cell_size = 25;
int cell_count = 25;
int offset = 50;

double last_update_time = 0;

bool eventTriggered(double interval)
{
    double current_time = GetTime();
    if (current_time - last_update_time >= interval) 
    {
        last_update_time = current_time;
        return true;
    }
    return false;
}

bool elementInDeque(Vector2 element, deque<Vector2> deque) 
{
    for (unsigned int i = 0; i < deque.size(); i++) 
    {
        if (Vector2Equals(deque[i], element)) 
        {
            return true;
        }
    }
    return false;
}

class Snake
{

public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = {1, 0};
    bool add_segment = false;

    void Draw()
    {
        for (unsigned int i = 0; i < body.size(); i++) 
        {
            int x = body[i].x;
            int y = body[i].y;
            Rectangle segment = {(float)((x * cell_size) + offset), 
                                 (float)((y * cell_size) + offset), 
                                 (float)cell_size, 
                                 (float)cell_size};
            DrawRectangleRounded(segment, 0.5f, 6, dark_green);
        }
    }

    void Update()
    {
        if (add_segment)
        {
            add_segment = false;
        } 
        else
        {
            body.pop_back();
        }
        body.push_front(Vector2Add(body[0], direction));
    }

    void Reset()
    {
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = {1, 0};
    }

};

class Food 
{

public:
    Vector2 position; // numbers represent cells, not pixels

    Food(deque<Vector2> snake_body) // constructor of the object
    {
        position = GenerateRandomPos(snake_body);
    }

    void Draw() 
    {
        Rectangle my_rect = {(float)((position.x * cell_size) + offset), 
                             (float)((position.y * cell_size) + offset), 
                             (float)cell_size, 
                             (float)cell_size};
        DrawRectangleRounded(my_rect, 1.0f, 0, red);
    }

    Vector2 GenerateRandomCell()
    {
        float x = GetRandomValue(0, cell_count - 1);
        float y = GetRandomValue(0, cell_count - 1);
        return Vector2{x, y}; // numbers represent cells, not pixels
    }

    Vector2 GenerateRandomPos(deque<Vector2> snake_body)
    {
        do 
        {
            position = GenerateRandomCell();
        } 
        while ((elementInDeque(position, snake_body)));
        return position;
    }

};

class Game
{

public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;
    int score = 0;

    void Draw()
    {
        food.Draw();
        snake.Draw();
    }

    void Update()
    {
        if (running) 
        {
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
        }
    }

    void CheckCollisionWithFood()
    {
        if (Vector2Equals(snake.body[0], food.position))
        {
            food.position = food.GenerateRandomPos(snake.body);
            snake.add_segment = true;
            score++;
        }
    }

    void CheckCollisionWithEdges()
    {
        if (snake.body[0].x == cell_count || snake.body[0].x == -1)
        {
            GameOver();
        }
        if (snake.body[0].y == cell_count || snake.body[0].y == -1)
        {
            GameOver();
        }
    }

    void CheckCollisionWithTail()
    {
        deque<Vector2> headless_body = snake.body;
        headless_body.pop_front();
        if (elementInDeque(snake.body[0], headless_body))
        {
            GameOver();
        }
    }

    void GameOver()
    {
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
        score = 0;
    }

};

int main () 
{

    cout << "Starting the game..." << endl;
    int window_size = cell_size * cell_count;
    InitWindow(window_size + (2 * offset), window_size + (2 * offset), 
              "Retro Snake");
    SetTargetFPS(60);

    Game game = Game();

    while (WindowShouldClose() == false) 
    {
        BeginDrawing();

        if (eventTriggered(0.2)) 
        {
            game.Update();
        }

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) 
        {
            game.snake.direction = {0, -1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) 
        {
            game.snake.direction = {0, 1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) 
        {
            game.snake.direction = {-1, 0};
            game.running = true;
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) 
        {
            game.snake.direction = {1, 0};
            game.running = true;
        }

        // Drawing
        ClearBackground(green);
        Rectangle window_rect = {(float)offset - 5, 
                                 (float)offset - 5, 
                                 (float)window_size + 10, 
                                 (float)window_size + 10};
        DrawRectangleLinesEx(window_rect, 5, dark_green);
        DrawText("Retro Snake", offset - 5, 20, 25, dark_green);
        DrawText(TextFormat("%i", game.score),
                 offset + window_size - 10, 20, 25, dark_green);
        game.Draw();

        EndDrawing();
    }


    CloseWindow();
    return 0;
}
