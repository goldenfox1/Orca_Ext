program pathdemo;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, pl_orca,
  pathdemofrm in 'pathdemofrm.pas' {frmPathDemo};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmPathDemo, frmPathDemo);
  Application.Run;
end.
