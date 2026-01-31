/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "../include/raylib.h"
#include <array>
#include <random>
#include <iostream>
#include <sys/types.h>


#define SCREEN_BOX_HEIGHT 20
#define SCREEN_BOX_WIDT 10

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
     Y--;
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


   virtual void rotateBlock(bool clockwise) = 0;
   virtual bool canRotate(std::array<std::array<int, SCREEN_BOX_WIDT>,SCREEN_BOX_HEIGHT> screen, bool clockwise) = 0;
   virtual bool canMoveDown(std::array<std::array<int, SCREEN_BOX_WIDT>,SCREEN_BOX_HEIGHT> screen) = 0;
   virtual bool canMoveLeft(std::array<std::array<int, SCREEN_BOX_WIDT>,SCREEN_BOX_HEIGHT> screen) = 0;
   virtual bool canMoveRight(std::array<std::array<int, SCREEN_BOX_WIDT>,SCREEN_BOX_HEIGHT> screen) = 0;
   virtual void placeBlock(std::array<std::array<int,SCREEN_BOX_WIDT>,SCREEN_BOX_HEIGHT>& screen) = 0;
   virtual void moveDown(std::array<std::array<int,SCREEN_BOX_WIDT>,SCREEN_BOX_HEIGHT>& screen) = 0;
};

class JBlock : public Block{

  private:
   std::array<std::array<std::array<int, 3>, 3>,4> BlockDescriptions{{
     {{{0,1,0},{0,1,0},{1,1,0}}},
     {{{1,0,0},{1,1,1},{0,0,0}}},
     {{{0,1,1},{0,1,0},{0,1,0}}},
     {{{0,0,0},{1,1,1},{0,0,1}}}
   }};
  
  public:
   JBlock() : Block(4, J_BLOCK, Point(1,5)) {};
  
  void rotateBlock(bool clockwise) override {
    if(clockwise){
      rotateClockwise();
    }
    else{
      rotateCounterClockwise();
    }
  }
  bool canRotate(std::array<std::array<int, SCREEN_BOX_WIDT>,SCREEN_BOX_HEIGHT> screen, bool clockwise) override{
    short currentRotation = _currentRotation;
    int x = _point.getX(),y = _point.getY();
    if(clockwise)
      switch (currentRotation) {
        case(0):{
          if(x == 0 || x == SCREEN_BOX_WIDT - 1) return false;
          bool res = screen[x-1][y] | screen[x+1][y] | screen[x+1][y+1];
          return !res;
        }
        case(1):{
          bool res = screen[x+1][y-1] || screen[x][y-1] || screen[x][y+1];
          return !res;
        }
        case(2):{
          if(x == 0 || x == SCREEN_BOX_WIDT - 1) return false;
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
          if(x == 0 || x == SCREEN_BOX_WIDT - 1) return false;
          bool res = screen[x-1][y] || screen[x-1][y-1] || screen[x+1][y];
          return !res;
        }
        case(1):{
          bool res = screen[x][y-1] || screen[x][y+1] || screen[x-1][y+1];
          return !res;
        }
        case(2):{
          if(x == 0 || x == SCREEN_BOX_WIDT - 1) return false;
          bool res = screen[x-1][y] | screen[x+1][y] | screen[x+1][y+1];
          return !res;
        }
        case(3):{
          bool res = screen[x+1][y-1] || screen[x][y-1] || screen[x][y+1];
          return !res;
        }
      }
    }
    return false;
  }

  bool canMoveDown(std::array<std::array<int, SCREEN_BOX_WIDT>,SCREEN_BOX_HEIGHT> screen) override{
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
  
  bool canMoveLeft(std::array<std::array<int, SCREEN_BOX_WIDT>,SCREEN_BOX_HEIGHT>screen) override{
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
  
  bool canMoveRight(std::array<std::array<int, SCREEN_BOX_WIDT>,SCREEN_BOX_HEIGHT> screen) override{
    int x = _point.getX(),y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        if(x == SCREEN_BOX_WIDT-1) return false;
        bool res = screen[x+1][y-1] || screen[x+1][y] || screen[x+1][y+1];
        return !res;
      }
      case(1):{
        if(x == SCREEN_BOX_WIDT-2) return false;
        bool res = screen[x+2][y+1] || screen[x+2][y];
        return !res;
      }
      case(2):{
        if(x == SCREEN_BOX_WIDT-2) return false;
        bool res = screen[x+2][y-1] || screen[x+1][y] || screen[x+1][y+1];
        return !res;
      }
      case(3):{
        if(x == SCREEN_BOX_WIDT-2) return false;
        bool res = screen[x+2][y] || screen[x][y-1];
        return !res;
      }
    }
    return false;
  }

  void moveDown(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> &screen) override{
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
  
  void placeBlock(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> &screen) override{
    int x = _point.getX(), y = _point.getY();
    screen[x][y]= 1;
    screen[x][y-1]= 1;
    screen[x][y+1]= 1;
    screen[x-1][y+1] = 1;
  }
};

