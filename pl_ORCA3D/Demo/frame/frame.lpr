program frame;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, pl_orca,
  mainfrm in 'mainfrm.pas' {frmMain},
  bindingfrm in 'bindingfrm.pas' {Form6};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmMain, frmMain);
  Application.CreateForm(TForm6, Form6);
  Application.Run;
end.
