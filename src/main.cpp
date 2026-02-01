/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "../include/raylib.h"
#include <array>
#include <cstdlib>
#include <sys/types.h>


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
};

class Score{
  private:
  long currentScore;
  long topScore;

  public:
  
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
};

class Block{
  protected:
   short _currentRotation; // 0 - N , 1 - E , 2 - S , 3 - W
   short _rotationCount; 
   BlockType _type;
   Point _point;
  public:

   Block(short rotationCount,BlockType type,Point point) : _currentRotation(0), _rotationCount(rotationCount), _type(type), _point(point){};
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


   virtual void rotateBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen,bool clockwise) = 0;
   virtual bool canRotate(std::array<std::array<int, SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH> screen, bool clockwise) = 0;
   virtual bool canMoveDown(std::array<std::array<int, SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH> screen) = 0;
   virtual bool canMoveLeft(std::array<std::array<int, SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH> screen) = 0;
   virtual bool canMoveRight(std::array<std::array<int, SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH> screen) = 0;
   virtual void placeBlock(std::array<std::array<int,SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH>& screen) = 0;
   virtual void moveDown(std::array<std::array<int,SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH>& screen) = 0;
  
   virtual void moveLeft(std::array<std::array<int,SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH>& screen) = 0;
   virtual void moveRight(std::array<std::array<int,SCREEN_BOX_HEIGHT>,SCREEN_BOX_WIDTH>& screen) = 0;
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
          screen[x][y-1] = 1;
          screen[x][y+1] = 1;
          screen[x-1][y+1] = 1;
          break;
        }
        case(1):{
          screen[x-1][y] = 1;
          screen[x+1][y] = 1;
          screen[x+1][y+1] = 1;
          screen[x][y-1] = 0;
          screen[x][y+1] = 0;
          screen[x-1][y+1] = 0;

          break;
        }
        case(2):{
          screen[x-1][y] = 0;
          screen[x+1][y] = 0;
          screen[x+1][y+1] = 0;
          screen[x+1][y-1] = 1;
          screen[x][y-1] = 1;
          screen[x][y+1] = 1;
          break;
        }
        case(3):{
          screen[x+1][y-1] = 0;
          screen[x][y-1] = 0;
          screen[x][y+1] = 0;
          screen[x-1][y] = 1;
          screen[x-1][y-1] = 1;
          screen[x+1][y] = 1;
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
          screen[x][y-1] = 1;
          screen[x][y+1] = 1;
          screen[x-1][y+1] = 1;
          break;
        }
        case(1):{
          screen[x-1][y] = 1;
          screen[x+1][y] = 1;
          screen[x+1][y+1] = 1;
          screen[x][y-1] = 0;
          screen[x][y+1] = 0;
          screen[x+1][y-1] = 0;

          break;
        }
        case(2):{
          screen[x-1][y] = 0;
          screen[x+1][y] = 0;
          screen[x-1][y-1] = 0;
          screen[x+1][y-1] = 1;
          screen[x][y-1] = 1;
          screen[x][y+1] = 1;
          break;
        }
        case(3):{
          screen[x-1][y+1] = 0;
          screen[x][y-1] = 0;
          screen[x][y+1] = 0;
          screen[x-1][y] = 1;
          screen[x-1][y-1] = 1;
          screen[x+1][y] = 1;
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
        screen[x-1][y+2] = 1;
        screen[x][y+2] = 1;
      break;
      }
      case(1):{
        screen[x-1][y] = 0;
        screen[x-1][y+1] = 1;
        screen[x][y] = 0;
        screen[x][y+1] = 1;
        screen[x+1][y] = 0;
        screen[x+1][y+2]=1;
      break;
      }
      case(2):{
        screen[x][y-1]=0;
        screen[x+1][y-1] = 0;
        screen[x+1][y-1] = 0;
        screen[x][y+2] = 1;
        screen[x+1][y] = 1;
      break;
      }
      case(3):{
        screen[x-1][y-1] = 0;
        screen[x-1][y+1] = 1;
        screen[x][y] = 0;
        screen[x][y+1] = 1;
        screen[x+1][y] = 0;
        screen[x+1][y+1] = 1;
      break;
      }
    }
    _point.goDown();
  }
  
  void placeBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
    int x = _point.getX(), y = _point.getY();
    screen[x][y]= 1;
    screen[x][y-1]= 1;
    screen[x][y+1]= 1;
    screen[x-1][y+1] = 1;
  }

  void moveLeft(std::array<std::array<int, 20>, 10> &screen) override{
    int x = _point.getX(), y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        screen[x][y-1] = 0;
        screen[x-1][y-1] = 1;
        screen[x][y] = 0;
        screen[x-1][y] = 1;
        screen[x][y+1] = 0;
        screen[x-2][y+1] = 1;
        break;
      }
      case(1):{
        screen[x+1][y+1] = 0;
        screen[x+1][y] = 0;
        screen[x][y+1] = 1;
        screen[x-2][y] = 1;
        break;
      }
      case(2):{
        screen[x+1][y-1]=0;
        screen[x-1][y-1] = 1;
        screen[x][y] = 0;
        screen[x-1][y] = 1;
        screen[x][y+1] = 0;
        screen[x-1][y+1] = 1;
        break;
      }
      case(3):{
        screen[x-1][y-1] = 0;
        screen[x-2][y-1] = 1;
        screen[x+1][y] = 0;
        screen[x-2][y] = 1;
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
        screen[x+1][y-1] = 1;
        screen[x][y] = 0;
        screen[x+1][y] = 1;
        screen[x-1][y+1] = 0;
        screen[x+1][y+1] = 1;
        break;
      }
      case(1):{
        screen[x+1][y+1] = 0;
        screen[x-1][y] = 0;
        screen[x+2][y+1] = 1;
        screen[x+2][y] = 1;
        break;
      }
      case(2):{
        screen[x][y-1] = 0;
        screen[x+2][y-1] = 1;
        screen[x][y] = 0;
        screen[x+1][y] = 1;
        screen[x][y+1] = 0;
        screen[x+1][y+1] = 1;
        break;
      }
      case(3):{
        screen[x-1][y-1] = 0;
        screen[x][y-1] = 1;
        screen[x-1][y] = 0;
        screen[x+2][y] = 1;
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
          screen[x][y-1] = 1;
          screen[x][y+1] = 1;
          screen[x+1][y+1] = 1;
          break;
        }
        case(1):{
          screen[x-1][y] = 1;
          screen[x+1][y] = 1;
          screen[x+1][y-1] = 1;
          screen[x][y-1] = 0;
          screen[x][y+1] = 0;
          screen[x+1][y+1] = 0;

          break;
        }
        case(2):{
          screen[x-1][y] = 0;
          screen[x+1][y] = 0;
          screen[x+1][y-1] = 0;
          screen[x-1][y-1] = 1;
          screen[x][y-1] = 1;
          screen[x][y+1] = 1;
          break;
        }
        case(3):{
          screen[x-1][y-1] = 0;
          screen[x][y-1] = 0;
          screen[x][y+1] = 0;
          screen[x-1][y] = 1;
          screen[x-1][y+1] = 1;
          screen[x+1][y] = 1;
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
          screen[x][y-1] = 1;
          screen[x][y+1] = 1;
          screen[x+1][y+1] = 1;
          break;
        }
        case(1):{
          screen[x-1][y] = 1;
          screen[x+1][y] = 1;
          screen[x+1][y-1] = 1;
          screen[x][y-1] = 0;
          screen[x][y+1] = 0;
          screen[x-1][y-1] = 0;

          break;
        }
        case(2):{
          screen[x-1][y] = 0;
          screen[x+1][y] = 0;
          screen[x-1][y+1] = 0;
          screen[x-1][y-1] = 1;
          screen[x][y-1] = 1;
          screen[x][y+1] = 1;
          break;
        }
        case(3):{
          screen[x+1][y+1] = 0;
          screen[x][y-1] = 0;
          screen[x][y+1] = 0;
          screen[x-1][y] = 1;
          screen[x-1][y+1] = 1;
          screen[x+1][y] = 1;
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
        screen[x+1][y+2] = 1;
        screen[x][y+2] = 1;
      break;
      }
      case(1):{
        screen[x-1][y] = 0;
        screen[x-1][y+1] = 1;
        screen[x][y] = 0;
        screen[x][y+1] = 1;
        screen[x+1][y-1] = 0;
        screen[x+1][y+1]=1;
        break;
      }
      case(2):{
        screen[x][y-1]=0;
        screen[x-1][y-1] = 0;
        screen[x-1][y-1] = 0;
        screen[x][y+2] = 1;
        screen[x-1][y] = 1;
      break;
      }
      case(3):{
        screen[x-1][y] = 0;
        screen[x-1][y+2] = 1;
        screen[x][y] = 0;
        screen[x][y+1] = 1;
        screen[x+1][y] = 0;
        screen[x+1][y+1] = 1;
      break;
      }
    }
    _point.goDown();
  }
  

  void placeBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
    int x = _point.getX(), y = _point.getY();
    screen[x][y]= 1;
    screen[x][y-1]= 1;
    screen[x][y+1]= 1;
    screen[x+1][y+1] = 1;
  }
  void moveLeft(std::array<std::array<int, 20>, 10> &screen) override{
    int x = _point.getX(), y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        screen[x][y-1] = 0;
        screen[x-1][y-1] = 1;
        screen[x][y] = 0;
        screen[x-1][y] = 1;
        screen[x+1][y+1] = 0;
        screen[x-1][y+1] = 1;
        break;
      }
      case(1):{
        screen[x+1][y-1] = 0;
        screen[x+1][y] = 0;
        screen[x][y-1] = 1;
        screen[x-2][y] = 1;
        break;
      }
      case(2):{
        screen[x][y-1]=0;
        screen[x-2][y-1] = 1;
        screen[x][y] = 0;
        screen[x-1][y] = 1;
        screen[x][y+1] = 0;
        screen[x-1][y+1] = 1;
        break;
      }
      case(3):{
        screen[x-1][y+1] = 0;
        screen[x-2][y+1] = 1;
        screen[x+1][y] = 0;
        screen[x-2][y] = 1;
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
        screen[x+1][y-1] = 1;
        screen[x][y] = 0;
        screen[x+1][y] = 1;
        screen[x][y+1] = 0;
        screen[x+2][y+1] = 1;
        break;
      }
      case(1):{
        screen[x+1][y-1] = 0;
        screen[x-1][y] = 0;
        screen[x+2][y-1] = 1;
        screen[x+2][y] = 1;
        break;
      }
      case(2):{
        screen[x-1][y-1] = 0;
        screen[x+1][y-1] = 1;
        screen[x][y] = 0;
        screen[x+1][y] = 1;
        screen[x][y+1] = 0;
        screen[x+1][y+1] = 1;
        break;
      }
      case(3):{
        screen[x-1][y+1] = 0;
        screen[x][y+1] = 1;
        screen[x-1][y] = 0;
        screen[x+2][y] = 1;
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
          screen[x][y-1] = 1;
          screen[x][y+1] = 1;
          screen[x][y+2] = 1;
          screen[x-1][y] = 0;
          screen[x+1][y] = 0;
          screen[x+2][y] = 0;
          break;
        }
        case(1):{
          screen[x][y-1] = 0;
          screen[x][y+1] = 0;
          screen[x][y+2] = 0;
          screen[x-1][y] = 1;
          screen[x+1][y] = 1;
          screen[x+2][y] = 1;
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
          screen[x][y+3] = 1;
          break;
        }
        case(1):{
          screen[x-1][y] = 0;
          screen[x-1][y+1] = 1;
          screen[x][y] = 0;
          screen[x][y+1] = 1;
          screen[x+1][y] = 0;
          screen[x+1][y+1] = 1;
          screen[x+2][y] = 0;
          screen[x+2][y+1] = 1;
          break;
        }
      }
      _point.goDown();
    }

    void placeBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
      int x = _point.getX(), y = _point.getY();
      screen[x][y]= 1;
      screen[x][y-1]= 1;
      screen[x][y+1]= 1;
      screen[x][y+2] = 1;
    }
    void moveLeft(std::array<std::array<int, 20>, 10> &screen) override{
    int x = _point.getX(), y = _point.getY();
    switch (_currentRotation) {
      case(0):{
       screen[x][y-1] = 0;
       screen[x-1][ y-1] = 1;
       screen[x][y] = 0;
       screen[x-1][y]= 1;
       screen[x][y+1] = 0;
       screen[x-1][y+1] = 1;
       screen[x][y+2] = 0;
       screen[x-1][y+2] = 1;
       break;
      }
      case(1):{
        screen[x+2][y] = 0;
        screen[x-2][y] = 1;
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
       screen[x+1][ y-1] = 1;
       screen[x][y] = 0;
       screen[x+1][y]= 1;
       screen[x][y+1] = 0;
       screen[x+1][y+1] = 1;
       screen[x][y+2] = 0;
       screen[x+1][y+2] = 1;
       break;
      }
      case(1):{
        screen[x-1][y] = 0;
        screen[x+3][y] = 1;
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
    screen[x][y+1] = 1;
    screen[x+1][y+1] = 1;
    _point.goDown();
  }

  void placeBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
    int x = _point.getX(), y = _point.getY();
    screen[x+1][y-1] = 1;
    screen[x+1][y] = 1;
    screen[x][y] = 1;
    screen[x][y-1] = 1;

  }

  void moveLeft(std::array<std::array<int, 20>, 10> &screen) override{
    int x = _point.getX(), y = _point.getY();
    screen[x+1][y-1] = 0;
    screen[x-1][y-1] = 1;
    screen[x+1][y] = 0;
    screen[x-1][y] = 1;
    _point.moveHorizontal(false);
  }
  void moveRight(std::array<std::array<int, 20>, 10> &screen) override{
    int x = _point.getX(), y = _point.getY();
    screen[x][y-1] = 0;
    screen[x+2][y-1] = 1;
    screen[x][y] = 0;
    screen[x+2][y] = 1;

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
          screen[x][y-1] = 1;
          screen[x+1][y-1] = 1;
          screen[x-1][y-1] = 0;
          screen[x][y+1] = 0;
          break;
        }
        case(1):{
          screen[x][y-1] = 0;
          screen[x+1][y-1] = 0;
          screen[x-1][y-1] = 1;
          screen[x][y+1] = 1;
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
        screen[x-1][y+1]=1;
        screen[x][y-1] = 0;
        screen[x+1][y-1]=0;
        screen[x][y+1] = 1;
        screen[x+1][y] = 1;
        break;
       }
       case(1):{
        screen[x-1][y-1] = 0;
        screen[x][y]=0;
        screen[x-1][y+1]=1;
        screen[x][y+2]=1;
        break;
       }
     }
     _point.goDown();
    }

    void placeBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
      int x = _point.getX(), y = _point.getY();
      screen[x][y] = 1;
      screen[x-1][y] = 1;
      screen[x][y-1] = 1;
      screen[x+1][y-1] = 1;  
    }

    void moveLeft(std::array<std::array<int, 20>, 10> &screen) override{
      int x = _point.getX(), y = _point.getY();
      switch (_currentRotation) {
        case(0):{
          screen[x+1][y-1] = 0;
          screen[x-1][y-1] = 1;
          screen[x][y] = 0;
          screen[x-2][y] = 1;
          break;
        }
        case(1):{
          screen[x-1][y-1] = 0;
          screen[x-2][y-1] = 1;
          screen[x][y] = 0;
          screen[x-2][y] = 1;
          screen[x][y+1] = 0;
          screen[x-1][y+1] = 1;
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
          screen[x+2][y-1] = 1;
          screen[x-1][y] = 0;
          screen[x+1][y] = 1;
          break;
        }
        case(1):{
          screen[x-1][y-1] = 0;
          screen[x][y-1] = 1;
          screen[x-1][y] = 0;
          screen[x+1][y] = 1;
          screen[x][y+1] = 0;
          screen[x+1][y+1] = 1;
          break;
        }
      }
      _point.moveHorizontal(true);
    }

};
class TBlock : public Block{
  public:
   TBlock() : Block(4,S_BLOCK,Point(5,1)){};
   void rotateBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen,bool clockwise) override {
      int x = _point.getX(),y = _point.getY();
      if(clockwise){
        rotateClockwise();
        switch (_currentRotation) {
          case(0):{
            screen[x][y+1]=0;
            screen[x+1][y]=1;
            break;
          }
          case(1):{
            screen[x-1][y] = 0;
            screen[x][y+1] = 1;
            break;
          }
          case(2):{
            screen[x][y-1]=0;
            screen[x-1][y]=1;
            break;
          }
          case(3):{
            screen[x+1][y] = 0;
            screen[x][y-1] = 1;
            break;
          }
        
        }
      }
      else{
        rotateCounterClockwise();
        switch (_currentRotation) {
          case(0):{
            screen[x][y+1]=0;
            screen[x-1][y]=1;
            break;
          }
          case(1):{
            screen[x-1][y] = 0;
            screen[x][y-1] = 1;
            break;
          }
          case(2):{
            screen[x][y-1]=0;
            screen[x+1][y]=1;
            break;
          }
          case(3):{
            screen[x+1][y] = 0;
            screen[x][y+1] = 1;
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
            if(y == SCREEN_BOX_HEIGHT-1) return false;
            bool res = screen[x][y+1];
            return !res;
          }
          case(1):{
            if(x == SCREEN_BOX_WIDTH-1) return false;
            bool res = screen[x-1][y];
            return !res;
          }
          case(2):{
            bool res = screen[x][y-1];
            return !res;
          }
          case(3):{
            if(x == 0) return false;
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
          if(x == 1) return false;
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
          if(x == SCREEN_BOX_WIDTH-1) return false;
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
         screen[x-1][y+1] = 1;
         screen[x][y-1] = 0;
         screen[x][y+1] = 1;
         screen[x+1][y] = 0;
         screen[x+1][y+1] = 1;
         break;
        }
        case(1):{
         screen[x+1][y] = 0;
         screen[x+1][y+1] = 1;
         screen[x][y-1] = 0;
         screen[x][y+2]=1;
         break;
        }
        case(2):{
         screen[x-1][y] = 0;
         screen[x-1][y+1] = 1;
         screen[x][y]=0;
         screen[x][y+2] = 1;
         screen[x+1][y] = 0;
         screen[x+1][y+1] = 1;
         break;
        }
        case(3):{
         screen[x-1][y] = 0;
         screen[x-1][y+1] = 1;
         screen[x][y-1] = 0;
         screen[x][y+2]=1;
         break;
        }
      }
      _point.goDown();
    }

    void placeBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
      int x = _point.getX(),y = _point.getY();
      screen[x][y] = 1;
      screen[x-1][y] = 1;
      screen[x][y-1] = 1;
      screen[x+1][y] = 1;
    }

  void moveLeft(std::array<std::array<int, 20>, 10> &screen) override{
    int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(0):{
         screen[x][y-1] = 0;
         screen[x-1][y-1] = 1;
         screen[x+1][y] = 0;
         screen[x-2][y] = 1;
         break;
        }
        case(1):{
          screen[x][y-1] = 0;
          screen[x-1][y-1] = 1;
          screen[x+1][y] = 0;
          screen[x-1][y] = 1;
          screen[x][y+1] = 0;
          screen[x-1][y+1] = 1;
          break;
        }
        case(2):{
          screen[x+1][y] = 0;
          screen[x-2][y] = 1;
          screen[x][y+1] = 0;
          screen[x-1][y+1] = 1;
          break;
        }
        case(3):{
          screen[x][y-1] = 0;
          screen[x-1][y-1] = 1;
          screen[x][y] = 0;
          screen[x-2][y] = 1;
          screen[x][y+1] = 0;
          screen[x-1][y+1] = 1;
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
         screen[x+2][y] = 1;
         screen[x][y-1] = 0;
         screen[x+1][y-1] = 1;
         break;
        }
        case(1):{
          screen[x][y-1] = 0;
          screen[x+1][y-1] = 1;
          screen[x][y] = 0;
          screen[x+2][y] = 1;
          screen[x][y+1] = 0;
          screen[x+1][y+1] = 1;
          break;
        }
        case(2):{
          screen[x-1][y] = 0;
          screen[x+2][y] = 1;
          screen[x][y+1] = 0;
          screen[x+1][y+1] = 1;
          break;
        }
        case(3):{
          screen[x][y-1] = 0;
          screen[x+1][y-1] = 1;
          screen[x-1][y] = 0;
          screen[x+1][y] = 1;
          screen[x][y+1] = 0;
          screen[x+1][y+1] = 1;
          break;
        }
      }
      _point.moveHorizontal(true);
    }
};
class ZBlock : public Block{
  public:
    ZBlock() : Block(2,S_BLOCK,Point(5,1)){};

