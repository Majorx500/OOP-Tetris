/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "../include/raylib.h"
#include <array>
#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <utility>

int levelTimes[29] = {30,28,26,25,24,21,19,17,16,14,10,10,7,7,7,5,5,5,5,4,4,4,4,4,2,2,2,2,1};
int linesLevelUp[29] = {10,20,30,40,50,60,70,80,90,100,100,100,100,100,100,100,110,120,130,140,150,160,170,180,190,200,200,200,200};
Color colors[] = {BLACK,WHITE,BLUE,RED,GREEN,GOLD,DARKPURPLE,DARKBLUE,DARKGREEN,DARKGRAY,ORANGE};
short colorsTable[10][4] = {
  {0,1,2,3},
  {0,1,4,3},
  {0,1,5,2},
  {0,1,10,7},
  {0,1,6,8},
  {0,1,9,3},
  {0,1,4,7},
  {0,1,3,10},
  {0,1,7,5},
  {0,1,6,2}
};


#define SCREEN_BOX_WIDTH 10
#define SCREEN_BOX_HEIGHT 20

enum BlockType{
  L_BLOCK = 0,
  J_BLOCK = 1,
  I_BLOCK = 2,
  O_BLOCK = 3,
  S_BLOCK = 4,
  T_BLOCK = 5,
  Z_BLOCK = 6
};

class Point{
  int X;
  int Y;

  public:
   Point(int x, int y) : X(x), Y(y){};
   int getX(){
     return X;
   }
   int getY(){
     return Y;
   }
   void goDown(){
     Y++;
   }
   void moveHorizontal(bool right){
     if(right) X++;
     else X--;
   }
   void setPoint(int x, int y){
     X = x;
     Y = y;
   }
};

class Score{
  private:
  long currentScore;
  long topScore;

  public:
  
  Score() : currentScore(0), topScore(0){

  }

  int getCurrentScore(){
    return currentScore;
  }

  int increaseScore(short lineCount, short level) {
    int scoreIncrease;
    level++;
    switch (lineCount) {
        case 1:
          scoreIncrease = (40 * level);
          break;  
        case 2:
          scoreIncrease = (10 * level);
          break;
        case 3:
          scoreIncrease = (300 * level);
          break;
        case 4:
          scoreIncrease = (1200 * level);
          break;
        default:
          return -1;
    }
    currentScore += scoreIncrease;
    return scoreIncrease;
  }
  long getTopScore(){
    return topScore;
  }
  void resetScore(){
    topScore = topScore < currentScore ? currentScore : topScore;
    currentScore = 0;
  }
};

class Block{
  protected:
   short _currentRotation; // 0 - N , 1 - E , 2 - S , 3 - W
   short _rotationCount; 
   BlockType _type;
   Point _point;
   short _color;
   short _aroundColor;
  public:

   Block(short rotationCount,BlockType type,Point point) : _currentRotation(0), _rotationCount(rotationCount), _type(type), _point(point){
     _color = GetRandomValue(2,3);
     _aroundColor = GetRandomValue(0,1);
   };
   void rotateClockwise(){
     _currentRotation++;
     _currentRotation = _currentRotation % _rotationCount;
   }
   void rotateCounterClockwise(){
     _currentRotation--;
     if(_currentRotation < 0){
       _currentRotation = _rotationCount - 1;
     }
   }
   short getCurrentRotation(){
     return _currentRotation;
   }
   short getRotationCount(){
     return _rotationCount;
   }
   BlockType getBlockType(){
     return _type;
   }
   Point getPoint(){
     return _point;
   }
   void setPoint(int x, int y){
     _point.setPoint(x, y);
   }
    
   short getColor(){
     return _color;
   }
    

   virtual void rotateBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen,bool clockwise) = 0;
   virtual bool canRotate(std::array<std::array<int, SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH> screen, bool clockwise) = 0;
   virtual bool canMoveDown(std::array<std::array<int, SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH> screen) = 0;
   virtual bool canMoveLeft(std::array<std::array<int, SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH> screen) = 0;
   virtual bool canMoveRight(std::array<std::array<int, SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH> screen) = 0;
   virtual void placeBlock(std::array<std::array<int,SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH>& screen) = 0;
   virtual void moveDown(std::array<std::array<int,SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH>& screen) = 0;
  
   virtual void moveLeft(std::array<std::array<int,SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH>& screen) = 0;
   virtual void moveRight(std::array<std::array<int,SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH>& screen) = 0;
   virtual bool canPlaceBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH>& screen) = 0;
};

class JBlock : public Block{
  
  public:
   JBlock() : Block(4, J_BLOCK, Point(5,1)) {};
  
