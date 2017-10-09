#include <nds.h>
#include <stdlib.h>
#include <vector>

struct Player
{
  int x;
  int y;
  int speed;
};

void clear_screen();
void draw_players(std::vector<Player> &players, const int n_players);
void erase_players(std::vector<Player> &players, const int n_players);
void initial_players(std::vector<Player>& players, const int n_players);
void move_players(std::vector<Player> &players, int last_key);
void set_player(Player& player, int x_speed, int y_speed, int x_start, int y_start);

int main()
{
  const int n_players = 2;
  std::vector<Player> players(n_players);
  int last_key = 0;

  irqInit();
  irqEnable(IRQ_VBLANK);

  videoSetMode(MODE_FB0);
  vramSetBankA(VRAM_A_LCD);

  clear_screen();
  initial_players(players, n_players);

  while(1)
  {
    scanKeys();
    if(keysDown())
    {
      last_key = keysHeld();
    }
    swiWaitForVBlank();

    erase_players(players, n_players);
    draw_players(players, n_players);
    move_players(players, last_key);
  }
  return 0;
}

void clear_screen()
{
  for(int i = 0; i < 256 * 192; i++)
  {
    VRAM_A[i] = RGB15(0,0,0);
  }
}

void draw_players(std::vector<Player>& players, const int n_players)
{
  for(int i = 0; i < n_players; i++)
  {
    VRAM_A[players[i].x + players[i].y * SCREEN_WIDTH] = RGB15(255, 0, 255);
  }
}

void initial_players(std::vector<Player>& players, const int n_players)
{
  for(int i = 0; i < n_players; i++)
  {
    players[i].x = rand() % 256;
    players[i].y = rand() % 192;
    players[i].speed = 1;
  }
}

void erase_players(std::vector<Player>& players, const int n_players)
{
  for(int i = 0; i < n_players; i++)
  {
    VRAM_A[players[i].x + players[i].y * SCREEN_WIDTH] = RGB15(0,0,0);
  }
}

void move_players(std::vector<Player>& players, int last_key)
{
  if(KEY_RIGHT & last_key)
  {
    set_player(players[0], players[0].speed, 0, 0, rand() % SCREEN_HEIGHT);
  }
  else if(KEY_LEFT & last_key)
  {
    set_player(players[0], -players[0].speed, 0, SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
  }
  else if(KEY_UP & last_key)
  {
    set_player(players[0], 0, -players[0].speed, rand() % SCREEN_WIDTH, SCREEN_HEIGHT);
  }
  else if(KEY_DOWN & last_key)
  {
    set_player(players[0], 0, players[0].speed, rand() % SCREEN_WIDTH, 0);
  }
  else if(KEY_A & last_key)
  {
    set_player(players[1], players[1].speed, 0, 0, rand() % SCREEN_HEIGHT);
  }
  else if(KEY_Y & last_key)
  {
    set_player(players[1], -players[1].speed, 0, SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
  }
  else if(KEY_X & last_key)
  {
    set_player(players[1], 0, -players[1].speed, rand() % SCREEN_WIDTH, SCREEN_HEIGHT);
  }
  else if(KEY_B & last_key)
  {
    set_player(players[1], 0, players[1].speed, rand() % SCREEN_WIDTH, 0);
  }
  else
  {
    set_player(players[0], players[0].speed, 0, 0, rand() % SCREEN_HEIGHT);
    set_player(players[1], players[1].speed, 0, 0, rand() % SCREEN_HEIGHT);
  }
}

void set_player(Player& player, int x_speed, int y_speed, int x_start, int y_start)
{
  player.x += x_speed;
  player.y += y_speed;

  if(player.y < 0 || player.y > SCREEN_HEIGHT)
  {
    player.x = x_start;
    player.y = y_start;
  }
  else if(player.x < 0 || player.x > SCREEN_WIDTH)
  {
    player.x = x_start;
    player.y = y_start;
  }
}
