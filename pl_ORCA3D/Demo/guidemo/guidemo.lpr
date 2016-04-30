program guidemo;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, pl_orca,
  guiform in 'guiform.pas' {frmGuiDemo};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmGuiDemo, frmGuiDemo);
  Application.Run;
end.
