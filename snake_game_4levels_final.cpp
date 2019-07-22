#include<iostream>
#include<cstdlib>
#include<vector>
#include<ctime>
#include<queue>
#include<list>
#include<fstream>
#include<Windows.h>   //Sleep(500) ...500 milliseconds delay
using namespace std;
#define N 30
#define w 72
#define s 80
#define a 75
#define d 77
//STATIC POISON ADDED
class SnakeGame
{
   public:
   int i,j,score,lastr,lastc,stage,score_and_level_check,snake_speed,player_count;
   char input;
   bool eat;
   SnakeGame(){score=0;
   stage=1;
   player_count=1;
   snake_speed=200;
   score_and_level_check=0;
   input='d';
   }	
   char matrix[N][N];
   int headr,headc,fruitr,fruitc,poisonr,poisonc; 
   vector<int> vr,vc;
   queue<int> qpr,qpc;
   void initialize();
   void display();  
   void takeInputs();
   char continousMove(char);
   void bodyMove(char );
   void randomPosition(int *,int *);
   void addPoison();
   void removePoison();
   void checkLevel();
   void clearScreen();
   void levelFromFile();
   void initializeFruit();
   void initializeBody();
   void setLevelAccordingToStage();
   void increaseSnakeLength();
   void secondPlayer();
   void headMove(int,int,char);	    // left right up down
};

HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);

void SnakeGame::initializeFruit()
{
	int fr=rand()%23;
    int fc=rand()%23;
  	while(matrix[fr][fc]==(char)219|| matrix[fr][fc]==(char)233 || matrix[fr][fc]==(char)248 || matrix[fr][fc]=='p' )    // fr==r || fc==c
  	{
        randomPosition(&fr,&fc);
       }		
   	matrix[fr][fc]='f';	
}
void SnakeGame::initializeBody()
{
	 matrix[vr.front()][vc.front()]=(char)233; 
	matrix[vr.front()][vc.front()-1]=(char)248;
	vr.push_back(vr.front()); vc.push_back(vc.front()-1);
	matrix[vr.front()][vc.front()-2]=(char)248;	
	vr.push_back(vr.front()); vc.push_back(vc.front()-2);	
}

void SnakeGame::levelFromFile()
{
	system("cls");
	vr.clear();
	vc.clear();	
	int r,c;
	initialize();	
	fstream fin;
	fin.open("snake.txt",ios::in);
	
	if (fin.is_open())
	{
		while(!fin.eof())
		{
			fin>>r;
			fin>>c;
			matrix[r][c]=char(219);
		}
	}	
	initializeFruit();
    takeInputs();   
    display();	
}
void SnakeGame::clearScreen()
{
    COORD top_left= {0, 0};
	SetConsoleCursorPosition(hout,top_left);
}

void SnakeGame::setLevelAccordingToStage()
{
		
	   if(stage==4 && eat==true && score==30)
		{
			snake_speed=100;
			levelFromFile();
			removePoison();
		}			   
	
	 if(stage==3 && eat==true)
		{
			snake_speed=150;
			removePoison();
			addPoison();
		}		
	 if(stage==2 && eat==true)
		{
			removePoison();
			addPoison();
		}		
		if(stage==1 && score==20)
		{		
		system("cls");
	 
		stage++;        
		addPoison();
		}	
		clearScreen();
		display();
	
}
void SnakeGame::checkLevel()
{	
    if(score_and_level_check/10==1)
    {
    	score_and_level_check=0;
    	stage++;
    }
}
void SnakeGame::removePoison()
{
	while(qpr.size()>0)
	{
		matrix[qpr.front()][qpc.front()]=' ';
		qpr.pop();
		qpc.pop();
	}	
}

void SnakeGame::addPoison()
{
	int pois=3;
	while(pois>0)
	{
		poisonr=rand()%23;
		poisonc=rand()%23;		
		while(matrix[poisonr][poisonc]==(char)219 || matrix[poisonr][poisonc]=='f' || matrix[poisonr][poisonc]==(char)233 || matrix[poisonr][poisonc]==(char)248)
		{
			randomPosition(&poisonr,&poisonc);
		}
		qpr.push(poisonr);
		qpc.push(poisonc);
		matrix[poisonr][poisonc]='p';
		pois--;
	}	
}

void SnakeGame::increaseSnakeLength()
{
	vr.push_back(lastr);
	vc.push_back(lastc);
	matrix[lastr][lastc]=(char)248;
}
void SnakeGame::headMove(int xr,int xc,char ch)
{
	int r,c,x;
	if(ch=='w')
	{ r=xr-1;               // up
	  c=xc; }
	
	if(ch=='s')
	{ r=xr+1;               //down
	  c=xc; }
	
	if(ch=='a')
	{ r=xr;                //left
	  c=xc-1;}
	  
	if(ch=='d')
	{ r=xr;                 //right
	  c=xc+1;}  

	if(matrix[r][c]!=(char)248 && matrix[r][c]!=(char)219)
	 {	  
	     if(matrix[r][c]=='p')
	     {
	        Beep(1800,50);
	     	/*cout<<"Game Over!!"<<endl;
	     	if (player_count==1)
	     	{
			  player_count++;
			  secondPlayer();
	        }*/ 
	     	exit(0);
		 }	
	     if(matrix[r][c]=='f')
		 {
		    Beep(700,50);
		    initializeFruit();
		    score+=5; 
			score_and_level_check+=5;  
			increaseSnakeLength(); 
			eat=true;  
		 }	
		matrix[r][c]=(char)233;
	    vr.insert(vr.begin(),r);vc.insert(vc.begin(),c);
		clearScreen();
		display();     		     	
	}
	else
	{
	    Beep(900,50);
		cout<<"lost"<<endl;
		cout<<score<<endl;
		exit(0);
	}
	checkLevel();
	setLevelAccordingToStage();	
}

