#include <iostream>
using namespace std;

/*
 规则：
 1)每次交易都必须遵守购买->出售，然后再购买->出售
 2)一天只能进行一次操作（购买or出售）
 3)若没特殊指出，没有冷却时间，既出售后隔天就可以再次购买
 */

/*没有冷却时间*/

//一次交易
int tradingOnlyOnce(vector<int>prices, int n){
    int currentMinNum = prices[1] , maxProfit = -1;
    for (int i = 2 ; i <= n ; ++ i) {
        if (currentMinNum > prices[i]) {
            currentMinNum = prices[i];
        }else{
            maxProfit = maxProfit > prices[i] - currentMinNum ? maxProfit : prices[i] - currentMinNum;
        }
    }
    return maxProfit;
}

//允许多次交易
int tradingMultiple(vector<int> prices, int n){
    vector<int>sub(n,0);
    int sum = 0 ;
    for (int i = 1 ; i < n ; ++ i) {
        if (prices[i] < prices[i+1]) {
            sum += prices[i+1] - prices[i];
        }
    }
    return sum;
}

//最多两次交易
int tradingOnlyTwice(vector<int> prices, int n) {
    /*
     buy[i]:第i个物品有买和不买两种操作，不买则buy[i]=buy[i-1]；买则buy[i]=-prices[i]；因此buy[i]表示在i个物体前（包括i）第一次购买所得到的最大收益
     同理，sale[i]表示在第i个物体前（包括i）第一个出售所得到的最大收益。那么buy_2则表示第二次购买所得到的最大收益
    */
    vector<int>buy(n+1,0), sale(n+1,0), buy_2(n+1,0), sale_2(n+1,0);
    buy[1] = buy_2[1] = -prices[1];
    for (int i = 2 ; i <= n; ++ i) {
        buy[i] = max(-prices[i], buy[i-1]);
        sale[i] = max(buy[i-1]+prices[i], sale[i-1]);
        buy_2[i] = max(sale[i-1]-prices[i], buy_2[i-1]);//买和卖的最大值
        sale_2[i] = max(buy_2[i-1]+prices[i], sale_2[i-1]);
    }
    return sale_2[n];
}

//最多k次交易
int tradingWithKtimes(vector<int>prices, int n, int k){
    /*
    类似于交易最大交易两次，将buy和buy_2合为一个二维数组；buy[i][j]表示在第i次交易中，购买第j个物体的最大收益
    */
    vector<vector<int>>buy(k+1,vector<int>(n+1,0)), sale(k+1,vector<int>(n+1,0));
    for(int i = 1 ; i <= k ; ++ i){
        buy[i][1] = -prices[1];
    }
    for (int j = 2 ; j <= n ; ++ j) {
        for (int i = 1 ; i <= k ; ++ i) {
            buy[i][j] = max(buy[i][j-1], sale[i-1][j-1]-prices[j]);
            sale[i][j] = max(buy[i][j-1]+prices[j], sale[i][j-1]);
        }
    }
    return sale[k][n];
}

/*有冷却时间*/

int tradingWithCoolDown(vector<int>prices, int n, int k, int coolDown){
    /*
     可以这么认为：没有冷却时间是有冷却时间的特殊情况，即cooldown=0
     */
    vector<vector<int>>buy(k+1,vector<int>(n+1,0)), sale(k+1,vector<int>(n+1,0));
    for(int i = 1 ; i <= k ; ++ i){
        buy[i][1] = -prices[1];
    }
    for (int j = 2 ; j <= n ; ++ j) {
        for (int i = 1 ; i <= k ; ++ i) {
            if(j > coolDown){
                buy[i][j] = max(buy[i][j-1], sale[i-1][j-1-coolDown]-prices[j]);
                sale[i][j] = max(buy[i][j-1]+prices[j], sale[i][j-1]);
            }else{
                buy[i][j] = max(buy[i][j-1], sale[i-1][0]-prices[j]);
                sale[i][j] = max(buy[i][j-1]+prices[j], sale[i][j-1]);
            }
        }
    }
    return sale[k][n];
}


int main()
{
    int n;
    cin>>n;
    vector<int>prices(n+1,0);
    for (int i = 1 ; i <= n ; ++ i) {
        cin>>prices[i];
    }
    return tradingWithCoolDown(prices, n , 2 , 1);
}
