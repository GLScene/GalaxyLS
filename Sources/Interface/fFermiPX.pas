unit fFermiPX;

interface

uses
  Winapi.Windows,
  Winapi.Messages,
  System.SysUtils,
  System.Variants,
  System.Classes,
  Vcl.Graphics,
  Vcl.Controls,
  Vcl.Forms,
  Vcl.Dialogs,
  FireDAC.Stan.Intf,
  FireDAC.Stan.Option,
  FireDAC.Stan.Error,
  FireDAC.UI.Intf,
  FireDAC.Phys.Intf,
  FireDAC.Stan.Def,
  FireDAC.Stan.Pool,
  FireDAC.Stan.Async,
  FireDAC.Phys,
  FireDAC.Phys.SQLite,
  FireDAC.Phys.SQLiteDef,
  FireDAC.Stan.ExprFuncs,
  FireDAC.VCLUI.Wait,
  FireDAC.Stan.Param,
  FireDAC.DatS,
  FireDAC.DApt.Intf,
  FireDAC.DApt,
  Data.DB,
  FireDAC.Comp.DataSet,
  FireDAC.Comp.Client,
  Vcl.ExtDlgs,
  Vcl.Menus,
  Vcl.ExtCtrls,
  Vcl.StdCtrls,
  Vcl.CheckLst,
  Vcl.ComCtrls,

  GLS.Cadencer,
  GLS.Scene,
  GLS.VectorFileObjects,
  GLS.GeomObjects,
  GLS.Objects,
  GLS.Coordinates,
  GLS.BaseClasses,
  GLS.SceneViewer,
  GLS.VectorTypes,
  GLS.Color, GLS.Graph, GLS.SimpleNavigation;


type

//---------------------------------------------------------------------------

DelaunayBase = record
	color: TGLFloat; //float *color;
	nodeCount: Integer;
	node: PDouble;   //double **node;
	edgeCount: Integer;
	edge: PInteger;  // int **edge;
	//int faceCount;
	//int **face;
	//int tetraCount;
  //int **tetra;
end;

VoronoiBase = record
	color: TGLFloat; //float *color;
	nodeCount: Integer;
	node: PDouble;   //double **node;
	edgeCount: Integer;
	edge: PInteger;  // int **edge;
	//int faceCount;
	//int **face;
end;

type
  TFormScene = class(TForm)
    StatusBar1: TStatusBar;
    GLSceneViewer1: TGLSceneViewer;
    PanelRight: TPanel;
    CheckListBox1: TCheckListBox;
    GLScene1: TGLScene;
    GLCamera1: TGLCamera;
    GLDummyCube1: TGLDummyCube;
    GLLines1: TGLLines;
    GLPoints1: TGLPoints;
    GLPolygon1: TGLPolygon;
    GLTetrahedron1: TGLTetrahedron;
    GLFreeForm1: TGLFreeForm;
    GLLightSource1: TGLLightSource;
    GLCadencer1: TGLCadencer;
    Timer1: TTimer;
    MainMenu1: TMainMenu;
    mnFile: TMenuItem;
    Mode1: TMenuItem;
    Points1: TMenuItem;
    Delaunay1: TMenuItem;
    Voronoi1: TMenuItem;
    N1: TMenuItem;
    Exit1: TMenuItem;
    Data1: TMenuItem;
    OpenTextFileDialog1: TOpenTextFileDialog;
    FDConnection1: TFDConnection;
    FDQuery1: TFDQuery;
    FDPhysSQLiteDriverLink1: TFDPhysSQLiteDriverLink;
    GLSimpleNavigation1: TGLSimpleNavigation;
    GLXYZGrid1: TGLXYZGrid;
    procedure FormCreate(Sender: TObject);
    procedure GLSceneViewer1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  FormScene: TFormScene;
  mx, my : Integer; // vars for saving position

  // All Delaunay records
  O_Delaunay,
  B_Delaunay,
  A_Delaunay,
  F_Delaunay,
  G_Delaunay,
  K_Delaunay,
  M_Delaunay: DelaunayBase;

  // All Voronoi records
  O_Voronoi,
  B_Voronoi,
  A_Voronoi,
  F_Voronoi,
  G_Voronoi,
  K_Voronoi,
  M_Voronoi: VoronoiBase;

const
// Colors for each star class
///lightblue: TGLColor = [0, 0.8, 1];  // float lightblue[3] = {0, 0.8, 1};

(*
float skyblue[3] = {0.803, 1, 1};
float white[3] = {1, 1, 1};
float lightyellow[3] = {0.996, 1, 0.6};
float yellow[3] = {1, 1, 0.003};
float orange[3] = {1, 0.4, 0};
float red[3] = {0.992, 0, 0.003};
*)
  prefix: String = '.\..\..\DATA\';

implementation

{$R *.dfm}

procedure TFormScene.FormCreate(Sender: TObject);
begin
	Points1.Checked := true;
 //	Form1.Caption := 'HYG 3D | Points mode';

	Mode1.Enabled := false;
	Data1.Enabled := false;

	CheckListBox1.Checked[0] := true;

end;

procedure TFormScene.GLSceneViewer1MouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
	 mx := X; my := Y;
end;

end.
