#include <vector>
#include <chrono>
#include <thread>
#include <iostream>

#include "jogoSP.hpp"

#include <ncurses.h>
using namespace std::chrono;

Corpo::Corpo(std::vector<float> velocidade_init, std::vector<float> posicao_init) {
  
  this->velocidade = velocidade_init;
  this->posicao = posicao_init;
}

void Corpo::update(std::vector<float> nova_velocidade_x, std::vector<float> nova_posicao) {
  this->velocidade = nova_velocidade_x;
  this->posicao = nova_posicao;
}


std::vector< float > Corpo::get_velocidade() {
  return this->velocidade;
}

std::vector< float >  Corpo::get_posicao() {
  return this->posicao;
}

ListaDeCorpos::ListaDeCorpos() {
  this->corpos = new std::vector<Corpo *>(0);
}

void ListaDeCorpos::hard_copy(ListaDeCorpos *ldc) {
  std::vector<Corpo *> *corpos = ldc->get_corpos();

  for (int k=0; k<corpos->size(); k++) {
    Corpo *c = new Corpo( (*corpos)[k]->get_velocidade(),
                          (*corpos)[k]->get_posicao()                          
                        );
    this->add_corpo(c);
  }

}


void ListaDeCorpos::add_corpo(Corpo *c) {
  (this->corpos)->push_back(c);
}

std::vector<Corpo*> *ListaDeCorpos::get_corpos() {
  return (this->corpos);
}


Fisica::Fisica(ListaDeCorpos *ldc) {
  this->lista = ldc;
}

void Fisica::update(float deltaT, int sentido, int direcao) {
  // Atualiza parametros dos corpos!
  std::vector<Corpo *> *c = this->lista->get_corpos();
  for (int i = 0; i < (*c).size(); i++) {

    std::vector< float > new_vel = (*c)[i]->get_velocidade();
    std::vector< float > new_pos = (*c)[i]->get_posicao();
    
    new_vel[direcao] = 4*sentido;
    new_pos[direcao] = (*c)[i]->get_posicao()[direcao] + (float)deltaT * new_vel[direcao]/1000;

    move(i*LINES/(*c).size(),6);  
    
    refresh();

    (*c)[i]->update(new_vel, new_pos);
  }
}


Tela::Tela(ListaDeCorpos *ldc, int maxI, int maxJ, float maxX, float maxY) {
  this->lista = ldc;
  this->lista_anterior = new ListaDeCorpos();
  this->lista_anterior->hard_copy(this->lista);
  this->maxI = maxI;
  this->maxJ = maxJ;
  this->maxX = maxX;
  this->maxY = maxY;
}

void Tela::init() {
  initscr();             /* Start curses mode     */
  raw();                 /* Line buffering disabled */
  curs_set(0);           /* Do not display cursor */
}

void Tela::update() {
  int x,y;

  std::vector<Corpo *> *corpos_old = this->lista_anterior->get_corpos();

  // Apaga corpos na tela
  for (int k=0; k<corpos_old->size(); k++)
  {
    x = (int) ((*corpos_old)[k]->get_posicao()[1]) * \
        (this->maxI / this->maxX);
    y = (int) ((*corpos_old)[k]->get_posicao()[0]) * \
        (this->maxI / this->maxX);
    move(x, y);   /* Move cursor to position */
    echochar(' ');  /* Prints character, advances a position */
  }

  // Desenha corpos na tela
  std::vector<Corpo *> *corpos = this->lista->get_corpos();

  for (int k=0; k<corpos->size(); k++)
  {
    x = (int) ((*corpos_old)[k]->get_posicao()[1]) * \
        (this->maxI / this->maxX);
    y = (int) ((*corpos_old)[k]->get_posicao()[0]) * \
        (this->maxI / this->maxX);
    
     // Resolve bug de printar fora da tela
    if (x < LINES and y < COLS and x > -1 and y > -1){
      move(x, y);   /* Move cursor to position */
      // if ((*corpos)[k]->get_souPrincipal())
        echochar('C');  /* Prints character, advances a position */
      // else
        echochar('W');  /* Prints character, advances a position */  
    }

    // Atualiza corpos antigos
    (*corpos_old)[k]->update(  
                                (*corpos)[k]->get_velocidade(),\
                                (*corpos)[k]->get_posicao());
  }

  // Atualiza tela
  refresh();
}

void Tela::stop() {
  endwin();
}

Tela::~Tela() {
  this->stop();;
}

void threadfun (char *keybuffer, int *control)
{
  char c;
  while ((*control) == 1) {
    c = getch();
    if (c!=ERR) (*keybuffer) = c;
    else (*keybuffer) = 0;
    std::this_thread::sleep_for (std::chrono::milliseconds(10));
  }
  return;
}

Teclado::Teclado() {
}

Teclado::~Teclado() {
}

void Teclado::init() {
  // Inicializa ncurses
  raw();                 /* Line buffering disabled */
  keypad(stdscr, TRUE);  /* We get F1, F2 etc..   */
  noecho();              /* Don't echo() while we do getch */
  curs_set(0);           /* Do not display cursor */

  this->rodando = 1;
  std::thread newthread(threadfun, &(this->ultima_captura), &(this->rodando));
  (this->kb_thread).swap(newthread);

}

void Teclado::stop() {
  this->rodando = 0;
  (this->kb_thread).join();
}

char Teclado::getchar() {
  char c = this->ultima_captura;
  this->ultima_captura = 0;
  return c;
}