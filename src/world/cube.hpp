#ifndef CUBE_H
#define CUBE_H

class Cube {
public:
    
    static const int CUBE_SIZE = 1;
    
    Cube();

    ~Cube();
    
    bool getActive();
    
    void setActive(bool active);
    
private:
    
    bool _active;

};

#endif // CUBE_H