  void rotateBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen,bool clockwise) override {
    int x = _point.getX(),y = _point.getY();
    if(clockwise){
      rotateClockwise();
      switch (_currentRotation) {
        case(0):{
          screen[x-1][y] = 0;
          screen[x-1][y-1]=0;
          screen[x+1][y] = 0;
          screen[x][y-1] = _color;
          screen[x][y+1] = _color;
          screen[x-1][y+1]= _color;
          break;
        }
        case(1):{
          screen[x-1][y] =  _color;
          screen[x+1][y] =  _color;
          screen[x+1][y+1] = _color;
          screen[x][y-1] = 0;
          screen[x][y+1] = 0;
          screen[x-1][y+1] = 0;

          break;
        }
        case(2):{
          screen[x-1][y] = 0;
          screen[x+1][y] = 0;
          screen[x+1][y+1] = 0;
          screen[x+1][y-1] = _color;
          screen[x][y-1] = _color ;
          screen[x][y+1] =  _color;
          break;
        }
        case(3):{
          screen[x+1][y-1] = 0;
          screen[x][y-1] = 0;
          screen[x][y+1] = 0;
          screen[x-1][y] =  _color;
          screen[x-1][y-1] =  _color;
          screen[x+1][y] =  _color;
          break;
        }
      }
    }
    else{
      rotateCounterClockwise();
      switch (_currentRotation) {
        case(0):{
          screen[x-1][y] = 0;
          screen[x+1][y+1]=0;
          screen[x+1][y] = 0;
          screen[x][y-1] = _color;
          screen[x][y+1] =  _color;
          screen[x-1][y+1] =  _color;
          break;
        }
        case(1):{
          screen[x-1][y] =  _color;
          screen[x+1][y] =  _color;
          screen[x+1][y+1] =  _color;
          screen[x][y-1] = 0;
          screen[x][y+1] = 0;
          screen[x+1][y-1] = 0;

          break;
        }
        case(2):{
          screen[x-1][y] = 0;
          screen[x+1][y] = 0;
          screen[x-1][y-1] = 0;
          screen[x+1][y-1] = _color;
          screen[x][y-1] = _color;
          screen[x][y+1] = _color;
          break;
        }
        case(3):{
          screen[x-1][y+1] = 0;
          screen[x][y-1] = 0;
          screen[x][y+1] = 0;
          screen[x-1][y] = _color;
          screen[x-1][y-1] = _color;
          screen[x+1][y] = _color;
          break;
        }
      }

    }

  }
  bool canRotate(std::array<std::array<int, SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH> screen, bool clockwise) override{
    short currentRotation = _currentRotation;
    int x = _point.getX(),y = _point.getY();
    if(clockwise)
      switch (currentRotation) {
        case(0):{
          if(x == 0 || x == SCREEN_BOX_WIDTH - 1) return false;
          bool res = screen[x-1][y] | screen[x+1][y] | screen[x+1][y+1];
          return !res;
        }
        case(1):{
          bool res = screen[x+1][y-1] || screen[x][y-1] || screen[x][y+1];
          return !res;
        }
        case(2):{
          if(x == 0 || x == SCREEN_BOX_WIDTH - 1) return false;
          bool res = screen[x-1][y] || screen[x-1][y-1] || screen[x+1][y];
          return !res;
        }
        case(3):{
          bool res = screen[x][y-1] || screen[x][y+1] || screen[x-1][y+1];
          return !res;
        }
      }
    else{
      switch (currentRotation) {
        case(0):{
          if(x == 0 || x == SCREEN_BOX_WIDTH - 1) return false;
          bool res = screen[x-1][y] || screen[x-1][y-1] || screen[x+1][y];
          return !res;
        }
        case(1):{
          bool res = screen[x][y-1] || screen[x][y+1] || screen[x-1][y+1];
          return !res;
        }
        case(2):{
          if(x == 0 || x == SCREEN_BOX_WIDTH - 1) return false;
          bool res = screen[x-1][y] | screen[x+1][y] | screen[x+1][y+1];
          return !res;
        }
        case(3):{
          if(y == SCREEN_BOX_HEIGHT-1) return false;
          bool res = screen[x+1][y-1] || screen[x][y-1] || screen[x][y+1];
          return !res;
        }
      }
    }
    return false;
  }

  bool canMoveDown(std::array<std::array<int, SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH> screen) override{
    int x = _point.getX(),y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        if(y == SCREEN_BOX_HEIGHT-2) return false;
        bool res = screen[x][y+2] || screen[x-1][y+2];
        return !res;
      }
      case(1):{
        if(y == SCREEN_BOX_HEIGHT-2) return false;
        bool res = screen[x-1][y+1] || screen[x][y+1] || screen[x+1][y+2];
        return !res;
      }
      case(2):{
        if(y == SCREEN_BOX_HEIGHT-2) return false;
        bool res = screen[x+1][y] || screen[x][y+2];
        return !res;
      }
      case(3):{
       if(y == SCREEN_BOX_HEIGHT - 1) return false;         
       bool res = screen[x-1][y+1] || screen[x][y+1] || screen[x+1][y+1];
       return !res;
      }
    }
    return false;
  }
  
  bool canMoveLeft(std::array<std::array<int, SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH>screen) override{
    int x = _point.getX(),y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        if(x == 1) return false;
        bool res = screen[x-1][y-1] || screen[x-1][y] || screen[x-2][y+1];
        return !res;
      }
      case(1):{
        if(x == 1) return false;
        bool res = screen[x][y+1] || screen[x-2][y];
        return !res;
      }
      case(2):{
        if(x == 0) return false;
        bool res = screen[x-1][y-1] || screen[x-1][y] || screen[x-1][y+1];
        return !res;
      }
      case(3):{
        if(x == 1) return false;
        bool res = screen[x-2][y] || screen[x-2][y-1];
        return !res;
      }
    }
    return false;
  }
  
  bool canMoveRight(std::array<std::array<int, SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH> screen) override{
    int x = _point.getX(),y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        if(x == SCREEN_BOX_WIDTH-1) return false;
        bool res = screen[x+1][y-1] || screen[x+1][y] || screen[x+1][y+1];
        return !res;
      }
      case(1):{
        if(x == SCREEN_BOX_WIDTH-2) return false;
        bool res = screen[x+2][y+1] || screen[x+2][y];
        return !res;
      }
      case(2):{
        if(x == SCREEN_BOX_WIDTH-2) return false;
        bool res = screen[x+2][y-1] || screen[x+1][y] || screen[x+1][y+1];
        return !res;
      }
      case(3):{
        if(x == SCREEN_BOX_WIDTH-2) return false;
        bool res = screen[x+2][y] || screen[x][y-1];
        return !res;
      }
    }
    return false;
  }

  void moveDown(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
    int x = _point.getX(), y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        screen[x][y-1] = 0;
        screen[x-1][y+1] = 0;
        screen[x-1][y+2] =_color;
        screen[x][y+2] =_color;
      break;
      }
      case(1):{
        screen[x-1][y] = 0;
        screen[x-1][y+1] =_color;
        screen[x][y] = 0;
        screen[x][y+1] =_color;
        screen[x+1][y] = 0;
        screen[x+1][y+2]=_color;
      break;
      }
      case(2):{
        screen[x][y-1]=0;
        screen[x+1][y-1] = 0;
        screen[x+1][y-1] = 0;
        screen[x][y+2] =_color;
        screen[x+1][y] =_color;
      break;
      }
      case(3):{
        screen[x-1][y-1] = 0;
        screen[x-1][y+1] =_color;
        screen[x][y] = 0;
        screen[x][y+1] =_color;
        screen[x+1][y] = 0;
        screen[x+1][y+1] =_color;
      break;
      }
    }
    _point.goDown();
  }
  
  void placeBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
    int x = _point.getX(), y = _point.getY();
    screen[x][y]=_color;
    screen[x][y-1]=_color;
    screen[x][y+1]=_color;
    screen[x-1][y+1] =_color;
  }
  bool canPlaceBlock(std::array<std::array<int, 20>, 10> &screen) override{
    int x = _point.getX(), y = _point.getY();
    if(screen[x][y] ==1 || screen[x][y-1]!= 0 ||  screen[x][y+1]== 1 || screen[x-1][y+1] == 1)return false;
    return true;
  }

  void moveLeft(std::array<std::array<int, 20>, 10> &screen) override{
    int x = _point.getX(), y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        screen[x][y-1] = 0;
        screen[x-1][y-1] =_color;
        screen[x][y] = 0;
        screen[x-1][y] =_color;
        screen[x][y+1] = 0;
        screen[x-2][y+1] =_color;
        break;
      }
      case(1):{
        screen[x+1][y+1] = 0;
        screen[x+1][y] = 0;
        screen[x][y+1] =_color;
        screen[x-2][y] =_color;
        break;
      }
      case(2):{
        screen[x+1][y-1]=0;
        screen[x-1][y-1] =_color;
        screen[x][y] = 0;
        screen[x-1][y] =_color;
        screen[x][y+1] = 0;
        screen[x-1][y+1] =_color;
        break;
      }
      case(3):{
        screen[x-1][y-1] = 0;
        screen[x-2][y-1] =_color;
        screen[x+1][y] = 0;
        screen[x-2][y] =_color;
        break;
      }
    }
    _point.moveHorizontal(false);
  }

  void moveRight(std::array<std::array<int, 20>, 10> &screen) override{
    int x = _point.getX(), y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        screen[x][y-1] = 0;
        screen[x+1][y-1] =_color;
        screen[x][y] = 0;
        screen[x+1][y] =_color;
        screen[x-1][y+1] = 0;
        screen[x+1][y+1] =_color;
        break;
      }
      case(1):{
        screen[x+1][y+1] = 0;
        screen[x-1][y] = 0;
        screen[x+2][y+1] =_color;
        screen[x+2][y] =_color;
        break;
      }
      case(2):{
        screen[x][y-1] = 0;
        screen[x+2][y-1] =_color;
        screen[x][y] = 0;
        screen[x+1][y] =_color;
        screen[x][y+1] = 0;
        screen[x+1][y+1] =_color;
        break;
      }
      case(3):{
        screen[x-1][y-1] = 0;
        screen[x][y-1] =_color;
        screen[x-1][y] = 0;
        screen[x+2][y] =_color;
        break;
      }
    }
    _point.moveHorizontal(true);
  }

};

class LBlock : public Block{

  public:

