#include"Application.h"
#include "glad/glad.h"

Keyboard App::keyboard;
sf::Window App::window;

App::App() 
{
    
}

App::~App() 
{
    delete _game;
}

void App::init() 
{
    init_win();

    //extract opengl functions from driver
    if (!gladLoadGL()) {

        exit(-1);
    }
    //init GL
    glViewport(0, 0, window.getSize().x, window.getSize().y);
    glCullFace(GL_BACK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //init game AFTER context been initialized !!!
    _game = new Game();
}

void App::init_win() 
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    sf::VideoMode winMode(WIN_WIDTH, WIN_HEIGHT);
    window.create(winMode, "Qin's Minecraft", sf::Style::Default, settings);
}

void App::poll_event() 
{
    sf::Event e;
    while (window.pollEvent(e)) {
        App::keyboard.update(e);
        if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)  window.close();
    }
}

void App::loop() 
{    
    while (1) 
    {
        poll_event();
        
        if (App::keyboard.is_key_down(sf::Keyboard::Key::Space)) 
        {
            printf("space pressed\n");
            exit(0);
        }

        _game->update();
        _game->render();
                      
        window.display();
    }
}