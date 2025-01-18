#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <ctime>
#include <windows.h>

using namespace std;

class TicTacToe
{
private:
    char board[10][10];
    char currentPlayer;
    bool gameOver;
    int movesCount;
    int boardSize;

    bool checkWin()
    {
        int winLength;
        if (boardSize == 3)
            winLength = 3;
        else if (boardSize == 4)
            winLength = 4;
        else
            winLength = 5;

        // Check rows
        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j <= boardSize - winLength; j++)
            {
                bool win = true;
                for (int k = 0; k < winLength - 1; k++)
                {
                    if (board[i][j + k] == ' ' || board[i][j + k] != board[i][j + k + 1])
                    {
                        win = false;
                        break;
                    }
                }
                if (win)
                    return true;
            }
        }
        // Check columns
        for (int j = 0; j < boardSize; j++)
        {
            for (int i = 0; i <= boardSize - winLength; i++)
            {
                bool win = true;
                for (int k = 0; k < winLength - 1; k++)
                {
                    if (board[i + k][j] == ' ' || board[i + k][j] != board[i + k + 1][j])
                    {
                        win = false;
                        break;
                    }
                }
                if (win)
                    return true;
            }
        }
        // Check diagonals (top-left to bottom-right)
        for (int i = 0; i <= boardSize - winLength; i++)
        {
            for (int j = 0; j <= boardSize - winLength; j++)
            {
                bool win = true;
                for (int k = 0; k < winLength - 1; k++)
                {
                    if (board[i + k][j + k] == ' ' || board[i + k][j + k] != board[i + k + 1][j + k + 1])
                    {
                        win = false;
                        break;
                    }
                }
                if (win)
                    return true;
            }
        }
        // Check diagonals (top-right to bottom-left)
        for (int i = 0; i <= boardSize - winLength; i++)
        {
            for (int j = boardSize - 1; j >= winLength - 1; j--)
            {
                bool win = true;
                for (int k = 0; k < winLength - 1; k++)
                {
                    if (board[i + k][j - k] == ' ' || board[i + k][j - k] != board[i + k + 1][j - k - 1])
                    {
                        win = false;
                        break;
                    }
                }
                if (win)
                    return true;
            }
        }

        return false;
    }

