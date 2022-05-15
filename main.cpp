#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <ctime>

#define VK_A 0x41
#define VK_S 0x53
#define VK_W 0x57
#define VK_D 0x44
using namespace std;

class land{
public:
    bool trap{};
    bool disposal{};
    bool flagged{};
    bool dogtag{};
    bool wall{};
    string sign;
    land* prev{};
};
int score = 0;
bool win = false;
int i, j;
const int height = 28;
const int width = 28;
HANDLE h = GetStdHandle( STD_OUTPUT_HANDLE );
land map_array[height][width];
void SetCursorPosition(int x, int y);
void ClearScreen(const string& ch);
void SetCursorPosition(int x, int y){
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(output, pos);
}
class player{
public:
    int width = 0;
    int height = 0;
    int level = 0;
}player;
//OOP
void define_character(){
    map_array[player.height][player.width].sign="<*>";
}
int CHASER_NUM = 0;
int bomb_num = 0;
void define_map_array_reset() {
        for(j=0;j<height;j++) {
            for (i = 0; i < width; i++) {
                if(map_array[j][i].trap){
                    win = false;
                }
                map_array[j][i].sign="/~/";
                if(map_array[j][i].flagged){
                    map_array[j][i].sign="/*/";
                }if(map_array[j][i].dogtag){
                    map_array[j][i].sign="/$/";
                }if(map_array[j][i].wall){
                    map_array[j][i].sign="/#/";
                }if(map_array[j][i].disposal){
                    map_array[j][i].sign="/X/";
                }
                if(map_array[j][i].trap && (map_array[j+1][i].flagged&&map_array[j][i+1].flagged)){
                    if(map_array[j-1][i].flagged&&map_array[j][i-1].flagged){
                        map_array[j][i].sign="/X/";
                        map_array[j][i].trap = false;
                        bomb_num--;
                        map_array[j][i].disposal = true;
                        player.level ++;
                    }
                }if(map_array[j][i].trap&&j==0){
                    {
                        map_array[j][i].sign="/X/";
                        map_array[j][i].trap = false;
                        bomb_num--;
                        map_array[j][i].disposal = true;
                        player.level ++;
                    }

                }
            }
            win = true;
        }
    define_character();
}
void draw_map_array() {
    cout<<&(map_array[player.height][player.width])<<endl;
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            SetConsoleTextAttribute(h,FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            cout << map_array[j][i].sign;
        }
        printf("\n");
    }
    cout<<endl;
    cout<<"score: "<< score << " || "<<"level: "<<player.level;
    cout<<endl<<bomb_num<<" bombs remain"<<endl;
}
void define_map_array_flag(){
    int y = player.height;
    int x = player.width;
    map_array[y][x].flagged = true;
    draw_map_array();
}
void setDogtag(int indicator){
    srand (time(NULL));
    for(int i = 1; i< indicator; i++){
        int num1 = rand() % 28 + 1;
        int num2 = rand() % 28 + 1;
        map_array[num1][num2].dogtag = true;
    }
}
void setWall(int indicator){
    for(int i = 1; i< indicator; i++){
        int num1 = rand() % 28 + 1;
        int num2 = rand() % 28 + 1;
        map_array[num1][num2].wall = true;
    }
}

void setBomb(int indicator){
    vector<land> trapped_blocks(1);
    trapped_blocks[0] = map_array[0][0];
    for(int i = 1; i < indicator; i++) {
        int num1 = rand() % 28 + 1;
        int num2 = rand() % 28 + 1;
        map_array[num1][num2].trap = true;
        trapped_blocks.push_back(map_array[num1][num2]);
        map_array[num1][num2].prev = &(trapped_blocks[i-1]);
        //set up a reverse linked list here
        cout << map_array[num1][num2].prev <<endl;
        cout << num1 << " " << num2 << " ";
        bomb_num++;
    }
}
void dead_scene(){
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            if(map_array[j][i].trap){
                map_array[j][i].sign="!@!";
            }
            SetConsoleTextAttribute(h,FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            cout << map_array[j][i].sign;
        }
        printf("\n");
    }
}
void detectUserinput(){
    char input;
    bool quit = false;
    while(!quit) {
        input = _getch();
        if (GetAsyncKeyState(VK_A)) {
            player.width --;
            system("CLS");
            define_map_array_reset();
            draw_map_array();
        }
        if (GetAsyncKeyState(VK_W)) {
            player.height --;
            system("CLS");
            define_map_array_reset();
            draw_map_array();
        }
        if (GetAsyncKeyState(VK_D)) {
            player.width ++;
            system("CLS");
            define_map_array_reset();
            draw_map_array();
        }
        if (GetAsyncKeyState(VK_S)) {
            player.height ++;
            system("CLS");
            define_map_array_reset();
            draw_map_array();
        }
        if(input == 'x'){
            break;
        }
        if(input=='c'){
            cout<<"your position is ("<<player.height<<" "<<player.width<<endl;
        }
        if(input=='g'){
            int og = 30;
            for(int i=0;i<height;i++){
                for(int j=0;j<width;j++){
                    if (map_array[i][j].trap)
                    {
                        SetCursorPosition(30,og);
                        cout<<"g: "<<i<<", "<<j<<" ";
                        og++;
                    }
                }
            }
        }
        if(input == 'q'){
            for(int i = 0; i < width/4; i ++){
                map_array[player.height][player.width+i].sign="???";
                map_array[player.height][player.width-i].sign="???";
                map_array[player.height+i][player.width].sign="???";
                for(int i = 0; i < width; i ++){
                    if(map_array[player.height][i].trap){
                        map_array[player.height][i].sign="!!!";
                    }if(map_array[i][player.width].trap){
                        map_array[i][player.width].sign="!!!";
                    }
                }
            }
            draw_map_array();
        }
        if(input == 'e'){
            define_map_array_flag();
            draw_map_array();
        }
        if(input == 'v'&&map_array[player.height][player.width].dogtag){
            score = score + 1;
            player.level ++;
            draw_map_array();
            map_array[player.height][player.width].dogtag = false;
        }
        if (map_array[player.height][player.width].trap){
            system("CLS");
            dead_scene();
            cout<<"Game Over! \n press any key to exit"<<endl;
            quit = true;
            auto end = getch();
        }
        if(player.height==height&&player.width>=width) {
            quit = true;
            system("CLS");
            cout << "Game Over! \npress any key to exit" << endl;
        }
    }
}
// Driver Code
int main() {
    system("cls");
    cout<<"press X to load map"<<endl;
    detectUserinput();
    setDogtag(20);
    setWall(40);
    setBomb(30);
    define_map_array_reset();
    draw_map_array();
    detectUserinput();
    if (win){
        system("CLR");
        cout<<"hi, you won";
    }
    return 0;
}
//1. once play disposal all bombs, game over