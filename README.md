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
  
public:
  Shape(const Circle& c): type(ShapeType::Circle)
  {
    new (&circle) Circle(c);
  }
  
  Shape(const Rectangle& r): type(ShapeType::Rectangle)
  {
    new (&rectangle) Rectangle(r);
  }
  
  ShapeType getType() const { return type; }
  
  Circle& getAsCircle()
  {
    assert(type == ShapeType::Circle);
    return circle;
  }
  
  Rectangle& getAsRectangle()
  {
    assert(type == ShapeType::Rectangle);
    return rectangle;
  }
  
  ~Shape()
  {
    switch (type)
    {
      case ShapeType::Circle:
        circle.~Circle();
        break;
      case ShapeType::Rectangle:
        rectangle.~Rectangle();
        break;
    }
  }
};
```

... and that's still not the whole story: depend on my needs I may want a copy/move constructor, a copy/move assignment operator and more convenient static factory functions. A "perfect" tagged union implementation that suits all my needs is going to be two to three times longer that I list here.

Boost provides the "Variant" library that kind of integrate a discriminated union into the language, but it is quite heavy-weight and I don't quite want to base my project on it. I could have also write a library that uses template metaprogramming to do the same thing, but there are some technical challenges there and I'm still not sure whether the result will be easy to use or not. 

This project takes another approach: it takes a specification and produce C++ codes that implement that specification directly. You can view it as a macro system that's independent of the compiler. While template metaprogramming has to obey the syntax rule of the language, this approach enjoys more freedom because I get to define my own syntax that's completely independent. The approach has its own downsides, but it's by far the most easy-to-implement and most easy-to-use method I can think of. 

## The name of this project
In ancient greek myths, Hermes is the god of transition and boundaries whos delivers messages from the gods. This project does something similar: it takes the programmer's messages of ADT, translates into C++ codes and deliver it to the C++ compiler. 