public:
    string loggedInUser;
    string userID;
    void initializeGame(int size)
    {
        boardSize = size;
        // Initialize empty board
        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                board[i][j] = ' ';
            }
        }
        currentPlayer = 'X';
        gameOver = false;
        movesCount = 0;
    }

    void displayBoard()
    {
        cout << "\n\t\t\t\t-----------------------------------------";
        cout << "\n\t\t\t\t         *    TIC TAC TOE    *";
        cout << "\n\t\t\t\t-----------------------------------------";
        cout << "\n  ";
        for (int j = 1; j <= boardSize; j++)
        {
            cout << j << " ";
        }
        cout << endl;

        for (int i = 0; i < boardSize; i++)
        {
            cout << i + 1 << " ";
            for (int j = 0; j < boardSize; j++)
            {
                cout << board[i][j];
                if (j < boardSize - 1)
                    cout << "|";
            }
            cout << endl;
            if (i < boardSize - 1)
            {
                cout << "  ";
                for (int k = 0; k < (boardSize * 2 - 1); k++)
                {
                    cout << (k % 2 == 0 ? "-" : "+");
                }
                cout << endl;
            }
        }
        cout << endl;
    }

    bool makeMove(int row, int col)
    {
        row--;
        col--;

        if (row >= 0 && row < boardSize && col >= 0 && col < boardSize && board[row][col] == ' ')
        {
            board[row][col] = currentPlayer;
            movesCount++;

            if (checkWin())
            {
                gameOver = true;
                system("cls");
                displayBoard();
                cout << "Player " << currentPlayer << " wins!" << endl;
                getch();
                return true;
            }

            if (movesCount == boardSize * boardSize)
            {
                gameOver = true;
                system("cls");
                displayBoard();
                cout << "It's a draw!" << endl;
                getch();
                return true;
            }

            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            return true;
        }
        return false;
    }

    void saveGame()
    {
        if (loggedInUser.empty() || userID.empty())
        {
            cout << "No user logged in. Cannot save game!" << endl;
            return;
        }

        time_t now = time(0);
        char dateBuffer[20];
        strftime(dateBuffer, sizeof(dateBuffer), "%Y%m%d_%H%M%S", localtime(&now));
        string filename = loggedInUser + "_" + userID + "_" +
                          to_string(boardSize) + "x" + to_string(boardSize) + "_" +
                          dateBuffer + "_game.txt";

        ofstream saveFile(filename);
        if (!saveFile.is_open())
        {
            cout << "Unable to save game!" << endl;
            return;
        }

        saveFile << "Saved on: " << dateBuffer << endl;
        saveFile << boardSize << endl;
        saveFile << movesCount << endl;

        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                saveFile << board[i][j];
            }
            saveFile << endl;
        }
        saveFile << currentPlayer << endl;
        saveFile.close();

        ofstream indexFile("saved_games_index.txt", ios::app);
        indexFile << filename << endl;
        indexFile.close();

        cout << "Game saved successfully!" << endl;
    }

    bool loadGame(string username, string userID)
    {
        const int MAX_SAVED_GAMES = 100;
        string savedGameFiles[MAX_SAVED_GAMES];
        int fileCount = 0;
        string searchPattern = username + "_" + userID + "_" +
                               to_string(boardSize) + "x" + to_string(boardSize);

        ifstream fileList("saved_games_index.txt");
        string filename;
        while (getline(fileList, filename) && fileCount < MAX_SAVED_GAMES)
        {
            if (filename.find(searchPattern) != string::npos &&
                filename.find("_game.txt") != string::npos)
            {
                savedGameFiles[fileCount++] = filename;
            }
        }
        fileList.close();

        if (fileCount == 0)
        {
            cout << "No saved " << boardSize << "x" << boardSize << " games found for this user!" << endl;
            getch();
            return false;
        }

        system("cls");
        cout << "\n\t\tAvailable " << boardSize << "x" << boardSize << " saved games:" << endl;
        for (int i = 0; i < fileCount; i++)
        {
            ifstream tempFile(savedGameFiles[i]);
            string savedDateTime;
            getline(tempFile, savedDateTime);
            string dateStr = savedDateTime.substr(10, 8); // Get YYYYMMDD
            string timeStr = savedDateTime.substr(19, 4); // Get HHMM

            // Format as YYYY-MM-DD HH:MM
            string formattedDateTime = dateStr.substr(0, 4) + "-" +
                                       dateStr.substr(4, 2) + "-" +
                                       dateStr.substr(6, 2) + " " +
                                       timeStr.substr(0, 2) + ":" +
                                       timeStr.substr(2, 2);

            cout << "\n\t\t" << (i + 1) << ". Game saved on: " << formattedDateTime << endl;
            tempFile.close();
        }

        cout << "\n\n\t\tEnter choice (or 0 to start new game): ";
        int choice;
        cin >> choice;
        if (choice == 0)
            return false;
        if (choice < 1 || choice > fileCount)
        {
            cout << "Invalid choice!" << endl;
            return false;
        }

        string selectedFile = savedGameFiles[choice - 1];
        ifstream loadFile(selectedFile);
        if (!loadFile.is_open())
        {
            cout << "Unable to open saved game file!" << endl;
            getch();
            return false;
        }

        string savedDate;
        getline(loadFile, savedDate);
        string dateStr = savedDate.substr(10, 8);
        string timeStr = savedDate.substr(19, 4);
        string formattedDateTime = dateStr.substr(0, 4) + "-" +
                                   dateStr.substr(4, 2) + "-" +
                                   dateStr.substr(6, 2) + " " +
                                   timeStr.substr(0, 2) + ":" +
                                   timeStr.substr(2, 2);

        int savedBoardSize;
        loadFile >> savedBoardSize;

        if (savedBoardSize != boardSize)
        {
            cout << "Invalid board size in saved game!" << endl;
            loadFile.close();
            getch();
            return false;
        }

        loadFile >> movesCount;
        loadFile.ignore();

        for (int i = 0; i < boardSize; i++)
        {
            string line;
            getline(loadFile, line);
            for (int j = 0; j < boardSize; j++)
            {
                board[i][j] = line[j];
            }
        }

        loadFile >> currentPlayer;

        int actualMoves = 0;
        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                if (board[i][j] != ' ')
                    actualMoves++;
            }
        }
        if (movesCount != actualMoves)
            movesCount = actualMoves;

        loadFile.close();
        cout << "\n\t\tGame loaded successfully! (Saved on: " << formattedDateTime << ")" << endl;
        getch();
        return true;
    }

    void setUser(string user, string id = "")
    {
        loggedInUser = user;
        userID = id;
    }

    bool isGameOver() { return gameOver; }
    char getCurrentPlayer() { return currentPlayer; }
    int getBoardSize() { return boardSize; }
};

string generateRandomID()
{
    srand(time(0));
    return to_string(1000 + rand() % 9000);
}

