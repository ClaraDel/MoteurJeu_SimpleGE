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
} // namespace SimpleGE
