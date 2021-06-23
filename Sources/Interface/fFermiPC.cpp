//---------------------------------------------------------------------------
#define MULTICOEF 1
#include <vcl.h>
#include <cstdlib>
#include <ctime>
#include <typeinfo>
#include <fstream>
#include <string>
#pragma hdrstop

#include "fFermiPC.h"
#include "fFermiPCTable.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "GLS.BaseClasses"
#pragma link "GLS.Cadencer"
#pragma link "GLS.Coordinates"
#pragma link "GLS.Objects"
#pragma link "GLS.Scene"
#pragma link "GLS.SceneViewer"
#pragma link "GLS.GeomObjects"
#pragma link "GLS.VectorFileObjects"
#pragma link "GLS.Mesh"
#pragma link "GLS.SimpleNavigation"
#pragma resource "*.dfm"
TFormScene *FormScene;

int mx, my; // vars for saving position

int*** voxelData;
VoxelCube** voxels;
DelaunayBase* Points;

// All Delaunay structs
DelaunayBase All_Delaunay;
DelaunayBase O_Delaunay;
DelaunayBase B_Delaunay;
DelaunayBase A_Delaunay;
DelaunayBase F_Delaunay;
DelaunayBase G_Delaunay;
DelaunayBase K_Delaunay;
DelaunayBase M_Delaunay;

// All Voronoi structs
VoronoiBase All_Voronoi;
VoronoiBase O_Voronoi;
VoronoiBase B_Voronoi;
VoronoiBase A_Voronoi;
VoronoiBase F_Voronoi;
VoronoiBase G_Voronoi;
VoronoiBase K_Voronoi;
VoronoiBase M_Voronoi;

// Colors for each star class
float lightblue[3] = {0, 0.8, 1};
float skyblue[3] = {0.803, 1, 1};
float white[3] = {1, 1, 1};
float lightyellow[3] = {0.996, 1, 0.6};
float yellow[3] = {1, 1, 0.003};
float orange[3] = {1, 0.4, 0};
float red[3] = {0.992, 0, 0.003};

String datapath;


//---------------------------------------------------------------------------

void __fastcall TFormScene::FormCreate(TObject *Sender)
{
	datapath = GetCurrentDir();
	datapath = ".\\..\\..\\..\\DATA\\";

	FormScene->Caption = "HYG 3D | Stars mode";

	Data1->Enabled = false;

	clbMethods->Checked[0] = true;
}
//---------------------------------------------------------------------------
// Voxel
VoxelCube*** InitVoxel(DelaunayBase* Points, int sizeCube, int sizeVoxel) {
	int size = (sizeCube/sizeVoxel)*2;
	int i,j,t,x,y,z;
	VoxelCube*** voxels;
	voxels = new VoxelCube**[size];
	for (int i = 0; i < size; ++i)
	{
		voxels[i] = new VoxelCube*[size];
		for (int j = 0; j < size; ++j)
			voxels[i][j] = new VoxelCube[size];
	}
	for (int k = 0; k < 7; k++) {
		for (i = 0; i < Points[k].nodeCount; i++) {
			x = (sizeCube+Points[k].node[i][0]) / sizeVoxel;
			y = (sizeCube+Points[k].node[i][1]) / sizeVoxel;
			z = (sizeCube+Points[k].node[i][2]) / sizeVoxel;
			voxels[x][y][z].Increase(k);
		}
	}
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			for (t = 0; t < size; t++) {
				voxels[i][j][t].TakeMostClass();
			}
		}
	}

	return voxels;
}

int*** Voxel(DelaunayBase dt_struct, int sizeCube, int sizeVoxel) {
	int size = (sizeCube/sizeVoxel)*2;
	int i,j,t,x,y,z;
	int***  dataVoxel;
	dataVoxel = new int**[size];
	for (int i = 0; i < size; ++i)
	{
		dataVoxel[i] = new int*[size];
		for (int j = 0; j < size; ++j)
			dataVoxel[i][j] = new int[size];
	}
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			for (t = 0; t < size; t++) {
				dataVoxel[i][j][t] = 0;
			}
        }
	}

	for (i = 0; i < dt_struct.nodeCount; i++) {
		x = (sizeCube+dt_struct.node[i][0]) / sizeVoxel;
		y = (sizeCube+dt_struct.node[i][1]) / sizeVoxel;
		z = (sizeCube+dt_struct.node[i][2]) / sizeVoxel;
		dataVoxel[x][y][z] += 1;
	}

    return dataVoxel;
}

