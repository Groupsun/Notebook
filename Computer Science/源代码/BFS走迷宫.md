```C++
#include <iostream>
#include <queue>
#include <cstring>
using namespace std;
#define Max_Size 10001
char map[Max_Size][Max_Size];
bool vis[Max_Size][Max_Size];
int dis[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
int row,col;

struct step
{
        int x,y;
        int t;
}now,temp;
int bfs()
{
        now.t=0;
        queue<step> q;
        q.push(now);
        while(!q.empty())
        {
                 temp=q.front();
                 q.pop();
                 if(map[temp.x][temp.y]=='E')
                 {
                          return temp.t;
                  }
                  for(int i=0;i<4;i++)
                  {
                           int dx=temp.x+dis[i][0];
                           int dy=temp.y+dis[i][1];
                           if(dx>=0&&dx<row&&dy>=0&&dy<col&&map[dx][dy]!='#'&&!vis[dx][dy])
                            {
                                     now.x=dx;
                                     now.y=dy;
                                     now.t=temp.t+1;
                                     vis[now.x][now.y]=true;
                                     q.push(now);
                             }
                   }
         }
}
int main()
{
        cin>>row>>col;
        memset(map,'#',sizeof(map));
        memset(vis,false,sizeof(vis));
        for(int i=0;i<row;i++)
        {
                 for(int j=0;j<col;j++)
                 {
                          cin>>map[i][j];
                          if(map[i][j]=='S')
                          {
                                  now.x=i;
                                  now.y=j;
                           }
                  }
         }
         int ans=bfs();
         cout<<ans<<endl;
         return 0;
} 

```