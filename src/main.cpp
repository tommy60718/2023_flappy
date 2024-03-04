//easy: 1.music
//hard: 2.Add openCV to replace bird by face
 
#include <iostream>
#include <"/header/SDL.h">
#include <"/header/SDL_image.h">
#include <"/header/SDL_ttf.h">
#include <"header/SDL_mixer.h">

#include <cmath> // For std::atan2
#include <vector>
#include <random>
#include <string>
//#include "/usr/local/include/SDL2/SDL_ttf.h"

using namespace std;

// constants
// const float gravity = 0.04f; // Adjust as needed
// const float jumpStrength = -2.7f; // Adjust as needed
// const float moveSpeed = 1.4f; // pipe & background speed
const float gravity = 0.014f; // Adjust as needed
const float jumpStrength = -1.6f; // Adjust as needed
const float moveSpeed = 0.6f; // pipe & background speed
const float windowWidth = 640;
const float windowHeight = 480;

// structs
struct Bird {
    float x, y;
    float velocity;
    SDL_Texture* texture;
};
struct Pipe {
    float x, y; // Position of the top left corner of the bottom pipe
    SDL_Texture* texture; // Texture for the pipe
    static const int width = 50; // Width of the pipe
    static const int gapHeight = 160; // Gap between the top and bottom pipes
};
class Background {
public:
    //initializer
    Background(const std::string& imagePath, SDL_Renderer* renderer, int windowWidth, int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight), renderer(renderer) {
        // Load the textures
        texture1 = LoadTexture(imagePath, renderer);
        texture2 = LoadTexture(imagePath, renderer);
        // Initialize positions
        x1 = 0;
        x2 = windowWidth-8;
    }
    ~Background() {
        if (texture1) SDL_DestroyTexture(texture1);
        if (texture2) SDL_DestroyTexture(texture2);
    }

    void Update() {
        // Update positions
        x1 -= moveSpeed;
        x2 -= moveSpeed;
        // Reset positions when off-screen
        if (x1 + windowWidth < 0) x1 = windowWidth-8;
        if (x2 + windowWidth < 0) x2 = windowWidth-8;
    }
    void Reset(){
        x1 = 0;
        x2 = windowWidth-8;
    }
    void Render() const {
        // Render the backgrounds
        SDL_Rect bgRect1 = { static_cast<int>(x1), 0, windowWidth+10, windowHeight };
        SDL_Rect bgRect2 = { static_cast<int>(x2), 0, windowWidth+10, windowHeight };
        SDL_RenderCopy(renderer, texture1, NULL, &bgRect1);
        SDL_RenderCopy(renderer, texture2, NULL, &bgRect2);
    }

private:
    SDL_Texture* LoadTexture(const std::string& path, SDL_Renderer* renderer) {
        SDL_Texture* newTexture = nullptr;
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface) {
            newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
            SDL_FreeSurface(loadedSurface);
        }
        return newTexture;
    }
    SDL_Texture* texture1;
    SDL_Texture* texture2;
    float x1, x2;
    int windowWidth, windowHeight;
    SDL_Renderer* renderer;
};

// Function to load textures
SDL_Texture* LoadTexture(const char* filePath, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(filePath);
    if (loadedSurface != nullptr) {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    } else {
        SDL_Log("Unable to load image %s! SDL_image Error: %s\n", filePath, IMG_GetError());
    }
    return newTexture;
}

// Function to update bird's & Pipe's position
void UpdateBird(Bird& bird, const float gravity) {
    bird.velocity += gravity; // Apply gravity
    bird.y += bird.velocity; // Update position
}
void UpdatePipes(vector<Pipe>& pipes, float speed, int windowWidth, int windowHeight) {
    for (auto& pipe : pipes) {
        pipe.x -= speed; // Move pipes to the left
        if(pipe.x + Pipe::width <0){ //the pipe is off-screen to the left
            pipe.x = windowWidth;

            int maxPipeTopY = windowHeight - Pipe::gapHeight - 100; //randomize between top~100+gapHeight
            pipe.y = rand() % maxPipeTopY + Pipe::gapHeight; //randomize the height of pipe pair
        }

    }
}

