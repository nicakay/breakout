### Breakout
### An old-school Breakout / Arkanoid game creasted in C++ and Raylib

![](https://github.com/nicakay/breakout/blob/main/gifs/bomb_suicide.gif)

# Main features

* The game visual style and colors are based on the look of the original Breakout released on Arcade Atari in 70s.
* There are 8 rows like in the Arcade version, however in my game there are 13 columns of bricks. In the original Atari there were 14.
* The game supports Controllers
* Whether you fail or beat the game, you can restart it and try again by pressing the corresponding button
* Upon starting or restarting the game, it generates 8 bombs. Each bomb is planted in a random position of each row. Upon breaking the corresponding brick the bomb will show up and will start falling down
* There are 3 different game end screens: 'Game Over' (+ a failure sound) - appears if the ball is missed (collides with the invisible floor), 'You Died' (+ a failure sound) - appears if the paddle collides with any of the falling bombs, and 'Victory' (+ an applause sound) - appears as soon as the player hits the last brick.