  LBlock() : Block(4, L_BLOCK, Point(5,1)) {};
  void rotateBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen,bool clockwise) override {
    int x = _point.getX(),y = _point.getY();
    if(clockwise){
      rotateClockwise();
      switch (_currentRotation) {
        case(0):{
          screen[x-1][y] = 0;
          screen[x-1][y+1]=0;
          screen[x+1][y] = 0;
          screen[x][y-1] =_color;
          screen[x][y+1] =_color;
          screen[x+1][y+1] =_color;
          break;
        }
        case(1):{
          screen[x-1][y] =_color;
          screen[x+1][y] =_color;
          screen[x+1][y-1] =_color;
          screen[x][y-1] = 0;
          screen[x][y+1] = 0;
          screen[x+1][y+1] = 0;

          break;
        }
        case(2):{
          screen[x-1][y] = 0;
          screen[x+1][y] = 0;
          screen[x+1][y-1] = 0;
          screen[x-1][y-1] =_color;
          screen[x][y-1] =_color;
          screen[x][y+1] =_color;
          break;
        }
        case(3):{
          screen[x-1][y-1] = 0;
          screen[x][y-1] = 0;
          screen[x][y+1] = 0;
          screen[x-1][y] =_color;
          screen[x-1][y+1] =_color;
          screen[x+1][y] =_color;
          break;
        }
      }
    }
    else{
      rotateCounterClockwise();
      switch (_currentRotation) {
        case(0):{
          screen[x-1][y] = 0;
          screen[x+1][y-1]=0;
          screen[x+1][y] = 0;
          screen[x][y-1] =_color;
          screen[x][y+1] =_color;
          screen[x+1][y+1] =_color;
          break;
        }
        case(1):{
          screen[x-1][y] =_color;
          screen[x+1][y] =_color;
          screen[x+1][y-1] =_color;
          screen[x][y-1] = 0;
          screen[x][y+1] = 0;
          screen[x-1][y-1] = 0;

          break;
        }
        case(2):{
          screen[x-1][y] = 0;
          screen[x+1][y] = 0;
          screen[x-1][y+1] = 0;
          screen[x-1][y-1] =_color;
          screen[x][y-1] =_color;
          screen[x][y+1] =_color;
          break;
        }
        case(3):{
          screen[x+1][y+1] = 0;
          screen[x][y-1] = 0;
          screen[x][y+1] = 0;
          screen[x-1][y] =_color;
          screen[x-1][y+1] =_color;
          screen[x+1][y] =_color;
          break;
        }
      }

    }
  }
  bool canRotate(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen, bool clockwise) override{
    short currentRotation = _currentRotation;
    int x = _point.getX(),y = _point.getY();
    if(clockwise)
      switch (currentRotation) {
        case(0):{
          if(x == 0 || x == SCREEN_BOX_WIDTH - 1) return false;
          bool res = screen[x-1][y] | screen[x+1][y] | screen[x+1][y-1];
          return !res;
        }
        case(1):{
          bool res = screen[x-1][y-1] || screen[x][y-1] || screen[x][y+1];
          return !res;
        }
        case(2):{
          if(x == 0 || x == SCREEN_BOX_WIDTH - 1) return false;
          bool res = screen[x-1][y] || screen[x-1][y+1] || screen[x+1][y];
          return !res;
        }
        case(3):{
          bool res = screen[x][y-1] || screen[x][y+1] || screen[x+1][y+1];
          return !res;
        }
      }
    else{
      switch (currentRotation) {
        case(0):{
          if(x == 0 || x == SCREEN_BOX_WIDTH - 1) return false;
          bool res = screen[x-1][y] || screen[x-1][y+1] || screen[x+1][y];
          return !res;
        }
        case(1):{
          bool res = screen[x][y-1] || screen[x][y+1] || screen[x+1][y+1];
          return !res;
        }
        case(2):{
          if(x == 0 || x == SCREEN_BOX_WIDTH - 1) return false;
          bool res = screen[x-1][y] | screen[x+1][y] | screen[x+1][y-1];
          return !res;
        }
        case(3):{
          bool res = screen[x-1][y-1] || screen[x][y-1] || screen[x][y+1];
          return !res;
        }
      }
    }
    return false;
  }

  bool canMoveDown(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen) override{
    int x = _point.getX(),y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        if(y == SCREEN_BOX_HEIGHT-2) {
          return false;
        }
        bool res = screen[x][y+2] || screen[x+1][y+2];
        return !res;
      }
      case(1):{
        if(y == SCREEN_BOX_HEIGHT-1) return false;
        bool res = screen[x-1][y+1] || screen[x][y+1] || screen[x+1][y+1];
        return !res;
      }
      case(2):{
        if(y == SCREEN_BOX_HEIGHT-2) return false;
        bool res = screen[x-1][y] || screen[x][y+2];
        return !res;
      }
      case(3):{
       if(y == SCREEN_BOX_HEIGHT - 2) return false;         
       bool res = screen[x-1][y+2] || screen[x][y+1] || screen[x+1][y+1];
       return !res;
      }
    }
    return false;
  }
  
  bool canMoveLeft(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen) override{
    int x = _point.getX(),y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        if(x == 0) return false;
        bool res = screen[x-1][y-1] || screen[x-1][y] || screen[x-1][y+1];
        return !res;
      }
      case(1):{
        if(x == 1) return false;
        bool res = screen[x][y-1] || screen[x-2][y];
        return !res;
      }
      case(2):{
        if(x == 1) return false;
        bool res = screen[x-2][y-1] || screen[x-1][y] || screen[x-1][y+1];
        return !res;
      }
      case(3):{
        if(x == 1) return false;
        bool res = screen[x-2][y] || screen[x-2][y+1];
        return !res;
      }
    }
    return false;
  }
  
  bool canMoveRight(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen) override{
    int x = _point.getX(),y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        if(x == SCREEN_BOX_WIDTH-2) return false;
        bool res = screen[x+1][y-1] || screen[x+1][y] || screen[x+2][y+1];
        return !res;
      }
      case(1):{
        if(x == SCREEN_BOX_WIDTH-2) return false;
        bool res = screen[x+2][y-1] || screen[x+2][y];
        return !res;
      }
      case(2):{
        if(x == SCREEN_BOX_WIDTH-1) return false;
        bool res = screen[x+1][y-1] || screen[x+1][y] || screen[x+1][y+1];
        return !res;
      }
      case(3):{
        if(x == SCREEN_BOX_WIDTH-2) return false;
        bool res = screen[x+2][y] || screen[x][y+1];
        return !res;
      }
    }
    return false;
  }
  void moveDown(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
    int x = _point.getX(), y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        screen[x][y-1] = 0;
        screen[x+1][y+1] = 0;
        screen[x+1][y+2] =_color;
        screen[x][y+2] =_color;
      break;
      }
      case(1):{
        screen[x-1][y] = 0;
        screen[x-1][y+1] =_color;
        screen[x][y] = 0;
        screen[x][y+1] =_color;
        screen[x+1][y-1] = 0;
        screen[x+1][y+1]=_color;
        break;
      }
      case(2):{
        screen[x][y-1]=0;
        screen[x-1][y-1] = 0;
        screen[x-1][y-1] = 0;
        screen[x][y+2] =_color;
        screen[x-1][y] =_color;
      break;
      }
      case(3):{
        screen[x-1][y] = 0;
        screen[x-1][y+2] =_color;
        screen[x][y] = 0;
        screen[x][y+1] =_color;
        screen[x+1][y] = 0;
        screen[x+1][y+1] =_color;
      break;
      }
    }
    _point.goDown();
  }
  

  void placeBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
    int x = _point.getX(), y = _point.getY();
    screen[x][y]=_color;
    screen[x][y-1]=_color;
    screen[x][y+1]=_color;
    screen[x+1][y+1] =_color;
  }
  bool canPlaceBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
    int x = _point.getX(), y = _point.getY();
    if(screen[x][y] != 0 || screen[x][y-1] == 1 || screen[x][y+1] == 1 || screen[x+1][y+1] == 1)
      return false;
    return true;
  }

  void moveLeft(std::array<std::array<int, 20>, 10> &screen) override{
    int x = _point.getX(), y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        screen[x][y-1] = 0;
        screen[x-1][y-1] =_color;
        screen[x][y] = 0;
        screen[x-1][y] =_color;
        screen[x+1][y+1] = 0;
        screen[x-1][y+1] =_color;
        break;
      }
      case(1):{
        screen[x+1][y-1] = 0;
        screen[x+1][y] = 0;
        screen[x][y-1] =_color;
        screen[x-2][y] =_color;
        break;
      }
      case(2):{
        screen[x][y-1]=0;
        screen[x-2][y-1] =_color;
        screen[x][y] = 0;
        screen[x-1][y] =_color;
        screen[x][y+1] = 0;
        screen[x-1][y+1] =_color;
        break;
      }
      case(3):{
        screen[x-1][y+1] = 0;
        screen[x-2][y+1] =_color;
        screen[x+1][y] = 0;
        screen[x-2][y] =_color;
        break;
      }
    }
    _point.moveHorizontal(false);
  }

  void moveRight(std::array<std::array<int, 20>, 10> &screen) override{
    int x = _point.getX(), y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        screen[x][y-1] = 0;
        screen[x+1][y-1] =_color;
        screen[x][y] = 0;
        screen[x+1][y] =_color;
        screen[x][y+1] = 0;
        screen[x+2][y+1] =_color;
        break;
      }
      case(1):{
        screen[x+1][y-1] = 0;
        screen[x-1][y] = 0;
        screen[x+2][y-1] =_color;
        screen[x+2][y] =_color;
        break;
      }
      case(2):{
        screen[x-1][y-1] = 0;
        screen[x+1][y-1] =_color;
        screen[x][y] = 0;
        screen[x+1][y] =_color;
        screen[x][y+1] = 0;
        screen[x+1][y+1] =_color;
        break;
      }
      case(3):{
        screen[x-1][y+1] = 0;
        screen[x][y+1] =_color;
        screen[x-1][y] = 0;
        screen[x+2][y] =_color;
        break;
      }
    }
    _point.moveHorizontal(true);
  }
};