class LBlock : public Block{
  std::array<std::array<std::array<int, 3>, 3>,4> BlockDescriptions{{
    {{{0,1,0},{0,1,0},{1,1,0}}},
      {{{1,0,0},{1,1,1},{0,0,0}}},
      {{{0,1,1},{0,1,0},{0,1,0}}},
      {{{0,0,0},{1,1,1},{0,0,1}}}
  }};

  public:

  LBlock() : Block(4, L_BLOCK, Point(1,5)) {};
  void rotateBlock(bool clockwise) override {
    if(clockwise){
      rotateClockwise();
    }
    else{
      rotateCounterClockwise();
    }
  }
  bool canRotate(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen, bool clockwise) override{
    short currentRotation = _currentRotation;
    int y = _point.getX(),x = _point.getY();
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
          if(x == 0 || x == SCREEN_BOX_WIDT - 1) return false;
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
          if(x == 0 || x == SCREEN_BOX_WIDT - 1) return false;
          bool res = screen[x-1][y] || screen[x-1][y+1] || screen[x+1][y];
          return !res;
        }
        case(1):{
          bool res = screen[x][y-1] || screen[x][y+1] || screen[x+1][y+1];
          return !res;
        }
        case(2):{
          if(x == 0 || x == SCREEN_BOX_WIDT - 1) return false;
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

  bool canMoveDown(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen) override{
    int y = _point.getX(),x = _point.getY();
    switch (_currentRotation) {
      case(0):{

          std::cout << "AAA\n";
        if(y == SCREEN_BOX_HEIGHT-2) return false;
          std::cout << "AAA\n";
        bool res = screen[x][y+2] || screen[x+1][y+2];
          std::cout << "AAA\n";
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
  
  bool canMoveLeft(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen) override{
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
  
  bool canMoveRight(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen) override{
    int x = _point.getX(),y = _point.getY();
    switch (_currentRotation) {
      case(0):{
        if(x == SCREEN_BOX_WIDT-2) return false;
        bool res = screen[x+1][y-1] || screen[x+1][y] || screen[x+2][y+1];
        return !res;
      }
      case(1):{
        if(x == SCREEN_BOX_WIDT-2) return false;
        bool res = screen[x+2][y-1] || screen[x+2][y];
        return !res;
      }
      case(2):{
        if(x == SCREEN_BOX_WIDT-1) return false;
        bool res = screen[x+1][y-1] || screen[x+1][y] || screen[x+1][y+1];
        return !res;
      }
      case(3):{
        if(x == SCREEN_BOX_WIDT-2) return false;
        bool res = screen[x+2][y] || screen[x][y+1];
        return !res;
      }
    }
    return false;
  }
  void moveDown(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> &screen) override{
    int y = _point.getX(), x = _point.getY();
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
        screen[x+1][y] = 0;
        screen[x+1][y+2]=1;
      break;
      }
      case(2):{
        screen[x][y-1]=0;
        screen[x-1][y-1] = 0;
        screen[x-1][y-1] = 0;
        screen[x][y+2] = 1;
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
  

  void placeBlock(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> &screen) override{
    int x = _point.getX(), y = _point.getY();
    screen[x][y]= 1;
    screen[x-1][y]= 1;
    screen[x+1][y]= 1;
    screen[x+1][y+1] = 1;
  }

};

class IBlock : public Block{
  private:



  public:
  IBlock() : Block(2, I_BLOCK, Point(1,5)) {};
  void rotateBlock(bool clockwise) override {
    if(clockwise){
      rotateClockwise();
    }
    else{
      rotateCounterClockwise();
    }
  }
  bool canRotate(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen, bool clockwise) override{
    short currentRotation = _currentRotation;
    int x = _point.getX(),y = _point.getY();
    switch (currentRotation) {
      case(1):{
        if(x == 0 || x == SCREEN_BOX_WIDT - 2) return false;
        bool res = screen[x-1][y] || screen[x+1][y] || screen[x+2][y];
        return !res;
      }
      case(2):{
        if(y >= SCREEN_BOX_HEIGHT-2 ) return false;
        bool res = screen[x][y-1] || screen[x][y+1] || screen[x][y+2];
        return !res;
      }
    }
    return false;
  }

  bool canMoveDown(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen) override{
    int x = _point.getX(),y = _point.getY();
    switch (_currentRotation) {
      case(1):{
        if(y == SCREEN_BOX_HEIGHT-3) return false;
        bool res = screen[x][y+3];
        return !res;
      }
      case(2):{
        if(y == SCREEN_BOX_HEIGHT-1) return false;
        bool res = screen[x-1][y+1] || screen[x][y+1] || screen[x+1][y+1] || screen[x+2][y+1];
        return !res;
      }
    }
    return false;
  }
  
  bool canMoveLeft(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen) override{
    int x = _point.getX(),y = _point.getY();
    switch (_currentRotation) {
      case(1):{
        if(x == 0) return false;
        bool res = screen[x-1][y-1] || screen[x-1][y] || screen[x-1][y+1] || screen[x-1][y+2];
        return !res;
      }
      case(2):{
        if(x == 1) return false;
        bool res = screen[x-2][y];
        return !res;
      }
    }
    return false;
  }
  
  bool canMoveRight(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen) override{
    int x = _point.getX(),y = _point.getY();
    switch (_currentRotation) {
      case(1):{
        if(x == SCREEN_BOX_WIDT-1) return false;
        bool res = screen[x+1][y-1] || screen[x+1][y] || screen[x+1][y+1] || screen[x+1][y+2];
        return !res;
      }
      case(2):{
        if(x == SCREEN_BOX_WIDT-3) return false;
        bool res = screen[x+3][y];
        return !res;
      }
    }
    return false;
  }

    void moveDown(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> &screen) override{
      int x = _point.getX(), y = _point.getY();
      switch (_currentRotation) {
        case(1):{
          screen[x][y-1] = 0;
          screen[x][y+3] = 1;
          break;
        }
        case(2):{
          screen[x-1][y] = 0;
          screen[x-1][y+1] = 1;
          screen[x][y] = 0;
          screen[x][y+1] = 1;
          screen[x+1][y] = 0;
          screen[x+1][y+1] = 1;
          screen[x+1][y] = 0;
          screen[x+1][y+1] = 1;
          break;
        }
      }
      _point.goDown();
    }

    void placeBlock(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> &screen) override{
      int x = _point.getX(), y = _point.getY();
      screen[x][y]= 1;
      screen[x][y-1]= 1;
      screen[x][y+1]= 1;
      screen[x+1][y+1] = 1;
    }

};
class OBlock : public Block{
  public:
  OBlock() : Block(1, O_BLOCK, Point(5,1)) {}; 
  void rotateBlock(bool clockwise) override {
      if(clockwise){
        rotateClockwise();
      }
      else{
        rotateCounterClockwise();
      }
    }

  bool canRotate(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen, bool clockwise) override{
    return true;
  }
  bool canMoveDown(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen) override{
    int x = _point.getX(), y = _point.getY();
    if(y == SCREEN_BOX_HEIGHT-1 || screen[x][y+1] || screen[x+1][y+1]) return false;
    return true;
  }
  bool canMoveLeft(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen) override{
    int x = _point.getX(), y = _point.getY();
    if(x == 0 || screen[x-1][y-1] || screen[x-1][y]) return false;
    return true;
  }

  bool canMoveRight(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen) override{
    int x = _point.getX(), y = _point.getY();
    if(x == SCREEN_BOX_WIDT - 2 || screen[x+2][y-1] || screen[x+2][y]) return false;
    return true;
  }
  
  void moveDown(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> &screen) override{
    int x = _point.getX(), y = _point.getY();
    screen[x][y-1] = 0;
    screen[x+1][y-1] = 0;
    screen[x][y+1] = 1;
    screen[x+1][y+1] = 1;
    _point.goDown();
  }

  void placeBlock(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> &screen) override{
    int x = _point.getX(), y = _point.getY();
    screen[x][y-1] = 1;
    screen[x][y] = 1;
    screen[x][y+1] = 1;
    screen[x][y+2] = 1;

  }

};
class SBlock : public Block{
  public:
    SBlock() : Block(2,S_BLOCK,Point(5,1)){};
       void rotateBlock(bool clockwise) override {
      if(clockwise){
        rotateClockwise();
      }
      else{
        rotateCounterClockwise();
      }
    }

    bool canRotate(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen, bool clockwise) override{
      short currentRotation = _currentRotation;
      int x = _point.getX(),y = _point.getY();
      switch (currentRotation) {
        case(1):{
          if(y == SCREEN_BOX_HEIGHT - 2) return false;
          bool res = screen[x-1][y-1] || screen[x][y+1];
          return !res;
        }
        case(2):{
          if(x == SCREEN_BOX_WIDT - 1) return false;
          bool res = screen[x][y-1] || screen[x+1][y-1];
          return !res;
        }
      }
      return false;
    }

    bool canMoveDown(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(1):{
          if(y == SCREEN_BOX_HEIGHT-1) return false;
          bool res = screen[x-1][y+1] || screen[x][y+1] || screen[x+1][y];
          return !res;
        }
        case(2):{
          if(y == SCREEN_BOX_HEIGHT-2) return false;
          bool res = screen[x-1][y+1] || screen[x][y+2];
          return !res;
        }
      }
      return false;
    }
    
    bool canMoveLeft(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(1):{
          if(x == 1) return false;
          bool res = screen[x-2][y] || screen[x-1][y-1];
          return !res;
        }
        case(2):{
          if(x == 1) return false;
          bool res = screen[x-2][y-1] || screen[x-2][y] || screen[x-1][y+1];
          return !res;
        }
      }
      return false;
    }
    
    bool canMoveRight(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(1):{
          if(x == SCREEN_BOX_WIDT-2) return false;
          bool res = screen[x+1][y] || screen[x+2][y-1];
          return !res;
        }
        case(2):{
          if(x == SCREEN_BOX_WIDT-1) return false;
          bool res = screen[x][y-1] || screen[x+1][y] || screen[x+1][y+1];
          return !res;
        }
      }
      return false;
    }
    void moveDown(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> &screen) override{
     int x = _point.getX(), y = _point.getY();
     switch (_currentRotation) {
       case(1):{
        screen[x-1][y]=0;
        screen[x-1][y+1]=1;
        screen[x][y-1] = 0;
        screen[x+1][y-1]=0;
        screen[x][y+1] = 1;
        screen[x+1][y] = 1;
        break;
       }
       case(2):{
        screen[x-1][y-1] = 0;
        screen[x][y]=0;
        screen[x-1][y+1]=1;
        screen[x][y+2]=1;
        break;
       }
     }
     _point.goDown();
    }

    void placeBlock(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> &screen) override{
      int x = _point.getX(), y = _point.getY();
      screen[x][y] = 1;
      screen[x-1][y] = 1;
      screen[x][y-1] = 1;
      screen[x+1][y-1] = 1;  
    }
    
};
class TBlock : public Block{
  public:
   TBlock() : Block(2,S_BLOCK,Point(5,1)){};
   void rotateBlock(bool clockwise) override {
      if(clockwise){
        rotateClockwise();
      }
      else{
        rotateCounterClockwise();
      }
    }
    bool canRotate(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen, bool clockwise) override{
      short currentRotation = _currentRotation;
      int x = _point.getX(),y = _point.getY();
      if(clockwise)
        switch (currentRotation) {
          case(1):{
            if(y == SCREEN_BOX_HEIGHT-1) return false;
            bool res = screen[x][y+1];
            return !res;
          }
          case(2):{
            if(x == SCREEN_BOX_WIDT-1) return false;
            bool res = screen[x+1][y];
            return !res;
          }
          case(3):{
            bool res = screen[x][y-1];
            return !res;
          }
          case(4):{
            if(x == 0) return false;
            bool res = screen[x-1][y];
            return !res;
          }
        }
      else{
        switch (currentRotation) {
          case(1):{
            bool res = screen[x][y-1];
            return !res;
          }
          case(2):{
            if(x == 0) return false;
            bool res = screen[x][y-1];
            return !res;
          }
          case(3):{
            if(y == SCREEN_BOX_HEIGHT - 1) return false;
            bool res = screen[x][y+1];
            return !res;
          }
          case(4):{
            if(x == SCREEN_BOX_WIDT - 1) return false;
            bool res = screen[x+1][y];
            return !res;
          }
        }
      }
      return false;
    }

    bool canMoveDown(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(1):{
          if(y == SCREEN_BOX_HEIGHT-1) return false;
          bool res = screen[x-1][y+1] || screen[x][y+1] || screen[x+1][y+1];
          return !res;
        }
        case(2):{
          if(y == SCREEN_BOX_HEIGHT-2) return false;
          bool res = screen[x-1][y+1] || screen[x][y+2];
          return !res;
        }
        case(3):{
          if(y == SCREEN_BOX_HEIGHT-2) return false;
          bool res = screen[x-1][y+1] || screen[x][y+2] || screen[x+1][y+1];
          return !res;
        }
        case(4):{
         if(y == SCREEN_BOX_HEIGHT - 2) return false;         
         bool res = screen[x][y+2] || screen[x+1][y+1];
         return !res;
        }
      }
      return false;
    }
    
    bool canMoveLeft(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(1):{
          if(x == 1) return false;
          bool res = screen[x-1][y-1] || screen[x-2][y];
          return !res;
        }
        case(2):{
          if(x == 1) return false;
          bool res = screen[x-1][y-1] || screen[x-2][y] || screen[x-1][y+1];
          return !res;
        }
        case(3):{
          if(x == 1) return false;
          bool res = screen[x-2][y] || screen[x-1][y+1];
          return !res;
        }
        case(4):{
          if(x == 0) return false;
          bool res = screen[x-1][y-1] || screen[x-1][y] || screen[x-1][y+1];
          return !res;
        }
      }
      return false;
    }
    
    bool canMoveRight(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(1):{
          if(x == SCREEN_BOX_WIDT-2) return false;
          bool res = screen[x+1][y-1] || screen[x+2][y];
          return !res;
        }
        case(2):{
          if(x == SCREEN_BOX_WIDT-1) return false;
          bool res = screen[x+1][y-1] || screen[x+1][y] || screen[x+1][y+1];
          return !res;
        }
        case(3):{
          if(x == SCREEN_BOX_WIDT-2) return false;
          bool res = screen[x+2][y] || screen[x+1][y+1];
          return !res;
        }
        case(4):{
          if(x == SCREEN_BOX_WIDT-2) return false;
          bool res = screen[x+1][y-1] || screen[x+2][y] || screen[x][y+1];
          return !res;
        }
      }
      return false;
    }

