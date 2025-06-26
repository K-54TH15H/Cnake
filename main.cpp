#include <raylib.h>
#include <iostream>

using namespace std;

Color background_Color = {23,35,38,255};
Color snake_Color = {213,72,72,255};
Color food_color = {35,135,48,255};

int cell_size = 20;
int cell_count = 40;

class Food{
public:
    Texture2D food_texture;
    Vector2 position = GenerateRandomPosition();
    Food(){
        Image image = LoadImage("graphics/food.png");
        food_texture = LoadTextureFromImage(image);
        UnloadImage(image);
    }

    ~Food(){
        UnloadTexture(food_texture);
    }
    void Draw()
    {
        DrawTexture(food_texture,position.x*cell_size,position.y*cell_size,WHITE);
    }

    Vector2 GenerateRandomPosition(){
        float x = GetRandomValue(0,cell_count-1);
        float y = GetRandomValue(0,cell_count-1);

        return Vector2{x,y};
    }
};
int main(){

    InitWindow(cell_size*cell_count,cell_size*cell_count,"Snake");
    SetTargetFPS(60);

    Food food;

    while(WindowShouldClose() == false){
        BeginDrawing();
        //Game Drawing...

        //Clear BackGround...
        ClearBackground(background_Color);

        //DrawFood....
        food.Draw();
        EndDrawing();
    }
    CloseWindow();
    return 0;

}
