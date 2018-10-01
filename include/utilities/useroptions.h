#ifndef USEROPTIONS_H
#define USEROPTIONS_H

/**
 * @todo write docs
 */
class UserOptions
{
public:
    
    //static void initUserOptions();
    
    ~UserOptions();
    
private:
    UserOptions();

    static int _windowWidth;
    static int _windowHeight;
    static int _targetFrameRate;
    static int _drawDistance;
    
};

#endif // USEROPTIONS_H
