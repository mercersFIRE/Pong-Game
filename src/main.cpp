#include <raylib.h>
#include <iostream>
#include <cmath>
class Ball
{
public:
    float x = 400, y = 400, rad = 50, speedx = 4, speedy = 4,speedtotal=6,alpha=0.0f;
    Color color = RED;

    void draw(float ColorAlpha)
    {
        if(alpha<1.0f)alpha += 0.04f;
        DrawCircleGradient(this->x, this->y, this->rad, WHITE, Fade(this->color,std::min(ColorAlpha,alpha)));
    }
    void move(Sound &bounce)
    {
        if (this->x + this->rad >= GetScreenWidth() || this->x - this->rad <= 0)
        {
            this->speedx *= -1;
            PlaySound(bounce);
            this->color = {(unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255};
            alpha=0.0f;
        }
        if (this->y + this->rad >= GetScreenHeight() || this->y - this->rad <= 0)
        {
            this->speedy *= -1;
            PlaySound(bounce);
            this->color = {(unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255};
            alpha=0.0f;
        }
        this->x += speedx;
        this->y += speedy;
    }

    int lost(float rdowny, Sound &lost, long long &score)
    {
        if(this->y + this->rad > rdowny){
            int textWidth = MeasureText("Press SPACE To Play Again", 50);
            PlaySound(lost);
            float alpha = 0.0f;
            while (!(IsKeyPressed(KEY_SPACE)||IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
            {
                ClearBackground(BLACK);
                BeginDrawing();
                alpha += 0.01f;
                if (alpha > 1.0f) alpha = 1.0f;
                DrawText("Press SPACE To Play Again", (GetScreenWidth() - textWidth) / 2, (GetScreenHeight() - 50) / 2, 50, Fade(WHITE, alpha));
                DrawText(TextFormat("Your Score : %ld", score), (GetScreenWidth() - MeasureText(TextFormat("Your Score : %ld", score),50) ) / 2, (GetScreenHeight() + 50) / 2, 50, Fade(WHITE, alpha));
                EndDrawing();
                if(WindowShouldClose()||IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))return 1;
            }
            this->x=GetScreenWidth()/2;
            this->y=GetScreenHeight()/2;
            this->speedx=3;
            this->speedy=3;
            this->speedtotal=6;
            score=0;
        }
        return 0;
    }
};

class rectangle
{
public:
    float x = 400, y =  GetScreenHeight() - this->height-20 , width = 300, height = 20;
    Color color = {232, 181, 72,100};
    void draw(float ColorAlpha)
    {
        DrawRectangleRounded(Rectangle{std::min(this->x = GetMouseX(),GetScreenWidth()- this->width), this->y = GetScreenHeight() - this->height-20, this->width, this->height},.9,0,Fade( this->color,ColorAlpha));
    }
};
int loadmenu()
{
    int textWidth = MeasureText("Press SPACE To Start", 50);
    float alpha = 0.0f;
    while (!(IsKeyPressed(KEY_SPACE)||IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
    {
        ClearBackground(BLACK);
        BeginDrawing();
        alpha += 0.01f;
        if (alpha > 1.0f) alpha = 1.0f;
        DrawText("Press SPACE To Start", (GetScreenWidth() - textWidth) / 2, (GetScreenHeight() - 50) / 2, 50, Fade(WHITE, alpha));
        DrawText("Press ESC To Exit", (GetScreenWidth() - MeasureText("Press ESC To Exit", 50)) / 2, (GetScreenHeight() + 50) / 2, 50, Fade(WHITE, alpha));
        EndDrawing();
        if(WindowShouldClose()||IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))return 1;
    }
    return 0;
}
int main()
{
    long long score = 0;
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetTargetFPS(70);
    float width = 1920, height = 1080, ColorAlpha = 0.0f;
    InitWindow(width, height, "Pong Game");
    ToggleFullscreen();
    InitAudioDevice();

    Sound bounce = LoadSound("res/bounce2.wav"), lost=LoadSound("res/lost.wav"), music = LoadSound("res/music.mp3");
    Ball ball;
    rectangle rdown;
    if(loadmenu())return 1;

    HideCursor();
    while (!IsKeyPressed(KEY_ESCAPE))
    {
        if(ball.lost(rdown.y,lost,score))break;
        ball.move(bounce);

        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.rad,Rectangle{rdown.x,rdown.y,rdown.width,rdown.height})){
            float diff = ball.x - (rdown.x + rdown.width/2);
            ball.speedx= ball.speedtotal*diff/rdown.width*2;
            ball.speedy=sqrt(ball.speedtotal*ball.speedtotal-ball.speedx*ball.speedx);
            ball.speedy *= -1;
            ball.speedtotal+=.5;
            ball.move(bounce);
            PlaySound(bounce);
            score+=ball.speedtotal/2;
            ball.alpha=0.0f;
            ball.color = {(unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255};
        }
        if(ColorAlpha<1.0f)
        ColorAlpha += 0.01f;
        ClearBackground(BLACK);
        BeginDrawing();
        DrawText(TextFormat("Score: %lld", score), 10, 50, 50, Fade(WHITE, ColorAlpha));
        DrawLineEx(Vector2{0, (float)height / 2}, Vector2{(float)GetScreenWidth(), (float)height / 2}, 5, Fade(DARKBROWN,ColorAlpha));
        rdown.draw(ColorAlpha);
        ball.draw(ColorAlpha);
        DrawFPS(10, 10);
        EndDrawing();
        if(!IsSoundPlaying(music))PlaySound(music);
    }
    if(loadmenu())return 1;
    UnloadSound(bounce);
    UnloadSound(lost);
    UnloadSound(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}