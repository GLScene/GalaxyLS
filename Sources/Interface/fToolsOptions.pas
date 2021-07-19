unit fToolsOptions;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.ComCtrls;

type
  TForm2 = class(TForm)
    PageControl1: TPageControl;
    tsInterface: TTabSheet;
    tsStars: TTabSheet;
    tsScenario: TTabSheet;
    tvOptions: TTreeView;
    tsGalablocks: TTabSheet;
    tsCoordinates: TTabSheet;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form2: TForm2;

implementation

{$R *.dfm}

end.
