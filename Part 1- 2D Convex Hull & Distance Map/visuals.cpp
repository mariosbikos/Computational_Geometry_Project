
#include "gl/glut.h"   
#include "visuals.h"   
#include "GeoLib.h"  
#include <stdio.h>    
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <stdlib.h>

#define SPACEBAR 32

using namespace std;

C2DPointSet intersectPts,testset,ptSet;
C2DPolygon myPoly,myCH;
C2DPoint point,p1,p2,p3,p4;
C2DLine cur_line;

vector<double> lengths,point_x,point_y;
vector<char> vs;
vector<C2DPoint> pp;
vector<C2DLine> lines_of_CH,lines_inside_CH,lines_of_Polygon,lines_of_CH_not_of_Polygon,rotatedLines,final_lines;
vector<vector<C2DPoint> > intersection_pts_vector;
vector<vector<C2DLine> > rotated_lines_of_each_external_line;

int i,number_of_points,number_Of_CH_Points,number_of_lines_inside_CH,number_of_lines_of_CH_not_of_Polygon;
char v;
double zoom=1.0,x,y,a,b,c,l,p,max2,min2;

void keyboardCallBack(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'o':
		zoom=zoom-0.5;
		break;
	case 'i':
		zoom = zoom + 0.5;
		break;
	case SPACEBAR:
		zoom = 1.0;
		break;
	default:
		printf("Press I-O for Zoom In-Zoom Out\n");
		printf("Press Spacebar to Reset\n");
	}
	glutPostRedisplay();
}


void Render()
{    
  glClear(GL_COLOR_BUFFER_BIT );  // Clean up the colour of the window
								  // and the depth buffer

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

//=============GEOLIB================
	glScaled(zoom,zoom,zoom);

	//DRAW INITIAL POLYGON(GREEN)
	glLineWidth(2);
	glColor3f(0.2, 0.9, 0.2);
	glBegin(GL_LINE_LOOP);
	for (int i=0;i<number_of_points;i++)
	{
		glVertex2f(ptSet.GetAt(i)->x, ptSet.GetAt(i)->y);
	}
	glEnd();


	//DRAW CONVEX HULL(RED)
	/*glColor3f(0.9, 0.2, 0.2);
    glPointSize(5);
	glBegin(GL_LINE_LOOP);
		for (int i=0;i<number_Of_CH_Points;i++){
			glVertex2f(myCH.GetPoint(i)->x, myCH.GetPoint(i)->y);
		}
	glEnd();*/

	//DRAW CONVEX HULL POINTS(YELLOW)
	/*glColor3f(0.9, 0.8, 0.2);
    glPointSize(6);
	glBegin(GL_POINTS);
		for (int i=0;i<number_Of_CH_Points;i++){
			glVertex2f(myCH.GetPoint(i)->x, myCH.GetPoint(i)->y);
	
		}
	glEnd();*/

	//DRAW POLYGON LINES(BLUE)
	/*glColor3f(0.2, 0.2, 0.9);
   	glBegin(GL_LINES);
	for (int i=0;i<number_of_points;i++){
			glVertex2f(lines_of_Polygon[i].GetPointFrom().x,lines_of_Polygon[i].GetPointFrom().y);
			glVertex2f(lines_of_Polygon[i].GetPointTo().x,lines_of_Polygon[i].GetPointTo().y);
		}
	glEnd();*/


	//DRAW LINES OF CH(WHITE)
	/*glColor3f(0.9, 0.9, 0.9);
   	glBegin(GL_LINES);
	for (int i=0;i<number_Of_CH_Points;i++)
	{
		glVertex2f(lines_of_CH[i].GetPointFrom().x,lines_of_CH[i].GetPointFrom().y);
		glVertex2f(lines_of_CH[i].GetPointTo().x,lines_of_CH[i].GetPointTo().y);
	
	}
	glEnd();*/

	//DRAW LINES OF CH NOT OF POLYGON(BLUE)
	glColor3f(0.0, 0.1, 0.9);
   	glBegin(GL_LINES);
	for (int i=0;i<number_of_lines_of_CH_not_of_Polygon ;i++)
	{
		glVertex2f(lines_of_CH_not_of_Polygon[i].GetPointFrom().x,lines_of_CH_not_of_Polygon[i].GetPointFrom().y);
		glVertex2f(lines_of_CH_not_of_Polygon[i].GetPointTo().x,lines_of_CH_not_of_Polygon[i].GetPointTo().y);
	
	}
	glEnd();

	//DRAW LINES OF POLYGON NOT IN CH(BLUE)
	/*glColor3f(0.7, 0.7, 0.2);
   	glBegin(GL_LINES);
	for (int i=0;i< number_of_lines_inside_CH;i++)
	{
		glVertex2f(lines_inside_CH[i].GetPointFrom().x,lines_inside_CH[i].GetPointFrom().y);
		glVertex2f(lines_inside_CH[i].GetPointTo().x,lines_inside_CH[i].GetPointTo().y);
	
	}
	glEnd();*/

	
	//DRAW DISTANCE MAP
	glColor3ub(255, 255, 255);
   	glBegin(GL_LINES);
	for(int i=0;i<number_of_lines_of_CH_not_of_Polygon;i++)
	{
		for(int j=0;j<rotated_lines_of_each_external_line[i].size() ;j++)
		{
			l=rotated_lines_of_each_external_line[i][j].GetPointFrom().Distance(rotated_lines_of_each_external_line[i][j].GetPointTo());
			p=1-((max2-l)/max2);
			b=c=255-(p*255);
			glColor3ub(255, b, c);
			glVertex2f(rotated_lines_of_each_external_line[i][j].GetPointFrom().x,rotated_lines_of_each_external_line[i][j].GetPointFrom().y);
			glVertex2f(rotated_lines_of_each_external_line[i][j].GetPointTo().x,rotated_lines_of_each_external_line[i][j].GetPointTo().y);

		}
	}
	glEnd();


	//Draw Points of intersection of rays to object
	glColor3f(0.99, 0.95, 0.9);
	glPointSize(1);
   	glBegin(GL_POINTS);
	for(int i=0;i<number_of_lines_of_CH_not_of_Polygon;i++)
	{
		for(int j=0;j<intersection_pts_vector[i].size() ;j++)
		{
			glVertex2f(intersection_pts_vector[i][j].x, intersection_pts_vector[i][j].y);
		}
	}
	glEnd();


//=============GEOLIB================
  
  glutSwapBuffers();             // All drawing commands applied to the 
                                 // hidden buffer, so now, bring forward
                                 // the hidden buffer and hide the visible one           
  

}

