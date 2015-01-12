
#include "gl/glut.h"   // - An interface and windows 
                       //   management library
#include "visuals.h"   // Header file for our OpenGL functions
#include "GeoLib.h"   // Header file for our OpenGL functions
#include <stdio.h>     // - Just for some ASCII messages
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <algorithm>    // std::random_shuffle
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */              //   management library

#include "src\Math\float3.h" //float3 is a 3d point
#include "src\Geometry\Polygon.h"
#include "src\Geometry\PolyHedron.h"
#include "src\Geometry\LineSegment.h"
#include "src\Geometry\Plane.h"
#include "src\Geometry\Triangle.h"
#include "src\Geometry\Line.h"
#include "src\Geometry\Plane.h"
#include "src\Geometry\Ray.h"
#include "src\Geometry\AABB.h"
#include "src\Geometry\Sphere.h"

const double PI = 3.141592653589793238462643383279502884197;
#define SPACEBAR 32
#define RADIUS 1

using namespace std;

float maxz=-100.0,bb,cc,d,p,dist;
vector<float3> sphere_points;
float3 intersect_point;
vector<float> vector_of_distances;
vector<Polygon> polygons_for_dmap,polygons_not_in_dmap;
vector<int> points_inter_indices;
vector<float3> points_inters;
float3 center_of_Sphere;
float minz;
float sfairax,sfairay,sfairaz;
Sphere sfaira;
int draw_facet,ind;
bool flagz=false;
vector<LineSegment> final_rays;
Ray ray_line;
Polygon temp_polygon;
vector<Polygon> polygons_of_ch;
C2DPointSet ptSet;
float radius;
float3 center;
vector<float> distances_of_faces_from_object;
vector<Triangle> triangles_of_object;
Triangle temp_triangle;
vector<double> point_x,point_y,point_z;
vector<int> point_a,point_b,point_c,ints,test,non_visible_facets_indexes,visible_facets_indexes,array_indexes;
vector<char> vs,fs;
vector<Polygon> facets,all_visible_polyfacets;
vector<LineSegment> vector_of_horizon_lines,all_lineSegments;
vector<Polyhedron::Face> vector_of_facets;
vector<pair<int,int> >  vector_of_verticesPairs,unique_vector_of_vertices_pairs;
vector<pair<int,int> >  vector_of_remaining_verticesPairs;
vector<float3> array_points,points_3D,new_points_3D,points_of_new_faces,current_set_of_vertices;
bool flag1=false,colFlag;
double first_scanned_point,second_scanned_point,third_scanned_point;
int draw_axis,draw_map,draw_ch,draw_aabb,draw_flag=0,pp1,pp2,counter,a,b,c,number_of_facets,number_of_points,deiktis_4ou_simeiou=0,deiktis_3ou_simeiou=0;
char v;
AABB bounding_box;
float3 p1,p2,p3,p4,pt,pointnumber1;
LineSegment current_line_p1_p2;
Line current_line_p1_p2_new;
Polyhedron initial_Polyhedron_p1_p2_p3_p4,test_polyhedron;
Polyhedron::Face facet1,facet2,facet3,facet4,temp_facet;
vector<Polyhedron::Face> final_faces;
Polygon cur_facet,p_facet1,polyg1,polyg2,polygon_p1_p2_p3;
Plane plane_p1_p2_p3;
Polyhedron polyhedron_of_boundingbox;

//======================================================
// GLOBAL VARIABLES WRITTEN TO BY displayCallBack( )
//======================================================
int display_count=0; //keeps count of invocations

//======================================================
// GLOBAL VARIABLES WRITTEN TO BY keyboardCallBack( )
//======================================================
float theta =0.0; // Rotation angle of square 
float yeta =0.0; // Rotation angle of square 
float xeta =0.0; // Rotation angle of square 
double zoom=1.0;
int timer_period_in_ms=100; // time between timer callbacks in milliseconds


