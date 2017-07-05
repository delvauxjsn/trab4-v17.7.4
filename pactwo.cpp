#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "allegro5/allegro_image.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <iostream>
#include <stdio.h>

using namespace std;

const float FPS = 2;
const int SCREEN_W = 500;
const int SCREEN_H = 550;
int points=0;

enum MYKEYS
{
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

//matriz definindo mapa do jogo: 1 representa paredes, 0 representa corredor
char MAPA[26][26] =
{
    "1111111111111111111111111",
    "1222222222222222222222221",
    "1211112111212121112111121",
    "1222212122212122212122221",
    "1211212121112111212121121",
    "1221212121222221212121221",
    "1121222221211121222221211",
    "1121112111222221112111211",
    "1122222222212122222222211",
    "1111211121112111211121111",
    "1112222222222222222222111",
    "1222111112112112111112221",
    "1211111222112112221111121",
    "1222122212112112122212221",
    "1112121112220222111212111",
    "1222122212112112122212221",
    "1211111222112112221111121",
    "1222211112112112111122221",
    "1121222222222222222221211",
    "1121111122112112211111211",
    "1222221112212122111222221",
    "1211122222222222222211121",
    "1211111111112111111111121",
    "1222222222222222222222221",
    "1111111111111111111111111",
};


ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *bouncer = NULL;
ALLEGRO_BITMAP *mapa   = NULL;
ALLEGRO_BITMAP *pacman   = NULL;
ALLEGRO_AUDIO_STREAM *musica = NULL;
ALLEGRO_SAMPLE *sample = NULL;

int i = 14, j = 12; //posição inicial do Pacman na matriz
int q = 20; //tamanho de cada célula no mapa
int posy = i*q;
int posx = j*q;
int dirpac = -1;
int balaoy = q;
int balaox = q;
ALLEGRO_BITMAP *balao = NULL;
ALLEGRO_BITMAP *bug1 = NULL;
ALLEGRO_BITMAP *bug2 = NULL;
ALLEGRO_BITMAP *bug3 = NULL;
ALLEGRO_BITMAP *bug4 = NULL;
int bug1x = 1*q;
int bug1y = 1*q;
int bug2x = 1*q;
int bug2y = 23*q;
int bug3x = 23*q;
int bug3y = 1*q;
int bug4x = 23*q;
int bug4y = 23*q;
int dirbug1=1;
int dirbug2=0;
int dirbug3=3;
int dirbug4=2;
char spriteb1[10] = {'b','u','g','1','1','.','t','g','a','\0'};
char sprt2[10] = {'b','u','g','1','0','.','t','g','a','\0'};
char spr3[10] = {'b','u','g','1','3','.','t','g','a','\0'};
char sp4[10] = {'b','u','g','1','2','.','t','g','a','\0'};


bool key[4] = { false, false, false, false };
bool redraw = true;
bool sair = false;

int inicializa() {
    if(!al_init())
    {
        cout << "Falha ao carregar Allegro" << endl;
        return 0;
    }

    // Inicialização do add-on para uso de fontes
    al_init_font_addon();
 
    // Inicialização do add-on para uso de fontes True Type
    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
        return 0;
    }

    if (!al_install_audio())
    {
        cout << "Falha ao inicializar audio" << endl;
        return 0;
    }

    if (!al_init_acodec_addon())
    {
        cout << "Falha ao inicializar codecs de audio." << endl;;
        return 0;
    }

    if (!al_reserve_samples(1))
    {
        cout << "Falha ao alocar canais de audio." << endl;
        return 0;
    }

    if(!al_install_keyboard())
    {
        cout << "Falha ao inicializar o teclado" << endl;
        return 0;
    }

    timer = al_create_timer(1.0 / FPS);
    if(!timer)
    {
        cout << "Falha ao inicializar o temporizador" << endl;
        return 0;
    }

