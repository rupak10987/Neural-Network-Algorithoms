#include<iostream>
namespace fuzzy_graphic
{
//the auto car should always be placed at middle
//and the relative positon of the fuzzy_car should be changed
void visualize(float x1, float y1,float x2, float y2,float sp1,float a1,float t1,float sp2)
{
cleardevice();
int width=24,height=50;

//first car
rectangle(x1-width/2,y1-height/2,x1+width/2,y1+height/2);
char chr[100];
sprintf(chr,"speed=%f,\nacceleration=%f\nthrust=%f",sp1,a1,t1);
outtextxy(x1,y1,chr);

//2nd car
rectangle(x2-width/2,y2-height/2,x2+width/2,y2+height/2);
char chr1[100];
sprintf(chr1,"speed=%f,\nacceleration=%f\nthrust=%f",sp2,0,0);
outtextxy(x2,y2,chr1);

swapbuffers();
}


void init_window(int width,int height)
{
    int gd = DETECT, gm;
    //initgraph(&gd, &gm, "C:\\TC\\BGI");
    initwindow(width, height);
}

}
