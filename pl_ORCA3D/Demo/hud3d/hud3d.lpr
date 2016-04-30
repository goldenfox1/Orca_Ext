program hud3d;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, pl_orca,
  hud3dfrm in 'hud3dfrm.pas' {frmHud3D};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmHud3D, frmHud3D);
  Application.Run;
end.
