#include<raylib.h>  
#include<iostream>
struct ball
{
    int x=400,y=400,rad=50;
    Color color=RED;
};

int main()
{
    int width = 1280, height = 720;
    InitWindow(width, height, "test");
    InitAudioDevice();
    SetTargetFPS(70);
    std::cout<<"Hello, World!"<<std::endl;
    Sound bounce = LoadSound("res/bounce.mp3");
    ball a;
    
    int x=5,y=5;
    while (!WindowShouldClose())
    {
        a.x+=x;
        if (a.x+a.rad>=width || a.x-a.rad<=0)
        {
            x*=-1;
            PlaySound(bounce);
            a.color = { (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255 };
        }
        a.y+=y;
        if (a.y+a.rad>=height || a.y-a.rad<=0)
        {
            y*=-1;
            PlaySound(bounce);
            a.color={ (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255 };
        }
        ClearBackground(BLACK);
        BeginDrawing();
        DrawLineEx(Vector2{0, (float)height/2}, Vector2{1280, (float)height/2}, 5, DARKBROWN);
        DrawText("Hello, World!", 10, 10, 20, DARKGRAY);
        DrawCircleGradient(a.x, a.y, a.rad, a.color, GRAY);

        DrawFPS(500, 10);
        EndDrawing();
    }
    UnloadSound(bounce);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}