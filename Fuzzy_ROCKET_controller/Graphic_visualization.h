#include<iostream>
namespace fuzzy_graphic
{
void visualize(float x1, float y1,float sp1,float a1,float t1)
{
cleardevice();
int width=24,height=60;

//first rocket
rectangle(x1-width/2,y1-height/2,x1+width/2,y1+height/2);

line(x1-width/2,y1-height/2,x1,y1-0.8*height);
line(x1+width/2,y1-height/2,x1,y1-0.8*height);

line(x1-10,y1+height/2+10,x1-15,y1+height/2+30+rand()%20);
line(x1,y1+height/2+10,x1,y1+height/2+30+rand()%20);
line(x1+10,y1+height/2+10,x1+15,y1+height/2+30+rand()%20);

char chr1[100],chr2[100],chr3[100];
sprintf(chr1,"speed=%f",sp1);
sprintf(chr2,"acceleration=%f",a1);
sprintf(chr3,"thrust=%f",t1);
outtextxy(500,10,chr1);
outtextxy(500,50,chr2);
outtextxy(500,90,chr3);

//draw road
line(10,0,10,700);
line(150,0,150,700);
for(int i=0;i<35;i++)
{
    line(10,i*20,15,i*20);
    line(145,i*20,150,i*20);
}

swapbuffers();
}


void init_window(int width,int height)
{
    int gd = DETECT, gm;
    //initgraph(&gd, &gm, "C:\\TC\\BGI");
    initwindow(width, height);
}

}
