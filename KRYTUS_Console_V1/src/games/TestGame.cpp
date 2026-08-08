#include "TestGame.h"
#include "../config.h"

void TestGame::init() {
  playerY = SCREEN_HEIGHT / 2;
  playerX = 20;
  score = 0;
  gameOver = false;
  lastSpawn = millis();
  lastMove = millis();
  for (int i = 0; i < MAX_OBSTACLES; i++) obstacles[i].active = false;
}

void TestGame::update(InputManager& input) {
  if (gameOver) {
    if (input.a()) init(); // retry
    return;
  }

  unsigned long now = millis();

  if (input.upHeld())   playerY -= 3;
  if (input.downHeld()) playerY += 3;

  if (playerY < 0) playerY = 0;
  if (playerY > SCREEN_HEIGHT - PLAYER_SIZE) playerY = SCREEN_HEIGHT - PLAYER_SIZE;

  // Move obstacles
  if (now - lastMove > 40) {
    lastMove = now;
    for (int i = 0; i < MAX_OBSTACLES; i++) {
      if (!obstacles[i].active) continue;
      obstacles[i].x -= 4;
      if (obstacles[i].x < -PLAYER_SIZE) {
        obstacles[i].active = false;
        score++;
      }
    }
  }

  // Spawn obstacles
  if (now - lastSpawn > 900) {
    lastSpawn = now;
    for (int i = 0; i < MAX_OBSTACLES; i++) {
      if (!obstacles[i].active) {
        obstacles[i].active = true;
        obstacles[i].x = SCREEN_WIDTH;
        obstacles[i].y = random(0, SCREEN_HEIGHT - PLAYER_SIZE);
        break;
      }
    }
  }

  // Collision demonstration (simple AABB overlap test)
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (!obstacles[i].active) continue;
    bool overlapX = (playerX < obstacles[i].x + PLAYER_SIZE) && (playerX + PLAYER_SIZE > obstacles[i].x);
    bool overlapY = (playerY < obstacles[i].y + PLAYER_SIZE) && (playerY + PLAYER_SIZE > obstacles[i].y);
    if (overlapX && overlapY) {
      gameOver = true;
      break;
    }
  }
}

void TestGame::render(DisplayManager& display) {
  display.clear(TFT_BLACK);

  if (gameOver) {
    display.centerText("GAME OVER", 60, TFT_RED, 1);
    char buf[24];
    snprintf(buf, sizeof(buf), "SCORE: %lu", (unsigned long)score);
    display.centerText(buf, 80, TFT_WHITE, 1);
    display.centerText("A:RETRY  B:MENU", 100, TFT_YELLOW, 1);
    return;
  }

  display.rect(playerX, playerY, PLAYER_SIZE, PLAYER_SIZE, TFT_GREEN, true);

  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (obstacles[i].active) {
      display.rect(obstacles[i].x, obstacles[i].y, PLAYER_SIZE, PLAYER_SIZE, TFT_RED, true);
    }
  }

  char buf[16];
  snprintf(buf, sizeof(buf), "SCORE:%lu", (unsigned long)score);
  display.text(buf, 2, 2, TFT_WHITE, 1);
}

void TestGame::shutdown() {
  // No dynamic resources held by this game -- nothing to release.
}
