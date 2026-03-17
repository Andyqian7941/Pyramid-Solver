#include <bits/stdc++.h>
using namespace std;
struct block{
    vector<pair<int,int>> v;
    block rotate(){
        block ret;
        for(auto [x,y]:v)
        ret.v.push_back({y,-x});
        int mx=0,my=0;
        for(auto [x,y]:ret.v)
            mx=min(mx,x),my=min(my,y);
        for(auto &[x,y]:ret.v)
            x-=mx,y-=my;
        return ret;
    }
    block flip(){
        block ret;
        for(auto [x,y]:v)
        ret.v.push_back({-x,y});
        int mx=0,my=0;
        for(auto [x,y]:ret.v)
            mx=min(mx,x),my=min(my,y);
        for(auto &[x,y]:ret.v)
            x-=mx,y-=my;
        return ret;
    }
    operator __int128(){
        __int128 ret=0;
        for(auto [x,y]:v){
            ret|=__int128(1)<<(x*10+y);
        }
        return ret;
    }
}a[]{{{{0,0},{0,1},{1,0},{1,1},{2,0}}},{{{2,0},{1,0},{1,1},{0,1},{0,2}}},{{{0,0},{1,0},{2,0},{2,1},{2,2}}},{{{0,0},{1,0},{0,1}}},{{{0,0},{1,0},{2,0},{2,1}}},{{{1,0},{1,1},{0,1},{2,1},{1,2}}},{{{0,0},{1,0},{0,1},{1,1}}},{{{0,0},{1,0},{0,1},{0,2},{1,2}}},{{{0,0},{1,0},{1,1},{1,2},{1,3}}},{{{0,0},{0,1},{0,2},{0,3}}},{{{0,0},{0,1},{0,2},{0,3},{1,2}}},{{{0,1},{1,1},{2,1},{2,0},{3,0}}}};
vector<__int128> v[12];
__int128 all;
unordered_map<__int128,int> f[110][1<<12];
int main(){
    auto st=clock();
    for(int i=0;i<10;i++)
    for(int j=0;j+i<10;j++)
    all|=__int128(1)<<(i*10+j);
    for(int i=0;i<12;i++){
        v[i].push_back(a[i]);
        v[i].push_back(a[i].rotate());
        v[i].push_back(a[i].rotate().rotate());
        v[i].push_back(a[i].rotate().rotate().rotate());
        v[i].push_back(a[i].flip());
        v[i].push_back(a[i].flip().rotate());
        v[i].push_back(a[i].flip().rotate().rotate());
        v[i].push_back(a[i].flip().rotate().rotate().rotate());
        sort(v[i].begin(),v[i].end()),v[i].resize(unique(v[i].begin(),v[i].end())-v[i].begin());
    }
    f[0][0][0]=1;
    __int128 cur=0;
    for(int i=0;i<100;i++){
        for(int j=0;j<1<<12;j++){
            for(int k=0;k<12;k++){
                if(j>>k&1)continue;
                int j1=1<<k|j;
                for(__int128 x:v[k]){
                    int mx=0;
                    for(int l=0;l<100;l++)
                        if(x>>l&1)mx=max(l%10,mx);
                    for(int l=0;l<100&&x<<l>=0;l++){
                        if(l%10+mx>=10)continue;
                        if(~all&(x<<l))continue;
                        if(~(x<<l)>>i&1)continue;
                        if(x<<l&cur)continue;
                        for(auto[key,val]:f[i][j]){
                            if(x<<l&key)continue;
                            __int128 key1=x<<l|key;
                            int i1=i;
                            while(i1<100&&(~all|key1)>>i1&1)i1++;
                            f[i1][j1][key1]+=val;
                        }
                    }
                }
            }
        }
        if(all>>i&1)cur|=__int128(1)<<i;
    }
    cout<<f[100][(1<<12)-1][all]<<endl;
    cerr<<1.*(clock()-st)/CLOCKS_PER_SEC;
}