void specialCallBack(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_F1:
			if(draw_facet==0)draw_facet=1;
			else if(draw_facet==1)draw_facet=0;
			break;
		case GLUT_KEY_F2:
			if(draw_ch==1)
				draw_ch=0;
			else if(draw_ch==0)
				draw_ch=1;
			break;
		case GLUT_KEY_F3:
			if(draw_flag==0)draw_flag=1;
			else if(draw_flag==1)draw_flag=0;
			break;

		case GLUT_KEY_F4:
			if(draw_map==1)
			draw_map=0;
			else if(draw_map==0)
				draw_map=1;
			break;
		case GLUT_KEY_F5:
			if(draw_aabb==0)draw_aabb=1;
			else if(draw_aabb==1)draw_aabb=0;
			break;
		case GLUT_KEY_F6:
			if(draw_axis==1)
				draw_axis=0;
			else if(draw_axis==0)
				draw_axis=1;
		default:
			;
	}
	glutPostRedisplay();
}
void keyboardCallBack(unsigned char key, int x, int y)
{
	switch(key)
	{
	case SPACEBAR:
		zoom=1.0;
		theta=0;
		xeta=0;
		yeta=0;
		break;
	case 'i':
		zoom = zoom + 0.3;
		break;
	case 'o':
		zoom = zoom - 0.3;
		break;
	case 'r':
		theta = theta + 5.0;
		break;
	case 't':
		theta = theta - 5.0;
		break;
	case 's':
		yeta = yeta + 5.0;
		break;
	case 'f':
		yeta = yeta - 5.0;
		break;
	case 'd':
		xeta = xeta - 5.0;
		break;
	case 'e':
		xeta = xeta + 5.0;
		break;
	case 'g':
		sfairax-=0.5;
		break;
    case 'j':
		sfairax+=0.5;
		break;
	case 'y':
		sfairay+=0.5;
		break;
	case 'h':
		sfairay-=0.5;
		break;
	case 'k':
		sfairaz-=0.5;
		break;
	case 'l':
		sfairaz+=0.5;
		break;
	default:
		printf("Press E-D or S-F or R-T for Rotation\n");
		printf("Press I-O for Zoom In-Zoom Out\n");
		printf("Press F1 to draw 3D Object as lines\n");
		printf("Press F2 to draw Convex Hull 3D\n");
		printf("Press F3 to change convex hull triangles to lines\n");
		printf("Press F4 to draw distance Map\n");
		printf("Press F5 to draw AABB of object\n");
		printf("Press F6 to draw Axis\n");
		
		printf("Press Spacebar to Reset\n");
	}
	glutPostRedisplay();
}

