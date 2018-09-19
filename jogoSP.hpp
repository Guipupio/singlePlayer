#include <thread>

class Corpo {
  private:
  
  std::vector<float> velocidade ={0,0};
  std::vector<float> posicao = {0,0};
  bool souPrincipal;


  public:
  Corpo(std::vector< float > velocidade_x, std::vector< float > posicao);
  void update(std::vector<float> nova_velocidade,std::vector<float> nova_posicao);
  std::vector<float> get_velocidade();
  std::vector<float> get_posicao();
  bool get_souPrincipal();
};

class Projetil {
  private:
  std::vector<float> velocidade ={0,0};
  std::vector<float> posicao = {0,0};
  
  public:
  Projetil(std::vector<float> velocidade, std::vector<float> posicao);
  void update(std::vector< float > nova_velocidade, std::vector< float > nova_posicao);
  std::vector< float >  get_velocidade();
  std::vector< float >  get_posicao();
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

class Fisica {
  private:
    ListaDeCorpos *lista;

  public:
    Fisica(ListaDeCorpos *ldc);
    void add_corpo(Corpo *c);
    void update(float deltaT, int sentido, int direcao);
};

class Tela {
  private:
    ListaDeCorpos *lista, *lista_anterior;
    int maxI, maxJ;
    float maxX, maxY;

  public:
    Tela(ListaDeCorpos *ldc, int maxI, int maxJ, float maxX, float maxY);
    ~Tela();
    void stop();
    void init();
    void update();
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