#include<stdio.h>
namespace NEURAL_GRAPHIC
{
  class Vec3
{
public:
    double x;
    double y;
    double z;
    Vec3(double i,double j,double k)
    {
        this->x=i;
        this->y=j;
        this->z=k;

    };
};
class Col
{
public:
    int r;
    int g;
    int b;
    Col(int r,int g,int b)
    {
        this->r=r;
        this->g=g;
        this->b=b;
    };
};

void draw_line(class Vec3 P,class Vec3 P1,class Col col)
{
    double DX=abs(P1.x-P.x);
    double DY=abs(P1.y-P.y);
    if(DX>DY)//horizontal
    {
        if(P.x>P1.x)//checking if P is smaller than p1 always goes from low to high
        {
            draw_line(P1,P,col);
        }
        else
        {
            double m=(double)(P1.y-P.y)/(double)(P1.x-P.x);// calculating slope
            double yn=P.y;
            for(int i=P.x; i<=P1.x; i++)
            {
                yn+=m;
                putpixel(i,yn,COLOR(col.r,col.g,col.b));
            }
        }
    }
    else//vertical
    {
        if(P.y>P1.y)//checking if P is smaller than p1 always goes from low to high
        {
            draw_line(P1,P,col);
        }
        else
        {
            double m=(double)(P1.x-P.x)/(double)(P1.y-P.y);//calculating slope
            double xn=P.x;
            for(int i=P.y; i<=P1.y; i++)
            {
                xn+=m;
                putpixel(xn,i,COLOR(col.r,col.g,col.b));
            }
        }
    }
}
void draw_filled_circle(int a,int b,int r,class Col col,float sigma)
{
    float x1=0;
    float x2=0;
    float y=b-r;
    circle(a,b,r);
    while(y<=b+r)
    {
        float c=pow(r,2)-pow(a,2)-pow(y,2)+2*b*y-pow(b,2);
        c=-c;
        x1=(2*a+pow((4*pow(a,2))-4*c,0.5))/2.0;
        x2=(2*a-pow((4*pow(a,2))-4*c,0.5))/2.0;
        class Vec3 p(x1,y,0);
        class Vec3 q(x2,y,0);
        draw_line(p,q,col);
        y+=10*(1.1-sigma);
    }

}
void view_node(float **Net,int layer_no,int node_num)
{
    //int number=10;
    //std::string s = std::to_string(number);
    //char const *pchar = s.c_str();

float max_=-1000;
for(int i=0; i<node_num; i++)
    {
        if(max_<Net[0][i])
            max_=Net[0][i];
    }
    for(int i=0; i<node_num; i++)
    {
        float sig=Net[0][i]/max_;
        class Col c(255,255,255);
        draw_filled_circle((layer_no+1)*200,(i+1)*100,20,c,sig);///////////

    float number=Net[0][i];
    char ch[10];
    sprintf(ch,"%f",number);
    outtextxy((layer_no+1)*200-20,(i+1)*100+20,ch);
    }

}
void view_network(int layer,float **weights,int num_of_row,int num_of_collumn)
{

    for(int i=0; i<num_of_collumn; i++)
    {
        for(int j=0; j<num_of_row; j++)
        {
            class Vec3 p((layer+1)*200,(j+1)*100,0);
            class Vec3 q((layer+2)*200,(i+1)*100,0);
            float sigma=weights[j][i];
            int r=255*1;
            int g=255*1;
            int b=255*1;
            class Col c(r,g,b);
            draw_line(q,p,c);
            float offsetx=(p.x+q.x)/2;
            float offsety=(p.y+q.y)/2;
            offsety+=10*(i-j);
            offsetx+=10*(j-i);
            char ch[10];
            sprintf(ch,"%0.3f",sigma);
            outtextxy(offsetx,offsety,ch);
        }
    }
}
}
