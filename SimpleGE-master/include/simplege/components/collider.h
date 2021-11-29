#pragma once

#include <simplege/pch/precomp.h>

#include <simplege/components/component.h>

#include <simplege/math.h>

#include <iostream>

namespace SimpleGE
{
  class CollisionComponent;

  struct ColliderImpl;

  class ColliderComponent : public Component
  {
  public:
    static constexpr auto Type = "Collider";

    struct Description
    {
      int flag{};
      int mask{};
      Size<int> size{};
      ComponentReference<CollisionComponent> handler;
    };

    ColliderComponent(Entity& owner);
    ~ColliderComponent() override;

    SetupResult Setup(const nlohmann::json& descr) override;
    SetupResult Setup(const Description& descr);

    [[nodiscard]] bool Collides(const ColliderComponent& other) const;

    void OnCollision(const ColliderComponent& other) const;

    [[nodiscard]] Area GetArea() const;

  private:
    std::unique_ptr<ColliderImpl> impl;
  };


  struct Rectangle
  {
    //Position en haut à gauche du rectangle
    float x ; 
    float y ;
    //dimension du rectangle
    float width ;
    float height ;

    Rectangle(float _x, float _y, float _width, float _height)
    {
      x = _x;
      y = _y;
      width = _width ;
      height = _height ;
    }
  };

  class QuadTree
  {
    public :
    // -------------------- ENLEVER LE POINTEUR ----------------------
    Rectangle *rectangle ;
    QuadTree(int level, float x, float y, float width, float height) ;

    void clear();
    void split();
    int getIndex(ColliderComponent* element);
    void insert(ColliderComponent* element);
    std::vector<ColliderComponent*> retrieve(std::vector<ColliderComponent*>& returnObj, ColliderComponent* element);
	

    private :
    int maxComponent = 2;
    int maxLevels = 5;
    int level ;
    std::vector<ColliderComponent*> objects;
    std::vector<QuadTree*> nodes;
  };
} // namespace SimpleGE
