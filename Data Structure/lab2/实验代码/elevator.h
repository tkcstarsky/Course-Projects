#include<iostream>

#define tp 1 //time piece
#define door_time 20*tp //door open and close time 
#define get_in_time 25*tp //people enter or exit time
#define test_time 40*tp //each time test whether close door autoly
#define reset_time 300*tp //time back to basic floor
#define toler_up_time 300*tp //wait upstairs
#define toler_down_time  150*tp //wait downstairs
#define elevater_speed 15*tp //each floor time

#define max_floor 13 //set building floors
#define max_people 20  //max people number

enum direction{
    up,     //elevator is up
    down    //elevator is down
} ;
enum state{
    run,    //elevator is moving
    in,     //people get in the elevator
    out,    //people get out the elevator
    opd,    //elevator is opening the door
    cld     //elevator is closing the door
} ;
//declare  elevator state

typedef struct p{
    int num;            //people number
    int arr_time;       //the time when people arrive 
    int arr_floor;      //the floor where people arrive 
    int des_floor;      //people's destenation
    int total_time;     //people total using time 
    int tol_time;       //people max waiting time
    p *next;            //next people pointer
} person;

class elevater
{
private:
    person *floors[max_floor+1];                //memory structure
public:
    direction dir;                              //elevator direction
    int people;                                 //elevator's current people number
    int cur_floor;                              //elevator current floor
    state sta;                                  //elevator current state
    elevater(int initfloor);                    //use init floor to setup elevator
    int waittime(int locfloor,int direction);   //get floorwaittime
    int findnextfloor(int *que);                //find next floor to move to
    bool getin(person *p,int T);                //people get in the floor
    int getout(int T);                          //return out people number
};

class quene
{
private:
    person* que[max_floor+1];                   //memory structure
public:
    int num[max_floor+1];                       //record each floor's people number
    quene();    
    bool insert(person *p);                     //people get in waiting queue
    person *quit(int floor,int T,int *check);   //people quit waiting queue
    int waitsum();                              //return current total people number in queue
};