#include<iostream>
#include<time.h>
#include<conio.h>
#include<math.h>
#include<graphics.h>
#include"Graphic_visualization.h"
int NL=30,NM=60,NS=90,ZE=120,PS=150,PM=180,PL=210;
struct uXX
{
    float uNL=0;
    float uNM=0;
    float uNS=0;
    float uZE=0;
    float uPS=0;
    float uPM=0;
    float uPL=0;
};

float delta_time;
float oldTime=0;

class CAR
{
public:
    float posX=25;
    float posY=250;
    float accelaration=0;
    float speed=210;
    float throttle=0;
    float mass=210;
    void update_car(class CAR* autocar)
    {
        this->accelaration=this->throttle/mass; //a=f/m
        this->speed=this->speed+this->accelaration*delta_time; //v=u+at
        //if(this->speed>210)
            //this->speed=210;
        this->posY-=(autocar->speed-this->speed)*delta_time;//slightly changed
    }
    void set_throttle(float Fthrottle)
    {
        this->throttle=Fthrottle;
    }
};

//func declarations
float minimum(float a,float b);
float maximum(float a,float b);
float CALC_AREA(float height);
float Apply_Rule_Base_And_Defuzzy(struct uXX* speed,struct uXX* accelaration);
float calculate_miu(float del1, float del2, float m1, float m2);
void membership(class CAR* car_FUZZY,const char* str,struct uXX* miu);


int main()
{
    fuzzy_graphic::init_window(500,500);
    //initialize the two cars one is random speed
    //another is controlled by fuzzy controller
    class CAR *car_AUTO=new CAR();

    class CAR *car_FUZZY=new CAR();
    car_FUZZY->accelaration=0;
    car_FUZZY->speed=0;
    car_FUZZY->throttle=0;
    car_FUZZY->posX=75;
    car_FUZZY->update_car(car_AUTO);

    while(true)
    {
//DELTATIME AND FPS COUNTING
        delta_time =0.001;//clock() - oldTime;
        double fps = (1.0 / delta_time) * 1000;
        oldTime = clock();


// find membership value
        struct uXX miu_speed;
        struct uXX miu_accelaration;
        membership(car_FUZZY,"speed",&miu_speed);
        membership(car_FUZZY,"accelaration",&miu_accelaration);

        float THROTTLE=Apply_Rule_Base_And_Defuzzy(&miu_speed,&miu_accelaration);
        std::cout<<THROTTLE<<std::endl;
        car_FUZZY->set_throttle(THROTTLE);
        car_FUZZY->update_car(car_AUTO);

        //graphics goes here;
        fuzzy_graphic::visualize(car_FUZZY->posX,car_FUZZY->posY,car_AUTO->posX,car_AUTO->posY);
    }

    return 0;
}

float minimum(float a,float b)
{
    if(a<=b)
        return a;
    else
        return b;
}
float maximum(float a,float b)
{
    if(a>=b)
        return a;
    else
        return b;
}
//defuzzification-based on center of gravity method
float CALC_AREA(float height)
{
    //base of the triangle a
    float a=60.0;
    //length of the triange cut at h height from base
    float b;
    b=a*(1-height);
    float area=0.5*height*(a+b);
    std::cout<<"area="<<area<<std::endl;
    return area;
}


