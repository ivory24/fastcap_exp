/*
*输入半径和正多边形的边数等相关信息，形成横截面是正多边形，
*侧面是很多矩形的类圆柱，划分面元后，将坐标输入文件中 
*/
#include<iostream>
#include<fstream>
#include<vector>
#include<string.h>
#include<stdio.h>
#include<math.h>
using namespace std;

#define pi 3.141592653

double sq( double t )
{
	return t * t;
}
struct Node
{
	double x;
	double y;
	double z;
	void inval( double x1, double y1, double z1 )
	{
		x = x1; y = y1; z = z1;   	
	}
	Node move( double xx, double yy, double zz )
	{
		Node nn;
		nn.inval(x+xx,yy+y,zz+z);
		return nn;
	}
	Node up(double height)
	{
		Node nn;
		nn.inval(x,y,z+height);
		return nn;
	}
	double distance( Node noo)
	{
		return sqrt( sq( noo.x - x ) + sq( noo.y - y ) + sq( noo.z - z ));
	}
	Node() {};
};

FILE* fin;
int vn = 0,num = 2;
double radius = 0,tv1 = 0,tv2 = 0,tv3 = 0,theta = 0.0,height= 0.0;
double small_edge_length_side = 0.0, small_edge_length_ground = 0.0;
int sn = 0, nn = 0;
string s;
Node* no = new Node[19];
Node* nodown = new Node[19];


struct Quad
{
	Node qno[4];
	void inval( Node lt, Node rt, Node ru, Node lu )
	{
		qno[0] = lt;
		qno[1] = rt;
		qno[2] = ru;
		qno[3] = lu;
	}
	void move( Quad pred, double xh1, double yh1, double zh1,double xh2, double yh2, double zh2 )
	{
		qno[0]=pred.qno[3];
		qno[1]=pred.qno[2];
		qno[2]=pred.qno[2].move(xh2,yh2,zh2);
		qno[3]=pred.qno[3].move(xh1,yh1,zh1);
	}
	void move_ltor( Quad pred, double xh1, double yh1, double zh1,double xh2, double yh2, double zh2 )
	{
		qno[0]=pred.qno[0].move(xh1,yh1,zh1);
		qno[1]=pred.qno[0];
		qno[2]=pred.qno[3];
		qno[3]=pred.qno[3].move(xh2,yh2,zh2);
	}
	void every_point_move(Quad pred, double xx, double yy, double zz)
	{
		for(int j = 0; j< 4; j++)
		{
			qno[j].x = pred.qno[j].x+xx;
			qno[j].y = pred.qno[j].y+yy;
			qno[j].z = pred.qno[j].z+zz;
		}
	}
	void outfile()
	{
		fin = fopen( s.c_str(), "aw+" );
		fprintf( fin, "Q %d",num);
		for( int j = 0; j < 4; j++ )
		{
			fprintf( fin, "  %.5f %.5f %.5f", qno[j].x, qno[j].y, qno[j].z);
		}
		fprintf( fin, "\n" );
		fclose(fin);
	}
	void print()
	{
		for( int j = 0; j < 4; j++ )
		{
			printf( "  %.5f %.5f %.5f\n", qno[j].x, qno[j].y, qno[j].z);
		}
	}
};
struct Tri
{
	Node tno[4];
	void inval( Node top, Node tl, Node tr)
	{
		tno[0] = top;
		tno[1] = tl;
		tno[2] = tr;
	}
	void every_point_move(Tri pred, double xx, double yy, double zz)
	{
		for(int j = 0; j< 3; j++)
		{
			tno[j].x = pred.tno[j].x+xx;
			tno[j].y = pred.tno[j].y+yy;
			tno[j].z = pred.tno[j].z+zz;
		}
	}
	void outfile()
	{
		fin = fopen( s.c_str(), "aw+" );
		fprintf( fin, "T %d",num);
		for( int j = 0; j < 3; j++ )
		{
			fprintf( fin, "  %.5f %.5f %.5f", tno[j].x, tno[j].y, tno[j].z);
		}
		fprintf( fin, "\n" );
		fclose(fin);
	}
	void print()
	{
		for( int j = 0; j < 3; j++ )
		{
			printf( "  %.5f %.5f %.5f\n", tno[j].x, tno[j].y, tno[j].z);
		}
	}
};

