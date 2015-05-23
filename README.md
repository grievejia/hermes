# hermes
A tool that translates from a specification of Algebraic Data Type to C++14 codes

## Motivation
C++14 lacks language-level support of algebraic data type. Defining an ADT is very concise in a language like Haskell:

```haskell
data Shape = 
  Circle Int |
  Rectangle Int Int
```
Two commonly-used approaches can be used to implement that in C++. The first one is the usage of class hierachy:

```cpp
class Shape {};

class Circle: public Shape
{
private:
  int radius;
public:
  Circle(int r): radius(r) {}
  int getRadius() const { return r; }
};

class Rectangle: public Shape
{
private:
  int side0, side1;
public:
  Rectangle(int s0, int s1): side0(s0), side1(s1) {}
  int getSide0() const { return side0; }
  int getSide1() const { return side1; }
};
```

... which is very verbose and tedious to write. Also, if the user wants to use the Shape class polymorphically, she has to define a virtual destructor for Shape, and then probably allocate Circle and Rectangle dynamically on heap. What I would do is to write additional wrapper class or typedef around a unique_ptr of those sub-classes I defined to make them easier to use. Moreover, if I want to get rid of RTTI (which I usually do), I need to stuck a tag field into Shape class to hand-roll a featherweight RTTI into my class hierachy. Again, those additional work costs my effort and adds code noises. 

If we want to maintain value semantics and stack allocation for Shape objects, we can implement ADT in another way: tagged union. Such a task is possible and efficient, but again it is very tedious to write:

```cpp
enum class ShapeType
{
  Circle,
  Rectangle,
};

class Circle: public Shape
{
private:
  int radius;
public:
  Circle(int r): radius(r) {}
  int getRadius() const { return r; }
};

class Rectangle: public Shape
{
private:
  int side0, side1;
public:
  Rectangle(int s0, int s1): side0(s0), side1(s1) {}
  int getSide0() const { return side0; }
  int getSide1() const { return side1; }
};

class Shape
{
private:
  ShapeType type;
  
  union
  {
    Circle circle;
    Rectangle rectangle;
  }
  
  Shape(const Circle& c): type(ShapeType::Circle)
  {
    new (&circle) Circle(c);
  }
  
public:
};
```