   void rotateBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen,bool clockwise) override {
      int x = _point.getX(),y = _point.getY();
      rotateClockwise();
      switch (_currentRotation) {
        case(0):{
          screen[x][y-1] = 1;
          screen[x-1][y-1] = 1;
          screen[x+1][y-1] = 0;
          screen[x][y+1] = 0;
          break;
        }
        case(1):{
          screen[x][y-1] = 0;
          screen[x-1][y-1] = 0;
          screen[x+1][y-1] = 1;
          screen[x][y+1] = 1;
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
          if(x == SCREEN_BOX_WIDTH - 1) return false;
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
        screen[x-1][y]=1;
        screen[x-1][y-1]=0;
        screen[x][y-1] = 0;
        screen[x+1][y]=0;
        screen[x][y+1] = 1;
        screen[x+1][y+1] = 1;
        break;
       }
       case(1):{
        screen[x+1][y-1] = 0;
        screen[x][y]=0;
        screen[x+1][y+1]=1;
        screen[x][y+2]=1;
        break;
       }
     }
     _point.goDown();
    }

    void placeBlock(std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> &screen) override{
      int x = _point.getX(), y = _point.getY();
      screen[x][y] = 1;
      screen[x-1][y-1] = 1;
      screen[x][y-1] = 1;
      screen[x+1][y] = 1;  
    }

    void moveLeft(std::array<std::array<int, 20>, 10> &screen) override{
      int x = _point.getX(), y = _point.getY();
      switch (_currentRotation) {
        case(0):{
          screen[x][y-1] = 0;
          screen[x-2][y-1] = 1;
          screen[x+1][y] = 0;
          screen[x-1][y] = 1;
          break;
        }
        case(1):{
          screen[x+1][y-1] = 0;
          screen[x][y-1] = 1;
          screen[x+1][y] = 0;
          screen[x-1][y] = 1;
          screen[x][y+1] = 0;
          screen[x-1][y+1] = 1;
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
          screen[x+1][y-1] = 1;
          screen[x][y] = 0;
          screen[x+2][y] = 1;
          break;
        }
        case(1):{
          screen[x+1][y-1] = 0;
          screen[x+2][y-1] = 1;
          screen[x][y] = 0;
          screen[x+2][y] = 1;
          screen[x][y+1] = 0;
          screen[x+1][y+1] = 1;
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
  public:
    std::array<std::array<int, SCREEN_BOX_HEIGHT>, SCREEN_BOX_WIDTH> screen;
    
    Game(){
      currentBlock = nullptr;
      nextBlock = nullptr;
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

    void DrawNextShape();
    
    void setCurrentBlock(){
      free(currentBlock);
      currentBlock = nextBlock;
      generateNextBlock(-1);
      currentBlock->placeBlock(screen);
    }
    void moveCurrentBlockDown(){
      if(currentBlock->canMoveDown(screen)){
        currentBlock->moveDown(screen);
      }
      else{
        setCurrentBlock();
      }
    }

    Block* getNextBlock(){
      return nextBlock;
    }
    Block* getCurrentBlock(){
      return currentBlock;
    }

};



int main ()
{
  Game game;

	SetConfigFlags(FLAG_WINDOW_HIGHDPI);

	InitWindow(400, 600, "OOP-Tetris");
  int framecount = 0;
  SetTargetFPS(60);
  int spawnedBlocks = 0;
  int screenStartX = 50, screenEndX = 300;
  int screenStartY = 80, screenEndY = 580;
  int dX = (screenEndX - screenStartX) / SCREEN_BOX_WIDTH;
  int dY = (screenEndY - screenStartY) / SCREEN_BOX_HEIGHT;

  game.generateNextBlock(5);
  game.setCurrentBlock();

	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		
		BeginDrawing();
  
		ClearBackground(WHITE);
    DrawFPS(0, 0);
    DrawRectangleLines(screenStartX,screenStartY,250,500, BLACK);
    for(int i = 0; i < SCREEN_BOX_HEIGHT; i++){
      for(int j = 0; j < SCREEN_BOX_WIDTH; j++){
        if(game.screen[j][i] == 1)
            DrawRectangle(screenStartX + j * dX, screenStartY + i * dY, dX,dY,RED);
      }
    }
    
    if(IsKeyPressed(KEY_X) && game.getCurrentBlock()->canRotate(game.screen, true)){
      game.getCurrentBlock()->rotateBlock(game.screen, true);
    }
    if(IsKeyPressed(KEY_Z) && game.getCurrentBlock()->canRotate(game.screen, false)){
      game.getCurrentBlock()->rotateBlock(game.screen,false);
    }
    if(IsKeyPressed(KEY_LEFT) && game.getCurrentBlock()->canMoveLeft(game.screen)){
      game.getCurrentBlock()->moveLeft(game.screen);
    }
    if(IsKeyPressed(KEY_RIGHT) && game.getCurrentBlock()->canMoveRight(game.screen)){
      game.getCurrentBlock()->moveRight(game.screen);
    }

    framecount++;
    //DrawRectangleLines(50,30,250,30, BLACK);
    if(framecount >= 20){
      game.moveCurrentBlockDown();
      framecount = 0;
    }
		EndDrawing();
    
	}


	CloseWindow();
	return 0;
}