// Function to render the bird with angle
void RenderBird(SDL_Renderer* renderer, const Bird& bird) {
    SDL_Rect birdRect = { static_cast<int>(bird.x), static_cast<int>(bird.y), 34, 24 }; // Adjust bird size as needed
    // Calculate angle based on velocity
    double angle = std::atan2(bird.velocity, 20.0) * 180.0 / M_PI; // Convert to degrees
    // Render bird with angle
    SDL_RenderCopyEx(renderer, bird.texture, NULL, &birdRect, angle, NULL, SDL_FLIP_NONE);
}
void RenderPipes(SDL_Renderer* renderer, const vector<Pipe>& pipes) {
    for (const auto& pipe : pipes) {
        // Bottom pipe
        SDL_Rect bottomPipeRect = { static_cast<int>(pipe.x), static_cast<int>(pipe.y), Pipe::width, static_cast<int>(windowHeight - pipe.y) };
        SDL_RenderCopy(renderer, pipe.texture, NULL, &bottomPipeRect);

        // Top pipe (assuming the gap is constant)
        SDL_Rect topPipeRect = { static_cast<int>(pipe.x), 0, Pipe::width, static_cast<int>(pipe.y) - Pipe::gapHeight };
        //SDL_RenderCopy(renderer, pipe.texture, NULL, &topPipeRect);
        SDL_RenderCopyEx(renderer, pipe.texture, NULL, &topPipeRect, 0, NULL, SDL_FLIP_VERTICAL); //upside down the pipe
    }
}
void RenderText(SDL_Renderer* renderer, TTF_Font* font, const string& text, SDL_Color textColor, int x, int y){
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(),textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect = {x,y,surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

//Collision check
bool CheckCollision ( const Bird& bird, const vector<Pipe>& pipes, int windowHeight){
    SDL_Rect birdRect = { static_cast<int>(bird.x), static_cast<int>(bird.y), 34,24};

    for(const auto& pipe: pipes){
        SDL_Rect bottomPipeRect = { static_cast<int>(pipe.x), static_cast<int>(pipe.y), Pipe::width, windowHeight - static_cast<int>(pipe.y) };
        if(SDL_HasIntersection(&birdRect, &bottomPipeRect)){
            return true;
        }
        SDL_Rect topPipeRect = { static_cast<int>(pipe.x), 0, Pipe::width, static_cast<int>(pipe.y) - Pipe::gapHeight };
        if(SDL_HasIntersection(&birdRect, &topPipeRect)){
            return true;
        }
    }
    // Check collision with the bottom boundary (lowered by 100 units)
    if (bird.y + 24 > windowHeight + 100) { // 24 is the height of the bird
        return true;
    }
    return false;
}

//reset game
void resetGame(Bird& bird, std::vector<Pipe>& pipes, SDL_Texture* pipeTexture, Background& bg, bool& gameStarted, bool& gameOver, bool& hitFlag, Mix_Music* bgMusic, int &score) {
    bird.x = 320; // Reset bird's X position
    bird.y = 240; // Reset bird's Y position
    bird.velocity = 0; // Reset bird's velocity

    pipes.clear(); // Clear existing pipes
    for (int i = 1; i <= 3; i++) { // Reinitialize pipes
        int pipeHeight = rand() % (300 - Pipe::gapHeight) + Pipe::gapHeight;
        pipes.push_back({static_cast<float>(300 + i * 225), static_cast<float>(pipeHeight), pipeTexture});
    }

    bg.Reset(); // Reset background position if needed

    gameStarted = false;
    gameOver = false;
    hitFlag = false;
    score = 0;
    Mix_HaltMusic(); // Stop any playing music
}

int main(int argc, char* argv[]) {
    //---------initialize-------------
    //Initialize SDL_mixer
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    return -1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return -1;
    }
    // Initialize SDL, font
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
        return -1; // Add return statement to exit if TTF_Init fails
    }
    int fontSize = 54; // Declare fontSize
    TTF_Font* font = TTF_OpenFont("/Resources/font/font.ttf", fontSize); // Load your font and set the size
    TTF_Font* smallFont= TTF_OpenFont("/Resources/font/font.ttf", 34);
    if (!font) {
        cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << endl;
        return -1; // Add return statement to exit if font loading fails
    }
    SDL_Color textColor = {245, 245, 245}; // near white color

    // Seed for random number generation
    srand(static_cast<unsigned int>(time(nullptr)));
    //------------initialize-----------------



    // Create window
    SDL_Window* window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }
    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); // Set default render color



    //-----------------------load-------------------------------
    //Load bird, pipe(initialize), background
    // Load bird texture
    Bird bird;
    bird.x = 320; // Starting X position
    bird.y = 240; // Starting Y position
    bird.velocity = 0; // Starting velocity
    bird.texture = LoadTexture("/Resources/images/bird.png", renderer); // Replace with your bird texture path
    // Load pipe texture
    SDL_Texture* pipeTexture = LoadTexture("/Resources/images/pipe.png", renderer); // Replace with your pipe texture path
    // Initialize pipes
    vector<Pipe> pipes;
    for(int i=1; i<=3; i++){
        int pipeHeight = rand() % (300 - Pipe::gapHeight) + Pipe::gapHeight;
        pipes.push_back({static_cast<float>(300+i*225),static_cast<float>(pipeHeight), pipeTexture});
    }
    //initialize background using class initializer
    Background bg("/Resources/images/sky3.jpg.avif" ,renderer, windowWidth, windowHeight);
    
    //Load music
    Mix_Music *bgMusic = Mix_LoadMUS("/Resources/music/will_you_dance.mp3"); // Replace with the path to your music file
    if (bgMusic == NULL) {
        printf("Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError());
        // Handle error, e.g., exit the program or continue without music
    }
    Mix_Chunk *swingSound = Mix_LoadWAV("/Resources/music/wing_sound.ogg");
    Mix_Chunk *hitSound = Mix_LoadWAV("/Resources/music/hit_sound.ogg");
    

    //-------------------------Flags for main loop------------------------
    // Main loop flag
    bool quit = false;
    // Event handler
    SDL_Event e;
    //gameStart & gameOver
    bool gameStarted = false;
    bool gameOver= false;
    bool hitFlag=false;
    //score & time
    int score =0;
    int lastScoreUpdateTime = SDL_GetTicks();
    int lastFrameTime = SDL_GetTicks();

    //-------------------------- Main game loop----------------------
    while (!quit) {
        // reaction to keyboard
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            //-------------------------User presses a key------------------
            else if (e.type == SDL_KEYDOWN) {
                if (!gameStarted) {
                    gameStarted = true; // Start the game on the first key press
                }
                // Space key makes bird jump
                if (e.key.keysym.sym == SDLK_SPACE) {
                    bird.velocity = jumpStrength;
                    Mix_PlayChannel(-1,swingSound,0); //play once
                }
                if(e.key.keysym.sym == SDLK_TAB){
                    resetGame(bird,pipes,pipeTexture, bg, gameStarted, gameOver, hitFlag, bgMusic,score);
                }
            }
        }
        
        //-------------------------renders------------------------------
        // Clear screen
        SDL_RenderClear(renderer);
        // Render bird & Pipe (order of rendering matters)
        bg.Render();// Render the background
        RenderBird(renderer, bird);
        RenderPipes(renderer, pipes);
        string scoreText = "Score: "+ to_string(score);
        RenderText(renderer,smallFont,scoreText, textColor, 10,windowHeight-50);
        //-------------------------gameStart, process, gameOver--------------------------
        //Texts "Press space to start"
        if (!gameStarted) {
            // Render the "Press space to start" message
            RenderText(renderer, font, "Press space to start", textColor, 100,200);
        }
        if(gameStarted && !gameOver){
            //update score according to time
            int currentTime = SDL_GetTicks();
            if(currentTime - lastScoreUpdateTime >= 1100){
                score ++;
                lastScoreUpdateTime = currentTime;
            }
            UpdateBird(bird, gravity);// Update bird
            UpdatePipes(pipes, moveSpeed, windowWidth, windowHeight); // Update Pipe (object, speed, width, height)
            bg.Update();// Update the background
            //check collision
            if(CheckCollision (bird, pipes, windowHeight)) gameOver = true ;
            //music
            if(!Mix_PlayingMusic()){
                Mix_PlayMusic(bgMusic, -1);
            }
            
        }
        //Texts  "Game over"
        if (gameOver) {
            RenderText(renderer,font,"Game Over",textColor,100,200);
            //hit
            if(hitFlag == false){
                Mix_PlayChannel(-1, hitSound, 0);
                hitFlag = true;
            }
            //stop music
            Mix_HaltMusic();
            
        }
        
        // Update screen
        SDL_RenderPresent(renderer);
    }
    //--------------------free------------------
    //freeSDL_ttf
    TTF_CloseFont(font);
    TTF_Quit();

    // Cleanup
    Mix_FreeChunk(swingSound);
    
    // Free resources and close SDL
    SDL_DestroyTexture(bird.texture);
    SDL_DestroyTexture(pipeTexture);
    for (auto& pipe : pipes) {
        SDL_DestroyTexture(pipe.texture); // If each pipe has its own texture
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}