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
    setfillstyle(SOLID_FILL,COLOR(col.r*sigma,col.g*sigma,col.b*sigma));
    circle(a,b,r);
    floodfill(a,b,WHITE);
}

void set_view_params(int number_of_layers,int* nodes_in_layer,int* node_radius,int* node_to_node_distance)
{
int max_nodes=-100;
for(int i=0;i<number_of_layers+1;i++)
{
if(max_nodes<nodes_in_layer[i])
    max_nodes=nodes_in_layer[i];
}
int x,y,a,b,c,d,m,n;//x=node diameter,y=node_to_node_distance,a=number_of_layer+1, b=number_of_layer,c=max_node_in_any_layer,d=c-1;
a=number_of_layers+1;
b=number_of_layers;
c=max_nodes;
d=c-1;
m=750;
n=450;
y=(m*c-n*a)/(b*c-a*d);
x=(m-b*y)/a;
*node_radius=x/2;
*node_to_node_distance=y;
}

void view_node(float **Net,int layer_no,int node_num)
{
    float max_=-1000;
    for(int i=0; i<node_num; i++)
    {
        if(max_<Net[0][i])
            max_=Net[0][i];
    }
    for(int i=0; i<node_num; i++)
    {
        float sig=(Net[0][i])/max_;
        class Col c(255,255,255);
        draw_filled_circle((layer_no+1)*200,(i+1)*100,20,c,sig);

        float number=Net[0][i];
        char ch[10];
        sprintf(ch,"%f",number);
        outtextxy((layer_no+1)*200-20,(i+1)*100+20,ch);
    }

}
void view_network(int layer,float **weights,int num_of_row,int num_of_collumn)
{
    float max_weight=-1000;
    for(int i=0; i<num_of_collumn; i++)
    {
        for(int j=0; j<num_of_row; j++)
        {
            if(abs(weights[j][i])>max_weight)
                max_weight=abs(weights[j][i]);

        }
    }
    for(int i=0; i<num_of_collumn; i++)
    {
        for(int j=0; j<num_of_row; j++)
        {
            class Vec3 p((layer+1)*200,(j+1)*100,0);
            class Vec3 q((layer+2)*200,(i+1)*100,0);
            float sigma=(weights[j][i]*10)/max_weight;
            int r,g,b;
            if(sigma>=0)
            {
                r=0;
                g=255;
                b=150;
            }
            else
            {
                r=255;
                g=150;
                b=0;
            }
            class Col c(r,g,b);
            sigma=abs(sigma);
            for(int i=0; i<sigma; i++)
            {
                q.y+=1;
                p.y+=1;
                draw_line(q,p,c);
            }

            //float offsetx=(p.x+q.x)/2;
            //float offsety=(p.y+q.y)/2;
            //offsety+=10*(i-j);
            //offsetx+=10*(j-i);
            //char ch[10];
            //sprintf(ch,"%0.3f",sigma);
            //outtextxy(offsetx,offsety,ch);
        }
    }
}

void view_chromosome(char** chromosomes,int population,int chromosome_length,int gene_length,int start_x_pos,int start_y_pos)
{
float r=255,g=255,b=0;
class Col col((int)r,(int)g,(int)b);
for(int c=0;c<population;c++)
{
char ch[10];
sprintf(ch,"CHROMOSOME %d",c);
outtextxy(start_x_pos-140,start_y_pos+(c*20)-6,ch);
for(int i=0;i<chromosome_length;i++)
{
    int u;
    u=(int)chromosomes[c][i]-(int)'0';
    r=((float)u/9.0)*255;
    g=((float)u/9.0)*255;
    b=((float)u/9.0)*255;
    col.r=r;
    col.g=g;
    col.b=0;
    setfillstyle(SOLID_FILL,COLOR(col.r,col.g,col.b));
    circle(start_x_pos+(i*10),start_y_pos+(c*20),5);
    floodfill(start_x_pos+(i*10),start_y_pos+(c*20),WHITE);
}

}


}





}
