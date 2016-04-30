program spritedemo;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, pl_orca,
  starsfrm in 'starsfrm.pas' {frmStars};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmStars, frmStars);
  Application.Run;
end.
