#include <vector>
#include <chrono>
#include <thread>
#include <iostream>

#include "jogoSP.hpp"

#include <stdlib.h>
#include <ncurses.h>
using namespace std::chrono;

extern bool JogadorPerdeu;
extern bool JogadorGanhou; 

#define Vel_H 3.75
#define Vel_V 3.1
#define MaxMoedas 4

char mapa[23][72] = {

  {'*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*'},
  {'*','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','*'},
  {'*','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ','|','*'},
  {'*','|',' ',' ',' ','|','$',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ','$',' ',' ',' ',' ',' ','|','*'},
  {'*','|',' ',' ',' ','|','-','-','-','-','-','-','-','-','-','|',' ',' ',' ',' ',' ',' ','|','-','-','-','-','-','-','-','-','-','|',' ',' ',' ',' ',' ',' ','|','-','-','-','-','-','-','-','-','-','|',' ',' ',' ',' ',' ',' ','|','-','-','-','-','-','-','-','-','-','|',' ',' ',' ','|','*'},
  {'*','|',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ','|','*'},
  {'*','|',' ','|',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ','|',' ','|','*'},
  {'*','|',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ','|','*'},
  {'*','|',' ','|',' ','-','-','|',' ',' ','|',' ',' ','|','-','-',' ','|',' ',' ','|',' ','-','-','|',' ',' ','|',' ',' ','|','-','-',' ','|',' ',' ','|',' ','-','-','|',' ',' ','|',' ',' ','|','-','-',' ','|',' ',' ','|',' ','-','-','|',' ',' ','|',' ',' ','|','-','-',' ','|',' ','|','*'},
  {'*','|',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ','|','*'},
  {'*','|',' ',' ',' ',' ',' ','-','-','-','|','-','-','-',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','-','-','-','|','-','-','-',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','-','-','-','|','-','-','-',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','-','-','-','|','-','-','-',' ',' ',' ',' ',' ','|','*'},
  {'*','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','*'},
  {'*','|',' ',' ',' ',' ',' ','-','-','-','|','-','-','-',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','-','-','-','|','-','-','-',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','-','-','-','|','-','-','-',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','-','-','-','|','-','-','-',' ',' ',' ',' ',' ','|','*'},
  {'*','|',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ','|','*'},
  {'*','|',' ','|',' ','-','-','|',' ',' ','|',' ',' ','|','-','-',' ','|',' ',' ','|',' ','-','-','|',' ',' ','|',' ',' ','|','-','-',' ','|',' ',' ','|',' ','-','-','|',' ',' ','|',' ',' ','|','-','-',' ','|',' ',' ','|',' ','-','-','|',' ',' ','|',' ',' ','|','-','-',' ','|',' ','|','*'},
  {'*','|',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ','|','*'},
  {'*','|',' ','|',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ','|',' ',' ','|',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ','|',' ','|','*'},
  {'*','|',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ','|','*'},
  {'*','|',' ',' ',' ','|','-','-','-','-','-','-','-','-','-','|',' ',' ',' ',' ',' ',' ','|','-','-','-','-','-','-','-','-','-','|',' ',' ',' ',' ',' ',' ','|','-','-','-','-','-','-','-','-','-','|',' ',' ',' ',' ',' ',' ','|','-','-','-','-','-','-','-','-','-','|',' ',' ',' ','|','*'},
  {'*','|',' ',' ',' ','|','$',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','$',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','*'},
  {'*','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ','|','*'},
  {'*','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','*'},
  {'*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*'}

};

void printStr(int row, int col,std::string frase){

  move(row,col);
  for(char& c : frase) {
    echochar(c);
  }

}

Corpo::Corpo(std::vector<float> velocidade_init, std::vector<float> posicao_init) {
  
  this->velocidade = velocidade_init;
  this->posicao = posicao_init;
}

Bot::Bot(std::vector< float > init_posicao){
  this->posicao = init_posicao;
}

void Bot::update(std::vector<float> velocidade_init,std::vector< float > nova_posicao){
  this->velocidade = velocidade_init;
  this->posicao = nova_posicao;
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

void Corpo::addMoeda(){
  this->moedas++;
}

int Corpo::getMoeda(){
  return this->moedas;
}

std::vector< float > Bot::get_velocidade() {
  return this->velocidade;
}

std::vector< float >  Bot::get_posicao() {
  return this->posicao;
}

ListaDeCorpos::ListaDeCorpos() {
  this->corpos = new std::vector<Corpo *>(0);
}

ListaDeBots::ListaDeBots() {
  this->bots = new std::vector<Bot *>(0);
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

void ListaDeBots::hard_copy(ListaDeBots *ldb) {
  std::vector<Bot *> *bots = ldb->get_bots();

  for (int k=0; k<bots->size(); k++) {
    Bot *bot = new Bot((*bots)[k]->get_posicao());
    this->add_bot(bot);
  }

}

void Bot::set_permissao(int orientacao, int movimentos){
  this->permissao[orientacao] = movimentos;
}

int Bot::get_permissao(int orientacao){
  return this->permissao[orientacao];
}

void Bot::update_permissao(){
  int i;

  for(i=0;i<4;i++){
    if(this->permissao[i] > 0)
      this->permissao[i]--;
  }
}

void ListaDeCorpos::add_corpo(Corpo *c) {
  (this->corpos)->push_back(c);
}

void ListaDeBots::add_bot(Bot *bot) {
  (this->bots)->push_back(bot);
}

void ListaDeBots::kill_bot(int indice) {
  this->bots->erase(this->bots->begin() + indice);
}

std::vector<Corpo*> *ListaDeCorpos::get_corpos() {
  return (this->corpos);
}

std::vector<Bot*> *ListaDeBots::get_bots() {
  return (this->bots);
}

Projetil::Projetil(std::vector< float > velocidade, std::vector< float > posicao, bool tenhoProjetil, bool aindaExisto){
  this->velocidade = velocidade;
  this->posicao = posicao;
  this->tenhoProjetil = tenhoProjetil;
  this->aindaExisto = aindaExisto;
}

std::vector<float>  Projetil::get_velocidade(){
  return this->velocidade;
}

std::vector<float>  Projetil::get_posicao(){
  return this->posicao;
}

bool Projetil::get_tenhoProjetil(){
  return this->tenhoProjetil;
}

bool Projetil::get_aindaExisto(){
  return this->aindaExisto;
}

void Projetil::hard_copy(Projetil *old_projetil, Projetil *projetil) {
  old_projetil = new Projetil( projetil->get_velocidade(),
                               projetil->get_posicao(),
                               projetil->get_tenhoProjetil(),
                               projetil->get_aindaExisto()
                             );
}

void Projetil::status_projetil(bool tenhoProjetil, bool aindaExisto){
  this->tenhoProjetil = tenhoProjetil;
  this->aindaExisto = aindaExisto;
}

Fisica::Fisica(ListaDeCorpos *ldc, ListaDeBots *ldb, Projetil *prj) {
  this->lista = ldc;
  this->listaBots = ldb;
  this->projetil = prj;
}

void Fisica::update_bot(float deltaT, Projetil *projetil){
  int j, aux = 0;
  std::vector<Bot *> *bot = this->listaBots->get_bots();
  std::vector<Corpo *> *corpo = this->lista->get_corpos();
  bool Atualizei = false;
  for (int i = 0; i < (*bot).size(); i++) {

    std::vector< float > new_vel = (*bot)[i]->get_velocidade();
    std::vector< float > new_pos = (*bot)[i]->get_posicao();

    Atualizei = false;
    do {

      // Caso posicao Vertical seja mais distante que a horizontal
      if((abs(int((*corpo)[0]->get_posicao()[0]) - int((*bot)[i]->get_posicao()[0])) > \
         abs(int((*corpo)[0]->get_posicao()[1]) - int((*bot)[i]->get_posicao()[1]))) || ((*bot)[i]->get_permissao(2) > 0 && (*bot)[i]->get_permissao(3) > 0))  {

          // Pac man esta em BAIXO da TELA
          if (((int)(*corpo)[0]->get_posicao()[0] - (int)(*bot)[i]->get_posicao()[0] > 0 && (*bot)[i]->get_permissao(1) == 0) || (((*bot)[i]->get_permissao(0) > 0 && (*bot)[i]->get_permissao(2) > 0) && (*bot)[i]->get_permissao(3) > 0)) {

            new_pos[0] = (*bot)[i]->get_posicao()[0] + (float)deltaT * Vel_V*1.5 /1000;

            if(projetil->get_aindaExisto() == true){
              if((int)projetil->get_posicao()[0] == (int)new_pos[0] && (int)projetil->get_posicao()[1] == (int)new_pos[1]){
                this->listaBots->kill_bot(i);
                break;
              }
            }
            
            // tentativa de avancar... Caso esteja livre, Gloria a Deus..
            if (mapa[(int) new_pos[1]][(int) new_pos[0]] == ' '){

              Atualizei = true;
              (*bot)[i]->update_permissao();
              (*bot)[i]->update(new_vel, new_pos);

              // Libero a possibilidade de movimentos lateriais
              
              break; // Proximo bot
            } else{
              
              // Nao posso ir para baixo
              (*bot)[i]->set_permissao(1, 3);
              continue; // Retorna nova tentativa

            }

          } //PAC MAN ESTA EM CIMA
          else if((*bot)[i]->get_permissao(0) == 0 || (((*bot)[i]->get_permissao(1) > 0 && (*bot)[i]->get_permissao(2) > 0) && (*bot)[i]->get_permissao(3) > 0)){

            new_pos[0] = (*bot)[i]->get_posicao()[0] - (float)deltaT * Vel_V*1.5 /1000;

            if(projetil->get_aindaExisto() == true){
              if((int)projetil->get_posicao()[0] == (int)new_pos[0] && (int)projetil->get_posicao()[1] == (int)new_pos[1]){
                this->listaBots->kill_bot(i);
                break;
              }
            }
            
            if (mapa[(int) new_pos[1]][(int) new_pos[0]] == ' '){

              Atualizei = true;
              (*bot)[i]->update_permissao();
              (*bot)[i]->update(new_vel, new_pos);

              // Libero a possibilidade de movimentos lateriais
              
              break; // Proximo bot
            } else{

              // Nao posso ir para cima
              (*bot)[i]->set_permissao(0, 3);
              continue;              
            }

          }
          else{
            Atualizei = true;
          }

        } else if((int)(*corpo)[0]->get_posicao()[0] == (int)(*bot)[i]->get_posicao()[0] and (int)(*corpo)[0]->get_posicao()[1] == (int)(*bot)[i]->get_posicao()[1]) {
          Atualizei = true;
          JogadorPerdeu = true;
        }
        else{  //Caso a distancia  Vertical > Horizontal

        // Pac man esta em uma posicao 'maior'
          if ((int((*corpo)[0]->get_posicao()[1]) - int((*bot)[i]->get_posicao()[1]) > 0 && (*bot)[i]->get_permissao(2) == 0) || (((*bot)[i]->get_permissao(0) > 0 && (*bot)[i]->get_permissao(1) > 0) && (*bot)[i]->get_permissao(3) > 0)){

            new_pos[1] = (*bot)[i]->get_posicao()[1] + (float)deltaT * Vel_H*1.5 /1000;

            if(projetil->get_aindaExisto() == true){
              if((int)projetil->get_posicao()[0] == (int)new_pos[0] && (int)projetil->get_posicao()[1] == (int)new_pos[1]){
                this->listaBots->kill_bot(i);
                break;
              }
            }

            if (mapa[(int) new_pos[1]][(int) new_pos[0]] == ' '){

              Atualizei =true;
              (*bot)[i]->update_permissao();
              (*bot)[i]->update(new_vel, new_pos);
              // Libero a possibilidade de movimentos Verticais
              break; // Proximo bot
            }
            else{
              // Nao posso ir para cima
              (*bot)[i]->set_permissao(2, 3);
              continue;              
            }

          } else if((*bot)[i]->get_permissao(3) == 0 || (((*bot)[i]->get_permissao(0) > 0 && (*bot)[i]->get_permissao(2) > 0) && (*bot)[i]->get_permissao(1) > 0) ){

            new_pos[1] = (*bot)[i]->get_posicao()[1] - (float)deltaT * Vel_H*1.5 /1000;

            if(projetil->get_aindaExisto() == true){
              if((int)projetil->get_posicao()[0] == (int)new_pos[0] && (int)projetil->get_posicao()[1] == (int)new_pos[1]){
                this->listaBots->kill_bot(i);
                break;
              }
            }
            
            if (mapa[(int) new_pos[1]][(int) new_pos[0]] == ' '){
              Atualizei = true;
              (*bot)[i]->update_permissao();
              (*bot)[i]->update(new_vel, new_pos);

              // Libero a possibilidade de movimentos Verticais
              
              break; // Proximo bot
            }
            else{
              // Nao posso ir para cima
              (*bot)[i]->set_permissao(3, 3);
              continue;              
            }

          }

          else{
            Atualizei = true;
          }

        }
    } while(!Atualizei);
  }
}

void Fisica::update_corpo(float deltaT, int sentido, int direcao) {
  // Atualiza parametros dos corpos!
  std::vector<Corpo *> *c = this->lista->get_corpos();
  for (int i = 0; i < (*c).size(); i++) {

    std::vector< float > new_vel = (*c)[i]->get_velocidade();
    std::vector< float > new_pos = (*c)[i]->get_posicao();
    
    new_vel[0] = Vel_H*sentido;
    new_vel[1] = Vel_V*sentido;

    new_pos[direcao] = (*c)[i]->get_posicao()[direcao] + (float)deltaT * new_vel[direcao]/1000;  
    if (mapa[(int) new_pos[1]][(int) new_pos[0]] == ' '){
      (*c)[i]->update(new_vel, new_pos);
    }
    if(mapa[(int) new_pos[1]][(int) new_pos[0]] == '$'){
      (*c)[i]->addMoeda();
      if ((*c)[i]->getMoeda() == MaxMoedas){
        JogadorGanhou = true;
      }

      printStr(23,6,"Moedas Coletadas: ");

      for (int num = 0; num < (*c)[i]->getMoeda(); num++)
        echochar('$');

      mapa[(int) new_pos[1]][(int) new_pos[0]] = ' ';
      (*c)[i]->update(new_vel, new_pos);
    }
    // if(new_pos[1] == && new_pos[0]] == )

  }
}

void Fisica::update_projetil(float deltaT, int sentido, int direcao) {

  // Atualiza parametros do projetil!
  std::vector<float> new_vel = {0,0};
  std::vector<float> new_pos = this->projetil->get_posicao();
       
  new_vel[0] = Vel_H*sentido;
  new_vel[1] = Vel_V*sentido;

  new_pos[direcao] = this->projetil->get_posicao()[direcao] + (float)deltaT * new_vel[direcao]/1000; 

  if (mapa[(int) new_pos[1]][(int) new_pos[0]] == ' '){
    this->projetil->update_projetil(new_vel, new_pos);
  }else{
    projetil->status_projetil(true, false);
  }
}

void Projetil::update_projetil(std::vector< float > nova_velocidade, std::vector< float > nova_posicao){
  this->velocidade = nova_velocidade;
  this->posicao = nova_posicao;
}


Tela::Tela(ListaDeCorpos *ldc, ListaDeBots *ldb, Projetil *projetil, int maxI, int maxJ, float maxX, float maxY) {
  this->lista = ldc;
  this->listaBots = ldb;
  this->lista_anterior = new ListaDeCorpos();
  this->lista_anterior->hard_copy(this->lista);
  this->listaBots_anterior = new ListaDeBots();
  this->listaBots_anterior->hard_copy(this->listaBots);
  this->maxI = maxI;
  this->maxJ = maxJ;
  this->maxX = maxX;
  this->maxY = maxY;
  this->projetil = projetil;
  this->projetil_old = new Projetil({0,0},{10,10},true,false); 
  this->projetil_old->hard_copy(this->projetil_old, this->projetil);
}

void Tela::init() {
  initscr();             /* Start curses mode     */
  raw();                 /* Line buffering disabled */
  curs_set(0);           /* Do not display cursor */
}

void Tela::update(Projetil *projetil, uint64_t deltaT) {
  int x,y;

  std::vector<Corpo *> *corpos_old = this->lista_anterior->get_corpos();
  std::vector<Bot *> *bots_old = this->listaBots_anterior->get_bots();
  this->projetil = projetil;

  x = (int) (projetil_old->get_posicao()[1]) * \
      (this->maxJ / this->maxY);
  y = (int) (projetil_old->get_posicao()[0]) * \
      (this->maxI / this->maxX);
  move(x, y);   /* Move cursor to position */
  echochar(' ');  /* Prints character, advances a position */

  if(projetil->get_tenhoProjetil() == true && projetil->get_aindaExisto() == true){
    x = (int) (projetil->get_posicao()[1]) * \
        (this->maxJ / this->maxY);
    y = (int) (projetil->get_posicao()[0]) * \
        (this->maxI / this->maxX);    
    move(x, y);   /* Move cursor to position */
    echochar('*');  /* Prints character, advances a position */
    projetil_old->update_projetil(  
    projetil->get_velocidade(),\
    projetil->get_posicao());
  }

  // Apaga corpos na tela
  for (int k=0; k<corpos_old->size(); k++)
  {
    x = (int) ((*corpos_old)[k]->get_posicao()[1]) * \
        (this->maxJ / this->maxY);
    y = (int) ((*corpos_old)[k]->get_posicao()[0]) * \
        (this->maxI / this->maxX);
    move(x, y);   /* Move cursor to position */
    echochar(' ');  /* Prints character, advances a position */
  }

  //Apaga bots da tela
  for (int k=0; k<bots_old->size(); k++)
  {
    x = (int) ((*bots_old)[k]->get_posicao()[1]) * \
        (this->maxJ / this->maxY);
    y = (int) ((*bots_old)[k]->get_posicao()[0]) * \
        (this->maxI / this->maxX);
    move(x, y);   /* Move cursor to position */
    echochar(' ');  /* Prints character, advances a position */
  }

  // Desenha corpos na tela
  std::vector<Corpo *> *corpos = this->lista->get_corpos();
  std::vector<Bot *> *bots = this->listaBots->get_bots();

  for (int k=0; k<corpos->size(); k++)
  {
    x = (int) ((*corpos)[k]->get_posicao()[1]) * \
        (this->maxJ / this->maxY);
    y = (int) ((*corpos)[k]->get_posicao()[0]) * \
        (this->maxI / this->maxX);
    
     // Resolve bug de printar fora da tela
    if (x < LINES and y < COLS and x > -1 and y > -1){
      move(x, y);   /* Move cursor to position */
      if(mapa[x][y] == ' '){
        echochar('O');  /* Prints character, advances a position */  
      }
    }

    // Atualiza corpos antigos
    (*corpos_old)[k]->update(  
                                (*corpos)[k]->get_velocidade(),\
                                (*corpos)[k]->get_posicao());
  }

  for (int k=0; k<bots->size(); k++)
  {
    x = (int) ((*bots)[k]->get_posicao()[1]) * \
        (this->maxJ / this->maxY);
    y = (int) ((*bots)[k]->get_posicao()[0]) * \
        (this->maxI / this->maxX);
    
     // Resolve bug de printar fora da tela
    if (x < LINES and y < COLS and x > -1 and y > -1){
      move(x, y);   /* Move cursor to position */
      echochar('@');  /* Prints character, advances a position */
    }

    // Atualiza corpos antigos
    (*bots_old)[k]->update((*bots)[k]->get_velocidade(),(*bots)[k]->get_posicao());
  }

  std::string TempoRestante;
  int min = deltaT / 60000;
  int sec = (deltaT - min*60000) / 1000;
  TempoRestante = "0" + std::to_string(min) + ":";
  if (sec > 9)
    TempoRestante = TempoRestante + std::to_string(sec);
  else
    TempoRestante = TempoRestante + "0" + std::to_string(sec);

  if (min < 0 and (*corpos)[0]->getMoeda() == MaxMoedas )
  {
    JogadorGanhou = true;
  }else if (min < 0){
    JogadorPerdeu = true;
  }else{
    printStr(2,73, "Tempo");
    printStr(3,72, "Restante");
    printStr(4,73,TempoRestante);
   
  }

   // Atualiza tela
  refresh();
}

void Tela::stop() {
  endwin();
}

void Tela::geraMapa(){

  for (int row = 1; row < 22; row++){
    for(int col = 1; col<71; col++){
      move(row,col);
      echochar(mapa[row][col]);
    }
  }
  refresh();

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