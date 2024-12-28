int main() {
    Renderer2D renderer("My 2D Game", 800, 600);
    
    // Create a red rectangle
    auto rect = std::make_shared<Shape>(
        Shape::Type::Rectangle,
        100, 100, 200, 100,
        SDL_Color{255, 0, 0, 255}
    );
    renderer.addShape(rect);
    
    // Load and create a sprite
    SDL_Texture* tex = renderer.loadTexture("player.bmp");
    auto sprite = std::make_shared<Sprite>(tex, 300, 300, 64, 64);
    renderer.addSprite(sprite);
    
    // Game loop
    bool running = true;
    while (running) {
        // Handle events here
        renderer.render();
    }
    
    return 0;
}
