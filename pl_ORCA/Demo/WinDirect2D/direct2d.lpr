program direct2d;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,
  direct2dfrm in 'direct2dfrm.pas' {frmD2D};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmD2D, frmD2D);
  Application.Run;
end.
