program backdemo;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, pl_orca,
  backdemofrm;

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrnBackDemo, frnBackDemo);
  Application.Run;
end.