class IBlock : public Block{

  public:
  IBlock() : Block(2, I_BLOCK, Point(5,1)) {};
  void rotateBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen,bool clockwise) override {
      int x = _point.getX(), y = _point.getY();
      rotateClockwise();
      switch (_currentRotation) {
        case(0):{
          screen[x][y-1] =_color;
          screen[x][y+1] =_color;
          screen[x][y+2] =_color;
          screen[x-1][y] = 0;
          screen[x+1][y] = 0;
          screen[x+2][y] = 0;
          break;
        }
        case(1):{
          screen[x][y-1] = 0;
          screen[x][y+1] = 0;
          screen[x][y+2] = 0;
          screen[x-1][y] =_color;
          screen[x+1][y] =_color;
          screen[x+2][y] =_color;
          break;
        }
      }
    }

  bool canRotate(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen, bool clockwise) override{
    short currentRotation = _currentRotation;
    int x = _point.getX(),y = _point.getY();
    switch (currentRotation) {
      case(0):{
        if(x == 0 || x == SCREEN_BOX_WIDTH - 2) return false;
        bool res = screen[x-1][y] || screen[x+1][y] || screen[x+2][y];
        return !res;
      }
      case(1):{
        if(y >= SCREEN_BOX_HEIGHT - 2 ) return false;
        bool res = screen[x][y-1] || screen[x][y+1] || screen[x][y+2];
        return !res;
      }
    }
    return false;
  }

  bool canMoveDown(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen) override{
    int x = _point.getX(),y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        if(y == SCREEN_BOX_HEIGHT -3) return false;
        bool res = screen[x][y+3];
        return !res;
      }
      case(1):{
        if(y == SCREEN_BOX_HEIGHT-1) return false;
        bool res = screen[x-1][y+1] || screen[x][y+1] || screen[x+1][y+1] || screen[x+2][y+1];
        return !res;
      }
    }
    return false;
  }
  
  bool canMoveLeft(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen) override{
    int x = _point.getX(),y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        if(x == 0) return false;
        bool res = screen[x-1][y-1] || screen[x-1][y] || screen[x-1][y+1] || screen[x-1][y+2];
        return !res;
      }
      case(1):{
        if(x == 1) return false;
        bool res = screen[x-2][y];
        return !res;
      }
    }
    return false;
  }
  
  bool canMoveRight(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen) override{
    int x = _point.getX(),y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        if(x == SCREEN_BOX_WIDTH-1) return false;
        bool res = screen[x+1][y-1] || screen[x+1][y] || screen[x+1][y+1] || screen[x+1][y+2];
        return !res;
      }
      case(1):{
        if(x == SCREEN_BOX_WIDTH-3) return false;
        bool res = screen[x+3][y];
        return !res;
      }
    }
    return false;
  }

    void moveDown(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
      int x = _point.getX(), y = _point.getY();
      switch (_currentRotation) {
        case(0):{
          screen[x][y-1] = 0;
          screen[x][y+3] =_color;
          break;
        }
        case(1):{
          screen[x-1][y] = 0;
          screen[x-1][y+1] =_color;
          screen[x][y] = 0;
          screen[x][y+1] =_color;
          screen[x+1][y] = 0;
          screen[x+1][y+1] =_color;
          screen[x+2][y] = 0;
          screen[x+2][y+1] =_color;
          break;
        }
      }
      _point.goDown();
    }

    void placeBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
      int x = _point.getX(), y = _point.getY();
      screen[x][y]=_color;
      screen[x][y-1]=_color;
      screen[x][y+1]=_color;
      screen[x][y+2] =_color;
    }
    bool canPlaceBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
      int x = _point.getX(), y = _point.getY();
      if(screen[x][y] != 0 || screen[x][y-1]== 1 || screen[x][y+1] == 1 || screen[x][y+2] == 1)
        return false;
      return true;
    }
    void moveLeft(std::array<std::array<int, 20>, 10> &screen) override{
    int x = _point.getX(), y = _point.getY();
    switch (_currentRotation) {
      case(0):{
       screen[x][y-1] = 0;
       screen[x-1][ y-1] =_color;
       screen[x][y] = 0;
       screen[x-1][y]=_color;
       screen[x][y+1] = 0;
       screen[x-1][y+1] =_color;
       screen[x][y+2] = 0;
       screen[x-1][y+2] =_color;
       break;
      }
      case(1):{
        screen[x+2][y] = 0;
        screen[x-2][y] =_color;
        break;
      }
    }
    _point.moveHorizontal(false);
  }

  void moveRight(std::array<std::array<int, 20>, 10> &screen) override{
    int x = _point.getX(), y = _point.getY();
    switch (_currentRotation) {
      case(0):{
       screen[x][y-1] = 0;
       screen[x+1][ y-1] =_color;
       screen[x][y] = 0;
       screen[x+1][y]=_color;
       screen[x][y+1] = 0;
       screen[x+1][y+1] =_color;
       screen[x][y+2] = 0;
       screen[x+1][y+2] =_color;
       break;
      }
      case(1):{
        screen[x-1][y] = 0;
        screen[x+3][y] =_color;
        break;
      }
    }
    _point.moveHorizontal(true);
  }

};
class OBlock : public Block{
  public:
  OBlock() : Block(1, O_BLOCK, Point(5,1)) {}; 
  void rotateBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen,bool clockwise) override {
    
  }

  bool canRotate(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen, bool clockwise) override{
    return true;
  }
  bool canMoveDown(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen) override{
    int x = _point.getX(), y = _point.getY();
    if(y == SCREEN_BOX_HEIGHT-1 || screen[x][y+1] || screen[x+1][y+1]) return false;
    return true;
  }
  bool canMoveLeft(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen) override{
    int x = _point.getX(), y = _point.getY();
    if(x == 0 || screen[x-1][y-1] || screen[x-1][y]) return false;
    return true;
  }

  bool canMoveRight(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen) override{
    int x = _point.getX(), y = _point.getY();
    if(x == SCREEN_BOX_WIDTH - 2 || screen[x+2][y-1] || screen[x+2][y]) return false;
    return true;
  }
  
  void moveDown(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
    int x = _point.getX(), y = _point.getY();
    screen[x][y-1] = 0;
    screen[x+1][y-1] = 0;
    screen[x][y+1] =_color;
    screen[x+1][y+1] =_color;
    _point.goDown();
  }

  void placeBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
    int x = _point.getX(), y = _point.getY();
    screen[x+1][y-1] =_color;
    screen[x+1][y] =_color;
    screen[x][y] =_color;
    screen[x][y-1] =_color;

  }
  bool canPlaceBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
    int x = _point.getX(), y = _point.getY();
    if(screen[x+1][y-1] != 0 || screen[x+1][y] == 1 || screen[x][y] == 1 || screen[x][y-1] == 1)
      return false;
    return true;

  }
  void moveLeft(std::array<std::array<int, 20>, 10> &screen) override{
    int x = _point.getX(), y = _point.getY();
    screen[x+1][y-1] = 0;
    screen[x-1][y-1] =_color;
    screen[x+1][y] = 0;
    screen[x-1][y] =_color;
    _point.moveHorizontal(false);
  }
  void moveRight(std::array<std::array<int, 20>, 10> &screen) override{
    int x = _point.getX(), y = _point.getY();
    screen[x][y-1] = 0;
    screen[x+2][y-1] =_color;
    screen[x][y] = 0;
    screen[x+2][y] =_color;

    _point.moveHorizontal(true);
  }
};
class SBlock : public Block{
  public:
    SBlock() : Block(2,S_BLOCK,Point(5,1)){};
       void rotateBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen,bool clockwise) override {
      int x = _point.getX(),y = _point.getY();
      rotateClockwise();
      switch (_currentRotation) {
        case(0):{
          screen[x][y-1] =_color;
          screen[x+1][y-1] =_color;
          screen[x-1][y-1] = 0;
          screen[x][y+1] = 0;
          break;
        }
        case(1):{
          screen[x][y-1] = 0;
          screen[x+1][y-1] = 0;
          screen[x-1][y-1] =_color;
          screen[x][y+1] =_color;
          break;
        }
      }
    }

    bool canRotate(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen, bool clockwise) override{
      short currentRotation = _currentRotation;
      int x = _point.getX(),y = _point.getY();
      switch (currentRotation) {
        case(0):{
          if(y == SCREEN_BOX_HEIGHT - 2) return false;
          bool res = screen[x-1][y-1] || screen[x][y+1];
          return !res;
        }
        case(1):{
          if(x == SCREEN_BOX_WIDTH - 1) return false;
          bool res = screen[x][y-1] || screen[x+1][y-1];
          return !res;
        }
      }
      return false;
    }

    bool canMoveDown(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(0):{
          if(y == SCREEN_BOX_HEIGHT-1) return false;
          bool res = screen[x-1][y+1] || screen[x][y+1] || screen[x+1][y];
          return !res;
        }
        case(1):{
          if(y == SCREEN_BOX_HEIGHT-2) return false;
          bool res = screen[x-1][y+1] || screen[x][y+2];
          return !res;
        }
      }
      return false;
    }
    
    bool canMoveLeft(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(0):{
          if(x == 1) return false;
          bool res = screen[x-2][y] || screen[x-1][y-1];
          return !res;
        }
        case(1):{
          if(x == 1) return false;
          bool res = screen[x-2][y-1] || screen[x-2][y] || screen[x-1][y+1];
          return !res;
        }
      }
      return false;
    }
    
    bool canMoveRight(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(0):{
          if(x == SCREEN_BOX_WIDTH-2) return false;
          bool res = screen[x+1][y] || screen[x+2][y-1];
          return !res;
        }
        case(1):{
          if(x == SCREEN_BOX_WIDTH-1) return false;
          bool res = screen[x][y-1] || screen[x+1][y] || screen[x+1][y+1];
          return !res;
        }
      }
      return false;
    }
    void moveDown(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
     int x = _point.getX(), y = _point.getY();
     switch (_currentRotation) {
       case(0):{
        screen[x-1][y]=0;
        screen[x-1][y+1] = _color;
        screen[x][y-1] = 0;
        screen[x+1][y-1]=0;
        screen[x][y+1] =_color;
        screen[x+1][y] =_color;
        break;
       }
       case(1):{
        screen[x-1][y-1] = 0;
        screen[x][y]=0;
        screen[x-1][y+1]=_color;
        screen[x][y+2]=_color;
        break;
       }
     }
     _point.goDown();
    }

    void placeBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
      int x = _point.getX(), y = _point.getY();
      screen[x][y] =_color;
      screen[x-1][y] =_color;
      screen[x][y-1] =_color;
      screen[x+1][y-1] =_color;  
    }
    bool canPlaceBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
      int x = _point.getX(), y = _point.getY();
      if(screen[x][y] != 0 || screen[x-1][y] == 1 || screen[x][y-1] == 1 || screen[x+1][y-1] == 1)
        return false;
      return true;
    }

    void moveLeft(std::array<std::array<int, 20>, 10> &screen) override{
      int x = _point.getX(), y = _point.getY();
      switch (_currentRotation) {
        case(0):{
          screen[x+1][y-1] = 0;
          screen[x-1][y-1] =_color;
          screen[x][y] = 0;
          screen[x-2][y] =_color;
          break;
        }
        case(1):{
          screen[x-1][y-1] = 0;
          screen[x-2][y-1] =_color;
          screen[x][y] = 0;
          screen[x-2][y] =_color;
          screen[x][y+1] = 0;
          screen[x-1][y+1] =_color;
          break;
        }
      }
      _point.moveHorizontal(false);
    }

    void moveRight(std::array<std::array<int, 20>, 10> &screen) override{
      int x = _point.getX(), y = _point.getY();
      switch (_currentRotation) {
        case(0):{
          screen[x][y-1] = 0;
          screen[x+2][y-1] =_color;
          screen[x-1][y] = 0;
          screen[x+1][y] =_color;
          break;
        }
        case(1):{
          screen[x-1][y-1] = 0;
          screen[x][y-1] =_color;
          screen[x-1][y] = 0;
          screen[x+1][y] =_color;
          screen[x][y+1] = 0;
          screen[x+1][y+1] =_color;
          break;
        }
      }
      _point.moveHorizontal(true);
    }

};
class TBlock : public Block{
  public:
   TBlock() : Block(4,T_BLOCK,Point(5,1)){};
   void rotateBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen,bool clockwise) override {
      int x = _point.getX(),y = _point.getY();
      if(clockwise){
        rotateClockwise();
        switch (_currentRotation) {
          case(0):{
            screen[x][y+1]=0;
            screen[x+1][y]=_color;
            break;
          }
          case(1):{
            screen[x-1][y] = 0;
            screen[x][y+1] =_color;
            break;
          }
          case(2):{
            screen[x][y-1]=0;
            screen[x-1][y]=_color;
            break;
          }
          case(3):{
            screen[x+1][y] = 0;
            screen[x][y-1] =_color;
            break;
          }
        
        }
      }
      else{
        rotateCounterClockwise();
        switch (_currentRotation) {
          case(0):{
            screen[x][y+1]=0;
            screen[x-1][y]=_color;
            break;
          }
          case(1):{
            screen[x-1][y] = 0;
            screen[x][y-1] =_color;
            break;
          }
          case(2):{
            screen[x][y-1]=0;
            screen[x+1][y]=_color;
            break;
          }
          case(3):{
            screen[x+1][y] = 0;
            screen[x][y+1] =_color;
            break;
          }

      }
    }
  }
    bool canRotate(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen, bool clockwise) override{
      short currentRotation = _currentRotation;
      int x = _point.getX(),y = _point.getY();
      if(clockwise)
        switch (currentRotation) {
          case(0):{
            if(x == 0 || y == SCREEN_BOX_HEIGHT-1) return false;
            bool res = screen[x][y+1];
            return !res;
          }
          case(1):{
            if(x == 0) return false;
            bool res = screen[x-1][y];
            return !res;
          }
          case(2):{
            bool res = screen[x][y-1];
            return !res;
          }
          case(3):{
            if(x == SCREEN_BOX_WIDTH - 1) return false;
            bool res = screen[x+1][y];
            return !res;
          }
        }
      else{
        switch (currentRotation) {
          case(0):{
                    
            bool res = screen[x][y+1];
            return !res;
          }
          case(1):{
            if(x == 0) return false;
            bool res = screen[x-1][y];
            return !res;
          }
          case(2):{
            if(y == SCREEN_BOX_HEIGHT - 1) return false;
            bool res = screen[x][y-1];
            return !res;
          }
          case(3):{
            if(x == SCREEN_BOX_WIDTH - 1) return false;
            bool res = screen[x+1][y];
            return !res;
          }
        }
      }
      return false;
    }

    bool canMoveDown(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(0):{
          if(y == SCREEN_BOX_HEIGHT-1) return false;
          bool res = screen[x-1][y+1] || screen[x][y+1] || screen[x+1][y+1];
          return !res;
        }
        case(1):{
          if(y == SCREEN_BOX_HEIGHT-2) return false;
          bool res = screen[x+1][y+1] || screen[x][y+2];
          return !res;
        }
        case(2):{
          if(y == SCREEN_BOX_HEIGHT-2) return false;
          bool res = screen[x-1][y+1] || screen[x][y+2] || screen[x+1][y+1];
          return !res;
        }
        case(3):{
         if(y == SCREEN_BOX_HEIGHT - 2) return false;         
         bool res = screen[x][y+2] || screen[x-1][y+1];
         return !res;
        }
      }
      return false;
    }
    
    bool canMoveLeft(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(0):{
          if(x ==1 ) return false;
          bool res = screen[x-1][y-1] || screen[x-2][y];
          return !res;
        }
        case(1):{
          if(x == 0) return false;
          bool res = screen[x-1][y-1] || screen[x-1][y] || screen[x-1][y+1];
          return !res;
        }
        case(2):{
          if(x == 1) return false;
          bool res = screen[x-2][y] || screen[x-1][y+1];
          return !res;
        }
        case(3):{
          if(x == 1) return false;
          bool res = screen[x-1][y-1] || screen[x-2][y] || screen[x-1][y+1];
          return !res;
        }
      }
      return false;
    }
    
    bool canMoveRight(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(0):{
          if(x == SCREEN_BOX_WIDTH-2) return false;
          bool res = screen[x+1][y-1] || screen[x+2][y];
          return !res;
        }
        case(1):{
          if(x == SCREEN_BOX_WIDTH-2) return false;
          bool res = screen[x+1][y-1] || screen[x+2][y] || screen[x+1][y+1];
          return !res;
        }
        case(2):{
          if(x == SCREEN_BOX_WIDTH-2) return false;
          bool res = screen[x+2][y] || screen[x+1][y+1];
          return !res;
        }
        case(3):{
          if(x == SCREEN_BOX_WIDTH-1) return false;
          bool res = screen[x+1][y-1] || screen[x+1][y] || screen[x+1][y+1];
          return !res;
        }
      }
      return false;
    }

    void moveDown(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(0):{
         screen[x-1][y] = 0;
         screen[x-1][y+1] =_color;
         screen[x][y-1] = 0;
         screen[x][y+1] =_color;
         screen[x+1][y] = 0;
         screen[x+1][y+1] =_color;
         break;
        }
        case(1):{
         screen[x+1][y] = 0;
         screen[x+1][y+1] =_color;
         screen[x][y-1] = 0;
         screen[x][y+2]=_color;
         break;
        }
        case(2):{
         screen[x-1][y] = 0;
         screen[x-1][y+1] =_color;
         screen[x][y]=0;
         screen[x][y+2] =_color;
         screen[x+1][y] = 0;
         screen[x+1][y+1] =_color;
         break;
        }
        case(3):{
         screen[x-1][y] = 0;
         screen[x-1][y+1] =_color;
         screen[x][y-1] = 0;
         screen[x][y+2]=_color;
         break;
        }
      }
      _point.goDown();
    }

    void placeBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
      int x = _point.getX(),y = _point.getY();
      screen[x][y] =_color;
      screen[x-1][y] =_color;
      screen[x][y-1] =_color;
      screen[x+1][y] =_color;
    }
    bool canPlaceBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
      int x = _point.getX(), y = _point.getY();
      if(screen[x][y] != 0 || screen[x-1][y] == 1 || screen[x][y-1] == 1 || screen[x+1][y] == 1)
        return false;
      return true;
    }
  void moveLeft(std::array<std::array<int, 20>, 10> &screen) override{
    int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(0):{
         screen[x][y-1] = 0;
         screen[x-1][y-1] =_color;
         screen[x+1][y] = 0;
         screen[x-2][y] =_color;
         break;
        }
        case(1):{
          screen[x][y-1] = 0;
          screen[x-1][y-1] =_color;
          screen[x+1][y] = 0;
          screen[x-1][y] =_color;
          screen[x][y+1] = 0;
          screen[x-1][y+1] =_color;
          break;
        }
        case(2):{
          screen[x+1][y] = 0;
          screen[x-2][y] =_color;
          screen[x][y+1] = 0;
          screen[x-1][y+1] =_color;
          break;
        }
        case(3):{
          screen[x][y-1] = 0;
          screen[x-1][y-1] =_color;
          screen[x][y] = 0;
          screen[x-2][y] =_color;
          screen[x][y+1] = 0;
          screen[x-1][y+1] =_color;
          break;
        }
      }
      _point.moveHorizontal(false);
    }
  void moveRight(std::array<std::array<int, 20>, 10> &screen) override{
    int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(0):{
         screen[x-1][y] = 0;
         screen[x+2][y] =_color;
         screen[x][y-1] = 0;
         screen[x+1][y-1] =_color;
         break;
        }
        case(1):{
          screen[x][y-1] = 0;
          screen[x+1][y-1] =_color;
          screen[x][y] = 0;
          screen[x+2][y] =_color;
          screen[x][y+1] = 0;
          screen[x+1][y+1] =_color;
          break;
        }
        case(2):{
          screen[x-1][y] = 0;
          screen[x+2][y] =_color;
          screen[x][y+1] = 0;
          screen[x+1][y+1] =_color;
          break;
        }
        case(3):{
          screen[x][y-1] = 0;
          screen[x+1][y-1] =_color;
          screen[x-1][y] = 0;
          screen[x+1][y] =_color;
          screen[x][y+1] = 0;
          screen[x+1][y+1] =_color;
          break;
        }
      }
      _point.moveHorizontal(true);
    }
};
class ZBlock : public Block{
  public:
    ZBlock() : Block(2,Z_BLOCK,Point(5,1)){};

