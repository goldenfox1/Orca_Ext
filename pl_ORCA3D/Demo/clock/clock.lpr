program clock;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,
  pl_orca,
  clockfrm in 'clockfrm.pas' ;

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(Tfrmd3Clock, frmd3Clock);
  Application.Run;
end.
