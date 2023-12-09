
// product
class Door 
{
};

// concrete products
class BlueDoor : public Door
{
};


// factory
// Door, Roof and Wall representa a family of related objects.
class HouseFactory
{
public:
    virtual std::unique_ptr<Door> createDoor() const = 0;
    virtual std::unique_ptr<Roof> createRoof() const = 0;
    virtual std::unique_ptr<Wall> createWall() const = 0;
    virtual ~HouseFactory() = 0;
};

// concrete factories
class BlueHouseFactory : public HouseFactory
{
public:
    std::unique_ptr<Door> createDoor() const override; // this returns a blue door
    std::unique_ptr<Roof> createRoof() const override; // this returns a blue roof
    std::unique_ptr<Wall> createWall() const override; // this returns a blue wall
};

class RedHouseFactory : public HouseFactory
{
public:
    std::unique_ptr<Door> createDoor() const override; // this returns a red door
    std::unique_ptr<Roof> createRoof() const override; // this returns a red roof
    std::unique_ptr<Wall> createWall() const override; // this returns a red wall
};

class YellowHouseFactory : public HouseFactory
{
public:
    std::unique_ptr<Door> createDoor() const override; // this returns a yellow door
    std::unique_ptr<Roof> createRoof() const override; // this returns a yellow roof
    std::unique_ptr<Wall> createWall() const override; // this returns a yellow wall
};
