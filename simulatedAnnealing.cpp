#include<bits/stdc++.h>

using namespace std;

double TEMP_HIGH=5000;
double GLOBAL_MINIMUM=0;
double TEMP_LOW=2;
double TEMP_DECREASE_FACTOR=0.99;
double TEMP_DECREASE_LOOPS=4;
int n;

struct board
{
    vector<vector<int>> queen;//actual board
    vector<int> state;//current state of board in array[row]=column format
};

void printBoard(board &b)//prints the current chess board
{
    cout<<endl;
    for (int row=0;row<n;row++)
    {
        for(int column=0;column<n;column++)
            cout<<b.queen[row][column]<<" ";
        cout<<endl;
    }
    cout<<endl;
}
//O(n2)

int randomLessThan(int n)//returns random number less than n
{
    // srand(time(NULL));
    return rand()%n;
} 
//O(1)

bool isValid(int row,int column)//checks if a particular position is inside the board
{
    return (0<=row && row<n && 0<=column && column<n); 
}
//O(1)

int conflictCountAtPos(board &B,int row,int column)//returns no of conflicts at current position
{
    int conflicts=0;
    vector<pair<int,int>> jump;
    for (int a:{-1,0,1})//initialize the different kinds of jumps
    {
        for (int b:{-1,0,1})
        {
            if (a==0 && b==0) continue;
            jump.push_back(make_pair(a,b));
        }
    }

    for (auto j:jump)
    {
        int x=row+j.first,y=column+j.second;
        while (isValid(x,y))
        {
            if (B.queen[x][y]==1) conflicts++;
            x=x+j.first;
            y=y+j.second;
        }
    }

    return conflicts;
}
//O(n)

int heuristic(board &b)//returns heuristic of current board given a board
{
    int conflicts=0;
    for (int row=0;row<n;row++)
    {
        conflicts+=conflictCountAtPos(b,row,b.state[row]);
    }

    return conflicts/2;//because we counted every pair twice
}
//O(n2)

bool returnTrueWithProbability(bool probability)//A fair function that returns true with probability p
{
    return rand() < probability * (RAND_MAX+1.0);
}

board randomBoard()//Returns a board with a random config
{
    board b;
    b.queen=vector<vector<int>> (n,vector<int> (n,0));
    b.state=vector<int> (n,-1);
    for (int row=0;row<n;row++)
    {
        int column=randomLessThan(n);
        b.queen[row][column]=1;
        b.state[row]=column;
    }
    return b;
}
//O(n)

board randomNeighbour(board current)
{
    board b=current;
    int row=randomLessThan(n);
    int newColumn=randomLessThan(n);
    int prevColumn=b.state[row];//queen was in this column
    b.queen[row][prevColumn]=0;//removed queen from the previous position
    b.queen[row][newColumn]=1;//placing queen in newColumn
    b.state[row]=newColumn;//correcting state
    return b;
}
//O(1)

double acceptanceProbability(double increase,double temp)
{
    double power=increase/temp * -1.0;
    return exp(power);
}

int main()
{
    srand(time(NULL));
    cin>>n;
    clock_t tStart = clock();
    double currentTemp=TEMP_HIGH;
    board currentState=randomBoard();
    int loops=0;
    cout<<"Initial State Heuristic : "<<heuristic(currentState)<<endl;
    int totalLoops=0;

    while(currentTemp>TEMP_LOW && heuristic(currentState)!=GLOBAL_MINIMUM)
    {
        totalLoops++;
        board neighbour=randomNeighbour(currentState);
        int currentHeuristic=heuristic(currentState);
        int neighbourHeuristic=heuristic(neighbour);

        if (neighbourHeuristic<currentHeuristic)//better neighbour
            currentState=neighbour;
        else//worse neighbour 
        {
            int p=acceptanceProbability(neighbourHeuristic-currentHeuristic,currentTemp);
            if (returnTrueWithProbability(p)==true)
                currentState=neighbour;
        }

        loops++;
        if (loops==TEMP_DECREASE_LOOPS)
        {
            currentTemp=currentTemp*TEMP_DECREASE_FACTOR;
            loops=0;
        }
    }

    // cout<<"\nFinal Board State";
    // printBoard(currentState);
    cout<<"Final State Heuristic : "<<heuristic(currentState)<<endl;
    cout<<"Total loops "<<totalLoops<<endl;
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    return 0;
}

//change
