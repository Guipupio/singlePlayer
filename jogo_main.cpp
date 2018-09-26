#include "01-playback.hpp"
#include <ncurses.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>


#include "jogoSP.hpp"

#define GameTimeMs 80000 

 bool JogadorPerdeu;
 bool JogadorGanhou; 


using namespace std::chrono;
uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main ()
{

  uint64_t t0;
  uint64_t t1;
  uint64_t deltaT;
  uint64_t T;
  uint64_t tPause = 0;
  uint64_t tPeteleco;

  JogadorPerdeu = false;
  JogadorGanhou = false;

  std::string text;
  int i = 0;
  int sentido = 0, direcao = 0;
  int sentido_projetil = 0, direcao_projetil = 0;
  int num_projeteis = 3;

  bool jogoRodando = false;

  Audio::Sample *samplePeteleco;
  samplePeteleco = new Audio::Sample();
  samplePeteleco->load("assets/fire.dat");

  Audio::Sample *sampleDerrota;
  sampleDerrota = new Audio::Sample();
  sampleDerrota->load("assets/derrota.dat");

  Audio::Sample *sampleVitoria;
  sampleVitoria = new Audio::Sample();
  sampleVitoria->load("assets/Vitoria.dat");

  Audio::Sample *sampleFundo;
  sampleFundo = new Audio::Sample();
  sampleFundo->load("assets/fundo.dat");

  Audio::Player *player;
  player = new Audio::Player();
  player->init();

  // Espera
  while (1) {
    std::this_thread::sleep_for (std::chrono::milliseconds(1));
    t1 = get_now_ms();
    if (t1-t0 > 6500) break;
  }

  Corpo *pacMan = new Corpo({0,0},{20,11});
  Bot *bot1 = new Bot({2,2});
  Bot *bot2 = new Bot({65,2});
  Bot *bot3 = new Bot({65,20});
  Bot *bot4 = new Bot({2,20});

  int corpoSelecionado = 0;

  ListaDeCorpos *l = new ListaDeCorpos();
  ListaDeBots *b = new ListaDeBots();
  l->add_corpo(pacMan);
  b->add_bot(bot1);
  b->add_bot(bot2);
  b->add_bot(bot3);
  b->add_bot(bot4);

  Projetil *projetil = new Projetil({0,0},{20,11},true,false);

  Fisica *f = new Fisica(l,b,projetil);

  Tela *tela = new Tela(l, b, projetil, 40, 40, 40, 40);

  tela->init();

  std::ifstream file("telaInit.txt");
    std::string str;
    int intCount = 3; 
    while (std::getline(file, str))
    {
        printStr(intCount,15,str);
        intCount++;
    }

  Teclado *teclado = new Teclado();
  teclado->init();

  uint64_t lensampleFundo = static_cast<float>(sampleFundo->get_data().size());
  uint64_t lensamplePeteleco = static_cast<float>(samplePeteleco->get_data().size());

  for(int i=5;i>0;i--){
    std::ifstream file(std::to_string(i)+".txt");
    std::string str;
    int intCount = 8; 
    while (std::getline(file, str))
    {
        printStr(intCount,30,str);
        intCount++;
    }
    std::this_thread::sleep_for (std::chrono::milliseconds(1000));
    std::ifstream file2("clean.txt");
    intCount = 8; 
    while (std::getline(file2, str))
    {
        printStr(intCount,30,str);
        intCount++;
    }    
  }

  std::ifstream file3("start.txt");
  intCount = 8; 
  while (std::getline(file3, str))
  {
      printStr(intCount,22,str);
      intCount++;
  }
  std::this_thread::sleep_for (std::chrono::milliseconds(1000));

  tela->geraMapa();
  printStr(22,6,"Projeteis restantes: " + std::to_string(num_projeteis));
  printStr(23,6,"Moedas Coletadas: " + std::to_string(pacMan->getMoeda()));
  jogoRodando = true;
      
  player->play(sampleFundo);


  T = get_now_ms();
  t1 = T;
  while (!JogadorPerdeu and !JogadorGanhou) {
    // Atualiza timers
    char c = teclado->getchar();
      t0 = t1;
      t1 = get_now_ms();
      deltaT = t1-t0;

    // if(jogoRodando){
      // Atualiza modelo
      f->update_corpo(deltaT,sentido,direcao);

      if(projetil->get_aindaExisto() == true)
        f->update_projetil(deltaT,sentido_projetil,direcao_projetil);

      f->update_bot(deltaT, projetil);
      
      // Atualiza tela
      tela->update(projetil, GameTimeMs + T- t1);
     

     // Lê o teclado
      
      if (c=='s' or c=='S' or c ==31) {
        sentido = +1;
        direcao = 1;
      }
      if (c=='w' or c=='w' or c == 30) {
        sentido = -1;
        direcao = 1;
      }
      if (c=='a' or c=='A' or c == 17){
        sentido = -1;
        direcao = 0;
      }
      if (c=='d' or c=='D' or c == 16){
        sentido = +1;
        direcao = 0;
      }
      if (c== 0x20 && num_projeteis > 0){  //Verifica se a barra de espaço foi apertada para lançar o projetil
        if(projetil->get_aindaExisto() == false && projetil->get_tenhoProjetil() == true){
          tPeteleco = get_now_ms();
          player->pause();
          player->play(samplePeteleco);
          projetil->status_projetil(true, true);
          sentido_projetil = -sentido;
          direcao_projetil = +direcao;
          projetil->update_projetil(pacMan->get_velocidade(), pacMan->get_posicao());
          num_projeteis--;         
          printStr(22,6,"Projeteis restantes: " + std::to_string(num_projeteis));
        }
      }
    // }
    if (c=='q' or c=='Q') {
      break;
    }
    
    // if (c=='p' or c=='P') {    
    //     if (jogoRodando)
    //     {
    //       //tPause = 
    //       printStr(22,32,"Jogo Em Pausa");
    //     }else{
    //       printStr(22,32,"             ");
    //     }    
    //     jogoRodando = !jogoRodando;

    // }


    // Condicao de rebobinar
    if (sampleFundo->get_position() + 100 > lensampleFundo ) sampleFundo->set_position(0);
      if ( get_now_ms() - tPeteleco > 500 ){
        samplePeteleco->set_position(0);
         player->play(sampleFundo);
      }
      std::this_thread::sleep_for (std::chrono::milliseconds(100));
      i++;
  }

    if (JogadorPerdeu)
    {
      player->pause();
      sampleDerrota->set_position(0);
      player->play(sampleDerrota);
      std::ifstream file("telaDerrota.txt");
      std::string str;
      int intCount = 9; 
      while (std::getline(file, str))
      {
          printStr(intCount,11,str);
          intCount++;
      }

      

      std::this_thread::sleep_for (std::chrono::milliseconds(5000));
      // while(teclado->getchar() != 11 or teclado->getchar() != 0x13);
    
    }

    if (JogadorGanhou)
    {
      player->pause();
      sampleDerrota->set_position(0);
      player->play(sampleVitoria);
      std::ifstream file("telaVitoria.txt");
      std::string str;
      int intCount = 9; 
      while (std::getline(file, str))
      {
          printStr(intCount,11,str);
          intCount++;
      }

      std::this_thread::sleep_for (std::chrono::milliseconds(5000));
      // while(teclado->getchar() != 11 or teclado->getchar() != 13);
    }

  player->stop();
  tela->stop();
  teclado->stop();
  return 0;
}
