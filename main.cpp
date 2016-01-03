#include <iostream>
#include <random>
#include <time.h>
#include <conio.h>
#include <stdlib.h>

#define X_MAX 80
#define Y_MAX 23
#define ROOMS 8
#define ROOM_SIZE 7
#define ENEMIES_IN_LEVEL 5
#define HP_MAX 50

#define WALL 0xB2
#define EMPTY 0xFF
#define PLAYER 0x02
#define ENEMY 'X'
#define EXIT '>'
#define DOUBLELINE 0xCD

using namespace std;

//Mappa del gioco
unsigned char map[X_MAX][Y_MAX];

//Tutta sta roba non si potrebbe mettere in una classe giocatore o qualcosa del genere?
//Numero del piano raggiunto dal giocatore
int depth = 1;

//Salute del giocatore
int hp = HP_MAX;

//Inventario del giocatore
int pozioni_vita_piccole = 3;
int pozioni_vita_medie = 2;
int pozioni_vita_grandi = 1;

//Classe dei nemici
class Enemy
{
    public:
        int x;
        int y;
        void move()
        {
            //Se intorno c'è il giocatore
            if(map[x-1][y] == PLAYER || map[x+1][y] == PLAYER || map[x][y-1] == PLAYER || map[x][y+1] == PLAYER)
            {
                //Forse sarebbe meglio fare una funzione per togliere vita che controlla anche se va a 0...
                hp--;
            }
            else
            {
                //Muoviti in una direzione casuale
                bool moving = true;
                while(moving)
                {
                    int direction = rand() % 4;
                    switch(direction)
                    {
                        case 0: //Sinistra
                            if(map[x-1][y] == EMPTY)
                            {
                                map[x][y] = EMPTY;
                                map[x-1][y] = ENEMY;
                                x--;
                                moving = false;
                            }
                            break;
                        case 1: //Destra
                            if(map[x+1][y] == EMPTY)
                            {
                                map[x][y] = EMPTY;
                                map[x+1][y] = ENEMY;
                                x++;
                                moving = false;
                            }
                            break;
                        case 2: //Su
                            if(map[x][y-1] == EMPTY)
                            {
                                map[x][y] = EMPTY;
                                map[x][y-1] = ENEMY;
                                y--;
                                moving = false;
                            }
                            break;
                        case 3: //Giù
                            if(map[x][y+1] == EMPTY)
                            {
                                map[x][y] = EMPTY;
                                map[x][y+1] = ENEMY;
                                y++;
                                moving = false;
                            }
                            break;
                    }
                }
            }
        }
};

//Visualizza l'inventario
void inventory()
{
    system("cls");
    cout << "INVENTARIO: \n";
    for(int i = 0; i < X_MAX; i++)
    {
        cout << (char) DOUBLELINE;
    }
    //Anche qui, credo si possa migliorare qualcosa...
    cout << pozioni_vita_piccole << "x Pozione di Vita (p)iccola\tRipristina 10 Vita\n";
    cout << pozioni_vita_medie << "x Pozione di Vita (n)ormale\tRipristina 20 Vita\n";
    cout << pozioni_vita_grandi << "x Pozione di Vita (m)aggiore\tRipristina tutta la Vita\n";
    //Selezione dell'oggetto da usare.
    cout << "Scrivi la lettera corrispondente all'oggetto che vuoi usare.\n";
    while(true)
    {
        unsigned char selezione = getch();
        if(selezione == 'p')
        {

        }
        else if(selezione == 'n')
        {

        }
        else if(selezione == 'm')
        {
            
        }
    }
}

//Fai muovere il giocatore
int move(int player[2])
{
    int player_x = player[0];
    int player_y = player[1];
    bool waiting = true;
    //Rileva i tasti freccia
    while(waiting)
    {
        unsigned char input = getch();
        if(input == 224)
        {
            switch(getch())
            {
                case 72: //Freccia su
                    if(map[player_x][player_y-1] == EMPTY)
                    {
                        map[player_x][player_y] = EMPTY;
                        map[player_x][player_y-1] = PLAYER;
                        player_y--;
                        waiting = false;
                    }
                    else if(map[player_x][player_y-1] == EXIT)
                    {
                        return 1;
                    }
                    break;
                case 80: //Freccia giù
                    if(map[player_x][player_y+1] == EMPTY)
                    {
                        map[player_x][player_y] = EMPTY;
                        map[player_x][player_y+1] = PLAYER;
                        player_y++;
                        waiting = false;
                    }
                    else if(map[player_x][player_y+1] == EXIT)
                    {
                        return 1;
                    }
                    break;
                case 75: //Freccia sinistra
                    if(map[player_x-1][player_y] == EMPTY)
                    {
                        map[player_x][player_y] = EMPTY;
                        map[player_x-1][player_y] = PLAYER;
                        player_x--;
                        waiting = false;
                    }
                    else if(map[player_x-1][player_y] == EXIT)
                    {
                        return 1;
                    }
                    break;
                case 77: //Freccia destra
                    if(map[player_x+1][player_y] == EMPTY)
                    {
                        map[player_x][player_y] = EMPTY;
                        map[player_x+1][player_y] = PLAYER;
                        player_x++;
                        waiting = false;
                    }
                    else if(map[player_x+1][player_y] == EXIT)
                    {
                        return 1;
                    }
                    break;
            }
        }
        else if(input == 's') //S
        {
            //Salta un turno
            waiting = false;
        }
        else if(input == 'i') //I
        {
            //Apri l'inventario
            inventory();
        }
    }
    player[0] = player_x;
    player[1] = player_y;
    return 0;
}