    void moveDown(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> &screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(1):{
         screen[x-1][y] = 0;
         screen[x-1][y+1] = 1;
         screen[x][y-1] = 0;
         screen[x][y+1] = 1;
         screen[x+1][y] = 0;
         screen[x+1][y+1] = 1;
         break;
        }
        case(2):{
         screen[x-1][y] = 0;
         screen[x-1][y+1] = 1;
         screen[x][y-1] = 0;
         screen[x][y+2]=1;
         break;
        }
        case(3):{
         screen[x-1][y] = 0;
         screen[x-1][y+1] = 1;
         screen[x][y]=0;
         screen[x][y+2] = 1;
         screen[x+1][y] = 0;
         screen[x+1][y+1] = 1;
         break;
        }
        case(4):{
         screen[x+1][y] = 0;
         screen[x+1][y+1] = 1;
         screen[x][y-1] = 0;
         screen[x][y+2]=1;
         break;
        }
      }
      _point.goDown();
    }

    void placeBlock(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> &screen) override{
      int x = _point.getX(),y = _point.getY();
      screen[x][y] = 1;
      screen[x-1][y] = 1;
      screen[x][y-1] = 1;
      screen[x+1][y] = 1;
    }

};
class ZBlock : public Block{
  public:
    ZBlock() : Block(2,S_BLOCK,Point(5,1)){};

