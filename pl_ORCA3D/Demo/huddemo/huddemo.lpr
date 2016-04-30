program huddemo;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, pl_orca,
  huddemofrm in 'huddemofrm.pas' {frnHudDemo};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrnHudDemo, frnHudDemo);
  Application.Run;
end.
