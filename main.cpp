#include <raylib.h>
#include <iostream>
#include <deque>
#include <raymath.h>
using namespace std;

//Required Colors
Color background_Color = {23,35,38,255};
Color snake_Color = {213,72,72,255};
Color food_color = {35,135,48,255};

//Grid Logic
int cell_size = 20;
int cell_count = 40;
double last_update_time = 0;

bool updateEvent(double interval){
    double current_time = GetTime();
    if(current_time - last_update_time >= interval){
        last_update_time = current_time;
        return true;
    }
    else{
        return false;
    }
}
//The Player Snake
class Snake{
public:
    deque<Vector2> Body = {Vector2{8,9},Vector2{7,9},Vector2{6,9}};
    Texture2D snake_Texture;
    Vector2 direction = {1,0};

    Snake(){
        Image image = LoadImage("graphics/snake_node.png");
        snake_Texture = LoadTextureFromImage(image);
        UnloadImage(image);
    }
    ~Snake(){
        UnloadTexture(snake_Texture);
    }
    void Draw(){
        for(Vector2 pos : Body){
            DrawTexture(snake_Texture,pos.x*cell_size,pos.y*cell_size,WHITE);
        }
    }
    void Update(){
        Body.pop_back();
        Body.push_front(Vector2Add(Body[0],direction));
    }
};

//The Food
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

//main call
int main(){

    InitWindow(cell_size*cell_count,cell_size*cell_count,"Snake");
    SetTargetFPS(60);

    Food food;
    Snake snake;

    //gameloop
    while(WindowShouldClose() == false){

        BeginDrawing();

        //Clear BackGround...
        ClearBackground(background_Color);
        
        //DrawFood....
        food.Draw();
        snake.Draw();
        if(updateEvent(0.15)){
            snake.Update();
        }
        if(IsKeyPressed(KEY_W) && snake.direction.y != 1){
            snake.direction = {0,-1};
        }
        else if(IsKeyPressed(KEY_S) && snake.direction.y != -1){
            snake.direction = {0,1};
        }
        else if(IsKeyPressed(KEY_A) && snake.direction.x != 1){
            snake.direction = {-1,0};
        }
        else if(IsKeyPressed(KEY_D) && snake.direction.x != -1){
            snake.direction = {1,0};
        }
        EndDrawing();
    }

    //close window and return
    CloseWindow();
    return 0;

}