vector<Quad> bQuad;
vector<Quad> sQuad;
vector<Quad> vsQuad;
vector<Tri>  bTri;
vector<Quad> g_vsQuad;
vector<Tri>  g_bTri;
void input()
{
	cout << "Please input the number of regular polygon's vertices: " <<endl;
	cin >> vn;
	Node* no = new Node[vn];
	Node* nodown = new Node[vn];
	cout << "Please input the sphere's radius: "<<endl;
	cin >> radius;
	cout << "Please input the height of cylinder: "<<endl;
	cin >> height;
	cout << "Please input the length of small edge ont the side" <<endl;
	cin >> small_edge_length_side;
	cout << "Please input the length of the small edge on the ground" << endl;
	cin >> small_edge_length_ground;
	cout << "Please input the filename" <<endl;
	cin >> s;
	cout << "Please input the conduct num" << endl;
	cin >> num;
	theta = 360.0 /double(vn);
}
void mesh_to_squad(double small_edge_length)
{
	//需要计算sn； 
	Node tt0, tt1;
	Quad te,t1,t2;
	double xl = 0.0, yl=0.0,zl=0.0,x2 = 0.0, y2 =0.0, z2 = 0.0;
	for(int i = 0, n = bQuad.size(); i<n; i++)
	{
		te = bQuad[i];
		sn = (te.qno[3].distance(te.qno[0]))/small_edge_length;
		if(sn < 3)
		{
			cout << "jump sn: " <<sn <<   endl;
			sQuad.push_back(te);
			continue;
		}
		xl = (te.qno[3].x - te.qno[0].x)/sn;
		yl = (te.qno[3].y - te.qno[0].y)/sn;
		zl = (te.qno[3].z - te.qno[0].z)/sn;
		x2 = (te.qno[2].x - te.qno[1].x)/sn;
		y2 = (te.qno[2].y - te.qno[1].y)/sn;
		z2 = (te.qno[2].z - te.qno[1].z)/sn;
		t1.inval(te.qno[0],te.qno[1], te.qno[1].move(x2,y2,z2),te.qno[0].move(xl,yl,zl));
		sQuad.push_back(t1);
		for(int k = 0; k < sn-2; k++ )
		{
			t2.move(t1,xl,yl,zl,x2,y2,z2);
			sQuad.push_back(t2);
			t1 = t2;
			if( k == sn -3)
			{
				tt0 = t1.qno[3];
				tt1 = t1.qno[2];
			}
		}
		t1.inval(tt0,tt1, te.qno[2],te.qno[3]);
		sQuad.push_back(t1);
	}
}

