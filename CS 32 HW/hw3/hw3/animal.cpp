class Animal
{
 public:
    Animal (string nameInput)
    {
        m_name = nameInput;
        m_move = "walk";
    }
    
    virtual void speak() const
    {
    }
    
    virtual string moveAction() const
    {
        return m_move;
    }
    
    virtual string name() const
    {
        return m_name;
    }
    
    virtual ~Animal()
    {
    }
    
private:
    string m_name;
    string m_move;
};

class Cat : public Animal
{
public:
    Cat(string nameInput) : Animal(nameInput)
    {
    }
    
    virtual void speak() const
    {
        cout << "Meow";
    }
    
    virtual ~Cat()
    {
        cout << "Destroying " << name() << " the cat" << endl;
    }
};

class Pig : public Animal
{
public:
    Pig(string nameInput, int lbsInput) : Animal(nameInput)
    {
        lbs = lbsInput;
    }
    virtual void speak() const
    {
        if (lbs < 180)
            cout << "Oink";
        else
            cout << "Grunt";
    }
    virtual ~Pig()
    {
        cout << "Destroying " << name() << " the pig" << endl;
    }
    
private:
    int lbs;
};

class Duck : public Animal
{
public:
    Duck(string nameInput) : Animal(nameInput)
    {
    }
    virtual void speak() const
    {
        cout << "Quack";
    }
    
    virtual string moveAction() const
    {
        return "swim";
    }
    
    virtual ~Duck()
    {
        cout << "Destroying " << name() << " the duck" << endl;
    }
};