//-----------------------------------------------------------

void Resize(int w, int h)
{ // w and h are window sizes returned by glut
  // define the visible area of the window ( in pixels )
  
  if (h==0) h=1;
  glViewport(0,0,w,h);
  //glViewport(100,100,400,400);

  // Setup viewing volume
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
 
 
  //         L,      R,      B,     T,     N,      F
  glOrtho (-50.0f, 50.0f, -50.0f, 50.0f, 100.0f, -100.0f);


}


void Setup() 
{ 
	//Parameter handling
	//glShadeModel (GL_SMOOTH);
	glShadeModel (GL_FLAT);

	// polygon rendering mode
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  
	//Set up light source
	GLfloat light_position[] = { -50.0, 20.0, 150.0, 1.0 };
	GLfloat ambientLight[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	   
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	glLightfv( GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_POINT_SMOOTH);

	// Black background
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	
//================Define your static values after this===============
	C2DRect *myRect = new C2DRect(-50,50,50,-50);	//borders

	//Scan Model_2d_test.poly file for vertices
	ifstream myfile ("Model_2d_test.poly");
	if (myfile.is_open())
	{
		 i=0;
		 while ( !myfile.fail() )
		     {
			 	 myfile>>v;
				 //only accept points with a v in front of coordinates
				 if (strcmp("v",&v)==0)
				 {
					 vs.push_back(v);
			 		 myfile>>x;
					 point_x.push_back((x));   
			 		 myfile>>y;
					 point_y.push_back((y));   
					 i++;
				 }
		     }
			 //calculate total number of points scanned
			 number_of_points=i;
	}
	else 
	{
		cout << "Unable to open file";
	}
	cout<<"Total number of 2d Points scanned: "<<number_of_points<<endl;

	for(int i=0;i<number_of_points;i++)
	{
		//print points from 2d points file
		printf("%c %f %f\n",vs[i],point_x[i],point_y[i]);

		//add points to C2DPointSet ptSet
		ptSet.AddCopy(point_x[i],point_y[i]);
	}

	//create polygon from scanned coordinates
	myPoly.Create(ptSet,false);
	//create convex hull of polygon created
	myCH.CreateConvexHull(myPoly);
	//count CH number of Points
	number_Of_CH_Points=myCH.GetPointsCount();

	//create vector of polygon line-segments
	for (int i=0;i<number_of_points;i++)
	{
		cur_line.Set(*ptSet.GetAt(i),*ptSet.GetAt((i+1)%number_of_points));
		lines_of_Polygon.push_back(cur_line);
	}
	//create vector of CH lines same way
	for (int i=0;i<number_Of_CH_Points;i++)
	{
		cur_line.Set(*myCH.GetPoint(i),*myCH.GetPoint((i+1)%number_Of_CH_Points));
		lines_of_CH.push_back(cur_line);
	}
	
	//create vector of lines which are part of CH but not part of the polygon
	bool flag=false;
	for (int i=0;i<number_Of_CH_Points;i++)
	{
		p1=lines_of_CH[i].GetPointFrom();
		p2=lines_of_CH[i].GetPointTo();
		for(int j=0;j<number_of_points;j++)
		{
			p3=lines_of_Polygon[j].GetPointFrom();
			p4=lines_of_Polygon[j].GetPointTo();
			if(    p1==p3   )
			{
				if (p2==p4){
				flag=true; //vector=line is in polygon(we dont want it)
				}
			}
			if(p2==p3){
				if (p1==p4)
				{
					flag=true;
				}
			}
		}
		if(flag==false)
		{	
			lines_of_CH_not_of_Polygon.push_back(lines_of_CH[i]);
			number_of_lines_of_CH_not_of_Polygon++;
		}
		flag=false;
	}


	//create vector of lines which are part of the polygon(lines_inside_CH) but not part of CH
	bool flag2=false;
	for (int i=0;i<number_of_points;i++)
	{
		p1=lines_of_Polygon[i].GetPointFrom();
		p2=lines_of_Polygon[i].GetPointTo();
		for(int j=0;j<number_Of_CH_Points;j++)
		{
			p3=lines_of_CH[j].GetPointFrom();
			p4=lines_of_CH[j].GetPointTo();
			if(    p1==p3   )
			{
				if (p2==p4){
				flag2=true; //line is in ch(we dont want it)
				}
			}
			if(p2==p3){
				if (p1==p4)
				{
					flag2=true;
				}
			}
		}
		if(flag2==false)
		{
			//lines_inside_ch
			lines_inside_CH.push_back(lines_of_Polygon[i]);
			number_of_lines_inside_CH++;
		}
		flag2=false;
	}


	//create a vector for each line of CH not in polygon and split it and create rays
	for(int i=0;i<number_of_lines_of_CH_not_of_Polygon;i++)
	{
		vector<C2DLine> row;
		
		cur_line=lines_of_CH_not_of_Polygon[i];
		//Here we set the precision and the number of rays per line
		for(double cnt=0.001;cnt<0.99;cnt+=0.0001)
		{
			cur_line.Set(lines_of_CH_not_of_Polygon[i].GetPointOn(cnt),lines_of_CH_not_of_Polygon[i].GetPointTo());
			//Rotate 90degrees because they are CW
			cur_line.RotateToRight(   1.570796327,cur_line.GetPointFrom()  );
			//Increase their Length to intersect the whole object
			cur_line.SetLength(100);
			//push back the new ray
			row.push_back(cur_line);
		}
		//push back all of the rays for the i line
		rotated_lines_of_each_external_line.push_back(row);
	}
	int winner=0;
	double min=10000000;
	bool test=false;

	for(int i=0;i<number_of_lines_of_CH_not_of_Polygon;i++)
	{
		vector<C2DPoint> row1;
		for(int j=0;j<rotated_lines_of_each_external_line[i].size() ;j++)
		{
			for(int z=0;z<number_of_lines_inside_CH;z++)
			{
				test=lines_inside_CH[z].CrossesRay(rotated_lines_of_each_external_line[i][j],&intersectPts);
				if(test==true)
				{
					for(int t=0;t<intersectPts.size();t++)
					{
						pp.push_back(*intersectPts.GetAt(t));
					}
				}
				test=false;
			}
			intersectPts.RemoveAll();
			for(int k=0;k<pp.size();k++)
			{
				if( pp[k].Distance(rotated_lines_of_each_external_line[i][j].GetPointFrom()) <min )
				{
					winner=k;
					min=pp[k].Distance(rotated_lines_of_each_external_line[i][j].GetPointFrom());
				}
			}
			rotated_lines_of_each_external_line[i][j].SetPointTo(pp[winner]);
			min=10000000;
			row1.push_back(pp[winner]);
			pp.clear();
			winner=0;
		}

		intersection_pts_vector.push_back(row1);
	}
	
	for(int i=0;i<number_of_lines_of_CH_not_of_Polygon;i++)
	{
		for(int j=0;j<rotated_lines_of_each_external_line[i].size() ;j++)
		{
			lengths.push_back(rotated_lines_of_each_external_line[i][j].GetPointFrom().Distance(rotated_lines_of_each_external_line[i][j].GetPointTo()));
		}
	}

	//find max and min lengths
	max2=-2.0;
	min2=100000;
	for(int i=0;i<lengths.size();i++)
	{
		if(lengths[i]>max2)
		{
			max2=lengths[i];
		}
	}
	
	for(int i=0;i<lengths.size();i++)
	{
		if(lengths[i]<min2)
		{
			min2=lengths[i];
		}
	}

	
	/*cout<<number_Of_CH_Points<<endl;
	cout<<number_of_lines_of_CH_not_of_Polygon;*/


}
