#include <raylib.h>
#include <iostream>
class Ball
{
public:
    float x = 400, y = 400, rad = 50, speedx = 5, speedy = 5;
    Color color = RED;

    void draw()
    {
        DrawCircleGradient(this->x, this->y, this->rad, WHITE, this->color);
    }
    void move(Sound &bounce)
    {
        this->x += speedx;
        this->y += speedy;
        if (this->x + this->rad >= GetScreenWidth() || this->x - this->rad <= 0)
        {
            this->speedx *= -1;
            PlaySound(bounce);
            this->color = {(unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255};
        }
        if (this->y + this->rad >= GetScreenHeight() || this->y - this->rad <= 0)
        {
            this->speedy *= -1;
            PlaySound(bounce);
            this->color = {(unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255};
        }
    }
};
class rectangle
{
public:
    float x = 400, y = 0, width = 250, height = 100;
    Color color = RED;
    void draw()
    {
        DrawRectangle(std::min(this->x = GetMouseX(),GetScreenWidth()- this->width), this->y = GetScreenHeight() - this->height-20, this->width, this->height, this->color);
    }
};

int main()
{
    std::cout << "Hello, World!" << std::endl;
    SetTargetFPS(70);
    float width = 1280, height = 720;
    InitWindow(width, height, "Ponng Game");
    InitAudioDevice();
    Sound bounce = LoadSound("res/bounce.mp3");
    Ball ball;
    rectangle rdown;

    HideCursor();
    while (!WindowShouldClose())
    {
        ball.move(bounce);
        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.rad,Rectangle{rdown.x,rdown.y,rdown.width,rdown.height})){
            ball.speedy *= -1;
            PlaySound(bounce);
            ball.color = {(unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255};
        }
        ClearBackground(BLACK);
        BeginDrawing();
        DrawLineEx(Vector2{0, (float)height / 2}, Vector2{1280, (float)height / 2}, 5, DARKBROWN);
        rdown.draw();
        ball.draw();
        DrawFPS(500, 10);
        EndDrawing();
    }
    UnloadSound(bounce);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}