   void rotateBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen,bool clockwise) override {
      int x = _point.getX(),y = _point.getY();
      rotateClockwise();
      switch (_currentRotation) {
        case(0):{
          screen[x][y-1] =_color;
          screen[x-1][y-1] =_color;
          screen[x+1][y-1] = 0;
          screen[x][y+1] = 0;
          break;
        }
        case(1):{
          screen[x][y-1] = 0;
          screen[x-1][y-1] = 0;
          screen[x+1][y-1] =_color;
          screen[x][y+1] =_color;
          break;
        }
      }
    }

    
    bool canRotate(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen, bool clockwise) override{
      short currentRotation = _currentRotation;
      int x = _point.getX(),y = _point.getY();
      switch (currentRotation) {
        case(0):{
          if(y == SCREEN_BOX_HEIGHT - 2) return false;
          bool res = screen[x][y+1] || screen[x+1][y-1];
          return !res;
        }
        case(1):{
          if(x == 0) return false;
          bool res = screen[x][y-1] || screen[x-1][y-1];
          return !res;
        }
      }
      return false;
    }

    bool canMoveDown(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(0):{
          if(y == SCREEN_BOX_HEIGHT-1) return false;
          bool res = screen[x-1][y] || screen[x][y+1] || screen[x+1][y+1];
          return !res;
        }
        case(1):{
          if(y == SCREEN_BOX_HEIGHT -2) return false;
          bool res = screen[x][y+2] || screen[x+1][y+1];
          return !res;
        }
      }
      return false;
    }
    
    bool canMoveLeft(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(0):{
          if(x == 1) return false;
          bool res = screen[x-1][y] || screen[x-2][y-1];
          return !res;
        }
        case(1):{
          if(x == 0) return false;
          bool res = screen[x][y-1] || screen[x-1][y] || screen[x-1][y+1];
          return !res;
        }
      }
      return false;
    }
    
    bool canMoveRight(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(0):{
          if(x == SCREEN_BOX_WIDTH-2) return false;
          bool res = screen[x+1][y-1] || screen[x+2][y];
          return !res;
        }
        case(1):{
          if(x == SCREEN_BOX_WIDTH-2) return false;
          bool res = screen[x+2][y-1] || screen[x+2][y] || screen[x+1][y+1];
          return !res;
        }
      }
      return false;
    }
    void moveDown(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
     int x = _point.getX(), y = _point.getY();
     switch (_currentRotation) {
       case(0):{
        screen[x-1][y]= _color;
        screen[x-1][y-1]=0;
        screen[x][y-1] = 0;
        screen[x+1][y]=0;
        screen[x][y+1] =_color;
        screen[x+1][y+1] =_color;
        break;
       }
       case(1):{
        screen[x+1][y-1] = 0;
        screen[x][y]=0;
        screen[x+1][y+1]= _color;
        screen[x][y+2]= _color;
        break;
       }
     }
     _point.goDown();
    }

    void placeBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
      int x = _point.getX(), y = _point.getY();
      screen[x][y] =_color;
      screen[x-1][y-1] =_color;
      screen[x][y-1] =_color;
      screen[x+1][y] =_color;  
    } 
    bool canPlaceBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
      int x = _point.getX(), y = _point.getY();
      if(screen[x][y] != 0 || screen[x-1][y-1] == 1 || screen[x][y-1] == 1 || screen[x+1][y] == 1)
        return false;
      return true;
    }

    void moveLeft(std::array<std::array<int, 20>, 10> &screen) override{
      int x = _point.getX(), y = _point.getY();
      switch (_currentRotation) {
        case(0):{
          screen[x][y-1] = 0;
          screen[x-2][y-1] =_color;
          screen[x+1][y] = 0;
          screen[x-1][y] =_color;
          break;
        }
        case(1):{
          screen[x+1][y-1] = 0;
          screen[x][y-1] =_color;
          screen[x+1][y] = 0;
          screen[x-1][y] =_color;
          screen[x][y+1] = 0;
          screen[x-1][y+1] =_color;
          break;
        }
      }
      _point.moveHorizontal(false);
    }

    void moveRight(std::array<std::array<int, 20>, 10> &screen) override{
      int x = _point.getX(), y = _point.getY();
      switch (_currentRotation) {
        case(0):{
          screen[x-1][y-1] = 0;
          screen[x+1][y-1] =_color;
          screen[x][y] = 0;
          screen[x+2][y] =_color;
          break;
        }
        case(1):{
          screen[x+1][y-1] = 0;
          screen[x+2][y-1] =_color;
          screen[x][y] = 0;
          screen[x+2][y] =_color;
          screen[x][y+1] = 0;
          screen[x+1][y+1] =_color;
          break;
        }
      }
      _point.moveHorizontal(true);
    }


};