void Render()
{    
  
  glClear(GL_COLOR_BUFFER_BIT  );  // Clean up the colour of the window
  // and the depth buffer
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

//=============GEOLIB================
	
	/*glRotated(165,0,1,0);
	glTranslated(1,0,0);
	glScaled(3,3,3);*/
    
	glRotatef(theta, 0, 0, 1.0);
	glRotatef(yeta,0,1.0,0);
	glRotatef(xeta,1.0,0,0);
	glScaled(zoom,zoom,zoom);

	//Draw Points of object
	glColor3f(0.2, 0.5, 0.9);
	glBegin(GL_POINTS);
	for (int i=0;i<number_of_points;i++)
	{
		glVertex3f(points_3D[i].x,points_3D[i].y,points_3D[i].z);
	}
	glEnd();

	//DRAW CONVEX HULL 3D
	if (draw_ch==1)
	{
		glColor3f(0.8, 0.8, 0.1);
		if(draw_flag==1)glBegin(GL_LINES);
		else if(draw_flag==0)glBegin(GL_TRIANGLES);
		for (int i=0;i<initial_Polyhedron_p1_p2_p3_p4.f.size();i++)
		{
		
			glVertex3f(initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[0]].x,initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[0]].y,initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[0]].z);
			glVertex3f(initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[1]].x,initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[1]].y,initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[1]].z);
			glVertex3f(initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[2]].x,initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[2]].y,initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[2]].z);
		}
		glEnd();
	}

	/*glColor3f(0.8, 0.5, 0.8);
	glPointSize(5);
	glBegin(GL_POINTS);
	for (int i=0;i<initial_Polyhedron_p1_p2_p3_p4.f.size();i++)
	{
		
		glVertex3f(initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[0]].x,initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[0]].y,initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[0]].z);
		glVertex3f(initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[1]].x,initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[1]].y,initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[1]].z);
		glVertex3f(initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[2]].x,initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[2]].y,initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[2]].z);
	}
	glEnd();
*/


	//DRAW FACES OF OBJECT
	glColor3f(0.2, 0.5, 0.9);
	if(draw_facet==1)
	{
		glBegin(GL_LINES);
		for (int i=0;i<facets.size();i++)
		{
			glVertex3f(facets[i].p[0].x,facets[i].p[0].y,facets[i].p[0].z);
			glVertex3f(facets[i].p[1].x,facets[i].p[1].y,facets[i].p[1].z);
			glVertex3f(facets[i].p[1].x,facets[i].p[1].y,facets[i].p[1].z);
			glVertex3f(facets[i].p[2].x,facets[i].p[2].y,facets[i].p[2].z);
			glVertex3f(facets[i].p[2].x,facets[i].p[2].y,facets[i].p[2].z);
			glVertex3f(facets[i].p[0].x,facets[i].p[0].y,facets[i].p[0].z);
		}
	}
	else if(draw_facet==0)
	{
		glBegin(GL_TRIANGLES);
		for (int i=0;i<facets.size();i++)
		{
			glVertex3f(facets[i].p[0].x,facets[i].p[0].y,facets[i].p[0].z);
			glVertex3f(facets[i].p[1].x,facets[i].p[1].y,facets[i].p[1].z);
			glVertex3f(facets[i].p[2].x,facets[i].p[2].y,facets[i].p[2].z);
		}
	}
	glEnd();



	//DRAW AABB
	if (draw_aabb==1)
	{
		glColor3f(0.9, 0.2, 0.1);
		glBegin(GL_LINE_LOOP);
		for (int i=0;i<polyhedron_of_boundingbox.f.size();i++)
		{
			for( int j=0;j<polyhedron_of_boundingbox.f[i].v.size();j++ )
			{
				glVertex3f(polyhedron_of_boundingbox.v[polyhedron_of_boundingbox.f[i].v[j]].x,polyhedron_of_boundingbox.v[polyhedron_of_boundingbox.f[i].v[j]].y,polyhedron_of_boundingbox.v[polyhedron_of_boundingbox.f[i].v[j]].z);
			}
		
		}
		glEnd();
	}
	
	//DRAW AXIS
	if (draw_axis==1){
		glColor3f(1.0, 1.0, 1.0);
		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(40.0,0.0,0.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,40.0,0.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,0.0,40.0);
		glEnd();
	}

	if (draw_map==1){
		//DRAW DISTANCE MAP
		glColor3ub(255, 255, 255);
	
		glBegin(GL_TRIANGLES);
		for (int i=0;i<polygons_for_dmap.size();i++)
		{
			d=final_rays[i].Length();
			
			p=1-((maxz-d)/maxz);
			bb=cc=255-(p*255);
			glColor3ub(255, bb, cc);
			/*glVertex3f(final_rays[i].a.x,final_rays[i].a.y,final_rays[i].a.z);
			glVertex3f(final_rays[i].b.x,final_rays[i].b.y,final_rays[i].b.z);*/
			glVertex3f(polygons_for_dmap[i].p[0].x,polygons_for_dmap[i].p[0].y,polygons_for_dmap[i].p[0].z);
			glVertex3f(polygons_for_dmap[i].p[1].x,polygons_for_dmap[i].p[1].y,polygons_for_dmap[i].p[1].z);
			glVertex3f(polygons_for_dmap[i].p[2].x,polygons_for_dmap[i].p[2].y,polygons_for_dmap[i].p[2].z);

		}
		glEnd();

		glShadeModel(GL_SMOOTH);
		glColor3ub(255, 0, 0);
		glBegin(GL_TRIANGLES);
		for (int i=0;i<polygons_not_in_dmap.size();i++)
		{
			glVertex3f(polygons_not_in_dmap[i].p[0].x,polygons_not_in_dmap[i].p[0].y,polygons_not_in_dmap[i].p[0].z);
			glVertex3f(polygons_not_in_dmap[i].p[1].x,polygons_not_in_dmap[i].p[1].y,polygons_not_in_dmap[i].p[1].z);
			glVertex3f(polygons_not_in_dmap[i].p[2].x,polygons_not_in_dmap[i].p[2].y,polygons_not_in_dmap[i].p[2].z);
		}
		glEnd();
	}
	colFlag=false;
	center.Set(sfairax,sfairay,sfairaz);
    // Iterate through phi, theta then convert r,theta,phi to  XYZ
    for (double phi = 0.; phi < 2*PI; phi += PI/100.) // Azimuth [0, 2PI]
    {
        for (double theta = 0.; theta < PI; theta += PI/100.) // Elevation [0, PI]
        {
            float3 point;
            point.x = RADIUS * cos(phi) * sin(theta) + center.x;
            point.y = RADIUS * sin(phi) * sin(theta) + center.y;
            point.z = RADIUS            * cos(theta) + center.z;
            sphere_points.push_back(point);        
        }
    }
	/*for(int j=0;j<polygons_of_ch.size();j++)
	{*/
			if(initial_Polyhedron_p1_p2_p3_p4.Distance(center)<= RADIUS)
			{
				colFlag=true;
				//break;
			}
	//}
	
	if (colFlag==true)
	{
		glColor3f(0.9,0.1,0.1);
	}
	else
	{
		glColor3f(0.5,0.5,0.5);
	}
	
	glBegin(GL_POINTS);
	glPointSize(3);
	for (int i=0;i<sphere_points.size();i++)
	{
		glVertex3f(sphere_points[i].x,sphere_points[i].y,sphere_points[i].z);	
	}
	sphere_points.clear();
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


