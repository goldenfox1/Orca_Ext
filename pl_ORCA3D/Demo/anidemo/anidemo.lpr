program anidemo;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, pl_orca,
  anidemofrm;

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmAniDemo, frmAniDemo);
  Application.Run;
end.
