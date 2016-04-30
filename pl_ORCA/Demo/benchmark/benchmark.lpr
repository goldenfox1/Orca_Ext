program benchmark;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,
  benchmarkfrm;

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmBenchmark, frmBenchmark);
  Application.Run;
end.
