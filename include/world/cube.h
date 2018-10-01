#ifndef CUBE_H
#define CUBE_H

/**
 * @todo write docs
 */
class Cube
{
public:

    Cube();

    ~Cube();
    
    bool getActive();
    
    void setActive(bool active);
    
private:
    
    bool _active;

};

#endif // CUBE_H
