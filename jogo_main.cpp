#include "01-playback.hpp"

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include "jogoSP.hpp"

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

  uint64_t tPeteleco;

  Audio::Sample *samplePeteleco;
  samplePeteleco = new Audio::Sample();
  samplePeteleco->load("assets/peteleco.dat");

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
    if (t1-t0 > 3500) break;
  }

  Corpo *c1 = new Corpo({0,0},{10,10});
  // Corpo *c2 = new Corpo(10.0, 0.0, 0.0, 19.0, 5.0, 2.0);
  // Corpo *c3 = new Corpo(10.0, 0.0, 0.0, 17.0, 15.0, 4.0);
  // Corpo *c4 = new Corpo(10.0, 0.0, 0.0, 15.0, 20.0, 8.0);

  int corpoSelecionado = 0;

  ListaDeCorpos *l = new ListaDeCorpos();
  l->add_corpo(c1);
  // l->add_corpo(c2);
  // l->add_corpo(c3);
  // l->add_corpo(c4);

  Fisica *f = new Fisica(l);

  Tela *tela = new Tela(l, 20, 20, 20, 20);
  tela->init();

  Teclado *teclado = new Teclado();
  teclado->init();

  uint64_t lenSampleFundo = static_cast<float>(sampleFundo->get_data().size());
  //uint64_t lenSamplePeteleco = static_cast<float>(samplePeteleco->get_data().size());


  int i = 0;
  int sentido = 0;
  int direcao = 0;

  T = get_now_ms();
  t1 = T;
  player->play(sampleFundo);
  while (1) {
    // Atualiza timers
    t0 = t1;
    t1 = get_now_ms();
    deltaT = t1-t0;

    // Atualiza modelo
    f->update(deltaT,sentido,direcao);

    // Atualiza tela
    tela->update();

     // Lê o teclado
    char c = teclado->getchar();
    if (c=='s') {
      tPeteleco = get_now_ms();
      player->pause();
      player->play(samplePeteleco);
      sentido = +1;
      direcao = 1;
      f->update(deltaT,sentido,direcao);
    }
    if (c=='w') {
      tPeteleco = get_now_ms();
      player->pause();
      player->play(samplePeteleco);
      sentido = -1;
      direcao = 1;
      f->update(deltaT,sentido,direcao);
    }
    if (c=='a'){
      if (corpoSelecionado - 1 < 0)
        corpoSelecionado = l->get_corpos()->size() -1;
      else
        corpoSelecionado--;
    }
    if (c=='d'){
      if (corpoSelecionado + 1 == l->get_corpos()->size())
        corpoSelecionado = 0;
      else
        corpoSelecionado++;
    }
    if (c=='q') {
      break;
    }

    // Condicao de rebobinar
    if ( sampleFundo->get_position() + 100 > lenSampleFundo ) sampleFundo->set_position(0);
    if ( get_now_ms() - tPeteleco > 500 ){
      samplePeteleco->set_position(0);
      player->play(sampleFundo);
    }
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
    i++;
  }

  player->stop();
  tela->stop();
  teclado->stop();
  return 0;
}