   void rotateBlock(bool clockwise) override {
      if(clockwise){
        rotateClockwise();
      }
      else{
        rotateCounterClockwise();
      }
    }

    
    bool canRotate(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen, bool clockwise) override{
      short currentRotation = _currentRotation;
      int x = _point.getX(),y = _point.getY();
      switch (currentRotation) {
        case(1):{
          if(y == SCREEN_BOX_HEIGHT - 2) return false;
          bool res = screen[x][y+1] || screen[x+1][y-1];
          return !res;
        }
        case(2):{
          if(x == SCREEN_BOX_WIDT - 1) return false;
          bool res = screen[x][y-1] || screen[x-1][y-1];
          return !res;
        }
      }
      return false;
    }

    bool canMoveDown(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(1):{
          if(y == SCREEN_BOX_HEIGHT-2) return false;
          bool res = screen[x-1][y] || screen[x][y+1] || screen[x+1][y+1];
          return !res;
        }
        case(2):{
          if(y == SCREEN_BOX_HEIGHT-2) return false;
          bool res = screen[x][y+2] || screen[x+1][y+1];
          return !res;
        }
      }
      return false;
    }
    
    bool canMoveLeft(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(1):{
          if(x == 1) return false;
          bool res = screen[x-1][y] || screen[x-2][y-1];
          return !res;
        }
        case(2):{
          if(x == 0) return false;
          bool res = screen[x][y-1] || screen[x-1][y] || screen[x-1][y+1];
          return !res;
        }
      }
      return false;
    }
    
    bool canMoveRight(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen) override{
      int x = _point.getX(),y = _point.getY();
      switch (_currentRotation) {
        case(1):{
          if(x == SCREEN_BOX_WIDT-2) return false;
          bool res = screen[x+1][y-1] || screen[x+2][y];
          return !res;
        }
        case(2):{
          if(x == SCREEN_BOX_WIDT-2) return false;
          bool res = screen[x+2][y-1] || screen[x+2][y] || screen[x+1][y+1];
          return !res;
        }
      }
      return false;
    }
    void moveDown(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> &screen) override{
     int x = _point.getX(), y = _point.getY();
     switch (_currentRotation) {
       case(1):{
        screen[x-1][y]=1;
        screen[x-1][y-1]=0;
        screen[x][y-1] = 0;
        screen[x+1][y]=0;
        screen[x][y+1] = 1;
        screen[x+1][y+1] = 1;
        break;
       }
       case(2):{
        screen[x+1][y-1] = 0;
        screen[x][y]=0;
        screen[x+1][y+1]=1;
        screen[x][y+2]=1;
        break;
       }
     }
     _point.goDown();
    }

    void placeBlock(std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> &screen) override{
      int x = _point.getX(), y = _point.getY();
      screen[x][y] = 1;
      screen[x-1][y-1] = 1;
      screen[x][y-1] = 1;
      screen[x+1][y] = 1;  
    }
};

