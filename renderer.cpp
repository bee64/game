#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <cmath>

class Sprite {
public:
    SDL_Texture* texture;
    SDL_Rect position;
    
    Sprite(SDL_Texture* tex, int x, int y, int w, int h) {
	    texture = tex;
	    position = (SDL_Rect){x,y,w,h};
    }
};

class Shape {
public:
    enum Type { Rectangle, Circle };
    Type type;
    SDL_Rect bounds;
    SDL_Color color;
    
    Shape(Type t, int x, int y, int w, int h, SDL_Color c) 
        : type(t), bounds((SDL_Rect){x, y, w, h}), color(c) {}
};

class Renderer2D {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<std::shared_ptr<Sprite> > sprites;
    std::vector<std::shared_ptr<Shape> > shapes;
    
public:
    Renderer2D(const char* title, int width, int height) {
        if ( SDL_Init(SDL_INIT_VIDEO) < 0 ){
		printf("SDL could not initialize~! %s\n", SDL_GetError());
	}
        
        window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_SHOWN
        );

	if ( window == NULL ){
		printf("Window could not be created! %s\n", SDL_GetError());
	}
        
        renderer = SDL_CreateRenderer(
            window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        );
    }
    
    ~Renderer2D() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    
    // Add a sprite to the render queue
    void addSprite(std::shared_ptr<Sprite> sprite) {
        sprites.push_back(sprite);
    }
    
    // Add a shape to the render queue
    void addShape(std::shared_ptr<Shape> shape) {
        shapes.push_back(shape);
    }
    
    // Load a texture from a file
    SDL_Texture* loadTexture(const char* path) {
        SDL_Surface* surface = SDL_LoadBMP(path);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        return texture;
    }
    
    // Draw a circle using the midpoint circle algorithm
    void drawCircle(int centerX, int centerY, int radius, SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        
        int x = radius - 1;
        int y = 0;
        int dx = 1;
        int dy = 1;
        int err = dx - (radius << 1);
        
        while (x >= y) {
            SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
            SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
            SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
            SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
            SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
            SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
            SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
            
            if (err <= 0) {
                y++;
                err += dy;
                dy += 2;
            }
            if (err > 0) {
                x--;
                dx += 2;
                err += dx - (radius << 1);
            }
        }
    }
    
    // Render everything in the queue
    void render() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Draw shapes
        for (const auto& shape : shapes) {
            switch (shape->type) {
                case Shape::Type::Rectangle:
                    SDL_SetRenderDrawColor(renderer, 
                        shape->color.r, shape->color.g, 
                        shape->color.b, shape->color.a);
                    SDL_RenderFillRect(renderer, &shape->bounds);
                    break;
                    
                case Shape::Type::Circle:
                    drawCircle(
                        shape->bounds.x + shape->bounds.w/2,
                        shape->bounds.y + shape->bounds.h/2,
                        shape->bounds.w/2,
                        shape->color
                    );
                    break;
            }
        }
        
        // Draw sprites
        for (const auto& sprite : sprites) {
            SDL_RenderCopy(renderer, sprite->texture, nullptr, &sprite->position);
        }
        
        SDL_RenderPresent(renderer);
    }
};

int main() {
    Renderer2D renderer("My 2D Game", 800, 600);
    
    // Create a red rectangle
    auto rect = std::make_shared<Shape>(
        Shape::Type::Rectangle,
        100, 100, 200, 100,
        (SDL_Color){255, 0, 0, 255}
    );
    renderer.addShape(rect);
    
    // Load and create a sprite
    //SDL_Texture* tex = renderer.loadTexture("player.bmp");
    //auto sprite = std::make_shared<Sprite>(tex, 300, 300, 64, 64);
    //renderer.addSprite(sprite);
    
    // Game loop
    bool running = true;
    while (running) {
        // Handle events here
	SDL_Event event;
	SDL_PollEvent( &event );

	if (event.type == SDL_KEYUP) {
		return 0;
	}

	SDL_PumpEvents();
	// TODO: this prints, but only after the game exits. figure logging out
	printf("hey");
        renderer.render();
    }
    
    return 0;
}
