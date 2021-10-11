unit fOptionsD;

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
  Vcl.ComCtrls;

type
  TfrmOptions = class(TForm)
    PageControl: TPageControl;
    tsInterface: TTabSheet;
    tsStars: TTabSheet;
    tsScenario: TTabSheet;
    tvOptions: TTreeView;
    tsGalablocks: TTabSheet;
    tsCoordinates: TTabSheet;
  private
  public
  end;

var
  frmOptions: TfrmOptions;

implementation

{$R *.dfm}

end.
