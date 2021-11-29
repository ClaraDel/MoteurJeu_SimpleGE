#include <simplege/simplege.h>

using json = nlohmann::json;

namespace SimpleGE
{
  struct ColliderImpl
  {
    int flag{};
    int mask{};
    Size<int> size{};
    ComponentReference<CollisionComponent> handler;
  };

  static void from_json(const json& j, ColliderComponent::Description& desc)
  {
    j.at("flag").get_to(desc.flag);
    j.at("mask").get_to(desc.mask);
    j.at("size").get_to(desc.size);
    if (j.contains("handler"))
    {
      j.at("handler").get_to(desc.handler);
    }
  }

  ColliderComponent::ColliderComponent(Entity& owner) : Component(owner), impl(std::make_unique<ColliderImpl>())
  {
    PhysicSystem::Instance().Register(this);
  }

  ColliderComponent::~ColliderComponent() { PhysicSystem::Instance().Unregister(this); }

  Component::SetupResult ColliderComponent::Setup(const nlohmann::json& descr)
  {
    return Setup(descr.get<Description>());
  }

  Component::SetupResult ColliderComponent::Setup(const Description& descr)
  {
    impl->flag = descr.flag;
    impl->mask = descr.mask;
    impl->size = descr.size;
    impl->handler = descr.handler;

    return {this, {}};
  }

  [[nodiscard]] bool ColliderComponent::Collides(const ColliderComponent& other) const
  {
    bool contact = false;
    
    if (((impl->mask & other.impl->flag) != 0) || ((other.impl->mask & impl->flag) != 0)){
      contact = true;
    }
    
    if (!contact) {
      return false;
    }

    return GetArea().Intersects(other.GetArea());
  }

  void ColliderComponent::OnCollision(const ColliderComponent& other) const
  {
    auto* handler = impl->handler.Resolve();
    if (handler != nullptr)
    {
      handler->OnCollision(other);
    }
  }

  [[nodiscard]] Area ColliderComponent::GetArea() const
  {
    gsl::not_null<PositionComponent*> posComp = Owner().GetComponent<PositionComponent>();
    const auto position = posComp->WorldPosition();
    return Area{position[0], position[1], float(impl->size.width), float(impl->size.height)};
  }


  QuadTree::QuadTree(int level, float x, float y, float width, float height)
	  {
      // ----------------- ENLEVER LE POINTEUR --------------------
      rectangle = new Rectangle(x, y, width, height);
	  }

  void QuadTree::clear(){
    try{
      objects.clear();
        for (int i = 0; i < nodes.size() ; i++){
          nodes[i]->clear();
          nodes[i]=NULL;
        }
    }catch(std::exception &e){
      std::cout << e.what() << std::endl;
    }
  }

  void QuadTree::split(){
    try{
      // ------------------- UTILISER DES PUSH_BACK ---------------------
      // ------------------- ENLEVER LES POINTEURS ----------------------
      nodes[0] = new QuadTree(level + 1, rectangle->x , rectangle->y, rectangle->width/2, rectangle->height/2);
      nodes[1] = new QuadTree(level + 1, rectangle->x + rectangle->width/2, rectangle->y, rectangle->width/2, rectangle->height/2);
      nodes[2] = new QuadTree(level + 1, rectangle->x ,  rectangle->y + rectangle->height/2, rectangle->width/2, rectangle->height/2);
      nodes[3] = new QuadTree(level + 1, rectangle->x + rectangle->width/2 , rectangle->y + rectangle->height/2, rectangle->width/2, rectangle->height/2);
    }catch(std::exception &e){
      std::cout << e.what() << std::endl;
    }
  }    

    int QuadTree::getIndex(ColliderComponent* element){
      try{
        int index =-1;
        Area elementArea = element->GetArea();
        float horizontalMidPoint = rectangle->x + rectangle->width/2;
        float verticalMidPoint = rectangle->y + rectangle->height/2;

        bool topQuadrant  = elementArea.yMin() < verticalMidPoint && elementArea.yMax() < verticalMidPoint;
        bool botQuadrant = elementArea.yMin() > verticalMidPoint && elementArea.yMax() > verticalMidPoint;

        if(elementArea.xMin() < horizontalMidPoint && elementArea.xMax() < horizontalMidPoint){
          if(topQuadrant){
            index = 0;
          }
          else if(botQuadrant){
            index=2;
          }
        }
        else if (elementArea.xMin() > horizontalMidPoint && elementArea.xMax() > horizontalMidPoint){
          if(topQuadrant){
            index = 1;
          }
          else if(botQuadrant){
            index=3;
          }
        }
        return index;
      }catch(std::exception &e){
        std::cout << e.what() << std::endl;
      }
    }
    
    void QuadTree::insert(ColliderComponent* element){
      try{
        Area elementArea = element->GetArea();

        // ----------------Cette variable n'est pas utilisée???--------------------
        Rectangle* elementRect = new Rectangle(elementArea.xMin() , elementArea.yMin() , elementArea.width() , elementArea.height());
        
        if (!nodes.empty()) //check si le quadtree a été divisé
        {
          int index = getIndex(element); //récupère lequel des quatre quadTree l'objet se trouve
          if(index != -1){
            nodes[index]->insert(element);
            return ;
          }
        }
        objects.push_back(element);

        if(objects.size() > maxComponent && level < maxLevels){ //si le quadtree contient trop d'objet
          if(!nodes.empty()){
            split();
          }
          
          int i = 0 ;
          while(i< objects.size()) {
            int index = getIndex(objects[i]);
            if(index != -1){
              nodes[index]->insert(objects[i]);
              objects.erase(objects.begin()+i);
            } else{
              i++ ;
            }
          }
        }
      }catch(std::exception &e){
        std::cout << e.what() << std::endl;
      }
    }
    
    std::vector<ColliderComponent*> QuadTree::retrieve(std::vector<ColliderComponent*>& returnObj, ColliderComponent* element){
      try{
        int index = getIndex(element);
        if (index != -1 && !nodes.empty()){
          returnObj = nodes[index]->retrieve(returnObj,element);
        }
        for(int i =0 ; i < objects.size(); i++){
          returnObj.push_back(objects[i]);
        }
        return returnObj;
      }catch(std::exception &e){
        std::cout << e.what() << std::endl;
      }
    }

} // namespace SimpleGE
