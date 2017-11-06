/*
*输入长宽高等相关信息，形成立方体，划分面元后，将坐标输入文件中 
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
int vn = 8,num;
double length = 0.0, width = 0.0, height = 0.0;
double slen = 0.0, swid = 0.0, shei=0.0; 
int ln = 0, wn = 0,hn = 0;
string s;
Node* no = new Node[8];

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

vector<Quad> bQuad;
vector<Quad> sQuad;
vector<Quad> vsQuad;

void input()
{
	cout << "Please input the length of cuboid: " <<endl;
	cin >> length;
	
	cout << "Please input the width of cuboid: "<<endl;
	cin >> width;
	cout << "Please input the height of cuboid: "<<endl;
	cin >> height;
	cout << "Please input the  small length edge:" <<endl;
	cin >> slen;
	ln = (int)(length/slen);
	cout << "the num of length -----------------" << ln << endl;
	cout << "Please input the small width edge: " << endl;
	cin >> swid;
	wn = (int)(width/swid);
	cout << "the num of width -----------------" << wn << endl;
	cout << "Please input the small height edge: " <<endl;
	cin >> shei;
	hn = (int)(height/shei);
	cout << "the num of height -----------------" << hn << endl;
	cout << "Please input the number of the conduct: " << endl;
	cin >> num;
	cout << "Please input the filename" <<endl;
	cin >> s;
}
/*
void meshqua( Quad sap, int n1, int n2)
{
	//n1----0:1   n2----0:3
	double x1 = ( sap.qno[1].x - sap. qno[0].x ) / n1;
	double y1 = ( sap.qno[1].y - sap. qno[0].y ) / n1;
	double z1 = ( sap.qno[1].z - sap. qno[0].z ) / n1;
	double x2 = ( sap.qno[3].x - sap. qno[0].x ) / n2;
	double y2 = ( sap.qno[3].y - sap. qno[0].y ) / n2;
	double z2 = ( sap.qno[3].z - sap. qno[0].z ) / n2;
	printf( "%.5f   %.5f   %.5f   %.5f   %.5f   %.5f\n" , x1, y1, z1, x2, y2, z2);
	Quad cor, me;													
	Node temn[3];
	temn[0].inval( sap.qno[0].x + x1, sap.qno[0].y + y1, sap.qno[0].z + z1 );
	temn[1].inval( sap.qno[0].x + x1 + x2, sap.qno[0].y + y1 + y2, sap.qno[0].z + z1 + z2 );
	temn[2].inval( sap.qno[0].x + x2, sap.qno[0].y + y2, sap.qno[0].z + z2 );
	cor.inval( sap.qno[0], temn[0], temn[1], temn[2] );
	cor.print();
	for( int r = 0; r < n1; r++ )
	{
		for ( int t=0; t < n2; t++ )
		{
			me.move ( cor, r * x1 + t * x2, r * y1 + t * y2, r * z1 + t * z2 );
			me.outfile();
		}
	}                                                                                                                                                                                                                                                                                                                                                                  
}
*/
void mesh_to_squad(int sn)
{
	//需要计算sn； 
	Node tt0, tt1;
	Quad te,t1,t2;
	double xl = 0.0, yl=0.0,zl=0.0,x2 = 0.0, y2 =0.0, z2 = 0.0;
	for(int i = 0, n = bQuad.size(); i<n; i++)
	{
		te = bQuad[i];
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

void mesh_vsQuad(int nn)
{
	Node ts0, ts1;
	Quad te,t1,t2;
	double xl = 0.0, yl=0.0,zl=0.0,x2 = 0.0, y2 =0.0, z2 = 0.0;
	for(int i = 0, n = sQuad.size(); i<n; i++)
	{
		te = sQuad[i];
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
void mesh_quad(int sn, int nn)
{
	Quad temp;
	mesh_to_squad(nn);
	mesh_vsQuad(sn);
	for( int i= 0, n = vsQuad.size(); i<n; i++)
	{
		temp = vsQuad[i];
		temp.outfile();
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
}
int main()
{
	input();
	fin = fopen( s.c_str(), "aw+");
	if ( !fin )   
		return -1;
	fprintf( fin, "0 <%s>\n",s.c_str() );
	fprintf( fin, "#this is a complete cuboid\n");
	fprintf( fin, "#length: %.5f width: %.5f height: %.5f\n", length, width, height);
	fprintf( fin, "#slen: %.5f swid: %.5f shei: %.5f\n", slen, swid, shei);
	fprintf( fin, "#ln: %d wn: %d hn: %d\n", ln, wn, hn);
	fclose(fin);
	no[0].inval( 0.0, 0.0, 0.0 );
	no[1].inval(length, 0.0, 0.0);
	no[2].inval(length, width, 0.0);
	no[3].inval(0.0, width, 0.0);
	no[4].inval(0.0, 0.0, height);
	no[5].inval(length, 0.0, height);
	no[6].inval(length, width, height);
	no[7].inval(0.0,width, height);
	Quad temp;
	temp.inval(no[4],no[5],no[6],no[7]);
	bQuad.push_back(temp);
	mesh_quad(ln,wn);
	
	temp.inval(no[2],no[3],no[7],no[6]);
	bQuad.push_back(temp);
	mesh_quad(ln,hn);
	
	temp.inval(no[1],no[2],no[6],no[5]);
	bQuad.push_back(temp);
	mesh_quad(wn,hn);
	 
	temp.inval(no[3],no[0],no[4],no[7]);
	bQuad.push_back(temp);
	mesh_quad(wn,hn);
	
	temp.inval(no[0],no[1],no[2],no[3]);
	bQuad.push_back(temp);
	mesh_quad(ln,wn);
	
	temp.inval(no[0],no[1],no[5],no[4]);
	bQuad.push_back(temp);
	mesh_quad(ln,hn);
	
	return 0;
}

