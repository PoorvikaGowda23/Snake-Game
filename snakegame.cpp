#include<iostream>
#include<cctype>
#include<windows.h>
#include<conio.h>
#include<unistd.h>

using namespace std;

typedef struct body
{
    int x,y;                           //x and y are coordinates to represent position
    struct body *link;                 //to increase the size of snake
}body;

struct body *head;
struct body *tail;

int mapp[40][120];                     //to create the boarders of game which has 40 rows and 120 colmns

int sizeX=90;                          //length of X-axis (row)
int sizeY=20;                          //length of Y-axis (column)
int score=0;
int foodX=40;                           //food position on X-axis
int foodY=10;                          //food position on Y-axis

void gotoxy(int x,int y)
{
    COORD c={x,y};
    SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),c);
}

void create_body(int x,int y)
{
    body *temp=new struct body;         //temp=body
    temp->x=x;
    temp->y=y;
    if(!head)
    {
        head=tail=temp;
    }
    else
    {
        tail->link=temp;
        tail=temp;
    }
    tail->link=NULL;
}

// This function is used to create the game board by setting up the walls, empty spaces, and the location of the food
//that the player needs to collect.
void create_map()                       //mapp[40][120]
{
    for(int i=0;i<sizeY;i++)
    {
        for(int j=0;j<sizeX;j++)
        {
            if(i==0||i==sizeY-1||j==0||j==sizeX-1)
                mapp[i][j]=1;
            else
                mapp[i][j]=0;
        }
    }
    mapp[foodY][foodX]=4;
}

void create_food()
{
    mapp[foodY][foodX]=0;
    do
    {
        foodX=rand()%(sizeX-1)+1;
        foodY=rand()%(sizeY-1)+1;
    }
    while(mapp[foodY][foodX]!=0);
    mapp[foodY][foodX]=4;
    gotoxy(foodX,foodY);
    cout<<"*";                         //after eating food this will arise
}

//Setting the map when we enter play(it views the outline of map)
void view_map()
{
    for(int i=0;i<sizeY;i++)
    {
        for(int j=0;j<sizeX;j++)
        {
            if(mapp[i][j]==1)
                cout<<"/";
            else if(mapp[i][j]==4)
                 cout<<"*";
            else
                cout<<" ";              //space in btw the boarder
        }
        puts(" ");                      //for purpose of alignment
    }
}

//Movement of snake and increase of score
int run(int x,int y)
{
    body *curr = head;
	int bodyX;
	int bodyY;
	int tempX;
	int tempY;
	while (curr)
    {
		if (curr==head)
        {
			if(score>2)
			{
				body *vcurr=head->link->link;
				while(vcurr)
				{
					if(vcurr->x==head->x && vcurr->y==head->y)
					{
                        return 0;       //when it collides with itself
					}
					vcurr=vcurr->link;
				}
			}

			mapp[curr->y][curr->x] = 0;
			if(head->x + x < 1 || head->x + x > sizeX - 2 || head->y + y < 1 || head->y + y > sizeY - 2)
			{
				return 0;               //when collides with the wall
			}
			if (head->x == foodX && head->y == foodY)
			{
				int newX = tail->x ;
				int newY = tail->y ;
				create_body(newX, newY);
				score++;
				create_food();
			}

			gotoxy(curr->x, curr->y);
			cout<<" ";
			bodyX = curr->x;
			bodyY = curr->y;
			curr->x = curr->x+x ;         //The snake won't move
			curr->y = curr->y+y ;
        }
//Updates the position of each Body object in the linked list that follows the head of the snake, based on
//the position of the Body object that came before it. This allows the snake to move smoothly and
//appear as a continuous, connected line on the screen.
        else
        {

			mapp[curr->y][curr->x] = 0;
			gotoxy(curr->x, curr->y);
			cout<<" ";
			tempX = bodyX;
			tempY = bodyY;

			bodyX = curr->x;
			bodyY = curr->y;

			curr->x = tempX;
			curr->y = tempY;
		}

		mapp[curr->y][curr->x] = 10;
		gotoxy(curr->x, curr->y);
		cout<<"o";                      //snake body
		curr = curr->link;
	}
    return 1;
}

