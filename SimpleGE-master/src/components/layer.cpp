#include <simplege/simplege.h>

#include "../graphics/graphicapi.h"

namespace SimpleGE
{
  static std::vector<gsl::not_null<SpriteComponent*>> ListSprites(const std::shared_ptr<Entity>& owner)
  {
    std::queue<std::shared_ptr<Entity>> queue;
    queue.push(owner);

    std::vector<gsl::not_null<SpriteComponent*>> sprites;

    while (!queue.empty())
    {
      auto node = queue.front();
      queue.pop();

      for (const auto& child : node->GetChildren())
      {
        if (child->Active())
        {
          queue.push(child);
        }

        auto* comp = child->GetComponent<SpriteComponent>();
        if (comp != nullptr)
        {
          sprites.emplace_back(comp);
        }
      }
    }

    return sprites;
  }

  void LayerComponent::Display(const Timing& timing)
  {
    auto layerSprites = ListSprites(Owner().shared_from_this());
    if (layerSprites.empty())
    {
      return;
    }

    auto spriteSheet = gsl::at(layerSprites, 0)->GetSpriteSheet();
    Ensures(spriteSheet.Ready());

    std::uint16_t offset = 0;
    std::vector<SpriteComponent::Vertex> verticesList ;
    std::vector<std::uint16_t> indexList ;
    
    for (int i=0; i<layerSprites.size(); i++){

      for (int j=0; j<4; j++){
        verticesList.push_back(layerSprites[i]->getVertices()[j]);
      }

      for (int j=0; j<6; j++){
        indexList.push_back(layerSprites[i]->getIndices()[j] + offset*4) ;
      }

      offset++;
      
    }

    std::shared_ptr<Graphic::VertexBuffer> vertexBuffer = Graphic::VertexBuffer::Create(gsl::span<const SpriteComponent::Vertex>(verticesList), Graphic::VertexBuffer::Usage::Dynamic);
    std::shared_ptr<Graphic::IndexBuffer> indexBuffer = Graphic::IndexBuffer::Create(gsl::span<const std::uint16_t>(indexList), Graphic::IndexBuffer::Usage::Dynamic);

    spriteSheet->Bind(vertexBuffer, indexBuffer);
    glDrawElements(GL_TRIANGLES, (GLsizei) indexList.size() , GL_UNSIGNED_SHORT, nullptr);
    spriteSheet->Unbind();
  }
} // namespace SimpleGE