#include "definitions.h"
#include "control.h"

int main()
{
    cout<<"Enter the desired functionality:\n1 => Visualization of all types of nets\n2 => Evaluation of all types of nets"<<endl;
    int command;
    cin>>command;

    if(command == 1)
    {
        visualize();
    }
    else
    {
        evaluate();
    }


    return 0;
}
