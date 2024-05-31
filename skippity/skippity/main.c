#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <signal.h>
//VİDEO LİNKİ BURDA**************************
//https://youtu.be/M6qZLJPkh0M
//VİDEO LİNKİ BURDA**************************
typedef struct Player;
typedef struct{
    char name;
    char player_undo;
    int game_id;
    int all_point;
    int A_point;
    int B_point;
    int C_point;
    int D_point;
    int E_point;
}Player;
void printBoard(int choice,int size,int saved_index,WINDOW *win,WINDOW *win_player1,WINDOW *win_player2,WINDOW *info_win,char** matrix);
void print_saved();
void print_menu();
void print_new_game_menu();
void print_pause_win(WINDOW *pause_win,int game_started,int size,int saved_index,WINDOW *win,WINDOW *win_player_1,WINDOW *win_player_2,WINDOW *info_win,char** matrix,Player *player1,Player *player2,int move);
void print_game_set(int choice);

char** createMatrix(int size);
void print_delete_menu();
void game_engine(int choice,const char* dimension[],int saved_index);
char** createMatrix(int size) {
    // Bellek ayırma
    char** matrix = (char**)malloc(size * sizeof(char*));
    for (int i = 0; i < size; i++) {
        matrix[i] = (char*)malloc(size * sizeof(char));
    }

    // Harf dağılımını dengeli yapmak için sayıları belirle
    int letterCount[5] = {size * size / 5, size * size / 5, size * size / 5, size * size / 5, size * size / 5};
    int remaining = size * size - (letterCount[0] + letterCount[1] + letterCount[2] + letterCount[3] + letterCount[4]);
    for (int i = 0; i < remaining; i++) {
        letterCount[i % 5]++;
    }

    // Ortadaki 2x2 kareyi "0" yapma
    int mid = size / 2;
    matrix[mid - 1][mid - 1] = ' ';
    matrix[mid - 1][mid] = ' ';
    matrix[mid][mid - 1] = ' ';
    matrix[mid][mid] = ' ';
    letterCount[0] -= 4;

    // Rastgele harfleri matrise atama
    char letters[5] = {'A', 'B', 'C', 'D', 'E'};
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (matrix[i][j] != ' ') {
                int randIndex;
                do {
                    randIndex = rand() % 5;
                } while (letterCount[randIndex] == 0);
                matrix[i][j] = letters[randIndex];
                letterCount[randIndex]--;
            }
        }
    }

    return matrix;
}
void print_menu(){
    int max_y,max_x;
    const char* menu[]={"New Game","Load Game","Quit"};
    initscr();
     noecho();                 // Kullanýcý girdisini ekranda göstermeme
    cbreak();
    getmaxyx(stdscr,max_y,max_x);
    WINDOW *win = newwin(5, 16,1, (max_x-23)/2);


    keypad(win,true);
    box(win,0,0);

    mvwprintw(win,2,4,"Skippity");
    wrefresh(win);
    //wmove(win,5,10);
    WINDOW *win2 = newwin(10, 16,5, (max_x-24)/2);
    keypad(win2,true);
    box(win2,0,0);
    wrefresh(win2);
    int curr_x=getcurx(win2);
    int curr_y=getcury(win2);
    int btn,choice=1;
    curr_y=curr_y+2;
    curr_x=curr_x+4;
    wmove(win2,curr_y,curr_x);
    wattron(win2,A_REVERSE);
    mvwprintw(win2,curr_y,curr_x,menu[0]);
    wattroff(win2,A_REVERSE);
    mvwprintw(win2,curr_y+2,curr_x,menu[1]);
    mvwprintw(win2,curr_y+4,curr_x+2,menu[2]);
    wmove(win2,curr_y,curr_x);
    wrefresh(win2);
    while((btn=wgetch(win2))!=10){
    //ten equals enter button

    switch(btn){
        case KEY_UP:
            if(choice==1){
                break;
            }
            else{

                if(choice==3){
                mvwprintw(win2,curr_y,curr_x+2,menu[choice-1]);
                }
                else{

                mvwprintw(win2,curr_y,curr_x,menu[choice-1]);
                }

                curr_y=curr_y-2;
                choice--;
                break;
            }
        case KEY_DOWN:
            if(choice==3){
                break;
            }
            else{

                mvwprintw(win2,curr_y,curr_x,menu[choice-1]);

                curr_y=curr_y+2;
                choice++;
                break;
            }
        default:
            break;
    }
    wmove(win2,curr_y,curr_x);
    wrefresh(win2);
    for(int i=1;i<4;i++){
        if(i==choice){

        wattron(win2,A_REVERSE);
        if(i==3){
             mvwprintw(win2,curr_y,curr_x+2,menu[i-1]);
        }
        else{
            mvwprintw(win2,curr_y,curr_x,menu[i-1]);

        }
        wattroff(win2,A_REVERSE);
        wrefresh(win);
        }
    }
}

if(choice==3){
    delwin(win);
    delwin(win2);
    endwin();
    exit(-1);
}
else if(choice==2){
    delwin(win);
    delwin(win2);
    endwin();
    print_saved();
}
else{
    //Choice 1
    delwin(win);
    delwin(win2);
    endwin();
    print_new_game_menu();
}
}
int main() {
    print_menu();
    return 0;
}
void print_new_game_menu(){
    const char* game_menu[]={"Two Players","Against the Com.","Back"};
    int max_y,max_x,curr_x,curr_y;
    initscr();
    noecho();
    cbreak();
    getmaxyx(stdscr,max_y,max_x);
    WINDOW *win = newwin(10, 25,1, (max_x-31)/2);
    keypad(win,true);
    box(win,0,0);
    curr_x=getcurx(win);
    curr_y=getcury(win);
    curr_x=curr_x+7;
    curr_y=curr_y+2;
    wmove(win,curr_y,curr_x);
    wattron(win,A_REVERSE);
    mvwprintw(win,curr_y,curr_x,game_menu[0]);
    wattroff(win,A_REVERSE);
    mvwprintw(win,curr_y+2,curr_x-2,game_menu[1]);
    mvwprintw(win,curr_y+4,curr_x+3,game_menu[2]);
    wmove(win,curr_y,curr_x);
    wrefresh(win);

    int choice=1;
    int btn;
    while((btn=wgetch(win))!=10){
    //ten equals enter button

    switch(btn){
        case KEY_UP:
            if(choice==1){
                break;
            }
            else{

                if(choice==3){
                mvwprintw(win,curr_y,curr_x+3,game_menu[choice-1]);
                }
                else if(choice==2){
                    mvwprintw(win,curr_y,curr_x-2,game_menu[choice-1]);
                }
                else{

                mvwprintw(win,curr_y,curr_x,game_menu[choice-1]);
                }

                curr_y=curr_y-2;
                choice--;
                break;
            }
        case KEY_DOWN:
            if(choice==3){
                break;
            }
            else if(choice==2){
                mvwprintw(win,curr_y,curr_x-2,game_menu[choice-1]);
            }
            else{

                mvwprintw(win,curr_y,curr_x,game_menu[choice-1]);

            }
            curr_y=curr_y+2;
            choice++;
            break;
        case 27:
            choice=3;
            goto button;
        default:
            break;
    }
    wmove(win,curr_y,curr_x);
    wrefresh(win);
    for(int i=1;i<4;i++){
        if(i==choice){

        wattron(win,A_REVERSE);
        if(i==3){
             mvwprintw(win,curr_y,curr_x+3,game_menu[i-1]);
        }
        else if(i==2){
            mvwprintw(win,curr_y,curr_x-2,game_menu[i-1]);

        }
        else{
            mvwprintw(win,curr_y,curr_x,game_menu[i-1]);

        }
        wattroff(win,A_REVERSE);
        wrefresh(win);
        }
    }
    }
button:
    if(choice==1){
        delwin(win);
        endwin();
        print_game_set(choice);
}

    else if(choice==2){
        wclear(win);
        wrefresh(win);
        delwin(win);
        endwin();
        print_not_available();

    }
    else{
        delwin(win);
        endwin();
        print_menu();
    }
}
void print_not_available() {
    int max_y, max_x, curr_x, curr_y;
    getmaxyx(stdscr, max_y, max_x);

    // Ortada pencere oluşturma
    WINDOW *temp_win = newwin(7, 28, 2, 44);
    box(temp_win, 0, 0);

    // Mesaj yazma
    mvwprintw(temp_win, 2, 4, "This is not available");

    // Back tuşu yazma
    wattron(temp_win,A_REVERSE);
    mvwprintw(temp_win, 4, 12, "Back");
    wattroff(temp_win,A_REVERSE);
    // Pencereyi gösterme
    wrefresh(temp_win);
    keypad(temp_win, true);

    int btn;
    while ((btn = wgetch(temp_win)) != 10) {
        // Enter tuşuna basılana kadar bekle
    }

    // Pencereyi temizleme ve ana menüye dönme
    wclear(temp_win);
    wrefresh(temp_win);
    delwin(temp_win);
    endwin();
    print_menu();
}
void print_game_set(int game_choice){
    const char* set_menu[]={"10x10","12x12","14x14","16x16","18x18","20x20","Back"};

    game_choice=1;
    int max_y,max_x,curr_x,curr_y;
    int choice=1;
    int btn;
    initscr();
    noecho();
    cbreak();
    getmaxyx(stdscr,max_y,max_x);
    WINDOW *win = newwin(20, 25,1, (max_x-31)/2);
    keypad(win,true);
    box(win,0,0);
    curr_x=getcurx(win);
    curr_y=getcury(win);
    curr_x=curr_x+10;
    curr_y=curr_y+2;
    wmove(win,curr_y,curr_x);
    mvwprintw(win,curr_y,curr_x-8,"Select Board Dimension");
    wattron(win,A_REVERSE);
    curr_y=curr_y+2;
    mvwprintw(win,curr_y,curr_x,set_menu[0]);
    wattroff(win,A_REVERSE);
    mvwprintw(win,curr_y+2,curr_x,set_menu[1]);
    mvwprintw(win,curr_y+4,curr_x,set_menu[2]);
    mvwprintw(win,curr_y+6,curr_x,set_menu[3]);
    mvwprintw(win,curr_y+8,curr_x,set_menu[4]);
    mvwprintw(win,curr_y+10,curr_x,set_menu[5]);
    mvwprintw(win,curr_y+12,curr_x,set_menu[6]);
    wmove(win,curr_y,curr_x);
    wrefresh(win);

    while((btn=wgetch(win))!=10){
    //ten equals enter button

    switch(btn){
        case KEY_UP:
            if(choice==1){
                break;
            }
            else{
                mvwprintw(win,curr_y,curr_x,set_menu[choice-1]);
                curr_y=curr_y-2;
                choice--;
                break;
            }
        case KEY_DOWN:
            if(choice==7){
                break;
            }
            else{

                mvwprintw(win,curr_y,curr_x,set_menu[choice-1]);

            }
            curr_y=curr_y+2;
            choice++;
            break;
        case 27:
            choice=7;
            goto button;

        default:
            break;
    }
    wmove(win,curr_y,curr_x);
    wrefresh(win);
    for(int i=1;i<8;i++){
        if(i==choice){
        wattron(win,A_REVERSE);
        mvwprintw(win,curr_y,curr_x,set_menu[i-1]);
        wattroff(win,A_REVERSE);
        wrefresh(win);
        }
    }
    }
button:
    if(choice==1){
        delwin(win);
        endwin();
        game_engine(game_choice,set_menu[choice-1],0);
}

    else if(choice==2){
        delwin(win);
        endwin();
        game_engine(game_choice,set_menu[choice-1],0);
    }
    else if(choice==3){
        delwin(win);
        endwin();
        game_engine(game_choice,set_menu[choice-1],0);
    }else if(choice==4){
        delwin(win);
        endwin();
        game_engine(game_choice,set_menu[choice-1],0);
    }else if(choice==5){
        delwin(win);
        endwin();
        game_engine(game_choice,set_menu[choice-1],0);
    }else if(choice==6){
        delwin(win);
        endwin();
        game_engine(game_choice,set_menu[choice-1],0);
    }
    else{
        delwin(win);
        endwin();
        print_new_game_menu();
    }

}
void printBoard(int choice, int size, int saved_index, WINDOW *win, WINDOW *win_player_1, WINDOW *win_player_2, WINDOW *info_win, char **matrix) {
    const char* letters[] = {"A:", "B:", "C:", "D:", "E:", "Point:", '\0'};

        int max_x = getmaxx(stdscr);

        // Configuring location of windows
        int curr_y = 20;
        int curr_x = 4 * size;
        int count = size / 2;
        int count_check = 0;
        int z=0;
        if (count % 2 == 0) {
            count_check = 1;
        }

        // Printing board for size
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {

                mvwprintw(win, i + 2,  2*j, "|");
                mvwprintw(win, i + 2,  2*j+1, "%c", matrix[i][j]);
            }
        }

        wrefresh(win);

        box(info_win, 0, 0);
        curr_x = getcurx(info_win);
        curr_y = getcury(info_win);
        mvwprintw(info_win, curr_y + 1, curr_x + 1, "Press Enter to Start!");

        mvwprintw(win_player_2, 1, 4, "Player2");
        mvwprintw(win_player_1, 1, 4, "Player1");
        for (int i = 0; letters[i] != '\0'; i++) {
            if (letters[i + 1] == '\0') {
                mvwprintw(win_player_1, i + 5, 2, letters[i]);
                mvwprintw(win_player_2, i + 5, 2, letters[i]);
            } else {
                mvwprintw(win_player_1, i + 3, 2, letters[i]);
                mvwprintw(win_player_2, i + 3, 2, letters[i]);
            }
        }

        box(win_player_1, 0, 0);
        box(win_player_2, 0, 0);
        wrefresh(info_win);
        wrefresh(win_player_1);
        wrefresh(win_player_2);
        return;

}
void print_saved(){
    const char* load_menu[]={"Slot1|","Slot2|","Slot3|","Slot4|","Back"};
    const char* delete_menu[]={"Delete Slot1","Delete Slot2","Delete Slot3","Delete Slot4","Back"};
    int max_y,max_x,curr_x,curr_y;
    int choice=1;
    int btn;
    int game_choice=0;
    getmaxyx(stdscr,max_y,max_x);
    WINDOW *win = newwin(16, 20,1, (max_x-25)/2);
    keypad(win,true);
    box(win,0,0);
    curr_x=getcurx(win);
    curr_y=getcury(win);
    curr_x=curr_x+5;
    curr_y=curr_y+2;
    wmove(win,curr_y,curr_x);
    mvwprintw(win,curr_y,curr_x,"Saved Games");
    wattron(win,A_REVERSE);
    curr_y=curr_y+2;
    mvwprintw(win,curr_y,curr_x+2,load_menu[0]);
    wattroff(win,A_REVERSE);
    mvwprintw(win,curr_y+2,curr_x+2,load_menu[1]);
    mvwprintw(win,curr_y+4,curr_x+2,load_menu[2]);
    mvwprintw(win,curr_y+6,curr_x+2,load_menu[3]);
    mvwprintw(win,curr_y+8,curr_x+3,load_menu[4]);
    wmove(win,curr_y,curr_x);
    wrefresh(win);

    while((btn=wgetch(win))!=10){
    //ten equals enter button

    switch(btn){
        case KEY_UP:
            if(choice==1){
                break;
            }
            else if(choice==2||choice==3||choice==4){
                mvwprintw(win,curr_y,curr_x+2,load_menu[choice-1]);

            }
            else{
                mvwprintw(win,curr_y,curr_x+3,load_menu[choice-1]);
            }
            curr_y=curr_y-2;
            choice--;
            break;
        case KEY_DOWN:
            if(choice==5){
                break;
            }
            else if(choice==1||choice==2||choice==3||choice==4){
                mvwprintw(win,curr_y,curr_x+2,load_menu[choice-1]);

            }
            else{

                mvwprintw(win,curr_y,curr_x+3,load_menu[choice-1]);

            }
            curr_y=curr_y+2;
            choice++;
            break;
        case 27:
            choice=5;
            goto button;
        case KEY_DC:
            wclear(win);
            wrefresh(win);
            delwin(win);
            endwin();
            print_delete_menu();
            return;
        default:
            break;
    }
    wmove(win,curr_y,curr_x);
    wrefresh(win);
    for(int i=1;i<6;i++){
        if(i==choice){
        wattron(win,A_REVERSE);
        if(choice==1||choice==2||choice==3||choice==4){
            mvwprintw(win,curr_y,curr_x+2,load_menu[i-1]);

        }
        else{
            mvwprintw(win,curr_y,curr_x+3,load_menu[i-1]);

        }
        wattroff(win,A_REVERSE);
        wrefresh(win);
        }
    }
}
button:
wclear(win);
wrefresh(win);
delwin(win);
endwin();
    if(choice==1){
        game_engine(game_choice,NULL,1);
    }
    else if(choice==2){
        game_engine(game_choice,NULL,2);
    }
    else if(choice==3){
        game_engine(game_choice,NULL,3);
    }
    else if(choice==4){
        game_engine(game_choice,NULL,4);
    }
    else{
        print_menu();
    }
}

