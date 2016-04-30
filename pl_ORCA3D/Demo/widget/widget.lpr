program widget;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, pl_orca,
  widgetfrm in 'widgetfrm.pas' {frmWidget};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmWidget, frmWidget);
  Application.Run;
end.
