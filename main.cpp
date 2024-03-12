#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <SDL2/SDL.h>
#include <stdio.h>
using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ESC 27

int capacity = 10;
int savedGameData = 0;
int len = 0;
int score = 0;
int boardSize = 0;
int **Board;
enum colors
{
    BLACK, BLUE, GREEN, CYAN, RED, PURPLE, YELLOW, GREY,
    LIGHTGREY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED,
    LIGHTPURPLE, LIGHTYELLOW, WHITE
};
struct GameData{
    string name;
    int score;
    int boardSize;
};
bool moved = true;
bool savedNewData = false;
bool firstPlay = true;
GameData *gameDataArray = new GameData[capacity];

void displayMenu();
void displayHowToPlay();
void displayPauseMenu();
void displayGame();
void menuStart();
void displayLeaderBoard();
void generateTable();
void movement();
void mergeCell(char sw);
void setCellColor(int i , int j);
void setNumberColor(int num);
void setConsoleColor(int textColor);
int printNumber(int i , int j);
void printScore(int num);
void calculateScore(int n);
bool checkWin();
bool checkLose();
void generateRandomNumber();
void initializeTable();
void SaveGameDataToFile(const GameData &data, const string& fileAddress);
void LoadGameDataFromFile(const string& fileAddress);
void makeTableN();
void resetGame();
void winLoseMessage(string str);
void ShowConsoleCursor(bool showFlag);
void append(const GameData &value);
int compare(GameData one , GameData two);
void resize();
void Exit();
void playWinSound();
void playLoseSound();
//-----------------------------------------------
int main(int argc, char** argv)
{
    displayHowToPlay();
    displayMenu();
    return 0;
}
//-----------------------------------------------
void menuStart()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    ShowConsoleCursor(false);
    while(!checkLose() && !checkWin()){
        if (moved) {
            displayGame();
            generateTable();
            moved = false;
        }
        movement();
    }
    if (checkLose()){
        displayGame();
        generateTable();
        winLoseMessage("LOSE");
    }
    if (checkWin()){
        displayGame();
        generateTable();
        winLoseMessage("WIN");
    }
}
//-----------------------------------------------
void displayHowToPlay()
{
    system("cls");
    setConsoleColor(LIGHTYELLOW);
    cout << "~~~~~~~~~~~~~ Welcome to 2048 game! ~~~~~~~~~~~~~\n";
    setConsoleColor(CYAN );
    cout << "How to play: ";
    setConsoleColor(LIGHTPURPLE);
    cout << "Use" << " arrow up " << "to move cells upward , " << "arrow down" << " downward\n" << "arrow left" << " to left and " << "arrow right" << " to right.\n";
    setConsoleColor(CYAN);
    cout << "How to win: ";
    setConsoleColor(LIGHTPURPLE);
    cout << "You have to merge cells till one of them reaches 2048 value\non tables with a size greater than 3, or 1024 on 3x3.\n";
    setConsoleColor(GREEN);
    system("pause");
}
//-----------------------------------------------
void displayMenu()
{
    system("cls");
    //messages
    {
        setConsoleColor(RED);
        cout << "~~~~~~~~~~~~~ ";
        setConsoleColor(LIGHTYELLOW);
        cout << "Main menu";
        setConsoleColor(RED);
        cout << " ~~~~~~~~~~~~~\n";
        setConsoleColor(RED);
        cout << "[1] ";
        setConsoleColor(LIGHTYELLOW);
        cout << "New game";
        setConsoleColor(RED);
        cout << "\n[2] ";
        setConsoleColor(LIGHTYELLOW);
        cout << "Leader Board";
        setConsoleColor(RED);
        cout << "\n[3] ";
        setConsoleColor(LIGHTYELLOW);
        cout << "Exit\n";
    }
    switch (getch()) {
        case 49:
            makeTableN();
            menuStart();
            break;
        case 50:
            displayLeaderBoard();
            break;
        case 51:
            resetGame();
            Exit();
            break;
        default:
            setConsoleColor(CYAN);
            cout << "Invalid input!";
            Sleep(1000);
            displayMenu();
            break;
    }

}
//-----------------------------------------------
void displayGame()
{
    system("cls");
    string _2048 = "____/\\\\\\\\\\\\\\\\\\_________/\\\\\\\\\\\\\\_______________/\\\\\\________/\\\\\\\\\\\\\\\\\\____        \n"
                   " __/\\\\\\///////\\\\\\_____/\\\\\\/////\\\\\\___________/\\\\\\\\\\______/\\\\\\///////\\\\\\__       \n"
                   "  _\\///______\\//\\\\\\___/\\\\\\____\\//\\\\\\________/\\\\\\/\\\\\\_____\\/\\\\\\_____\\/\\\\\\__      \n"
                   "   ___________/\\\\\\/___\\/\\\\\\_____\\/\\\\\\______/\\\\\\/\\/\\\\\\_____\\///\\\\\\\\\\\\\\\\\\/___     \n"
                   "    ________/\\\\\\//_____\\/\\\\\\_____\\/\\\\\\____/\\\\\\/__\\/\\\\\\______/\\\\\\///////\\\\\\__    \n"
                   "     _____/\\\\\\//________\\/\\\\\\_____\\/\\\\\\__/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\__/\\\\\\______\\//\\\\\\_   \n"
                   "      ___/\\\\\\/___________\\//\\\\\\____/\\\\\\__\\///////////\\\\\\//__\\//\\\\\\______/\\\\\\__  \n"
                   "       __/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\__\\///\\\\\\\\\\\\\\/_____________\\/\\\\\\_____\\///\\\\\\\\\\\\\\\\\\/___ \n"
                   "        _\\///////////////_____\\///////_______________\\///________\\/////////_____\n";
    for (int i = 0 ; i < _2048.length() ; i++){
        if (_2048[i] == '_')
            setConsoleColor(WHITE);
        else if (_2048[i] == '/')
            setConsoleColor(BLUE);
        else if (_2048[i] == '\\') {
            if (_2048[i - 1] == '_')
                setConsoleColor(BLUE);
            else
                setConsoleColor(LIGHTCYAN);
        }
        cout << _2048[i];
    }
    setConsoleColor(LIGHTPURPLE);
    cout << "\n\u250C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2510\n";
    cout << "\u2502";
    setConsoleColor(LIGHTYELLOW);
    cout << "Score:                      ";
    printScore(score);
    setConsoleColor(LIGHTPURPLE);
    cout << "\u2502" << "\n\u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518\n\n";
    setConsoleColor(WHITE);

}
//-----------------------------------------------
void displayPauseMenu()
{
    system("cls");
    setConsoleColor(RED);
    cout << "~~~~~~~~~~~~~ ";
    setConsoleColor(LIGHTYELLOW);
    cout << "Pause menu";
    setConsoleColor(RED);
    cout << " ~~~~~~~~~~~~~\n";
    setConsoleColor(RED);
    cout << "[1] ";
    setConsoleColor(LIGHTYELLOW);
    cout << "Continue";
    setConsoleColor(RED);
    cout << "\n[2] ";
    setConsoleColor(LIGHTYELLOW);
    cout << "Exit";
    switch (getch()) {
        case 49:
            menuStart();
        case 50:
            displayMenu();
            break;
        default:
            setConsoleColor(CYAN);
            cout << "\nInvalid input!";
            Sleep(1000);
            displayPauseMenu();
            break;
    }

}
//-----------------------------------------------
void displayLeaderBoard()
{
    int i;
    if (savedNewData)
        i = 1;
    else
        i = 0;
    LoadGameDataFromFile("leader_board.txt");
    sort(gameDataArray, gameDataArray + savedGameData, compare);
    Sleep(1000);
    system("cls");
    setConsoleColor(LIGHTCYAN);
    cout << "Rank" << "\t";
    cout << "Name" << "\t\t";
    cout << "Score" << "\t\t";
    cout << "Size\n";

    for (; i < savedGameData ; i++){
        setConsoleColor(RED);
        if (savedNewData)
            cout << i << "\t";
        else
            cout << i + 1 << "\t";
        setConsoleColor(LIGHTPURPLE);
        cout << gameDataArray[i].name << "\t\t";
        setConsoleColor(LIGHTGREEN);
        cout << gameDataArray[i].score << "\t\t";
        setConsoleColor(LIGHTYELLOW);
        cout << gameDataArray[i].boardSize << "\n";
    }
    savedNewData = false;
    savedGameData = 0;
    len = 0;
    system("pause");
    displayMenu();
}
//-----------------------------------------------
void generateTable()
{
    int size;
    setConsoleColor(WHITE);
    for (int i = 0 ; i < (3 * boardSize) ; i++){
        for (int j = 0; j < (6 * boardSize) + 1 ; j++){
            setCellColor(i , j);
            if (i % 3 == 0)
            {
                if (j == 0) {
                    cout << "\u256D";
                    continue;
                }
                if (j == (6 * boardSize)) {
                    setCellColor(i , j - 1);
                    cout << "\u256E";
                    continue;
                }
                if (j % 6 == 0) {
                    setCellColor(i , j - 1);
                    cout << "\u256E";
                    setCellColor(i , j + 1);
                    cout << "\u256D";
                    continue;
                }
                cout << "\u2500";
                continue;
            }
            if (i % 3 == 2){
                if (j == 0) {
                    cout << "\u2570";
                    continue;
                }
                if (j == (6 * boardSize)) {
                    setCellColor(i , j - 1);
                    cout << "\u256F";
                    continue;
                }
                if (j % 6 == 0) {
                    setCellColor(i , j - 1);
                    cout << "\u256F";
                    setCellColor(i , j + 1);
                    cout << "\u2570";
                    continue;
                }
                cout << "\u2500";
                continue;
            }
            if (j % (6 * boardSize) == 0) {
                setCellColor(i , j - 1);
                cout << "\u2502";
                continue;
            }
            if (j % 6 == 0) {
                setCellColor(i , j - 1);
                cout << "\u2502";
                setCellColor(i , j + 1);
                cout << "\u2502";
                continue;
            }
            if (j % 3 == 0){
                size = printNumber(i , j);
                continue;
            }
            if (size == 3 || size == 4) {
                size = 0;
                continue;
            }
            cout << " ";
        }
        cout << '\n';
    }
}
//-----------------------------------------------
void movement()
{
    switch(getch()) {
        case KEY_UP: {
            // code for arrow up
            for (int j = 0; j < boardSize; j++) {
                for (int i = 1; i < boardSize; i++) {
                    if (Board[i][j] != 0) {
                        int temp = i;
                        while (temp > 0) {
                            if (Board[temp - 1][j] == 0) {
//                                
                                swap(Board[temp][j], Board[temp - 1][j]);
                                moved = true;
                            }
                            temp--;
                        }
                    }
                }
            }
            mergeCell('U');
            if (moved) {
                generateRandomNumber();
//                moved = false;
            }
            break;
        }
        case KEY_DOWN: {
            // code for arrow down
            for (int j = 0; j < boardSize; j++) {
                for (int i = boardSize - 2; i >= 0; i--) {
                    if (Board[i][j] != 0) {
                        int temp = i;
                        while (temp < boardSize - 1) {
                            if (Board[temp + 1][j] == 0) {
//                                
                                swap(Board[temp][j], Board[temp + 1][j]);
                                moved = true;
                            }
                            temp++;
                        }
                    }
                }
            }
            mergeCell('D');
            if (moved) {
                generateRandomNumber();
//                moved = false;
            }
            break;
        }
        case KEY_RIGHT: {
            // code for arrow right
            for (int i = 0; i < boardSize; i++) {
                for (int j = boardSize - 2; j >= 0; j--) {
                    if (Board[i][j] != 0) {
                        int temp = j;
                        while (temp < boardSize - 1) {
                            if (Board[i][temp + 1] == 0) {
//                                
                                swap(Board[i][temp], Board[i][temp + 1]);
                                moved = true;
                            }
                            temp++;
                        }
                    }
                }
            }
            mergeCell('R');
            if (moved) {
                generateRandomNumber();
//                moved = false;
            }
            break;
        }
        case KEY_LEFT: {
            // code for arrow left
            for (int i = 0; i < boardSize; i++) {
                for (int j = 1; j < boardSize; j++) {
                    if (Board[i][j] != 0) {
                        int temp = j;
                        while (temp > 0) {
                            if (Board[i][temp - 1] == 0) {
//                                
                                swap(Board[i][temp], Board[i][temp - 1]);
                                moved = true;
                            }
                            temp--;
                        }
                    }
                }
            }
            mergeCell('L');
            if (moved) {
                generateRandomNumber();
//                moved = false;
            }
            break;
        }
        case ESC:
            displayPauseMenu();
            break;
    }
}
//-----------------------------------------------
void mergeCell(char sw)
{
    switch(sw){
        case 'U': {
            for (int j = 0; j < boardSize; j++) {
                for (int i = 1; i < boardSize; i++) {
                    if (Board[i - 1][j] == Board[i][j]) {
                        Board[i - 1][j] *= 2;
                        calculateScore(Board[i - 1][j]);
                        Board[i][j] = 0;
                        moved = true;
                        int temp = i;
                        while (temp < boardSize - 1) {
                            swap(Board[temp][j], Board[temp + 1][j]);
                            temp++;
                        }
                    }
                }
            }
            break;
        }
        case 'D': {
            for (int j = 0; j < boardSize; j++) {
                for (int i = boardSize - 2; i >= 0; i--) {
                    if (Board[i + 1][j] == Board[i][j]) {
                        Board[i + 1][j] *= 2;
                        calculateScore(Board[i + 1][j]);
                        Board[i][j] = 0;
                        int temp = i;
                        moved = true;
                        while (temp > 0) {
                            swap(Board[temp][j], Board[temp - 1][j]);
                            temp--;
                        }
                    }
                }
            }
            break;
        }
        case 'R': {
            for (int i = 0; i < boardSize; i++) {
                for (int j = boardSize - 2; j >= 0; j--) {
                    if (Board[i][j] == Board[i][j + 1]) {
                        Board[i][j + 1] *= 2;
                        calculateScore(Board[i][j + 1]);
                        Board[i][j] = 0;
                        int temp = j;
                        moved = true;
                        while (temp > 0) {
                            swap(Board[i][temp], Board[i][temp - 1]);
                            temp--;
                        }
                    }
                }
            }
            break;
        }
        case 'L': {
            for (int i = 0; i < boardSize; i++) {
                for (int j = 1; j < boardSize; j++) {
                    if (Board[i][j] == Board[i][j - 1]) {
                        Board[i][j - 1] *= 2;
                        calculateScore(Board[i][j - 1]);
                        Board[i][j] = 0;
                        int temp = j;
                        moved = true;
                        while (temp < boardSize - 1) {
                            swap(Board[i][temp], Board[i][temp + 1]);
                            temp++;
                        }
                    }
                }
            }
            break;
        }
    }
}
//-----------------------------------------------
void Exit()
{
    system("cls");
    {
        setConsoleColor(CYAN);
        cout << "Do you really want to exit?\n";
        setConsoleColor(RED);
        cout << "[1] ";
        setConsoleColor(GREEN);
        cout << "Yes\n";
        setConsoleColor(RED);
        cout << "[2] ";
        setConsoleColor(GREEN);
        cout << "No\n";
    }
    switch (getch())
    {
        case 49:
            delete[] gameDataArray;
            system("cls");
            setConsoleColor(WHITE);
            exit(1);
        case 50:
            displayMenu();
            break;

        default:
            setConsoleColor(RED);
            cout << "Invalid input!";
            Sleep(1000);
            Exit();
            break;
    }
}
//-----------------------------------------------
void setConsoleColor(int textColor)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (textColor));
}
//-----------------------------------------------
void setCellColor(int i , int j)
{
    int num = Board[i/3][j/6];
    setNumberColor(num);
}
//-----------------------------------------------
void setNumberColor(int num)
{
    if (num == 0){
        setConsoleColor(WHITE);
    }
    else if (num == 2){
        setConsoleColor(LIGHTCYAN);
    }
    else if (num == 4) {
        setConsoleColor(CYAN);
    }
    else if (num == 8){
        setConsoleColor(BLUE);
    }
    else if (num == 16){
        setConsoleColor(LIGHTYELLOW);
    }
    else if (num == 32){
        setConsoleColor(YELLOW);
    }
    else if (num == 64){
        setConsoleColor(LIGHTRED);
    }
    else if (num == 128){
        setConsoleColor(RED);
    }
    else if (num == 256){
        setConsoleColor(LIGHTPURPLE);
    }
    else if (num == 512){
        setConsoleColor(PURPLE);
    }
    else if (num == 1024){
        setConsoleColor(LIGHTGREEN);
    }
    else {
        setConsoleColor(GREEN);
    }
}
//-----------------------------------------------
int printNumber(int i , int j)
{
    int num = Board[(i-1)/3][(j-3)/6];
    if (num == 0){
        cout << " ";
        return 0;
    }
    int size = 0;
    int temp = num;
    while (temp > 0){
        temp /= 10;
        size++;
    }
    if (size == 1){
        cout << num;
    }
    else if (size < 4){
        cout << "\b" << num;
    }
    else{
        cout << "\b\b" << num;
    }
    return size;
}
//-----------------------------------------------
bool checkWin()
{
    for (int i = 0 ; i < boardSize ; i++){
        for (int j = 0 ; j < boardSize ; j++){
            if (Board[i][j] == 2048 && boardSize > 3){
                return true;
            }
            if (Board[i][j] == 1024 && boardSize == 3){
                return true;
            }
        }
    }
    return false;
}
//-----------------------------------------------
bool checkLose()
{
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (Board[i][j] == 0) {
                return false;
            }
        }
    }
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 1; j < boardSize; ++j) {
            if (Board[i][j] == Board[i][j - 1]) {
                return false;
            }
        }
    }
    for (int j = 0; j < boardSize; ++j) {
        for (int i = 1; i < boardSize; ++i) {
            if (Board[i][j] == Board[i - 1][j]) {
                return false;
            }
        }
    }
    return true;
}
//-----------------------------------------------
void calculateScore(int n)
{
    score += n;
}
//-----------------------------------------------
void generateRandomNumber()
{
    bool generated = false;
    while (!generated) {
        int randomRow = rand() % (boardSize) + 1;
        int randomColum = rand() % (boardSize) + 1;
        if (Board[randomRow - 1][randomColum - 1] == 0) {
            int randomSelectNumber = rand() % 4 + 1;
            if (randomSelectNumber <= 3) {
                Board[randomRow - 1][randomColum - 1] = 2;
                generated = true;
            } else {
                Board[randomRow - 1][randomColum - 1] = 4;
                generated = true;
            }
        }
    }
}
//-----------------------------------------------
void initializeTable()
{
    int lastRow , lastColum , counter = 0;
    while (counter < 2) {
        int randomRow = rand() % (boardSize) + 1;
        int randomColum = rand() % (boardSize) + 1;
        if (lastRow != randomRow - 1 && lastColum != randomColum - 1) {
            lastRow = randomRow - 1;
            lastColum = randomColum - 1;
            int randomSelectNumber = rand() % 4 + 1;
            if (randomSelectNumber <= 3) {
                Board[randomRow - 1][randomColum - 1] = 2;
                counter++;
            } else {
                Board[randomRow - 1][randomColum - 1] = 4;
                counter++;
            }
        }
    }
}
//-----------------------------------------------
void SaveGameDataToFile(const GameData &data, const string& filename)
{

    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << data.name << " " << data.score << " " << data.boardSize;
        file.close();
        setConsoleColor(LIGHTGREEN);
        cout << "\nPlayer data has been saved!";
        savedNewData = true;
        savedGameData++;
    } else {
        cout << "\nUnable to save the player data!";
    }
}
//-----------------------------------------------
void LoadGameDataFromFile(const string& fileAddress)
{
    system("cls");
    ifstream file(fileAddress);
    GameData data;
    if (file.is_open()) {
        while (!file.eof()) {
            file >> data.name >> data.score >> data.boardSize;
            savedGameData++;
            append(data);
        }
        file.close();
        setConsoleColor(LIGHTGREEN);
        cout << "Leaderboard data have been loaded from the file.\n";
    } else {
        setConsoleColor(RED);
        cout << "Unable to open the file for loading leaderboard data. \nPlease play games and then come and see leader board.";
        Sleep(1000);
        displayMenu();
    }

}
//-----------------------------------------------
void printScore(int num)
{
    int size = 0;
    int temp = num;
    while (temp > 0){
        temp /= 10;
        size++;
    }
    for (int i = size ; i > 1 ; i--){
        cout << "\b";
    }
    cout << num;
}
//-----------------------------------------------
void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
//-----------------------------------------------
void makeTableN()
{
    if (!firstPlay){
        resetGame();
    }
    system("cls");
    setConsoleColor(RED);
    cout << "Enter size of the table you want to play(Note original games table is 4x4): ";
    setConsoleColor(LIGHTYELLOW);
    cin >> boardSize;
    if (boardSize > 2) {
        firstPlay = false;
        Board = new int *[boardSize];
        for (int i = 0; i < boardSize; i++) {
            Board[i] = new int[boardSize];
        }
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                Board[i][j] = 0;
            }
        }
    }
    else{
        setConsoleColor(RED);
        cout << "\nPlease enter a size greater than two!";
        Sleep(1000);
        makeTableN();
    }
    initializeTable();
}
//-----------------------------------------------
void resetGame()
{
    for (int i = 0 ; i < boardSize ; i++)
    {
        delete[] Board[i];
    }
    delete[] Board;
    score = 0;
    boardSize = 0;
}
//-----------------------------------------------
void winLoseMessage(string str)
{
    ShowConsoleCursor(true);
    string name;
    GameData player;
    if (str == "WIN"){
        setConsoleColor(LIGHTGREEN);
        cout << "\nVictory!";
        playWinSound();
        setConsoleColor(CYAN);
        cout << "\nEnter your nickname to save your score(maximum of 6 characters): ";
        setConsoleColor(LIGHTPURPLE);
        cin >> name;
        if (name.size() > 6){
            setConsoleColor(RED);
            cout << "More characters that maximum limit are entered! \nPlease enter a maximum of 6 character";
            winLoseMessage("WIN");
        }
    }
    else {
        setConsoleColor(LIGHTGREEN);
        cout << "\nGame over!";
        playLoseSound();
        setConsoleColor(CYAN);
        cout << "\nEnter your nickname to save your score(maximum of 6 characters): ";
        setConsoleColor(LIGHTPURPLE);
        cin >> name;
        if (name.size() > 6){
            setConsoleColor(RED);
            cout << "More characters that maximum limit are entered! \nPlease enter a maximum of 6 character";
            winLoseMessage("LOSE");
        }
    }
    player.name = name;
    player.score = score;
    player.boardSize = boardSize;
    SaveGameDataToFile(player, "leader_board.txt");
    Sleep(1000);
    displayMenu();
}
//-----------------------------------------------
void resize()
{
    capacity = capacity * 2;
    GameData *gameDataArray2 = new GameData [capacity];
    for (int i = 0 ; i < len ; i++)
        gameDataArray2[i] = gameDataArray[i];
    delete[] gameDataArray;
    gameDataArray = new GameData[capacity];
    for (int i = 0 ; i < len ; i++)
        gameDataArray[i] = gameDataArray2[i];
    delete[] gameDataArray2;
}
//-----------------------------------------------
void append(const GameData &value)
{
    if (capacity == len)
        resize();
    gameDataArray[len] = value;
    len++;
}
//-----------------------------------------------
int compare(GameData one , GameData two) {
    if (one.score > two.score) {
        return 1;
    }
    return 0;
}
//-----------------------------------------------
void playWinSound()
{
    SDL_Init(SDL_INIT_AUDIO);
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;

    SDL_LoadWAV("Win.wav", &wavSpec, &wavBuffer, &wavLength);
    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(nullptr, 0, &wavSpec, nullptr, 0);

    int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);
    SDL_Delay(9000);
    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
    SDL_Quit();
}
//-----------------------------------------------
void playLoseSound()
{
    SDL_Init(SDL_INIT_AUDIO);
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;

    SDL_LoadWAV("Lose.wav", &wavSpec, &wavBuffer, &wavLength);
    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(nullptr, 0, &wavSpec, nullptr, 0);

    int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);
    SDL_Delay(3000);
    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
    SDL_Quit();
}
//-----------------------------------------------