void print_delete_menu() {
    const char* delete_menu[]={"Delete Slot1","Delete Slot2","Delete Slot3","Delete Slot4","Back"};
    int max_y, max_x, curr_x, curr_y;
    int choice = 1;
    int btn;
    getmaxyx(stdscr, max_y, max_x);
    WINDOW *win = newwin(16, 20, 1, (max_x - 25) / 2);
    keypad(win, true);
    box(win, 0, 0);
    curr_x = getcurx(win);
    curr_y = getcury(win);
    curr_x = curr_x + 5;
    curr_y = curr_y + 2;
    wmove(win, curr_y, curr_x);
    mvwprintw(win, curr_y, curr_x, "Delete Slot");
    wattron(win, A_REVERSE);
    curr_y = curr_y + 2;
    mvwprintw(win, curr_y, curr_x - 1, delete_menu[0]);
    wattroff(win, A_REVERSE);
    mvwprintw(win, curr_y + 2, curr_x - 1, delete_menu[1]);
    mvwprintw(win, curr_y + 4, curr_x - 1, delete_menu[2]);
    mvwprintw(win, curr_y + 6, curr_x - 1, delete_menu[3]);
    mvwprintw(win, curr_y + 8, curr_x + 3, delete_menu[4]);
    wmove(win, curr_y, curr_x);
    wrefresh(win);

    while((btn = wgetch(win)) != 10) {
        switch(btn) {
            case KEY_UP:
                if(choice == 1) {
                    break;
                } else {
                    if(choice == 2 || choice == 3 || choice == 4) {
                        mvwprintw(win, curr_y, curr_x - 1, delete_menu[choice - 1]);
                    } else {
                        mvwprintw(win, curr_y, curr_x + 3, delete_menu[choice - 1]);
                    }
                    curr_y = curr_y - 2;
                    choice--;
                    break;
                }
            case KEY_DOWN:
                if(choice == 5) {
                    break;
                } else {
                    if(choice == 1 || choice == 2 || choice == 3 || choice == 4) {
                        mvwprintw(win, curr_y, curr_x - 1, delete_menu[choice - 1]);
                    } else {
                        mvwprintw(win, curr_y, curr_x + 3, delete_menu[choice - 1]);
                    }
                    curr_y = curr_y + 2;
                    choice++;
                    break;
                }
            default:
                break;
        }
        wmove(win, curr_y, curr_x);
        wrefresh(win);
        for(int i = 1; i < 6; i++) {
            if(i == choice) {
                wattron(win, A_REVERSE);
                if(choice == 1 || choice == 2 || choice == 3 || choice == 4) {
                    mvwprintw(win, curr_y, curr_x - 1, delete_menu[i - 1]);
                } else {
                    mvwprintw(win, curr_y, curr_x + 3, delete_menu[i - 1]);
                }
                wattroff(win, A_REVERSE);
                wrefresh(win);
            }
        }
    }
    // İşaretli slotu sil
    if (choice >= 1 && choice <= 4) {
        delete_save_slot(choice);
    }
    wclear(win);
    wrefresh(win);
    delwin(win);
    endwin();
    print_menu();
}

