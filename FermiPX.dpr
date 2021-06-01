program FermiPX;

uses
  Vcl.Forms,
  fFermiPX in 'Sources\Interface\fFermiPX.pas' {FormScene},
  fFermiPXTable in 'Sources\Interface\fFermiPXTable.pas' {Form1},
  uRandomAM in 'Sources\Code\uRandomAM.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TFormScene, FormScene);
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
