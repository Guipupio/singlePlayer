#include <thread>
#include <string>
#include <fstream>

extern bool JogadorPerdeu;
extern bool JogadorGanhou; 

void printStr(int row, int col,std::string frase);

class Corpo {
  private:
  
  std::vector<float> velocidade ={0,0};
  std::vector<float> posicao = {0,0};
  int moedas = 0;

  public:
  Corpo(std::vector< float > velocidade_x, std::vector< float > posicao);
  void update(std::vector<float> nova_velocidade,std::vector<float> nova_posicao);
  std::vector<float> get_velocidade();
  std::vector<float> get_posicao();
  void addMoeda();
  int getMoeda();
};


class Bot {
  private:  
  std::vector<float> velocidade ={0,0};
  std::vector<float> posicao = {0,0}; 
  std::vector<int> permissao = {0,0,0,0}; //cima, baixo, direita, esquerda
  //std::vector<bool> direcaoOk = {true,true,true,true};  //baixo, direita, esquerda, cima


  public:
  Bot(std::vector< float > posicao);
  void update(std::vector<float> nova_velocidade,std::vector<float> nova_posicao);
  std::vector<float> get_velocidade();
  std::vector<float> get_posicao();
  // void setDirecao(int posicao);
  // bool getDirecao(int posicao);
  // void clearDirecao(int posicao);
  void update_permissao();
  void set_permissao(int orientacao, int movimentos);
  int get_permissao(int orientacao);
};

class Projetil {
  private:
  std::vector<float> velocidade ={0,0};
  std::vector<float> posicao = {0,0};
  bool tenhoProjetil;
  bool aindaExisto;
  
  public:
  Projetil(std::vector< float > velocidade, std::vector< float > posicao, bool tenhoProjetil, bool aindaExisto);
  void update_projetil(std::vector< float > nova_velocidade, std::vector< float > nova_posicao);
  void status_projetil(bool tenhoProjetil, bool aindaExisto);
  std::vector<float>  get_velocidade();
  std::vector<float>  get_posicao();
  void hard_copy(Projetil * old_projetil, Projetil *projetil);
  bool get_tenhoProjetil();
  bool get_aindaExisto();
};

class ListaDeCorpos {
 private:
    std::vector<Corpo*> *corpos;

  public:
    ListaDeCorpos();
    void hard_copy(ListaDeCorpos *ldc);
    void add_corpo(Corpo *c);
    std::vector<Corpo*> *get_corpos();
};

class ListaDeBots {
 private:
    std::vector<Bot*> *bots;

  public:
    ListaDeBots();
    void hard_copy(ListaDeBots *ldc);
    void add_bot(Bot *bot);
    void kill_bot(int posicao);
    std::vector<Bot*> *get_bots();
};

class Fisica {
  private:
    ListaDeCorpos *lista;
    ListaDeBots *listaBots;
    Projetil  *projetil;

  public:
    Fisica(ListaDeCorpos *ldc, ListaDeBots *listaBots, Projetil *prj);
    void update_corpo(float deltaT, int sentido, int direcao);
    void update_projetil(float deltaT, int sentido, int direcao);
    void update_bot(float deltaT, Projetil *projetil);
};

class Tela {
  private:
    ListaDeCorpos *lista, *lista_anterior;
    ListaDeBots *listaBots, *listaBots_anterior;
    Projetil *projetil, *projetil_old;
    int maxI, maxJ;
    float maxX, maxY;

  public:
    Tela(ListaDeCorpos *ldc, ListaDeBots *ldb, Projetil *projetil_old, int maxI, int maxJ, float maxX, float maxY);
    ~Tela();
    void geraMapa();
    void stop();
    void init();
    void update(Projetil *projetil, uint64_t deltaT);
};

void threadfun (char *keybuffer, int *control);

class Teclado {
  private:
    char ultima_captura;
    int rodando;

    std::thread kb_thread;

  public:
    Teclado();
    ~Teclado();
    void stop();
    void init();
    char getchar();
};