class Game{
  private:
    Block* currentBlock;
    Block* nextBlock;
    Score score;
    int currentLevel;
    int linesBroken;
  public:
    std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen;
    std::array<std::array<int, 5>, 5> nextBlockScreen;
    Game(){
      currentBlock = nullptr;
      nextBlock = nullptr;
      currentLevel = 0;
      linesBroken = 0;
      for(int i = 0; i < SCREEN_BOX_WIDTH;i++){
        for(int j = 0; j < SCREEN_BOX_HEIGHT; j++){
          screen[i][j] = 0;
        }
      }
      for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
          nextBlockScreen[i][j] = 0;
        }
      }
    }

    void restartScreen(){
      for(int i = 0; i < SCREEN_BOX_WIDTH;i++){
        for(int j = 0; j < SCREEN_BOX_HEIGHT; j++){
          screen[i][j] = 0;
        }
      }

    }

    void generateNextBlock(int n){
      BlockType N;
      if(n == -1) N = static_cast<BlockType>(GetRandomValue(0, 6));
      else N = static_cast<BlockType>(n);
      switch (N) {
        case(L_BLOCK):{
          nextBlock = new LBlock();
          break;
        }
        case(J_BLOCK):{
          nextBlock = new JBlock();
          break;
        }
        case(I_BLOCK):{
          nextBlock = new IBlock();
          break;
        }
        case(O_BLOCK):{
          nextBlock = new OBlock();
          break;
        }
        case(S_BLOCK):{
          nextBlock = new SBlock();
          break;
        }
        case(T_BLOCK):{
          nextBlock = new TBlock();
          break;
        }
        case(Z_BLOCK):{
          nextBlock = new ZBlock();
          break;
        }
      }
    }

    void drawNextShape(){
      short color = nextBlock->getColor();
      switch (nextBlock->getBlockType()) {
        case(J_BLOCK):{
          nextBlockScreen = {{
            {0,0,0,0,0},
            {0,0,color,0,0},
            {0,0,color,0,0},
            {0,color,color,0,0},
            {0,0,0,0,0}
          }};
          break;
        }
        case(L_BLOCK):{
          nextBlockScreen = {{
            {0,0,0,0,0},
            {0,0,color,0,0},
            {0,0,color,0,0},
            {0,0,color,color,0},
            {0,0,0,0,0}
          }};
          break;
        }
        case(I_BLOCK):{
          nextBlockScreen = {{
            {0,0,0,0,0},
            {0,0,color,0,0},
            {0,0,color,0,0},
            {0,0,color,0,0},
            {0,0,color,0,0}
          }};
          break;
        }
        case(O_BLOCK):{
          nextBlockScreen = {{
            {0,0,0,0,0},
            {0,0,color,color,0},
            {0,0,color,color,0},
            {0,0,0,0,0},
            {0,0,0,0,0}
          }};
          break;
        }
        case(S_BLOCK):{
          nextBlockScreen = {{
            {0,0,0,0,0},
            {0,0,color,color,0},
            {0,color,color,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0}
          }};
          break;
        }
        case(T_BLOCK):{
          nextBlockScreen = {{
            {0,0,0,0,0},
            {0,color,color,color,0},
            {0,0,color,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0}
          }};
          break;
        }
        case(Z_BLOCK):{
          nextBlockScreen = {{
            {0,0,0,0,0},
            {0,color,color,0,0},
            {0,0,color,color,0},
            {0,0,0,0,0},
            {0,0,0,0,0}
          }};
          break;
        }
      }
    }
    
    bool isLineFull(std::array<std::array<int, SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH> screen, int n){
      for(int i = 0; i < SCREEN_BOX_WIDTH; i++){
        if(screen[i][n] == 0) return false;
      }
      return true;
    }
    bool isLineEmpty(std::array<std::array<int, SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH> screen, int n){
      for(int i = 0; i < SCREEN_BOX_WIDTH; i++){
        if(screen[i][n] != 0) return false;
      }
      return true;
    }
    
    void clearLine(std::array<std::array<int, SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH> &screen, int n){
      for(int i = 0; i < SCREEN_BOX_WIDTH; i++){
        screen[i][n] = 0;
      }

    }

    void swapLines(std::array<std::array<int, SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH> &screen, int n,int m){
     for(int i = 0; i < SCREEN_BOX_WIDTH; i++){
      std::swap(screen[i][n],screen[i][m]);
     } 
    }

    void clearLines(){
      int linesDeleted = 0;
      for(int i = 0; i < SCREEN_BOX_HEIGHT; i++){
        if(isLineFull(screen, i)){
          clearLine(screen,i);
          int j = i;
          linesDeleted++;
          linesBroken++;
          while (j > 0 && !isLineEmpty(screen,j-1)) {
            swapLines(screen, j-1, j);
            j--;
          }
        }
      }
      score.increaseScore(linesDeleted,currentLevel);
      if(linesBroken >= linesLevelUp[std::min(28,currentLevel)]){
        currentLevel++;
      }
    }

    bool setCurrentBlock(){
      free(currentBlock);
      currentBlock = nextBlock;
      generateNextBlock(-1);
      clearLines();
      if(currentBlock->canPlaceBlock(screen)){
        currentBlock->placeBlock(screen);
        return true;
      }
      return false;
    }
    int moveCurrentBlockDown(){
      if(currentBlock->canMoveDown(screen)){
        currentBlock->moveDown(screen);
        return 0; // can move down;
      }
      else{
        if(setCurrentBlock()){
          return 1;
        }
        return -1;
      }
    }

    Block* getNextBlock(){
      return nextBlock;
    }
    Block* getCurrentBlock(){
      return currentBlock;
    }
    
    Score getScore(){
      return score;
    }
    
    int getCurrentLevel(){
      return currentLevel;
    }

};