//Aggiorna la console con la situazione corrente del gioco.
void draw()
{
    //Svuota lo schermo della console. Sono sicuro che ci sia un modo molto migliore per farlo, ma non mi viene in mente...
    system("cls");
    for(int y=0; y<Y_MAX; y++)
    {
        for(int x=0; x<X_MAX; x++)
        {
            cout << map[x][y];
        }
    }
    cout << "Piano: " << depth << ' ' << "Vita: " << hp;
}

//Funzioni per la generazione della mappa
//Inizializza la mappa con spazi vuoti
void init()
{
    for(int y=0; y<Y_MAX; y++)
    {
        for(int x=0; x<X_MAX; x++)
        {
            map[x][y] = WALL;
        }
    }
}

//Crea una stanza quadrata
void room(int start_x, int start_y, int end_x, int end_y)
{
    for(int y=start_y; y<=end_y; y++)
    {
        for(int x=start_x; x<=end_x; x++)
        {
            map[x][y] = EMPTY;
        }
    }
}

//Crea un corridoio che connetta due punti
void corridor(int start_x, int start_y, int end_x, int end_y, bool verticale)
{
    if(verticale)
    {
        if(start_y > end_y)
        {
            for(int y=end_y; y<=start_y; y++)
            {
                map[start_x][y] = EMPTY;
            }
        }
        else
        {
            for(int y=start_y; y<=end_y; y++)
            {
                map[start_x][y] = EMPTY;
            }
        }
        if(start_x > end_x)
        {
            for(int x=end_x; x<=start_x; x++)
            {
                map[x][end_y] = EMPTY;
            }
        }
        else
        {
            for(int x=start_x; x<=end_x; x++)
            {
                map[x][end_y] = EMPTY;
            }
        }
    }
    else
    {
        if(start_x > end_x)
        {
            for(int x=end_x; x<=start_x; x++)
            {
                map[x][start_y] = EMPTY;
            }
        }
        else
        {
            for(int x=start_x; x<=end_x; x++)
            {
                map[x][start_y] = EMPTY;
            }
        }
        if(start_y > end_y)
        {
            for(int y=end_y; y<=start_y; y++)
            {
                map[end_x][y] = EMPTY;
            }
        }
        else
        {
            for(int y=start_y; y<=end_y; y++)
            {
                map[end_x][y] = EMPTY;
            }
        }
    }
}

void generate(int player[2], Enemy* list[ENEMIES_IN_LEVEL])
{
    int corridor_x;
    int corridor_y;
    //Creazione delle stanze
    for(int r=0; r<ROOMS; r++)
    {
        int size_x = rand() % ROOM_SIZE + 1;
        int size_y = rand() % ROOM_SIZE + 1;
        int start_x = rand() % (X_MAX - size_x - 2) + 1;
        int start_y = rand() % (Y_MAX - size_y - 2) + 1;
        room(start_x, start_y, start_x + size_x, start_y + size_y);
        //Se non è la prima stanza, crea un corridoio che connetta quella appena generata con quella precedente
        if(r > 0)
        {
            int link_x = rand() % size_x + 1 + start_x;
            int link_y = rand() % size_y + 1 + start_y;
            corridor(link_x, link_y, corridor_x, corridor_y, rand() % 2);
        }
        corridor_x = rand() % size_x + start_x;
        corridor_y = rand() % size_y + start_y;
        //Posiziona il giocatore se è l'ultima stanza
        if(r == ROOMS - 1)
        {
            player[0] = corridor_x;
            player[1] = corridor_y;
            map[corridor_x][corridor_y] = PLAYER;
        }
    }
    //Posizionamento nemici
    for(int e=0; e<ENEMIES_IN_LEVEL; e++)
    {
        while(true)
        {
            int x = rand() % (X_MAX - 1) + 1;
            int y = rand() % (Y_MAX - 1) + 1;
            if(map[x][y] == EMPTY)
            {
                map[x][y] = ENEMY;
                Enemy* created = new Enemy();
                created->x = x;
                created->y = y;
                list[e] = created;
                break;
            }
        }
    }
    //Posizionamento uscita
    while(true)
    {
        int x = rand() % (X_MAX - 1) + 1;
        int y = rand() % (Y_MAX - 1) + 1;
        if(map[x][y] == EMPTY)
        {
            map[x][y] = EXIT;
            break;
        }
    }

}

//Processa il resto di un turno, dopo il movimento del giocatore.
void tick(Enemy* list[ENEMIES_IN_LEVEL])
{
    for(int e=0; e<ENEMIES_IN_LEVEL; e++)
    {
        list[e]->move();
    }
}

int main()
{
    int player[2];
    Enemy* list[ENEMIES_IN_LEVEL]; //Lista di tutti i nemici nel livello
    srand(0); //TODO: Rendere il seed modificabile...?
    //Ciclo del gioco
    while(true)
    {
        init();
        generate(player, list);
        draw();
        //Ciclo di un livello
        while(true)
        {
            int trigger = move(player);
            if(trigger == 1) //Uscita toccata
            {
                break;
            }
            tick(list);
            draw();
        }
        depth++;
    }
    return 0;
}
