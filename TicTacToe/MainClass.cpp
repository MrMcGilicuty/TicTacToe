#include <SFML/Graphics.hpp>
#include <SFML/System/Err.hpp>
#include <vector>
#include <cmath>
#include <iostream>

const float PI = 3.14159265359f; // Define the value of pi manually

//function for testing; prints board
void printBoard(const std::vector<std::vector<int>>& board) {
    std::cout << "\n";
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            std::cout << board[row][col] << " ";
        }
        std::cout << std::endl;
    }
}

//checks the board and returns who won
int checkWinner(std::vector<std::vector<int>>& board) {
    //check rows
    for (int x = 0; x < 3; ++x) {
        if (board[x][0] == board[x][1] && board[x][1] == board[x][2]) {
            if (board[x][2] != 0) return board[x][2];
        }
    }

    //check columns
    for (int y = 0; y < 3; ++y) {
        if (board[0][y] == board[1][y] && board[1][y] == board[2][y]) {
            if (board[2][y] != 0) return board[2][y];
        }
    }

    //check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != 0) {
        return board[0][0];
    }

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != 0) {
        return board[0][2];
    }

    //no winner
    return 0; 
}

//checks the board and returns where they won
sf::Vector2f checkWinner(std::vector<std::vector<int>>& board, bool start, float cellWidth, float cellHeight) {
    //check rows for winner start and end
    sf::Vector2f location;
    for (int x = 0; x < 3; ++x) {
        if (board[x][0] == board[x][1] && board[x][1] == board[x][2] && board[x][0] != 0) {
            if (start) {
                location = sf::Vector2f(0 * cellWidth + 15, x * cellHeight + (cellWidth / 2));
                return location;
            }
            //end
            else {
                location = sf::Vector2f(2 * cellWidth + 255, x * cellHeight + (cellWidth / 2));
                return location;
            }
        }
    }

    //check columns for winner start and end
    for (int y = 0; y < 3; ++y) {
        if (board[0][y] == board[1][y] && board[1][y] == board[2][y] && board[2][y] != 0) {
            if (start) {
                location = sf::Vector2f(y * cellWidth + cellWidth / 2, 0 * cellHeight + 15);
                return location;
            }
            else {
                location = sf::Vector2f(y * cellWidth + cellWidth / 2, 2 * cellHeight + 255);
                return location;
            }
        }
    }

    //check diagonals for winner start and end
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != 0) {
        if (start) {
            location = sf::Vector2f(0 * cellWidth + (cellWidth - 200) / 2, 0 * cellHeight + (cellWidth - 200) / 2);
            return location;
        }
        //end
        else {
            location = sf::Vector2f(3 * cellWidth - (cellWidth - 200) / 2, 3 * cellHeight - (cellWidth - 200) / 2);
            return location;
        }
    }

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[2][0] != 0) {
        if (start) {
            location = sf::Vector2f(0 * cellWidth + (cellWidth - 200) / 2, 2 * cellHeight + 200 + (cellWidth - 200) / 2);
            return location;
        }
        else {
            location = sf::Vector2f(2 * cellWidth + 200 + (cellWidth - 200) / 2, 0 * cellHeight + (cellWidth - 200) / 2);
            return location;
        }
    }
    //return a default value if no winner is found
    return sf::Vector2f(0, 0);
}

//returns the line to be drawn
sf::RectangleShape winLine(const sf::Vector2f& start, const sf::Vector2f& end, sf::Color color) {
    //stinky trig required here
    sf::RectangleShape line;
    line.setPosition(start);
    line.setSize(sf::Vector2f(std::hypot(end.x - start.x, end.y - start.y), 5.0f));
    line.setFillColor(color);
    //imma be honest I didn't make this formula at all
    line.setRotation(std::atan2(end.y - start.y, end.x - start.x) * 180.0f / PI); // Use the manually defined PI
    return line;
}

int main()
{
    //seting constants
    const int windowWidth = 800;
    const int windowHeight = 800;
    int winner = 0;
    bool turn = true;
    bool finish = false;

    //creates the board vector by nesting vectors
    std::vector<std::vector<int>> board(3, std::vector<int>(3, 0));

    //gets the float of width and height of each square
    const int gridSizeX = 3;
    const int gridSizeY = 3;
    const float cellWidth = windowWidth / static_cast<float>(gridSizeX);
    const float cellHeight = windowHeight / static_cast<float>(gridSizeY);

    //creates screen objects
    sf::RenderWindow screen(sf::VideoMode(windowWidth, windowHeight), "Tic Tac Toe");
    sf::RectangleShape lineV;
    sf::RectangleShape lineH;

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


            if (event.type == sf::Event::MouseButtonReleased && !finish) {
                if (turn && board[row][column] == 0) {
                    board[row][column] = 1;
                }
                else if (board[row][column] == 0) {
                    board[row][column] = 2;
                }
                else {
                    turn = !turn;
                }
                printBoard(board);

                // Draw the winning line directly on the screen
                if (checkWinner(board) != 0) {
                    finish = true;
                    winner = checkWinner(board);
                }
                
                //cycles turn
                turn = !turn;
            }
        }
        
        // Get the start and end positions of the winning line
        sf::Vector2f start = checkWinner(board, true, cellWidth, cellHeight);
        sf::Vector2f end = checkWinner(board, false, cellWidth, cellHeight);

        //draw stuff here:
        screen.draw(lineV);
        screen.draw(lineH);

        //when game over draw lines
        if (finish) {
            if (winner == 1) {
                screen.draw(winLine(start, end, sf::Color::Blue));
            }
            else {
                screen.draw(winLine(start, end, sf::Color::Red));
            }
        }

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
        screen.display();
    }

    return 0;
}