int main ()
{
  Game game;
  int delay = 0;
	SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  bool gameActive = true;
	InitWindow(600, 700, "OOP-Tetris");
  int framecount = 0;
  int blockSpeed = levelTimes[0];
  SetTargetFPS(60);
  int spawnedBlocks = 0;
  int screenStartX = 50, screenEndX = 300;
  int screenStartY = 80, screenEndY = 580;
  int nextScreenStartX = 350, nextScreenEndX = 475,
      nextScreenStartY = 150, nextScreenEndY = 275;
  int dX = (screenEndX - screenStartX) / SCREEN_BOX_WIDTH;
  int dY = (screenEndY - screenStartY) / SCREEN_BOX_HEIGHT;

  game.generateNextBlock(-1);
  game.setCurrentBlock();

	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		
		BeginDrawing();
  
		ClearBackground(WHITE);
    //DrawFPS(0, 0);
    if(gameActive){
      // Display main game screen
      DrawRectangleLines(screenStartX,screenStartY,250,500, BLACK);
      for(int i = 0; i < SCREEN_BOX_HEIGHT; i++){
        for(int j = 0; j < SCREEN_BOX_WIDTH; j++){
          if(game.screen[j][i] != 0)
              DrawRectangle(screenStartX + j * dX, screenStartY + i * dY, dX,dY,colors[colorsTable[game.getCurrentLevel()][game.screen[j][i]]]);
        }
      }
      DrawRectangleLines(nextScreenStartX, nextScreenStartY, 5*dX, 5*dY,BLACK);
      game.drawNextShape();
      // Display next Block
      for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
          if(game.nextBlockScreen[j][i] != 0)
            DrawRectangle(nextScreenStartX + i * dX,nextScreenStartY + j * dY, dX, dY,colors[colorsTable[game.getCurrentLevel()][game.nextBlockScreen[j][i]]]);
        }
      }
      // Display current Score;
      DrawRectangleLines(nextScreenStartX, nextScreenStartY + 150, 125, 125, BLACK);
      std::string txt = "Score: " + std::to_string(game.getScore().getCurrentScore());
      DrawText(txt.c_str(), 360, 320 , 20, BLACK);
      txt = "Level: " + std::to_string(game.getCurrentLevel());
      DrawText(txt.c_str(), 360, 360 , 20, BLACK);
      framecount++;
      if(delay != 0 && framecount <= delay){
        EndDrawing();
        continue;
      }
      else if (delay != 0) {
        delay = 0;
      }
      // catch key presses
      if(IsKeyPressed(KEY_Z) && game.getCurrentBlock()->canRotate(game.screen, true)){
        game.getCurrentBlock()->rotateBlock(game.screen, true);
      }
      if(IsKeyPressed(KEY_X) && game.getCurrentBlock()->canRotate(game.screen, false)){
        game.getCurrentBlock()->rotateBlock(game.screen,false);
      }
      if(IsKeyPressed(KEY_LEFT) && game.getCurrentBlock()->canMoveLeft(game.screen)){
        game.getCurrentBlock()->moveLeft(game.screen);
      }
      if(IsKeyPressed(KEY_RIGHT) && game.getCurrentBlock()->canMoveRight(game.screen)){
        game.getCurrentBlock()->moveRight(game.screen);
      }
      
      if(IsKeyDown(KEY_DOWN)){
        blockSpeed = 2;
      }
      else{
        blockSpeed = levelTimes[std::min(game.getCurrentLevel(),28)];
      }


      //DrawRectangleLines(50,30,250,30, BLACK);
      if(framecount >= blockSpeed){
        switch (game.moveCurrentBlockDown()) {
          case(0):{
            break;
          }
          case(1):{
            delay = 16;
            break;
          }
          case(-1):{
            gameActive = false;
            break;
          }
        }      
        framecount = 0;
      }
    }else{
      DrawText("GameOver\n press SPACE to restart", 300, 300, 20, BLACK);
      if(IsKeyPressed(KEY_SPACE)){
        gameActive = true;
      }
      game.restartScreen();
      game.getScore().resetScore();
    }

		EndDrawing();
    
	}
	CloseWindow();
	return 0;
}
