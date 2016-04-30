program anitest;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,
  anitestfrm;

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm19, Form19);
  Application.Run;
end.