void mesh_vsQuad(double small_edge_length)
{
	Node ts0, ts1;
	Quad te,t1,t2;
	double xl = 0.0, yl=0.0,zl=0.0,x2 = 0.0, y2 =0.0, z2 = 0.0;
	for(int i = 0, n = sQuad.size(); i<n; i++)
	{
		te = sQuad[i];
		nn = (te.qno[0].distance(te.qno[1]))/small_edge_length;
		if(nn < 3)
		{
			cout << "jump nn: " <<nn <<   endl;
			vsQuad.push_back(te);
			continue;
		}
		xl = (te.qno[0].x - te.qno[1].x)/nn;
		yl = (te.qno[0].y - te.qno[1].y)/nn;
		zl = (te.qno[0].z - te.qno[1].z)/nn;
		x2 = (te.qno[3].x - te.qno[2].x)/nn;
		y2 = (te.qno[3].y - te.qno[2].y)/nn;
		z2 = (te.qno[3].z - te.qno[2].z)/nn;
		t1.inval(te.qno[1].move(xl,yl,zl),te.qno[1],te.qno[2], te.qno[2].move(x2,y2,z2));
		vsQuad.push_back(t1);
		for(int k = 0; k < nn-2; k++ )
		{
			t2.move_ltor(t1,xl,yl,zl,x2,y2,z2);
			vsQuad.push_back(t2);
			t1 = t2;
			if( k == nn -3)
			{
				ts0 = t1.qno[0];
				ts1 = t1.qno[3];
			}
		}
		t1.inval(te.qno[0],ts0,ts1, te.qno[3]);
		vsQuad.push_back(t1);
	}
}
int main()
{
	input();
	//cout << hi << "  " << ls << endl;
	fin = fopen( s.c_str(), "aw+");
	if ( !fin )   
		return -1;
	fprintf( fin, "0 <%s>\n",s.c_str() );
	fprintf( fin, "#radius: %.5f height: %.5f\n", radius,height);
	fprintf( fin, "#small_edge_length_side: %.5f small_edge_length_ground: %.5f\n", small_edge_length_side,small_edge_length_ground);
	fprintf( fin, "# this is a polygon cylinder with bottom\n");
	fclose(fin);
	for(int i = 0; i < vn; i++)
	{
		tv1 = radius*cos((theta*i)/180*pi)+radius;
		tv2 = radius*sin((theta*i)/180*pi)+radius;
		no[i].inval(tv1,tv2,tv3);	
	}
	cout << "hello" << endl;
	//可以将正多边形划分为以下的四边形，加入bQuad 
	Quad temp;
	for(int i = 0; i <= (vn/4)-2; i++)
	{
		temp.inval(no[i],no[(vn/2)-i], no[(vn/2)-i-1],no[i+1]);
		cout << i << " " << (vn/2)-i << " " << (vn/2)-i-1 << " " << i+1 << endl;
		bQuad.push_back(temp);
	} 
	cout << "hi" << endl;
	for(int i = 0; i <= (vn/4)-2; i++)
	{
		temp.inval(no[vn-i-1], no[(vn/2)+i+1],no[(vn/2)+i],no[(vn-i)%vn]);
		cout << vn-i-1 << " " << (vn/2)+i+1<< " " << (vn/2)+i << " " << (vn-i)%vn<< endl;
		bQuad.push_back(temp);
	}
	cout << "no" << endl;
	//将两头的剩余的归结为三角形 
	Tri ttemp;
	ttemp.inval(no[(vn/4)],no[(vn/4)+1],no[(vn/4)-1]);
	bTri.push_back(ttemp);
	ttemp.inval(no[vn*3/4],no[(vn*3/4)+1],no[(vn*3/4)-1]);
	bTri.push_back(ttemp);
	
	cout << "origin size: " << bQuad.size()<<endl;
	//将梯形划分成很窄的长梯形。 
	mesh_to_squad(small_edge_length_ground);
	mesh_vsQuad(small_edge_length_ground);
	cout << "new size: " << sQuad.size()<<endl;
	cout << "new size: " << vsQuad.size()<<endl;

	for( int i= 0, n = vsQuad.size(); i<n; i++)
	{
		temp = vsQuad[i];
		g_vsQuad.push_back(temp);
		temp.every_point_move(temp,0.0,0.0,height);
		temp.outfile();
	}

	for( int i= 0, n = bTri.size(); i<n; i++)
	{
		ttemp = bTri[i];
		g_bTri.push_back(ttemp);
		ttemp.every_point_move(ttemp,0.0,0.0,height);
		ttemp.outfile();
	}

	//vs s b 都清空 
	for( int i= 0, n = vsQuad.size(); i<n; i++)
	{
		vsQuad.pop_back();
	}
	for( int i= 0, n = sQuad.size(); i<n; i++)
	{
		sQuad.pop_back();
	}
	for( int i= 0, n = bQuad.size(); i<n; i++)
	{
		bQuad.pop_back();
	}
	cout << "late size: " << bQuad.size()<<endl;
	cout << "late size: " << sQuad.size()<<endl;
	cout << "late size: " << vsQuad.size()<<endl;
	
	for(int i = 0 ;i < vn; i++)
	{
		temp.inval(no[i].up(height),no[i],no[(i+1)%vn],no[(i+1)%vn].up(height));
		bQuad.push_back(temp);
	}
	//把需要划分的四边形压入栈 
	cout << "mesh side" << endl;
	mesh_to_squad(small_edge_length_ground);
	mesh_vsQuad(small_edge_length_side);
	cout << "side pieces num: " << vsQuad.size() << endl; 
	for( int i= 0, n = vsQuad.size(); i<n; i++)
	{
		temp = vsQuad[i];
		temp.outfile();
	}
	for( int i= 0, n = g_bTri.size(); i<n; i++)
	{
		ttemp = g_bTri[i];
		ttemp.outfile();
	}
	for( int i= 0, n = g_vsQuad.size(); i<n; i++)
	{
		temp = g_vsQuad[i];
		temp.outfile();
	}
	return 0;
}