    if(!al_init_image_addon())
    {
        cout <<"Falha ao iniciar al_init_image_addon!" << endl;
        return 0;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display)
    {
        cout << "Falha ao inicializar a tela" << endl;
        al_destroy_timer(timer);
        return 0;
    }

    mapa = al_load_bitmap("map2.bmp");
    if(!mapa)
    {
        cout << "Falha ao carregar o mapa!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(mapa,0,0,0);

    // Carregando o arquivo de fonte

    fonte = al_load_font("fonts/DejaVuSans-Bold.ttf", 30, 0);

    if (!fonte)
    {
        fprintf(stderr, "Falha ao carregar fonte.\n");
        al_destroy_display(display);
        return 0;
    }

    pacman = al_load_bitmap("pacmanL.tga");
    if(!pacman)
    {
        cout << "Falha ao carregar o pacman!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(pacman,posx,posy,0);

    sample = al_load_sample("musicas/mc.wav");
    if (!sample)
    {
        cout<< "Falha ao carregar sample" << endl;;
        al_destroy_display(display);
        return 0;
    }

    balao = al_load_bitmap("balao.tga");
    if(!balao)
    {
        cout << "Falha ao carregar os baloes!" << endl;
        al_destroy_display(display);
        return 0;
    }

    for(int l=1; l < 24; l++)
        for(int k=1; k < 24; k++){
            if (MAPA[l][k] == '2'){
                balaox = k * q;
                balaoy = l * q;
                al_draw_bitmap(balao, balaox, balaoy,0);
            }
        }
    bug1 = al_load_bitmap(spriteb1);
    bug2 = al_load_bitmap(sprt2);
    bug3 = al_load_bitmap(spr3);
    bug4 = al_load_bitmap(sp4);
    if(!bug1 || !bug2 || !bug3 || !bug4)
    {
        cout << "Falha ao carregar os bugs!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(bug1,bug1x,bug1y,0);
    al_draw_bitmap(bug1,bug2x,bug2y,0);
    al_draw_bitmap(bug1,bug3x,bug3y,0);
    al_draw_bitmap(bug1,bug4x,bug4y,0);

    event_queue = al_create_event_queue();
    if(!event_queue)
    {
        cout << "Falha ao criar a fila de eventos" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        return 0;
    }

    al_set_window_title(display, "PacLevi");

    musica = al_load_audio_stream("musicas/sas.ogg", 4, 1024);
    if (!musica)
    {
        cout<<"Falha ao carregar audio."<<endl;
        al_destroy_event_queue(event_queue);
        al_destroy_display(display);
        al_destroy_sample(sample);
        return 0;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_W / 2, 500, ALLEGRO_ALIGN_LEFT, "Pontuacao: %d", points);
    al_flip_display();
    al_start_timer(timer);

    return 1;
}

int gera_num(){
      int aleatorio = ( rand() % 4 );
      return aleatorio;
}

bool possivel (int const &bugi, int const &bugj, int const &dir ){
    switch(dir){
        case 0:
            if ( MAPA[bugi-1][bugj] == '1')
            return false;
            break;
        case 1:
            if ( MAPA[bugi][bugj+1] == '1')
            return false;
            break;
        case 2:
            if( MAPA[bugi+1][bugj] == '1')
            return false;
            break;
        case 3:
            if ( MAPA[bugi][bugj-1] == '1')
            return false;
            break;
    }
    return true;
}

int possibilidades (int const &bugi, int const &bugj){
    int possibilidade=4;
    if ( MAPA[bugi-1][bugj] == '1')
        possibilidade--;
    if ( MAPA[bugi][bugj+1] == '1')
        possibilidade--;
    if( MAPA[bugi+1][bugj] == '1')
        possibilidade--;
    if ( MAPA[bugi][bugj-1] == '1')
        possibilidade--;
    cout << "possibilidades:" << possibilidade << endl;
    return possibilidade;
}

bool movimenta_bug (int &bugy, int &bugx, int &dir, char sprite[]){
    if (sprite[3] == '1')
        sprite[3] = '2';
    else
        sprite[3] = '1';

    int bugi = bugy/q;
    int bugj = bugx/q;
    cout << "direcao: "<<dir << endl;
    if (possibilidades(bugi,bugj) > 2 || !possivel(bugi,bugj,dir)){
        do{
            for(int muda = dir; dir == muda;)
                dir = gera_num();
            cout << "mudou dir para:"  << dir << endl;
        }while(!possivel(bugi,bugj,dir));

       switch(dir){
            case 0:
                sprite[4] = '0';
                break;
            case 1:
                sprite[4] = '1';
                break;
            case 2:
                sprite[4] = '2';
                break;
            case 3:
                sprite[4] = '3';
                break;
        }
    }

    switch(dir){
        case 0:
            bugi--;
            bugy = bugi*q;
            if( (posy == bug1y && posx == bug1x) || (posy == bug2y && posx == bug2x) || (posy == bug3y && posx == bug3x) || (posy == bug4y && posx == bug4x)){
                cout << "Game Over!!!" << endl;
                return true;
            }
        break;

        case 1:
            bugj++;
            bugx = bugj*q;
            if( (posy == bug1y && posx == bug1x) || (posy == bug2y && posx == bug2x) || (posy == bug3y && posx == bug3x) || (posy == bug4y && posx == bug4x)){
                cout << "Game Over!!!" << endl;
                return true;
            }
        break;

        case 2:
            bugi++;
            bugy = bugi*q;
            if( (posy == bug1y && posx == bug1x) || (posy == bug2y && posx == bug2x) || (posy == bug3y && posx == bug3x) || (posy == bug4y && posx == bug4x)){
                cout << "Game Over!!!" << endl;
                return true;
            }
        break;

        case 3:
            bugj--;
            bugx = bugj*q;
            if( (posy == bug1y && posx == bug1x) || (posy == bug2y && posx == bug2x) || (posy == bug3y && posx == bug3x) || (posy == bug4y && posx == bug4x)){
                cout << "Game Over!!!" << endl;
                return true;
            }
        break;
    }
    return false;
}

void movimenta_pacman (){
    static int dir = -1;
    if ( possivel(i,j,dirpac))
        dir=dirpac;
    switch (dir){
        case 0:
            if(MAPA[i-1][j] != '1')
            {
                 pacman = al_load_bitmap("pacmanU.tga");
                if(MAPA[i-1][j]=='2'){
                    points++;
                    cout << "Mais um ponto!!!" << endl;
                    MAPA[i-1][j]='0';
                }
                i--;
                posy = i*q;
            }
        break;

        case 1:
            if(MAPA[i][j+1] != '1')
            {
                pacman = al_load_bitmap("pacmanR.tga");
                if(MAPA[i][j+1]=='2'){
                    points++;
                    cout << "Mais um ponto!!!" << endl;
                    MAPA[i][j+1]='0';
                }
                j++;
                posx = j*q;
            }
        break;

        case 2:
            if(MAPA[i+1][j] != '1')
            {
                 pacman = al_load_bitmap("pacmanD.tga");
                if(MAPA[i+1][j] =='2'){
                    points++;
                    cout << "Mais um ponto!!!" << endl;
                    MAPA[i+1][j]='0';
                }
                i++;
                posy = i*q;
            }
        break;

        case 3:
            if(MAPA[i][j-1] != '1')
            {
                 pacman = al_load_bitmap("pacmanL.tga");
                if(MAPA[i][j-1]=='2'){
                    points++;
                    cout << "Mais um ponto!!!" << endl;
                    MAPA[i][j-1]='0';
                }
                j--;
                posx = j*q;
            }
        break;
    }
}

void movimento(){
    bug1 = al_load_bitmap(spriteb1);
    bug2 = al_load_bitmap(sprt2);
    bug3 = al_load_bitmap(spr3);
    bug4 = al_load_bitmap(sp4);
}

int main(int argc, char **argv)
{
    if(!inicializa()) return -1;

    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
    al_set_audio_stream_playing(musica, true);


    while(!sair)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            if(key[KEY_UP])
            {
                dirpac = 0;
                if( (posy == bug1y && posx == bug1x) || (posy == bug2y && posx == bug2x) || (posy == bug3y && posx == bug3x) || (posy == bug4y && posx == bug4x)){
                    cout << "Game Over!!!" << endl;
                    sair = true;
                    break;
                }
            }

            if(key[KEY_DOWN])
            {
                dirpac = 2;
                 if( (posy == bug1y && posx == bug1x) || (posy == bug2y && posx == bug2x) || (posy == bug3y && posx == bug3x) || (posy == bug4y && posx == bug4x)){
                    cout << "Game Over!!!" << endl;
                    sair = true;
                    break;
                }
            }

            if(key[KEY_LEFT])
            {
                dirpac = 3;
                 if( (posy == bug1y && posx == bug1x) || (posy == bug2y && posx == bug2x) || (posy == bug3y && posx == bug3x) || (posy == bug4y && posx == bug4x)){
                    cout << "Game Over!!!" << endl;
                    sair = true;
                    break;
                }
            }

            if(key[KEY_RIGHT])
            {
                dirpac = 1;
                if( (posy == bug1y && posx == bug1x) || (posy == bug2y && posx == bug2x) || (posy == bug3y && posx == bug3x) || (posy == bug4y && posx == bug4x)){
                    cout << "Game Over!!!" << endl;
                    sair = true;
                    break;
                }
            }
            movimenta_pacman();
            sair=movimenta_bug(bug1y,bug1x,dirbug1,spriteb1);
            sair=movimenta_bug(bug2y,bug2x,dirbug2,sprt2);
            sair=movimenta_bug(bug3y,bug3x,dirbug3,spr3);
            sair=movimenta_bug(bug4y,bug4x,dirbug4,sp4);

            movimento();

            redraw = true;
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                key[KEY_UP] = true;
                key[KEY_DOWN] = false;
                key[KEY_LEFT] = false;
                key[KEY_RIGHT] = false;
                break;

            case ALLEGRO_KEY_DOWN:
                key[KEY_UP] = false;
                key[KEY_DOWN] = true;
                key[KEY_LEFT] = false;
                key[KEY_RIGHT] = false;
                break;

            case ALLEGRO_KEY_LEFT:
                key[KEY_UP] = false;
                key[KEY_DOWN] = false;
                key[KEY_LEFT] = true;
                key[KEY_RIGHT] = false;
                break;

            case ALLEGRO_KEY_RIGHT:
                key[KEY_UP] = false;
                key[KEY_DOWN] = false;
                key[KEY_LEFT] = false;
                key[KEY_RIGHT] = true;
                break;
            }
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(ev.keyboard.keycode){
	            case ALLEGRO_KEY_ESCAPE:
	                sair = true;
	                break;
            }
        }

        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;

            al_clear_to_color(al_map_rgb(0,0,0));

            al_draw_bitmap(mapa,0,0,0);
            al_draw_bitmap(pacman,posx,posy,0);
            al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_W / 2, 500, ALLEGRO_ALIGN_LEFT, "Pontuacao: %d", points);
            for(int l=1; l < 25; l++)
                for(int k=1; k < 25; k++){
                    if (MAPA[l][k] == '2'){
                        balaox = k * q;
                        balaoy = l * q;
                        al_draw_bitmap(balao, balaox, balaoy,0);
                    }
                }
            al_draw_bitmap(bug1,bug1x,bug1y,0);
            al_draw_bitmap(bug2,bug2x,bug2y,0);
            al_draw_bitmap(bug3,bug3x,bug3y,0);
            al_draw_bitmap(bug4,bug4x,bug4y,0);
            al_flip_display();
        }
    }

    al_destroy_audio_stream(musica);
    al_destroy_sample(sample);
    al_destroy_bitmap(bouncer);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}