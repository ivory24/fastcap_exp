//输入正多边形的总定点数和外接圆的半径，输出顶点坐标
#include<iostream>
#include<math.h>
using namespace std;

#define pi 3.141592653
int main()
{
	int num;
	cout << "please input the num" <<endl;
	cin >> num;
	double radius;
	cout << "please input radius" <<endl;
	cin >> radius;
	double theta = 360.0 /double(num);
	double tv1,tv2;
	for(int i = 0; i < num; i++)
	{
		tv1 = radius*cos((theta*i)/180*pi);
		tv2 = radius*sin((theta*i)/180*pi);
		cout << "x: "<< tv1  << " y: "<< tv2 <<endl;
	}
	cout << "hahah "<<endl;
	return 0;
}