bool userExists(string username, string id)
{
    ifstream file("users.txt");
    if (!file.is_open())
    {
        cout << "Error accessing user data!" << endl;
        return false;
    }

    string u, p, existingID;
    while (file >> u >> p >> existingID)
    {
        if (u == username && existingID == id)
        {
            return true;
        }
    }
    return false;
}

string getPasswordInput()
{
    string password;
    char ch;
    while (true)
    {
        ch = _getch(); // Get character without echoing
        if (ch == 13)
        { // Enter key
            cout << endl;
            break;
        }
        else if (ch == 8)
        { // Backspace
            if (!password.empty())
            {
                cout << "\b \b"; // Move back, print space, move back again
                password.pop_back();
            }
        }
        else
        {
            password.push_back(ch);
            cout << "*";
        }
    }
    return password;
}

// Function to validate Yes/No input
bool validateYesNo(char input)
{
    return (input == 'y' || input == 'Y' || input == 'n' || input == 'N');
}

bool isAlphabetic(string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if (!isalpha(str[i]))
        {
            return false;
        }
    }
    return true;
}

bool registerUser(string username, string password)
{
    if (!isAlphabetic(username))
    {
        cout << "\n\t\tUsername must contain only alphabets!" << endl;
        return false;
    }

    // Check if username already exists
    ifstream checkFile("users.txt");
    string u, p, existingID;
    while (checkFile >> u >> p >> existingID)
    {
        if (u == username)
        {
            // Username exists, generate a new unique ID
            string newID;
            do
            {
                newID = generateRandomID();
            } while (userExists(username, newID));

            // Open file in append mode to add the new user
            ofstream file("users.txt", ios::app);
            if (!file.is_open())
            {
                cout << "Error accessing user data!" << endl;
                return false;
            }

            file << username << " " << password << " " << newID << endl;
            file.close();

            cout << "\n\t\tUsername already exists. Registered with unique ID: " << newID << endl;
            return true;
        }
    }

    // If username doesn't exist, generate a unique ID
    string newID = generateRandomID();
    while (userExists(username, newID))
    {
        newID = generateRandomID();
    }

    // Add new user with unique ID
    ofstream file("users.txt", ios::app);
    if (!file.is_open())
    {
        cout << "Error accessing user data!" << endl;
        return false;
    }

    file << username << " " << password << " " << newID << endl;
    file.close();

    cout << "\n\t\tRegistered with unique ID: " << newID << endl;
    return true;
}

bool loginUser(string username, string password, string userInputID)
{
    if (!isAlphabetic(username))
    {
        cout << "\n\t\tUsername must contain only alphabets!" << endl;
        return false;
    }

    ifstream file("users.txt");
    if (!file.is_open())
    {
        cout << "Error accessing user data!" << endl;
        return false;
    }

    string u, p, storedID;
    while (file >> u >> p >> storedID)
    {
        if (u == username && p == password && storedID == userInputID)
        {
            return true;
        }
    }

    return false;
}

