#ifndef VTASK_HPP
#define VTASK_HPP

class VTask
{
public:
    VTask()
    {

    }
    virtual ~VTask()
    {

    }
    virtual void execute() = 0;
};

#endif // VTASK_HPP
