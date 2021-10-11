program GalaxyLSd;

uses
  Vcl.Forms,
  fSpaceD in 'Interface\fSpaceD.pas' {FormScene},
  fTableD in 'Interface\fTableD.pas' {Form1},
  uRandomAM in 'Code\uRandomAM.pas',
  fOptionsD in 'Interface\fOptionsD.pas' {frmOptions},
  fAboutD in 'Interface\fAboutD.pas' {frmAbout};

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TFormScene, FormScene);
  Application.Run;
end.
