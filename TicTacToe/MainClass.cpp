#include <SFML/Graphics.hpp>
#include <SFML/System/Err.hpp>
#include <vector>
#include <iostream>

std::vector<std::vector<int>> board(3, std::vector<int>(3, 0));

void printBoard() {
    std::cout << "\n";
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            std::cout << board[row][col] << " ";
        }
        std::cout << std::endl;
    }
}

int main()
{
    const int windowWidth = 800;
    const int windowHeight = 800;
    bool turn = true;

    //creates the board vector by nesting vectors
 
    //gets the float of width and height of each square
    const int gridSizeX = 3;
    const int gridSizeY = 3;
    const float cellWidth = windowWidth / static_cast<float>(gridSizeX);
    const float cellHeight = windowHeight / static_cast<float>(gridSizeY);

    //creates screen objects
    sf::RenderWindow screen(sf::VideoMode(windowWidth, windowHeight), "Tic Tac Toe");
    sf::RectangleShape lineV;
    sf::RectangleShape lineH;

    //creates texture to bloomify
    sf::RenderTexture renderTexture;
    renderTexture.create(windowWidth, windowHeight);

    //draws board
    //vertical lines
    lineV.setSize(sf::Vector2f(windowWidth + 20, cellHeight));
    lineV.setOutlineThickness(10.0f);
    lineV.setFillColor(sf::Color::Transparent);
    lineV.setOutlineColor(sf::Color::White);
    lineV.setPosition(sf::Vector2f(0, cellHeight));

    //horizontal lines
    lineH.setSize(sf::Vector2f(cellWidth, windowHeight + 20));
    lineH.setOutlineThickness(10.0f);
    lineH.setFillColor(sf::Color::Transparent);
    lineH.setOutlineColor(sf::Color::White);
    lineH.setPosition(sf::Vector2f(cellWidth, 0));

    while (screen.isOpen())
    {
        //draws black every frame
        screen.clear(sf::Color(0x0a0d19));
        
        //gets the position of the mouse on the screen with `worldPos`
        sf::Vector2i mousePos = sf::Mouse::getPosition(screen);
        sf::Vector2f worldPos = screen.mapPixelToCoords(mousePos);

        //gets the column and row that the mouse is in based off the size of the cells
        int column = static_cast<int>(worldPos.x / cellWidth);
        int row = static_cast<int>(worldPos.y / cellHeight);

        //creates empty event
        sf::Event event;
        while (screen.pollEvent(event))
        {
            //if event closes screen then it closes
            if (event.type == sf::Event::Closed)
                screen.close();


            if (event.type == sf::Event::MouseButtonReleased) {
                if (turn) {
                    board[row][column] = 1;
                }
                else {
                    board[row][column] = 2;
                }
                printBoard();
                turn = !turn;
            }
        }
        sf::Shader bloomShader;
        if (!bloomShader.loadFromFile("bloom_shader.frag", sf::Shader::Fragment)) {
            // Log the error to the console or a log file
            std::cerr << "Error loading shader: " << bloomShader.isAvailable() << std::endl;

        }

        renderTexture.clear();
        // Draw your content here
        screen.draw(lineV);
        screen.draw(lineH);

        for (int _row = 0; _row < 3; ++_row) {
            for (int col = 0; col < 3; ++col) {
                if (board[_row][col] == 1) {

                    //sets up for drawing each O
                    sf::CircleShape circle(100.0f);
                    circle.setOutlineThickness(10.0f);
                    circle.setOutlineColor(sf::Color::Blue);
                    circle.setPosition(col * cellWidth + (cellWidth - 200) / 2, _row * cellHeight + (cellWidth - 200) / 2);
                    circle.setFillColor(sf::Color::Transparent);

                    screen.draw(circle);
                }
                else if (board[_row][col] == 2) {

                    //sets up for drawing each X
                    sf::RectangleShape x1;
                    sf::RectangleShape x2;
                    //good
                    x1.setSize(sf::Vector2f(239, 10));
                    x1.setRotation(45.0f);
                    x1.setPosition(col * cellWidth + (cellWidth - 170) / 2, _row * cellHeight + (cellWidth - 170) / 2);
                    x1.setFillColor(sf::Color::Red);

                    x2.setSize(sf::Vector2f(239, 10));
                    x2.setRotation(-45.0f);
                    x2.setPosition(col * cellWidth + (cellWidth - 170) / 2 - 6, _row * cellHeight + (cellWidth - 170) / 2 + 169);
                    x2.setFillColor(sf::Color::Red);

                    screen.draw(x1);
                    screen.draw(x2);
                }
            }
        }

        renderTexture.display();
        // Apply the bloom shader to the off-screen buffer
        sf::RenderStates bloomRenderStates;
        bloomRenderStates.shader = &bloomShader;
        bloomRenderStates.blendMode = sf::BlendAdd; // You can adjust the blend mode for different bloom effects

        // Draw the off-screen buffer with the bloom shader applied to the window
        screen.draw(sf::Sprite(renderTexture.getTexture()), bloomRenderStates);

        screen.display();
    }

    return 0;
}