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

bool inDeque(Vector2 element,deque<Vector2> Body){
    for(Vector2 position : Body){
        if(Vector2Equals(element,position)){
            return true;
        }
    }
    return false;
}
//The Player Snake
class Snake{
public:
    deque<Vector2> snakeBody = {Vector2{8,9},Vector2{7,9},Vector2{6,9}};
    Texture2D snake_Texture;
    Vector2 direction = {1,0};
    bool addSegment = false;

    Snake(){
        Image image = LoadImage("graphics/snake_node.png");
        snake_Texture = LoadTextureFromImage(image);
        UnloadImage(image);
    }
    ~Snake(){
        UnloadTexture(snake_Texture);
    }
    void Draw(){
        for(Vector2 pos : snakeBody){
            DrawTexture(snake_Texture,pos.x*cell_size,pos.y*cell_size,WHITE);
        }
    }
    void Update(){
        snakeBody.push_front(Vector2Add(snakeBody[0],direction));
        if(addSegment == true){
            addSegment = false;
        }
        else{
            snakeBody.pop_back();
        }
    }
    void Reset(){
        snakeBody = {Vector2{8,9},Vector2{7,9},Vector2{6,9}};
        direction = {1,0};
    }
};

//The Food
class Food{
public:
    Texture2D food_texture;
    deque<Vector2> snakeBody;
    Vector2 position; 
    Food(deque<Vector2> snakeBody){
        Image image = LoadImage("graphics/food.png");
        food_texture = LoadTextureFromImage(image);
        UnloadImage(image);
        this->snakeBody = snakeBody;
        position = GenerateRandomPosition(this->snakeBody);
    }

    ~Food(){
        UnloadTexture(food_texture);
    }
    void Draw()
    {
        DrawTexture(food_texture,position.x*cell_size,position.y*cell_size,WHITE);
    }
    Vector2 GenerateRandomCell(){
        float x = GetRandomValue(0,cell_count-1);
        float y = GetRandomValue(0,cell_count-1);
        Vector2 position = {x,y};
        return position;
    }
    Vector2 GenerateRandomPosition(deque<Vector2> Body){
        Vector2 position = GenerateRandomCell();
        while(inDeque(position,Body)){
            position = GenerateRandomCell();
        }
        return position;
    }
};
//Game Class a container for all the components required for the game.
class Game{
public:
    Snake snake;
    Food food = Food(snake.snakeBody);
    bool game = true;

    void Draw(){
        food.Draw();
        snake.Draw();
    }

    void Update(){
        if(game == true){
            snake.Update();
            foodCollision();
            edgeCollision();
            bodyCollision();
        }
    }
    void foodCollision(){
        if(Vector2Equals(snake.snakeBody[0], food.position)){
            food.position = food.GenerateRandomPosition(snake.snakeBody);
            snake.addSegment = true;
        }
    }
    void gameOver(){
        snake.Reset();
        food.position = food.GenerateRandomPosition(snake.snakeBody);
        game = false;
    }
    void edgeCollision(){
        if(snake.snakeBody[0].x == cell_count || snake.snakeBody[0].y == cell_count || snake.snakeBody[0].y == -1 || snake.snakeBody[0].x == -1){
            gameOver();
        }
    }

    void bodyCollision(){
        for(int i = 1; i < snake.snakeBody.size() ; i++){
            if(Vector2Equals(snake.snakeBody[0],snake.snakeBody[i])){
                gameOver();
            }
        }
    }
};
//main call
int main(){

    InitWindow(cell_size*cell_count,cell_size*cell_count,"Snake");
    SetTargetFPS(60);

    Game game;

    //gameloop
    while(WindowShouldClose() == false){

        BeginDrawing();

        //Clear BackGround...
        ClearBackground(background_Color);
        
        //DrawGame....
        game.Draw();
        if(updateEvent(0.15)){
            game.Update();
        }

        //Key Inputs

        if(game.game == false && IsKeyPressed(KEY_ENTER)){
            game.game = true;
        }

        if(IsKeyPressed(KEY_W) && game.snake.direction.y != 1){
            game.snake.direction = {0,-1};
        }
        if(IsKeyPressed(KEY_S) && game.snake.direction.y != -1){
            game.snake.direction = {0,1};
        }
        if(IsKeyPressed(KEY_A) && game.snake.direction.x != 1){
            game.snake.direction = {-1,0};
        }
        if(IsKeyPressed(KEY_D) && game.snake.direction.x != -1){
            game.snake.direction = {1,0};
        }

        //End Of KeyInpus and End Drawing
        EndDrawing();
    }

    //close window and return
    CloseWindow();
    return 0;

}
