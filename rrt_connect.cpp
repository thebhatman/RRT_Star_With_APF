#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

using namespace cv;
using namespace std;
float max_step_size = 2.5;

typedef struct point
{
	float x,y;
}point;

typedef struct node
{
	point curr;
	Point_<float> parent;
}node;
float dist(node p1, node p2)
{
	float d = sqrt(pow(p1.curr.x-p2.curr.x,2)+pow(p1.curr.y-p2.curr.y,2));
	return d;
}
int z1=0,z2=0,z3=0,z4=0,z5=0,z6=0;
Vector<node> srctree;
Vector<node> dest1tree;
Vector<node> dest2tree;
int index(Vector<node> &v, Point_<float> p)
{
	int i;
	for(i=0;i<v.size();i++)
	{
		if(v[i].curr.x==p.x&&v[i].curr.y==p.y)
		return i;
	}
	return -1;
}

Mat bin(Mat a)
{
	int p,q;
	for(p=0;p<a.rows;p++)
		for(q=0;q<a.cols;q++)
			if(a.at<uchar>(p,q)>127) a.at<uchar>(p,q)=255;
			else a.at<uchar>(p,q)=0;

	return a;
}

int main()
{
Mat a = imread("Untitled1.png",0);
a=bin(a);
int m,n;
/*for(m = 220;m<420;m++)
	for(n=220;n<420;n++)
	a.at<uchar>(m,n)=255;
for( m = 30;m<100;m++)
	for(n=30;n<100;n++)
	a.at<uchar>(m,n)=255;*/
srand(time(0));
node source,dest1,dest2;
cout<<"Enter the co-ordinates of source: "<<endl;
cin>>source.curr.x>>source.curr.y;
int i = source.curr.x;
int j = source.curr.y;
if(a.at<uchar>(j,i)>150) {cout<<"source is on the obstacle\n"; return 0;}
a.at<uchar>(j,i) = 255;
cout<<"Enter the co-ordinates of the destination1: "<<endl;
cin>>dest1.curr.x>>dest1.curr.y;
i = dest1.curr.x;
j = dest1.curr.y;
if(a.at<uchar>(j,i)>150) {cout<<"dest1 is on the obstacle\n"; return 0;}
a.at<uchar>(j,i) = 255;
cout<<"Enter the co-ordinates of the destination2: "<<endl;
cin>>dest2.curr.x>>dest2.curr.y;
i = dest2.curr.x;
j = dest2.curr.y;
if(a.at<uchar>(j,i)>150) {cout<<"dest2 is on the obstacle\n"; return 0;}
a.at<uchar>(j,i) = 255;
source.parent.x = source.curr.x;
source.parent.y = source.curr.y;
dest1.parent.x = dest1.curr.x;
dest1.parent.y = dest1.curr.y;
dest2.parent.x = dest2.curr.x;
dest2.parent.y = dest2.curr.y;
srctree.push_back(source);
dest1tree.push_back(dest1);
dest2tree.push_back(dest2);
namedWindow("Image",WINDOW_NORMAL);
imshow("Image",a);
node qnear,qneard1,qneard2,qnearest,qnearestd1,qnearestd2,qrand;
qnear = source;
qneard1 = dest1;
qneard2 = dest2;
float d,d1,d2;
	
printf("%d, %d\n",a.rows,a.cols);

int s,t,u,v,flag1=0,flag2=0,si1,si2,d1i,d2i,f,g,h,test=0;
Point_<float> p1,p2,p3,p4;
while(1)
{
	if(srctree.size()==1)
	{
		qrand.curr.x = rand()%a.rows;
		qrand.curr.y = rand()%a.cols;
		d = dist(qnear,qrand);
		if(d>max_step_size)
		{
		qnearest.curr.x = ((d-max_step_size)*qnear.curr.x+(max_step_size)*qrand.curr.x)/d;
		qnearest.curr.y = ((d-max_step_size)*qnear.curr.y+(max_step_size)*qrand.curr.y)/d;
		}
		else qnearest = qrand;
		qnearest.parent.x = qnear.curr.x;
		qnearest.parent.y = qnear.curr.y;
		srctree.push_back(qnearest);
		
		p1.x = qnear.curr.x; p1.y = qnear.curr.y; p2.x = qnearest.curr.x; p2.y = qnearest.curr.y;
		line(a,p1,p2,Scalar(128),1,8,0);
		qnear = qnearest;
	}
	else
	{
		here:
		qrand.curr.y = rand()%a.cols;
		qrand.curr.x = rand()%a.rows;
		d = dist(qnear,qrand);
		for(s=srctree.size()-1;s>=0;s--)
		{
			if(dist(srctree[s],qrand)<=d)
			{
				d = dist(srctree[s],qrand);
				for(i=0,z1=0;i<d;i++)
				{
					float p = d;
					node q; q.curr.x = ((p-i)*srctree[s].curr.x+i*qrand.curr.x)/p;
					q.curr.y = ((p-i)*srctree[s].curr.y+i*qrand.curr.y)/p;
					int c,d; c= q.curr.y; d= q.curr.x;
					if(a.at<uchar>(c,d)>150) goto here;
				}
				if(d>max_step_size)
				{
				qnearest.curr.x = ((d-max_step_size)*srctree[s].curr.x+(max_step_size)*qrand.curr.x)/d;
				qnearest.curr.y = ((d-max_step_size)*srctree[s].curr.y+(max_step_size)*qrand.curr.y)/d;
				}
				else qnearest = qrand;
				for(i=0,z2=0;i<dist(srctree[s],qnearest);i++)
				{
					float p = dist(srctree[s],qnearest);
					node q; q.curr.x = ((p-i)*srctree[s].curr.x+i*qnearest.curr.x)/p;
					q.curr.y = ((p-i)*srctree[s].curr.y+i*qnearest.curr.y)/p;
					int c,d; c= q.curr.y; d= q.curr.x;
					
					if(a.at<uchar>(c,d)>150) goto here;
				}
				qnearest.parent.x = srctree[s].curr.x;
				qnearest.parent.y = srctree[s].curr.y; f=s;
			}
		}
		p1.x = srctree[f].curr.x; p1.y = srctree[f].curr.y; p2.x = qnearest.curr.x; p2.y = qnearest.curr.y;
		line(a,p1,p2,Scalar(128),1,8,0);
		srctree.push_back(qnearest);
		qnear = qnearest;
	}
	if(!flag1)
	{
	if(dest1tree.size()==1)
	{
		qrand.curr.x = rand()%a.rows;
		qrand.curr.y = rand()%a.cols;
		d1 = dist(qneard1,qrand);
		if(d1>max_step_size)
		{
		qnearestd1.curr.x = ((d1-max_step_size)*qneard1.curr.x+(max_step_size)*qrand.curr.x)/d1;
		qnearestd1.curr.y = ((d1-max_step_size)*qneard1.curr.y+(max_step_size)*qrand.curr.y)/d1;
		}
		else qnearestd1 = qrand;
		qnearestd1.parent.x = qneard1.curr.x;
		qnearestd1.parent.y = qneard1.curr.y;
		dest1tree.push_back(qnearestd1);
		p1.x = qneard1.curr.x; p1.y = qneard1.curr.y; p2.x = qnearestd1.curr.x; p2.y = qnearestd1.curr.y;
		line(a,p1,p2,Scalar(128),1,8,0);
		qneard1 = qnearestd1;
	}
	else
	{
		here1:
		qrand.curr.x = rand()%a.rows;
		qrand.curr.y = rand()%a.cols;
		d1 = dist(qneard1,qrand);
		for(s=dest1tree.size()-1;s>=0;s--)
		{
			if(dist(dest1tree[s],qrand)<=d1)
			{
				d1 = dist(dest1tree[s],qrand);
				for(i=0,z3=0;i<d1;i++)
				{
					float p = d1;
					node q; q.curr.x = ((p-i)*dest1tree[s].curr.x+i*qrand.curr.x)/p;
					q.curr.y = ((p-i)*dest1tree[s].curr.y+i*qrand.curr.y)/p;
					int c,d; c= q.curr.y; d= q.curr.x;
					if(a.at<uchar>(c,d)>150) goto here1;
				}
				if(d1>max_step_size)
				{
				qnearestd1.curr.x = ((d1-max_step_size)*dest1tree[s].curr.x+(max_step_size)*qrand.curr.x)/d1;
				qnearestd1.curr.y = ((d1-max_step_size)*dest1tree[s].curr.y+(max_step_size)*qrand.curr.y)/d1;
				}
				else qnearestd1 = qrand;
				for(i=0,z4=0;i<dist(dest1tree[s],qnearestd1);i++)
				{
					float p = dist(dest1tree[s],qnearestd1);
					node q; q.curr.x = ((p-i)*dest1tree[s].curr.x+i*qnearestd1.curr.x)/p;
					q.curr.y = ((p-i)*dest1tree[s].curr.y+i*qnearestd1.curr.y)/p;
					int c,d; c= q.curr.y; d= q.curr.x;
					if(a.at<uchar>(c,d)>150) goto here1;
				}
				qnearestd1.parent.x = dest1tree[s].curr.x;
				qnearestd1.parent.y = dest1tree[s].curr.y; g = s;
			}
		}
		p1.x = dest1tree[g].curr.x; p1.y = dest1tree[g].curr.y; p2.x = qnearestd1.curr.x; p2.y = qnearestd1.curr.y;
		line(a,p1,p2,Scalar(128),1,8,0);
		dest1tree.push_back(qnearestd1);
		qneard1 = qnearestd1;
	}
	}
	if(!flag2)
	{
	if(dest2tree.size()==1)
	{
		qrand.curr.x = rand()%a.rows;
		qrand.curr.y = rand()%a.cols;
		d2 = dist(qneard2,qrand);
		if(d2>max_step_size)
		{
		qnearestd2.curr.x = ((d2-max_step_size)*qneard2.curr.x+(max_step_size)*qrand.curr.x)/d2;
		qnearestd2.curr.y = ((d2-max_step_size)*qneard2.curr.y+(max_step_size)*qrand.curr.y)/d2;
		}
		else qnearestd2 = qrand;
		qnearestd2.parent.x = qneard2.curr.x;
		qnearestd2.parent.y = qneard2.curr.y;
		dest2tree.push_back(qnearestd2);
		p1.x = qneard2.curr.x; p1.y = qneard2.curr.y; p2.x = qnearestd2.curr.x; p2.y = qnearestd2.curr.y;
		line(a,p1,p2,Scalar(128),1,8,0);
		qneard2 = qnearestd2;
	}
	else
	{
		here2:
		qrand.curr.x = rand()%a.rows;
		qrand.curr.y = rand()%a.cols;
		d2 = dist(qneard2,qrand);
		for(s=dest2tree.size()-1;s>=0;s--)
		{
			if(dist(dest2tree[s],qrand)<=d2)
			{
				d2 = dist(dest2tree[s],qrand);
				for(i=0,z5=0;i<d2;i++)
				{
					float p = d2;
					node q; q.curr.x = ((p-i)*dest2tree[s].curr.x+i*qrand.curr.x)/p;
					q.curr.y = ((p-i)*dest2tree[s].curr.y+i*qrand.curr.y)/p;
					int c,d; c= q.curr.y; d= q.curr.x;
					if(a.at<uchar>(c,d)>150) goto here2;
				}
				if(d2>max_step_size)
				{
				qnearestd2.curr.x = ((d2-max_step_size)*dest2tree[s].curr.x+(max_step_size)*qrand.curr.x)/d2;
				qnearestd2.curr.y = ((d2-max_step_size)*dest2tree[s].curr.y+(max_step_size)*qrand.curr.y)/d2;
				}
				else qnearestd2 = qrand;
				for(i=0,z6=0;i<dist(dest2tree[s],qnearestd2);i++)
				{
					float p = dist(dest2tree[s],qnearestd2);
					node q; q.curr.x = ((p-i)*dest2tree[s].curr.x+i*qnearestd2.curr.x)/p;
					q.curr.y = ((p-i)*dest2tree[s].curr.y+i*qnearestd2.curr.y)/p;
					int c,d; c= q.curr.y; d= q.curr.x;
					if(a.at<uchar>(c,d)>150) goto here2;
				}
				qnearestd2.parent.x = dest2tree[s].curr.x;
				qnearestd2.parent.y = dest2tree[s].curr.y; h=s;
			}
		}
		p1.x = dest2tree[h].curr.x; p1.y = dest2tree[h].curr.y; p2.x = qnearestd2.curr.x; p2.y = qnearestd2.curr.y;
		line(a,p1,p2,Scalar(128),1,8,0);
		dest2tree.push_back(qnearestd2);
		qneard2 = qnearestd2;	
	}
	}
	for(t=0;t<srctree.size();t++)
	{
		for(u=0;u<dest1tree.size();u++)
		{
			if(dist(srctree[t],dest1tree[u])<max_step_size)
			{
				flag1 = 1; si1 = t; d1i = u; 
				p1.x = srctree[si1].curr.x; p1.y = srctree[si1].curr.y; 
				p2.x = dest1tree[d1i].curr.x; p2.y = dest1tree[d1i].curr.y; 
				line(a,p1,p2,Scalar(128),1,8,0);				
				break;			
			}
		}
		if(flag1==1) break;
	}
	for(t=0;t<srctree.size();t++)
	{
		for(v=0;v<dest2tree.size();v++)
		{
			if(dist(srctree[t],dest2tree[v])<max_step_size)
			{
				flag2 = 1; si2 = t; d2i = v;
				p1.x = srctree[si2].curr.x; p1.y = srctree[si2].curr.y; 
				p2.x = dest2tree[d2i].curr.x; p2.y = dest2tree[d2i].curr.y; 
				line(a,p1,p2,Scalar(128),1,8,0);
				break;
			}
		}
		if(flag2==1) break;
	}
	imshow("Image",a);
	waitKey(2);
	if(flag1&&flag2) break;
}

Mat b(a.rows,a.cols,CV_8UC1,Scalar(0));
namedWindow("Final",WINDOW_NORMAL);
p1.x = srctree[si1].curr.x;
p1.y = srctree[si1].curr.y;
p2 = srctree[si1].parent;
line(b,p1,p2,Scalar(255),1,8,0);
while(1)
{
	line(b,p1,p2,Scalar(255),1,8,0);
	p1.x=p2.x; p1.y = p2.y; int e = index(srctree,p1);
	p2=srctree[e].parent;
	line(b,p1,p2,Scalar(255),1,8,0);
	if(p2.x==source.curr.x&&p2.y==source.curr.y) break;
}
p1.x = dest1tree[d1i].curr.x;
p1.y = dest1tree[d1i].curr.y;
p2 = dest1tree[d1i].parent;
line(b,p1,p2,Scalar(255),1,8,0);
while(1)
{
	line(b,p1,p2,Scalar(255),1,8,0);
	p1.x=p2.x; p1.y = p2.y; int e = index(dest1tree,p1);
	p2=dest1tree[e].parent;
	line(b,p1,p2,Scalar(255),1,8,0);
	if(p2.x==dest1.curr.x&&p2.y==dest1.curr.y) break;
}
p1.x = srctree[si1].curr.x; p1.y = srctree[si1].curr.y; 
p2.x = dest1tree[d1i].curr.x; p2.y = dest1tree[d1i].curr.y; 
line(a,p1,p2,Scalar(255),1,8,0);
p1.x = srctree[si2].curr.x;
p1.y = srctree[si2].curr.y;
p2 = srctree[si2].parent;
line(b,p1,p2,Scalar(150),1,8,0);
while(1)
{
	line(b,p1,p2,Scalar(255),1,8,0);
	p1.x=p2.x; p1.y = p2.y; int e = index(srctree,p1);
	p2=srctree[e].parent;
	line(b,p1,p2,Scalar(255),1,8,0);
	if(p2.x==source.curr.x&&p2.y==source.curr.y) break;
}
p1.x = dest2tree[d2i].curr.x;
p1.y = dest2tree[d2i].curr.y;
p2 = dest2tree[d2i].parent;
line(b,p1,p2,Scalar(150),1,8,0);
while(1)
{
	line(b,p1,p2,Scalar(150),1,8,0);
	p1.x=p2.x; p1.y = p2.y; int e = index(dest2tree,p1);
	p2=dest2tree[e].parent;
	line(b,p1,p2,Scalar(150),1,8,0);
	if(p2.x==dest2.curr.x&&p2.y==dest2.curr.y) break;
}
p1.x = srctree[si2].curr.x; p1.y = srctree[si2].curr.y; 
p2.x = dest2tree[d2i].curr.x; p2.y = dest2tree[d2i].curr.y; 
line(b,p1,p2,Scalar(150),1,8,0);
imshow("Final",b);

while(waitKey(0)!=27){}
return 0;
}

	
	
	
				
