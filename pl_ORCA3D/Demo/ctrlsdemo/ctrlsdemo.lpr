program ctrlsdemo;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, pl_orca,
  ctrlsform in 'ctrlsform.pas' ;

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmCtrlsDemo, frmCtrlsDemo);
  Application.Run;
end.
