#include <nds.h>
#include <stdlib.h>
#include <vector>

// For 32 bit systems, add this line:
// https://github.com/devkitPro/buildscripts/issues/26
extern "C" void __sync_synchronize() {}

#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

struct Player
{
  int x;
  int y;

  ///Speed in x direction
  int dx;

  ///Speed in y direction
  int dy;

  void move()
  {
    x += dx + SCREEN_WIDTH;
    y += dy + SCREEN_HEIGHT;
    x %= SCREEN_WIDTH;
    y %= SCREEN_HEIGHT;
  }
};

void clear_screen();
void draw_players(std::vector<Player> &players, const int n_players);
void erase_players(std::vector<Player> &players, const int n_players);
void initial_players(std::vector<Player>& players, const int n_players);
void respond_to_keys(std::vector<Player> &players, int last_key);
void set_player(Player& player, int x_speed, int y_speed, int x_start, int y_start);

void move(std::vector<Player>& players)
{
  for (auto& p: players)
  {
    p.move();
  }
}


int main()
{
  const int n_players = 2;
  std::vector<Player> players(n_players);

  videoSetMode(MODE_FB0);
  vramSetBankA(VRAM_A_LCD);

  //Initialize music
  mmInitDefaultMem((mm_addr)soundbank_bin);

  //Load music
  mmLoad(MOD_MY_MUSIC);

  //Start music
  mmStart(MOD_MY_MUSIC, MM_PLAY_LOOP);


  clear_screen();
  initial_players(players, n_players);

  while(1)
  {
    scanKeys();
    respond_to_keys(players, keysHeld());
    swiWaitForVBlank();
    //erase_players(players, n_players);
    move(players);
    draw_players(players, n_players);
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
    players[i].x = (1 + (2 * i)) * SCREEN_WIDTH / 4;
    players[i].y = SCREEN_HEIGHT / 2;
    players[i].dx = 1 - (i * 2);
    players[i].dy = 0;
  }
}

void erase_players(std::vector<Player>& players, const int n_players)
{
  for(int i = 0; i < n_players; i++)
  {
    VRAM_A[players[i].x + players[i].y * SCREEN_WIDTH] = RGB15(0,0,0);
  }
}

void respond_to_keys(std::vector<Player>& players, int last_keys)
{
  if(KEY_RIGHT & last_keys)
  {
    players[0].dx = 1;
    players[0].dy = 0;
    //set_player(players[0], players[0].speed, 0, 0, rand() % SCREEN_HEIGHT);
  }
  else if(KEY_LEFT & last_keys)
  {
    players[0].dx = -1;
    players[0].dy = 0;
    //set_player(players[0], -players[0].speed, 0, SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
  }
  else if(KEY_UP & last_keys)
  {
    players[0].dx = 0;
    players[0].dy = -1;
    //set_player(players[0], 0, -players[0].speed, rand() % SCREEN_WIDTH, SCREEN_HEIGHT);
  }
  else if(KEY_DOWN & last_keys)
  {
    players[0].dx = 0;
    players[0].dy = 1;
    //set_player(players[0], 0, players[0].speed, rand() % SCREEN_WIDTH, 0);
  }
  else if(KEY_A & last_keys) // Right button
  {
    players[1].dx = 1;
    players[1].dy = 0;
    //set_player(players[1], players[1].speed, 0, 0, rand() % SCREEN_HEIGHT);
  }
  else if(KEY_Y & last_keys) // Left button
  {
    players[1].dx = -1;
    players[1].dy = 0;
    //set_player(players[1], -players[1].speed, 0, SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
  }
  else if(KEY_X & last_keys) // Top button
  {
    players[1].dx = 0;
    players[1].dy = -1;
    //set_player(players[1], 0, -players[1].speed, rand() % SCREEN_WIDTH, SCREEN_HEIGHT);
  }
  else if(KEY_B & last_keys) // Bottom button
  {
    players[1].dx = 0;
    players[1].dy = 1;
    //set_player(players[1], 0, players[1].speed, rand() % SCREEN_WIDTH, 0);
  }
  /*
  else
  {
    set_player(players[0], players[0].speed, 0, 0, rand() % SCREEN_HEIGHT);
    set_player(players[1], players[1].speed, 0, 0, rand() % SCREEN_HEIGHT);
  }
  */
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
