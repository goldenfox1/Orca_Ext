program imdemo;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,
  imdemofrm in 'imdemofrm.pas' {frmIM};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmIM, frmIM);
  Application.Run;
end.
