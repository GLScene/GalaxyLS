program FermiPX;

uses
  Vcl.Forms,
  fFermiPX in 'Interface\fFermiPX.pas' {FormScene},
  fFermiPXTable in 'Interface\fFermiPXTable.pas' {Form1},
  uRandomAM in 'Code\uRandomAM.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TFormScene, FormScene);
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