// Init Delaunay struct
DelaunayBase __fastcall TFormScene::InitDelaunay(String filename, float color[])
{
	String quer;
	// Connect to specified DB
	FDConnection1->Connected = false;
	FDConnection1->DriverName = "SQLite";
	FDConnection1->LoginPrompt = false;
	FDConnection1->Params->DriverID = "SQLite";
	FDConnection1->Params->Database = datapath + filename;
	FDConnection1->Connected = true;

	// Init struct for DT
	DelaunayBase dt_struct;

	// Getting all vertices for current class of stars
	FDQuery1->Close();
	FDQuery1->Connection = FDConnection1;
	FDQuery1->SQL->Clear();

	quer = "SELECT X,Y,Z FROM dt_node";
	FDQuery1->SQL->Text = quer;
	FDQuery1->Open();

	// Go to last record, count all vertices and go to first record
	FDQuery1->Last();
	dt_struct.nodeCount = FDQuery1->RecordCount; // DT_node count
	FDQuery1->First();

	dt_struct.node = new double*[dt_struct.nodeCount]; // DT_node
	for (int i = 0; i < dt_struct.nodeCount; ++i)
		dt_struct.node[i] = new double[3];

	// Init nodes with saved coordinates
	FDQuery1->First();
	for (int i = 0; i < dt_struct.nodeCount; i++) {
		dt_struct.node[i][0] = FDQuery1->FieldByName("X")->AsFloat;
		dt_struct.node[i][1] = FDQuery1->FieldByName("Y")->AsFloat;
		dt_struct.node[i][2] = FDQuery1->FieldByName("Z")->AsFloat;
		FDQuery1->Next();
	}
	//-------------------------------------------------------------------------
	// Getting all edges for current class of stars
	FDQuery1->Close();
	FDQuery1->SQL->Clear();
	quer = "SELECT Node1, Node2 FROM dt_edge";
	FDQuery1->SQL->Text = quer;
	FDQuery1->Open();

	// Go to last record, count all edges and go to first record
	FDQuery1->Last();
	dt_struct.edgeCount = FDQuery1->RecordCount; // DT_edge count
	FDQuery1->First();

	dt_struct.edge = new int*[dt_struct.edgeCount]; // DT_edge
	for (int i = 0; i < dt_struct.edgeCount; ++i) {
		dt_struct.edge[i] = new int[2];
	}

	// Init edges
	FDQuery1->First();
	for (int i = 0; i < dt_struct.edgeCount; i++) {
		dt_struct.edge[i][0] = FDQuery1->FieldByName("Node1")->AsInteger;
		dt_struct.edge[i][1] = FDQuery1->FieldByName("Node2")->AsInteger;
		FDQuery1->Next();
	}

	dt_struct.color = color;

    FDQuery1->Close();
	FDQuery1->Connection = FDConnection1;
	FDQuery1->SQL->Clear();

	quer = "SELECT Node1,Node2,Node3,Node4 FROM dt_ele";
	FDQuery1->SQL->Text = quer;
	FDQuery1->Open();

	FDQuery1->Last();
	dt_struct.faceCount = FDQuery1->RecordCount; // DT_node count
	FDQuery1->First();

	TVector4i n;
	for (int i = 0; i < dt_struct.faceCount; i++) {
		n.X = FDQuery1->FieldByName("Node1")->AsInteger;
		n.Y = FDQuery1->FieldByName("Node2")->AsInteger;
		n.Z = FDQuery1->FieldByName("Node3")->AsInteger;
		n.W = FDQuery1->FieldByName("Node4")->AsInteger;
		dt_struct.faceNode.push_back(n);
		FDQuery1->Next();
	}

	return dt_struct;
}
//---------------------------------------------------------------------------
// Init Voronoi struct
VoronoiBase __fastcall TFormScene::InitVoronoi(String filename, float color[])
{
	String quer;
	// Connect to specified DB
	FDConnection1->Connected = False;
	FDConnection1->DriverName = "SQLite";
	FDConnection1->LoginPrompt = False;
	FDConnection1->Params->DriverID = "SQLite";
	FDConnection1->Params->Database = datapath + filename;
	FDConnection1->Connected = True;

	// Init struct for VD
	VoronoiBase vd_struct;

	// Getting all vertices for current class of stars
	FDQuery1->Close();
	FDQuery1->Connection = FDConnection1;
	FDQuery1->SQL->Clear();
	quer = "SELECT X,Y,Z FROM vd_node";
	FDQuery1->SQL->Text = quer;
	FDQuery1->Open();

	// Go to last record, count all vertices and go to first record
	FDQuery1->Last();
	vd_struct.nodeCount = FDQuery1->RecordCount; // VD_node count
	FDQuery1->First();

	vd_struct.node = new double*[vd_struct.nodeCount]; // VD_node
	for (int i = 0; i < vd_struct.nodeCount; ++i)
		vd_struct.node[i] = new double[3];

	// Init nodes with saved coordinates
	FDQuery1->First();
	for (int i = 0; i < vd_struct.nodeCount; i++) {
		vd_struct.node[i][0] = FDQuery1->FieldByName("X")->AsFloat;
		vd_struct.node[i][1] = FDQuery1->FieldByName("Y")->AsFloat;
		vd_struct.node[i][2] = FDQuery1->FieldByName("Z")->AsFloat;
		FDQuery1->Next();
	}
	//-------------------------------------------------------------------------
	// Getting all edges for current class of stars
	FDQuery1->Close();
	FDQuery1->SQL->Clear();
	quer = "SELECT Node1, Node2, X, Y, Z FROM vd_edge";
	FDQuery1->SQL->Text = quer;
	FDQuery1->Open();

	// Go to last record, count all edges and go to first record
	FDQuery1->Last();
	vd_struct.edgeCount = FDQuery1->RecordCount; // VD_edge count
	FDQuery1->First();

	vd_struct.edge = new double*[vd_struct.edgeCount]; // VD_edge
	for (int i = 0; i < vd_struct.edgeCount; ++i) {
		vd_struct.edge[i] = new double[5];
	}

	// Init edges
	FDQuery1->First();
	for (int i = 0; i < vd_struct.edgeCount; i++) {
		vd_struct.edge[i][0] = FDQuery1->FieldByName("Node1")->AsFloat;
		vd_struct.edge[i][1] = FDQuery1->FieldByName("Node2")->AsFloat;

		// Prevent null value instead of float
		if (FDQuery1->FieldByName("X") && \
			FDQuery1->FieldByName("Y") &&
			FDQuery1->FieldByName("Z"))
		{
			vd_struct.edge[i][2] = 0;
			vd_struct.edge[i][3] = 0;
			vd_struct.edge[i][4] = 0;
		}
		else
		{
			vd_struct.edge[i][2] = FDQuery1->FieldByName("X")->AsFloat;
			vd_struct.edge[i][3] = FDQuery1->FieldByName("Y")->AsFloat;
			vd_struct.edge[i][4] = FDQuery1->FieldByName("Z")->AsFloat;
		}
		FDQuery1->Next();
	}

	vd_struct.color = color;

	return vd_struct;
}
//---------------------------------------------------------------------------
void __fastcall TFormScene::DrawPoints()
{
	// Delete all points from the scene
	GLPoints1->Free();
	GLPoints1 = (TGLPoints *)(GLDummyCube1->AddNewChild(__classid(TGLPoints)));

	GLPoints1->Size = 2;
    GLPoints1->Positions->Clear();
	// Delete all lines from the scene
	//GLLines1->Free();
	//GLLines1 = (TGLLines *)(GLDummyCube1->AddNewChild(__classid(TGLLines)));

	// Temp vars for coordinates and colors values
	float X, Y, Z, R, G, B;
		switch (ClassGroup->ItemIndex)
		{
			case 0:
				for (int i = 0; i < All_Delaunay.nodeCount; i++) {
					X = All_Delaunay.node[i][0]*MULTICOEF;
					Y = All_Delaunay.node[i][1]*MULTICOEF;
					Z = All_Delaunay.node[i][2]*MULTICOEF;

					R = All_Delaunay.color[0];
					G = All_Delaunay.color[1];
					B = All_Delaunay.color[2];

					GLPoints1->Positions->Add(X, Y, Z);
					GLPoints1->Colors->AddPoint(R, G, B);
				}
				break;
			case 1:
				for (int i = 0; i < O_Delaunay.nodeCount; i++) {

					X = O_Delaunay.node[i][0]*MULTICOEF;
					Y = O_Delaunay.node[i][1]*MULTICOEF;
					Z = O_Delaunay.node[i][2]*MULTICOEF;

					R = O_Delaunay.color[0];
					G = O_Delaunay.color[1];
					B = O_Delaunay.color[2];

					GLPoints1->Positions->Add(X, Y, Z);
					GLPoints1->Colors->AddPoint(R, G, B);
				}
				break;
			case 2:
				for (int i = 0; i < B_Delaunay.nodeCount; i++) {
					X = B_Delaunay.node[i][0]*MULTICOEF;
					Y = B_Delaunay.node[i][1]*MULTICOEF;
					Z = B_Delaunay.node[i][2]*MULTICOEF;

					R = B_Delaunay.color[0];
					G = B_Delaunay.color[1];
					B = B_Delaunay.color[2];

					GLPoints1->Positions->Add(X, Y, Z);
					GLPoints1->Colors->AddPoint(R, G, B);
				}
				break;
			case 3:
				for (int i = 0; i < A_Delaunay.nodeCount; i++) {
					X = A_Delaunay.node[i][0]*MULTICOEF;
					Y = A_Delaunay.node[i][1]*MULTICOEF;
					Z = A_Delaunay.node[i][2]*MULTICOEF;

					R = A_Delaunay.color[0];
					G = A_Delaunay.color[1];
					B = A_Delaunay.color[2];

					GLPoints1->Positions->Add(X, Y, Z);
					GLPoints1->Colors->AddPoint(R, G, B);
				}
				break;
			case 4:
				for (int i = 0; i < F_Delaunay.nodeCount; i++) {
					X = F_Delaunay.node[i][0]*MULTICOEF;
					Y = F_Delaunay.node[i][1]*MULTICOEF;
					Z = F_Delaunay.node[i][2]*MULTICOEF;

					R = F_Delaunay.color[0];
					G = F_Delaunay.color[1];
					B = F_Delaunay.color[2];

					GLPoints1->Positions->Add(X, Y, Z);
					GLPoints1->Colors->AddPoint(R, G, B);
				}
				break;
			case 5:
				for (int i = 0; i < G_Delaunay.nodeCount; i++) {
					X = G_Delaunay.node[i][0]*MULTICOEF;
					Y = G_Delaunay.node[i][1]*MULTICOEF;
					Z = G_Delaunay.node[i][2]*MULTICOEF;

					R = G_Delaunay.color[0];
					G = G_Delaunay.color[1];
					B = G_Delaunay.color[2];

					GLPoints1->Positions->Add(X, Y, Z);
					GLPoints1->Colors->AddPoint(R, G, B);
				}
				break;
			case 6:
				for (int i = 0; i < K_Delaunay.nodeCount; i++) {
					X = K_Delaunay.node[i][0]*MULTICOEF;
					Y = K_Delaunay.node[i][1]*MULTICOEF;
					Z = K_Delaunay.node[i][2]*MULTICOEF;

					R = K_Delaunay.color[0];
					G = K_Delaunay.color[1];
					B = K_Delaunay.color[2];

					GLPoints1->Positions->Add(X, Y, Z);
					GLPoints1->Colors->AddPoint(R, G, B);
				}
				break;
			case 7:
				for (int i = 0; i < M_Delaunay.nodeCount; i++) {
					X = M_Delaunay.node[i][0]*MULTICOEF;
					Y = M_Delaunay.node[i][1]*MULTICOEF;
					Z = M_Delaunay.node[i][2]*MULTICOEF;

					R = M_Delaunay.color[0];
					G = M_Delaunay.color[1];
					B = M_Delaunay.color[2];

					GLPoints1->Positions->Add(X, Y, Z);
					GLPoints1->Colors->AddPoint(R, G, B);
				}
				break;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormScene::DrawDelaunay()
{
	// Delete all points from the scene
	//GLPoints1->Free();
	//GLPoints1 = (TGLPoints *)(GLDummyCube1->AddNewChild(__classid(TGLPoints)));
	// Delete all lines from the scene
	GLLines1->Free();
	GLLines1 = (TGLLines *)(GLDummyCube1->AddNewChild(__classid(TGLLines)));

	// Delete all faces from the scene
	//GLPolygon1->Free();
	//GLPolygon1 = (TGLPolygon *)(GLDummyCube1->AddNewChild(__classid(TGLPolygon)));

	float X1, Y1, Z1, X2, Y2, Z2, R, G, B;
	int NodeIndex1, NodeIndex2, NodeIndex3;
	switch (ClassGroup->ItemIndex)
	{
		case 0:
			R = All_Delaunay.color[0];
			G = All_Delaunay.color[1];
			B = All_Delaunay.color[2];
			for (int i = 0; i < All_Delaunay.edgeCount; i++) {
				NodeIndex1 = All_Delaunay.edge[i][0];
				NodeIndex2 = All_Delaunay.edge[i][1];
				X1 = All_Delaunay.node[NodeIndex1][0]*MULTICOEF;
				Y1 = All_Delaunay.node[NodeIndex1][1]*MULTICOEF;
				Z1 = All_Delaunay.node[NodeIndex1][2]*MULTICOEF;

				X2 = All_Delaunay.node[NodeIndex2][0]*MULTICOEF;
				Y2 = All_Delaunay.node[NodeIndex2][1]*MULTICOEF;
				Z2 = All_Delaunay.node[NodeIndex2][2]*MULTICOEF;

				GLLines1->Nodes->AddNode(X1, Y1, Z1);
				GLLines1->Nodes->AddNode(X2, Y2, Z2);
			}
			// Set edges color
			GLLines1->LineColor->SetColor(R, G, B, 1);
			// Delete axes of points
			GLLines1->NodesAspect = lnaInvisible;
			break;
		case 1:
			R = O_Delaunay.color[0];
			G = O_Delaunay.color[1];
			B = O_Delaunay.color[2];

			for (int i = 0; i < O_Delaunay.edgeCount; i++) {
				NodeIndex1 = O_Delaunay.edge[i][0];
				NodeIndex2 = O_Delaunay.edge[i][1];

				X1 = O_Delaunay.node[NodeIndex1][0]*MULTICOEF;
				Y1 = O_Delaunay.node[NodeIndex1][1]*MULTICOEF;
				Z1 = O_Delaunay.node[NodeIndex1][2]*MULTICOEF;

				X2 = O_Delaunay.node[NodeIndex2][0]*MULTICOEF;
				Y2 = O_Delaunay.node[NodeIndex2][1]*MULTICOEF;
				Z2 = O_Delaunay.node[NodeIndex2][2]*MULTICOEF;

				GLLines1->Nodes->AddNode(X1, Y1, Z1);
				GLLines1->Nodes->AddNode(X2, Y2, Z2);
			}
			// Set edges color
			GLLines1->LineColor->SetColor(R, G, B, 1);
			// Delete axes of points
			GLLines1->NodesAspect = lnaInvisible;
			break;
		case 2:
			R = B_Delaunay.color[0];
			G = B_Delaunay.color[1];
			B = B_Delaunay.color[2];

			for (int i = 0; i < B_Delaunay.edgeCount; i++) {
				NodeIndex1 = B_Delaunay.edge[i][0];
				NodeIndex2 = B_Delaunay.edge[i][1];

				X1 = B_Delaunay.node[NodeIndex1][0]*MULTICOEF;
				Y1 = B_Delaunay.node[NodeIndex1][1]*MULTICOEF;
				Z1 = B_Delaunay.node[NodeIndex1][2]*MULTICOEF;

				X2 = B_Delaunay.node[NodeIndex2][0]*MULTICOEF;
				Y2 = B_Delaunay.node[NodeIndex2][1]*MULTICOEF;
				Z2 = B_Delaunay.node[NodeIndex2][2]*MULTICOEF;

				GLLines1->Nodes->AddNode(X1, Y1, Z1);
				GLLines1->Nodes->AddNode(X2, Y2, Z2);
			}
			// Set edges color
			GLLines1->LineColor->SetColor(R, G, B, 1);
			// Delete axes of points
			GLLines1->NodesAspect = lnaInvisible;
			break;
		case 3:
			R = A_Delaunay.color[0];
			G = A_Delaunay.color[1];
			B = A_Delaunay.color[2];

			for (int i = 0; i < A_Delaunay.edgeCount; i++) {
				NodeIndex1 = A_Delaunay.edge[i][0];
				NodeIndex2 = A_Delaunay.edge[i][1];

				X1 = A_Delaunay.node[NodeIndex1][0]*MULTICOEF;
				Y1 = A_Delaunay.node[NodeIndex1][1]*MULTICOEF;
				Z1 = A_Delaunay.node[NodeIndex1][2]*MULTICOEF;

				X2 = A_Delaunay.node[NodeIndex2][0]*MULTICOEF;
				Y2 = A_Delaunay.node[NodeIndex2][1]*MULTICOEF;
				Z2 = A_Delaunay.node[NodeIndex2][2]*MULTICOEF;

				GLLines1->Nodes->AddNode(X1, Y1, Z1);
				GLLines1->Nodes->AddNode(X2, Y2, Z2);
			}
			// Set edges color
			GLLines1->LineColor->SetColor(R, G, B, 1);
			// Delete axes of points
			GLLines1->NodesAspect = lnaInvisible;
			break;
		case 4:
			R = F_Delaunay.color[0];
			G = F_Delaunay.color[1];
			B = F_Delaunay.color[2];

			for (int i = 0; i < F_Delaunay.edgeCount; i++) {
				NodeIndex1 = F_Delaunay.edge[i][0];
				NodeIndex2 = F_Delaunay.edge[i][1];

				X1 = F_Delaunay.node[NodeIndex1][0]*MULTICOEF;
				Y1 = F_Delaunay.node[NodeIndex1][1]*MULTICOEF;
				Z1 = F_Delaunay.node[NodeIndex1][2]*MULTICOEF;

				X2 = F_Delaunay.node[NodeIndex2][0]*MULTICOEF;
				Y2 = F_Delaunay.node[NodeIndex2][1]*MULTICOEF;
				Z2 = F_Delaunay.node[NodeIndex2][2]*MULTICOEF;

				GLLines1->Nodes->AddNode(X1, Y1, Z1);
				GLLines1->Nodes->AddNode(X2, Y2, Z2);
			}
			// Set edges color
			GLLines1->LineColor->SetColor(R, G, B, 1);
			// Delete axes of points
			GLLines1->NodesAspect = lnaInvisible;
			break;
		case 5:
			R = G_Delaunay.color[0];
			G = G_Delaunay.color[1];
			B = G_Delaunay.color[2];

			for (int i = 0; i < G_Delaunay.edgeCount; i++) {
				NodeIndex1 = G_Delaunay.edge[i][0];
				NodeIndex2 = G_Delaunay.edge[i][1];

				X1 = G_Delaunay.node[NodeIndex1][0]*MULTICOEF;
				Y1 = G_Delaunay.node[NodeIndex1][1]*MULTICOEF;
				Z1 = G_Delaunay.node[NodeIndex1][2]*MULTICOEF;

				X2 = G_Delaunay.node[NodeIndex2][0]*MULTICOEF;
				Y2 = G_Delaunay.node[NodeIndex2][1]*MULTICOEF;
				Z2 = G_Delaunay.node[NodeIndex2][2]*MULTICOEF;

				GLLines1->Nodes->AddNode(X1, Y1, Z1);
				GLLines1->Nodes->AddNode(X2, Y2, Z2);
			}
			// Set edges color
			GLLines1->LineColor->SetColor(R, G, B, 1);
			// Delete axes of points
			GLLines1->NodesAspect = lnaInvisible;
			break;
		case 6:
			R = K_Delaunay.color[0];
			G = K_Delaunay.color[1];
			B = K_Delaunay.color[2];

			for (int i = 0; i < K_Delaunay.edgeCount; i++) {
				NodeIndex1 = K_Delaunay.edge[i][0];
				NodeIndex2 = K_Delaunay.edge[i][1];

				X1 = K_Delaunay.node[NodeIndex1][0]*MULTICOEF;
				Y1 = K_Delaunay.node[NodeIndex1][1]*MULTICOEF;
				Z1 = K_Delaunay.node[NodeIndex1][2]*MULTICOEF;

				X2 = K_Delaunay.node[NodeIndex2][0]*MULTICOEF;
				Y2 = K_Delaunay.node[NodeIndex2][1]*MULTICOEF;
				Z2 = K_Delaunay.node[NodeIndex2][2]*MULTICOEF;

				GLLines1->Nodes->AddNode(X1, Y1, Z1);
				GLLines1->Nodes->AddNode(X2, Y2, Z2);
			}
			// Set edges color
			GLLines1->LineColor->SetColor(R, G, B, 1);
			// Delete axes of points
			GLLines1->NodesAspect = lnaInvisible;
			break;
		case 7:
			R = M_Delaunay.color[0];
			G = M_Delaunay.color[1];
			B = M_Delaunay.color[2];

			for (int i = 0; i < M_Delaunay.edgeCount; i++) {
				NodeIndex1 = M_Delaunay.edge[i][0];
				NodeIndex2 = M_Delaunay.edge[i][1];

				X1 = M_Delaunay.node[NodeIndex1][0]*MULTICOEF;
				Y1 = M_Delaunay.node[NodeIndex1][1]*MULTICOEF;
				Z1 = M_Delaunay.node[NodeIndex1][2]*MULTICOEF;

				X2 = M_Delaunay.node[NodeIndex2][0]*MULTICOEF;
				Y2 = M_Delaunay.node[NodeIndex2][1]*MULTICOEF;
				Z2 = M_Delaunay.node[NodeIndex2][2]*MULTICOEF;

				GLLines1->Nodes->AddNode(X1, Y1, Z1);
				GLLines1->Nodes->AddNode(X2, Y2, Z2);
			}
			// Set edges color
			GLLines1->LineColor->SetColor(R, G, B, 1);
			// Delete axes of stars
			GLLines1->NodesAspect = lnaInvisible;
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormScene::DrawVoronoi()
{
	// Delete all stars from the scene
	//GLPoints1->Free();
	//GLPoints1 = (TGLPoints *)(GLDummyCube1->AddNewChild(__classid(TGLPoints)));

	// Delete all lines from the scene
	GLLines2->Free();
	GLLines2 = (TGLLines *)(GLDummyCube1->AddNewChild(__classid(TGLLines)));

	// Temp vars for coordinates and colors values
	float X, Y, Z, R, G, B;
	float X1, Y1, Z1, X2, Y2, Z2;
	int NodeIndex1, NodeIndex2;

	switch (ClassGroup->ItemIndex)
	{
		case 0:
			R = All_Voronoi.color[0];
			G = All_Voronoi.color[1];
			B = All_Voronoi.color[2];

			for (int i = 0; i < All_Voronoi.edgeCount; i++) {
				NodeIndex1 = All_Voronoi.edge[i][0];
				NodeIndex2 = All_Voronoi.edge[i][1];

				X1 = All_Voronoi.node[NodeIndex1][0]*MULTICOEF;
				Y1 = All_Voronoi.node[NodeIndex1][1]*MULTICOEF;
				Z1 = All_Voronoi.node[NodeIndex1][2]*MULTICOEF;

				if (NodeIndex2 == -1) {
					X2 = All_Voronoi.edge[i][2];
					Y2 = All_Voronoi.edge[i][3];
					Z2 = All_Voronoi.edge[i][4];
				}
				else
				{
					X2 = All_Voronoi.node[NodeIndex2][0]*MULTICOEF;
					Y2 = All_Voronoi.node[NodeIndex2][1]*MULTICOEF;
					Z2 = All_Voronoi.node[NodeIndex2][2]*MULTICOEF;
				}

				GLLines2->Nodes->AddNode(X1, Y1, Z1);
				GLLines2->Nodes->AddNode(X2, Y2, Z2);
			}
			// Set edges color
			GLLines2->LineColor->SetColor(R*0.7, G*0.7, B*0.7, 1);
			// Delete axes of points
			GLLines2->NodesAspect = lnaInvisible;
			break;
		case 1:
			R = O_Voronoi.color[0];
			G = O_Voronoi.color[1];
			B = O_Voronoi.color[2];

			for (int i = 0; i < O_Voronoi.edgeCount; i++) {
				NodeIndex1 = O_Voronoi.edge[i][0];
				NodeIndex2 = O_Voronoi.edge[i][1];

				X1 = O_Voronoi.node[NodeIndex1][0]*MULTICOEF;
				Y1 = O_Voronoi.node[NodeIndex1][1]*MULTICOEF;
				Z1 = O_Voronoi.node[NodeIndex1][2]*MULTICOEF;

				if (NodeIndex2 == -1) {
					X2 = O_Voronoi.edge[i][2];
					Y2 = O_Voronoi.edge[i][3];
					Z2 = O_Voronoi.edge[i][4];
				}
				else
				{
					X2 = O_Voronoi.node[NodeIndex2][0]*MULTICOEF;
					Y2 = O_Voronoi.node[NodeIndex2][1]*MULTICOEF;
					Z2 = O_Voronoi.node[NodeIndex2][2]*MULTICOEF;
				}

				GLLines2->Nodes->AddNode(X1, Y1, Z1);
				GLLines2->Nodes->AddNode(X2, Y2, Z2);
			}
			// Set edges color
			GLLines2->LineColor->SetColor(R*0.7, G*0.7, B*0.7, 1);
			// Delete axes of points
			GLLines2->NodesAspect = lnaInvisible;
			break;
		case 2:
			R = B_Voronoi.color[0];
			G = B_Voronoi.color[1];
			B = B_Voronoi.color[2];

			for (int i = 0; i < B_Voronoi.edgeCount; i++) {
				NodeIndex1 = B_Voronoi.edge[i][0];
				NodeIndex2 = B_Voronoi.edge[i][1];

				X1 = B_Voronoi.node[NodeIndex1][0]*MULTICOEF;
				Y1 = B_Voronoi.node[NodeIndex1][1]*MULTICOEF;
				Z1 = B_Voronoi.node[NodeIndex1][2]*MULTICOEF;

				if (NodeIndex2 == -1) {
					X2 = B_Voronoi.edge[i][2];
					Y2 = B_Voronoi.edge[i][3];
					Z2 = B_Voronoi.edge[i][4];
				}
				else
				{
					X2 = B_Voronoi.node[NodeIndex2][0]*MULTICOEF;
					Y2 = B_Voronoi.node[NodeIndex2][1]*MULTICOEF;
					Z2 = B_Voronoi.node[NodeIndex2][2]*MULTICOEF;
				}

				GLLines2->Nodes->AddNode(X1, Y1, Z1);
				GLLines2->Nodes->AddNode(X2, Y2, Z2);
			}
			// Set edges color
			GLLines2->LineColor->SetColor(R*0.7, G*0.7, B*0.7, 1);
			// Delete axes of points
			GLLines2->NodesAspect = lnaInvisible;
			break;
		case 3:
			R = A_Voronoi.color[0];
			G = A_Voronoi.color[1];
			B = A_Voronoi.color[2];

			for (int i = 0; i < A_Voronoi.edgeCount; i++) {
				NodeIndex1 = A_Voronoi.edge[i][0];
				NodeIndex2 = A_Voronoi.edge[i][1];

				X1 = A_Voronoi.node[NodeIndex1][0]*MULTICOEF;
				Y1 = A_Voronoi.node[NodeIndex1][1]*MULTICOEF;
				Z1 = A_Voronoi.node[NodeIndex1][2]*MULTICOEF;

				if (NodeIndex2 == -1) {
					X2 = A_Voronoi.edge[i][2];
					Y2 = A_Voronoi.edge[i][3];
					Z2 = A_Voronoi.edge[i][4];
				}
				else
				{
					X2 = A_Voronoi.node[NodeIndex2][0]*MULTICOEF;
					Y2 = A_Voronoi.node[NodeIndex2][1]*MULTICOEF;
					Z2 = A_Voronoi.node[NodeIndex2][2]*MULTICOEF;
				}

				GLLines2->Nodes->AddNode(X1, Y1, Z1);
				GLLines2->Nodes->AddNode(X2, Y2, Z2);
			}
			// Set edges color
			GLLines2->LineColor->SetColor(R*0.7, G*0.7, B*0.7, 1);
			// Delete axes of points
			GLLines2->NodesAspect = lnaInvisible;
			break;
		case 4:
			R = F_Voronoi.color[0];
			G = F_Voronoi.color[1];
			B = F_Voronoi.color[2];

			for (int i = 0; i < F_Voronoi.edgeCount; i++) {
				NodeIndex1 = F_Voronoi.edge[i][0];
				NodeIndex2 = F_Voronoi.edge[i][1];

				X1 = G_Voronoi.node[NodeIndex1][0]*MULTICOEF;
				Y1 = G_Voronoi.node[NodeIndex1][1]*MULTICOEF;
				Z1 = G_Voronoi.node[NodeIndex1][2]*MULTICOEF;

				if (NodeIndex2 == -1) {
					X2 = F_Voronoi.edge[i][2];
					Y2 = F_Voronoi.edge[i][3];
					Z2 = F_Voronoi.edge[i][4];
				}
				else
				{
					X2 = F_Voronoi.node[NodeIndex2][0]*MULTICOEF;
					Y2 = F_Voronoi.node[NodeIndex2][1]*MULTICOEF;
					Z2 = F_Voronoi.node[NodeIndex2][2]*MULTICOEF;
				}

				GLLines2->Nodes->AddNode(X1, Y1, Z1);
				GLLines2->Nodes->AddNode(X2, Y2, Z2);
			}
			// Set edges color
			GLLines2->LineColor->SetColor(R*0.7, G*0.7, B*0.7, 1);
			// Delete axes of points
			GLLines2->NodesAspect = lnaInvisible;
			break;
		case 5:
			R = G_Voronoi.color[0];
			G = G_Voronoi.color[1];
			B = G_Voronoi.color[2];

			for (int i = 0; i < G_Voronoi.edgeCount; i++) {
				NodeIndex1 = G_Voronoi.edge[i][0];
				NodeIndex2 = G_Voronoi.edge[i][1];

				X1 = G_Voronoi.node[NodeIndex1][0]*MULTICOEF;
				Y1 = G_Voronoi.node[NodeIndex1][1]*MULTICOEF;
				Z1 = G_Voronoi.node[NodeIndex1][2]*MULTICOEF;

				if (NodeIndex2 == -1) {
					X2 = G_Voronoi.edge[i][2];
					Y2 = G_Voronoi.edge[i][3];
					Z2 = G_Voronoi.edge[i][4];
				}
				else
				{
					X2 = G_Voronoi.node[NodeIndex2][0]*MULTICOEF;
					Y2 = G_Voronoi.node[NodeIndex2][1]*MULTICOEF;
					Z2 = G_Voronoi.node[NodeIndex2][2]*MULTICOEF;
				}

				GLLines2->Nodes->AddNode(X1, Y1, Z1);
				GLLines2->Nodes->AddNode(X2, Y2, Z2);
			}
			// Set edges color
			GLLines2->LineColor->SetColor(R*0.7, G*0.7, B*0.7, 1);
			// Delete axes of points
			GLLines2->NodesAspect = lnaInvisible;
			break;
		case 6:
			R = K_Voronoi.color[0];
			G = K_Voronoi.color[1];
			B = K_Voronoi.color[2];

			for (int i = 0; i < K_Voronoi.edgeCount; i++) {
				NodeIndex1 = K_Voronoi.edge[i][0];
				NodeIndex2 = K_Voronoi.edge[i][1];

				X1 = K_Voronoi.node[NodeIndex1][0]*MULTICOEF;
				Y1 = K_Voronoi.node[NodeIndex1][1]*MULTICOEF;
				Z1 = K_Voronoi.node[NodeIndex1][2]*MULTICOEF;

				if (NodeIndex2 == -1) {
					X2 = K_Voronoi.edge[i][2];
					Y2 = K_Voronoi.edge[i][3];
					Z2 = K_Voronoi.edge[i][4];
				}
				else
				{
					X2 = K_Voronoi.node[NodeIndex2][0]*MULTICOEF;
					Y2 = K_Voronoi.node[NodeIndex2][1]*MULTICOEF;
					Z2 = K_Voronoi.node[NodeIndex2][2]*MULTICOEF;
				}

				GLLines2->Nodes->AddNode(X1, Y1, Z1);
				GLLines2->Nodes->AddNode(X2, Y2, Z2);
			}
			// Set edges color
			GLLines2->LineColor->SetColor(R*0.7, G*0.7, B*0.7, 1);
			// Delete axes of points
			GLLines2->NodesAspect = lnaInvisible;
			break;
		case 7:
			R = M_Voronoi.color[0];
			G = M_Voronoi.color[1];
			B = M_Voronoi.color[2];

			for (int i = 0; i < M_Voronoi.edgeCount; i++) {
				NodeIndex1 = M_Voronoi.edge[i][0];
				NodeIndex2 = M_Voronoi.edge[i][1];

				X1 = M_Voronoi.node[NodeIndex1][0]*MULTICOEF;
				Y1 = M_Voronoi.node[NodeIndex1][1]*MULTICOEF;
				Z1 = M_Voronoi.node[NodeIndex1][2]*MULTICOEF;

				if (NodeIndex2 == -1) {
					X2 = M_Voronoi.edge[i][2];
					Y2 = M_Voronoi.edge[i][3];
					Z2 = M_Voronoi.edge[i][4];
				}
				else
				{
					X2 = M_Voronoi.node[NodeIndex2][0]*MULTICOEF;
					Y2 = M_Voronoi.node[NodeIndex2][1]*MULTICOEF;
					Z2 = M_Voronoi.node[NodeIndex2][2]*MULTICOEF;
				}

				GLLines2->Nodes->AddNode(X1, Y1, Z1);
				GLLines2->Nodes->AddNode(X2, Y2, Z2);
			}
			// Set edges color
			GLLines2->LineColor->SetColor(R*0.7, G*0.7, B*0.7, 1);
			// Delete axes of points
			GLLines2->NodesAspect = lnaInvisible;
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormScene::DrawVoxel() {
	int size,cubeSize,count,half;
	float R,G,B;
	size = 300;
	cubeSize = 10;
	count = size/cubeSize*2;
	half = count/2;
	switch (ClassGroup->ItemIndex)
	{
		case 0:
			voxelData = Voxel(All_Delaunay,size,cubeSize);
			R = All_Delaunay.color[0];
			G = All_Delaunay.color[1];
			B = All_Delaunay.color[2];
			break;
		case 1:
			voxelData = Voxel(O_Delaunay,size,cubeSize);
			R = O_Delaunay.color[0];
			G = O_Delaunay.color[1];
			B = O_Delaunay.color[2];
			break;
		case 2:
			voxelData = Voxel(B_Delaunay,size,cubeSize);
			R = B_Delaunay.color[0];
			G = B_Delaunay.color[1];
			B = B_Delaunay.color[2];
			break;
		case 3:
			voxelData = Voxel(A_Delaunay,size,cubeSize);
			R = A_Delaunay.color[0];
			G = A_Delaunay.color[1];
			B = A_Delaunay.color[2];
			break;
		case 4:
			voxelData = Voxel(F_Delaunay,size,cubeSize);
			R = F_Delaunay.color[0];
			G = F_Delaunay.color[1];
			B = F_Delaunay.color[2];
			break;
		case 5:
			voxelData = Voxel(G_Delaunay,size,cubeSize);
			R = G_Delaunay.color[0];
			G = G_Delaunay.color[1];
			B = G_Delaunay.color[2];
			break;
		case 6:
			voxelData = Voxel(K_Delaunay,size,cubeSize);
			R = K_Delaunay.color[0];
			G = K_Delaunay.color[1];
			B = K_Delaunay.color[2];
			break;
		case 7:
			voxelData = Voxel(M_Delaunay,size,cubeSize);
            R = M_Delaunay.color[0];
			G = M_Delaunay.color[1];
			B = M_Delaunay.color[2];
			break;
	}
	//if (clbMethods->Checked[0]==false && clbMethods->Checked[0]==false && clbMethods->Checked[0]==false) {
		VoxelDummy->Free();
		VoxelDummy = (TGLDummyCube *)(GLDummyCube1->AddNewChild(__classid(TGLDummyCube)));
		int x,y,z;
		for (int i = 0; i < count; i++) {
			for (int j = 0; j < count; j++) {
				for (int t = 0; t < count; t++) {
					if (voxelData[i][j][t] > 0) {
						x = i-half;
						y = j-half;
						z = t-half;
						TGLCube *cube;
						cube = (TGLCube *)(VoxelDummy->AddNewChild(__classid(TGLCube)));
						cube->Material->FrontProperties->Emission->Red = R;
						cube->Material->FrontProperties->Emission->Green = G;
						cube->Material->FrontProperties->Emission->Blue = B;
						//cube->Material->PolygonMode = pmLines;
						cube->Material->BlendingMode = bmCustom;
						//cube->Material->FrontProperties->Diffuse->Alpha = 0.85;
						//cube->Material->BackProperties->Diffuse->Alpha = 0.85;
						/*cube->CubeDepth = cubeSize*2;
						cube->CubeHeight = cubeSize*2;
						cube->CubeWidth = cubeSize*2;*/
						cube->Scale->Scale(cubeSize);
						cube->Position->X = x*cubeSize + cubeSize/2;
						cube->Position->Y = y*cubeSize + cubeSize/2;
						cube->Position->Z = z*cubeSize + cubeSize/2;
					}
				}
			}
		}
}

void __fastcall TFormScene::InitDraw()
{
	// Draw 3D model based on selected mode
	if (clbMethods->Checked[0]) {
		DrawPoints();
	}
	if (clbMethods->Checked[1]) {
		DrawDelaunay();
	}
	if (clbMethods->Checked[2]) {
		DrawVoronoi();
	}
	if (clbMethods->Checked[3]) {
		DrawVoxel();
	}
	/*
	if (Points1->Checked == true) {
		DrawPoints();
	}
	else if (Delaunay1->Checked == true) {
		DrawDelaunay();
	}
	else if (Voronoi1->Checked == true) {
		DrawVoronoi();
	}  */
}
//---------------------------------------------------------------------------
__fastcall TFormScene::TFormScene(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormScene::Timer1Timer(TObject *Sender)
{
	FormScene->StatusBar1->Panels->Items[0]->Text = \
	Format("Total stars: %d", ARRAYOFCONST((GLPoints1->Positions->Count)));
	FormScene->StatusBar1->Panels->Items[1]->Text = \
	Format("FPS: %.2f", ARRAYOFCONST((GLSceneViewer1->FramesPerSecond())));
	GLSceneViewer1->ResetPerformanceMonitor();
}
//---------------------------------------------------------------------------
void __fastcall TFormScene::GLCadencer1Progress(TObject *Sender, const double deltaTime,
		  const double newTime)
{
	GLSceneViewer1->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFormScene::Start1Click(TObject *Sender)
{
	// Auxiliary array with files' names
	String filenames[14];
	filenames[0] = "O_Delaunay.sqlite"; filenames[1] = "O_Voronoi.sqlite";
	filenames[2] = "A_Delaunay.sqlite"; filenames[3] = "A_Voronoi.sqlite";
	filenames[4] = "B_Delaunay.sqlite"; filenames[5] = "B_Voronoi.sqlite";
	filenames[6] = "F_Delaunay.sqlite"; filenames[7] = "F_Voronoi.sqlite";
	filenames[8] = "G_Delaunay.sqlite"; filenames[9] = "G_Voronoi.sqlite";
	filenames[10] = "K_Delaunay.sqlite"; filenames[11] = "K_Voronoi.sqlite";
	filenames[12] = "M_Delaunay.sqlite"; filenames[13] = "M_Voronoi.sqlite";

	// Checking if all files exist
	ifstream ifile;
	for (int i = 0; i < 14; i++) {
		ifile.open((datapath + filenames[i]).c_str());
		if(!ifile) {
			ShowMessage(AnsiString("File \'") + filenames[i].c_str() \
			+ AnsiString("\' not found!"));
		}
		ifile.close();
	}


	// Init all Delaunay data
	All_Delaunay = InitDelaunay("All_Del.sqlite", white);
	A_Delaunay = InitDelaunay("A_Del.sqlite", white);
	B_Delaunay = InitDelaunay("B_Del.sqlite", skyblue);
	F_Delaunay = InitDelaunay("F_Del.sqlite", lightyellow);
	G_Delaunay = InitDelaunay("G_Del.sqlite", yellow);
	K_Delaunay = InitDelaunay("K_Del.sqlite", orange);
	M_Delaunay = InitDelaunay("M_Del.sqlite", red);
	O_Delaunay = InitDelaunay("O_Del.sqlite", lightblue);

	Points = new DelaunayBase[7];
	Points[0] = A_Delaunay;
	Points[1] = B_Delaunay;
	Points[2] = F_Delaunay;
	Points[3] = G_Delaunay;
	Points[4] = K_Delaunay;
	Points[5] = M_Delaunay;
	Points[6] = O_Delaunay;
	InitVoxel(Points, 300, 10);

	// Init all Voronoi data
	All_Voronoi = InitVoronoi("All_Vor.sqlite", white);
	A_Voronoi = InitVoronoi("A_Vor.sqlite", white);
	B_Voronoi = InitVoronoi("B_Vor.sqlite", skyblue);
	F_Voronoi = InitVoronoi("F_Vor.sqlite", lightyellow);
	G_Voronoi = InitVoronoi("G_Vor.sqlite", yellow);
	K_Voronoi = InitVoronoi("K_Vor.sqlite", orange);
	M_Voronoi = InitVoronoi("M_Vor.sqlite", red);
	O_Voronoi = InitVoronoi("O_Vor.sqlite", lightblue);

	Start1->Enabled = False;
	Data1->Enabled = True;

	// Visualization
	InitDraw();
}
//---------------------------------------------------------------------------
void __fastcall TFormScene::clbSpectrasClickCheck(TObject *Sender)
{
	// Init draw method on each checkbox selecting
	InitDraw();
}
//---------------------------------------------------------------------------
void __fastcall TFormScene::Data1Click(TObject *Sender)
{
	FormTable->Show();
}
//---------------------------------------------------------------------------


void __fastcall TFormScene::ClassGroupClick(TObject *Sender)
{
	 InitDraw();
}
//---------------------------------------------------------------------------


void __fastcall TFormScene::Panel2Click(TObject *Sender)
{
	ClassGroup->ItemIndex =((TPanel *)(Sender))->Tag;;
}
//---------------------------------------------------------------------------

void __fastcall TFormScene::clbMethodsClickCheck(TObject *Sender)
{
	GLPoints1->Free();
	GLPoints1 = (TGLPoints *)(GLDummyCube1->AddNewChild(__classid(TGLPoints)));
	GLLines1->Free();
	GLLines1 = (TGLLines *)(GLDummyCube1->AddNewChild(__classid(TGLLines)));
	GLLines2->Free();
	GLLines2 = (TGLLines *)(GLDummyCube1->AddNewChild(__classid(TGLLines)));
	VoxelDummy->Free();
	VoxelDummy = (TGLDummyCube *)(GLDummyCube1->AddNewChild(__classid(TGLDummyCube)));
	InitDraw();
}
//---------------------------------------------------------------------------

