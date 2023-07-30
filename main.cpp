#include "raylib.h"
#include <cstdlib>

int main()
{
    // Window dimensions
    const int windowWidth{1000};
    const int windowHeight{600};
    InitWindow(windowWidth, windowHeight, "Breakout!");

    // Loading Sound files
    InitAudioDevice();
    Wave paddleHitWav = LoadWave("sounds/paddle_hit.wav");
    Wave gameOverWav = LoadWave("sounds/game_over.wav");
    Wave brickBreakWav = LoadWave("sounds/brick_break.wav");
    Sound paddleHit = LoadSoundFromWave(paddleHitWav);
    Sound gameOver = LoadSoundFromWave(gameOverWav);
    Sound brickBreak = LoadSoundFromWave(brickBreakWav);
    bool gameOverSoundPlayed = false; // To avoid the sound loop

    // --- TEXT BAR ---
    const int space{25};
    const int b_x{760};
    const int r_x{b_x + space};
    const int e_x{r_x + space};
    const int a_x{e_x + space};
    const int k_x{a_x + space};
    const int o_x{k_x + space};
    const int u_x{o_x + space};
    const int t_x{u_x + space};
    const int title_y{20};
    const int titleFontSize{30};

    // --- WALLS ---
    // Top Wall coordinates
    Rectangle topWall{0, 65, windowWidth, 50};

    // Left Wall coordinates
    Rectangle leftWall{topWall.x, topWall.y + topWall.height, 65, windowHeight - topWall.y - topWall.height};

    // Right Wall coordinates
    Rectangle rightWall{windowWidth - leftWall.width, topWall.y + topWall.height, leftWall.width, windowHeight - topWall.y - topWall.height};

    // Floor coordinates
    Rectangle floor{65, windowHeight - 5, windowWidth - leftWall.width - rightWall.width, 5};

    // --- PADDLE ---
    // Paddle coordinates
    Rectangle paddle{topWall.height + 215, 560, 102, 15};
    const int paddle_speed{9};
    // I need this value in order to detect 5 areas on the top of the Paddle that is moving, so the value will need to update
    int movingPaddle_x = paddle.x;
    
    // --- BALL ---
    // Ball coordinates
    Rectangle ball{350, 320, 15, 15};
    int ball_direction_x{3};
    int ball_direction_y{4};
    // The following values - the middle of the bottom of the Ball, the bottom letf and the bottom right corner of the Ball will be helpful for calculating the points of the collision between the Ball and the Paddle in order to make the Ball correctly bounce into different directions
    Vector2 ball_bottomMiddle;
    ball_bottomMiddle.x = ball.x + ball.width / 2;

    // --- BRICKS ---
    // Bricks dimensions
    Rectangle brick;
    brick.width = 60;
    brick.height = 18;
    const int brickSpacing{5}; 
    int score = 0;

    // Bricks coordinates
    brick.x = topWall.height + 30;
    brick.y = topWall.y + topWall.height + 15;
    Rectangle brickLine2 = brick;
    brickLine2.y = brick.y + brick.height + brickSpacing;
    Rectangle brickLine3 = brick;
    brickLine3.y = brickLine2.y + brick.height + brickSpacing;
    Rectangle brickLine4 = brick;
    brickLine4.y = brickLine3.y + brick.height + brickSpacing;
    Rectangle brickLine5 = brick;
    brickLine5.y = brickLine4.y + brick.height + brickSpacing;
    Rectangle brickLine6 = brick;
    brickLine6.y = brickLine5.y + brick.height + brickSpacing;
    Rectangle brickLine7 = brick;
    brickLine7.y = brickLine6.y + brick.height + brickSpacing;
    Rectangle brickLine8 = brick;
    brickLine8.y = brickLine7.y + brick.height + brickSpacing;

    // Define the Brick Colors and their Y positions so that I can use it later to create a loop in order to draw all the bricks
    Color brickColors[] = { RED, RED, ORANGE, ORANGE, GREEN, GREEN, YELLOW, YELLOW };
    const float brickYPositions[] = { brick.y, brickLine2.y, brickLine3.y, brickLine4.y,
                          brickLine5.y, brickLine6.y, brickLine7.y, brickLine8.y };

    // I need to store each brick in a separate Rectangle variable, as I will need this later in order to manage collision with the ball and each brick to make them disappear. In my game there are 8 lines with 13 bricks each, so a two-dimensional array would be helpful
    const int numLines{8};
    const int numCols{13};
    Rectangle bricks[numLines][numCols];
    // An array of booleans that will help me to determine whether a particular Brick has or hasn't been hit by the Ball. The Brciks hit by the Ball will be hidden (not drawn at all)
    bool isBrickActive[numLines][numCols];
    // The array of booleans where the Bombs will be placed. The array illustrates the actual diagram of the Bricks
    bool hasABomb[numLines][numCols];

    for (int line = 0; line < numLines; line++) 
    {
        for (int col = 0; col < numCols; col++) 
        {
            int brickXPosition = brick.x + col * (brick.width + brickSpacing);
            int brickYPosition = brickYPositions[line];
            bricks[line][col] = { static_cast<float>(brickXPosition), static_cast<float>(brickYPosition),
                                  static_cast<float>(brick.width), static_cast<float>(brick.height) };

            // At the beginning, all the Bricks are visible, as none of them has been hit yet
            isBrickActive[line][col] = true;

            // Initially the are no bombs, as they will be planted later in the code, so -
            hasABomb[line][col] = false;
        }
    }

    // --- BOMBS ---
    // Initially, there will be 8 bombs, one for each row/line
    const int maxBombs = 8;

    // Build a Bomb
    struct Bomb 
    {
        int x;
        int y;
        bool active;
    };

    // Create an array of Bomb structs to store their X, Y and the active boolean
    Bomb bombs[maxBombs];

    // I want to plant 1 bomb at a random brick of each row, so loop through the rows
    for (int rowNum = 0; rowNum < 8; rowNum++)
    {
        // Pull a random number between 0 and 12 and assign the result to colNum
        int colNum = rand() % 13;
        // Plant a bomb at the colNum of the current row
        hasABomb[rowNum][colNum] = true;
    }

    // Player dies if any of the Bombs hits the Paddle
    bool playerDied = false;

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(BLACK);

        if (CheckCollisionRecs(ball, floor))
        {
            DrawText("Game Over!", 350, 240, 50, RED);

            // If statement to avoid the game over sound being looped by the main While loop
            if (!gameOverSoundPlayed)
            {
                // Play sound effect on Game Over
                PlaySound(gameOver);
                gameOverSoundPlayed = true;
            }
        }
        else if (playerDied)
        {
            DrawText("You Died!", 350, 240, 50, RED);

            // If statement to avoid the game over sound being looped by the main While loop
            if (!gameOverSoundPlayed)
            {
                // Play sound effect on Game Over
                PlaySound(gameOver);
                gameOverSoundPlayed = true;
            }
        }
        else
        {
            // Update variables for the collision between the Ball and the Paddle
            ball_bottomMiddle.x = ball.x + ball.width / 2;
            movingPaddle_x = paddle.x;

            // **** DRAW section ****

            // Draw the Text Bar
            // The score on the left
            DrawText(TextFormat("Your Score: %i", score), 30, 25, 25, YELLOW);

            // Game title on the right
            DrawText("B", b_x, title_y, titleFontSize, RED);
            DrawText("R", r_x, title_y, titleFontSize, ORANGE);
            DrawText("E", e_x, title_y, titleFontSize, GREEN);
            DrawText("A", a_x, title_y, titleFontSize, YELLOW);
            DrawText("K", k_x, title_y, titleFontSize, RED);
            DrawText("O", o_x, title_y, titleFontSize, ORANGE);
            DrawText("U", u_x, title_y, titleFontSize, GREEN);
            DrawText("T", t_x, title_y, titleFontSize, YELLOW);

            // Draw the Top Wall
            DrawRectangle(topWall.x, topWall.y, topWall.width, topWall.height, GRAY);

            // Draw the Left Wall
            DrawRectangle(leftWall.x, leftWall.y, leftWall.width, leftWall.height, GRAY);

            // Draw the Right Wall
            DrawRectangle(rightWall.x, rightWall.y, rightWall.width, rightWall.height, GRAY);

            // Draw the Floor
            DrawRectangle(floor.x, floor.y, floor.width, floor.height, BLANK);
            
            // Draw the Paddle
            DrawRectangle(paddle.x, paddle.y, paddle.width, paddle.height, BLUE);

            // Draw all the bricks (8 lines / 13 bricks each) with a nested loop.
            for (int line = 0; line < numLines; line++) 
            {
                for (int col = 0; col < numCols; col++) 
                {
                    // Check if the Brick in this particulat position hasn't been hit by the ball
                    if (isBrickActive[line][col])
                    {
                        // If the Brick is active, then draw the Brick
                        DrawRectangleRec(bricks[line][col], brickColors[line]);
                    }   
                }
            }

            // Draw the Ball
            DrawRectangle(ball.x, ball.y, ball.width, ball.height, BLUE);

            // *** GAME LOGIC section ***

            // Move the Paddle right if the Right Arrow key or the D key is pressed
            if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2)) && paddle.x < windowWidth - paddle.width - rightWall.width - 15)
            {
                paddle.x += paddle_speed;
            }
            else if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_2)) && paddle.x > leftWall.width + 15)
            {
                paddle.x -= paddle_speed;
            }

            // Move the Ball along the x-axis
            ball.x += ball_direction_x;

            // Check the Ball for collision with the Left or the Right Wall
            if (CheckCollisionRecs(ball, leftWall) || CheckCollisionRecs(ball, rightWall))
            {
                // Reverse horizontal direction
                ball_direction_x = -ball_direction_x; 

                // Play sound effect on hit
                PlaySound(paddleHit);
            }

            // Check for collision between the Ball and the Bricks along the x-axis
            for (int line = 0; line < numLines; line++) 
            {
                for (int col = 0; col < numCols; col++) 
                {
                    // If there's a collision between the Ball and a Brick that is still active
                    if (isBrickActive[line][col] && CheckCollisionRecs(ball, bricks[line][col]))
                    {
                        // Hit the left or right side => reverse horizontal direction
                        ball_direction_x = -ball_direction_x;

                        // Deactivate the Brick after the hit
                        isBrickActive[line][col] = false;

                        // If the Brick that was hit has a Bomb planted
                        if (hasABomb[line][col])
                        {
                            // Create a temporary variable bombIndex
                            int bombIndex = -1;
                            // Repeat maxBombs times
                            for (int i = 0; i < maxBombs; i++) 
                            {
                                // If the Bomb is not activated yet
                                if (!bombs[i].active) 
                                {
                                    // Update bombIndex
                                    bombIndex = i;
                                    // Break the loop
                                    break;
                                }
                            }

                            // If bombIndex was updated (the bomb in not active yet)
                            if (bombIndex != -1) 
                            {
                                // Position the Bomb at the center of the Brick that has been hit
                                bombs[bombIndex].x = bricks[line][col].x + bricks[line][col].width / 2;
                                bombs[bombIndex].y = bricks[line][col].y + bricks[line][col].height / 2;
                                // Activate the bomb
                                bombs[bombIndex].active = true;
                            }
                        }

                        // Play sound effect on hit
                        PlaySound(brickBreak);

                        // Update the Score
                        score++;
                    }
                }
            }

            // Move the Ball along the y-axis
            ball.y += ball_direction_y;

            // Check the Ball for collision with the Top Wall
            if (CheckCollisionRecs(ball, topWall))
            {
                // Reverse vertical direction
                ball_direction_y = -ball_direction_y; 

                // Play sound effect on hit
                PlaySound(paddleHit);
            }

            // Check the Ball for collision with the Paddle
            if (CheckCollisionRecs(ball, paddle))
            {
               
                /*
                * Here I'm dividing the top surface of the Paddle into 6 separate parts. 
                * The ball direction will be different, depending on which part od the Paddle 
                * will contact with the bottom middle of the ball.
                * This simple solution adds some simple player control mechanics, 
                * so that they can try to direct the ball to the brick or bricks of their choice.
                */

                if (ball_bottomMiddle.x < movingPaddle_x + paddle.width / 6)
                {
                    // The Ball bounces and goes far to the left
                    ball_direction_x = -4;
                    ball_direction_y = -5;
                }
                else if (ball_bottomMiddle.x >= movingPaddle_x + paddle.width / 6 && ball_bottomMiddle.x < movingPaddle_x + paddle.width / 6 * 2)
                {
                    // The Ball bounces and goes slightly to the left
                    ball_direction_x = -3;
                    ball_direction_y = -5;
                }
                else if (ball_bottomMiddle.x >= movingPaddle_x + paddle.width / 6 * 2 && ball_bottomMiddle.x < movingPaddle_x + paddle.width / 6 * 3)
                {
                    // The Ball bounces and goes straight up (actually it's tiny bit right to avoid a weird issue with the brick collision)
                    ball_direction_x = -1;
                    ball_direction_y = -5;
                }
                else if (ball_bottomMiddle.x >= movingPaddle_x + paddle.width / 6 * 3 && ball_bottomMiddle.x < movingPaddle_x + paddle.width / 6 * 4)
                {
                    // The Ball bounces and goes slightly to the right
                    ball_direction_x = 1;
                    ball_direction_y = -5;
                }
                else if (ball_bottomMiddle.x >= movingPaddle_x + paddle.width / 6 * 4 && ball_bottomMiddle.x < movingPaddle_x + paddle.width / 6 * 5)
                {
                    // The Ball bounces and goes far to the right
                    ball_direction_x = 3;
                    ball_direction_y = -5;
                }
                else if (ball_bottomMiddle.x >= movingPaddle_x + paddle.width / 6 * 5)
                {
                    // The Ball bounces and goes far to the right
                    ball_direction_x = 4;
                    ball_direction_y = -5;
                }
                // The else statement in case any other kind of collision, so hopefully only when the ball hits collides with the left or the right side of the paddle
                else
                {
                    ball_direction_x = -ball_direction_x;
                }

                // Play sound effect on hit
                PlaySound(paddleHit);

            }

            // Check each Brick for collision with the Ball along the y-axis
            for (int line = 0; line < numLines; line++) 
            {
                for (int col = 0; col < numCols; col++) 
                {
                    // If there's a collision between the Ball and a Brick that is still active
                    if (isBrickActive[line][col] && CheckCollisionRecs(ball, bricks[line][col]))
                    {
                        // Hit the top or bottom, reverse vertical direction
                        ball_direction_y = -ball_direction_y;

                        // Deactivate the Brick after the hit
                        isBrickActive[line][col] = false;

                        if (hasABomb[line][col])
                        {
                            // Create a temporary variable bombIndex
                            int bombIndex = -1;
                            // Repeat maxBombs times
                            for (int i = 0; i < maxBombs; i++) 
                            {
                                // If the Bomb is not active
                                if (!bombs[i].active) 
                                {
                                    // Update bombIndex
                                    bombIndex = i;
                                    // Break the loop
                                    break;
                                }
                            }

                            // If bombIndex was updated (the bomb in not active yet)
                            if (bombIndex != -1) 
                            {
                                // Position the Bomb at the center of the Brick that has been hit
                                bombs[bombIndex].x = bricks[line][col].x + bricks[line][col].width / 2;
                                bombs[bombIndex].y = bricks[line][col].y + bricks[line][col].height / 2;
                                // Activate the bomb
                                bombs[bombIndex].active = true;
                            }
                        }

                        // Play sound effect on hit
                        PlaySound(brickBreak);

                        // Update the Score
                        score++;
                    }
                }
            }

            // Draw a Bomb if it's active
            for (int i = 0; i < maxBombs; i++) 
            {
                // If the Bomb is activated
                if (bombs[i].active) 
                {
                    // Draw the Bomb
                    DrawCircle(bombs[i].x, bombs[i].y, 6, RED);

                    // Drop the Bomb
                    bombs[i].y++;

                    // If the Bomb hits the Floor
                    if(bombs[i].active && CheckCollisionCircleRec({static_cast<float>(bombs[i].x), static_cast<float>(bombs[i].y)}, 6, floor))
                    {
                        // Deactivate the Bomb
                        bombs[i].active = false;
                    }

                    // If the Bomb hits the Paddle
                    if(bombs[i].active && CheckCollisionCircleRec({static_cast<float>(bombs[i].x), static_cast<float>(bombs[i].y)}, 6, paddle))
                    {
                        // Kill the Player
                        playerDied = true;
                    }
                }
            }

        }   

        EndDrawing();
    }
    UnloadSound(paddleHit);
    UnloadSound(gameOver);
    UnloadSound(brickBreak);
    UnloadWave(paddleHitWav);
    UnloadWave(gameOverWav);
    UnloadWave(brickBreakWav);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}