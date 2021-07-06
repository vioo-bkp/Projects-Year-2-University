#include <bits/stdc++.h>
using namespace std;

#define ll long long int
#define fl float

int main()
{
	int x, n, y;
	int sum = 0;
	cin >> x >>n>> y;

	int vieti[x];
	for (int i = 0; i <x; ++i){
		cin >> vieti[i];
		sum += vieti[i];
	}
	
	int gloante[n];
	for (int i = 0; i <n; ++i){
		cin >> gloante[i];
	}
	
	for(int i=0;i<n;i++)
	{		
		for(int j=i+1;j<n;j++)
		{
			if(gloante[i]<gloante[j])
			{ 
				int temp = gloante[i];
				gloante[i]=gloante[j];
				gloante[j]=temp;
			}
		}
	}
	int sum2 = 0;
	for (int i = 0; i <y; ++i){
		sum2 += gloante[i];
	}

	// cout << sum2 << endl;
	if (sum2 <= sum) {
		cout << "Nu "<< sum-sum2;
	} else {
		cout << "Da "<< sum2-sum;
	}

	return 0;
}
