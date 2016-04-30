program multilang;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,
  multilangfrm in 'multilangfrm.pas' {frmMultilang};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmMultilang, frmMultilang);
  Application.Run;
end.
