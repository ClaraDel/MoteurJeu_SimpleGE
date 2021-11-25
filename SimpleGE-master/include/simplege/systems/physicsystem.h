#pragma once

#include <simplege/pch/precomp.h>

#include <simplege/components/collider.h>
#include <simplege/entity.h>
#include <simplege/systems/system.h>

namespace SimpleGE
{
  class PhysicSystem : public ISystem
  {
  public:
    QuadTree* quadTree;
    static PhysicSystem& Instance()
    {
      static PhysicSystem instance;
      
      //récupère les dimentions de la fenêtre
      const auto* currentCamera = CameraComponent::Current();
      Ensures(currentCamera != nullptr);
      const auto viewWidth = currentCamera->ViewWidth();
      const auto viewHeight = currentCamera->ViewHeight();

      //créée un quadTree
      instance.quadTree = new QuadTree(0, 0, 0, viewWidth, viewHeight);
      
      return instance;
    }

    PhysicSystem(const PhysicSystem&) = delete;
    PhysicSystem(PhysicSystem&&) = delete;
    PhysicSystem& operator=(const PhysicSystem&) = delete;
    PhysicSystem& operator=(PhysicSystem&&) = delete;

    void Register(gsl::not_null<ColliderComponent*> comp) { colliders.insert(comp); }

    void Unregister(gsl::not_null<ColliderComponent*> comp) { colliders.erase(comp); }

    void Iterate(const Timing& timing) override
    {
      std::vector<gsl::not_null<ColliderComponent*>> collidersVec;
      collidersVec.reserve(colliders.size());

      quadTree->clear();
      for (int i = 0; i < collidersVec.size(); i++) {
        quadTree->insert(allObjects.get(i));
      } //TODO

      for (auto c : colliders)
      {
        if (c->Enabled() && c->Owner().Active())
        {
          collidersVec.push_back(c);
        }
      }
      std::vector<std::pair<gsl::not_null<ColliderComponent*>, gsl::not_null<ColliderComponent*>>> collisions;

      for (int i = 0; i < collidersVec.size(); i++)
      {
        auto c1 = collidersVec[i];

        for (int j = i + 1; j < collidersVec.size(); j++)
        {
          auto c2 = collidersVec[j];

          if (c1->Collides(*c2))
          {
            collisions.emplace_back(c1, c2);
          }
        }
      }

      for (auto col : collisions)
      {
        col.first->OnCollision(*col.second);
        col.second->OnCollision(*col.first);
      }
    }

  private:
    PhysicSystem() = default;

    ~PhysicSystem() override { Expects(colliders.empty()); }

    std::unordered_set<gsl::not_null<ColliderComponent*>> colliders;
  };
} // namespace SimpleGE