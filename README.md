# 2023_flappy_bird
1. For mac, you might need to download the four extension libraries referenced in the report.pptx file.
and compile with the following command in terminal. 
2. click main to play the flappy bird.

### Challenges and Overcoming Them:
1. Latency:
  - Challenge: The update rate was unstable.
  - Solution: Implemented the SDL_GetTicks timing function to make real-time adjustments to the update speed, reducing latency by 50% to under 5%.

2. Readability and Maintainability:
  - Challenge: The code was disorganized and hard to read.
  - Solution: Consolidated objects into classes, cut down the code by 50%, and enhanced readability.

### Screen shots
1. Start: Pressing space to start the game.
<img width="489" alt="start" src="https://github.com/tommy60718/2023_flappy/assets/128281234/2ddc339c-cc2a-4290-8bb2-64ad3af59811">

2. Manipulate:Player can press “space” to bounce up, to avoid colliding with Pipes.
- Sounds such as :
  - background music
  - swing sound
  - hit sound

<img width="367" alt="gaming" src="https://github.com/tommy60718/2023_flappy/assets/128281234/fb2ae995-cb6f-4572-b5e5-ca58419644bb">

3. dead: when bird collide with the pipe or out of boundary, then Game Over.

<img width="416" alt="game_over" src="https://github.com/tommy60718/2023_flappy/assets/128281234/db598801-d05d-498a-becd-1d662d837736">

### Libraries
1. Libraries I use.
- The game using SDL library, and its extensions.
  - SDL : Core library for graphic, events, and window managements.
  - SDL_image : Image (loading & manipulating).
  - SDL_ttf : Font.
  - SDL_mixer : Audio loading, mixing.
2. What is SDL?
- low level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D. 
3. installation within terminal（Mac):
- brew install sdl2
- brew install sdl2_image
- brew install sdl2_ttf
- brew install sdl2_mixer
4. reference:
- SDL webpage: https://www.libsdl.org/


### Important components
1. Start and update elements

<img width="627" alt="startn" src="https://github.com/tommy60718/2023_flappy/assets/128281234/59ed9912-63dd-41b1-9cbe-75db2dcedfbf">

2. End game

<img width="672" alt="endgame" src="https://github.com/tommy60718/2023_flappy/assets/128281234/d4207019-b18c-4fa7-ba68-726c5cb9be1c">

3. Functions for updating the location of bird & Pipe.

<img width="688" alt="update" src="https://github.com/tommy60718/2023_flappy/assets/128281234/be199913-131f-4b1a-bb8f-fed62ae9198b">

4. Render function for bird & pipe

<img width="721" alt="rander" src="https://github.com/tommy60718/2023_flappy/assets/128281234/4962b4f9-241e-468e-808a-a857322c9d7f">

5. Function of collision.

<img width="720" alt="collision" src="https://github.com/tommy60718/2023_flappy/assets/128281234/5df3d5d9-fbc2-4885-84e2-11f46556c5e9">