void delete_save_slot(int slot_index) {
    FILE *save_file = fopen("save.txt", "r");
    if (save_file == NULL) {
        perror("Failed to open save file");
        exit(EXIT_FAILURE);
    }

    FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        perror("Failed to open temporary file");
        fclose(save_file);
        exit(EXIT_FAILURE);
    }

    char line[1024];
    int current_line = 0;
    while (fgets(line, sizeof(line), save_file)) {
        current_line++;
        if (current_line != slot_index) {
            fputs(line, temp_file);
        }
    }

    fclose(save_file);
    fclose(temp_file);

    remove("save.txt");
    rename("temp.txt", "save.txt");
}
void game_engine(int choice,const char* dimension[],int saved_index){
int move = 1;
int curr_x, curr_y;
int max_x = getmaxx(stdscr);
int size;
char **matrix;


Player *player1 = (Player *)malloc(sizeof(Player));
Player *player2 = (Player *)malloc(sizeof(Player));
    player1->A_point = 0;
    player1->B_point = 0;
    player1->C_point = 0;
    player1->D_point = 0;
    player1->E_point = 0;
    player1->all_point = 0;

    player2->A_point = 0;
    player2->B_point = 0;
    player2->C_point = 0;
    player2->D_point = 0;
    player2->E_point = 0;
    player2->all_point = 0;

if (player1 == NULL || player2 == NULL) {
    fprintf(stderr, "Bellek ayırma başarısız!\n");
    exit(1);
}

if (choice == 0) {
// Load saved game
FILE *save_file = fopen("save.txt", "r");
        if (save_file == NULL) {
            perror("Failed to open save file for reading");
            exit(1);
        }
        char line[1024];
        int current_line = 0;
        int found = 0;

        while (fgets(line, sizeof(line), save_file)) {
            current_line++;
            if (current_line == saved_index) {
                found = 1;
                break;
            }
        }

        if (!found) {
            fclose(save_file);
            print_not_available();
        }

        // Parse the saved data
        char *token = strtok(line, ";");
        token = strtok(NULL, ";"); // Boyut bilgisi (ikinci token)
        size = atoi(token);

        // Create matrix
        matrix = createMatrix(size);

        token = strtok(NULL, ";"); // Matris verisi
        char *matrix_token = strtok(token, ",");
        for (int i = 0; i < size; i++) {
            strncpy(matrix[i], matrix_token, size);
            matrix_token = strtok(NULL, ",");
        }

        token = strtok(NULL, ";"); // İlk oyuncunun puanları
        if (token != NULL) {
            sscanf(token, "A:%d,B:%d,C:%d,D:%d,E:%d,point:%d",
                &player1->A_point, &player1->B_point, &player1->C_point,
                &player1->D_point, &player1->E_point, &player1->all_point);
        }

        token = strtok(NULL, ";"); // İkinci oyuncunun puanları
        if (token != NULL) {
            sscanf(token, "A:%d,B:%d,C:%d,D:%d,E:%d,point:%d",
                &player2->A_point, &player2->B_point, &player2->C_point,
                &player2->D_point, &player2->E_point, &player2->all_point);
        }

        token = strtok(NULL, ";"); // Hamle sırası
        if (token != NULL) {
            sscanf(token, "move:%d", &move);
        }

        fclose(save_file);
} else {
    // Yeni oyun başlatma
    char str[3];

    strncpy(str, dimension, 2);
    str[2] = '\0';
    size = atoi(str);
    matrix = createMatrix(size);
}
initscr();
noecho();
cbreak();
keypad(stdscr, TRUE);
WINDOW *win=NULL,*info_win=NULL,*win_player_1=NULL,*win_player_2=NULL,*pause_win=NULL;
win_player_1 = newwin(13, 15, 1, 1);
win_player_2 = newwin(13, 15, 1, max_x - 15);

 if(size==10){
        win = newwin(size + 4, 4 * size , 4, (max_x - (4 * size-15)) / 2);
        info_win = newwin(4, 23,0,(max_x - (4 * size-13)) / 2);
    }
    else if(size==12){
        win = newwin(size + 4, 4 * size , 4, (max_x - (4 * size-20)) / 2);
       info_win = newwin(4, 23,0,(max_x - (4 * size-22)) / 2);
    }
    else if(size==14){
       win = newwin(size + 4, 4 * size , 4, (max_x - (4 * size-22)) / 2);
        info_win = newwin(4, 23,0,(max_x - (4 * size-28)) / 2);
    }
    else if(size==16){
        win = newwin(size + 4, 4 * size , 4, (max_x - (4 * size-28)) / 2);
        info_win = newwin(4, 23,0,(max_x - (4 * size-38)) / 2);
    }
    else if(size==18){
        win = newwin(size + 4, 4 * size , 4, (max_x - (4 * size-32)) / 2);
        info_win = newwin(4, 23,0,(max_x - (4 * size-46)) / 2);
    }
    else{
        win = newwin(size + 4, 4 * size , 4, (max_x - (4 * size-36)) / 2);
        info_win = newwin(4, 23,0,(max_x - (4 * size-52)) / 2);
}
printBoard(choice,size,saved_index,win,win_player_1,win_player_2,info_win,matrix);
wrefresh(win);
wrefresh(win_player_1);
wrefresh(win_player_2);
wrefresh(info_win);
int btn;
while(1){
btn=wgetch(win);
    if(btn==27){
        print_pause_win(pause_win,0,size,saved_index,win,win_player_1,win_player_2,info_win,matrix,NULL,NULL,0);
        //pause menu
    }
    else if(btn==10)
    {
        const char* info_box[]={"Player 1's Turn","Player 2's Turn"};
        curr_x=1;
        curr_y=2;
        wmove(win,curr_y,curr_x);
        wrefresh(win);
        wclear(info_win);
        box(info_win,0,0);
        if(move==1){
            mvwprintw(info_win,1,4,info_box[0]);
        }
        else{
             mvwprintw(info_win,1,4,info_box[1]);
        }
        keypad(win,TRUE);
        int btn;
        //if choice esc -> -1  if choice enter -> 0 if choice enter after esc -> 1 if choice enter after enter -> 2; if(choice==-2) thats explains its start
        int x=0,y=0,is_entered=0;
        wattron(win,A_REVERSE);
        mvwprintw(win,curr_y,curr_x,"%c",matrix[y][x]);
        wattroff(win,A_REVERSE);
        wrefresh(info_win);
        int entered_y,entered_x;
        // this location show us that
        // if you enter button you select a rock
        //that location points old location of rock
        int entered_first_move=0;
        int entered_y_matrix,entered_x_matrix;
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_RED);

        while(btn=wgetch(win)){
            switch(btn){

                case KEY_DOWN:
                    if(size-1==y){
                        break;
                    }
                    if(is_entered==1&&entered_x==curr_x&&curr_y+1==entered_y){
                        break;
                    }
                    if(is_entered==1&&entered_first_move==1){
                        entered_first_move=0;

                    }
                    else{
                        mvwprintw(win,curr_y,curr_x,"%c",matrix[y][x]);
                    }
                    y++;
                    curr_y++;
                    wattron(win,A_REVERSE);
                    mvwprintw(win,curr_y,curr_x,"%c",matrix[y][x]);
                    wattroff(win,A_REVERSE);
                    break;
                case KEY_UP:
                    if(y==0){
                        break;
                    }
                    if(is_entered==1&&entered_x==curr_x&&curr_y-1==entered_y){
                        break;
                    }
                   if(is_entered==1&&entered_first_move==1){
                        entered_first_move=0;

                    }
                    else{
                        mvwprintw(win,curr_y,curr_x,"%c",matrix[y][x]);
                    }

                    y--;
                    curr_y--;
                    wattron(win,A_REVERSE);
                    mvwprintw(win,curr_y,curr_x,"%c",matrix[y][x]);
                    wattroff(win,A_REVERSE);
                    break;
                case KEY_LEFT:
                    if(x==0){
                        break;
                    }
                    if(is_entered==1&&entered_x==curr_x-2&&curr_y==entered_y){
                        break;
                    }
                    if(is_entered==1&&entered_first_move==1){
                        entered_first_move=0;
                    }
                    else{
                        mvwprintw(win,curr_y,curr_x,"%c",matrix[y][x]);
                    }
                    x--;
                    curr_x=curr_x-2;

                    wattron(win,A_REVERSE);
                    mvwprintw(win,curr_y,curr_x,"%c",matrix[y][x]);
                    wattroff(win,A_REVERSE);
                    break;
                case KEY_RIGHT:
                    if(size-1==x){
                        break;
                    }
                    if(is_entered==1&&entered_x==curr_x+2&&curr_y==entered_y){
                        break;
                    }
                    if(is_entered==1&&entered_first_move==1){
                        entered_first_move=0;
                    }
                    else{
                        mvwprintw(win,curr_y,curr_x,"%c",matrix[y][x]);
                    }
                    x++;
                    curr_x=curr_x+2;
                    wattron(win,A_REVERSE);
                    mvwprintw(win,curr_y,curr_x,"%c",matrix[y][x]);
                    wattroff(win,A_REVERSE);
                    break;
                case 27:
                    print_pause_win(pause_win,1,size,saved_index,win,win_player_1,win_player_2,info_win,matrix,player1,player2,move);
                    if(is_entered==1){
                        mvwprintw(win,entered_y,entered_x," ");
                        wattron(win,COLOR_PAIR(1));
                        mvwprintw(win,entered_y,entered_x,"#");
                        wattroff(win,COLOR_PAIR(1));
                    }
                     if(curr_x!=entered_x||curr_y!=entered_y){
                         mvwprintw(win,curr_y,curr_x," ");
                         wattron(win,A_REVERSE);
                        mvwprintw(win,curr_y,curr_x,"%c",matrix[y][x]);
                        wattroff(win,A_REVERSE);
                     }
                    wclear(info_win);
                    box(info_win,0,0);
                    mvwprintw(info_win,1,4,info_box[0]);
                    wrefresh(info_win);
                    break;
                    //goto button;
                    //esc
                case 10:
                    if(is_entered==0){
                        entered_x=curr_x;
                        entered_y=curr_y;
                        entered_x_matrix=x;
                        entered_y_matrix=y;
                        is_entered=1;
                        wattron(win,COLOR_PAIR(1));
                        mvwprintw(win,curr_y,curr_x,"#");
                        wattroff(win,COLOR_PAIR(1));
                        entered_first_move=1;
                        break;
                    }
                    else{
                        char rock;
                        // kullanıcı hamle ile aldığı taşı temsil eden veri
                        mvwprintw(win,entered_y,entered_x,"%c",matrix[entered_y_matrix][entered_x_matrix]);
                        if(curr_y==entered_y-2&&curr_x==entered_x&&matrix[entered_y_matrix][entered_x_matrix]!=' '&&matrix[y][x]==' '&&matrix[y+1][x]!=' '){
                            mvwprintw(win,entered_y,entered_x," ");
                            mvwprintw(win,entered_y-1,entered_x," ");
                            wattron(win,A_REVERSE);
                            mvwprintw(win,curr_y,curr_x,"%c",matrix[entered_y_matrix][entered_x_matrix]);
                            wattroff(win,A_REVERSE);
                            matrix[y][x]=matrix[entered_y_matrix][entered_x_matrix];
                            matrix[entered_y_matrix][entered_x_matrix]=' ';
                            rock=matrix[y+1][x];
                            matrix[y+1][x]=' ';
                            //yukarı taş
                        }
                        else if(curr_x==entered_x&&curr_y==entered_y+2&&matrix[entered_y_matrix][entered_x_matrix]!=' '&&matrix[y][x]==' '&&matrix[y-1][x]!=' '){
                            mvwprintw(win,entered_y,entered_x," ");
                             mvwprintw(win,entered_y+1,entered_x," ");
                              wattron(win,A_REVERSE);
                            mvwprintw(win,curr_y,curr_x,"%c",matrix[entered_y_matrix][entered_x_matrix]);
                            wattroff(win,A_REVERSE);
                            matrix[y][x]=matrix[entered_y_matrix][entered_x_matrix];
                            matrix[entered_y_matrix][entered_x_matrix]=' ';
                            rock=matrix[y-1][x];
                            matrix[y-1][x]=' ';
                            //aşağı taş
                        }
                        else if(curr_x==entered_x+4&&curr_y==entered_y &&matrix[entered_y_matrix][entered_x_matrix]!=' '&&matrix[y][x]==' '&&matrix[y][x-1]!=' '){
                            mvwprintw(win,entered_y,entered_x," ");
                            mvwprintw(win,entered_y,entered_x+2," ");
                            wattron(win,A_REVERSE);
                            mvwprintw(win,curr_y,curr_x,"%c",matrix[entered_y_matrix][entered_x_matrix]);
                            wattroff(win,A_REVERSE);
                            matrix[y][x]=matrix[entered_y_matrix][entered_x_matrix];
                            matrix[entered_y_matrix][entered_x_matrix]=' ';
                            rock=matrix[y][x-1];
                            matrix[y][x-1]=' ';
                            //sağ taş
                        }
                        else if(curr_x==entered_x-4&&curr_y==entered_y&&matrix[entered_y_matrix][entered_x_matrix]!=' '&&matrix[y][x]==' '&&matrix[y][x+1]!=' '){
                            mvwprintw(win,entered_y,entered_x," ");
                            mvwprintw(win,entered_y,entered_x-2," ");
                             wattron(win,A_REVERSE);
                            mvwprintw(win,curr_y,curr_x,"%c",matrix[entered_y_matrix][entered_x_matrix]);
                            wattroff(win,A_REVERSE);
                            matrix[y][x]=matrix[entered_y_matrix][entered_x_matrix];
                            matrix[entered_y_matrix][entered_x_matrix]=' ';
                            rock=matrix[y][x+1];
                            matrix[y][x+1]=' ';
                            //puanlama
                            //sol taş
                        }
                        else{
                            is_entered=0;
                            break;
                            //not valid
                            //bu hamle geçersiz
                        }
                        is_entered=0;
                        switch(rock){
                        case 'A':
                            if(move==1){
                                player1->A_point++;
                                move=2;
                            }
                            else{
                                player2->A_point++;
                                move=1;
                            }
                            break;
                        case 'B':
                            if(move==1){
                                player1->B_point++;
                                move=2;
                            }
                            else{
                                player2->B_point++;
                                move=1;
                            }
                            break;
                        case 'C':
                            if(move==1){
                                player1->C_point++;
                                move=2;
                            }
                            else{
                                player2->C_point++;
                                move=1;
                            }
                            break;
                        case 'D':
                            if(move==1){
                                player1->D_point++;
                                move=2;
                            }
                            else{
                                player2->D_point++;
                                move=1;
                            }
                            break;
                        case 'E':
                            if(move==1){
                                player1->E_point++;
                                move=2;
                            }
                            else{
                                player2->E_point++;
                                move=1;
                            }
                            break;
                        default:
                            break;
                        }

                        wclear(info_win);
                        box(info_win,0,0);
                       if(move==1){
                        mvwprintw(info_win,1,4,"Players 1's Turn");
                       }
                       else{
                        mvwprintw(info_win,1,4,"Players 2's Turn");
                       }
                        int points1[]={player1->A_point,player1->B_point,player1->C_point,player1->D_point,player1->E_point};
                        int points2[]={player2->A_point,player2->B_point,player2->C_point,player2->D_point,player2->E_point};
                        int min1=player1->A_point,min2=player2->A_point;
                        int i;
                        for (i=0;i<5;i++){
                            mvwprintw(win_player_1, i + 3, 4,"%d",points1[i]);
                            mvwprintw(win_player_2, i + 3, 4,"%d",points2[i]);

                            if(points1[i]<min1){
                                min1=points1[i];
                            }
                           if(points2[i]<min2){
                                min2=points2[i];
                            }
                        }
                        player1->all_point=min1;
                        player2->all_point=min2;
                        mvwprintw(win_player_1,10, 8,"%d",min1);
                        mvwprintw(win_player_2,10, 8,"%d",min2);
                        break;
                        //move control
                        //and update and control move
                    }
                    //goto button;
                default:
                    break;
            }
            wmove(win,curr_y,curr_x);
            wrefresh(win);
            wrefresh(info_win);
            wrefresh(win_player_1);
            wrefresh(win_player_2);
            int is_done=1;
            for(int i=0;i<size;i++){
                for(int j=0;j<size;j++){
                if(i==0&&j==0){
                    if(matrix[i][j]!=' '&&(matrix[i][j+1]!=' '||matrix[i+1][j]!=' ')){
                        is_done=0;
                    }
                }
                else if(i==0&&j!=size-1){
                     if(matrix[i][j]!=' '&&(matrix[i][j-1]!=' '||matrix[i+1][j]!=' '||matrix[i][j+1]!=' ')){
                        is_done=0;
                    }
                }
                else if(i==0&&j==size-1){
                     if(matrix[i][j]!=' '&&(matrix[i][j-1]!=' '||matrix[i+1][j]!=' ')){
                        is_done=0;
                    }
                }
                else if(j==0&&i!=size-1){
                    if(matrix[i][j]!=' '&&(matrix[i][j+1]!=' '||matrix[i+1][j]!=' '||matrix[i-1][j]!=' ')){
                        is_done=0;
                    }
                }
                else if(j!=size-1&&i!=size-1){
                    if(matrix[i][j]!=' '&&(matrix[i][j+1]!=' '||matrix[i+1][j]!=' '||matrix[i-1][j]!=' '||matrix[i][j-1]!=' ')){
                        is_done=0;
                    }
                }
                else if(j==size-1&&i!=size-1){
                     if(matrix[i][j]!=' '&&(matrix[i][j-1]!=' '||matrix[i+1][j]!=' '||matrix[i-1][j]!=' ')){
                        is_done=0;
                    }
                }

                else if(j==0&&i==size-1){
                     if(matrix[i][j]!=' '&&(matrix[i-1][j]!=' '||matrix[i][j+1]!=' ')){
                        is_done=0;
                    }
                }
                else if(j!=size-1&&i==size-1){
                     if(matrix[i][j]!=' '&&(matrix[i-1][j]!=' '||matrix[i][j+1]!=' '||matrix[i][j-1]!=' ')){
                        is_done=0;
                    }
                }
                else if(j==size-1&&i==size-1){
                     if(matrix[i][j]!=' '&&(matrix[i][j-1]!=' '||matrix[i-1][j]!=' ')){
                        is_done=0;
                    }
                }
                else{
                    // continue;
                }
                if(is_done==0){
                    break;
                }
                }
            }
            if(is_done==1){
                break;
            }
            else{
                continue;
            }
            //control updated matrix
            // if cant play anymore steps for players
            //stop game break;
           //Control
        }
        //ended
        //we need to control everything
        //oyun bitti ekranı
        wclear(win);
        wclear(win_player_1);
        wclear(win_player_2);
        wclear(info_win);
        wrefresh(win);
        wrefresh(win_player_1);
        wrefresh(win_player_2);
        wrefresh(info_win);
        for (int i = 0; i < size; i++){
        free(matrix[i]);
    }
        free(matrix);
        delwin(win);
        delwin(win_player_1);
        delwin(win_player_2);
        delwin(info_win);

        WINDOW *game_end_title=newwin(5,16,1,50);
        box(game_end_title,0,0);
        //COLOR_PAIR()
        mvwprintw(game_end_title,2,3,"GAME OVER!");

        WINDOW *game_end_win=newwin(15,40,6,38);
        WINDOW *game_end_btn=newwin(7,16,21,50);
        const char *game_end_btn_list[]={"Back to Menu","Exit",'\0'};
        box(game_end_btn,0,0);
        box(game_end_win,0,0);
        wmove(game_end_btn,1,2);
        curr_x=getcurx(game_end_btn);
        curr_y=getcury(game_end_btn);
        wattron(game_end_btn,A_REVERSE);
        mvwprintw(game_end_btn,curr_y,curr_x,game_end_btn_list[0]);
        wattroff(game_end_btn,A_REVERSE);

         mvwprintw(game_end_btn,curr_y+2,curr_x+4,game_end_btn_list[1]);
        int btn_choice=1;
        keypad(game_end_btn,true);
        //print istatiskleri
        const char *letters[]={"A:","B:","C:","D:","E:","Point:",'\0'};
        int points1[]={player1->A_point,player1->B_point,player1->C_point,player1->D_point,player1->E_point,player1->all_point};
        int points2[]={player2->A_point,player2->B_point,player2->C_point,player2->D_point,player2->E_point,player2->all_point};
         mvwprintw(game_end_btn,curr_y,curr_x,game_end_btn_list[btn_choice-1]);
        mvwprintw(game_end_win,1,1,"Player 1");
        mvwprintw(game_end_win,1,30,"Player 2");
        for(int i=0;letters[i]!='\0';i++){
            mvwprintw(game_end_win,2*i+2,1,letters[i]);
            if(letters[i+1]=='\0'){
                mvwprintw(game_end_win,2*i+2,7,"%d",points1[i]);
                mvwprintw(game_end_win,2*i+2,36,"%d",points2[i]);
            }
            else{
                mvwprintw(game_end_win,2*i+2,3,"%d",points1[i]);
                mvwprintw(game_end_win,2*i+2,32,"%d",points2[i]);

            }
            mvwprintw(game_end_win,2*i+2,30,letters[i]);
        }
        if(points1[5]>points2[5]){
             mvwprintw(game_end_win,6,13,"PLAYER 1 WON!");
        }
        else if(points1[5]<points2[5]){
             mvwprintw(game_end_win,6,13,"PLAYER 2 WON!");
        }
        else{
             mvwprintw(game_end_win,6,17,"DRAW!");
        }
        wrefresh(game_end_title);
        wrefresh(game_end_btn);
        wrefresh(game_end_win);
        btn=0;
        while((btn=wgetch(game_end_btn))!=10){
            switch(btn){
                case KEY_DOWN:
                     if(btn_choice==2){
                            break;
                    }
                    else{
                        mvwprintw(game_end_btn,curr_y,curr_x,game_end_btn_list[btn_choice-1]);
                        btn_choice++;
                        curr_y=curr_y+2;
                        wattron(game_end_btn,A_REVERSE);
                        mvwprintw(game_end_btn,curr_y,curr_x+4,game_end_btn_list[btn_choice-1]);
                        wattroff(game_end_btn,A_REVERSE);
                        break;
                    }
                case KEY_UP:
                    if(btn_choice==1){
                        break;
                    }
                    else{
                        mvwprintw(game_end_btn,curr_y,curr_x+4,game_end_btn_list[btn_choice-1]);
                        btn_choice--;
                        curr_y=curr_y-2;
                        wattron(game_end_btn,A_REVERSE);
                        mvwprintw(game_end_btn,curr_y,curr_x,game_end_btn_list[btn_choice-1]);
                        wattroff(game_end_btn,A_REVERSE);
                        break;
                    }
                    break;
                default:
                    break;
            }
        wrefresh(game_end_btn);
    }
    wclear(game_end_btn);
    wclear(game_end_win);
    wclear(game_end_title);
    wrefresh(game_end_btn);
    wrefresh(game_end_title);
    wrefresh(game_end_win);
    delwin(game_end_btn);
    delwin(game_end_title);
    delwin(game_end_win);
    printf("%d\n",btn_choice);
    if(btn_choice==1){
        free(player1);
        free(player2);
        endwin();
        print_menu();
    }
    else{
    free(player1);
    free(player2);
    endwin();
     exit(0);
    }
    break;
    //recursive
    }
    else{
        continue;
    }
}
//quit: //with goto
exit(0);
}
void print_pause_win(WINDOW *pause_win,int game_started,int size,int saved_index,WINDOW *win,WINDOW *win_player_1,WINDOW *win_player_2,WINDOW *info_win,char** matrix,Player *player1,Player *player2,int move){
const char* pause_menu1[]={"Continue","Save","Exit"};
const char* pause_menu2[]={"Continue","Back"};
wclear(stdscr);
wrefresh(stdscr);
pause_win=newwin(8,12,10,52);
keypad(pause_win,true);
box(pause_win,0,0);
int curr_x,curr_y;
curr_x=getcurx(pause_win);
curr_y=getcury(pause_win);
curr_x=curr_x+2;
curr_y=curr_y+2;
int btn;
int choice=1;
int max_x=getmaxx(stdscr);

if(game_started==1){
    curr_y=curr_y-1;
    wattron(pause_win,A_REVERSE);
    mvwprintw(pause_win,curr_y,curr_x,pause_menu1[0]);
    wattroff(pause_win,A_REVERSE);
    mvwprintw(pause_win,curr_y+2,curr_x+2,pause_menu1[1]);
    mvwprintw(pause_win,curr_y+4,curr_x+2,pause_menu1[2]);

}
else{
    wattron(pause_win,A_REVERSE);
    mvwprintw(pause_win,curr_y,curr_x,pause_menu2[0]);
    wattroff(pause_win,A_REVERSE);
    mvwprintw(pause_win,curr_y+2,curr_x+2,pause_menu2[1]);
}
wrefresh(pause_win);
while((btn=wgetch(pause_win))!=10){

    switch(btn){
    case KEY_DOWN:
        if(game_started==1){
            if(choice==3){
                break;
            }
            else{
                if(choice==1){
                    mvwprintw(pause_win,curr_y,curr_x,pause_menu1[choice-1]);

                }
                else{
                    mvwprintw(pause_win,curr_y,curr_x+2,pause_menu1[choice-1]);
                }
                curr_y=curr_y+2;
                choice++;
                break;
            }
        }
        else{
            if(choice==2){
                break;
            }
            else{
                if(choice==1){
                mvwprintw(pause_win,curr_y,curr_x,pause_menu2[choice-1]);

                }
                else{
                    mvwprintw(pause_win,curr_y,curr_x+2,pause_menu2[choice-1]);

                }
                curr_y=curr_y+2;
                choice++;
                break;
            }
        }
    case KEY_UP:
        if(game_started==1)
        {
             if(choice==1){
                break;
            }
            else{

                mvwprintw(pause_win,curr_y,curr_x+2,pause_menu1[choice-1]);

                curr_y=curr_y-2;
                choice--;
                break;
            }
        }

        else{
             if(choice==1){
                break;
            }
            else{
                if(choice==1){
                    mvwprintw(pause_win,curr_y,curr_x,pause_menu2[choice-1]);

                }
                else{
                    mvwprintw(pause_win,curr_y,curr_x+2,pause_menu2[choice-1]);
                }
                curr_y=curr_y-2;
                choice--;
                break;
            }
        }
    case 27:
        choice=1;
        goto button;
    default:
        break;
    }
    //wattron config
    for(int i=1;i<4;i++){

        if(i==choice){
            wattron(pause_win,A_REVERSE);
            if(game_started==1){
                if(choice==1){
                mvwprintw(pause_win,curr_y,curr_x,pause_menu1[choice-1]);

                }
                else{
                mvwprintw(pause_win,curr_y,curr_x+2,pause_menu1[choice-1]);
                }
            }
            else{
                if(choice==1){
                mvwprintw(pause_win,curr_y,curr_x,pause_menu2[choice-1]);

                }
                else{
                mvwprintw(pause_win,curr_y,curr_x+2,pause_menu2[choice-1]);
                }
            }
            wattroff(pause_win,A_REVERSE);

        }
        else{
            continue;
            //mvprintw();
        }
    }
}
button:
win=NULL;
if(game_started==1){

if(choice==1){

            wclear(stdscr);
            wrefresh(stdscr);
            if(size==10){
            win = newwin(size + 4, 4 * size , 4, (max_x - (4 * size-15)) / 2);
            }
            else if(size==12){
                win = newwin(size + 4, 4 * size , 4, (max_x - (4 * size-20)) / 2);
            }
            else if(size==14){
               win = newwin(size + 4, 4 * size , 4, (max_x - (4 * size-22)) / 2);
            }
            else if(size==16){
                win = newwin(size + 4, 4 * size , 4, (max_x - (4 * size-28)) / 2);
            }
            else if(size==18){
                win = newwin(size + 4, 4 * size , 4, (max_x - (4 * size-32)) / 2);
            }
            else{
                win = newwin(size + 4, 4 * size , 4, (max_x - (4 * size-36)) / 2);
            }
            wclear(info_win);
            printBoard(choice,size,saved_index,win,win_player_1,win_player_2,info_win,matrix);
}
else if(choice==2)
{
 // Save game state to file
    FILE *save_file = fopen("save.txt", "r");
    if (save_file == NULL) {
        perror("Failed to open save file for reading");
        exit(EXIT_FAILURE);
    }

    int line_count = 0;
    char ch;
    while(!feof(save_file))
    {
        ch = fgetc(save_file);
        if(ch == '\n')
        {
            line_count++;
        }
    }
    fclose(save_file);

    // Kaydetme işlemi
    save_file = fopen("save.txt", "a");
    if (save_file == NULL) {
        perror("Failed to open save file for appending");
        exit(EXIT_FAILURE);
    }

    fprintf(save_file, "%d;%d;", line_count + 1, size); // Satır numarasını ve boyutu yaz
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fputc(matrix[i][j], save_file);
        }
        if (i < size - 1) {
            fputc(',', save_file); // Satır sonu işaretçileri
        }
    }
    fprintf(save_file, ";A:%d,B:%d,C:%d,D:%d,E:%d,point:%d;",
            player1->A_point, player1->B_point, player1->C_point,
            player1->D_point, player1->E_point, player1->all_point);
    fprintf(save_file, "A:%d,B:%d,C:%d,D:%d,E:%d,point:%d",
            player2->A_point, player2->B_point, player2->C_point,
            player2->D_point, player2->E_point, player2->all_point);
    fprintf(save_file, ";move:%d\n", move); // Hamle sırasını kaydet
    fclose(save_file);

    wclear(stdscr);
    wrefresh(stdscr);
    print_new_game_menu();
}
else{
    wclear(stdscr);
    wrefresh(stdscr);
    print_new_game_menu();
}
}
else{
    if(choice==1){
            wclear(stdscr);
            wrefresh(stdscr);

            if(size==10){
            win = newwin(size + 4, 4 * size , 4, (max_x - (4 * size-15)) / 2);

            }
            else if(size==12){
                win = newwin(size + 4, 4 * size , 4, (max_x - (4 * size-20)) / 2);

            }
            else if(size==14){
               win = newwin(size + 4, 4 * size , 4, (max_x - (4 * size-22)) / 2);

            }
            else if(size==16){
                win = newwin(size + 4, 4 * size , 4, (max_x - (4 * size-28)) / 2);

            }
            else if(size==18){
                win = newwin(size + 4, 4 * size , 4, (max_x - (4 * size-32)) / 2);

            }
            else{
                win = newwin(size + 4, 4 * size , 4, (max_x - (4 * size-36)) / 2);
            }
            wclear(info_win);
            printBoard(choice,size,saved_index,win,win_player_1,win_player_2,info_win,matrix);
    }
    else{
        wclear(stdscr);
        wrefresh(stdscr);
        print_new_game_menu();
    }
}
}
