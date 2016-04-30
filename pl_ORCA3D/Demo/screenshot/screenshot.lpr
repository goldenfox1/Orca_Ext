program screenshot;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, pl_orca,
  screenshotfrm in 'screenshotfrm.pas' {frmScreenshot};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmScreenshot, frmScreenshot);
  Application.Run;
end.