class Game{
  private:
    Block* currentBlock;
    Block* nextBlock;
    Score score;
  public:
    std::array<std::array<int, SCREEN_BOX_WIDT>, SCREEN_BOX_HEIGHT> screen;
    
    Game(){
      currentBlock = nullptr;
      nextBlock = nullptr;
      for(int i = 0; i < SCREEN_BOX_HEIGHT;i++){
        for(int j = 0; j < SCREEN_BOX_WIDT; j++){
          screen[i][j] = 0;
        }
      }
    }

    void generateNextBlock(int n){
      BlockType N = static_cast<BlockType>(n);
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
      currentBlock = nextBlock;
      nextBlock->placeBlock(screen);
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
  int dX = (screenEndX - screenStartX) / SCREEN_BOX_WIDT;
  int dY = (screenEndY - screenStartY) / SCREEN_BOX_HEIGHT;

  game.generateNextBlock(0);
  game.setCurrentBlock();


	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		
		BeginDrawing();

		ClearBackground(WHITE);
    DrawFPS(0, 0);

    DrawRectangleLines(screenStartX,screenStartY,250,500, BLACK);
    for(int i = 0; i < SCREEN_BOX_WIDT; i++){
      for(int j = 0; j < SCREEN_BOX_HEIGHT; j++){
        if(game.screen[j][i] == 1)
            DrawRectangle(screenStartX + i * dX, screenStartY + j * dY, dX,dY,(i+j)%2 == 0? RED : BLUE);
        else
            DrawRectangleLines(screenStartX + i * dX, screenStartY + j * dY, dX,dY,(i+j)%2 == 0? RED : BLUE);
      }
    }
        
    framecount++;
    //DrawRectangleLines(50,30,250,30, BLACK);
    if(framecount >= 30){
      game.moveCurrentBlockDown();
      framecount = 0;
    }
		EndDrawing();
    
	}


	CloseWindow();
	return 0;
}