void showMainMenu(TicTacToe &game)
{
    int choice;
    do
    {
        system("cls");
        cout << "\n\t\t\t\t-----------------------------------------";
        cout << "\n\t\t\t\t         *    TIC TAC TOE    *";
        cout << "\n\t\t\t\t-----------------------------------------";
        cout << "\n\n\t\t1. Register\n\t\t2. Login\n\t\t3. Exit\n";
        cout << "\n\t\tEnter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            system("cls");
            string username, password;
            cout << "\n\t\t\t\t-----------------------------------------";
            cout << "\n\t\t\t\t         *    TIC TAC TOE    *";
            cout << "\n\t\t\t\t-----------------------------------------";
            cout << "\n\n\t\tEnter username: ";
            cin >> username;
            cout << "\n\t\tEnter password: ";
            password = getPasswordInput();
            if (registerUser(username, password))
            {
                cout << "\n\n\t\tRegistration successful! Please log in." << endl;
                getch();
            }
            else
            {
                cout << "\n\n\t\tRegistration failed!" << endl;
                getch();
            }
            break;
        }
        case 2:
        {
            system("cls");
            string username, password, userID;
            cout << "\n\t\t\t\t-----------------------------------------";
            cout << "\n\t\t\t\t         *    TIC TAC TOE    *";
            cout << "\n\t\t\t\t-----------------------------------------";
            cout << "\n\n\t\tEnter username: ";
            cin >> username;
            cout << "\n\t\tEnter password: ";
            password = getPasswordInput();
            cout << "\n\t\tEnter user ID: ";
            cin >> userID;

            if (loginUser(username, password, userID))
            {
                cout << "\n\n\t\tLogin successful!" << endl;
                getch();
                game.setUser(username, userID);
                int boardSize;
                do
                {
                    system("cls");
                    cout << "\n\t\t\t\t-----------------------------------------";
                    cout << "\n\t\t\t\t         *    TIC TAC TOE    *";
                    cout << "\n\t\t\t\t-----------------------------------------";
                    cout << "\n\n\t\tSelect Board Size:";
                    cout << "\n\n\t\t1. 3x3 Board";
                    cout << "\n\n\t\t2. 4x4 Board";
                    cout << "\n\n\t\t3. 5x5 Board";
                    cout << "\n\n\t\t4. Back to Main Menu";
                    cout << "\n\n\t\tEnter choice: ";
                    cin >> boardSize;

                    if (boardSize >= 1 && boardSize <= 3)
                    {
                        game.initializeGame(boardSize == 1 ? 3 : (boardSize == 2 ? 4 : 5));
                        break;
                    }
                    else if (boardSize == 4)
                    {
                        showMainMenu(game);
                        return;
                    }
                    else
                    {
                        cout << "\n\t\tInvalid choice! Press any key to try again." << endl;
                        getch();
                    }
                } while (true); // yaha tk ho chuka
                char gameChoice;
                while (true)
                {
                    system("cls");
                    cout << "\n\t\t\t\t-----------------------------------------";
                    cout << "\n\t\t\t\t         *    TIC TAC TOE    *";
                    cout << "\n\t\t\t\t-----------------------------------------";
                    cout << "\n\n\t\t1. New Game" << endl;
                    cout << "\n\n\t\t2. Load Game" << endl;
                    cout << "\n\n\t\t3. Back to Board Selection Page" << endl;
                    cout << "\n\n\t\t4. Back to Main Menu" << endl;
                    cout << "\n\n\t\tChoose option : ";
                    cin >> gameChoice;

                    if (gameChoice == '1')
                    {
                        break; // Proceed with new game
                    }
                    else if (gameChoice == '2')
                    {
                        system("cls");
                        if (!game.loadGame(username, userID))
                        {
                            cout << "\n\t\tNo saved game found. Starting new game..." << endl;
                            game.initializeGame(game.getBoardSize());
                        }
                        break;
                    }
                    else if (gameChoice == '3')
                    {
                        // Return to board size selection
                        do
                        {
                            system("cls");
                            cout << "\n\t\t\t\t-----------------------------------------";
                            cout << "\n\t\t\t\t         *    TIC TAC TOE    *";
                            cout << "\n\t\t\t\t-----------------------------------------";
                            cout << "\n\n\t\tSelect Board Size:";
                            cout << "\n\n\t\t1. 3x3 Board";
                            cout << "\n\n\t\t2. 4x4 Board";
                            cout << "\n\n\t\t3. 5x5 Board";
                            cout << "\n\n\t\t4. Back to Main Menu";
                            cout << "\n\n\t\tEnter choice: ";
                            int boardSize;
                            cin >> boardSize;

                            if (boardSize >= 1 && boardSize <= 3)
                            {
                                game.initializeGame(boardSize == 1 ? 3 : (boardSize == 2 ? 4 : 5));
                                break;
                            }
                            else if (boardSize == 4)
                            {
                                showMainMenu(game);
                                return;
                            }
                            else
                            {
                                cout << "\n\t\tInvalid choice! Press any key to try again." << endl;
                                getch();
                            }
                        } while (true);
                        // After selecting board size, go back to game choice menu
                        continue;
                    }
                    else if (gameChoice == '4')
                    {
                        showMainMenu(game);
                        return;
                    }
                    else
                    {
                        cout << "\n\t\tInvalid choice! Press any key to try again." << endl;
                        getch();
                    }
                } // yaha tk bhi done
                while (!game.isGameOver())
                {
                    system("cls");
                    game.displayBoard();
                    cout << "Player " << game.getCurrentPlayer() << "'s turn" << endl;
                    cout << "Enter row (1-" << game.getBoardSize() << ") and column (1-" << game.getBoardSize() << "), or 's' to save: ";

                    string input;
                    cin >> input;
                    if (input == "s" || input == "S")
                    {
                        game.saveGame();
                        char continueGame;
                        do
                        {
                            cout << "\nDo you want to continue the game (y/n)? ";
                            cin >> continueGame;
                            if (!validateYesNo(continueGame))
                            {
                                cout << "Invalid input! Please enter 'y' or 'n'." << endl;
                            }
                        } while (!validateYesNo(continueGame));

                        if (continueGame == 'n' || continueGame == 'N')
                        {
                            // Restart board size selection process
                            int boardSize;
                            do
                            {
                                system("cls");
                                cout << "\n\t\t\t\t-----------------------------------------";
                                cout << "\n\t\t\t\t         *    TIC TAC TOE    *";
                                cout << "\n\t\t\t\t-----------------------------------------";
                                cout << "\n\n\t\tSelect Board Size:";
                                cout << "\n\n\t\t1. 3x3 Board";
                                cout << "\n\n\t\t2. 4x4 Board";
                                cout << "\n\n\t\t3. 5x5 Board";
                                cout << "\n\n\t\t4. Back to Main Menu";
                                cout << "\n\n\t\tEnter choice: ";
                                cin >> boardSize;
                                if (boardSize >= 1 && boardSize <= 3)
                                {
                                    game.initializeGame(boardSize == 1 ? 3 : (boardSize == 2 ? 4 : 5));
                                    break;
                                }
                                else if (boardSize == 4)
                                {
                                    showMainMenu(game);
                                    return;
                                }
                                else
                                {
                                    cout << "\n\t\tInvalid choice! Press any key to try again." << endl;
                                    getch();
                                }
                            } while (true);
                            char gameChoice;
                            while (true)
                            {
                                system("cls");
                                cout << "\n\t\t\t\t-----------------------------------------";
                                cout << "\n\t\t\t\t         *    TIC TAC TOE    *";
                                cout << "\n\t\t\t\t-----------------------------------------";
                                cout << "\n\n\t\t1. New Game" << endl;
                                cout << "\n\n\t\t2. Load Game" << endl;
                                cout << "\n\n\t\t3. Back to Board Selection Page" << endl;
                                cout << "\n\n\t\t4. Back to main menu" << endl;
                                cout << "\n\n\t\t Choose option : ";
                                cin >> gameChoice;

                                if (gameChoice == '1')
                                {
                                    break;
                                }
                                else if (gameChoice == '2')
                                {
                                    system("cls");
                                    if (!game.loadGame(game.loggedInUser, game.userID))
                                    {
                                        cout << "Starting new game instead..." << endl;
                                        game.initializeGame(game.getBoardSize());
                                    }
                                    break;
                                }
                                else if (gameChoice == '3')
                                {
                                    // Return to board size selection
                                    do
                                    {
                                        system("cls");
                                        cout << "\n\t\t\t\t-----------------------------------------";
                                        cout << "\n\t\t\t\t         *    TIC TAC TOE    *";
                                        cout << "\n\t\t\t\t-----------------------------------------";
                                        cout << "\n\n\t\tSelect Board Size:";
                                        cout << "\n\n\t\t1. 3x3 Board";
                                        cout << "\n\n\t\t2. 4x4 Board";
                                        cout << "\n\n\t\t3. 5x5 Board";
                                        cout << "\n\n\t\t4. Back to Main Menu";
                                        cout << "\n\n\t\tEnter choice: ";
                                        int boardSize;
                                        cin >> boardSize;

                                        if (boardSize >= 1 && boardSize <= 3)
                                        {
                                            game.initializeGame(boardSize == 1 ? 3 : (boardSize == 2 ? 4 : 5));
                                            break;
                                        }
                                        else if (boardSize == 4)
                                        {
                                            showMainMenu(game);
                                            return;
                                        }
                                        else
                                        {
                                            cout << "\n\t\tInvalid choice! Press any key to try again." << endl;
                                            getch();
                                        }
                                    } while (true);
                                    // After selecting board size, go back to game choice menu
                                    continue;
                                }
                                else if (gameChoice == '4')
                                {
                                    showMainMenu(game);
                                    return;
                                }
                                else
                                {
                                    cout << "\n\t\tInvalid choice! Press any key to try again." << endl;
                                    getch();
                                }
                            }
                        }
                    }
                    else
                    {
                        if (input.length() < 2)
                        {
                            cout << "\n\n\t\tInvalid move! Try again." << endl;
                            getch();
                            continue;
                        }
                        int row = input[0] - '0';
                        int col = input[1] - '0';
                        if (!game.makeMove(row, col))
                        {
                            cout << "\n\n\t\tInvalid move! Try again." << endl;
                            getch();
                        }
                    }
                }
                break;
            }
            else
            {
                cout << "\n\n\t\tInvalid credentials!" << endl;
                getch();
                break;
            }
        }
        case 3:
            cout << "\n\n\t\tExiting....!" << endl;
            exit(0);
        default:
            cout << "\nInvalid choice!" << endl;
            getch();
        }
    } while (true);
}

int main()
{
    TicTacToe game;
    showMainMenu(game);
    return 0;
}