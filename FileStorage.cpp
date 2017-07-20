#include "opencv2/opencv.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cmath>


using namespace cv;
using namespace std;

struct Marker{
	int id;
	vector<Point3d> corners3D;
};

void save_distance(vector<Marker> markers, int id);

int main()
{
	char yml[50];
	int id;
	cout<<"yml name,EX:home.yml\n";
	cin>>yml;
	cout<<"Main ID position(in order, 2 6 8 10, if you want the distance regarding 8, is the position 2)\n";
 	cin>>id;
	FileStorage fs(yml, FileStorage::READ);
	if(!fs.isOpened()){
		cout<<"Error\n ";
		exit(1);
	}
	int nmarkers;
	fs["aruco_bc_nmarkers"] >> nmarkers;
	cout << "Loading info from " << nmarkers << " markers\n";

	FileNode node = fs["aruco_bc_markers"];
	FileNodeIterator it = node.begin(), itEnd = node.end();
	vector<Marker> markers;
	vector<vector<double> > corners;
	for( ; it != itEnd; it++){
		Marker m;
		int id = (int)(*it)["id"];
		cout << "\t Marker id: " << id << endl;
		m.id = id;
		(*it)["corners"] >> corners;

		 for(int i=0;i<4;i++){
		 	Point3d p;
		 	p.x = corners[i][0];
		 	p.y = corners[i][1];
		 	p.z = corners[i][2];
		 	cout<<p.x<<" "<<p.y<<" "<<p.z<<endl;
		 	m.corners3D.push_back(p);
		 }
		markers.push_back(m);

	}
	save_distance(markers,id);


	fs.release();
}

void save_distance(vector<Marker> markers, int id){
	ofstream arq;
	char save_file[40];
	cout<<"Save file name\n";
	cin>> save_file;
	arq.open(save_file);
	double norma;
	for(size_t i=0; i<markers.size(); i++){
			if(i==id){
				continue;
			}
				norma =  sqrt(pow((markers[id].corners3D[0].x - markers[i].corners3D[0].x),2)
					+pow((markers[id].corners3D[0].y - markers[i].corners3D[0].y),2)
					+pow((markers[id].corners3D[0].z - markers[i].corners3D[0].z),2));
			arq<<"distance "<< markers[id].id<<"-"<< markers[i].id<<" = "<<norma<<endl;
	}
}
