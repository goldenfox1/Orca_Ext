program textbox3d;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, pl_orca,
  textbox3dfrm in 'textbox3dfrm.pas' {frmTextBox3D};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmTextBox3D, frmTextBox3D);
  Application.Run;
end.