float Apply_Rule_Base_And_Defuzzy(struct uXX* speed,struct uXX* accelaration)
{
///RULE BASE
    std::cout<<"RULE\n";
//if(speed Difference is NL) (and Acceleration is ZE) then throttle is PL
    float hPL=minimum(speed->uNL,accelaration->uZE);
    std::cout<<"min("<<speed->uNL<<","<<accelaration->uZE<<") = "<<hPL<<"\n";
//if(speed Difference is ZE) (and Acceleration is NL) then throttle is PL
    float hPL2=minimum(speed->uZE,accelaration->uNL);
    std::cout<<"min("<<speed->uZE<<","<<accelaration->uNL<<") = "<<hPL2<<"\n";
//if(speed Difference is NM) (and Acceleration is ZE) then throttle is PM
    float hPM=minimum(speed->uNM,accelaration->uZE);
    std::cout<<"min("<<speed->uNM<<","<<accelaration->uZE<<") = "<<hPM<<"\n";
//if(speed Difference is NS) (and Acceleration is PS) then throttle is PS
    float hPS=minimum(speed->uNS,accelaration->uPS);
    std::cout<<"min("<<speed->uNS<<","<<accelaration->uPS<<") = "<<hPS<<"\n";
//if(speed Difference is PS) (and Acceleration is NS) then throttle is NS
    float hNS=minimum(speed->uPS,accelaration->uNS);
    std::cout<<"min("<<speed->uPS<<","<<accelaration->uNS<<") = "<<hNS<<"\n";
//if(speed Difference is PL) (and Acceleration is ZE) then throttle is NL
    float hNL=minimum(speed->uPL,accelaration->uZE);
    std::cout<<"min("<<speed->uPL<<","<<accelaration->uZE<<") = "<<hNL<<"\n";
//if(speed Difference is ZE) (and Acceleration is NS) then throttle is PS
    float hPS2=minimum(speed->uZE,accelaration->uNS);
    std::cout<<"min("<<speed->uZE<<","<<accelaration->uNS<<") = "<<hPS2<<"\n";
//if(speed Difference is ZE) (and Acceleration is NM) then throttle is PM
    float hPM2=minimum(speed->uZE,accelaration->uNM);
    std::cout<<"min("<<speed->uZE<<","<<accelaration->uNM<<") = "<<hPM2<<"\n";

//taking the maximum of the rule that produc e same throttle
    hPL=maximum(hPL2,hPL);
    hPM=maximum(hPM2,hPM);
    hPS=maximum(hPS2,hPS);

///DEFUZZYFICATION
//sum(Ai*Xi)/sum(Ai)
float SUM_Ai_Xi=(CALC_AREA(hPL)*PL+CALC_AREA(hPM)*PM+CALC_AREA(hPS)*PS+CALC_AREA(hNS)*NS+CALC_AREA(hNL)*NL);
float SUM_AI=(CALC_AREA(hPL)+CALC_AREA(hPM)+CALC_AREA(hPS)+CALC_AREA(hNS)+CALC_AREA(hNL));
if(SUM_AI!=0)
return SUM_Ai_Xi/SUM_AI;
else
return 0;
}


float calculate_miu(float del1, float del2, float m1, float m2)
{
    float miu1,miu2;
    miu1=del1*m1;
    miu2=del2*m2;
    if(miu1>=miu2)//minimum miu stored in miu1
    {
        miu1=miu2;
    }
    if(miu1<1)
        return miu1;
    else
        return 1;
}

void membership(class CAR* car_FUZZY,const char* str,struct uXX* miu)
{
    float x;
    if(str=="speed")
        x=car_FUZZY->speed;
    else if(str=="accelaration")
        x=car_FUZZY->accelaration;

    int inp=(int)(x/30);
    switch(inp)
    {
    case 0://less than 30
    {
        miu->uNL=1.0;
        break;
    }
    case 1://31-60
    {
        miu->uNL=calculate_miu(x-(NL-30),NL+30-x,0.033,0.033);
        miu->uNM=calculate_miu(x-(NM-30),NM+30-x,0.033,0.033);
        break;
    }
    case 2:
    {
        miu->uNM=calculate_miu(x-(NM-30),NM+30-x,0.033,0.033);
        miu->uNS=calculate_miu(x-(NS-30),NS+30-x,0.033,0.033);
        break;
    }
    case 3:
    {
        miu->uNS=calculate_miu(x-(NS-30),NS+30-x,0.033,0.033);
        miu->uZE=calculate_miu(x-(ZE-30),ZE+30-x,0.033,0.033);
        break;
    }
    case 4:
    {
        miu->uZE=calculate_miu(x-(ZE-30),ZE+30-x,0.033,0.033);
        miu->uPS=calculate_miu(x-(PS-30),PS+30-x,0.033,0.033);
        break;
    }
    case 5:
    {
        miu->uPS=calculate_miu(x-(PS-30),PS+30-x,0.033,0.033);
        miu->uPM=calculate_miu(x-(PM-30),PM+30-x,0.033,0.033);
        break;
    }
    case 6:
    {
        miu->uPM=calculate_miu(x-(PM-30),PM+30-x,0.033,0.033);
        miu->uPL=calculate_miu(x-(PL-30),PL+30-x,0.033,0.033);
        break;
    }
    case 7:
    {
        miu->uPL=calculate_miu(x-(PL-30),PL+30-x,0.033,0.033);
        break;
    }
    }
}