void play()
{
	char go;
	int direc=4;
	int move=1;                         //to move in all 4 directions using coordinates
	int speed;
	score=0;

	while(move)
    {
		if(score>2)
            speed=150;
		else if(score>4)
            speed=100;
		else if(score<6)
            speed=200;
		else
            speed=50;

		if (kbhit())                     //checks if a key has been pressed in the keyboard buffer
		{
			switch(tolower(go=getch()))
			{
                case 'w':                //not to interchange head and tail
                  if (direc!= 3)
                      direc=1;
                    if(direc==1)
                        speed/=2;
                    break;
                case 'a':
                    if (direc!= 4)
                        direc=2;
                    if(direc==2)
                        speed/=2;
                    break;
                case 's':
                    if (direc!=1)
                        direc=3;
                    if(direc==3)
                        speed/=2;
                    break;
                case 'd':
                   if (direc!= 2)
                        direc= 4;
                    if(direc==4)
                        speed/=2;
                    break;
			}
		}

		switch(direc)
		{
            case 1:
                move=run(0,-1);
                break;
            case 2:
                move=run(-1,0);
                break;
            case 3:
                move=run(0,1);
                break;
            case 4:
                move=run(1,0);
                break;
		}

		gotoxy(1,sizeY+1);
		cout<<"Score : "<<score;

		Sleep(speed);
	}
}

void popall()       //deletes all nodes in a linked list of type Body
{
	while(head)
    {
		body *curr;
		curr=head;
		head=head->link;
		free(curr);
	}
}
void rules()
{
    system("cls");
    cout<<endl;
    cout<<"\t\t ------------------ RULES -----------------\n"<<endl;
    cout<<"\t\t   1.Keys to be Pressed"<<endl;
    cout<<"   \t\t\tW = up"<<endl<<"   \t\t\tA = left"<<endl<< "   \t\t\tD = right"<<endl<<"   \t\t\tS = down"<<endl;
    cout<<"\t\t   2.If the Snake eats the food it will increase its size."<<endl;
    cout<<"\t\t   3.The game will get over when the snake touches the border or itself."<<endl;
    cout<<"\t\t   4.The score will be displayed below."<<endl;
    cout<<"\t\t   5.At starting of game the score will be 0 and speed of snake will be 50."<<endl;
    cout<<"\t\t   6.When score is greater than 4 then speed of snake gradually increases."<<endl;

    cout<<endl;
}

int main()
{
    int choice;
    char another='y';
    cout<<"\n\n\n\n\n\n\n\t\t\t___________________________________________________________"<<endl;
    cout<<"\n\t\t\t\t\t\t SNAKE GAME"<<endl;
    cout<<"\t\t\t\t By C.Poorvika - 22PD10 and Chaithanya.N - 22PD09"<<endl<<endl;
    cout<<"\t\t\t___________________________________________________________"<<endl;
    cout<<"\n\n\t\t\t\t\t\tloading";
    sleep(1);
    cout<<".";
    sleep(1);
    cout<<".";
    sleep(1);
    cout<<".";
    sleep(1);
    sleep(2);

    system("cls");
    while(another=='y'||another=='Y')
    {
        system("cls");
        cout<<"\t\t --------------- Main Menu ---------------\n"<<endl;
        cout<<"\t\t\t\t[1].Rules\n"<<"\t\t\t\t[2].Play\n"<<"\t\t\t\t[3].Exit"<<endl;
        cout<<"\nEnter Your Choice: ";
        cin>>choice;

        switch(choice)
        {
        case 1:
            rules();
            break;
        case 2:
            system("cls");
			create_body(4,4);
			create_map();
			view_map();
			play();
			gotoxy(1, sizeY + 2);
			cout<<"Game Over"<<endl;

			popall();
			break;
        case 3:
            cout<<"\n\t\t --------------- Thank you for playing ---------------"<<endl;
            return 0;
            break;
        default:
            cout<<"Invalid Choice"<<endl;
            break;
        }
        cout<<"Do you want to Continue(Y/N)..?";
        cin>>another;
        cout<<endl;
    }
}