void SnakeGame::randomPosition(int *r,int *c)
{
	*r=rand()%23;
	*c=rand()%23;	
}
void SnakeGame::bodyMove(char ch)
{
  	int size=vr.size();
	int r,c,x;
	r=vr.front();
	c=vc.front();
	
	while(size-1>0)
	{
		matrix[vr.front()][vc.front()]=(char)248;
		vr.push_back(vr.front()); vc.push_back(vc.front());
		vr.erase(vr.begin()); vc.erase(vc.begin());
		size--;		
	}
	matrix[vr.front()][vc.front()]=' ';
	lastr=vr.front(); lastc=vc.front();
    vr.erase(vr.begin()); vc.erase(vc.begin());
    headMove(r,c,ch);
}
char SnakeGame::continousMove(char input)
{
	if(GetAsyncKeyState('W')&1)
	{
		input='w';
	}
	else if(GetAsyncKeyState('A')&1)
	{
		input='a';
	}
	else if(GetAsyncKeyState('D')&1)
	{
		input='d';
	}
	else if(GetAsyncKeyState('S')&1)
	{
		input='s';
	}
	else if(GetAsyncKeyState('P')&1)
	{		
		system("pause");
	}
	return input;
}


void SnakeGame::takeInputs()
{
   char ch;
   while(1) 
   {
	  input=continousMove(input);
	  Sleep(snake_speed);
	  eat=false;
	  
   	  if(input=='a')
   	  {
   	    bodyMove(input);  //left
	  }
	  else if(input=='d')
	  {
	  	bodyMove(input);    //right
	  }
   	  else if(input=='s')
   	  {
   	     bodyMove(input); 	//down
	  }
	  else if(input=='w')
	  {
	  	 bodyMove(input);   //up
	  }
	  else
	  {
	  	continue;
			 }   	
   }	
}
void SnakeGame::display()
{	
    cout<<"Level = "<<stage<<" Player = "<<player_count<<" Score = "<<score<<endl;
   for (int i=0; i<N; i++)
   {
    for (int j=0; j<N; j++)
  	 {
         cout<<matrix[i][j];  		
	  }
	  cout<<endl;
  }	
}

void SnakeGame::initialize()
{	
	srand(time(0));
   	headr=rand()%23;
	headc=rand()%23;
	while(headr<3 || headc<3 || matrix[headr][headc]=='X')   
    {  
        randomPosition(&headr,&headc);	 	   
    }    
  
    vr.push_back(headr);
	vc.push_back(headc);		
  
  for (int i=0; i<N; i++)
  {
    for (int j=0; j<N; j++)
  	 {         	
		 if (i==0 || j==0 || i==N-1||j==N-1)
		 {
		 	matrix[i][j]=(char)219;
		 }
		 else
		 {
		 	 matrix[i][j]=' '; 
			 }	
	  }
  }
}

int main()
{
     SetConsoleTitle("Snake Game");      
   CONSOLE_CURSOR_INFO ci;
   ci.dwSize=100;
   ci.bVisible=0;
   SnakeGame obj;
   system("Color 1E");
   cout<<"\t Instructions\n"; 
   cout<<"1)Use a,s,w,d for movement left,down,up,right respectively\n2)Eat fruits(f) each fruits is worth 5 points\n3)If the snake Eats poison it dies and game over!! \n4)Level changes after every 20 points\n5)Press p to pause the game..and any key except p to resume";
   Sleep(6000);
   int n=3;
   while(n)
   {   	 
     system("cls");
  	 cout<<"Game starts in "<<n<<" seconds";
  	 Sleep(1000);
     n--;
   }
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 100;                          //INDEX OF THE FONT IN THE SYSTEM FONT TABLE
    cfi.dwFontSize.X = 20;                  // Width of each character in the font
    cfi.dwFontSize.Y = 20;                 // Height
    cfi.FontFamily = FF_DONTCARE;          // Arial or Courier etc 
    cfi.FontWeight = 500;
    std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &cfi);   // if parameter is true the font info is set font  max window size...if is false then current window size.
   
   cout<<"\nLevel "<<obj.stage<<endl;
   Sleep(1000); 
   SetConsoleCursorInfo(hout,&ci);
    obj.initialize();
    obj.initializeBody();
    obj.initializeFruit();
    system("cls");
    obj.takeInputs();      
    obj.display();
      
    return 0;
}
