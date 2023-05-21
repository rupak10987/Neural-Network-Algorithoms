#include<iostream>
namespace fuzzy_graphic
{
//the auto car should always be placed at middle
//and the relative positon of the fuzzy_car should be changed
void visualize(float x1, float y1,float x2, float y2)
{
cleardevice();
int width=24,height=50;
rectangle(x1-width/2,y1-height/2,x1+width/2,y1+height/2);

rectangle(x2-width/2,y2-height/2,x2+width/2,y2+height/2);

swapbuffers();
}


void init_window(int width,int height)
{
    int gd = DETECT, gm;
    //initgraph(&gd, &gm, "C:\\TC\\BGI");
    initwindow(width, height);
}

}
