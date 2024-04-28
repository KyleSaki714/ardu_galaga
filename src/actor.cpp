#include "actor.h"

Actor::Actor(int x, int y, int width, int height) : Rectangle(x, y , width, height)
{
  _centerw = width / 2;
  // _centerw = width % 2 == 1 ? width / 2 : width / 2 + 1;
  _drawSprite = true;
  _drawBoundingBox = false;
  _hidden = false;
  _drawCenterW = false;
}

void Actor::setSprite(const unsigned char* byteArray) {
  _sprite = byteArray;
}

void Actor::setDrawSprite(bool drawSprite) {
  _drawSprite = drawSprite;
}

void Actor::setDrawCenterW(bool drawCenterw) {
  _drawCenterW = drawCenterw;
}

void Actor::draw( Adafruit_SSD1306& display) {
  int centerWidthOffset = _drawCenterW ? _centerw : 0;

  if (_drawBoundingBox) {
    display.drawRect(_x - centerWidthOffset, _y, _width, _height, SSD1306_WHITE);
  }
  if (!_hidden && _drawSprite) {
    display.drawBitmap(_x - centerWidthOffset, _y, _sprite, _width, _height, SSD1306_WHITE);
  }
}

bool Actor::isHidden() {
  return _hidden;
}

void Actor::hide() {
  _hidden = true;
  setLocation(-OFFSCREEN_COORDS, -OFFSCREEN_COORDS);
}

void Actor::show() {
  _hidden = false;
}

bool Actor::wasJustHit() {
  return _justHit;
}

/**
 * This Actor was hit.
*/
void Actor::hit() {
  _justHit = true;
}

/**
 * This actor is not dead anymore
*/
void Actor::recover() {
  _justHit = false;
}

String Actor::getName() const {
  return "Actor";
}

Laser::Laser(int x, int y, int width, int height) : Actor(x, y , width, height)
{
}

String Laser::getName() const {
  return "Laser";
}

Explosion::Explosion(int x, int y, int width, int height) : Actor(x, y , width, height)
{
}

String Explosion::getName() const {
  return "Explosion";
}

Ship::Ship(int x, int y, int width, int height) : Actor(x, y , width, height)
{
}

String Ship::getName() const {
  return "Ship";
}

Enemy::Enemy(int x, int y, int width, int height) : Actor(x, y , width, height)
{
  _startPosx = x;
  _startPosy = y;
  _justHit = false;
  _isDiving = false;
}

int Enemy::getStartPositionX() {
  return _startPosx;
}

int Enemy::getStartPositionY() {
  return _startPosy;
}

bool Enemy::isDiving() {
  return _isDiving;
}

void Enemy::setDive(bool isDiving) {
  _isDiving = isDiving;
}

String Enemy::getName() const {
  return "Enemy";
}

Bee::Bee(int x, int y, int width, int height) : Enemy(x, y , width, height)
{
}

String Bee::getName() const {
  return "Bee";
}

BossGalaga::BossGalaga(int x, int y, int width, int height) : Enemy(x, y , width, height)
{
}

String BossGalaga::getName() const {
  return "BossGalaga";
}