void Setup()  // DON'T TOUCH IT 
{ 
	//Parameter handling
	//glShadeModel (GL_SMOOTH);
	glShadeModel (GL_FLAT);

	// polygon rendering mode
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  
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

	//borders of screen
	C2DRect *myRect = new C2DRect(-50,50,50,-50);

	int no_poi=0,no_fac=0,cnt=0;
	char etc;

	ifstream myfile ("3D_2.OBJ");

	if (myfile.is_open())
	{
		//Scan Object
		 no_poi=0;
		 no_fac=0;
		 while ( !myfile.fail() )
		     {
			 	 myfile>>v;
				 //only accept points with a v in front of coordinates
				 if ((strcmp("v",&v)==0) )
				 {
					 vs.push_back(v);

			 		 myfile>>first_scanned_point;				 
					 point_x.push_back((first_scanned_point));  
			 		 myfile>>second_scanned_point;
					 point_y.push_back((second_scanned_point));  
					 myfile>>third_scanned_point;
					 point_z.push_back((third_scanned_point)); 
					 no_poi++; 
				 }
				 
				 //Scan Faces with f in front of integers
				 if ((strcmp("f",&v)==0) )
				 {
					 fs.push_back(v);

			 		 myfile>>a;		
					 point_a.push_back((a));   
			 		 myfile>>b;					
					 point_b.push_back((b));  
					 myfile>>c;
					 point_c.push_back((c));   
					 no_fac++;
				 }
		     }
			 number_of_points=no_poi;
			 number_of_facets=no_fac-1;			 
	}
	else 
	{
		cout << "Unable to open file";
	}
	
	//Print number of Points and number of Facets
	cout<<"Total number of 3d Points: "<<number_of_points<<endl;
	cout<<"Total number of object facets(polygons): "<<number_of_facets<<endl;
	
	//create vector points_3D which includes every single point of our scanned object
	for(int i=0;i<number_of_points;i++)
	{
		//print points from 2d points file
		//printf("%c %f %f %f\n",vs[i],point_x[i],point_y[i],point_z[i]);

		pt.Set(point_x[i],point_y[i],point_z[i]);
		//points_3D is a vector of Points(object)
		points_3D.push_back(pt);
	}
	
	//delete last elements due to scan error
	point_a.pop_back();
	point_b.pop_back();
	point_c.pop_back();

	
	//create vector of facets which includes every facet of our object as a polygon
	for(int counter1=0;counter1<number_of_facets;counter1++)
	{
		//create current vector of  3 points
		current_set_of_vertices.push_back(points_3D[point_a[counter1]-1]);
		current_set_of_vertices.push_back(points_3D[point_b[counter1]-1]);
		current_set_of_vertices.push_back(points_3D[point_c[counter1]-1]);
		//copy vector of points to current facet's vector of points
		//cur_facet.p is a vector of points
		cur_facet.p=current_set_of_vertices;
		//clear cur_facet;
		current_set_of_vertices.clear();
		//push back new facet to our vector
		//facets is a vector of Polygons
		facets.push_back(cur_facet);
	}

	//******//
	//Start of Incremental 3D Convex Hull Algorithm
	clock_t startClock = clock();

	p1.Set(points_3D[0].x,points_3D[0].y,points_3D[0].z);  //1st point of initial tetrahedron
	p2.Set(points_3D[1].x,points_3D[1].y,points_3D[1].z);  //2nd point of initial tetrahedron

	current_line_p1_p2.a=p1;  //start of current line segment
	current_line_p1_p2.b=p2;  //end of current line segment
	current_line_p1_p2_new=current_line_p1_p2.ToLine();//change linesegment to Line(infinite)

	for(int counter2=2;counter2<points_3D.size();counter2++)
	{
		if( !( current_line_p1_p2_new.Contains(points_3D[counter2],0.00001) ) )
		{
			p3.Set(points_3D[counter2].x,points_3D[counter2].y,points_3D[counter2].z);  //3rd point of initial tetrahedron
			deiktis_3ou_simeiou=counter2;
			break;
		}
	}
	
	//polygon of p1,p2,p3
	polygon_p1_p2_p3.p.push_back(p1);
	polygon_p1_p2_p3.p.push_back(p2);
	polygon_p1_p2_p3.p.push_back(p3);
	plane_p1_p2_p3=polygon_p1_p2_p3.PlaneCCW();
	for(int counter3=2; counter3<points_3D.size(); counter3++)
	{
		if( counter3!=deiktis_3ou_simeiou)
		{
			if( !(plane_p1_p2_p3.Contains(points_3D[ counter3],0.000001)) )
			{
				p4.Set(points_3D[ counter3].x,points_3D[ counter3].y,points_3D[ counter3].z);  //4th point of initial tetrahedron
				deiktis_4ou_simeiou= counter3;
				break;
			}
		}
	}

	//sto new_points_3D egkathisto ola ta simeia ektos apo ta 4 prota pou sximatizoun to arxiko CH polyedro
	new_points_3D=points_3D;
	
	//diagrafo ta simeia tou arxikou 4edrou
	new_points_3D.erase(new_points_3D.begin(),new_points_3D.begin()+2);
	new_points_3D.erase(new_points_3D.begin()+(deiktis_3ou_simeiou-2));
	new_points_3D.erase(new_points_3D.begin()+(deiktis_4ou_simeiou-3));
	
	random_shuffle( new_points_3D.begin(), new_points_3D.end() );
	//sto  vector initial_Polyhedron_p1_p2_p3_p4.v exoume ola ta points tou arxikou poluedrou
	initial_Polyhedron_p1_p2_p3_p4.v.push_back(p1);
	initial_Polyhedron_p1_p2_p3_p4.v.push_back(p2);
	initial_Polyhedron_p1_p2_p3_p4.v.push_back(p3);
	initial_Polyhedron_p1_p2_p3_p4.v.push_back(p4);

	//initial_Polyhedron_p1_p2_p3_p4.f.push_back(face)
	facet1.v.push_back(0);
	facet1.v.push_back(1);
	facet1.v.push_back(2);

	facet2.v.push_back(0);
	facet2.v.push_back(1);
	facet2.v.push_back(3);

	facet3.v.push_back(0);
	facet3.v.push_back(2);
	facet3.v.push_back(3);

	facet4.v.push_back(1);
	facet4.v.push_back(2);
	facet4.v.push_back(3);

	//sto vector initial_Polyhedron_p1_p2_p3_p4.f exoume faces,kathe face einai vector apo ints ton deikton
	initial_Polyhedron_p1_p2_p3_p4.f.push_back(facet1); //f[0]=facet1
	initial_Polyhedron_p1_p2_p3_p4.f.push_back(facet2); //f[1]=facet2
	initial_Polyhedron_p1_p2_p3_p4.f.push_back(facet3); //--//--
	initial_Polyhedron_p1_p2_p3_p4.f.push_back(facet4);

	//gia kathe face prostheto tous deiktes ston array_indexes
	//for(int i=0;i<initial_Polyhedron_p1_p2_p3_p4.NumFaces();i++)
	//{
	//	for(int j=0;j<initial_Polyhedron_p1_p2_p3_p4.f[i].v.size();j++)
	//	{
	//		array_indexes.push_back(initial_Polyhedron_p1_p2_p3_p4.f[i].v[j]);
	//		//printf("Arxika: %d",initial_Polyhedron_p1_p2_p3_p4.f[i].v[j]);
	//	}
	//}
	
	for(int counter4=0;counter4<initial_Polyhedron_p1_p2_p3_p4.NumVertices();counter4++)
	{
		array_points.push_back(initial_Polyhedron_p1_p2_p3_p4.v[counter4]);  //array of points
	}
	
	//Me tin parakato entoli to vector v gia kathe face anadiorganonetai etsi oste kathe face na exei to normal tou pros ta ekso
	//**************************SOS*********************//
	initial_Polyhedron_p1_p2_p3_p4.OrientNormalsOutsideConvex();   //HERE!!!
	//**************************SOS*********************//

	
	//epistrefei san polygono to face tou polyedrou pou tou zito me vasi index px 0= to proto face tou poluedrou
	//p_facet1=initial_Polyhedron_p1_p2_p3_p4.FacePolygon(3);
	
	for(int i=0;i<new_points_3D.size();i++)
	{
		if(initial_Polyhedron_p1_p2_p3_p4.Contains(new_points_3D[i]))
		{
			//AN TO NEO SIMEIO VRISKETAI MESA STO POLYEDRO MAS DEN KANOUME KATI
			continue;
		}
		else
		{
				//AN TO NEO SIMEIO DEN EINAI MESA STO POLYEDRO MAS,TOTE KOITAO POIES EDRES VLEPEI TO NEO SIMEIO
				for(int counter5=0; counter5<initial_Polyhedron_p1_p2_p3_p4.NumFaces(); counter5++)
				{
					Plane plane=initial_Polyhedron_p1_p2_p3_p4.FacePlane(counter5);
					if( plane.IsOnPositiveSide(new_points_3D[i]) )
					{
						visible_facets_indexes.push_back(counter5);
						//periexei deiktes pros ta faces tou poluedrou pou einai visible
						//to facet auto fainetai apo to simeio
						// PX AN TO visiblae_facets_indexes PERIEXEI [0,1,2] SIMAINEI OTI TA FACETS 0,1,2 EINAI VISIBLE APO TO SIMEIO
					}
					else
						//AN TO NEO SIMEIO DEN EINAI MESA STO POLYEDRO MAS,TOTE KOITAO POIES EDRES DEN VLEPEI TO NEO SIMEIO
					{
						non_visible_facets_indexes.push_back(counter5);
						//periexei deiktes pros ta faces tou poluedrou pou einai DEN EINAI VISIBLE
						//to facet den fainetai apo to simeio kai ara den diagrafetai alla
						//paramenei sto epomeno CH os exei
					}
				}
				//GIA KATHE VISIBLE EDRA ftiakse vector me tis korufes ana line se zeugaria
				for(int counter6=0;counter6<visible_facets_indexes.size();counter6++)
				{
					vector_of_verticesPairs.push_back( make_pair(initial_Polyhedron_p1_p2_p3_p4.f[visible_facets_indexes[counter6]].v[0],initial_Polyhedron_p1_p2_p3_p4.f[visible_facets_indexes[counter6]].v[1]) );
					vector_of_verticesPairs.push_back( make_pair(initial_Polyhedron_p1_p2_p3_p4.f[visible_facets_indexes[counter6]].v[1],initial_Polyhedron_p1_p2_p3_p4.f[visible_facets_indexes[counter6]].v[2]) );
					vector_of_verticesPairs.push_back( make_pair(initial_Polyhedron_p1_p2_p3_p4.f[visible_facets_indexes[counter6]].v[2],initial_Polyhedron_p1_p2_p3_p4.f[visible_facets_indexes[counter6]].v[0]) );
				}

				flag1=false;
				pp1=0;
				pp2=0;
				for(int counter7=0;counter7<vector_of_verticesPairs.size();counter7++)
				{
						for(int counter9=0;counter9<vector_of_verticesPairs.size();counter9++)
						{
							if(counter9!=counter7)
							{
								if( (vector_of_verticesPairs[counter7].first==vector_of_verticesPairs[counter9].first && vector_of_verticesPairs[counter7].second==vector_of_verticesPairs[counter9].second) || (vector_of_verticesPairs[counter7].first==vector_of_verticesPairs[counter9].second && vector_of_verticesPairs[counter7].second==vector_of_verticesPairs[counter9].first))
								{
									flag1=true;
								}
							}
						}
						if (flag1==false)
						{
							vector_of_remaining_verticesPairs.push_back(vector_of_verticesPairs[counter7]);
						}
						flag1=false;
					
				}
				
			
				
				
				
				for(int counter8=0;counter8<non_visible_facets_indexes.size();counter8++)
				{

					final_faces.push_back(initial_Polyhedron_p1_p2_p3_p4.f[non_visible_facets_indexes[counter8]]);
					
				}

				//prostheto to simeio sta simeia tou poluedrou mas
				initial_Polyhedron_p1_p2_p3_p4.v.push_back(new_points_3D[i]);
				initial_Polyhedron_p1_p2_p3_p4.f.clear();


				initial_Polyhedron_p1_p2_p3_p4.f=final_faces;
				for(int l=0;l<vector_of_remaining_verticesPairs.size();l++)
				{
					temp_facet.v.push_back(initial_Polyhedron_p1_p2_p3_p4.v.size()-1);
					temp_facet.v.push_back(vector_of_remaining_verticesPairs[l].first);
					temp_facet.v.push_back(vector_of_remaining_verticesPairs[l].second);
					initial_Polyhedron_p1_p2_p3_p4.f.push_back(temp_facet);
					temp_facet.v.clear();
				}

				//Orient normals outside convex for place intersection to work
				initial_Polyhedron_p1_p2_p3_p4.OrientNormalsOutsideConvex();

				//clear created vector for new point procedure
				visible_facets_indexes.clear();
				non_visible_facets_indexes.clear();
				vector_of_verticesPairs.clear();
				vector_of_remaining_verticesPairs.clear();
				final_faces.clear();
		}

		
	}
	cout<<"Convex Hull number of Points: "<<initial_Polyhedron_p1_p2_p3_p4.v.size()<<endl;
	clock_t endClock = clock();
	printf("Algorithm Execution Time:  %f Seconds\n", (double)(endClock - startClock)/CLOCKS_PER_SEC) ;
	cout<<"Convex Hull number of faces: "<<initial_Polyhedron_p1_p2_p3_p4.f.size()<<endl;
	bounding_box=initial_Polyhedron_p1_p2_p3_p4.MinimalEnclosingAABB();
	polyhedron_of_boundingbox=bounding_box.ToPolyhedron();

	//CREATE TRIANGLES FROM OBJECT FACETS
	for(int i=0;i<facets.size();i++)
	{
		temp_triangle.a=facets[i].p[0];
		temp_triangle.b=facets[i].p[1];
		temp_triangle.c=facets[i].p[2];
		triangles_of_object.push_back(temp_triangle);
	}

	//CREATE POLYGONS FROM CONVEX HULL 3D
	for(int i=0;i<initial_Polyhedron_p1_p2_p3_p4.f.size();i++)
	{
		temp_polygon.p.push_back(initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[0]]);
		temp_polygon.p.push_back(initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[1]]);
		temp_polygon.p.push_back(initial_Polyhedron_p1_p2_p3_p4.v[initial_Polyhedron_p1_p2_p3_p4.f[i].v[2]]);
		polygons_of_ch.push_back(temp_polygon);
		temp_polygon.p.clear();
	}

	
	for(int i=0;i<polygons_of_ch.size();i++)
	{
		//Create rayLine starting from centroid of CH polygon and direction of NormalCW
		ray_line.pos=polygons_of_ch[i].Centroid();
		ray_line.dir=polygons_of_ch[i].NormalCW();
		//For each triangle of object check intersection points
		for(int j=0;j<triangles_of_object.size();j++)
		{
			if((ray_line.Intersects(triangles_of_object[j],&dist,&intersect_point)) && dist!=0.0)
			{
				flagz=true;
				vector_of_distances.push_back(dist);
				points_inters.push_back(intersect_point);
			}
		}
		minz=10000.0;
		//If there were 1 or more intersections acquire the one with the minimum length
		if(flagz==true)
		{
			for(int k=0;k<points_inters.size();k++)
			{
				if(vector_of_distances[k]<minz)
				{
					minz=vector_of_distances[k];
					ind=k;
				}
			}
			distances_of_faces_from_object.push_back(minz);
			final_rays.push_back( LineSegment(polygons_of_ch[i].Centroid(),points_inters[ind]) );
			polygons_for_dmap.push_back(polygons_of_ch[i]);
			points_inters.clear();
			vector_of_distances.clear();
		}
		else
		{
			distances_of_faces_from_object.push_back(0.0);
			polygons_not_in_dmap.push_back(polygons_of_ch[i]);
		}
		flagz=false;
	}
	
	for(int i=0;i<final_rays.size();i++)
	{
		if(final_rays[i].Length()>maxz)
		{
			maxz=final_rays[i].Length();
		}
	}	
	

	sfairax=20.0;
	sfairay=0.0;
	sfairaz=0.0;
	
	//radius=0.5;
	center.Set(sfairax,sfairay,sfairaz);
    // Iterate through phi, theta then convert r,theta,phi to  XYZ
    for (double phi = 0.; phi < 2*PI; phi += PI/100.) // Azimuth [0, 2PI]
    {
        for (double theta = 0.; theta < PI; theta += PI/100.) // Elevation [0, PI]
        {
            float3 point;
            point.x = RADIUS * cos(phi) * sin(theta) + center.x;
            point.y = RADIUS * sin(phi) * sin(theta) + center.y;
            point.z = RADIUS            * cos(theta) + center.z;
            sphere_points.push_back(point);        
        }
    }
	
//===================================================================
}

